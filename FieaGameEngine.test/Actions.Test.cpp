#include "pch.h"
#include "Serialization.h"
#include <vector>
#include <codecvt>
#include "CppUnitTest.h"
#include "FieaGameEngine.Test.cpp"

#include "GameObject.h"
#include "TypeRegistry.h"
#include "FactoryManager.h"
#include "Factory.h"
#include "Monster.h"
#include "GameClock.h"

#include "ActionIncrement.h"
#include "ActionWhile.h"


using namespace Fiea::GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;


namespace Fiea::GameEngine::Test
{

	TEST_CLASS(ActionsTest)
	{

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			TypeRegistry::RegisterSignatures(GameObject::TypeIdClass(), GameObject::Signatures());
			TypeRegistry::RegisterSignatures(Monster::TypeIdClass(), Monster::Signatures(), GameObject::TypeIdClass());
			TypeRegistry::RegisterSignatures(Action::TypeIdClass(), Action::Signatures());
			TypeRegistry::RegisterSignatures(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures(), Action::TypeIdClass());
			TypeRegistry::RegisterSignatures(ActionList::TypeIdClass(), ActionList::Signatures(), Action::TypeIdClass());
			TypeRegistry::RegisterSignatures(ActionWhile::TypeIdClass(), ActionWhile::Signatures(), ActionList::TypeIdClass());

			FactoryManager<Scope>::AddFactory("GameObject", *(new GameObjectFactory()));
			FactoryManager<Scope>::AddFactory("Monster", *(new MonsterFactory()));
			FactoryManager<Scope>::AddFactory("ActionList", *(new ActionListFactory()));
			FactoryManager<Scope>::AddFactory("ActionIncrement", *(new ActionIncrementFactory()));
			FactoryManager<Scope>::AddFactory("ActionWhile", *(new ActionWhileFactory()));
		}

		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			FactoryManager<Scope>::Clear();
		}


		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState,
				&endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}


		TEST_METHOD(ActionIncrementUpdateTest)
		{
			GameClock Clock;

			Scope* MonsterScope = FactoryManager<Scope>::Create("Monster");
			Assert::IsNotNull(MonsterScope);
			Monster* Monst = MonsterScope->As<Monster>();
			Assert::IsNotNull(Monst);

			Action* Act = Monst->CreateAction("ActionIncrement", "HealthIncrement");
			Assert::IsNotNull(Act);
			ActionIncrement* IncrementAct = Act->As<ActionIncrement>();
			Assert::IsNotNull(IncrementAct);

			Datum* HealAmountDatum = Monst->Find("HealAmount");
			Assert::IsNotNull(HealAmountDatum);
			IncrementAct->SetValues("Health", *HealAmountDatum);

			Datum* HealthDatum = Monst->Find("Health");
			Assert::IsNotNull(HealthDatum);
			Assert::AreEqual(0.0f, HealthDatum->GetFloat());

			IncrementAct->Update_Outer(Clock.Current());

			Assert::AreEqual(5.0f, HealAmountDatum->GetFloat());

			delete Monst;
		}

		TEST_METHOD(SingleUpdate)
		{
			GameClock Clock;

			Scope* MonsterScope = FactoryManager<Scope>::Create("Monster");
			Assert::IsNotNull(MonsterScope);
			Monster* Monst = MonsterScope->As<Monster>();
			Assert::IsNotNull(Monst);

			Action* Act = Monst->CreateAction("ActionIncrement", "HealthIncrementAction");
			Assert::IsNotNull(Act);
			ActionIncrement* IncrementAct = Act->As<ActionIncrement>();
			Assert::IsNotNull(IncrementAct);

			IncrementAct->SetSingleUpdate(true);

			const ActionList* Actions = Monst->GetActions();
			Assert::IsNotNull(Actions);
			Assert::AreEqual((unsigned int) 1, Actions->Size());

			Datum* HealAmountDatum = Monst->Find("HealAmount");
			Assert::IsNotNull(HealAmountDatum);
			IncrementAct->SetValues("Health", *HealAmountDatum);

			Datum* HealthDatum = Monst->Find("Health");
			Assert::IsNotNull(HealthDatum);
			Assert::AreEqual(0.0f, HealthDatum->GetFloat());

			Monst->Update(Clock.Current());

			Assert::AreEqual(5.0f, HealAmountDatum->GetFloat());

			// Check that action was removed after one use
			Assert::AreEqual((unsigned int)0, Actions->Size());

			delete Monst;
		}

		TEST_METHOD(ActionWhileTest)
		{
			GameClock Clock;

			// Create monster
			Scope* MonsterScope = FactoryManager<Scope>::Create("Monster");
			Assert::IsNotNull(MonsterScope);
			Monster* Monst = MonsterScope->As<Monster>();
			Assert::IsNotNull(Monst);

			// Init monster health to 100
			Datum* HealthDatum = Monst->Find("Health");
			Assert::IsNotNull(HealthDatum);
			HealthDatum->Set(100.0f);

			// Create action while - tick damage action
			Action* Act = Monst->CreateAction("ActionWhile", "TickDamageAction");
			Assert::IsNotNull(Act);
			ActionWhile* TickDamageAction = Act->As<ActionWhile>();
			Assert::IsNotNull(TickDamageAction);

			// Create preamble
			Scope* PreambleScope = FactoryManager<Scope>::Create("ActionIncrement");
			Assert::IsNotNull(PreambleScope);
			ActionIncrement* Preamble = PreambleScope->As<ActionIncrement>();
			Assert::IsNotNull(Preamble);

			Datum& TickCountDatum = TickDamageAction->Append("TotalTickCount");
			TickCountDatum.PushBack(10);

			Preamble->SetSubject(TickDamageAction);
			Preamble->SetValues("Condition", TickCountDatum);
			TickDamageAction->SetPreamble(*Preamble);

			// Populate action list for loop body
			Scope* DamageActionScope = FactoryManager<Scope>::Create("ActionIncrement");
			Assert::IsNotNull(DamageActionScope);
			ActionIncrement* DamageAction = DamageActionScope->As<ActionIncrement>();
			Assert::IsNotNull(DamageAction);

			Datum& DamagePerTickDatum = TickDamageAction->Append("DamagePerTick");
			DamagePerTickDatum.PushBack(-5.0f);

			DamageAction->SetSubject(Monst);
			DamageAction->SetValues("Health", DamagePerTickDatum);
			TickDamageAction->AddAction(*DamageAction);

			// Create action while increment
			Scope* IncrementActionScope = FactoryManager<Scope>::Create("ActionIncrement");
			Assert::IsNotNull(IncrementActionScope);
			ActionIncrement* IncrementAction = IncrementActionScope->As<ActionIncrement>();
			Assert::IsNotNull(IncrementAction);

			Datum& IncrementDatum = TickDamageAction->Append("IncrementAmount");
			IncrementDatum.PushBack(-1);

			IncrementAction->SetSubject(TickDamageAction);
			IncrementAction->SetValues("Condition", IncrementDatum);
			TickDamageAction->SetIncrement(*IncrementAction);


			// Test
			TickDamageAction->Update_Outer(Clock.Current());

			Assert::AreEqual(50.0f, HealthDatum->GetFloat());
			
			Datum* ConditionDatum = TickDamageAction->Find("Condition");
			Assert::IsNotNull(ConditionDatum);
			Assert::AreEqual(0, ConditionDatum->GetInt());

			delete Monst;
		}

	private:
		inline static _CrtMemState _startMemState;
	};

}