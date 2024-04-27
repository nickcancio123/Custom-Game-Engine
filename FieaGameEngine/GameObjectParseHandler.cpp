#include "pch.h"
#include "GameObjectParseHandler.h"
#include "ScopeWrapper.h"


using namespace Fiea::GameEngine;



bool GameObjectParseHandler::Start(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue)
{

	return true;
}

bool GameObjectParseHandler::End(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name)
{
	return false;
}

