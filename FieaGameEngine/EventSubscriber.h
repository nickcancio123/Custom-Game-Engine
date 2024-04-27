#pragma once

namespace Fiea::GameEngine
{

	class EventBase;

	class EventSubscriber
	{
	public:
		//virtual ~EventSubscriber() = 0;

		/**
		 * @brief An event callback. Called by a broadcasting event that is subscribed to.
		 * @param Event The broadcasting event that called this function and is subscribed to.
		*/
		virtual void Notify(const EventBase& Caller) = 0;
	};

}