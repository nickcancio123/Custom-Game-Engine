#include "pch.h"
#include "ParseCoordinator.h"
#include "ParseHandler.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>


using namespace Fiea::GameEngine;
using BaseWrapper = ParseCoordinator::BaseWrapper;


RTTI_DEFINITIONS(BaseWrapper);


#pragma region === Base Wrapper ===

void ParseCoordinator::BaseWrapper::Initialize() {}

void ParseCoordinator::BaseWrapper::Cleanup() {}

#pragma endregion


ParseCoordinator::ParseCoordinator(BaseWrapper& Wrapper)
{
	SetWrapper(Wrapper);
}

BaseWrapper& ParseCoordinator::GetWrapper()
{
	return *_Wrapper;
}

void ParseCoordinator::SetWrapper(BaseWrapper& Wrapper)
{
	_Wrapper = &Wrapper;
}


#pragma region === Parsing ===

void ParseCoordinator::ParseMembers(const Json::Value& JsonValue)
{
	for (auto Itr = JsonValue.begin(); Itr != JsonValue.end(); ++Itr)
	{
		Parse(Itr.name(), JsonValue[Itr.name()]);
	}
}

void ParseCoordinator::Parse(const std::string& Name, const Json::Value& JsonValue)
{
	std::shared_ptr<ParseHandler> ActiveHandler = nullptr;

	for (auto Handler : _ParseHandlers)
	{
		if (Handler.get()->Start(*_Wrapper, Name, JsonValue))
		{
			ActiveHandler = Handler;
			break;
		}
	}

	if (ActiveHandler == nullptr)
	{
		throw std::runtime_error("Unable to parse a JsonValue");
	}

	if (JsonValue.isObject())
	{
		ParseMembers(JsonValue);
	}


	if (JsonValue.isArray() && JsonValue[0].isObject())
	{
		for (unsigned int i = 0; i < JsonValue.size(); ++i)
		{
			ActiveHandler.get()->Start(*_Wrapper, Name, JsonValue[i]);
			ParseMembers(JsonValue[i]);
			ActiveHandler->End(*_Wrapper, Name);
		}
	}
	
	ActiveHandler->End(*_Wrapper, Name);
}


void ParseCoordinator::AddParseHandler(std::shared_ptr<ParseHandler> NewHandler)
{
	_ParseHandlers.push_back(NewHandler);
}

void ParseCoordinator::RemoveParseHandler(std::shared_ptr<ParseHandler> HandlerToRemove)
{
	for (auto Handler : _ParseHandlers)
	{
		if (Handler == HandlerToRemove)
		{
			_ParseHandlers.remove(Handler);
		}
	}
}

#pragma endregion


#pragma region === Deserialization ===

void ParseCoordinator::DeserializeObject(const std::string& StringObject)
{
	std::stringstream Stream(StringObject);

	DeserializeObject(Stream);
}

void ParseCoordinator::DeserializeObjectFromFile(const std::string& FileName)
{
	std::ifstream FileStream;
	FileStream.open(FileName);

	if (!FileStream.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}

	DeserializeObject(FileStream);
}

void ParseCoordinator::DeserializeObject(std::istream& Stream)
{
	Json::Value RootValue;

	try
	{
		Stream >> RootValue;
	}
	catch (std::exception e)
	{
		throw std::runtime_error("Unable to create Json::Value. Ensure correct json formatting.");
	}

	_Wrapper->Initialize();

	for (auto Handler : _ParseHandlers)
	{
		Handler->Initialize(*_Wrapper);
	}

	ParseMembers(RootValue);

	for (auto Handler : _ParseHandlers)
	{
		Handler->Cleanup();
	}

	_Wrapper->Cleanup();
}

#pragma endregion
