#pragma once

#pragma once


#include "ActionList.h"
#include "Factory.h"


namespace Fiea
{
	namespace GameEngine
	{
		struct GameTime;

		/**
		 * @brief An actionlist that updates its list of actions until a condition is met
		*/
		class ActionWhile : public ActionList
		{
			RTTI_DECLARATIONS(ActionWhile, ActionList);

		public:

			ActionWhile(IdType TypeId = TypeIdClass()) : ActionList(TypeId) {}

			/**
			 * @brief The prescribed attribute signatures of this class
			*/
			static shared_ptr<deque<Signature>> Signatures();

			virtual ActionWhile* Clone() const override;

			/**
			 * @brief Calls update on preamble action, then calls update on action list and increment
			 * until condition is zero. Ignores marked for delete on preamble, loop outer, and increment to 
			 * avoid infinite loops
			*/
			virtual void Update(const GameTime& Time) override;

			/**
			 * @brief Sets the action which updates once before looping over action list
			*/
			void SetPreamble(Action& Preamble);

			/**
			 * @brief Sets the action to update after every update iteration of action list
			*/
			void SetIncrement(Action& Increment);

		private:

			/**
			 * @brief The value used to devide to execute or break loop
			*/
			int _Condition = 0;

			/**
			 * @brief The action to take before beginning the loop
			*/
			Action* _Preamble = nullptr;

			/**
			 * @brief The action to take at the end of each iteration
			*/
			Action* _Increment = nullptr;
		};


		MAKE_FACTORY(Scope, ActionWhile);
	}
}