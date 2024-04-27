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
using namespace Fiea::GameEngine::Test;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Fiea::GameEngine::Test
{
	TEST_CLASS(ScopeTests)
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


		TEST_METHOD(Constructor)
		{
			Scope Scope1;
			Assert::IsNull(Scope1.GetParent());

			Scope& Scope2 = Scope1.AppendScope("Scope2");
			Assert::AreSame(Scope1, *Scope2.GetParent());
		}

		TEST_METHOD(AppendAndFind)
		{
			Scope Scope1;

			// Find when empty
			Assert::IsNull(Scope1.Find("Key"));

			Datum& Datum1 = Scope1.Append("D1");

			// Check is default constructed
			Assert::AreEqual(Datum::UKNOWN, Datum1.GetType());
			Assert::AreEqual((unsigned int)0, Datum1.Size());

			// Check reference is valid with find
			Datum1.SetType(Datum::FLOAT);
			Datum1.PushBack(5.0f);

			Datum* Datum1Ptr = Scope1.Find("D1");

			Assert::IsNotNull(Datum1Ptr);
			Assert::AreEqual(Datum::FLOAT, Datum1Ptr->GetType());
			Assert::AreEqual((unsigned int)1, Datum1Ptr->Size());
			Assert::AreEqual(5.0f, Datum1Ptr->GetFloat());

			// Check append retrieval
			Assert::AreEqual(Datum1, Scope1.Append("D1"));
		}

		TEST_METHOD(BracketOps)
		{
			// String input bracket op: basically just Append tests
			{
				Scope Scope1;

				// Find when empty
				Assert::IsNull(Scope1.Find(std::string("Key")));

				Datum& Datum1 = Scope1["D1"];

				// Check is default constructed
				Assert::AreEqual(Datum::UKNOWN, Datum1.GetType());
				Assert::AreEqual((unsigned int)0, Datum1.Size());

				// Check reference is valid with find
				Datum1.SetType(Datum::FLOAT);
				Datum1.PushBack(5.0f);

				Datum* Datum1Ptr = Scope1.Find("D1");

				Assert::IsNotNull(Datum1Ptr);
				Assert::AreEqual(Datum::FLOAT, Datum1Ptr->GetType());
				Assert::AreEqual((unsigned int)1, Datum1Ptr->Size());
				Assert::AreEqual(5.0f, Datum1Ptr->GetFloat());

				// Check append retrieval
				Assert::AreEqual(Datum1, Scope1["D1"]);
			}

			// Integer input bracket op
			{
				Scope Scope1;

				Datum& Datum1 = Scope1.Append("D1");
				Datum& Datum2 = Scope1.Append("D2");
				Datum& Datum3 = Scope1.Append("D3");

				Datum1.SetType(Datum::INT);
				Datum2.SetType(Datum::FLOAT);
				Datum3.SetType(Datum::STRING);

				Datum& D1 = Scope1[0];
				Datum& D2 = Scope1[1];
				Datum& D3 = Scope1[2];

				Assert::AreEqual(Datum::INT, D1.GetType());
				Assert::AreEqual(Datum::FLOAT, D2.GetType());
				Assert::AreEqual(Datum::STRING, D3.GetType());

				Assert::ExpectException<std::out_of_range>([&Scope1] { Scope1[100]; });
			}
		}

		TEST_METHOD(IsAncestorOf)
		{
			// Null other
			{
				Scope S1;
				Assert::IsFalse(S1.IsAncestorOf(nullptr));
			}

			// Is ancestor of self?
			{
				Scope S1;
				Assert::IsFalse(S1.IsAncestorOf(&S1));
			}

			// Valid case
			{
				Scope S1;
				Scope& S2 = S1.AppendScope("2");
				Scope& S3 = S2.AppendScope("3");

				Assert::IsTrue(S1.IsAncestorOf(&S3));
			}
		}

		TEST_METHOD(IsDescentandOf)
		{
			// Null other
			{
				Scope S1;
				Assert::IsFalse(S1.IsDescendantOf(nullptr));
			}

			// Is ancestor of self?
			{
				Scope S1;
				Assert::IsFalse(S1.IsDescendantOf(&S1));
			}

			// Valid case
			{
				Scope S1;
				Scope& S2 = S1.AppendScope("2");
				Scope& S3 = S2.AppendScope("3");

				Assert::IsTrue(S3.IsDescendantOf(&S1));
			}
		}

		TEST_METHOD(AppendScope)
		{
			// Singly nested
			{
				Scope S1;
				Scope& S2 = S1.AppendScope("S2");

				Assert::AreEqual(S1, *S2.GetParent());

				// Make into non-default scope
				Datum& D1 = S2.Append("D1");
				D1.PushBack(5.0f);

				Datum* FoundDatum = S1.Find("S2");
				Assert::AreEqual(Datum::SCOPE, FoundDatum->GetType());
				Assert::AreEqual((unsigned int)1, FoundDatum->Size());
				Assert::AreEqual(S2, *FoundDatum->GetScope(0));
			}

			// Doubly nested
			{
				Scope S1;
				Scope& S2 = S1.AppendScope("S2");
				Scope& S3 = S2.AppendScope("S3");

				Assert::AreEqual(S2, *S3.GetParent());

				// Make into non-default scope
				Datum& D1 = S3.Append("D1");
				D1.PushBack(5.0f);

				Datum* FoundDatum = S2.Find("S3");
				Assert::AreEqual(Datum::SCOPE, FoundDatum->GetType());
				Assert::AreEqual((unsigned int)1, FoundDatum->Size());
				Assert::AreEqual(S3, *FoundDatum->GetScope());
			}
		}

		TEST_METHOD(ComparisonOps)
		{
			// Unequal sizes
			{
				Scope S1;
				S1.Append("D1").SetType(Datum::INT);
				S1.Append("D2").SetType(Datum::FLOAT);
				S1.Append("D3").SetType(Datum::STRING);

				Scope S2;
				S1.Append("D1").SetType(Datum::INT);

				Assert::IsFalse(S1 == S2);
				Assert::IsTrue(S1 != S2);
			}

			// Unequal orders
			{
				Scope S1;
				S1.Append("D1").SetType(Datum::INT);
				S1.Append("D2").SetType(Datum::FLOAT);
				S1.Append("D3").SetType(Datum::STRING);

				Scope S2;
				S2.Append("D2").SetType(Datum::FLOAT);
				S2.Append("D1").SetType(Datum::INT);
				S2.Append("D3").SetType(Datum::STRING);

				Assert::IsFalse(S1 == S2);
				Assert::IsTrue(S1 != S2);
			}

			// Unequal values
			{
				Scope S1;
				S1.Append("D1").PushBack(5);
				S1.Append("D2").PushBack(10);
				S1.Append("D3").PushBack(15);


				Scope S2;
				S2.Append("D1").PushBack(5);
				S2.Append("D2").PushBack(10);
				S2.Append("D3").PushBack(20);

				Assert::IsFalse(S1 == S2);
				Assert::IsTrue(S1 != S2);
			}

			// Equal
			{
				Scope S1;
				S1.Append("D1").SetType(Datum::INT);
				S1.Append("D2").SetType(Datum::FLOAT);
				S1.Append("D3").SetType(Datum::STRING);

				Scope S2;
				S2.Append("D1").SetType(Datum::INT);
				S2.Append("D2").SetType(Datum::FLOAT);
				S2.Append("D3").SetType(Datum::STRING);

				Assert::IsTrue(S1 == S2);
				Assert::IsFalse(S1 != S2);
			}
		}

		TEST_METHOD(FindContainedScope)
		{
			// Nullptr scope to find
			{
				Scope S1;
				unsigned int Index = -1;
				Assert::IsNull(S1.FindContainedScope(nullptr, Index));
				Assert::AreEqual((unsigned int)-1, Index);
			}

			// Singly nested scope
			{
				Scope S1;

				Scope& S2 = S1.AppendScope("S2");
				S2.Append("D1").PushBack(5.0f);
				Scope& S3 = S1.AppendScope("S3");

				unsigned int Index = -1;
				Datum* FoundDatum = S1.FindContainedScope(&S2, Index);
				Assert::IsNotNull(FoundDatum);
				Assert::AreEqual((unsigned int)0, Index);
				Assert::AreEqual(S2, *FoundDatum->GetScope(Index));
			}

			// Singly nested scope, array of scopes
			{
				Scope S1;

				Scope& S2A = S1.AppendScope("S2");
				Scope& S2B = S1.AppendScope("S2");
				S2B.Append("D1").PushBack(5.0f);

				unsigned int Index = -1;
				Datum* FoundDatum = S1.FindContainedScope(&S2B, Index);
				Assert::IsNotNull(FoundDatum);
				Assert::AreEqual((unsigned int)1, Index);
				Assert::AreEqual(S2B, *FoundDatum->GetScope(Index));
			}

			// Doubly nested scope
			{
				Scope S1;

				Scope& S2 = S1.AppendScope("S2");

				Scope& S2A = S2.AppendScope("S2A");
				Scope& S2B = S2.AppendScope("S2B");
				S2B.Append("D1").PushBack(10.f);

				unsigned int Index = -1;
				Datum* FoundDatum = S1.FindContainedScope(&S2B, Index);
				Assert::IsNotNull(FoundDatum);
				Assert::AreEqual((unsigned int)0, Index);
				Assert::AreEqual(S2B, *FoundDatum->GetScope(Index));
			}
		}

		TEST_METHOD(FindNameOfContainedScope)
		{
			// Nullptr scope to find
			{
				Scope S1;
				std::string Name = std::string("NotSet");
				bool bFoundScope = S1.FindNameOfContainedScope(nullptr, Name);

				Assert::IsFalse(bFoundScope);
				Assert::AreEqual(std::string("NotSet"), Name);
			}

			// Not found
			{
				Scope S1;

				Scope& S2 = S1.AppendScope("S2");
				S2.Append("D1").PushBack(5.0f);

				Scope S3;

				std::string Name = std::string("NotSet");
				bool bFoundScope = S1.FindNameOfContainedScope(&S3, Name);

				Assert::IsFalse(bFoundScope);
				Assert::AreEqual(std::string("NotSet"), Name);
			}

			// Success
			{
				Scope S1;

				Scope& S2 = S1.AppendScope("S2");
				S2.Append("D1").PushBack(5.0f);
				Scope& S3 = S1.AppendScope("S3");

				std::string Name;
				bool bFoundScope = S1.FindNameOfContainedScope(&S2, Name);
				Assert::IsTrue(bFoundScope);
				Assert::AreEqual(std::string("S2"), Name);
			}
		}

		TEST_METHOD(Search)
		{
			// Not found at all
			{
				Scope S1;
				S1.AppendScope("S2");
				Scope S3;
				S3.Append("D1").PushBack(5.0f);

				Scope* OutScopePtr = nullptr;
				Datum* FoundDatum = S1.Search("S3", &OutScopePtr);
				Assert::IsNull(FoundDatum);
				Assert::IsNull(OutScopePtr);
			}

			// Found in self
			{
				Scope S1;
				S1.AppendScope("S2");
				Scope& S3 = S1.AppendScope("S3");
				S3.Append("D1").PushBack(5.0f);

				Scope* OutScopePtr = nullptr;
				Datum* FoundDatum = S1.Search("S3", &OutScopePtr);

				Assert::AreEqual(*S1.Find("S3"), *FoundDatum);
				Assert::AreEqual(S3, *OutScopePtr);
			}

			// Found in ancestor
			{
				Scope S1;

				Scope& S2 = S1.AppendScope("S2");
				S2.Append("D1").PushBack(2.0f);

				Scope& S3 = S1.AppendScope("S3");
				S3.Append("D1").PushBack(5.0f);

				Scope* OutScopePtr = nullptr;
				Datum* FoundDatum = S3.Search("S2", &OutScopePtr);

				Assert::AreEqual(*S1.Find("S2"), *FoundDatum);
				Assert::AreEqual(S2, *OutScopePtr);
			}
		}

		TEST_METHOD(ConstSearch)
		{
			// Not found at all
			{
				Scope S1;
				S1.AppendScope("S2");
				Scope S3;
				S3.Append("D1").PushBack(5.0f);

				const Scope SCopy(S1);

				Scope* OutScopePtr = nullptr;
				const Datum* FoundDatum = SCopy.Search("S3", &OutScopePtr);
				Assert::IsNull(FoundDatum);
				Assert::IsNull(OutScopePtr);
			}

			// Found in self
			{
				Scope S1;
				S1.AppendScope("S2");
				Scope& S3 = S1.AppendScope("S3");
				S3.Append("D1").PushBack(5.0f);

				const Scope* S1Ptr = &S1;

				Scope* OutScopePtr = nullptr;
				const Datum* SearchedDatum = S1Ptr->Search("S3", &OutScopePtr);
				Assert::IsNotNull(SearchedDatum);

				Datum* FoundDatum = S1.Find("S3");
				Assert::IsNotNull(FoundDatum);
				Assert::AreEqual(*FoundDatum, *SearchedDatum);
				Assert::AreEqual(S3, *OutScopePtr);
			}

			// Found in ancestor
			{
				Scope S1;

				Scope& S2 = S1.AppendScope("S2");
				S2.Append("D1").PushBack(2.0f);

				Scope& S3 = S1.AppendScope("S3");
				S3.Append("D1").PushBack(5.0f);

				const Scope SCopy(S3);

				Scope* OutScopePtr = nullptr;
				const Datum* FoundDatum = SCopy.Search("S2", &OutScopePtr);

				Assert::AreSame(*SCopy.GetParent(), *OutScopePtr);
			}
		}

		TEST_METHOD(AdoptAndOrphan)
		{
			// Adopt self
			{
				Scope S1;
				S1.Adopt("S1", S1);

				unsigned int Index = -1;
				Assert::IsNull(S1.FindContainedScope(&S1, Index));
			}

			// No pre existing parent
			{
				Scope S1;
				S1.Append("D1").PushBack(5);

				Scope S2;
				S2.Append("D2").PushBack("String");

				S1.Adopt("S2", S2);

				Assert::AreSame(S1, *S2.GetParent());
				Assert::AreSame(S2, *S1.Find("S2")->GetScope(0));
			}

			// With pre existing parent, calls orphan
			{
				Scope S1;
				S1.Append("D1").PushBack(5);

				Scope SP;

				Scope& S2 = SP.AppendScope("SP2");
				S2.Append("D2").PushBack("String");

				S1.Adopt("S2", S2);

				Assert::AreSame(S1, *S2.GetParent());
				Assert::AreSame(S2, *S1.Find("S2")->GetScope(0));

				unsigned int Index = -1;
				Assert::IsNull(SP.FindContainedScope(&S2, Index));
			}
		}

		TEST_METHOD(Clone)
		{
			// Tree depth = 1 clone
			{
				Scope S1;

				Datum& D1 = S1.Append("D1");
				D1.PushBack(5.0f);

				Scope* S2 = S1.Clone();

				Assert::AreEqual(S1, *S2);

				delete S2;
			}

			// Tree depth = 2 clone
			{
				Scope S1;

				Scope& S1A = S1.AppendScope("SA");
				Datum& DA = S1A.Append("DA");
				DA.PushBack("Hello");

				Scope* S2 = S1.Clone();

				Assert::AreEqual(S1, *S2);

				delete S2;
			}
		}

		TEST_METHOD(Assignment)
		{
			// Assignment to self
			{
				Scope S1;
				S1 = S1;
			}

			// Check basic equality and that parent is not transferred
			{
				Scope SP;

				Scope& S1 = SP.AppendScope("S1");
				S1.Append("D1").PushBack(5);
				S1.Append("D2").PushBack("Hello");

				Scope S2;
				S1.Append("A").PushBack(10);
				S1.Append("B").PushBack("Bye");

				S2 = S1;

				Assert::AreEqual(S1, S2);
				Assert::IsNull(S2.GetParent());
			}

			// Check nested scopes are deep copied
			{
				Scope S1;

				Scope& SA = S1.AppendScope("SA");
				SA.Append("DA").PushBack(5);

				Scope S2;
				S1.Append("A").PushBack(10);
				S1.Append("B").PushBack("Bye");

				S2 = S1;

				Assert::AreEqual(S1, S2);
				Scope* SACopy = S2.Find("SA")->GetScope(0);
				Assert::AreEqual(SA, *SACopy);
				Assert::AreNotSame(SA, *SACopy);
			}
		}

		TEST_METHOD(CopyConstructor)
		{
			// Check basic equality and that parent is not transferred
			{
				Scope SP;

				Scope& S1 = SP.AppendScope("S1");
				S1.Append("D1").PushBack(5);
				S1.Append("D2").PushBack("Hello");

				Scope S2(S1);

				Assert::AreEqual(S1, S2);
				Assert::IsNull(S2.GetParent());
			}

			// Check nested scopes are deep copied
			{
				Scope S1;

				Scope& SA = S1.AppendScope("SA");
				SA.Append("DA").PushBack(5);

				Scope S2(S1);

				Assert::AreEqual(S1, S2);
				Scope* SACopy = S2.Find("SA")->GetScope(0);
				Assert::AreEqual(SA, *SACopy);
				Assert::AreNotSame(SA, *SACopy);
			}
		}

		TEST_METHOD(CircularReferences)
		{
			// Depth = 2
			{
				Scope S1;
				Scope& S2 = S1.AppendScope("S2");
				S2.Adopt("S1", S1);

				Assert::IsFalse(S1.IsDescendantOf(&S2));
			}

			// Depth = 4
			{
				Scope S1;
				Scope& S2 = S1.AppendScope("S2");
				Scope& S3 = S2.AppendScope("S3");
				S3.Adopt("S1", S1);

				Assert::IsFalse(S1.IsDescendantOf(&S3));
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			// Move construction
			{
				Scope SA;
				Datum& D1 = SA.Append("D1");
				D1.PushBack(5.0f);
				Scope& S1 = SA.AppendScope("D2");

				Scope SB = std::move<Scope>(Scope());
			}

			// Move assignment
			{
				Scope SA;
				Datum& D1 = SA.Append("D1");
				D1.PushBack(5.0f);
				Scope& S1 = SA.AppendScope("D2");

				Scope SB;
				SB = std::move(SA);

				Assert::IsNull(SA.Find("D1"));
				Assert::IsNull(SA.Find("D2"));

				Assert::AreSame(D1, *SB.Find("D1"));
				unsigned int Index = 0;
				Assert::AreSame(S1, *SB.FindContainedScope(&S1, Index)->GetScope(0));
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}