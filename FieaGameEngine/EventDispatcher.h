#pragma once

#include <list>

namespace Fiea::GameEngine
{

	class EventBase;
	struct GameTime;

	class EventDispatcher
	{
		
	public:
		/**
		 * @brief Adds NewEvent to a queue of events to be broadcasted at some unspecified point
		 * @param NewEvent Event to broadcast at some point
		*/
		static void Add(EventBase& NewEvent);

		/**
		 * @brief Broadcasts all queued events
		*/
		static void DispatchAll(const GameTime& Time);

	private:
		/**
		 * @brief A queue of events to be broadcasted
		*/
		inline static std::list<EventBase*> _Events;
	};
}
