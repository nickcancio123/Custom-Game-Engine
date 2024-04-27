#include "pch.h"
#include "TestParseHandler.h"

using namespace Fiea::GameEngine::Test;


RTTI_DEFINITIONS(TestWrapper);


bool TestHandler::Start(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue)
{
	if (!Wrapper.Is(TestWrapper::TypeIdClass()))
	{
		return false;
	}

	Wrapper.As<TestWrapper>()->_Reached = true;

	return true;
}

bool TestHandler::End(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name)
{
	return false;
}
