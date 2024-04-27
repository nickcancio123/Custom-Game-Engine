#include "pch.h"
#include "AttributedChild.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::Test;


RTTI_DEFINITIONS(AttributedChild);


AttributedChild::AttributedChild(size_t TypeId)
	: Attributed(TypeId)
{
	_Foo = new Foo(0);
	_FooArray[0] = new Foo(0);
	_FooArray[1] = new Foo(1);
	_FooArray[2] = new Foo(2);
}

AttributedChild::AttributedChild(const AttributedChild& Other)
	: Attributed(Other)
{
	_Foo = new Foo(*Other._Foo);
	_FooArray[0] = new Foo(*Other._FooArray[0]);
	_FooArray[1] = new Foo(*Other._FooArray[1]);
	_FooArray[2] = new Foo(*Other._FooArray[2]);
}

AttributedChild::~AttributedChild()
{
	delete _Foo;
	delete _FooArray[0];
	delete _FooArray[1];
	delete _FooArray[2];
}

AttributedChild* AttributedChild::Clone() const
{
	return new AttributedChild(*this);
}

AttributedChild& AttributedChild::operator=(const AttributedChild& Other)
{
	if (this == &Other)
	{
		return *this;
	}

	Attributed::operator=(Other);

	// Steal pointer ownership
	if (_Foo) delete _Foo;
	if (_FooArray[0]) delete _FooArray[0];
	if (_FooArray[1]) delete _FooArray[1];
	if (_FooArray[2]) delete _FooArray[2];

	_Foo = new Foo(*Other._Foo);
	_FooArray[0] = new Foo(*Other._FooArray[0]);
	_FooArray[1] = new Foo(*Other._FooArray[1]);
	_FooArray[2] = new Foo(*Other._FooArray[2]);

	return *this;
}

AttributedChild& AttributedChild::operator=(AttributedChild&& Other) noexcept
{
	if (&Other == this)
	{
		return *this;
	}

	Attributed::operator=(std::move(Other));

	// Steal pointer ownership
	if (_Foo) delete _Foo;
	if (_FooArray[0]) delete _FooArray[0];
	if (_FooArray[1]) delete _FooArray[1];
	if (_FooArray[2]) delete _FooArray[2];

	_Foo = Other._Foo;
	_FooArray[0] = Other._FooArray[0];
	_FooArray[1] = Other._FooArray[1];
	_FooArray[2] = Other._FooArray[2];

	Other._Foo = nullptr;
	Other._FooArray[0] = nullptr;
	Other._FooArray[1] = nullptr;
	Other._FooArray[2] = nullptr;

	return *this;
}

shared_ptr<deque<Signature>> AttributedChild::Signatures()
{
	shared_ptr<deque<Signature>> Sigs = std::make_shared<deque<Signature>>();

	Sigs->push_back(Signature("this", Datum::POINTER, 1, 0));

	Sigs->push_back(Signature("Foo", Datum::POINTER, 1, offsetof(AttributedChild, _Foo)));
	Sigs->push_back(Signature("Int", Datum::INT, 1, offsetof(AttributedChild, _Int)));
	Sigs->push_back(Signature("Float", Datum::FLOAT, 1, offsetof(AttributedChild, _Float)));
	Sigs->push_back(Signature("String", Datum::STRING, 1, offsetof(AttributedChild, _String)));
	Sigs->push_back(Signature("Vec4", Datum::VEC4, 1, offsetof(AttributedChild, _Vec4)));
	Sigs->push_back(Signature("Mat4x4", Datum::MAT4X4, 1, offsetof(AttributedChild, _Mat4x4)));

	Sigs->push_back(Signature("FooArray", Datum::POINTER, 3, offsetof(AttributedChild, _FooArray)));
	Sigs->push_back(Signature("IntArray", Datum::INT, 3, offsetof(AttributedChild, _IntArray)));
	Sigs->push_back(Signature("FloatArray", Datum::FLOAT, 3, offsetof(AttributedChild, _FloatArray)));
	Sigs->push_back(Signature("StringArray", Datum::STRING, 3, offsetof(AttributedChild, _StringArray)));
	Sigs->push_back(Signature("Vec4Array", Datum::VEC4, 3, offsetof(AttributedChild, _Vec4Array)));
	Sigs->push_back(Signature("Mat4x4Array", Datum::MAT4X4, 3, offsetof(AttributedChild, _Mat4x4Array)));

	return Sigs;
}
