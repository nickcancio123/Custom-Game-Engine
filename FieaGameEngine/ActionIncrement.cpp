#include "pch.h"
#include "ActionIncrement.h"
#include "GameObject.h"
#include "TypeRegistry.h"

using namespace Fiea::GameEngine;


RTTI_DEFINITIONS(ActionIncrement);


void ActionIncrement::Update(const GameTime& DeltaTime)
{
	assert(_Subject);
	Datum* FoundDatum = _Subject->Find(_AttributeToIncrement);
	assert(FoundDatum);

	if (FoundDatum->GetType() != Datum::FLOAT && FoundDatum->GetType() != Datum::INT)
	{
		throw std::runtime_error("IncrementAction: attribute to increment needs to be int or float");
	}

	if (_IncrementAmountDatum->GetType() != Datum::FLOAT && _IncrementAmountDatum->GetType() != Datum::INT)
	{
		throw std::runtime_error("IncrementAction: increment amount datum needs to be int or float");
	}
	
	float PreValue = FoundDatum->GetType() == Datum::FLOAT ? FoundDatum->GetFloat() : FoundDatum->GetInt();

	assert(_IncrementAmountDatum);
	float DeltaValue = _IncrementAmountDatum->GetType() == Datum::FLOAT ? _IncrementAmountDatum->GetFloat() : _IncrementAmountDatum->GetInt();

	float NewValue = PreValue + DeltaValue;

	if (FoundDatum->GetType() == Datum::FLOAT)
	{
		FoundDatum->Set(NewValue);
	}
	else
	{
		FoundDatum->Set((int) NewValue);
	}
}

ActionIncrement* ActionIncrement::Clone() const
{
	return new ActionIncrement(*this);
}

shared_ptr<deque<Signature>> ActionIncrement::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("AttributeName", Datum::STRING, 1, offsetof(ActionIncrement, _AttributeToIncrement)));
	Sigs->push_back(Signature("AttributeDatum", Datum::SCOPE, 0, 0));

	return Sigs;
}

void ActionIncrement::SetValues(const std::string& AttributeToIncrement, Datum& IncrementAmountDatum)
{
	_AttributeToIncrement = AttributeToIncrement;
	_IncrementAmountDatum = &IncrementAmountDatum;
}
