#include "pch.h"
#include "StringParseHandler.h"
#include "ScopeWrapper.h"
#include <string>

using namespace Fiea::GameEngine;


Datum::Type StringParseHandler::GetType() const
{
	return Datum::STRING;
}

bool StringParseHandler::IsJsonOfType(const Json::Value& JsonValue) const
{
	return JsonValue.isString() || (JsonValue.isArray() && JsonValue[0].isString());
}
