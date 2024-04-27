#include "pch.h"
#include "Vec4ParseHandler.h"
#include "ScopeWrapper.h"
#include <String>

using namespace Fiea::GameEngine;


Datum::Type Vec4ParseHandler::GetType() const
{
	return Datum::VEC4;
}

bool Vec4ParseHandler::IsJsonOfType(const Json::Value& JsonValue) const
{
	bool IsString = JsonValue.isString() || (JsonValue.isArray() && JsonValue[0].isString());

	if (!IsString)
	{
		return false;
	}

	std::string SerializedItem = JsonValue.isArray() ? JsonValue[0].asString() : JsonValue.asString();

	bool HasPrefix = SerializedItem.substr(0, 5) == "vec4|";
	
	return HasPrefix;
}

