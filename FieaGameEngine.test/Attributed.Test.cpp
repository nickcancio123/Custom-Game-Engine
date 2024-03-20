#include "pch.h"
#include "CppUnitTest.h"
#include "Serialization.h"
#include <vector>
#include <codecvt>
#include "AttributeRegistry.h"
#include "FieaGameEngine.Test.cpp"

#include "Datum.h"
#include "Scope.h"
#include "RTTI.h"
#include "Attributed.h" 
#include "Empty.h"
#include "EmptyChild.h"
#include "Foo.h"
#include "AttributedChild.h"


using namespace Fiea::GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace FieaGameEngineTest
{
	TEST_CLASS(AttributedTests)
	{
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			AttributeRegistry::Create();
		}

		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			AttributeRegistry::Destroy();
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


		TEST_METHOD(CreatePrescribedAttributes)
		{
			// This 
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("this");
				Assert::IsNotNull(D1);

				// Test double binding
				Assert::IsTrue(reinterpret_cast<RTTI*>(&AC) == D1->GetPointer());
			}

			// Double prescribed attribute creation
			{
				AttributedChild AC;

				AC.CreatePrescribedAttributesAgain();
			}

			// Pointer
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("Foo");
				Assert::IsNotNull(D1);

				// Test double binding
				Assert::IsTrue(reinterpret_cast<RTTI*>(&AC._Foo) == D1->GetPointer());
			}

			// Pointer array
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("FooArray");
				Assert::IsNotNull(D1);

				Assert::AreEqual((unsigned int)3, D1->Size());

				// Test double binding
				Assert::IsTrue(reinterpret_cast<RTTI*>(&AC._FooArray[0]) == D1->GetPointer(0));
				Assert::IsTrue(reinterpret_cast<RTTI*>(&AC._FooArray[1]) == D1->GetPointer(1));
				Assert::IsTrue(reinterpret_cast<RTTI*>(&AC._FooArray[2]) == D1->GetPointer(2));
			}


			// Int
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("Int");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._Int, D1->GetInt());

				// Test double binding
				AC._Int = 5;
				Assert::AreEqual(AC._Int, D1->GetInt());
				D1->Set(10);
				Assert::AreEqual(AC._Int, D1->GetInt());
			}

			// Int array
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("IntArray");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._IntArray[0], D1->GetInt(0));
				Assert::AreEqual(AC._IntArray[1], D1->GetInt(1));
				Assert::AreEqual(AC._IntArray[2], D1->GetInt(2));

				// Test double binding
				AC._IntArray[0] = 5;
				Assert::AreEqual(AC._IntArray[0], D1->GetInt(0));
				D1->Set(10, 2);
				Assert::AreEqual(AC._IntArray[2], D1->GetInt(2));
			}

			// Float
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("Float");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._Float, D1->GetFloat());

				// Test double binding
				AC._Int = 5;
				Assert::AreEqual(AC._Float, D1->GetFloat());
				D1->Set(10);
				Assert::AreEqual(AC._Float, D1->GetFloat());
			}

			// Float array
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("FloatArray");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._FloatArray[0], D1->GetFloat(0));
				Assert::AreEqual(AC._FloatArray[1], D1->GetFloat(1));
				Assert::AreEqual(AC._FloatArray[2], D1->GetFloat(2));

				// Test double binding
				AC._FloatArray[0] = 5.0f;
				Assert::AreEqual(AC._FloatArray[0], D1->GetFloat(0));
				D1->Set(10.0f, 2);
				Assert::AreEqual(AC._FloatArray[2], D1->GetFloat(2));
			}

			// String
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("String");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._String, D1->GetString());

				// Test double binding
				AC._String = std::string("Test1");
				Assert::AreEqual(AC._String, D1->GetString());
				D1->Set(std::string("Test2"));
				Assert::AreEqual(AC._String, D1->GetString());
			}

			// String array
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("StringArray");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._StringArray[0], D1->GetString(0));
				Assert::AreEqual(AC._StringArray[1], D1->GetString(1));
				Assert::AreEqual(AC._StringArray[2], D1->GetString(2));

				// Test double binding
				AC._StringArray[0] = std::string("Test1");
				Assert::AreEqual(AC._StringArray[0], D1->GetString(0));
				D1->Set(std::string("Test2"), 2);
				Assert::AreEqual(AC._StringArray[2], D1->GetString(2));
			}

			// Vec4
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("Vec4");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._Vec4, D1->GetVec4());

				// Test double binding
				AC._Vec4 = glm::vec4(5);
				Assert::AreEqual(AC._Vec4, D1->GetVec4());
				D1->Set(glm::vec4(10));
				Assert::AreEqual(AC._Vec4, D1->GetVec4());
			}

			// Vec4 array
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("Vec4Array");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._Vec4Array[0], D1->GetVec4(0));
				Assert::AreEqual(AC._Vec4Array[1], D1->GetVec4(1));
				Assert::AreEqual(AC._Vec4Array[2], D1->GetVec4(2));

				// Test double binding
				AC._Vec4Array[0] = glm::vec4(5);
				Assert::AreEqual(AC._Vec4Array[0], D1->GetVec4(0));
				D1->Set(glm::vec4(10), 2);
				Assert::AreEqual(AC._Vec4Array[2], D1->GetVec4(2));
			}

			// Mat4x4
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("Mat4x4");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._Mat4x4, D1->GetMat4x4());

				// Test double binding
				AC._Mat4x4 = glm::mat4x4(5);
				Assert::AreEqual(AC._Mat4x4, D1->GetMat4x4());
				D1->Set(glm::mat4x4(10));
				Assert::AreEqual(AC._Mat4x4, D1->GetMat4x4());
			}

			// Mat4x4 array
			{
				AttributedChild AC;

				Datum* D1 = AC.Find("Mat4x4Array");
				Assert::IsNotNull(D1);

				Assert::AreEqual(AC._Mat4x4Array[0], D1->GetMat4x4(0));
				Assert::AreEqual(AC._Mat4x4Array[1], D1->GetMat4x4(1));
				Assert::AreEqual(AC._Mat4x4Array[2], D1->GetMat4x4(2));

				// Test double binding
				AC._Mat4x4Array[0] = glm::mat4x4(5);
				Assert::AreEqual(AC._Mat4x4Array[0], D1->GetMat4x4(0));
				D1->Set(glm::mat4x4(10), 2);
				Assert::AreEqual(AC._Mat4x4Array[2], D1->GetMat4x4(2));
			}
		}

		TEST_METHOD(IsPrescribedAttribute)
		{
			AttributedChild AC;

			Assert::IsTrue(AC.IsPrescribedAttribute("this"));

			Assert::IsTrue(AC.IsPrescribedAttribute("Foo"));
			Assert::IsTrue(AC.IsPrescribedAttribute("Int"));
			Assert::IsTrue(AC.IsPrescribedAttribute("Float"));
			Assert::IsTrue(AC.IsPrescribedAttribute("String"));
			Assert::IsTrue(AC.IsPrescribedAttribute("Vec4"));
			Assert::IsTrue(AC.IsPrescribedAttribute("Mat4x4"));

			Assert::IsTrue(AC.IsPrescribedAttribute("FooArray"));
			Assert::IsTrue(AC.IsPrescribedAttribute("IntArray"));
			Assert::IsTrue(AC.IsPrescribedAttribute("FloatArray"));
			Assert::IsTrue(AC.IsPrescribedAttribute("StringArray"));
			Assert::IsTrue(AC.IsPrescribedAttribute("Vec4Array"));
			Assert::IsTrue(AC.IsPrescribedAttribute("Mat4x4Array"));

			Assert::IsFalse(AC.IsPrescribedAttribute("test"));
			Assert::IsFalse(AC.IsPrescribedAttribute("blah"));
			Assert::IsFalse(AC.IsPrescribedAttribute("SOMETHING"));
		}

		TEST_METHOD(AppendAuxilliaryAttribute)
		{
			AttributedChild AC;

			Assert::ExpectException<std::runtime_error>([&AC]() { AC.AppendAuxilliaryAttribute("this"); });
			
			Datum& AppendedDatum = AC.AppendAuxilliaryAttribute("Aux");
			
			Assert::AreSame(AppendedDatum, *AC.Find("Aux"));
		}

		TEST_METHOD(IsAttribute)
		{
			AttributedChild AC;

			Assert::IsTrue(AC.IsAttribute("this"));

			Assert::IsTrue(AC.IsAttribute("Foo"));
			Assert::IsTrue(AC.IsAttribute("Int"));
			Assert::IsTrue(AC.IsAttribute("Float"));
			Assert::IsTrue(AC.IsAttribute("String"));
			Assert::IsTrue(AC.IsAttribute("Vec4"));
			Assert::IsTrue(AC.IsAttribute("Mat4x4"));

			Assert::IsTrue(AC.IsAttribute("FooArray"));
			Assert::IsTrue(AC.IsAttribute("IntArray"));
			Assert::IsTrue(AC.IsAttribute("FloatArray"));
			Assert::IsTrue(AC.IsAttribute("StringArray"));
			Assert::IsTrue(AC.IsAttribute("Vec4Array"));
			Assert::IsTrue(AC.IsAttribute("Mat4x4Array"));

			Assert::IsFalse(AC.IsAttribute("test"));
			Assert::IsFalse(AC.IsAttribute("blah"));
			Assert::IsFalse(AC.IsAttribute("SOMETHING"));
		}

		TEST_METHOD(IsAuxilliaryAttribute)
		{
			AttributedChild AC;

			// Prescribed attributes
			Assert::IsFalse(AC.IsAuxilliaryAttribute("this"));

			Assert::IsFalse(AC.IsAuxilliaryAttribute("Foo"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("Int"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("Float"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("String"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("Vec4"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("Mat4x4"));

			Assert::IsFalse(AC.IsAuxilliaryAttribute("FooArray"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("IntArray"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("FloatArray"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("StringArray"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("Vec4Array"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("Mat4x4Array"));

			// Unadded attributes
			Assert::IsFalse(AC.IsAuxilliaryAttribute("test"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("blah"));
			Assert::IsFalse(AC.IsAuxilliaryAttribute("SOMETHING"));

			// Added auxilliary attributes
			AC.AppendAuxilliaryAttribute("Aux1");
			AC.AppendAuxilliaryAttribute("Aux2");

			Assert::IsTrue(AC.IsAuxilliaryAttribute("Aux1"));
			Assert::IsTrue(AC.IsAuxilliaryAttribute("Aux2"));
		}

		TEST_METHOD(ComparisonOps)
		{
			AttributedChild AC1;
			AttributedChild AC2;

			Assert::IsTrue(AC1 == AC2);
			Assert::IsFalse(AC1 != AC2);

			AC1.AppendAuxilliaryAttribute("Aux1");
			AC1.AppendAuxilliaryAttribute("Aux2");

			Assert::IsTrue(AC1 != AC2);
			Assert::IsFalse(AC1 == AC2);
		}

		TEST_METHOD(CopyConstructor)
		{
			AttributedChild AC1;
			AttributedChild AC2(AC1);
			Assert::AreEqual(AC1, AC2);

			AC1.AppendAuxilliaryAttribute("foo");
			AttributedChild AC3(AC1);
			Assert::AreEqual(AC1, AC3);
		}

		TEST_METHOD(CopyAssignment)
		{
			AttributedChild AC1;

			// Self assignment
			Assert::AreSame(AC1, AC1 = AC1);

			AttributedChild AC2;
			AC2 = AC1;
			Assert::AreEqual(AC1, AC2);

			AC1.AppendAuxilliaryAttribute("foo");
			AttributedChild AC3;
			AC3 = AC1;
			Assert::AreEqual(AC1, AC3);
		}

		TEST_METHOD(MoveSemantics)
		{
			// Move construction
			{
				AttributedChild AC1;
				AttributedChild AC2 = std::move(AC1);
				Assert::AreEqual(AC1, AC2);

				AC1.AppendAuxilliaryAttribute("foo");
				AttributedChild AC3(std::move(AC1));
				Assert::AreEqual(AC1, AC3);
			}


			// Move assignment
			{
				AttributedChild AC1;

				// Self assignment
				Assert::AreSame(AC1, AC1 = std::move(AC1));

				AttributedChild AC2;
				AC2 = std::move(AC1);
				Assert::AreNotEqual(AC1, AC2);

				AttributedChild AC3;
				AC3.AppendAuxilliaryAttribute("foo");
				AttributedChild AC4;
				AC4 = std::move(AC3);
				Assert::AreNotEqual(AC3, AC4);
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};

}