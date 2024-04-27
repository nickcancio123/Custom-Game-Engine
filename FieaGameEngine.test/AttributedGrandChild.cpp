
#include "pch.h"
#include "AttributedGrandChild.h"
#include "TypeRegistry.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::Test;


RTTI_DEFINITIONS(AttributedGrandChild);


shared_ptr<deque<Signature>> AttributedGrandChild::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("SecondInt", Datum::INT, 1, offsetof(AttributedGrandChild, _SecondInt)));
	Sigs->push_back(Signature("SecondFloat", Datum::FLOAT, 1, offsetof(AttributedGrandChild, _SecondFloat)));

	return Sigs;
}


AttributedGrandChild::AttributedGrandChild()
	: AttributedChild(TypeIdInstance())
{ }

AttributedGrandChild::AttributedGrandChild(const AttributedGrandChild& Other)
	: AttributedChild(Other)
{
	_SecondInt = Other._SecondInt;
	_SecondFloat = Other._SecondFloat;
}

AttributedGrandChild* AttributedGrandChild::Clone() const
{
	return new AttributedGrandChild(*this);
}