#pragma once

#include "ParseHandler.h"
#include "Datum.h"
#include "ParseCoordinator.h"

namespace Fiea
{
	namespace GameEngine
	{

		class DatumParseHandler : public ParseHandler
		{
		public:

			virtual bool Start(BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue) override;

		protected:
			/**
			 * @brief The type of datum this parser should make/set
			*/
			virtual Datum::Type GetType() const = 0;

			/**
			 * @brief Is the given json the same as the datum type
			*/
			virtual bool IsJsonOfType(const Json::Value& JsonValue) const = 0;

			/**
			 * @brief If FoundDatum is valid, its sets the value at the index in the datum to the deserialized JsonValue. If not valid, appends an string datum
			 * @param CurrentScope The current scope to append a datum to
			 * @param FoundDatum The datum to set
			 * @param Type The type of datum FoundDatum is
			 * @param Name The name of the datum to append
			 * @param JsonValue The String
			*/
			void AssignScopeAnItem(Scope* CurrentScope, Datum* FoundDatum, const std::string& Name, const Json::Value& JsonValue, int Index = 0);
		};

	}
}