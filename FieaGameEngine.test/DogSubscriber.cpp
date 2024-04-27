#include "pch.h"
#include "DogSubscriber.h"
#include "Event.h"

using namespace Fiea::GameEngine::Test;

void DogSubscriber::Notify(const EventBase& Caller)
{
	if (const Event<BonePayload>* BoneEvent = Caller.As<Event<BonePayload>>())
	{
		std::shared_ptr<BonePayload> Bone = BoneEvent->Payload();
		_HasBone = true;
		_BoneWeight = Bone->_Weight;		
	}
	else if (const Event<TrickPayload>* TrickEvent = Caller.As<Event<TrickPayload>>())
	{
		std::shared_ptr<TrickPayload> Trick = TrickEvent->Payload();
		_LastCommand = Trick->_Command;
	}
}
