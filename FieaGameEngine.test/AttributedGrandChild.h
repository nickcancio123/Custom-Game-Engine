#pragma once

#include "AttributedChild.h"


namespace Fiea
{
	namespace GameEngine
	{
		namespace Test
		{

			class AttributedGrandChild : public AttributedChild
			{
				RTTI_DECLARATIONS(AttributedGrandChild, AttributedChild);

			public:
				AttributedGrandChild();

				AttributedGrandChild(const AttributedGrandChild& Other);

				virtual [[nodiscard]] AttributedGrandChild* Clone() const override;

				static shared_ptr<deque<Signature>> Signatures();

				int _SecondInt = 0;
				float _SecondFloat = 0;
			};

		}
	}
}