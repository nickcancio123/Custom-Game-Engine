#include "pch.h"
#include "TypeRegistry.h"
#include "Monster.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::Test;

RTTI_DEFINITIONS(Monster);

Monster* Monster::Clone() const
{
	return new Monster(*this);
}

shared_ptr<deque<Signature>> Monster::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("Health", Datum::FLOAT, 1, offsetof(Monster, _Health)));
	Sigs->push_back(Signature("HealAmount", Datum::FLOAT, 1, offsetof(Monster, _HealAmount)));

	return Sigs;
}

void Monster::Update(const GameTime& DeltaTime)
{
	GameObject::Update(DeltaTime);

	UpdateCalled = true;
}


