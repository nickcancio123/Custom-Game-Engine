#pragma once

#include "Attributed.h"
#include "Factory.h"


namespace Fiea
{
	namespace GameEngine
	{
		class Action;
		class ActionList;

		struct GameTime;


		struct Transform
		{
			glm::vec4 _Pos;
			glm::vec4 _Rot;
			glm::vec4 _Scale;
		};


		class GameObject : public Attributed
		{
			RTTI_DECLARATIONS(GameObject, Attributed);

		public:

			GameObject(IdType TypeId = TypeIdClass());

			virtual GameObject* Clone() const override;

			/**
			 * @brief The prescribed attribute signatures of this class
			*/
			static shared_ptr<deque<Signature>> Signatures();

			/**
			 * @brief Calls update on self and all of its children
			 * @param Time The game time wrapper
			*/
			virtual void Update(const GameTime& Time);

			/**
			 * @brief Adopts the child using scope adopt function call.
			 * @param NewChild The child to adopt.
			*/
			void AddChild(GameObject& NewChild);

			/**
			 * @brief Creates an action using factory of ClassName, adding to and giving ownership to action list
			 * @param ClassName The name of the action class to create
			 * @param InstanceName The name of the action instance created
			*/
			Action* CreateAction(const std::string& ClassName, const std::string& InstanceName);


			#pragma region === Accessors ===

			GameObject* GetChild(const std::string& Name);

			Transform& GetTransform();

			const Transform& GetTransform() const;

			const std::string& GetName() const;

			const ActionList* GetActions() const;

			#pragma endregion


		private:

			/**
			 * @brief The world space information of this gameobject
			*/
			Transform _Transform;

			/**
			 * @brief The name of this gameobject instance
			*/
			std::string _Name;

			/**
			 * @brief The list of actions owned by this gameobject by proxy of ActionList
			*/
			ActionList* _Actions;

			/**
			 * @brief A map for quick lookup by name
			*/
			std::unordered_map<std::string, GameObject*> _ChildrenLookup;
		};

		MAKE_FACTORY(Scope, GameObject)
	}
}