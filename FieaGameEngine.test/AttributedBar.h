#pragma once


#include "Attributed.h"
#include "Foo.h"


namespace Fiea
{
	namespace GameEngine
	{
		namespace Test
		{

			class AttributedBar : public Attributed
			{
				RTTI_DECLARATIONS(AttributedBar, Attributed);

			public:
				AttributedBar(size_t TypeId = TypeIdClass()) : Attributed(TypeId) {}

				AttributedBar(const AttributedBar& Other) = default;

				virtual ~AttributedBar() = default;

				AttributedBar& operator=(const AttributedBar& Other) = default;

				AttributedBar& operator=(AttributedBar&& Other) noexcept = default;

				virtual [[nodiscard]] AttributedBar* Clone() const override;

				static shared_ptr<deque<Signature>> Signatures();

				int _Int = 0;
			};

		}
	}
}