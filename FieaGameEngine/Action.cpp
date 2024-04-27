#include "pch.h"
#include "Action.h"
#include "GameObject.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(Action);


void Action::Update_Outer(const GameTime& Time)
{
	if (!_MarkedForDelete)
	{
		Update(Time);

		if (_SingleUpdate)
		{
			_MarkedForDelete = _SingleUpdate;
		}
	}
}

shared_ptr<deque<Signature>> Action::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("Name", Datum::STRING, 1, offsetof(Action, _Name)));

	return Sigs;
}


#pragma region === Accessors ===

const std::string& Action::Name() const
{
	return _Name;
}

bool Action::IsSingleUpdate() const
{
	return _SingleUpdate;
}

bool Action::MarkedForDelete() const
{
	return _MarkedForDelete;
}

#pragma endregion


#pragma region === Setters ===

void Action::SetName(const std::string& Name)
{
	_Name = Name;
}

void Action::SetSingleUpdate(bool SingleUpdate)
{
	_SingleUpdate = SingleUpdate;
}

bool Action::SetSubject(Attributed* Subject)
{
	if (_Subject != nullptr || Subject == nullptr)
	{
		return false;
	}

	_Subject = Subject;

	return true;
}

#pragma endregion


