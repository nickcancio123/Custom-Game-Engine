#include "pch.h"
#include "Attributed.h"
#include "AttributeRegistry.h"


using namespace Fiea::GameEngine;


#pragma region === Constructors & Destructors ===

Attributed::Attributed(const Attributed& Other)
	: Scope(Other)
{
	_AuxilliaryAttributeNames = Other._AuxilliaryAttributeNames;
	_PrescribedAttributesCreated = true;

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
	_PrescribedAttributesCreated = true;

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
	_PrescribedAttributesCreated = true;

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
	_PrescribedAttributesCreated = true;

	Datum* FoundDatum = Find("this");
	if (FoundDatum)
	{
		FoundDatum->Set((RTTI*)this);
	}

	return *this;
}

#pragma endregion


void Attributed::CreatePrescribedAttributes()
{
	if (_PrescribedAttributesCreated)
	{
		return;
	}

	AttributeRegistry* Registry = AttributeRegistry::GetInstance();
	assert(Registry);

	const std::vector<Signature>* PrescribedAttributeSignatures = Registry->GetAttributeSignatures(TypeIdInstance());
	if (!PrescribedAttributeSignatures)
	{
		PrescribedAttributeSignatures = RegisterAttributeSignatures();
	}
	assert(PrescribedAttributeSignatures);

	for (auto Sig : *PrescribedAttributeSignatures)
	{
		switch (Sig.Type())
		{

		case Datum::POINTER:
		{
			for (unsigned int i = 0; i < Sig.Size(); ++i)
			{
				auto MemberAddress = (unsigned char*)(this) + Sig.Offset() + (sizeof(RTTI*) * i);
				Datum& D1 = Append(Sig.Name());
				D1.PushBack(reinterpret_cast<RTTI*>(MemberAddress));
			}

			break;
		}

		case Datum::INT:
		{
			Datum& IntDatum = Append(Sig.Name());
			auto MemberAddress = (unsigned char*)(this) + Sig.Offset();
			IntDatum.StoreExternal(reinterpret_cast<int*>(MemberAddress), Sig.Size());
			break;
		}

		case Datum::FLOAT:
		{
			Datum& FloatDatum = Append(Sig.Name());
			auto MemberAddress = (unsigned char*)(this) + Sig.Offset();
			FloatDatum.StoreExternal(reinterpret_cast<float*>(MemberAddress), Sig.Size());
			break;
		}

		case Datum::STRING:
		{
			Datum& StringDatum = Append(Sig.Name());
			auto MemberAddress = (unsigned char*)(this) + Sig.Offset();
			StringDatum.StoreExternal(reinterpret_cast<std::string*>(MemberAddress), Sig.Size());
			break;
		}

		case Datum::VEC4:
		{
			Datum& Vec4Datum = Append(Sig.Name());
			auto MemberAddress = (unsigned char*)(this) + Sig.Offset();
			Vec4Datum.StoreExternal(reinterpret_cast<glm::vec4*>(MemberAddress), Sig.Size());
			break;		
		}

		case Datum::MAT4X4:
		{
			Datum& MatDatum = Append(Sig.Name());
			auto MemberAddress = (unsigned char*)(this) + Sig.Offset();
			MatDatum.StoreExternal(reinterpret_cast<glm::mat4x4*>(MemberAddress), Sig.Size());
			break;	
		}

		default:
		{
			throw std::runtime_error("Attempted to create attribute of invalid type");
			break;
		}
		}
	}

	_PrescribedAttributesCreated = true;
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
	AttributeRegistry* Registry = AttributeRegistry::GetInstance();
	assert(Registry);

	const std::vector<Signature>* PrescribedAttributeSignatures = Registry->GetAttributeSignatures(TypeIdInstance());
	if (!PrescribedAttributeSignatures)
	{
		PrescribedAttributeSignatures = RegisterAttributeSignatures();
	}

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