#include "pch.h"
#include "Serialization.h"
#include <codecvt>
#include "CppUnitTest.h"
#include "FieaGameEngine.Test.cpp"

#include "GameClock.h"
#include "Event.h"
#include "EventSubscriber.h"
#include "EventDispatcher.h"

#include "DogSubscriber.h"


using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::Test;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Fiea::GameEngine::Test
{

	TEST_CLASS(EventsTests)
	{
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			// Force create statics to appease memory leak tests
			Event<BonePayload> ThrowBoneEvent;
			Event<TrickPayload> TrickEvent;
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


		TEST_METHOD(Instance_Sub_Unsub_Notify)
		{
			GameClock Clock;
			GameTime Time = Clock.Current();

			Event<BonePayload> ThrowBoneEvent;

			std::shared_ptr<BonePayload> SmallBone = std::make_shared<BonePayload>(BonePayload(5.0f));
			std::shared_ptr<BonePayload> BigBone = std::make_shared<BonePayload>(BonePayload(10.0f));

			DogSubscriber Dog;

			// Subscribe and test received payload
			ThrowBoneEvent.Subscribe(Dog);
			ThrowBoneEvent.Publish(SmallBone, Clock.Current());
			

			EventDispatcher::DispatchAll(Clock.Current());
			Assert::IsTrue(Dog.HasBone());
			Assert::AreEqual(SmallBone->_Weight, Dog.BoneWeight());

			// Unsubscribe and test did not receive payload
			ThrowBoneEvent.Unsubscribe(Dog);
			ThrowBoneEvent.Publish(BigBone, Clock.Current());
			EventDispatcher::DispatchAll(Clock.Current());
			Assert::AreEqual(SmallBone->_Weight, Dog.BoneWeight());
			Assert::AreNotEqual(BigBone->_Weight, Dog.BoneWeight());
		}
		
		TEST_METHOD(Global_Sub_Unsub_Notify)
		{
			GameClock Clock;

			Event<BonePayload> OwnerThrowBoneEvent;
			Event<BonePayload> StrangerThrowBoneEvent;
			std::shared_ptr<BonePayload> SmallBone = std::make_shared<BonePayload>(BonePayload(5.0f));
			std::shared_ptr<BonePayload> BigBone = std::make_shared<BonePayload>(BonePayload(10.0f));
			
			Event<TrickPayload> TrickEvent;
			std::shared_ptr<TrickPayload> SitTrick = std::make_shared<TrickPayload>(TrickPayload("Sit"));


			DogSubscriber Dog;

			// Test global subscription is notified by any event of same type
			Event<BonePayload>::SubscribeGlobal(Dog);

			OwnerThrowBoneEvent.Publish(SmallBone, Clock.Current());
			EventDispatcher::DispatchAll(Clock.Current());
			Assert::IsTrue(Dog.HasBone());
			Assert::AreEqual(SmallBone->_Weight, Dog.BoneWeight());

			StrangerThrowBoneEvent.Publish(BigBone, Clock.Current());
			EventDispatcher::DispatchAll(Clock.Current());
			Assert::IsTrue(Dog.HasBone());
			Assert::AreEqual(BigBone->_Weight, Dog.BoneWeight());

			// Test global subscription only notifies for subscribed event type
			TrickEvent.Publish(SitTrick, Clock.Current());
			EventDispatcher::DispatchAll(Clock.Current());
			Assert::AreEqual(std::string("None"), Dog.LastCommand());

			// Unsubscribe global and test isnt notified anymore
			std::shared_ptr<BonePayload> HugeBone = std::make_shared<BonePayload>(BonePayload(20.0f));
			Event<BonePayload>::UnsubscribeGlobal(Dog);
			OwnerThrowBoneEvent.Publish(HugeBone, Clock.Current());
			EventDispatcher::DispatchAll(Clock.Current());
			Assert::AreNotEqual(HugeBone->_Weight, Dog.BoneWeight());
		}

		TEST_METHOD(UnsubscribeAll)
		{
			GameClock Clock;

			Event<BonePayload> ThrowBoneEvent;

			std::shared_ptr<BonePayload> SmallBone = std::make_shared<BonePayload>(BonePayload(5.0f));

			DogSubscriber Dog1;
			DogSubscriber Dog2;

			ThrowBoneEvent.Subscribe(Dog1);
			ThrowBoneEvent.Subscribe(Dog2);

			ThrowBoneEvent.UnsubscribeAll();

			ThrowBoneEvent.Publish(SmallBone, Clock.Current());
			EventDispatcher::DispatchAll(Clock.CreateElapsedTime(100));

			Assert::IsFalse(Dog1.HasBone());
			Assert::IsFalse(Dog2.HasBone());
		}

		TEST_METHOD(DelayedDispatch)
		{
			GameClock Clock;
			GameTime Time = Clock.Current();

			Event<BonePayload> ThrowBoneEvent(std::chrono::milliseconds(100));

			std::shared_ptr<BonePayload> SmallBone = std::make_shared<BonePayload>(BonePayload(5.0f));

			DogSubscriber Dog;

			// Subscribe and test received payload
			ThrowBoneEvent.Subscribe(Dog);
			ThrowBoneEvent.Publish(SmallBone, Clock.Current());

			// Expect dog not notified before delay
			EventDispatcher::DispatchAll(Clock.Current());
			Assert::IsFalse(Dog.HasBone());
			Assert::AreNotEqual(SmallBone->_Weight, Dog.BoneWeight());

			// Expect dog notified after delay
			EventDispatcher::DispatchAll(Clock.CreateElapsedTime(200));
			Assert::IsTrue(Dog.HasBone());
			Assert::AreEqual(SmallBone->_Weight, Dog.BoneWeight());
		}
		

	private:
		inline static _CrtMemState _startMemState;

	};

}