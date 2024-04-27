#pragma once

#include "Empty.h"

namespace Fiea
{
	namespace GameEngine
	{
		namespace Test
		{

			class EmptyChild : public Empty
			{
				RTTI_DECLARATIONS(EmptyChild, Empty);
			};
		}
	}
}