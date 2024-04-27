#include "pch.h"
#include "ScopeParseHandler.h"
#include "ScopeWrapper.h"
#include "FactoryManager.h"

using namespace Fiea::GameEngine;





void ScopeParseHandler::Initialize(ParseCoordinator::BaseWrapper& Wrapper)
{
	if (ScopeWrapper* AScopeWrapper = Wrapper.As<ScopeWrapper>())
	{
		ScopeStack.push(&AScopeWrapper->GetRoot());
	}
}

void ScopeParseHandler::Cleanup()
{
	while (!ScopeStack.empty())
	{
		ScopeStack.pop();
	}
}

bool ScopeParseHandler::Start(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue)
{
	ScopeWrapper* AScopeWrapper = Wrapper.As<ScopeWrapper>();

	if (AScopeWrapper == nullptr)
	{
		return false;
	}

	bool IsObjArray = JsonValue.isArray() && JsonValue[0].isObject();

	if (!JsonValue.isObject() && !IsObjArray)
	{
		return false;
	}

	Scope* PrevScope = ScopeStack.top();

	if (IsObjArray)
	{
		PrevScope->Append(Name).SetType(Datum::SCOPE);
		return true;
	}

	std::string TypeName;
	bool IsScopeSubclass = FindTypeName(JsonValue, TypeName);
	
	if (IsScopeSubclass)
	{
		Scope* NewScope = FactoryManager<Scope>::Create(TypeName);
		PrevScope->Adopt(Name, *NewScope);
		ScopeStack.push(NewScope);
	}
	else
	{
		ScopeStack.push(&PrevScope->AppendScope(Name));
	}
	
	AScopeWrapper->SetCurrent(*ScopeStack.top());

	return true;
}

bool ScopeParseHandler::End(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name)
{
	ScopeStack.pop();

	if (ScopeStack.size() > 0)
	{
		Wrapper.As<ScopeWrapper>()->SetCurrent(*ScopeStack.top());
	}

	return true;
}

bool ScopeParseHandler::FindTypeName(const Json::Value JsonValue, std::string& TypeName)
{
	if (!JsonValue.isObject())
	{
		return false;
	}

	for (auto Itr = JsonValue.begin(); Itr != JsonValue.end(); ++Itr)
	{
		if (Itr.key().asString() == "type")
		{
			TypeName = Itr->asString();
			return true;
		}
	}

	return false;
}
