
#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include "RTTI.h"
#include "Serialization.h"
#include <iostream>
#include <assert.h>



using Datum = Fiea::GameEngine::Datum;
using DataUnion = Fiea::GameEngine::Datum::DataUnion;
using Scope = Fiea::GameEngine::Scope;
using RTTI = Fiea::GameEngine::RTTI;


#pragma region === Constructors & Destructors ===
Datum::Datum()
	: _Type(Type::UKNOWN)
{
}

Datum::Datum(const Type InitType)
	: _Type(InitType)
{
}

Datum::Datum(const Datum& Other)
{
	if (Other._External)
	{
		CopyConstructExternal(Other);
		return;
	}

	if (Other._Type == Type::UKNOWN)
	{
		_Type = Type::UKNOWN;
		_Size = Other._Size;
		return;
	}

	_Type = Other._Type;

	Resize(Other._Size);

	for (unsigned int i = 0; i < _Size; ++i)
	{
		switch (_Type)
		{
		case Type::SCOPE:
			// Do nothing
			break;

		case Type::POINTER:
			_Data[i].RTTI = Other.GetPointer(i);
			break;

		case Type::INT:
			_Data[i].Int = new int(Other.GetInt(i));
			break;

		case Datum::FLOAT:
			_Data[i].Float = new float(Other.GetFloat(i));
			break;

		case Datum::STRING:
			_Data[i].String = new std::string(Other.GetString(i));
			break;

		case Datum::VEC4:
			_Data[i].Vec4 = new glm::vec4(Other.GetVec4(i));
			break;

		case Datum::MAT4X4:
			_Data[i].Mat4x4 = new glm::mat4x4(Other.GetMat4x4(i));
			break;

		default:
			throw std::runtime_error(INVALID_TYPE_MSG);
			break;
		}
	}
}

void Datum::CopyConstructExternal(const Datum& Other)
{
	_External = true;

	_Type = Other._Type;
	_Size = Other._Size;

	_Data = (DataUnion*)malloc(sizeof(DataUnion) * _Size);
	assert(_Data);
	memcpy(_Data, Other._Data, sizeof(DataUnion) * Other._Size);
}

Datum::Datum(Datum&& Other) noexcept
{
	_Type = Other._Type;
	_Size = Other._Size;
	_Capacity = _Size;

	// Move
	_Data = Other._Data;
	Other._Data = nullptr;

	Other._Size = 0;
	Other._Capacity = 0;
}

Datum::~Datum()
{
	if (_External)
	{
		free(_Data);
		return;
	}

	for (unsigned int i = 0; i < _Size; ++i)
	{
		switch (_Type)
		{
		case Type::SCOPE:
			// Do nothing
			break;
		case Type::POINTER:
			// Do nothing
			break;
		case Type::INT:
			delete _Data[i].Int;
			break;
		case Datum::FLOAT:
			delete _Data[i].Float;
			break;
		case Datum::STRING:
			delete _Data[i].String;
			break;
		case Datum::VEC4:
			delete _Data[i].Vec4;
			break;
		case Datum::MAT4X4:
			delete _Data[i].Mat4x4;
			break;
		default:
			break;
		}
	}

	free(_Data);
}

#pragma endregion


#pragma region === Operators ===

Datum& Datum::operator=(const Datum& Other)
{
	if (Other._External)
	{
		return AssignmentOpExternal(Other);
	}

	Clear();

	if (Other._Type == Type::UKNOWN)
	{
		_Type = Type::UKNOWN;
		_Size = Other._Size;
		return *this;
	}

	_Type = Other._Type;

	Resize(Other._Size);

	for (unsigned int i = 0; i < _Size; ++i)
	{
		switch (_Type)
		{
		case Type::POINTER:
			_Data[i].RTTI = Other.GetPointer(i);
			break;

		case Type::INT:
			_Data[i].Int = new int(Other.GetInt(i));
			break;

		case Datum::FLOAT:
			_Data[i].Float = new float(Other.GetFloat(i));
			break;

		case Datum::STRING:
			_Data[i].String = new std::string(Other.GetString(i));
			break;

		case Datum::VEC4:
			_Data[i].Vec4 = new glm::vec4(Other.GetVec4(i));
			break;

		case Datum::MAT4X4:
			_Data[i].Mat4x4 = new glm::mat4x4(Other.GetMat4x4(i));
			break;

		default:
			throw std::runtime_error(INVALID_TYPE_MSG);
			break;
		}
	}

	return *this;
}

