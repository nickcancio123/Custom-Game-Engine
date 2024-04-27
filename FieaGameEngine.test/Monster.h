#pragma once

#include "GameObject.h"
#include "Factory.h"


namespace Fiea
{
	namespace GameEngine
	{
		namespace Test
		{

			class Monster : public GameObject
			{
				RTTI_DECLARATIONS(Monster, GameObject);

			public: 

				Monster(IdType TypeId = TypeIdClass()) : GameObject(TypeId) {}

				virtual Monster* Clone() const override;

				static shared_ptr<deque<Signature>> Signatures();

				virtual void Update(const GameTime& DeltaTime) override;

				bool UpdateCalled = false;

				float _Health = 0;
				float _HealAmount = 5;
			};

			MAKE_FACTORY(Scope, Monster)
		}
	}
}