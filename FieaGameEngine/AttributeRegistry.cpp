#include "pch.h"
#include "AttributeRegistry.h"

using namespace Fiea::GameEngine;


void AttributeRegistry::Create()
{
	if (Instance == nullptr)
	{
		Instance = new AttributeRegistry();
	}
}

AttributeRegistry* AttributeRegistry::GetInstance()
{
	if (Instance == nullptr)
	{
		Create();
	}

	return Instance;
}

void AttributeRegistry::Destroy()
{
	delete Instance;
}

const std::vector<Signature>* AttributeRegistry::RegisterAttributeSignatures(RTTI::IdType ID, std::vector<Signature> Signatures)
{
	auto NewPair = TypeToSignatures.insert({ ID, Signatures });
	
	return &NewPair.first->second;
}

const std::vector<Signature>* AttributeRegistry::GetAttributeSignatures(RTTI::IdType ID) const
{
	auto FoundPair = TypeToSignatures.find(ID);

	if (FoundPair == TypeToSignatures.end())
	{
		return nullptr;
	}

	return &FoundPair->second;
}
