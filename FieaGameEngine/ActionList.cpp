#include "pch.h"
#include "ActionList.h"
#include "TypeRegistry.h"

using namespace Fiea::GameEngine;


RTTI_DEFINITIONS(ActionList);


shared_ptr<deque<Signature>> ActionList::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("Actions", Datum::SCOPE));

	return Sigs;
}

ActionList* ActionList::Clone() const
{
	return new ActionList(*this);
}

void ActionList::Update(const GameTime& Time)
{
	for (auto ActionElement : _Actions)
	{
		Action* OwnedAction = ActionElement.second;
		assert(OwnedAction != nullptr);

		OwnedAction->Update_Outer(Time);

		if (OwnedAction->MarkedForDelete())
		{
			_ActionsToDelete.push_back(OwnedAction);
		}
	}

	DeleteMarkedActions();
}

void ActionList::AddAction(Action& NewAction)
{
	NewAction.SetSubject(_Subject);

	Adopt("Actions", NewAction);

	_Actions.insert({ NewAction.Name(), &NewAction });
}

unsigned int ActionList::Size() const
{
	return _Actions.size();
}

void ActionList::DeleteMarkedActions()
{
	Datum* ActionsDatum = Find("Actions");
	assert(ActionsDatum != nullptr);
	
	while (!_ActionsToDelete.empty())
	{
		Action* ActionToDelete = _ActionsToDelete.back();
		assert(ActionToDelete != nullptr);

		bool SuccessfullyRemoved = ActionsDatum->RemoveScope(ActionToDelete);
		assert(SuccessfullyRemoved);

		_Actions.erase(ActionToDelete->Name());
		_ActionsToDelete.pop_back();

		delete ActionToDelete;
	}
}
