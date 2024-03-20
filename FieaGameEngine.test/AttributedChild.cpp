#include "pch.h"
#include "AttributedChild.h"
#include "AttributeRegistry.h"


using namespace Fiea::GameEngine;


const std::vector<Signature>* AttributedChild::RegisterAttributeSignatures()
{
	AttributeRegistry* Registry = AttributeRegistry::GetInstance();
	assert(Registry);

	return Registry->RegisterAttributeSignatures(AttributedChild::TypeIdClass(),
		{
			Signature("this", Datum::Type::POINTER, 1, 0),

			//Signature("Foo", Datum::POINTER, 1, offsetof(AttributedChild, _Foo)),
			//Signature("Int", Datum::INT, 1, offsetof(AttributedChild, _Int)),
			//Signature("Float", Datum::FLOAT, 1, offsetof(AttributedChild, _Float)),
			//Signature("String", Datum::STRING, 1, offsetof(AttributedChild, _String)),
			//Signature("Vec4", Datum::VEC4, 1, offsetof(AttributedChild, _Vec4)),
			//Signature("Mat4x4", Datum::MAT4X4, 1, offsetof(AttributedChild, _Mat4x4)),
			//
			//Signature("FooArray", Datum::POINTER, 3, offsetof(AttributedChild, _FooArray)),
			//Signature("IntArray", Datum::INT, 3, offsetof(AttributedChild, _IntArray)),
			//Signature("FloatArray", Datum::FLOAT, 3, offsetof(AttributedChild, _FloatArray)),
			//Signature("StringArray", Datum::STRING, 3, offsetof(AttributedChild, _StringArray)),
			//Signature("Vec4Array", Datum::VEC4, 3, offsetof(AttributedChild, _Vec4Array)),
			//Signature("Mat4x4Array", Datum::MAT4X4, 3, offsetof(AttributedChild, _Mat4x4Array))
		}
	);
}

AttributedChild::AttributedChild()
{
	_Foo = new Foo(0);
	_FooArray[0] = new Foo(0);
	_FooArray[1] = new Foo(1);
	_FooArray[2] = new Foo(2);

	CreatePrescribedAttributes();
}

AttributedChild::AttributedChild(const AttributedChild& Other)
	: Attributed(Other)
{
	_Foo = new Foo(0);
	_FooArray[0] = new Foo(0);
	_FooArray[1] = new Foo(1);
	_FooArray[2] = new Foo(2);
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

void AttributedChild::CreatePrescribedAttributesAgain()
{
	CreatePrescribedAttributes();
}
