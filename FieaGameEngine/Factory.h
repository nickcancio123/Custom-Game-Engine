#pragma once

#include <string>

namespace Fiea::GameEngine
{

	template<class BaseClass>
	class Factory
	{
	public:
		virtual BaseClass* Create() const = 0;

		virtual std::string ClassName() const = 0;

		virtual ~Factory() = default;
	};


#define MAKE_FACTORY(_Base, _Concrete)								\
	class _Concrete##Factory final : public Factory<_Base> {		\
																	\
		virtual _Base* Create() const override {					\
			return new _Concrete();									\
		}															\
																	\
		virtual std::string ClassName() const override {			\
			return #_Concrete;										\
		}															\
																	\
	};	
}