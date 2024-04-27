#include "pch.h"
#include "CppUnitTest.h"
#include "Serialization.h"
#include <vector>
#include <codecvt>
#include <fstream>
#include "FieaGameEngine.Test.cpp"

#include "Datum.h"
#include "Scope.h"
#include "RTTI.h"
#include "Attributed.h" 
#include "TypeRegistry.h"

#include "ParseCoordinator.h"
#include "ParseHandler.h"

#include "ScopeWrapper.h"
#include "FloatParseHandler.h"
#include "IntParseHandler.h"
#include "Vec4ParseHandler.h"
#include "MatrixParseHandler.h"
#include "StringParseHandler.h"
#include "ScopeParseHandler.h"

#include "IntTestHandler.h"
#include "TestParseHandler.h"



using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::Test;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;


namespace Fiea::GameEngine::Test
{

	TEST_CLASS(ParseCoordinatorTest)
	{
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


		TEST_METHOD(BasicDeserialization)
		{
			const std::string Root = R"(
			{
				"myInt" : 34
			}
			)"s;

			IntTestWrapper IntWrapper;

			ParseCoordinator PC(IntWrapper);

			Assert::ExpectException<std::runtime_error>([&PC, &Root]() { PC.DeserializeObject(Root); });
		}

		TEST_METHOD(TestParseHandling)
		{
			IntTestWrapper IntWrapper;
			TestWrapper WrapperTest;

			ParseCoordinator PC(IntWrapper);

			PC.AddParseHandler(std::make_shared<IntTestHandler>());
			PC.AddParseHandler(std::make_shared<TestHandler>());


			// TEST 1
			std::string Root = R"(
			{
				"myInt" : 24
			}
			)"s;

			PC.DeserializeObject(Root);

			Assert::AreEqual(24, IntWrapper.GetInt());
			Assert::IsFalse(WrapperTest._Reached);


