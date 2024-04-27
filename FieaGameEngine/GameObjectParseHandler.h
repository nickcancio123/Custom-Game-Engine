#pragma once

#include "ScopeParseHandler.h"
#include "ParseCoordinator.h"

namespace Fiea
{
	namespace GameEngine
	{
		using BaseWrapper = ParseCoordinator::BaseWrapper;



		class GameObjectParseHandler : public ScopeParseHandler
		{

		public:

			virtual bool Start(BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue) override;

			virtual bool End(BaseWrapper& Wrapper, const std::string& Name) override;
		};

	}
}