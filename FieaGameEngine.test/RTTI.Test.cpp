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
	TEST_CLASS(RTTITests)
	{
		TEST_METHOD(Is)
		{
			EmptyChild Ec;

			Assert::IsTrue(Ec.Is(EmptyChild::TypeIdClass()));
			Assert::IsTrue(Ec.Is(Empty::TypeIdClass()));

			Assert::IsFalse(Ec.Is(Foo::TypeIdClass()));
		}

		TEST_METHOD(As)
		{
			EmptyChild Ec;

			Assert::IsNotNull(Ec.As<EmptyChild>());
			Assert::IsNotNull(Ec.As<Empty>());

			Assert::IsNull(Ec.As<Foo>());
		}
	};
}