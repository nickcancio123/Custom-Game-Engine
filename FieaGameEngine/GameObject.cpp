#include "pch.h"
#include "GameObject.h"
#include "ActionList.h"
#include "FactoryManager.h"
#include <memory>
#include <deque>


using namespace Fiea::GameEngine;


RTTI_DEFINITIONS(GameObject);


GameObject::GameObject(IdType TypeId)
	: Attributed(TypeId)
{
	Append("Children").SetType(Datum::SCOPE);

	Scope* ActionListScope = FactoryManager<Scope>::Create("ActionList");
	assert(ActionListScope != nullptr);
	_Actions = ActionListScope->As<ActionList>();
	Adopt("Actions", *_Actions);
}

GameObject* GameObject::Clone() const
{
	return new GameObject(*this);
}

shared_ptr<deque<Signature>> GameObject::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("Name", Datum::STRING, 1, offsetof(GameObject, _Name)));
	Sigs->push_back(Signature("Pos", Datum::VEC4, 1, offsetof(GameObject, _Transform._Pos)));
	Sigs->push_back(Signature("Rot", Datum::VEC4, 1, offsetof(GameObject, _Transform._Rot)));
	Sigs->push_back(Signature("Scale", Datum::VEC4, 1, offsetof(GameObject, _Transform._Scale)));
	Sigs->push_back(Signature("Children", Datum::SCOPE));
	Sigs->push_back(Signature("Actions", Datum::SCOPE));

	return Sigs;
}


void GameObject::Update(const GameTime& Time)
{
	Datum* ChildrenDatum = Find("Children");
	assert(ChildrenDatum != nullptr);

	for (unsigned int i = 0; i < ChildrenDatum->Size(); ++i)
	{
		Scope* ChildScope = ChildrenDatum->GetScope(i);
		assert(ChildScope != nullptr);

		GameObject* ChildObject = static_cast<GameObject*>(ChildScope);
		assert(ChildObject != nullptr);

		ChildObject->Update(Time);
	}

	_Actions->Update_Outer(Time);
}

void GameObject::AddChild(GameObject& NewChild)
{
	Adopt("Children", NewChild);

	_ChildrenLookup.insert({ NewChild.GetName(), &NewChild });
}

Action* GameObject::CreateAction(const std::string& ClassName, const std::string& InstanceName)
{
	Scope* NewActionScope = FactoryManager<Scope>::Create(ClassName);
	Action* NewAction = NewActionScope->As<Action>();
	assert(NewAction);

	NewAction->SetSubject(this);
	NewAction->SetName(InstanceName);

	_Actions->AddAction(*NewAction);

	return NewAction;
}


#pragma region === Accessors ===

GameObject* GameObject::GetChild(const std::string& Name)
{
	auto Itr = _ChildrenLookup.find(Name);
	
	if (Itr == _ChildrenLookup.end())
	{
		return nullptr;
	}

	return Itr->second;
}

Transform& GameObject::GetTransform()
{
	return _Transform;
}

const Transform& GameObject::GetTransform() const
{
	return _Transform;
}

const std::string& GameObject::GetName() const
{
	return _Name;
}

const ActionList* GameObject::GetActions() const
{
	return _Actions;
}

#pragma endregion