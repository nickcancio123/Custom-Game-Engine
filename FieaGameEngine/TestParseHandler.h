#pragma once

#include "ParseHandler.h"
#include "ParseCoordinator.h"

namespace Fiea
{
	namespace GameEngine
	{
		namespace Test
		{
			/**
			 * @brief Wrapper used to test meta functionality of parse coordinator
			*/
			class TestWrapper : public ParseCoordinator::BaseWrapper
			{
				RTTI_DECLARATIONS(TestWrapper, ParseCoordinator::BaseWrapper);

			public:

				bool _Reached = false;
			};

			/**
			 * @brief Handler that only handlers TestWrapper
			*/
			class TestHandler : public ParseHandler
			{
			public:
				virtual bool Start(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue) override;

				virtual bool End(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name) override;
			};

		}
	}
}