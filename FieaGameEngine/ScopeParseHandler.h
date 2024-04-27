#pragma once


#include "ParseHandler.h"
#include "ParseCoordinator.h"
#include "Scope.h"
#include <stack>

namespace Fiea
{
	namespace GameEngine
	{

		class ScopeParseHandler : public ParseHandler
		{

		public:
			/**
			 * @brief If wrapper is a scope wrapper, pushes wrapper root scope onto scope stack
			 * @param Wrapper The wrapper this parse handler may operate on
			*/
			virtual void Initialize(ParseCoordinator::BaseWrapper& Wrapper) override;

			/**
			 * @brief Clears scope stack
			*/
			virtual void Cleanup() override;

			/**
			 * @brief Creates a new scope or scope subclass (if has type member).
			 * @return If wrapper and JsonValue can be parsed by this.
			*/
			virtual bool Start(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name, const Json::Value& JsonValue) override;

			/**
			 * @brief Pops the top scope in the scope stack and sets the wrappers current scope to the new top of the stack
			*/
			virtual bool End(ParseCoordinator::BaseWrapper& Wrapper, const std::string& Name) override;

			/**
			 * @brief Looks for member value with key "type", and returns its value as a string
			 * @param JsonValue The value whose members to look through
			 * @param TypeName Out variable for found type as string
			 * @return If found type member
			*/
			bool FindTypeName(const Json::Value JsonValue, std::string& TypeName);

		protected:

			/**
			 * @brief Keeps track of scope tree. The scope currently being populated is at the top of the stack.
			*/
			std::stack<Scope*> ScopeStack;
		};

	}
}