
#include "pch.h"
#include "Scope.h"
#include "Datum.h"
#include <vector>

using Scope = Fiea::GameEngine::Scope;
using Datum = Fiea::GameEngine::Datum;

RTTI_DEFINITIONS(Scope);


#pragma region === Constructors & Destructors ===

Scope::Scope(const unsigned int Capacity)
{
	if (Capacity > 0)
	{
		_NameToDatum.reserve(Capacity);
		_OrderedNames.reserve(Capacity);
	}
}

Scope::Scope(const Scope& Other)
{
	// Item-wise deep copy
	for (auto OrderedName : Other._OrderedNames)
	{
		auto KeyValue = Other._NameToDatum.find(OrderedName);
		assert(KeyValue != Other._NameToDatum.end());

		Datum* NamedDatum = KeyValue->second;
		assert(NamedDatum);

		// If contains scopes, manually create scopes
		if (NamedDatum->GetType() == Datum::SCOPE)
		{
			for (unsigned int i = 0; i < NamedDatum->Size(); ++i)
			{
				Adopt(OrderedName, *NamedDatum->GetScope(i)->Clone());
			}
		}
		else
		{
			Datum& NewDatum = Append(OrderedName);
			NewDatum = *NamedDatum;
		}
	}
}

Scope::Scope(Scope&& Other) noexcept
{
	// Copy data
	_NameToDatum = Other._NameToDatum;
	_OrderedNames = Other._OrderedNames;

	// Remove Other's ownership & delete
	Other._NameToDatum.clear();
	Other._OrderedNames.clear();
}

Scope::~Scope()
{
	if (_Parent)
	{
		_Parent->Orphan(this);
	}

	Clear();
}

#pragma endregion


#pragma region === Operators ===

Scope& Scope::operator=(const Scope& Other)
{
	if (&Other == this)
	{
		return *this;
	}

	Scope* Clone = Other.Clone();

	*this = std::move(*Clone);

	delete Clone;

	return *this;
}

Scope& Scope::operator=(Scope&& Other) noexcept
{
	Clear();

	// Copy data
	_NameToDatum = Other._NameToDatum;
	_OrderedNames = Other._OrderedNames;

	// Remove Other's ownership & delete
	Other._NameToDatum.clear();
	Other._OrderedNames.clear();

	return *this;
}

Datum& Scope::operator[] (std::string Name)
{
	return Append(Name);
}

Datum& Scope::operator[](unsigned int Index)
{
	if (Index > _OrderedNames.size())
	{
		throw std::out_of_range("Provided index out of range");
	}

	const std::string Name = _OrderedNames[Index];

	return *_NameToDatum.at(Name);
}

bool Scope::operator==(const Scope& Other) const
{
	// Compare size
	if (_NameToDatum.size() != Other._NameToDatum.size())
	{
		return false;
	}

	for (unsigned int i = 0; i < _OrderedNames.size(); ++i)
	{
		// HACK - Ignore "this" attribute
		if (_OrderedNames[i] == "this")
		{
			continue;
		}

		// Check keys
		if (_OrderedNames[i] != Other._OrderedNames[i])
		{
			return false;
		}

		// Check values
		auto FoundKeyValue = _NameToDatum.find(_OrderedNames[i]);
		auto OtherFoundKeyValue = Other._NameToDatum.find(Other._OrderedNames[i]);
		if (*FoundKeyValue->second != *OtherFoundKeyValue->second)
		{
			return false;
		}
	}

	return true;
}

bool Scope::operator!=(const Scope& Other) const
{
	return !(*this == Other);
}

#pragma endregion


Datum& Scope::Append(const std::string Name)
{		
	auto FoundKeyValue = _NameToDatum.find(Name);
	
	// If key doesn't already exists
	if (FoundKeyValue == _NameToDatum.end())
	{
		Datum* NewDatum = new Datum();
		_NameToDatum.insert({ Name, NewDatum });
		_OrderedNames.push_back(Name);
		return *NewDatum;
	}

	// If key already exists
	return *FoundKeyValue->second;
}

Scope& Scope::AppendScope(const std::string Name)
{
	Scope* NewScope = new Scope(1);
	NewScope->_Parent = this;
	
	Datum& Datum = Append(Name);
	Datum.PushBack(NewScope);
	
	return *NewScope;
}


void Scope::Adopt(const std::string Name, Scope& ChildScope)
{
	if (this == &ChildScope || IsDescendantOf(&ChildScope))
	{
		return;
	}

	if (ChildScope._Parent != nullptr)
	{
		ChildScope._Parent->Orphan(&ChildScope);
	}

	ChildScope._Parent = this;

	Datum& Datum = Append(Name);
	Datum.PushBack(&ChildScope);
}

void Scope::Orphan(Scope* ChildScope)
{
	unsigned int ScopeIndex = -1;
	Datum* FoundDatum = FindContainedScope(ChildScope, ScopeIndex);

	if (FoundDatum)
	{
		assert(ScopeIndex >= 0);

		Scope* RemovedScope = FoundDatum->RemoveScopeAt(ScopeIndex);
		assert(ChildScope == RemovedScope);

		RemovedScope->_Parent = nullptr;
	}
}


