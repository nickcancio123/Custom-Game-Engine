#include "pch.h"
#include "DatumParseHandler.h"
#include "Scope.h"
#include "ScopeWrapper.h"


using namespace Fiea::GameEngine;


bool DatumParseHandler::Start(BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue)
{
	ScopeWrapper* AScopeWrapper = Wrapper.As<ScopeWrapper>();

	if (AScopeWrapper == nullptr || !IsJsonOfType(JsonValue))
	{
		return false;
	}

	Scope* CurrentScope = AScopeWrapper->GetCurrent();
	assert(CurrentScope);

	Datum* FoundDatum = CurrentScope->Find(Name);

	if (JsonValue.isArray())
	{
		for (unsigned int Index = 0; Index < JsonValue.size(); ++Index)
		{
			AssignScopeAnItem(CurrentScope, FoundDatum, Name, JsonValue[Index], Index);
		}
	}
	else
	{
		AssignScopeAnItem(CurrentScope, FoundDatum, Name, JsonValue);
	}

	return true;
}

void DatumParseHandler::AssignScopeAnItem(Scope* CurrentScope, Datum* FoundDatum, const std::string& Name, const Json::Value& JsonValue, int Index)
{
	if (FoundDatum)
	{
		FoundDatum->SetSerializedItem(JsonValue.asString(), Index);
	}
	else
	{
		Datum& NewDatum = CurrentScope->Append(Name);
		NewDatum.SetType(GetType());
		NewDatum.PushBackSerialized(JsonValue.asString());
	}
}
