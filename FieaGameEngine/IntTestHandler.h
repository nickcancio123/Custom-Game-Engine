#pragma once

#include "ParseHandler.h"
#include "ParseCoordinator.h"
#include <vector>


namespace Fiea
{
	namespace GameEngine
	{
		namespace Test
		{

			class IntTestWrapper : public ParseCoordinator::BaseWrapper
			{
				RTTI_DECLARATIONS(IntTestWrapper, ParseCoordinator::BaseWrapper);

			public:
				void AddInt(int Int);

				int GetInt(int Index = 0);

				void Clear();

			private:

				std::vector<int> _Ints;
			};

			class IntTestHandler : public ParseHandler
			{
			public:
				virtual bool Start(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue) override;

				virtual bool End(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name) override;
			};

		}
	}
}