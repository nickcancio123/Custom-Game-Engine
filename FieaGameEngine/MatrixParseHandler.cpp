#include "pch.h"
#include "MatrixParseHandler.h"
#include "ScopeWrapper.h"
#include <String>

using namespace Fiea::GameEngine;

Datum::Type MatrixParseHandler::GetType() const
{
	return Datum::MAT4X4;
}

bool MatrixParseHandler::IsJsonOfType(const Json::Value& JsonValue) const
{
	bool IsString = JsonValue.isString() || (JsonValue.isArray() && JsonValue[0].isString());

	if (!IsString)
	{
		return false;
	}

	std::string SerializedItem = JsonValue.isArray() ? JsonValue[0].asString() : JsonValue.asString();

	bool HasPrefix = SerializedItem.substr(0, 7) == "mat4x4|";

	return HasPrefix;
}