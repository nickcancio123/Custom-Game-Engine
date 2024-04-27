#include "pch.h"
#include "Serialization.h"
#include <vector>
#include <codecvt>
#include "CppUnitTest.h"
#include "FieaGameEngine.Test.cpp"

#include "GameObject.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "TypeRegistry.h"
#include "FactoryManager.h"
#include "ParseCoordinator.h"
#include "ScopeWrapper.h"
#include "Monster.h"


using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::Test;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;


namespace Fiea::GameEngine::Test
{

	TEST_CLASS(GameObjectTest)
	{
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			TypeRegistry::RegisterSignatures(GameObject::TypeIdClass(), GameObject::Signatures());
			TypeRegistry::RegisterSignatures(Action::TypeIdClass(), Action::Signatures());
			TypeRegistry::RegisterSignatures(ActionList::TypeIdClass(), ActionList::Signatures(), Action::TypeIdClass());

			FactoryManager<Scope>::AddFactory("GameObject", *(new GameObjectFactory()));
			FactoryManager<Scope>::AddFactory("ActionList", *(new ActionListFactory()));
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


		TEST_METHOD(GameObjectJsonParsing)
		{
			std::string Root = R"(
			{
				"Goober": 
				{ 
					"type" : "GameObject", 
					"Pos" : "vec4| 1, 1, 1, 1 |",
					"Rot" : "vec4| 2, 2, 2, 2 |",
					"Scale" : "vec4| 3, 3, 3, 3 |"
				}
			}
			)"s;

			ScopeWrapper ScopeWrap;
			ParseCoordinator PC(ScopeWrap);
			ScopeWrap.AddScopeParseHandlers(PC);
			PC.DeserializeObject(Root);

			Datum* GooberDatum = ScopeWrap.GetRoot().Find("Goober");
			Assert::IsNotNull(GooberDatum);

			Scope* GooberScope = GooberDatum->GetScope();
			Assert::IsNotNull(GooberScope);

			Assert::IsTrue(GooberScope->Is(GameObject::TypeIdClass()));

			GameObject* Goober = static_cast<GameObject*>(GooberScope);
			Assert::IsNotNull(Goober);

			Assert::AreEqual(glm::vec4(1), Goober->GetTransform()._Pos);
			Assert::AreEqual(glm::vec4(2), Goober->GetTransform()._Rot);
			Assert::AreEqual(glm::vec4(3), Goober->GetTransform()._Scale);
		}

		TEST_METHOD(NestedGameObjects)
		{
			std::string Root = R"(
			{
				"Goober": 
				{
					"type" : "GameObject", 
					"Children" : [
						{
							"Name" : "Bob"
						},
						{
							"Name" : "Sally"
						}
					]
				}
			}
			)"s;

			ScopeWrapper ScopeWrap;
			ParseCoordinator PC(ScopeWrap);
			ScopeWrap.AddScopeParseHandlers(PC);
			PC.DeserializeObject(Root);

			// Get Goober children
			Datum* GooberDatum = ScopeWrap.GetRoot().Find("Goober");
			Assert::IsNotNull(GooberDatum);
			Scope* GooberScope = GooberDatum->GetScope();
			Assert::IsNotNull(GooberScope);
			Assert::IsTrue(GooberScope->Is(GameObject::TypeIdClass()));
			GameObject* Goober = static_cast<GameObject*>(GooberScope);
			Assert::IsNotNull(Goober);
			Datum* ChildrenDatum = Goober->Find("Children");
			Assert::IsNotNull(ChildrenDatum);

			// Bob
			Scope* BobScope = ChildrenDatum->GetScope(0);
			Assert::IsNotNull(BobScope);
			GameObject* Bob = static_cast<GameObject*>(BobScope);
			Assert::IsNotNull(Bob);
			Datum* ValueDatum = Bob->Find("Name");
			Assert::IsNotNull(ValueDatum);
			Assert::AreEqual(std::string("Bob"), ValueDatum->GetString());

			// Sally
			Scope* SallyScope = ChildrenDatum->GetScope(1);
			Assert::IsNotNull(SallyScope);
			GameObject* Sally = static_cast<GameObject*>(SallyScope);
			Assert::IsNotNull(Sally);
			ValueDatum = Sally->Find("Name");
			Assert::IsNotNull(ValueDatum);
			Assert::AreEqual(std::string("Sally"), ValueDatum->GetString());


			// TODO - Scope parser doesn't call AddChild
			// 
			//// Test quick lookup
			//GameObject* BobLookup = Goober->GetChild("Bob");
			//Assert::IsNotNull(BobLookup);
			//Assert::AreSame(*BobLookup, *Bob);
		}

	private:
		inline static _CrtMemState _startMemState;

	};

}