#pragma once


#include "Action.h"
#include "Factory.h"


namespace Fiea
{
	namespace GameEngine
	{
		struct GameTime;

		/**
		 * @brief An action that owns a set of other actions and calls their updates 
		*/
		class ActionList : public Action
		{
			RTTI_DECLARATIONS(ActionList, Action);
		 
		public:

			ActionList(IdType TypeId = TypeIdClass()) : Action(TypeId) {}

			/**
			 * @brief The prescribed attribute signatures of this class
			*/
			static shared_ptr<deque<Signature>> Signatures();

			virtual ActionList* Clone() const override;

			/**
			 * @brief Calls update on all actions in action list and then deletes actions marked for delete
			*/
			virtual void Update(const GameTime& Time) override;

			/**
			 * @brief Adds an action to datum of actions
			*/
			void AddAction(Action& NewAction);

			/**
			 * @brief Returns the number of actions contained
			*/
			unsigned int Size() const;

		private:

			/**
			 * @brief Delete actions in list that have been marked for delete
			*/
			void DeleteMarkedActions();

			/**
			 * @brief Actions owned by this action list, using their instance names as a key
			*/
			std::unordered_map<std::string, Action*> _Actions;

			/**
			 * @brief Cached actions marked for delete
			*/
			std::vector<Action*> _ActionsToDelete;
		};
	 

		MAKE_FACTORY(Scope, ActionList);
	}
}