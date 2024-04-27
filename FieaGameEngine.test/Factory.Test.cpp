#include "pch.h"
#include "CppUnitTest.h"
#include "Serialization.h"
#include <vector>
#include <codecvt>
#include "FieaGameEngine.Test.cpp"

#include "FactoryManager.h"
#include "ConcreteFactories.h"
#include "Scope.h"
#include "ScopeWrapper.h"
#include "TypeRegistry.h"
#include "ParseCoordinator.h"

#include "AttributedChild.h"
#include "AttributedBar.h"


namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Factory<Scope>>(const Factory<Scope>& T)
	{
		return std::to_wstring(0);
	}

	template<>
	std::wstring ToString<AttributedChildFactory>(const AttributedChildFactory& T)
	{
		return std::to_wstring(0);
	}

	template<>
	std::wstring ToString<AttributedBarFactory>(const AttributedBarFactory& T)
	{
		return std::to_wstring(0);
	}
}


using namespace Fiea::GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;


namespace Fiea::GameEngine::Test
{

	TEST_CLASS(FactoryTests)
	{

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			TypeRegistry::RegisterSignatures(AttributedChild::TypeIdClass(), AttributedChild::Signatures());
			TypeRegistry::RegisterSignatures(AttributedBar::TypeIdClass(), AttributedBar::Signatures());
		}

		TEST_METHOD_INITIALIZE(Initialize)
		{
			FactoryManager<Scope>::Clear();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			FactoryManager<Scope>::Clear();

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


		TEST_METHOD(FindFactoryAndClear)
		{
			Factory<Scope>* ACFactory = new AttributedChildFactory();
			Factory<Scope>* ABFactory = new AttributedBarFactory();

			FactoryManager<Scope>::AddFactory("AttributedChild", *ACFactory);
			FactoryManager<Scope>::AddFactory("AttributedBar", *ABFactory);
			
			Assert::AreSame(*ACFactory, *FactoryManager<Scope>::FindFactory("AttributedChild"));
			Assert::AreSame(*ABFactory, *FactoryManager<Scope>::FindFactory("AttributedBar"));
			Assert::IsNull(FactoryManager<Scope>::FindFactory("NonexistentClassName"));

			FactoryManager<Scope>::Clear();
			Assert::IsNull(FactoryManager<Scope>::FindFactory("AttributedChild"));
			Assert::IsNull(FactoryManager<Scope>::FindFactory("AttributedBar"));
			Assert::IsNull(FactoryManager<Scope>::FindFactory("NonexistentClassName"));
		}

		TEST_METHOD(CreateUsingFactory)
		{
			FactoryManager<Scope>::AddFactory("AttributedChild", *(new AttributedChildFactory()));
			FactoryManager<Scope>::AddFactory("AttributedBar", *(new AttributedBarFactory()));

			Scope* ACScope = FactoryManager<Scope>::Create("AttributedChild");
			Scope* ABScope = FactoryManager<Scope>::Create("AttributedBar");
			Scope* InvalidScope = FactoryManager<Scope>::Create("NonexistentClassName");

			Assert::IsNotNull(ACScope->As<AttributedChild>());
			Assert::IsNotNull(ABScope->As<AttributedBar>());
			Assert::IsNull(InvalidScope);

			delete ACScope;
			delete ABScope;
		}


		// TODO - uncomment me

		/*
		TEST_METHOD(AttributedBarParsing)
		{
			FactoryManager<Scope>::AddFactory("AttributedChild", *(new AttributedChildFactory()));
			FactoryManager<Scope>::AddFactory("AttributedBar", *(new AttributedBarFactory()));

			std::string Root = R"(
			{
				"AttributedBar_bar": {
					"int_Int" : 100
				}
			}
			)"s;

			ScopeWrapper ScopeWrap;

			ParseCoordinator PC(ScopeWrap);

			ScopeWrap.AddScopeParseHandlers(PC);

			PC.DeserializeObject(Root);

			Scope& RootScope = ScopeWrap.GetRoot();
			Datum* BarScopeDatum = RootScope.Find("bar");
			Assert::IsNotNull(BarScopeDatum);
			Scope* BarScope = BarScopeDatum->GetScope();
			Assert::IsNotNull(BarScope);
			Datum* IntDatum = BarScope->Find("Int");
			Assert::IsNotNull(IntDatum);
			Assert::AreEqual(100, IntDatum->GetInt());

			FactoryManager<Scope>::Clear();
		}
		*/


	private:
		inline static _CrtMemState _startMemState;
	};

}