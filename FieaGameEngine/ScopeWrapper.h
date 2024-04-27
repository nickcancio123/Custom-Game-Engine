#pragma once

#include "ParseCoordinator.h"
#include "Scope.h"


namespace Fiea
{
	namespace GameEngine
	{
		using BaseWrapper = ParseCoordinator::BaseWrapper;


		class ScopeWrapper : public BaseWrapper
		{
			RTTI_DECLARATIONS(ScopeWrapper, BaseWrapper);


		public:

			/**
			 * @brief Initializes current scope to root scope
			*/
			virtual void Initialize() override;

			/**
			 * @brief Does nothign ;)
			*/
			virtual void Cleanup() override;

			/**
			 * @brief Gets the root scope being wrapped
			*/
			Scope& GetRoot();
			
			/**
			 * @brief Gets the current scope being populated
			*/
			Scope* GetCurrent();

			/**
			 * @brief Sets the current scope to populate
			 * @param NewCurrentScope Should be child of root scope
			*/
			void SetCurrent(Scope& NewCurrentScope);

			/**
			 * @brief Populates a parse coordinator with necessary parse handlers to make a scope
			 * @param PC 
			*/
			void AddScopeParseHandlers(ParseCoordinator& PC);

		private:

			/**
			 * @brief The root scope being wrapped
			*/
			Scope _RootScope;

			/**
			 * @brief The current scope being populated. Must be child scope of root scope.
			*/
			Scope* _CurrentScope;
		};

	}
}