bool Scope::IsAncestorOf(const Scope* Other) const
{
	if (Other == nullptr || Other == this)
	{
		return false;
	}

	const Scope* CurrentScope = Other;
	while (CurrentScope != nullptr)
	{
		if (CurrentScope == this)
		{
			return true;
		}

		CurrentScope = CurrentScope->GetParent();
	}

	return false;
}

bool Scope::IsDescendantOf(const Scope* Other) const
{
	if (Other == nullptr || Other == this)
	{
		return false;
	}

	unsigned int Index = -1;
	Datum* FoundDatum = Other->FindContainedScope(this, Index);

	return FoundDatum != nullptr;
}


void Scope::Clear()
{
	// Delete named datum
	while (_NameToDatum.size() > 0)
	{
		auto KeyValue = _NameToDatum.begin();
		Datum* NamedDatum = KeyValue->second;

		if (NamedDatum != nullptr)
		{
			// Delete child scopes
			if (NamedDatum->GetType() == Datum::SCOPE)
			{
				for (unsigned int i = 0; i < NamedDatum->Size(); ++i)
				{
					Scope* ChildScope = NamedDatum->GetScope(i);
					if (ChildScope != nullptr)
					{
						ChildScope->_Parent = nullptr;
						delete ChildScope;
					}
				}
			}

			delete NamedDatum;
		}

		_NameToDatum.erase(KeyValue);
	}

	_OrderedNames.clear();
}

Scope* Scope::Clone() const
{
	return new Scope(*this);
}


#pragma region === Find & Search ===

Datum* Scope::FindContainedScope(const Scope* ScopeToFind, unsigned int& ScopeIndex) const
{
	if (ScopeToFind == nullptr)
	{
		return nullptr;
	}

	if (_NameToDatum.empty())
	{
		return nullptr;
	}

	for (auto KeyValue : _NameToDatum)
	{
		Datum* NamedDatum = KeyValue.second;
		
		if (NamedDatum->GetType() == Datum::SCOPE)
		{
			for (unsigned int i = 0; i < NamedDatum->Size(); ++i)
			{
				Scope* NestedScope = NamedDatum->GetScope(i);

				if (NestedScope == ScopeToFind)
				{
					ScopeIndex = i;
					return NamedDatum;
				}

				if (NestedScope)
				{
					Datum* NestedDatum = NestedScope->FindContainedScope(ScopeToFind, ScopeIndex);

					if (NestedDatum != nullptr)
					{
						return NestedDatum;
					}
				}
			}
		}
	}

	return nullptr;
}

bool Scope::FindNameOfContainedScope(const Scope* ScopeToFind, std::string& Name)
{
	if (ScopeToFind == nullptr)
	{
		return false;
	}

	for (auto KeyValue : _NameToDatum)
	{
		Datum* NamedDatum = KeyValue.second;

		if (NamedDatum->GetType() == Datum::SCOPE)
		{
			for (unsigned int i = 0; i < NamedDatum->Size(); ++i)
			{
				Scope* ChildScope = NamedDatum->GetScope(i);

				if (ChildScope == ScopeToFind)
				{
					Name = KeyValue.first;
					return true;
				}
			}
		}
	}

	return false;
}

Datum* Scope::Find(const std::string Name) const
{
	auto FindItr = _NameToDatum.find(Name);

	if (FindItr == _NameToDatum.end())
	{
		return nullptr;
	}
	else
	{
		return FindItr->second;
	}
}

Datum* Scope::Search(const std::string Name, Scope** FoundScope)
{
	for (auto KeyValue : _NameToDatum)
	{
		Datum* NamedDatum = KeyValue.second;

		// If found non-empty scope datum with same key, return
		if (KeyValue.first == Name && 
			NamedDatum->GetType() == Datum::SCOPE &&
			NamedDatum->Size() > 0)
		{
			if (FoundScope != nullptr)
			{
				*FoundScope = NamedDatum->GetScope(0);
			}

			return NamedDatum;
		}
	}

	if (_Parent != nullptr)
	{
		return _Parent->Search(Name, FoundScope);
	}

	return nullptr;
}

const Datum* Scope::Search(const std::string Name, Scope** FoundScope) const
{
	for (auto KeyValue : _NameToDatum)
	{
		Datum* NamedDatum = KeyValue.second;

		// If found non-empty scope datum with same key, return
		if (KeyValue.first == Name && 
			NamedDatum->GetType() == Datum::SCOPE &&
			NamedDatum->Size() > 0)
		{
			if (FoundScope != nullptr)
			{
				*FoundScope = NamedDatum->GetScope(0);
			}

			return NamedDatum;
		}
	}

	if (_Parent != nullptr)
	{
		return _Parent->Search(Name, FoundScope);
	}

	return nullptr;
}

#pragma endregion


Scope* Scope::GetParent() const
{
	return _Parent;
}
