#include "pch.h"
#include "CppUnitTest.h"
#include "Serialization.h"
#include <vector>
#include <codecvt>

#include "Datum.h"
#include "Scope.h"
#include "RTTI.h"
#include "Attributed.h" 
#include "Empty.h"
#include "EmptyChild.h"
#include "Foo.h"
#include "AttributedChild.h"


using namespace Fiea::GameEngine;


namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Datum::Type>(const Datum::Type& T)
	{
		RETURN_WIDE_STRING(T);
	}

	template<>
	std::wstring ToString<Datum>(const Datum& T)
	{
		return std::to_wstring(1);
	}

	template<>
	std::wstring ToString<Scope>(const Scope& T)
	{
		return std::to_wstring(0);
	}

	template<>
	std::wstring ToString<RTTI>(const RTTI& T)
	{
		std::string Str = T.ToString();
		return std::wstring(Str.cbegin(), Str.cend());
	}

	template<>
	std::wstring ToString<glm::vec4>(const glm::vec4& T)
	{
		std::string StrVec4 = SerializeVec4(T);
		return std::wstring(StrVec4.cbegin(), StrVec4.cend());
	}

	template<>
	std::wstring ToString<glm::mat4x4>(const glm::mat4x4& T)
	{
		std::string StrMat = SerializeMat4x4(T);
		return std::wstring(StrMat.cbegin(), StrMat.cend());
	}


	template<>
	std::wstring ToString<Foo>(const Foo& T)
	{
		std::string Str = std::to_string(T.Data());
		return std::wstring(Str.cbegin(), Str.cend());
	}

	template<>
	std::wstring ToString<AttributedChild>(const AttributedChild& T)
	{
		return std::to_wstring(0);
	}
}
