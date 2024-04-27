#pragma once

#include "EventSubscriber.h"
#include <string>


namespace Fiea::GameEngine::Test
{

	struct BonePayload
	{
		BonePayload(float Weight = 1) : _Weight(Weight) {}

		float _Weight;
	};

	struct TrickPayload
	{
		TrickPayload(std::string Command) : _Command(Command) {}

		std::string _Command;
	};


	class DogSubscriber : public EventSubscriber
	{
	public:
		virtual void Notify(const EventBase& Caller) override;

		bool HasBone() { return _HasBone; }

		float BoneWeight() { return _BoneWeight; }

		const std::string& LastCommand() { return _LastCommand; }

	private:

		bool _HasBone = false;

		float _BoneWeight = 0;

		std::string _LastCommand = "None";
	};

}