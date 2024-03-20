#include "pch.h"
#include "CppUnitTest.h"
#include "Serialization.h"
#include <vector>
#include <codecvt>
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
	TEST_CLASS(DatumTests)
	{
	public:

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


		TEST_METHOD(Constructors)
		{
			Datum Datum1;
			Assert::AreEqual(Datum::UKNOWN, Datum1.GetType());

			Datum Datum2(Datum::INT);
			Assert::AreEqual(Datum::INT, Datum2.GetType());
		}

		TEST_METHOD(SetType)
		{
			{
				Datum MyDatum;

				// Check setting to unknown
				Assert::IsFalse(MyDatum.SetType(Datum::UKNOWN));

				// Check double type set
				Assert::IsTrue(MyDatum.SetType(Datum::INT));
				Assert::IsFalse(MyDatum.SetType(Datum::FLOAT));
			}

			// Int
			{
				Datum MyDatum;
				Assert::IsTrue(MyDatum.SetType(Datum::INT));
				Assert::AreEqual(Datum::INT, MyDatum.GetType());
			}

			// Float
			{
				Datum MyDatum;
				Assert::IsTrue(MyDatum.SetType(Datum::FLOAT));
				Assert::AreEqual(Datum::FLOAT, MyDatum.GetType());
			}

			// String
			{
				Datum MyDatum;
				Assert::IsTrue(MyDatum.SetType(Datum::STRING));
				Assert::AreEqual(Datum::STRING, MyDatum.GetType());
			}

			// Vector4
			{
				Datum MyDatum;
				Assert::IsTrue(MyDatum.SetType(Datum::VEC4));
				Assert::AreEqual(Datum::VEC4, MyDatum.GetType());
			}

			// Mat4x4
			{
				Datum MyDatum;
				Assert::IsTrue(MyDatum.SetType(Datum::MAT4X4));
				Assert::AreEqual(Datum::MAT4X4, MyDatum.GetType());
			}
		}

		TEST_METHOD(PushBackAndGet)
		{
			// Check Int
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::INT);

				int MyInt = 1;

				// Valid push
				Assert::IsTrue(MyDatum.PushBack(MyInt));

				// Invalid push
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(Datum2.PushBack(1));


				// Valid get
				Assert::AreEqual(MyInt, MyDatum.GetInt());

				// Invalid gets
				auto UnderLamda = [&MyDatum] { MyDatum.GetInt(-1); };
				Assert::ExpectException<std::out_of_range>(UnderLamda);

				auto OverLamda = [&MyDatum] { MyDatum.GetInt(100); };
				Assert::ExpectException<std::out_of_range>(OverLamda);

				auto WrongTypeLamda = [&MyDatum] { MyDatum.GetFloat(); };
				Assert::ExpectException<std::runtime_error>(WrongTypeLamda);


				// Push without set type
				Datum MyDatum2;
				Assert::IsTrue(MyDatum2.PushBack(1));
				Assert::AreEqual(Datum::INT, MyDatum.GetType());
			}

			// Check Float
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::FLOAT);

				float MyFloat = 1.0f;

				// Valid push
				Assert::IsTrue(MyDatum.PushBack(MyFloat));

				// Invalid push
				Datum Datum2(Datum::INT);
				Assert::IsFalse(Datum2.PushBack(1.0f));


				// Valid get
				Assert::AreEqual(MyFloat, MyDatum.GetFloat(0));

				// Invalid gets
				auto UnderLamda = [&MyDatum] { MyDatum.GetFloat(-1); };
				Assert::ExpectException<std::out_of_range>(UnderLamda);

				auto OverLamda = [&MyDatum] { MyDatum.GetFloat(100); };
				Assert::ExpectException<std::out_of_range>(OverLamda);

				auto WrongTypeLamda = [&MyDatum] { MyDatum.GetString(0); };
				Assert::ExpectException<std::runtime_error>(WrongTypeLamda);


				// Push without set type
				Datum MyDatum2;
				Assert::IsTrue(MyDatum2.PushBack(1));
				Assert::AreEqual(Datum::FLOAT, MyDatum.GetType());
			}

			// Check String
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::STRING);

				std::string MyString("Hello World!");

				// Valid push
				Assert::IsTrue(MyDatum.PushBack(MyString));

				// Invalid push
				Datum Datum2(Datum::INT);
				Assert::IsFalse(Datum2.PushBack(std::string("1")));


				// Valid get
				Assert::AreEqual(MyString, MyDatum.GetString(0));

				// Invalid gets
				auto UnderLamda = [&MyDatum] { MyDatum.GetString(-1); };
				Assert::ExpectException<std::out_of_range>(UnderLamda);

				auto OverLamda = [&MyDatum] { MyDatum.GetString(100); };
				Assert::ExpectException<std::out_of_range>(OverLamda);

				auto WrongTypeLamda = [&MyDatum] { MyDatum.GetInt(0); };
				Assert::ExpectException<std::runtime_error>(WrongTypeLamda);


				// Push without set type
				Datum MyDatum2;
				Assert::IsTrue(MyDatum2.PushBack(std::string("Test")));
				Assert::AreEqual(Datum::STRING, MyDatum.GetType());
			}

			// Check Vec4
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::VEC4);

				glm::vec4 MyVec4 = glm::vec4(1);

				// Valid push
				Assert::IsTrue(MyDatum.PushBack(MyVec4));

				// Invalid push
				Datum Datum2(Datum::INT);
				Assert::IsFalse(Datum2.PushBack(glm::vec4(1.0f)));

				// Valid get
				Assert::AreEqual(MyVec4, MyDatum.GetVec4(0));

				// Invalid gets
				auto UnderLamda = [&MyDatum] { MyDatum.GetVec4(-1); };
				Assert::ExpectException<std::out_of_range>(UnderLamda);

				auto OverLamda = [&MyDatum] { MyDatum.GetVec4(100); };
				Assert::ExpectException<std::out_of_range>(OverLamda);

				auto WrongTypeLamda = [&Datum2] { Datum2.GetVec4(0); };
				Assert::ExpectException<std::runtime_error>(WrongTypeLamda);


				// Push without set type
				Datum MyDatum2;
				Assert::IsTrue(MyDatum2.PushBack(MyVec4));
				Assert::AreEqual(Datum::VEC4, MyDatum.GetType());
			}

			// Check Mat4x4
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::MAT4X4);

				glm::mat4x4 MyMat = glm::mat4x4(2);

				// Valid push
				Assert::IsTrue(MyDatum.PushBack(MyMat));

				// Invalid push
				Datum Datum2(Datum::INT);
				Assert::IsFalse(Datum2.PushBack(glm::mat4x4(1.0f)));

				// Valid get
				Assert::AreEqual(MyMat, MyDatum.GetMat4x4(0));

				// Invalid gets
				auto UnderLamda = [&MyDatum] { MyDatum.GetMat4x4(-1); };
				Assert::ExpectException<std::out_of_range>(UnderLamda);

				auto OverLamda = [&MyDatum] { MyDatum.GetMat4x4(100); };
				Assert::ExpectException<std::out_of_range>(OverLamda);

				auto WrongTypeLamda = [&Datum2] { Datum2.GetMat4x4(0); };
				Assert::ExpectException<std::runtime_error>(WrongTypeLamda);


				// Push without set type
				Datum MyDatum2;
				Assert::IsTrue(MyDatum2.PushBack(MyMat));
				Assert::AreEqual(Datum::MAT4X4, MyDatum.GetType());
			}

			// Check Scope/Scope
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::SCOPE);

				Scope MyScope;
				MyScope.Append("D1").PushBack(5);

				// Valid push
				Assert::IsTrue(MyDatum.PushBack(&MyScope));

				// Invalid push
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(Datum2.PushBack(&MyScope));


				// Valid get
				Assert::AreSame(MyScope, *MyDatum.GetScope(0));

				// Invalid gets
				auto UnderLamda = [&MyDatum] { MyDatum.GetScope(-1); };
				Assert::ExpectException<std::out_of_range>(UnderLamda);

				auto OverLamda = [&MyDatum] { MyDatum.GetScope(100); };
				Assert::ExpectException<std::out_of_range>(OverLamda);

				auto WrongTypeLamda = [&Datum2] { Datum2.GetScope(0); };
				Assert::ExpectException<std::runtime_error>(WrongTypeLamda);


				// Push without set type
				Datum MyDatum2;
				Assert::IsTrue(MyDatum2.PushBack(&MyScope));
				Assert::AreEqual(Datum::SCOPE, MyDatum.GetType());
			}

			// Check Pointer
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::POINTER);

				RTTI* R1 = new Empty();

				// Valid push
				Assert::IsTrue(MyDatum.PushBack(R1));

				// Invalid push
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(Datum2.PushBack(R1));


				// Valid get
				Assert::AreSame(*R1, *MyDatum.GetPointer(0));

				// Invalid gets
				auto UnderLamda = [&MyDatum] { MyDatum.GetPointer(-1); };
				Assert::ExpectException<std::out_of_range>(UnderLamda);

				auto OverLamda = [&MyDatum] { MyDatum.GetPointer(100); };
				Assert::ExpectException<std::out_of_range>(OverLamda);

				auto WrongTypeLamda = [&Datum2] { Datum2.GetPointer(0); };
				Assert::ExpectException<std::runtime_error>(WrongTypeLamda);


				// Push without set type
				Datum MyDatum2;
				Assert::IsTrue(MyDatum2.PushBack(R1));
				Assert::AreEqual(Datum::POINTER, MyDatum.GetType());


				delete R1;
			}

			// Push when external
			{
				int Data = 3;
				Datum Datum1(Datum::INT);
				Datum1.StoreExternal(&Data, 1);

				Assert::IsFalse(Datum1.PushBack(1));
				Assert::IsFalse(Datum1.PushBack(1));
				Assert::IsFalse(Datum1.PushBack(std::string("1")));
				Assert::IsFalse(Datum1.PushBack(glm::vec4(1.0f)));
				Assert::IsFalse(Datum1.PushBack(glm::mat4x4(1.0f)));
			}
		}

		TEST_METHOD(Size)
		{
			Datum MyDatum;
			MyDatum.SetType(Datum::INT);

			// Zero size test
			Assert::AreEqual((unsigned int)0, MyDatum.Size());

			// Add size test
			int ExpectedSize = 10;
			for (int i = 0; i < ExpectedSize; ++i)
			{
				MyDatum.PushBack(i);
			}
			Assert::AreEqual((unsigned int)ExpectedSize, MyDatum.Size());
		}

		TEST_METHOD(PopBack)
		{
			// Int test
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::INT);

				// Pop with zero size check
				Assert::IsFalse(MyDatum.PopBack());

				// Normal pop test
				for (int i = 0; i < 10; ++i)
				{
					MyDatum.PushBack(i);
				}

				unsigned int ExpectedSize = MyDatum.Size() - 1;
				Assert::IsTrue(MyDatum.PopBack());
				Assert::AreEqual(ExpectedSize, MyDatum.Size());
			}

			// Float test
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::FLOAT);

				// Pop with zero size check
				Assert::IsFalse(MyDatum.PopBack());

				// Normal pop test
				for (unsigned int i = 0; i < 10; ++i)
				{
					MyDatum.PushBack((float)i);
				}

				unsigned int ExpectedSize = MyDatum.Size() - 1;
				Assert::IsTrue(MyDatum.PopBack());
				Assert::AreEqual(ExpectedSize, MyDatum.Size());
			}

			// String test
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::STRING);

				// Pop with zero size check
				Assert::IsFalse(MyDatum.PopBack());

				// Normal pop test
				for (unsigned int i = 0; i < 10; ++i)
				{
					MyDatum.PushBack(std::string("Test"));
				}

				unsigned int ExpectedSize = MyDatum.Size() - 1;
				Assert::IsTrue(MyDatum.PopBack());
				Assert::AreEqual(ExpectedSize, MyDatum.Size());
			}

			// Vec4 test
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::VEC4);

				// Pop with zero size check
				Assert::IsFalse(MyDatum.PopBack());

				// Normal pop test
				for (unsigned int i = 0; i < 10; ++i)
				{
					MyDatum.PushBack(glm::vec4(1));
				}

				unsigned int ExpectedSize = MyDatum.Size() - 1;
				Assert::IsTrue(MyDatum.PopBack());
				Assert::AreEqual(ExpectedSize, MyDatum.Size());
			}

			// Vec4 test
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::MAT4X4);

				// Pop with zero size check
				Assert::IsFalse(MyDatum.PopBack());

				// Normal pop test
				for (unsigned int i = 0; i < 10; ++i)
				{
					MyDatum.PushBack(glm::mat4x4(1));
				}

				unsigned int ExpectedSize = MyDatum.Size() - 1;
				Assert::IsTrue(MyDatum.PopBack());
				Assert::AreEqual(ExpectedSize, MyDatum.Size());
			}

			// Pop back with disallowed types
			{
				Datum D1;
				Scope Scope1;
				D1.PushBack(&Scope1);
				Assert::ExpectException<std::runtime_error>([&D1] { D1.PopBack(); });

				RTTI* R1 = new Empty();
				Datum D2;
				D2.PushBack(R1);
				Assert::ExpectException<std::runtime_error>([&D2] { D2.PopBack(); });
				delete R1;
			}
		}

		TEST_METHOD(Clear)
		{
			{
				Datum MyDatum;

				int FilledSize = 10;

				for (int i = 0; i < FilledSize; ++i)
				{
					MyDatum.PushBack(i);
				}

				Assert::AreEqual((unsigned int)FilledSize, MyDatum.Size());

				MyDatum.Clear();

				Assert::AreEqual((unsigned int)0, MyDatum.Size());
			}

			// Is external
			{
				float Float = 3;
				Datum Datum1(Datum::FLOAT);
				Datum1.StoreExternal(&Float, 1);

				auto Lamda = [&Datum1] { Datum1.Clear(); };
				Assert::ExpectException<std::runtime_error>(Lamda);
			}
		}

		TEST_METHOD(SetByValue)
		{
			// Set before type set and empty 
			{
				Datum MyDatum;
				Assert::IsFalse(MyDatum.Set(0));
			}

			// Int
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::INT);

				// Set while empty
				Assert::IsFalse(MyDatum.Set(1));

				// Set wrong type
				Assert::IsFalse(MyDatum.Set(1));

				// Set out of bounds
				MyDatum.PushBack(1);
				Assert::IsFalse(MyDatum.Set(5, 1));

				// Test valid set
				int ExpectedValue = 9;
				Assert::IsTrue(MyDatum.Set(ExpectedValue));
				Assert::AreEqual(ExpectedValue, MyDatum.GetInt(0));
			}

			// Float
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::FLOAT);

				// Set while empty
				Assert::IsFalse(MyDatum.Set((float)1));

				// Set wrong type
				Assert::IsFalse(MyDatum.Set((float)1));

				// Set out of bounds
				MyDatum.PushBack(1.0f);
				Assert::IsFalse(MyDatum.Set((float)1, 5));

				// Test valid set
				float ExpectedValue = 9.0f;
				Assert::IsTrue(MyDatum.Set(ExpectedValue));
				Assert::AreEqual(ExpectedValue, MyDatum.GetFloat(0));
			}

			// String
			{
				std::string TestString1 = "Test1";

				Datum MyDatum;

				// Set wrong type
				Assert::IsFalse(MyDatum.Set("Test"));

				MyDatum.SetType(Datum::STRING);

				// Set while empty
				Assert::IsFalse(MyDatum.Set(TestString1));

				// Set out of bounds
				MyDatum.PushBack(TestString1);
				Assert::IsFalse(MyDatum.Set(TestString1, 5));

				// Test valid set
				std::string ExpectedValue = std::string("Expected");
				Assert::IsTrue(MyDatum.Set(ExpectedValue));
				Assert::AreEqual(ExpectedValue, MyDatum.GetString(0));
			}

			// Vec4
			{
				Datum MyDatum;

				// Set wrong type
				Assert::IsFalse(MyDatum.Set(glm::vec4(4)));

				MyDatum.SetType(Datum::VEC4);

				// Set while empty
				Assert::IsFalse(MyDatum.Set(glm::vec4(1)));

				// Set wrong type
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(MyDatum.Set(glm::vec4(1.0f)));

				// Set out of bounds
				MyDatum.PushBack(glm::vec4(1));
				Assert::IsFalse(MyDatum.Set(glm::vec4(2), 5));

				// Test valid set
				glm::vec4 ExpectedValue = glm::vec4(9);
				Assert::IsTrue(MyDatum.Set(ExpectedValue));
				Assert::AreEqual(ExpectedValue, MyDatum.GetVec4(0));
			}

			// Mat4x4
			{
				Datum MyDatum;

				// Set wrong type
				Assert::IsFalse(MyDatum.Set(glm::mat4x4(1)));

				MyDatum.SetType(Datum::MAT4X4);

				// Set while empty
				Assert::IsFalse(MyDatum.Set(glm::mat4x4(1)));

				// Set wrong type
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(MyDatum.Set(glm::mat4x4(1.0f)));

				// Set out of bounds
				MyDatum.PushBack(glm::mat4x4(1));
				Assert::IsFalse(MyDatum.Set(glm::mat4x4(2), 5));

				// Test valid set
				glm::mat4x4 ExpectedValue = glm::mat4x4(9);
				Assert::IsTrue(MyDatum.Set(ExpectedValue));
				Assert::AreEqual(ExpectedValue, MyDatum.GetMat4x4(0));
			}

			// Scope/Scope
			{
				Scope Scope1;
				Scope1.Append("D1").PushBack(5);

				Datum MyDatum;



				// Set wrong type
				Assert::IsFalse(MyDatum.Set(1.0f));

				MyDatum.SetType(Datum::SCOPE);

				// Set while empty
				Assert::IsFalse(MyDatum.Set(&Scope1));

				// Set wrong type
				Assert::IsFalse(MyDatum.Set(1));

				// Set out of bounds
				MyDatum.PushBack(&Scope1);
				Assert::IsFalse(MyDatum.Set(&Scope1, 5));

				// Test valid set
				Assert::IsTrue(MyDatum.Set(&Scope1));
				Assert::AreSame(Scope1, *MyDatum.GetScope(0));
			}

			// Pointer
			{
				RTTI* R1 = new Empty();

				Datum MyDatum;
				Scope* S1 = nullptr;

				// Set wrong type
				Assert::IsFalse(MyDatum.Set(S1));

				MyDatum.SetType(Datum::POINTER);

				// Set while empty
				Assert::IsFalse(MyDatum.Set(R1));

				// Set out of bounds
				MyDatum.PushBack(R1);
				Assert::IsFalse(MyDatum.Set(R1, 5));

				// Test valid set
				Assert::IsTrue(MyDatum.Set(R1));
				Assert::AreSame(*R1, *MyDatum.GetPointer(0));

				delete R1;
			}
		}

		TEST_METHOD(SetSerializedItem)
		{
			std::string SerializedInt = std::string("4");

			// Set while UKNOWN
			{
				Datum D1;
				Assert::IsFalse(D1.SetSerializedItem(std::string("Word")));
			}

			// Test invalid type
			{
				Datum D1;
				Scope S1;
				D1.PushBack(&S1);
				Assert::ExpectException<std::runtime_error>([&D1] { D1.SetSerializedItem("Word"); });


				Datum D2;
				RTTI* R1 = new Empty();
				D2.PushBack(&S1);
				Assert::ExpectException<std::runtime_error>([&D2] { D2.SetSerializedItem("Word"); });
				delete R1;
			}

			// Test invalid size
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::INT);
				Assert::IsFalse(MyDatum.SetSerializedItem(SerializedInt, 1));
			}

			// Int
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::INT);
				MyDatum.PushBack(0);

				Assert::IsTrue(MyDatum.SetSerializedItem(SerializedInt));

				Assert::AreEqual(4, MyDatum.GetInt(0));
			}

			// Float
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::FLOAT);
				MyDatum.PushBack(0.0f);

				std::string SerializedFloat = std::string("1.25");
				Assert::IsTrue(MyDatum.SetSerializedItem(SerializedFloat));
				Assert::AreEqual((float)1.25, MyDatum.GetFloat(0));
			}

			// String
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::STRING);
				MyDatum.PushBack(std::string("Test"));

				std::string SerializedString = std::string("Hello World!");
				Assert::IsTrue(MyDatum.SetSerializedItem(SerializedString));
				Assert::AreEqual(SerializedString, MyDatum.GetString(0));
			}

			// Vec4
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::VEC4);
				MyDatum.PushBack(glm::vec4(0));

				std::string SerializedVec4 = SerializeVec4(glm::vec4(0.0f, 1.0f, 2.0f, 3.0f));

				Assert::IsTrue(MyDatum.SetSerializedItem(SerializedVec4));

				Assert::AreEqual(glm::vec4(0.0f, 1.0f, 2.0f, 3.0f), MyDatum.GetVec4());
			}


			// Mat4x4
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::MAT4X4);
				MyDatum.PushBack(glm::mat4x4(0));

				glm::vec4 V(0.0f, 1.0f, 2.0f, 3.0f);

				std::string SerializedMat = SerializeMat4x4(glm::mat4x4(V, V, V, V));

				Assert::IsTrue(MyDatum.SetSerializedItem(SerializedMat));

				Assert::AreEqual(glm::mat4x4(V, V, V, V), MyDatum.GetMat4x4());
			}
		}

		TEST_METHOD(GetSerializedItem)
		{
			// Test invalid type
			{
				Datum MyDatum;
				Scope S1;
				MyDatum.PushBack(&S1);
				MyDatum.SetType(Datum::SCOPE);
				Assert::ExpectException<std::runtime_error>([&MyDatum] { MyDatum.GetSerializedItem(); });


				Datum D2;
				RTTI* R1 = new Empty();
				D2.PushBack(&S1);
				Assert::ExpectException<std::runtime_error>([&D2] { D2.GetSerializedItem(); });
				delete R1;
			}

			// Invalid index
			{
				Datum MyDatum;

				auto BelowRangeLamda = [&MyDatum] { MyDatum.GetSerializedItem(-1); };
				auto BeyondRangeLamda = [&MyDatum] { MyDatum.GetSerializedItem(100); };

				Assert::ExpectException<std::out_of_range>(BelowRangeLamda);
				Assert::ExpectException<std::out_of_range>(BeyondRangeLamda);
			}

			// Int
			{
				Datum MyDatum;
				MyDatum.PushBack(1);
				MyDatum.PushBack(5329);
				Assert::AreEqual(std::string("1"), MyDatum.GetSerializedItem());
				Assert::AreEqual(std::string("5329"), MyDatum.GetSerializedItem(1));
			}

			// Float
			{
				Datum MyDatum;
				MyDatum.PushBack(1.0f);
				MyDatum.PushBack(5.329f);
				Assert::AreEqual(std::to_string(1.0f), MyDatum.GetSerializedItem());
				Assert::AreEqual(std::to_string(5.329f), MyDatum.GetSerializedItem(1));
			}

			// String
			{
				Datum MyDatum;
				MyDatum.PushBack("1");
				MyDatum.PushBack("5329");
				Assert::AreEqual(std::string("1"), MyDatum.GetSerializedItem());
				Assert::AreEqual(std::string("5329"), MyDatum.GetSerializedItem(1));
			}

			// Vec4
			{
				Datum MyDatum;
				MyDatum.PushBack(glm::vec4(1.0f));
				MyDatum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				Assert::AreEqual(SerializeVec4(glm::vec4(1.0f)), MyDatum.GetSerializedItem());
				Assert::AreEqual(SerializeVec4(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f)), MyDatum.GetSerializedItem(1));
			}

			// Mat4x4
			{
				Datum MyDatum;

				glm::mat4x4 Mat1 = glm::mat4x4(1.0f);
				glm::vec4 V = glm::vec4(1.0f, 2.0f, 3.0f, 4.445f);
				glm::mat4x4 Mat2 = glm::mat4x4(V, V, V, V);

				MyDatum.PushBack(Mat1);
				MyDatum.PushBack(Mat2);
				Assert::AreEqual(SerializeMat4x4(Mat1), MyDatum.GetSerializedItem());
				Assert::AreEqual(SerializeMat4x4(Mat2), MyDatum.GetSerializedItem(1));
			}
		}

		TEST_METHOD(ReserveAndAutoResize_Coverage)
		{
			Datum MyDatum;
			MyDatum.SetType(Datum::FLOAT);

			MyDatum.PushBack(float(1));

			for (int i = 0; i < 1000; ++i)
			{
				MyDatum.PushBack(float(1));
			}
		}

		TEST_METHOD(Resize)
		{
			// NewSize == mSize
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::INT);

				// mSize = 0
				Assert::IsTrue(MyDatum.Resize(0));
				Assert::AreEqual((unsigned int)0, MyDatum.Size());

				// mSize = 3
				MyDatum.PushBack(1);
				MyDatum.PushBack(2);
				MyDatum.PushBack(3);

				Assert::IsTrue(MyDatum.Resize(3));
				Assert::AreEqual((unsigned int)3, MyDatum.Size());
			}

			// NewSize < mSize
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::INT);

				// NewSize = 0 with mSize = 3
				MyDatum.PushBack(1);
				MyDatum.PushBack(2);
				MyDatum.PushBack(3);

				Assert::IsTrue(MyDatum.Resize(0));
				Assert::AreEqual((unsigned int)0, MyDatum.Size());

				// NewSize = 1 with mSize = 4
				MyDatum.PushBack(1);
				MyDatum.PushBack(2);
				MyDatum.PushBack(3);
				MyDatum.PushBack(4);

				Assert::IsTrue(MyDatum.Resize(1));
				Assert::AreEqual((unsigned int)1, MyDatum.Size());
				Assert::AreEqual(1, MyDatum.GetInt(0));
			}

			// NewSize > mSize
			{
				Datum MyDatum;
				MyDatum.SetType(Datum::INT);

				// mSize = 0
				Assert::IsTrue(MyDatum.Resize(5));
				Assert::AreEqual((unsigned int)5, MyDatum.Size());

				// Push from end
				Assert::IsTrue(MyDatum.PushBack(1));
				Assert::AreEqual((unsigned int)6, MyDatum.Size());
				Assert::AreEqual(1, MyDatum.GetInt(5));
			}

			// Is external
			{
				float Float = 3;
				Datum Datum1(Datum::FLOAT);
				Datum1.StoreExternal(&Float, 1);
				Assert::IsFalse(Datum1.Resize(5));
			}
		}

		TEST_METHOD(CopyConstructor)
		{
			// Other._Type = UNKNOWN
			{
				Datum Datum1;
				Datum Datum2(Datum1);

				Assert::AreEqual(Datum::UKNOWN, Datum2.GetType());
			}

			// Other is external 
			{
				float Float = 3;
				Datum Datum1(Datum::FLOAT);
				Datum1.StoreExternal(&Float, 1);

				Datum Datum2(Datum1);
			}

			// Int
			{
				Datum Datum1;
				Datum1.SetType(Datum::INT);

				// Check type is set and size = 0
				Datum Datum2(Datum1);
				Assert::AreEqual(Datum::INT, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(0);
				Datum1.PushBack(1);
				Datum1.PushBack(2);

				Datum Datum3(Datum1);
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual(0, Datum3.GetInt());
				Assert::AreEqual(2, Datum3.GetInt(2));

				// Check deep copy
				Datum1.Set(7);
				Assert::AreEqual(7, Datum1.GetInt());
				Assert::AreEqual(0, Datum3.GetInt());
			}

			// Float
			{
				Datum Datum1;
				Datum1.SetType(Datum::FLOAT);

				// Check type is set and size = 0
				Datum Datum2(Datum1);
				Assert::AreEqual(Datum::FLOAT, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(0.0f);
				Datum1.PushBack(1.0f);
				Datum1.PushBack(2.0f);

				Datum Datum3(Datum1);
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual((float)0, Datum3.GetFloat());
				Assert::AreEqual((float)2, Datum3.GetFloat(2));

				// Check deep copy
				Datum1.Set((float)7);
				Assert::AreEqual((float)7, Datum1.GetFloat());
				Assert::AreEqual((float)0, Datum3.GetFloat());
			}

			// String
			{
				Datum Datum1;
				Datum1.SetType(Datum::STRING);

				// Check type is set and size = 0
				Datum Datum2(Datum1);
				Assert::AreEqual(Datum::STRING, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(std::string("0"));
				Datum1.PushBack(std::string("1"));
				Datum1.PushBack(std::string("2"));

				Datum Datum3(Datum1);
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual(std::string("0"), Datum3.GetString());
				Assert::AreEqual(std::string("2"), Datum3.GetString(2));

				// Check deep copy
				Datum1.Set(std::string("7"));
				Assert::AreEqual(std::string("7"), Datum1.GetString());
				Assert::AreEqual(std::string("0"), Datum3.GetString());
			}

			// Vec4
			{
				glm::vec4 V1 = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);

				Datum Datum1;
				Datum1.SetType(Datum::VEC4);

				// Check type is set and size = 0
				Datum Datum2(Datum1);
				Assert::AreEqual(Datum::VEC4, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(V1);
				Datum1.PushBack(V1);
				Datum1.PushBack(V1);

				Datum Datum3(Datum1);
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual(V1, Datum3.GetVec4(0));

				// Check deep copy
				glm::vec4 V2 = glm::vec4(7.0f);
				Datum1.Set(V2);
				Assert::AreEqual(V2, Datum1.GetVec4());
				Assert::AreEqual(V1, Datum3.GetVec4());
			}

			// Mat4x4
			{
				glm::mat4x4 M1 = glm::mat4x4(1.0f);

				Datum Datum1;
				Datum1.SetType(Datum::MAT4X4);

				// Check type is set and size = 0
				Datum Datum2(Datum1);
				Assert::AreEqual(Datum::MAT4X4, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(M1);
				Datum1.PushBack(M1);
				Datum1.PushBack(M1);


				Datum Datum3(Datum1);
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual(M1, Datum3.GetMat4x4());
				Assert::AreEqual(M1, Datum3.GetMat4x4(2));

				// Check deep copy
				glm::mat4x4 M2 = glm::mat4x4(7.0f);
				Datum1.Set(M2);
				Assert::AreEqual(M2, Datum1.GetMat4x4());
				Assert::AreEqual(M1, Datum3.GetMat4x4());
			}
		}

		TEST_METHOD(AssignmentOp)
		{
			// Other._Type = UNKNOWN
			{
				Datum Datum1;
				Datum Datum2;
				Datum2 = Datum1;

				Assert::AreEqual(Datum::UKNOWN, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());
			}

			// Other is external 
			{
				float Float = 3;
				Datum Datum1(Datum::FLOAT);
				Datum1.StoreExternal(&Float, 1);

				Datum Datum2;
				Datum2 = Datum1;
			}

			// Int
			{
				Datum Datum1;
				Datum1.SetType(Datum::INT);

				// Check type is set and size = 0
				Datum Datum2;
				Datum2 = Datum1;

				Assert::AreEqual(Datum::INT, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(0);
				Datum1.PushBack(1);
				Datum1.PushBack(2);

				// Check old data is cleared
				Datum Datum3;
				Datum3.PushBack(9);
				Datum3.PushBack(8);

				Datum3 = Datum1;
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual(0, Datum3.GetInt());
				Assert::AreEqual(2, Datum3.GetInt(2));

				// Check deep copy
				Datum1.Set(7);
				Assert::AreEqual(7, Datum1.GetInt());
				Assert::AreEqual(0, Datum3.GetInt());
			}


			// Float
			{
				Datum Datum1;
				Datum1.SetType(Datum::FLOAT);

				// Check type is set and size = 0
				Datum Datum2;
				Datum2 = Datum1;
				Assert::AreEqual(Datum::FLOAT, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(0.0f);
				Datum1.PushBack(1.0f);
				Datum1.PushBack(2.0f);

				// Check old data is cleared
				Datum Datum3;
				Datum3.PushBack(9.0f);
				Datum3.PushBack(8.0f);

				Datum3 = Datum1;
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual((float)0, Datum3.GetFloat(0));
				Assert::AreEqual((float)2, Datum3.GetFloat(2));

				// Check deep copy
				Datum1.Set((float)7);
				Assert::AreEqual((float)7, Datum1.GetFloat());
				Assert::AreEqual((float)0, Datum3.GetFloat());
			}

			// String
			{
				Datum Datum1;
				Datum1.SetType(Datum::STRING);

				// Check type is set and size = 0
				Datum Datum2;
				Datum2 = Datum1;
				Assert::AreEqual(Datum::STRING, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(std::string("0"));
				Datum1.PushBack(std::string("1"));
				Datum1.PushBack(std::string("2"));

				// Check old data is cleared
				Datum Datum3;
				Datum3.PushBack(std::string("A"));
				Datum3.PushBack(std::string("B"));

				Datum3 = Datum1;
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual(std::string("0"), Datum3.GetString());
				Assert::AreEqual(std::string("2"), Datum3.GetString(2));

				// Check deep copy
				Datum1.Set(std::string("7"));
				Assert::AreEqual(std::string("7"), Datum1.GetString());
				Assert::AreEqual(std::string("0"), Datum3.GetString());
			}

			// Vec4
			{
				glm::vec4 V1 = glm::vec4(1.0f);

				Datum Datum1;
				Datum1.SetType(Datum::VEC4);

				// Check type is set and size = 0
				Datum Datum2;
				Datum2 = Datum1;

				Assert::AreEqual(Datum::VEC4, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(V1);
				Datum1.PushBack(V1);
				Datum1.PushBack(V1);


				// Check old data is cleared
				glm::vec4 V2 = glm::vec4(2.0f);

				Datum Datum3;
				Datum3.PushBack(V2);
				Datum3.PushBack(V2);

				Datum3 = Datum1;
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual(V1, Datum3.GetVec4());

				// Check deep copy
				glm::vec4 V3 = glm::vec4(3.0f);
				Datum1.Set(V3);
				Assert::AreEqual(V3, Datum1.GetVec4());
				Assert::AreEqual(V1, Datum3.GetVec4());
			}

			// Mat4x4
			{
				glm::mat4x4 M1 = glm::mat4x4(1.0f);

				Datum Datum1;
				Datum1.SetType(Datum::MAT4X4);

				// Check type is set and size = 0
				Datum Datum2;
				Datum2 = Datum1;

				Assert::AreEqual(Datum::MAT4X4, Datum2.GetType());
				Assert::AreEqual((unsigned int)0, Datum2.Size());

				// Check data transfer
				Datum1.PushBack(M1);
				Datum1.PushBack(M1);
				Datum1.PushBack(M1);


				// Check old data is cleared
				glm::mat4x4 M2 = glm::mat4x4(2.0f);

				Datum Datum3;
				Datum3.PushBack(M2);
				Datum3.PushBack(M2);

				Datum3 = Datum1;
				Assert::AreEqual((unsigned int)3, Datum3.Size());
				Assert::AreEqual(M1, Datum3.GetMat4x4());

				// Check deep copy
				glm::mat4x4 M3 = glm::mat4x4(3.0f);
				Datum1.Set(M3);
				Assert::AreEqual(M3, Datum1.GetMat4x4());
				Assert::AreEqual(M1, Datum3.GetMat4x4());
			}
		}

		TEST_METHOD(ComparisonOps)
		{
			// Different types
			{
				Datum Datum1;
				Datum1.SetType(Datum::INT);

				Datum Datum2;
				Datum2.SetType(Datum::FLOAT);

				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);
			}

			// Same types
			{
				Datum Datum1;
				Datum1.SetType(Datum::INT);

				Datum Datum2;
				Datum2.SetType(Datum::INT);

				Assert::IsTrue(Datum1 == Datum2);
				Assert::IsFalse(Datum1 != Datum2);
			}

			// Different sizes
			{
				Datum Datum1;
				Datum1.SetType(Datum::INT);
				Datum1.Resize(5);

				Datum Datum2;
				Datum2.SetType(Datum::INT);

				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);
			}

			// Same sizes
			{
				Datum Datum1;
				Datum1.SetType(Datum::INT);
				Datum1.PushBack(1);
				Datum1.PushBack(1);

				Datum Datum2;
				Datum2.SetType(Datum::INT);
				Datum2.PushBack(1);
				Datum2.PushBack(1);

				Assert::IsTrue(Datum1 == Datum2);
				Assert::IsFalse(Datum1 != Datum2);
			}

			// Int data equality
			{
				Datum Datum1;
				Datum1.PushBack(0);
				Datum1.PushBack(1);
				Datum1.PushBack(2);

				Datum Datum2;

				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);

				Datum2.PushBack(0);
				Datum2.PushBack(1);
				Datum2.PushBack(2);

				Assert::IsTrue(Datum1 == Datum2);
				Assert::IsFalse(Datum1 != Datum2);

				Datum2.Set(10);
				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);
			}


			// Float data equality
			{
				Datum Datum1;
				Datum1.PushBack(0.0f);
				Datum1.PushBack(1.0f);
				Datum1.PushBack(2.0f);

				Datum Datum2;

				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);

				Datum2.PushBack(0.0f);
				Datum2.PushBack(1.0f);
				Datum2.PushBack(2.0f);

				Assert::IsTrue(Datum1 == Datum2);
				Assert::IsFalse(Datum1 != Datum2);

				Datum2.Set(10.0f);
				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);
			}


			// String data equality
			{
				Datum Datum1;
				Datum1.PushBack(std::string("0"));
				Datum1.PushBack(std::string("1"));
				Datum1.PushBack(std::string("2"));

				Datum Datum2;

				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);

				Datum2.PushBack(std::string("0"));
				Datum2.PushBack(std::string("1"));
				Datum2.PushBack(std::string("2"));

				Assert::IsTrue(Datum1 == Datum2);
				Assert::IsFalse(Datum1 != Datum2);

				Datum2.Set(std::string("BLAH"));
				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);
			}

			// Vec4 data equality
			{
				glm::vec4 V1 = glm::vec4(1.0f);
				glm::vec4 V2 = glm::vec4(2.0f);

				Datum Datum1;
				Datum1.PushBack(V1);
				Datum1.PushBack(V2);

				Datum Datum2;

				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);

				Datum2.PushBack(V1);
				Datum2.PushBack(V2);

				Assert::IsTrue(Datum1 == Datum2);
				Assert::IsFalse(Datum1 != Datum2);

				Datum2.Set(glm::vec4(10.0f));
				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);
			}

			// Mat4x4 data equality
			{
				glm::mat4x4 M1 = glm::mat4x4(1.0f);
				glm::mat4x4 M2 = glm::mat4x4(2.0f);

				Datum Datum1;
				Datum1.PushBack(M1);
				Datum1.PushBack(M2);

				Datum Datum2;

				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);

				Datum2.PushBack(M1);
				Datum2.PushBack(M2);

				Assert::IsTrue(Datum1 == Datum2);
				Assert::IsFalse(Datum1 != Datum2);

				Datum2.Set(glm::mat4x4(10.0f));
				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);
			}

			// Scope data equality
			{
				Scope S1;
				S1.Append("1").PushBack(5);
				Scope S2;
				S2.Append("2").PushBack(10);

				Datum Datum1;
				Datum1.PushBack(&S1);
				Datum1.PushBack(&S2);

				Datum Datum2;

				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);

				Datum2.PushBack(&S1);
				Datum2.PushBack(&S2);

				Assert::IsTrue(Datum1 == Datum2);
				Assert::IsFalse(Datum1 != Datum2);

				Datum2.Set(&S2);
				Assert::IsFalse(Datum1 == Datum2);
				Assert::IsTrue(Datum1 != Datum2);
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			// Move constructor
			{
				Datum Datum1 = std::move<Datum>(Datum(Datum::INT));
				Assert::AreEqual(Datum::INT, Datum1.GetType());
			}

			// Move assignment
			{
				Datum Datum1(Datum::FLOAT);
				Datum1.PushBack(0.0f);
				Datum1.PushBack(1.0f);
				Datum1.PushBack(2.0f);

				Datum Datum2(Datum::FLOAT);
				Datum2 = std::move(Datum1);

				Assert::AreEqual(Datum::FLOAT, Datum2.GetType());
				Assert::AreEqual((unsigned int)3, Datum2.Size());
				Assert::AreEqual(0.0f, Datum2.GetFloat());
				Assert::AreEqual(2.0f, Datum2.GetFloat(2));

				Datum Datum3(Datum::INT);
				Datum Datum4(Datum::STRING);
				auto DiffTypeLambda = [&Datum3, &Datum4] { Datum4 = std::move(Datum3); };
				Assert::ExpectException<std::runtime_error>(DiffTypeLambda);
			}
		}

		TEST_METHOD(ExternalStorage)
		{
			// Int
			{
				Datum Datum1;
				int ExternalData[2] = { 0, 1 };
				int* NullArray = nullptr;

				// Null storage
				Assert::IsFalse(Datum1.StoreExternal(NullArray, 2));

				// Wrong type
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(Datum2.StoreExternal(ExternalData, 2));

				// Valid store
				Assert::IsTrue(Datum1.StoreExternal(ExternalData, 2));

				Assert::AreEqual((unsigned int)2, Datum1.Size());
				Assert::AreEqual(1, Datum1.GetInt(1));

				// Invalid: double store
				Assert::IsFalse(Datum1.StoreExternal(ExternalData, 2));
			}

			// Float
			{
				Datum Datum1;
				float ExternalData[2] = { 0.0f, 1.0f };
				float* NullArray = nullptr;

				// Null storage
				Assert::IsFalse(Datum1.StoreExternal(NullArray, 2));

				// Wrong type
				Datum Datum2(Datum::INT);
				Assert::IsFalse(Datum2.StoreExternal(ExternalData, 2));

				// Valid store
				Assert::IsTrue(Datum1.StoreExternal(ExternalData, 2));

				Assert::AreEqual((unsigned int)2, Datum1.Size());
				Assert::AreEqual(1.0f, Datum1.GetFloat(1));

				// Invalid: double store
				Assert::IsFalse(Datum1.StoreExternal(ExternalData, 2));
			}

			// String
			{
				Datum Datum1;
				std::string ExternalData[2] = { "0", "1" };
				std::string* NullArray = nullptr;

				// Null storage
				Assert::IsFalse(Datum1.StoreExternal(NullArray, 2));

				// Wrong type
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(Datum2.StoreExternal(ExternalData, 2));

				// Valid store
				Assert::IsTrue(Datum1.StoreExternal(ExternalData, 2));

				Assert::AreEqual((unsigned int)2, Datum1.Size());
				Assert::AreEqual(std::string("1"), Datum1.GetString(1));

				// Invalid: double store
				Assert::IsFalse(Datum1.StoreExternal(ExternalData, 2));
			}

			// Vec4
			{
				Datum Datum1;
				glm::vec4 ExternalData[2] = { glm::vec4(0.0f), glm::vec4(1.0f) };
				glm::vec4* NullArray = nullptr;

				// Null storage
				Assert::IsFalse(Datum1.StoreExternal(NullArray, 2));

				// Wrong type
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(Datum2.StoreExternal(ExternalData, 2));

				// Valid store
				Assert::IsTrue(Datum1.StoreExternal(ExternalData, 2));

				Assert::AreEqual((unsigned int)2, Datum1.Size());
				Assert::AreEqual(glm::vec4(1.0f), Datum1.GetVec4(1));

				// Invalid: double store
				Assert::IsFalse(Datum1.StoreExternal(ExternalData, 2));
			}


			// Mat3x3
			{
				Datum Datum1;
				glm::mat4x4 ExternalData[2] = { glm::mat4x4(0.0f), glm::mat4x4(1.0f) };
				glm::mat4x4* NullArray = nullptr;

				// Null storage
				Assert::IsFalse(Datum1.StoreExternal(NullArray, 2));

				// Wrong type
				Datum Datum2(Datum::FLOAT);
				Assert::IsFalse(Datum2.StoreExternal(ExternalData, 2));

				// Valid store
				Assert::IsTrue(Datum1.StoreExternal(ExternalData, 2));

				Assert::AreEqual((unsigned int)2, Datum1.Size());
				Assert::AreEqual(glm::mat4x4(1.0f), Datum1.GetMat4x4(1));

				// Invalid: double store
				Assert::IsFalse(Datum1.StoreExternal(ExternalData, 2));
			}
		}

		TEST_METHOD(RemoveScopeAt)
		{
			// Invalid cases
			{
				Datum Datum1(Datum::FLOAT);
				Assert::ExpectException<std::runtime_error>(
					[&Datum1] { Scope* S = Datum1.RemoveScopeAt(0); });

				Datum Datum2(Datum::SCOPE);
				Assert::ExpectException<std::out_of_range>(
					[&Datum2] { Scope* S = Datum2.RemoveScopeAt(1000); });
			}

			// Valid cases
			{
				Scope Scope1;

				Datum& D1 = Scope1.Append("D1");

				Scope& Scope2 = Scope1.AppendScope("D1");
				Scope& Scope3 = Scope1.AppendScope("D1");
				Scope& Scope4 = Scope1.AppendScope("D1");

				Scope* RemovedScope = D1.RemoveScopeAt(1);
				Scope& RemovedScopeRef = *RemovedScope;
				Assert::IsNotNull(RemovedScope);
				Assert::AreEqual(Scope2, RemovedScopeRef);
				delete RemovedScope;

				Assert::AreEqual((unsigned int)2, D1.Size());
			}

		}

		TEST_METHOD(BracketOp)
		{
			// Wrong type
			{
				Datum D1(Datum::INT);
				D1.PushBack(1);
				Assert::ExpectException<std::runtime_error>([&D1] { D1[0]; });
			}

			// Out of bounds
			{
				Scope S1;
				Datum D1(Datum::SCOPE);
				D1.PushBack(&S1);
				Assert::ExpectException<std::out_of_range>([&D1] { D1[5]; });
			}

			// Valid
			{
				Scope S1;
				S1.Append("D").PushBack(5.0f);

				Datum D1(Datum::SCOPE);
				D1.PushBack(&S1);
				Scope& Scope2 = D1[0];

				Assert::AreSame(S1, Scope2);
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}