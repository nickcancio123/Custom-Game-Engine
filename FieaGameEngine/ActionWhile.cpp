#include "pch.h"
#include "TypeRegistry.h"
#include "ActionWhile.h"

using namespace Fiea::GameEngine;


RTTI_DEFINITIONS(ActionWhile);


shared_ptr<deque<Signature>> ActionWhile::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("Condition", Datum::INT, 1, offsetof(ActionWhile, _Condition)));
	Sigs->push_back(Signature("Preamble", Datum::SCOPE, 1, offsetof(ActionWhile, _Preamble)));
	Sigs->push_back(Signature("Increment", Datum::SCOPE, 1, offsetof(ActionWhile, _Increment)));

	return Sigs;
}

ActionWhile* ActionWhile::Clone() const
{
	return new ActionWhile(*this);
}

void ActionWhile::Update(const GameTime& Time)
{
	assert(_Preamble != nullptr);
	_Preamble->Update(Time);

	while (_Condition != 0)
	{
		ActionList::Update(Time);
	
		assert(_Increment != nullptr);
		_Increment->Update(Time);
	}
}

void ActionWhile::SetPreamble(Action& Preamble)
{
	if (_Preamble != nullptr)
	{
		Orphan(_Preamble);
		delete _Preamble;
	}

	_Preamble = &Preamble;
	Adopt("Preamble", *_Preamble);
	_Preamble->SetSubject(_Subject);
}

void ActionWhile::SetIncrement(Action& Increment)
{
	if (_Increment != nullptr)
	{
		Orphan(_Increment);
		delete _Increment;
	}

	_Increment = &Increment;
	Adopt("Increment", *_Increment);
	_Increment->SetSubject(_Subject);
}
