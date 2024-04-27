
#include "pch.h"
#include "TypeRegistry.h"


using namespace Fiea::GameEngine;


void TypeRegistry::RegisterSignatures(size_t TypeId, shared_ptr<deque<Signature>> NewTypeSignatures, size_t ParentTypeId)
{
	// If parent type given, append parent sigs to new sigs
	if (ParentTypeId != -1)
	{
		const shared_ptr<deque<Signature>> ParentSigs = GetSignaturesOfType(ParentTypeId);
		assert(ParentSigs != nullptr);

		for (int i = ParentSigs->size() - 1; i >= 0; --i)
		{
			NewTypeSignatures->push_front(ParentSigs->at(i));
		}
	}

	_TypeSignatures.insert({ TypeId, NewTypeSignatures });
}

const shared_ptr<deque<Signature>> TypeRegistry::GetSignaturesOfType(size_t TypeId)
{
	auto FoundPair = _TypeSignatures.find(TypeId);

	if (FoundPair != _TypeSignatures.end())
	{
		return FoundPair->second;
	}

	return nullptr;
}