Datum& Datum::AssignmentOpExternal(const Datum& Other)
{
	if (!_External)
	{
		Clear();
	}

	free(_Data);


	_External = true;

	_Type = Other._Type;
	_Size = Other._Size;


	_Data = (DataUnion*)malloc(sizeof(DataUnion) * _Size);
	assert(_Data);
	memcpy(_Data, Other._Data, sizeof(DataUnion) * Other._Size);

	return *this;
}

Datum& Datum::operator=(Datum&& Other)
{
	if (_Type != Other._Type)
	{
		throw std::runtime_error("Incompatible datum types");
	}

	Clear();
	free(_Data);

	_Size = Other._Size;
	_Capacity = _Size;

	// Move
	_Data = Other._Data;
	Other._Data = nullptr;

	Other._Size = 0;
	Other._Capacity = 0;

	return *this;
}

bool Datum::operator==(const Datum& Other) const
{
	if (_Type != Other._Type)
	{
		return false;
	}

	if (_Size != Other._Size)
	{
		return false;
	}

	for (unsigned int i = 0; i < _Size; ++i)
	{
		switch (_Type)
		{
		case Type::SCOPE:
			if (*_Data[i].Scope != *Other._Data[i].Scope)
			{
				return false;
			}
			break;

		case Type::POINTER:	break;

		case Type::INT:
			if (*_Data[i].Int != *Other._Data[i].Int)
			{
				return false;
			}
			break;

		case Type::FLOAT:
			if (*_Data[i].Float != *Other._Data[i].Float)
			{
				return false;
			}
			break;

		case Type::STRING:
			if (*_Data[i].String != *Other._Data[i].String)
			{
				return false;
			}
			break;

		case Type::VEC4:
			if (*_Data[i].Vec4 != *Other._Data[i].Vec4)
			{
				return false;
			}
			break;

		case Type::MAT4X4:
			if (*_Data[i].Mat4x4 != *Other._Data[i].Mat4x4)
			{
				return false;
			}
			break;

		default:
			throw std::runtime_error("Cannot compare elements with this datum type");
			break;
		}
	}

	return true;
}

bool Datum::operator!=(const Datum& Other) const
{
	return !(*this == Other);
}

Scope& Datum::operator[](const unsigned int Index)
{
	if (_Type != Datum::SCOPE)
	{
		throw std::runtime_error("Can't use bracket ops with this datum type");
	}

	if (Index >= _Size)
	{
		throw std::out_of_range("Index out of range");
	}

	return *GetScope(Index);
}

#pragma endregion


#pragma region === PushBack ===

bool Datum::PushBack(Scope* NewItem)
{
	assert(!_External);

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::SCOPE;
	}

	if (_Type != Type::SCOPE)
	{
		return false;
	}

	if (_Size == _Capacity)
	{
		AutoResize();
	}

	DataUnion* NewDataUnion = new(_Data + _Size) DataUnion();
	NewDataUnion->Scope = NewItem;

	++_Size;

	return true;
}

bool Datum::PushBack(RTTI* NewItem)
{
	assert(!_External);

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::POINTER;
	}

	if (_Type != Type::POINTER)
	{
		return false;
	}

	if (_Size == _Capacity)
	{
		AutoResize();
	}

	DataUnion* NewDataUnion = new(_Data + _Size) DataUnion();
	NewDataUnion->RTTI = NewItem;

	++_Size;

	return true;
}

bool Datum::PushBack(const int NewItem)
{
	if (_External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::INT;
	}

	if (_Type != Type::INT)
	{
		return false;
	}

	if (_Size == _Capacity)
	{
		AutoResize();
	}

	DataUnion* NewDataUnion = new(_Data + _Size) DataUnion();
	NewDataUnion->Int = new int(NewItem);

	++_Size;

	return true;
}

bool Datum::PushBack(const float NewItem)
{
	if (_External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::FLOAT;
	}

	if (_Type != Type::FLOAT)
	{
		return false;
	}

	if (_Size == _Capacity)
	{
		AutoResize();
	}

	DataUnion* NewDataUnion = new(_Data + _Size) DataUnion();
	NewDataUnion->Float = new float(NewItem);

	++_Size;

	return true;
}

