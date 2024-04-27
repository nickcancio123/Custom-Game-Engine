#include "pch.h"
#include "FloatParseHandler.h"
#include "ScopeWrapper.h"

using namespace Fiea::GameEngine;


Datum::Type FloatParseHandler::GetType() const
{
	return Datum::FLOAT;
}

bool FloatParseHandler::IsJsonOfType(const Json::Value& JsonValue) const
{
	return JsonValue.isDouble() || (JsonValue.isArray() && JsonValue[0].isDouble());
}

