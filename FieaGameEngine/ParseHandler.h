#pragma once

#include "ParseCoordinator.h"
#include "json/json.h"
#include <string>

namespace Fiea
{
	namespace GameEngine
	{

		using BaseWrapper = ParseCoordinator::BaseWrapper;

		class Scope;
		class Datum;

		class ParseHandler
		{

		public:
			ParseHandler() = default;

			virtual ~ParseHandler() = default;

			/**
			 * @brief Initializes the state of the handler per wrapper
			 * @param Wrapper The wrapper that may be operated on
			*/
			virtual void Initialize(BaseWrapper& Wrapper);

			/**
			 * @brief Cleans up the state of the handler
			*/
			virtual void Cleanup();

			/**
			 * @brief Checks if the given wrapper and json element can be operated on by this, and if so operates on it
			 * @param Wrapper The wrapper that may be operated on
			 * @param Name The name of the json element 
			 * @param JsonValue The json element
			 * @return Whether this parser can operator on the pair of wrapper and json element
			*/
			virtual bool Start(BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue) = 0;

			/**
			 * @brief Finalizes the operation on the pair of wrapper and json element
			 * @param Wrapper The wrapper operated on
			 * @param Name The name of the json element
			 * @return True - always
			*/
			virtual bool End(BaseWrapper& Wrapper, const std::string& Name) { return false; }
		};

	}
}