bool Datum::PushBack(const std::string NewItem)
{
	if (_External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::STRING;
	}

	if (_Type != Type::STRING)
	{
		return false;
	}

	if (_Size == _Capacity)
	{
		AutoResize();
	}

	DataUnion* NewDataUnion = new(_Data + _Size) DataUnion();
	NewDataUnion->String = new std::string(NewItem);

	++_Size;

	return true;
}

bool Datum::PushBack(const glm::vec4 NewItem)
{
	if (_External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::VEC4;
	}

	if (_Type != Type::VEC4)
	{
		return false;
	}

	if (_Size == _Capacity)
	{
		AutoResize();
	}

	DataUnion* NewDataUnion = new(_Data + _Size) DataUnion();
	NewDataUnion->Vec4 = new glm::vec4(NewItem);

	++_Size;

	return true;
}

bool Datum::PushBack(const glm::mat4x4 NewItem)
{
	if (_External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::MAT4X4;
	}

	if (_Type != Type::MAT4X4)
	{
		return false;
	}

	if (_Size == _Capacity)
	{
		AutoResize();
	}

	DataUnion* NewDataUnion = new(_Data + _Size) DataUnion();
	NewDataUnion->Mat4x4 = new glm::mat4x4(NewItem);

	++_Size;

	return true;
}

#pragma endregion


bool Datum::PopBack()
{
	if (_External || _Size == 0)
	{
		return false;
	}

	switch (_Type)
	{	
	case Type::INT:
		delete _Data[_Size - 1].Int;
		break;
	case Type::FLOAT:
		delete _Data[_Size - 1].Float;
		break;
	case Type::STRING:
		delete _Data[_Size - 1].String;
		break;
	case Type::VEC4:
		delete _Data[_Size - 1].Vec4;
		break;
	case Type::MAT4X4:
		delete _Data[_Size - 1].Mat4x4;
		break;
	default:
		throw std::runtime_error("Can't popback with current datum type");
		break;
	};

	--_Size;

	return true;
}

Scope* Datum::RemoveScopeAt(const unsigned int Index)
{
	if (_Type != Type::SCOPE)
	{
		throw std::runtime_error(INVALID_TYPE_MSG);
	}

	if (Index >= _Size)
	{
		throw std::out_of_range("Index out of range");
	}

	Scope* RemovedScope = _Data[Index].Scope;

	memmove(&_Data[Index], &_Data[Index + 1], sizeof(DataUnion) * (_Size - Index + 1));

	--_Size;

	return RemovedScope;
}

void Datum::Clear()
{
	if (_External)
	{
		throw std::runtime_error("Cannot resize external storage");
	}

	while (_Size > 0)
	{
		PopBack();
	}
}

bool Datum::Resize(const unsigned int NewSize)
{
	if (_External)
	{
		return false;
	}

	if (NewSize == _Size)
	{
		Reserve(NewSize);
		return true;
	}

	else if (NewSize < _Size)
	{
		unsigned int DeltaSize = _Size - NewSize;
		for (unsigned int i = 0; i < DeltaSize; ++i)
		{
			PopBack();
		}

		DataUnion* NewData = (DataUnion*)malloc(sizeof(DataUnion) * _Size);
		assert(NewData);

		memmove(NewData, _Data, sizeof(DataUnion) * _Size);

		free(_Data);
		_Data = NewData;
		_Capacity = _Size;

		return true;
	}

	// If NewSize > _Size
	else
	{
		Reserve(NewSize);

		unsigned int DeltaSize = NewSize - _Size;
		for (unsigned int i = 0; i < DeltaSize; ++i)
		{
			new (_Data + i) DataUnion();
		}

		_Size = NewSize;

		return true;
	}
}

void Datum::Reserve(const unsigned int NewCapacity)
{
	assert(!_External);

	assert(NewCapacity >= _Size);

	DataUnion* NewData = (DataUnion*)malloc(sizeof(DataUnion) * NewCapacity);
	assert(NewData);

	memmove(NewData, _Data, sizeof(DataUnion) * _Size);

	free(_Data);
	_Data = NewData;
	_Capacity = NewCapacity;
}

