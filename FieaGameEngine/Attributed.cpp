#include "pch.h"
#include "Attributed.h"
#include "TypeRegistry.h"
#include <deque>
#include <memory>


using namespace Fiea::GameEngine;


RTTI_DEFINITIONS(Attributed);



#pragma region === Constructors & Destructors ===

Attributed::Attributed(size_t TypeId)
{
	CreatePrescribedAttributes(TypeId);
}

Attributed::Attributed(const Attributed& Other)
	: Scope(Other)
{
	_AuxilliaryAttributeNames = Other._AuxilliaryAttributeNames;


	Datum* FoundDatum = Find("this");
	if (FoundDatum)
	{
		FoundDatum->Set((RTTI*)this);
	}
}

Attributed::Attributed(Attributed&& Other) noexcept
	: Scope(std::move(Other))
{
	_AuxilliaryAttributeNames = std::move(Other._AuxilliaryAttributeNames);


	Datum* FoundDatum = Find("this");
	if (FoundDatum)
	{
		FoundDatum->Set((RTTI*)this);
	}
}

#pragma endregion


#pragma region === Operators ===

bool Attributed::operator!=(const Attributed& Other) const
{
	return !(*this == Other);
}

Attributed& Attributed::operator=(const Attributed& Other)
{
	assert(&Other != this);

	Scope::operator=(Other);

	_AuxilliaryAttributeNames = Other._AuxilliaryAttributeNames;

	Datum* FoundDatum = Find("this");
	if (FoundDatum)
	{
		FoundDatum->Set((RTTI*)this);
	}
	return *this;
}

Attributed& Attributed::operator=(Attributed&& Other) noexcept
{
	assert(&Other != this);

	Scope::operator=(std::move(Other));

	_AuxilliaryAttributeNames = std::move(Other._AuxilliaryAttributeNames);

	Datum* FoundDatum = Find("this");
	if (FoundDatum)
	{
		FoundDatum->Set((RTTI*)this);
	}

	return *this;
}

#pragma endregion


void Attributed::CreatePrescribedAttributes(size_t TypeId)
{
	const std::shared_ptr<std::deque<Signature>> PrescribedSignatures = TypeRegistry::GetSignaturesOfType(TypeId);

	assert(PrescribedSignatures != nullptr);

	for (auto Sig : *PrescribedSignatures)
	{
		Datum& NewDatum = Append(Sig.Name());
		NewDatum.SetType(Sig.Type());

		if (Sig.Size() > 0)
		{
			unsigned char* MemberAddress = (unsigned char*)(this) + Sig.Offset();
			NewDatum.StoreExternalGeneric(MemberAddress, Sig.Size());
		}
	}
}

Datum& Attributed::AppendAuxilliaryAttribute(const std::string Name)
{
	if (IsPrescribedAttribute(Name))
	{
		throw std::runtime_error("Cannot append auxilliary attribute with same name as precribed attribute");
	}

	if (!IsAuxilliaryAttribute(Name))
	{
		_AuxilliaryAttributeNames.push_back(Name);
	}

	return Append(Name);
}


#pragma region === Attribute Checks ===

bool Attributed::IsAttribute(const std::string& Name)
{
	return IsPrescribedAttribute(Name) || IsAuxilliaryAttribute(Name);
}

bool Attributed::IsPrescribedAttribute(const std::string& Name)
{
	const std::shared_ptr<std::deque<Signature>> PrescribedAttributeSignatures = TypeRegistry::GetSignaturesOfType(TypeIdInstance());

	assert(PrescribedAttributeSignatures != nullptr);

	for (auto Sig : *PrescribedAttributeSignatures)
	{
		if (Name == Sig.Name())
		{
			return true;
		}
	}

	return false;
}

bool Attributed::IsAuxilliaryAttribute(const std::string& Name)
{
	for (auto AuxilliaryName : _AuxilliaryAttributeNames)
	{
		if (Name == AuxilliaryName)
		{
			return true;
		}
	}

	return false;
}

#pragma endregion