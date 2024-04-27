#pragma once

#include "Attributed.h"


namespace Fiea
{
	namespace GameEngine
	{
		class GameObject;

		struct GameTime;


		class Action : public Attributed
		{
			RTTI_DECLARATIONS(Action, Attributed);

		public:

			Action(IdType TypeId = TypeIdClass()) : Attributed(TypeId) {}

			/**
			 * @brief The prescribed attribute signatures of this class
			*/
			static shared_ptr<deque<Signature>> Signatures();

			/**
			 * @brief Checks if is marked for delete, if not, calls update on self
			 * @param DeltaTime 
			*/
			void Update_Outer(const GameTime& DeltaTime);

			/**
			 * @brief Child subclasses should implement this to apply changes to subject
			 * @param DeltaTime 
			*/
			virtual void Update(const GameTime& Time) = 0;


			#pragma region === Accessors ===

			/**
			 * @brief The name of this action
			*/
			const std::string& Name() const;

			/**
			 * @brief Whether should update more than once
			*/
			bool IsSingleUpdate() const;

			/**
			 * @brief Whether should be deleted by owner
			*/
			bool MarkedForDelete() const;

			#pragma endregion


			#pragma region === Setters ===
			/**
			 * @brief Set the name of this action
			*/
			void SetName(const std::string& Name);

			/**
			 * @brief Set whether this action should update more than once
			*/
			void SetSingleUpdate(bool SingleUpdate);

			/**
			 * @brief Sets the subject of this action
			 * @return Returns false if action already has a parent
			*/
			bool SetSubject(Attributed* Subject);

			#pragma endregion


		protected:

			/**
			 * @brief The name of this action
			*/
			std::string _Name;

			/**
			 * @brief Whether should update more than once
			*/
			bool _SingleUpdate = false;

			/**
			 * @brief Whether should be deleted by owner
			*/
			bool _MarkedForDelete = false;

			/**
			 * @brief The attributed object that this action will operate on
			*/
			Attributed* _Subject = nullptr;
		};

	}
}