void Datum::AutoResize()
{
	int NewCapacity = _Capacity == 0 ? MIN_ALLOC : _Capacity * AUTO_RESIZE_FACTOR;

	Reserve(NewCapacity);
}


#pragma region === Store External Data ===

bool Datum::StoreExternal(int* ExternalItems, const unsigned int Size)
{
	if (ExternalItems == nullptr || _External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::INT;
	}

	if (_Type != Type::INT)
	{
		return false;
	}

	Resize(Size);

	for (unsigned int i = 0; i < Size; ++i)
	{
		_Data[i].Int = &ExternalItems[i];
	}

	_External = true;

	return true;
}

bool Datum::StoreExternal(float* ExternalItems, const unsigned int Size)
{
	if (ExternalItems == nullptr || _External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::FLOAT;
	}

	if (_Type != Type::FLOAT)
	{
		return false;
	}

	Resize(Size);

	for (unsigned int i = 0; i < Size; ++i)
	{
		_Data[i].Float = &ExternalItems[i];
	}

	_External = true;

	return true;
}

bool Datum::StoreExternal(std::string* ExternalItems, const unsigned int Size)
{
	if (ExternalItems == nullptr || _External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::STRING;
	}

	if (_Type != Type::STRING)
	{
		return false;
	}

	Resize(Size);

	for (unsigned int i = 0; i < Size; ++i)
	{
		_Data[i].String = &ExternalItems[i];
	}

	_External = true;

	return true;
}

bool Datum::StoreExternal(glm::vec4* ExternalItems, const unsigned int Size)
{
	if (ExternalItems == nullptr || _External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::VEC4;
	}

	if (_Type != Type::VEC4)
	{
		return false;
	}

	Resize(Size);

	for (unsigned int i = 0; i < Size; ++i)
	{
		_Data[i].Vec4 = &ExternalItems[i];
	}

	_External = true;

	return true;
}

bool Datum::StoreExternal(glm::mat4x4* ExternalItems, const unsigned int Size)
{
	if (ExternalItems == nullptr || _External)
	{
		return false;
	}

	if (_Type == Type::UKNOWN)
	{
		_Type = Type::MAT4X4;
	}

	if (_Type != Type::MAT4X4)
	{
		return false;
	}

	Resize(Size);

	for (unsigned int i = 0; i < Size; ++i)
	{
		_Data[i].Mat4x4 = &ExternalItems[i];
	}

	_External = true;

	return true;
}

#pragma endregion


#pragma region === Setters ===

bool Datum::SetType(const Type NewType)
{
	// If type already set, disallow
	if (_Type != Type::UKNOWN)
	{
		return false;
	}

	if (NewType == Type::UKNOWN)
	{
		return false;
	}

	_Type = NewType;

	return true;
}

bool Datum::Set(Scope* Value, const unsigned int ItemIndex)
{
	if (_Type != Type::SCOPE)
	{
		return false;
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		return false;
	}

	assert(_Data);

	_Data[ItemIndex].Scope = Value;

	return true;
}

bool Datum::Set(RTTI* Value, const unsigned int ItemIndex)
{
	if (_Type != Type::POINTER)
	{
		return false;
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		return false;
	}

	assert(_Data);

	_Data[ItemIndex].RTTI = Value;

	return true;
}

bool Datum::Set(const int Value, const unsigned int ItemIndex)
{
	if (_Type != Type::INT)
	{
		return false;
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		return false;
	}

	assert(_Data);

	*_Data[ItemIndex].Int = Value;

	return true;
}

bool Datum::Set(const float Value, const unsigned int ItemIndex)
{
	if (_Type != Type::FLOAT)
	{
		return false;
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		return false;
	}

	assert(_Data);

	*_Data[ItemIndex].Float = Value;

	return true;
}

bool Datum::Set(const std::string Value, const unsigned int ItemIndex)
{
	if (_Type != Type::STRING)
	{
		return false;
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		return false;
	}

	assert(_Data);

	*_Data[ItemIndex].String = Value;

	return true;
}

bool Datum::Set(const glm::vec4 Value, const unsigned int ItemIndex)
{
	if (_Type != Type::VEC4)
	{
		return false;
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		return false;
	}

	assert(_Data);

	*_Data[ItemIndex].Vec4 = Value;

	return true;
}

