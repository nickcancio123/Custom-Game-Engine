#pragma once

#include "Action.h"
#include "Factory.h"

namespace Fiea
{
	namespace GameEngine
	{
		/**
		 * @brief An action that increments (or decrements) a float or int value on the subject by a value 
		 * contained in a given datum
		*/
		class ActionIncrement : public Action
		{

			RTTI_DECLARATIONS(ActionIncrement, Action);

		public:

			ActionIncrement(IdType TypeId = TypeIdClass()) : Action(TypeId) {}

			/**
			 * @brief Increments an int or float attribute on subject by a given value
			*/
			virtual void Update(const GameTime& DeltaTime) override;

			virtual ActionIncrement* Clone() const override;

			/**
			 * @brief The prescribed attribute signatures of this class
			*/
			static shared_ptr<deque<Signature>> Signatures();

			/**
			 * @brief Set the variables necessary for this action to function
			 * @param AttributeToIncrement Name of attribute to increment owned by subject
			 * @param IncrementAmountDatum The datum containing the value by which to increment
			*/
			void SetValues(const std::string& AttributeToIncrement, Datum& IncrementAmountDatum);

		private:

			/**
			 * @brief The name of the attribute on the subject to modify
			*/
			std::string _AttributeToIncrement;

			/**
			 * @brief The datum containing the value by which to increment 
			*/
			Datum* _IncrementAmountDatum = nullptr;

		};

		MAKE_FACTORY(Scope, ActionIncrement);

	}
}