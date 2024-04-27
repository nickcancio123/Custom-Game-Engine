#include "pch.h"
#include "Event.h"
#include "EventSubscriber.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(EventBase);

void EventBase::Subscribe(EventSubscriber& Subscriber)
{
	_InstanceSubscribers.push_back(&Subscriber);
}

void EventBase::Unsubscribe(EventSubscriber& Subscriber)
{
	_InstanceSubscribers.remove(&Subscriber);
}

void EventBase::UnsubscribeAll()
{
	_InstanceSubscribers.clear();
}

bool EventBase::IsDelayOver(const GameTime& Time)
{
	return Time.Game() >= _TimePublished + _Delay;
}