			// TEST 2
			std::string Root2 = R"(
			{
				"myInt" : 41
			}
			)"s;

			PC.SetWrapper(WrapperTest);

			PC.DeserializeObject(Root2);

			Assert::IsTrue(WrapperTest._Reached);
			Assert::AreNotEqual(41, IntWrapper.GetInt());


			// TEST 3
			std::string Root3 = R"(
			{
				"Ints" : [1, 2, 3]
			}
			)"s;

			PC.SetWrapper(IntWrapper);

			PC.DeserializeObject(Root3);

			Assert::AreEqual(1, IntWrapper.GetInt(0));
			Assert::AreEqual(2, IntWrapper.GetInt(1));
			Assert::AreEqual(3, IntWrapper.GetInt(2));
		}


		TEST_METHOD(ScopeParsing)
		{
			ScopeWrapper ScopeWrap;

			ParseCoordinator PC(ScopeWrap);

			ScopeWrap.AddScopeParseHandlers(PC);

			std::string Root = R"(
			{
				"dragon": {
					"health" : 100
				}
			}
			)"s;

			PC.DeserializeObject(Root);

			Scope& RootScope = ScopeWrap.GetRoot();
			Assert::IsNotNull(&RootScope);

			Datum* ChildScopeDatum = RootScope.Find("dragon");
			Assert::IsNotNull(ChildScopeDatum);

			Scope* ChildScope = ChildScopeDatum->GetScope();
			Assert::IsNotNull(ChildScope);

			Datum* HealthDatum = ChildScope->Find("health");
			Assert::AreEqual(100, HealthDatum->GetInt());
		}

		TEST_METHOD(ParsingNestedScopes)
		{
			ScopeWrapper ScopeWrap;

			ParseCoordinator PC(ScopeWrap);

			ScopeWrap.AddScopeParseHandlers(PC);

			// Triple nesting of scopes
			std::string Root = R"(
			{
				"dragon": 
				{
					"princess" : 
					{
						"health" : 50,
						"crown"	: 
						{
							"worth" : 123
						}
					}
				}
			}
			)"s;

			PC.DeserializeObject(Root);


			Scope& RootScope = ScopeWrap.GetRoot();
			Assert::IsNotNull(&RootScope);

			// Check dragon
			Datum* ChildScopeDatum = RootScope.Find("dragon");
			Assert::IsNotNull(ChildScopeDatum);

			Scope* DragonScope = ChildScopeDatum->GetScope();
			Assert::IsNotNull(DragonScope);

			// Check princess
			Datum* PrincessDatum = DragonScope->Find("princess");
			Assert::IsNotNull(ChildScopeDatum);

			Scope* PrincessScope = PrincessDatum->GetScope();
			Assert::IsNotNull(PrincessScope);

			// Check princess health
			Datum* HealthDatum = PrincessScope->Find("health");
			Assert::AreEqual(50, HealthDatum->GetInt());

			// Check crown
			Datum* CrownDatum = PrincessScope->Find("crown");
			Assert::IsNotNull(CrownDatum);

			Scope* CrownScope = CrownDatum->GetScope();
			Assert::IsNotNull(CrownScope);

			// Check crown worth
			Datum* WorthDatum = CrownScope->Find("worth");
			Assert::AreEqual(123, WorthDatum->GetInt());
		}

		TEST_METHOD(ScopeArray)
		{
			ScopeWrapper ScopeWrap;

			ParseCoordinator PC(ScopeWrap);

			ScopeWrap.AddScopeParseHandlers(PC);

			std::string Root = R"(
			{
				"dragon": {
					"treasures" : [
						{
							"value" : 100
						},
						{
							"value" : 200
						}
					]
				}
			}
			)"s;

			PC.DeserializeObject(Root);

			Scope& RootScope = ScopeWrap.GetRoot();
			Assert::IsNotNull(&RootScope);

			Datum* DragonDatum = RootScope.Find("dragon");
			Assert::IsNotNull(DragonDatum);

			Scope* DragonScope = DragonDatum->GetScope();
			Assert::IsNotNull(DragonScope);

			Datum* TreasureDatums = DragonScope->Find("treasures");
			Assert::IsNotNull(TreasureDatums);

			Scope* FirstScope = TreasureDatums->GetScope(0);
			Assert::IsNotNull(FirstScope);
			Datum* FirstValueDatum = FirstScope->Find("value");
			Assert::IsNotNull(FirstValueDatum);
			Assert::AreEqual(100, FirstValueDatum->GetInt());

			Scope* SecondScope = TreasureDatums->GetScope(1);
			Assert::IsNotNull(SecondScope);
			Datum* SecondValueDatum = SecondScope->Find("value");
			Assert::IsNotNull(SecondValueDatum);
			Assert::AreEqual(200, SecondValueDatum->GetInt());
		}

		TEST_METHOD(ScopeIntParsing)
		{
			ScopeWrapper Wrapper;

			ParseCoordinator PC(Wrapper);
			Wrapper.AddScopeParseHandlers(PC);

			const std::string Root = R"(
			{
				"dragon": {
					"int_health" : 100,
					"int_damage" : 40
				}
			}
			)"s;

			PC.DeserializeObject(Root);
		}

		TEST_METHOD(IntParsing)
		{
			std::string Root = R"(
			{
				"myInt" : 34,
				"myObj" : 
				{
					"myStr" : "hello",
					"myBool" : true
				}	
			}
			)"s;
		}



		TEST_METHOD(FloatParsing)
		{
			ScopeWrapper ScopeWrap;

			ParseCoordinator PC(ScopeWrap);

			ScopeWrap.AddScopeParseHandlers(PC);

			std::string Root = R"(
			{
				"single" : 123.456,
				"array" : [1.1, 2.2, 3.3]
			}
			)"s;

			PC.DeserializeObject(Root);

			Scope& RootScope = ScopeWrap.GetRoot();
			Assert::IsNotNull(&RootScope);

			// Single
			Datum* FloatDatum = RootScope.Find("single");
			Assert::IsNotNull(FloatDatum);

			Assert::AreEqual((float)123.456, FloatDatum->GetFloat());

			// Array
			Datum* FloatArrayDatum = RootScope.Find("array");
			Assert::IsNotNull(FloatArrayDatum);

			Assert::AreEqual((float)1.1, FloatArrayDatum->GetFloat(0));
			Assert::AreEqual((float)2.2, FloatArrayDatum->GetFloat(1));
			Assert::AreEqual((float)3.3, FloatArrayDatum->GetFloat(2));
		}

		
		TEST_METHOD(StringParsing)
		{
			ScopeWrapper ScopeWrap;

			ParseCoordinator PC(ScopeWrap);

			ScopeWrap.AddScopeParseHandlers(PC);

			std::string Root = R"(
			{
				"single" : "help",
				"array" : ["my", "head", "hurts"]
			}
			)"s;

			PC.DeserializeObject(Root);

			Scope& RootScope = ScopeWrap.GetRoot();
			Assert::IsNotNull(&RootScope);

			// Single float
			Datum* SingleValueDatum = RootScope.Find("single");
			Assert::IsNotNull(SingleValueDatum);

			Assert::AreEqual(std::string("help"), SingleValueDatum->GetString());

			// Float array
			Datum* ArrayDatum = RootScope.Find("array");
			Assert::IsNotNull(ArrayDatum);

			Assert::AreEqual(std::string("my"), ArrayDatum->GetString(0));
			Assert::AreEqual(std::string("head"), ArrayDatum->GetString(1));
			Assert::AreEqual(std::string("hurts"), ArrayDatum->GetString(2));
		}

		

		TEST_METHOD(Vec4Parsing)
		{
			ScopeWrapper ScopeWrap;

			ParseCoordinator PC(ScopeWrap);

			ScopeWrap.AddScopeParseHandlers(PC);

			std::string Root = R"(
			{
				"single" : "vec4| 1, 2, 3, 4 |",
				"array" : ["vec4| 10, 20, 30, 40 |", "vec4| 100, 200, 300, 400 |"]
			}
			)"s;



			PC.DeserializeObject(Root);

			Scope& RootScope = ScopeWrap.GetRoot();
			Assert::IsNotNull(&RootScope);

			// Single
			Datum* SingleValueDatum = RootScope.Find("single");
			Assert::IsNotNull(SingleValueDatum);

			Assert::AreEqual(glm::vec4(1,2,3,4), SingleValueDatum->GetVec4());

			// Array
			Datum* ArrayDatum = RootScope.Find("array");
			Assert::IsNotNull(ArrayDatum);

			Assert::AreEqual(glm::vec4(10, 20, 30, 40), ArrayDatum->GetVec4(0));
			Assert::AreEqual(glm::vec4(100, 200, 300, 400), ArrayDatum->GetVec4(1));
		}

		TEST_METHOD(MatrixParsing)
		{
			ScopeWrapper ScopeWrap;

			ParseCoordinator PC(ScopeWrap);

			ScopeWrap.AddScopeParseHandlers(PC);

			std::string Root = R"(
			{
				"single" : "mat4x4| 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 |",
				"array" : ["mat4x4| 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 |", "mat4x4| 1, 1, 1, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 |"]
			}
			)"s;

			PC.DeserializeObject(Root);

			Scope& RootScope = ScopeWrap.GetRoot();
			Assert::IsNotNull(&RootScope);

			glm::vec4 V(1);
			glm::vec4 V1(1, 2, 3, 4);
			glm::vec4 V2(5, 6, 7, 8);
			glm::vec4 V3(9, 10, 11, 12);
			glm::vec4 V4(13, 14, 15, 16);


			// Single
			Datum* SingleValueDatum = RootScope.Find("single");
			Assert::IsNotNull(SingleValueDatum);

			Assert::AreEqual(glm::mat4x4(V1, V2, V3, V4), SingleValueDatum->GetMat4x4());

			// Array
			Datum* ArrayDatum = RootScope.Find("array");
			Assert::IsNotNull(ArrayDatum);

			Assert::AreEqual(glm::mat4x4(V1, V2, V3, V4), ArrayDatum->GetMat4x4(0));
			Assert::AreEqual(glm::mat4x4(V, V2, V3, V4), ArrayDatum->GetMat4x4(1));
		}

		
		TEST_METHOD(FileParsing)
		{
			std::ifstream File;
			
			ScopeWrapper SWrapper;

			ParseCoordinator PC(SWrapper);
			
			SWrapper.AddScopeParseHandlers(PC);

			Assert::ExpectException<std::runtime_error>([&PC] { PC.DeserializeObjectFromFile("wrong_name.json"); });

			PC.DeserializeObjectFromFile("DragonConfig.json");

			Scope& RootScope = SWrapper.GetRoot();
			Assert::IsNotNull(&RootScope);

			Datum* ChildScopeDatum = RootScope.Find("dragon");
			Assert::IsNotNull(ChildScopeDatum);

			Scope* ChildScope = ChildScopeDatum->GetScope();
			Assert::IsNotNull(ChildScope);

			Datum* HealthDatum = ChildScope->Find("health");
			Assert::AreEqual(100, HealthDatum->GetInt());
		}
		


	private:
		inline static _CrtMemState _startMemState;
	};
}

