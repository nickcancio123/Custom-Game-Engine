#pragma once

#include "RTTI.h"
#include <list>
#include <memory>
#include <unordered_map>
#include "EventDispatcher.h"
#include "GameClock.h"


namespace Fiea::GameEngine
{
	class EventSubscriber;

	/**
	 * @brief Base class for all templated event types. Necessary for polymorphic events.
	*/
	class EventBase : public RTTI
	{
		RTTI_DECLARATIONS(EventBase, RTTI);

	public:
		EventBase(GameTime::Millis Delay = GameTime::Millis::zero()) : _Delay(Delay) {};

		virtual ~EventBase() = default;

		/**
		 * @brief Stores Subscriber in list to be notified when this event instance is dispatched
		*/
		void Subscribe(EventSubscriber& Subscriber);

		/**
		 * @brief Removes Subscriber from list of subscribers to be notified when this event is dispatched
		*/
		void Unsubscribe(EventSubscriber& Subscriber);

		/**
		 * @brief Clears entire list of subscribers, so no one is notified when this event is dispatched
		*/
		void UnsubscribeAll();

		/**
		 * @brief Returns if the delay time has fully elapsed
		 * @param Time Reference to the game time
		*/
		bool IsDelayOver(const GameTime& Time);


	protected:
		/**
		 * @brief List of event subscribers to be notified when this event is broadcasted
		*/
		std::list<EventSubscriber*> _InstanceSubscribers;

		/**
		 * @brief The time when this event was published.
		*/
		GameTime::Millis _TimePublished = GameTime::Millis::zero();

	private:
		/**
		 * @brief Friend function to call private Broadcast_Internal
		*/
		friend void EventDispatcher::DispatchAll(const GameTime& Time);

		/**
		 * @brief Notifies all subscribers, which can access the payload through RTTI
		*/
		virtual void Dispatch() = 0;

		/**
		 * @brief The amount of time after publishing to wait to dispatch
		*/
		GameTime::Millis _Delay = GameTime::Millis::zero();
	};


	/**
	 * @brief A templated event with a generic payload.
	 * @tparam PayloadClass The payload of the event which can be publicly accessed.
	*/
	template<class PayloadClass>
	class Event final: public EventBase
	{
		RTTI_DECLARATIONS(Event<PayloadClass>, EventBase);


	public:
		Event<PayloadClass>(GameTime::Millis Delay = GameTime::Millis::zero()) : EventBase(Delay) {};

		/**
		 * @brief Caches payload and enqueues this event on EventQueue<PayloadClass>
		 * @param Payload The payload of this event
		*/
		void Publish(std::shared_ptr<PayloadClass> Payload, const GameTime& Time);

		/**
		 * @brief Adds given subscriber to list of global subscribers, which will be notified when any instance of this
		 * event type is dispatched.
		 * @param Subscriber 
		*/
		static void SubscribeGlobal(EventSubscriber& Subscriber);

		/**
		 * @brief Finds and removes given subscriber from list of global subscribers.
		 * @param Subscriber 
		*/
		static void UnsubscribeGlobal(EventSubscriber& Subscriber);

		/**
		 * @brief Notifies all subscribers, instance and global, which can access the payload through RTTI
		*/
		virtual void Dispatch() override;

		/**
		 * @brief Accessor for the cached payload of this event
		 * @return A shared pointer to the payload
		*/
		const std::shared_ptr<PayloadClass> Payload() const;

	private:

		/**
		 * @brief The data/information/payload of this event
		*/
		std::shared_ptr<PayloadClass> _Payload;

		inline static std::list<EventSubscriber*> _GlobalSubscribers;
	};
}

#include "Event.inl"