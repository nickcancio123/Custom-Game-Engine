#include "pch.h"
#include "EventDispatcher.h"
#include "Event.h"


using namespace Fiea::GameEngine;


void EventDispatcher::Add(EventBase& NewEvent)
{
	_Events.push_back(&NewEvent);
}

void EventDispatcher::DispatchAll(const GameTime& Time)
{
	std::vector<EventBase*> DispatchedEvents;

	for (EventBase* Event : _Events)
	{
		if (Event->IsDelayOver(Time))
		{
			Event->Dispatch();
			DispatchedEvents.push_back(Event);
		}
	}

	while (!DispatchedEvents.empty())
	{
		_Events.remove(DispatchedEvents.back());
		DispatchedEvents.pop_back();
	}
}
