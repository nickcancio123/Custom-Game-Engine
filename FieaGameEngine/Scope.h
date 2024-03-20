#pragma once

#include "RTTI.h"
#include "Datum.h"
#include <unordered_map>
#include <vector>
#include <list>
		


namespace Fiea
{
	namespace GameEngine
	{

		class Scope : public RTTI
		{
			RTTI_DECLARATIONS(Scope, RTTI);

		public:

			#pragma region === Constructors & Desctructors ==

			/**
			 * @brief Constructor: Reserves capacity in containers
			 * @param _Parent The parent scope of this scope. If is nullptr, this scope is a root scope
			 * @param _Capacity Initial capacity for data containers
			*/
			Scope(const unsigned int Capacity = 0);

			/**
			 * @brief Copy constructor: performs a deep copy of other onto this.
			 * @param Other The other scope to copy from
			*/
			Scope(const Scope& Other);

			/**
			 * @brief Move constructor: Takes ownership of allocated memory of other scope
			 * @param Other The scope to steal ownership from
			*/
			Scope(Scope&& Other) noexcept;

			/**
			 * @brief Destructor: Orphans this scope, deletes all child scopes, and then deletes all named datums.
			*/
			virtual ~Scope() override;

			#pragma endregion


			#pragma region === Operators ===

			/**
			 * @brief Assignment operator: Clears this, then deep copies other.
			 * @param Other The other scope to deep copy
			 * @return Reference to self after assignment
			*/
			Scope& operator=(const Scope& Other);

			/**
			 * @brief Move assignment operator: Takes ownership of allocated memory of other scople
			 * @param Other The scope to steal ownership from
			 * @return Reference to self after move assignment operation
			*/
			Scope& operator=(Scope&& Other) noexcept;

			/**
			 * @brief Wraps Append() with convenient syntax
			 * @param Name The key of the retrieved datum
			 * @return The datum at the specified key
			*/
			Datum& operator[](std::string Name);

			/**
			 * @brief Returns the datum by order added
			 * @param Index Ordinal index of a datum
			 * @return The index-th datum to be added to this scope
			*/
			Datum& operator[](unsigned int Index);

			/**
			 * @brief Equality operator: checks for matching size, order, and key-value pairs
			 * @param Other Other Scope to compare to
			 * @return If the two scopes are equal
			*/
			bool operator==(const Scope& Other) const;

			/**
			 * @brief Inequality operator: checks for matching size, order, and key-value pairs
			 * @param Other Other Scope to compare to
			 * @return If the two scopes are NOT equal
			*/
			bool operator!=(const Scope& Other) const;

			#pragma endregion


			/**
			 * @brief Makes or retrieves a datum with name as key.
			 * @param Name The key for returned datum
			 * @return The datum with provided name
			*/
			Datum& Append(const std::string Name);

			/**
			 * @brief Makes a scope at given key/name. If key/name already exists, adds scope to datum.
			 * @param Name Key
			 * @return Reference to created scope
			*/
			Scope& AppendScope(const std::string Name);

			/**
			 * @brief Reparents ChildScope to this.
			 * @param Name The name to assign adopted scope
			 * @param ChildScope The scope to adopt
			*/
			void Adopt(const std::string Name, Scope& ChildScope);

			/**
			 * @brief Creates a deep copy of this scope and childs it to own parent
			 * @return Pointer to deep copy of this
			*/
			virtual [[nodiscard]] Scope* Clone() const;

			/**
			 * @brief Deletes all allocated memory and clears all records of stored datum
			*/
			void Clear();

			/**
			 * @brief Checks if OTHER scope is nested within THIS scope.
			 * @param Other Other scope to look for within this scope.
			*/
			bool IsAncestorOf(const Scope* Other) const;

			/**
			 * @brief Checks if THIS scope is nested within OTHER scope.
			 * @param Other The scope within which we are checking.
			*/
			bool IsDescendantOf(const Scope* Other) const;


			#pragma region === Find & Search ===

			/**
			 * @brief Recursively searches for a scope within this. 
			 * @param ScopeToFind The address of the scope we're looking for.
			 * @param ScopeIndex Out: The index within the datum the scope was found in. If not found, index is max unsigned int
			 * @return The datum within which the scope was found in
			*/
			Datum* FindContainedScope(const Scope* ScopeToFind, unsigned int& ScopeIndex) const;

			/**
			 * @brief Finds specified scope within this scope and returns its name in out param. NOT RECURSIVE.
			 * @param ScopeToFind The scope to find
			 * @param Name Out param: The name associated with given scope 
			 * @return Whether given scope was found within this scope
			*/
			bool FindNameOfContainedScope(const Scope* ScopeToFind, std::string& Name);

			/**
			 * @brief Retrieves the datum, if exists, mapped to given name/key
			 * @param Name The key for desired datum
			 * @return The datum mapped to given key. Returns nullptr if no such datum.
			*/
			Datum* Find(const std::string Name) const;

			/**
			 * @brief Finds the nearest scope-datum with given key in this scope or parents
			 * @param Name The key to find the scope-datum
			 * @param FoundScope Out: pointer to found scope
			 * @return The datum containing the found scope
			*/
			Datum* Search(const std::string Name, Scope** FoundScope = nullptr);

			/**
			 * @brief Finds the nearest scope-datum with given key in this scope or parents
			 * @param Name The key to find the scope-datum
			 * @param FoundScope Out: pointer to found scope
			 * @return The datum containing the found scope
			*/
			const Datum* Search(const std::string Name, Scope** FoundScope = nullptr) const;

			#pragma endregion


			/**
			 * @brief Returns the parent scope of this. If parent is nullptr, this is a root scope.
			*/
			Scope* GetParent() const;

		protected:
			/**
			 * @brief Maps a name to a datum.
			*/
			std::unordered_map<std::string, Datum*> _NameToDatum;

			/**
			 * @brief Stores names of datum in the order they were added to this scope.
			*/
			std::vector<std::string> _OrderedNames;

			/**
			 * @brief The scope that "owns" this 
			*/
			Scope* _Parent = nullptr;

			/**
			 * @brief Finds child scope and erases reference to it
			 * @param ChildScope
			*/
			void Orphan(Scope* ChildScope);
		};
	}
}