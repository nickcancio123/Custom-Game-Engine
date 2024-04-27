#include "pch.h"
#include "AttributedBar.h"
#include "TypeRegistry.h"
#include "AttributedChild.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::Test;


RTTI_DEFINITIONS(AttributedBar);

AttributedBar* AttributedBar::Clone() const
{
	return new AttributedBar(*this);
}

shared_ptr<deque<Signature>> AttributedBar::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("Int", Datum::INT, 1, offsetof(AttributedBar, _Int)));

	return Sigs;
}
