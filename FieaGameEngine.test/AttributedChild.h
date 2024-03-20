#pragma once


#include "Attributed.h"
#include "Foo.h"

namespace Fiea
{
	namespace GameEngine
	{

		class AttributedChild : public Attributed
		{

		public:
			virtual const std::vector<Signature>* RegisterAttributeSignatures() override;

			AttributedChild();

			AttributedChild(const AttributedChild& Other);

			virtual ~AttributedChild();

			virtual [[nodiscard]] AttributedChild* Clone() const override;

			AttributedChild& operator=(const AttributedChild& Other);
			AttributedChild& operator=(AttributedChild&& Other) noexcept;

			void CreatePrescribedAttributesAgain();

			Foo* _Foo;
			int _Int = 0;
			float _Float = 0;
			std::string _String;
			glm::vec4 _Vec4 = glm::vec4(0);
			glm::mat4x4 _Mat4x4 = glm::mat4x4(0);

			Foo* _FooArray[3];
			int _IntArray[3] = { 0, 1, 2 };
			float _FloatArray[3] = { 0, 1, 2 };
			std::string _StringArray[3] = {std::string("0"), std::string("1"), std::string("2")};
			glm::vec4 _Vec4Array[3] = { glm::vec4(0), glm::vec4(1), glm::vec4(2)};
			glm::mat4x4 _Mat4x4Array[3] = { glm::mat4x4(0), glm::mat4x4(1), glm::mat4x4(2) };
		};
	}
}