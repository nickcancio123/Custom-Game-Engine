#include "pch.h"
#include "IntParseHandler.h"
#include "ScopeWrapper.h"


using namespace Fiea::GameEngine;


Datum::Type IntParseHandler::GetType() const
{
	return Datum::INT;
}

bool IntParseHandler::IsJsonOfType(const Json::Value& JsonValue) const
{
	bool IsInteger = JsonValue.type() == Json::intValue || JsonValue.type() == Json::uintValue;
	bool IsIntegerArray = JsonValue.isArray() && 
		(JsonValue[0].type() == Json::intValue || JsonValue[0].type() == Json::uintValue);

	return IsInteger || IsIntegerArray;
}




