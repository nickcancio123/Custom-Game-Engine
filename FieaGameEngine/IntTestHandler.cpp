#include "pch.h"
#include "IntTestHandler.h"


using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::Test;


#pragma region === Int Test Handler ===

bool IntTestHandler::Start(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue)
{
	if (!Wrapper.Is(IntTestWrapper::TypeIdClass()))
	{
		return false;
	}

	IntTestWrapper* IntWrapper = Wrapper.As<IntTestWrapper>();

	IntWrapper->Clear();


	if (JsonValue.isArray())
	{
		for (unsigned int Index = 0; Index < JsonValue.size(); ++Index)
		{
			Json::Value ArrayValue = JsonValue[Index];

			if (!ArrayValue.isInt())
			{
				return false;
			}

			IntWrapper->AddInt(ArrayValue.asInt());
		}
	}
	else
	{
		IntWrapper->AddInt(JsonValue.asInt());
	}

	return true;
}

bool IntTestHandler::End(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name)
{
	return false;
}

#pragma region


#pragma region === Int Test Wrapper ===

RTTI_DEFINITIONS(IntTestWrapper);

void IntTestWrapper::AddInt(int Int)
{
	_Ints.push_back(Int);
}

int IntTestWrapper::GetInt(int Index)
{
	return _Ints[Index];
}

void IntTestWrapper::Clear()
{
	_Ints.clear();
}

#pragma endregion