bool Datum::Set(const glm::mat4x4 Value, const unsigned int ItemIndex)
{
	if (_Type != Type::MAT4X4)
	{
		return false;
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		return false;
	}

	assert(_Data);

	*_Data[ItemIndex].Mat4x4 = Value;

	return true;
}

bool Datum::SetSerializedItem(const std::string SerializedItem, const unsigned int ItemIndex)
{
	if (_Type == Type::UKNOWN)
	{
		return false;
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		return false;
	}

	switch (_Type)
	{
	case Type::INT:
		Set(DeserializeInt(SerializedItem), ItemIndex);
		break;

	case Type::FLOAT:
		Set(DeserializeFloat(SerializedItem), ItemIndex);
		break;

	case Type::STRING:
		Set(DeserializeString(SerializedItem), ItemIndex);
		break;

	case Type::VEC4:
		Set(DeserializeVec4(SerializedItem), ItemIndex);
		break;

	case Type::MAT4X4:
		Set(DeserializeMat4x4(SerializedItem), ItemIndex);
		break;

	default:
		throw std::runtime_error("Datum type not serializable");
	}

	return true;
}

#pragma endregion


#pragma region === Accessors ===

Scope* Datum::GetScope(const unsigned int ItemIndex) const
{
	if (_Type != Type::SCOPE)
	{
		throw std::runtime_error(INVALID_TYPE_MSG);
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		throw std::out_of_range("Item index out of range");
	}

	assert(_Data);

	return _Data[ItemIndex].Scope;
}

RTTI* Datum::GetPointer(const unsigned int ItemIndex) const
{
	if (_Type != Type::POINTER)
	{
		throw std::runtime_error(INVALID_TYPE_MSG);
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		throw std::out_of_range("Item index out of range");
	}

	assert(_Data);

	return _Data[ItemIndex].RTTI;
}

int Datum::GetInt(const unsigned int ItemIndex) const
{
	if (_Type != Type::INT)
	{
		throw std::runtime_error(INVALID_TYPE_MSG);
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		throw std::out_of_range("Item index out of range");
	}

	assert(_Data);

	return *_Data[ItemIndex].Int;
}

float Datum::GetFloat(const unsigned int ItemIndex) const
{
	if (_Type != FLOAT)
	{
		throw std::runtime_error(INVALID_TYPE_MSG);
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		throw std::out_of_range("Item index out of range");
	}
	
	assert(_Data);

	return *_Data[ItemIndex].Float;
}

std::string Datum::GetString(const unsigned int ItemIndex) const
{
	if (_Type != STRING)
	{
		throw std::runtime_error(INVALID_TYPE_MSG);
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		throw std::out_of_range("Item index out of range");
	}

	assert(_Data);

	return *_Data[ItemIndex].String;
}

glm::vec4 Datum::GetVec4(const unsigned int ItemIndex) const
{
	if (_Type != VEC4)
	{
		throw std::runtime_error(INVALID_TYPE_MSG);
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		throw std::out_of_range("Item index out of range");
	}

	assert(_Data);

	return *_Data[ItemIndex].Vec4;
}

glm::mat4x4 Datum::GetMat4x4(const unsigned int ItemIndex) const
{
	if (_Type != MAT4X4)
	{
		throw std::runtime_error(INVALID_TYPE_MSG);
	}

	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		throw std::out_of_range("Item index out of range");
	}

	assert(_Data);

	return *_Data[ItemIndex].Mat4x4;
}


std::string Datum::GetSerializedItem(const unsigned int ItemIndex) const
{
	if (ItemIndex < 0 || ItemIndex >= _Size)
	{
		throw std::out_of_range("Item index out of range");
	}

	assert(_Data);

	switch (_Type)
	{
	case Type::INT:
		return SerializeInt(*_Data[ItemIndex].Int);

	case Type::FLOAT:
		return SerializeFloat(*_Data[ItemIndex].Float);

	case Type::STRING:
		return SerializeString(*_Data[ItemIndex].String);

	case Type::VEC4:
		return SerializeVec4(*_Data[ItemIndex].Vec4);

	case Type::MAT4X4:
		return SerializeMat4x4(*_Data[ItemIndex].Mat4x4);

	default:
		throw std::runtime_error("Datum type not serializable");
	}
}


unsigned int Datum::Size() const
{
	return _Size;
}

Datum::Type Datum::GetType() const
{
	return _Type;
}

#pragma endregion

