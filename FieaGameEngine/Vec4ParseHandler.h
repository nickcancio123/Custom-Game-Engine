#pragma once

#include "DatumParseHandler.h"
#include "ParseCoordinator.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Vec4ParseHandler : public DatumParseHandler
		{

		protected:

			virtual Datum::Type GetType() const override;

			virtual bool IsJsonOfType(const Json::Value& JsonValue) const override;
		};

	}
}