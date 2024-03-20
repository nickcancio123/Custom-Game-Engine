#pragma once

#include "Datum.h"

namespace Fiea
{
	namespace GameEngine
	{

		struct Signature
		{
		public:
			Signature(std::string Name, Datum::Type Type, unsigned int Size, size_t Offset)
				: _Name(Name), _Type(Type), _Size(Size), _Offset(Offset)
			{ }

			const std::string& Name() { return _Name; }
			const Datum::Type& Type() { return _Type; }
			const unsigned int& Size() { return _Size; }
			const size_t Offset() { return _Offset; }

		private:
			std::string _Name;
			Datum::Type _Type;
			unsigned int _Size;
			size_t _Offset;
		};
	}
}