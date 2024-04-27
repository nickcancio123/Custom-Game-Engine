#include "Event.h"
#include "EventSubscriber.h"
#include "EventDispatcher.h"


using namespace Fiea::GameEngine;


template<class PayloadClass>
RTTI_DEFINITIONS(Event<PayloadClass>);


template<class PayloadClass>
void Event<PayloadClass>::Publish(std::shared_ptr<PayloadClass> Payload, const GameTime& Time)
{
	_Payload = Payload;

	_TimePublished = Time.Game();

	EventDispatcher::Add(*this);
}

template<class PayloadClass>
inline void Event<PayloadClass>::SubscribeGlobal(EventSubscriber& Subscriber)
{
	_GlobalSubscribers.push_back(&Subscriber);
}

template<class PayloadClass>
inline void Event<PayloadClass>::UnsubscribeGlobal(EventSubscriber& Subscriber)
{
	_GlobalSubscribers.remove(&Subscriber);
}

template<class PayloadClass>
inline void Event<PayloadClass>::Dispatch()
{
	for (EventSubscriber* Subscriber : _InstanceSubscribers)
	{
		Subscriber->Notify(*this);
	}

	for (EventSubscriber* Subscriber : _GlobalSubscribers)
	{
		Subscriber->Notify(*this);
	}
}

template<class PayloadClass>
inline const std::shared_ptr<PayloadClass> Event<PayloadClass>::Payload() const
{
	return _Payload;
}

