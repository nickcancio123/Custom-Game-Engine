#pragma once

#include <unordered_map>
#include <deque>
#include <memory>
#include "Signature.h"


namespace Fiea
{
	namespace GameEngine
	{
		using std::shared_ptr;
		using std::deque;

		/**
		 * @brief A pure-static class that stores and retrieves the prescribed attribute signatures of classes.
		*/
		class TypeRegistry
		{

		public:
			/**
			 * @brief Stores the given signatures in a map, using the TypeID as a key
			 * @param TypeId The key
			 * @param TypeSignatures Shared pointer to type signatures
			 * @param ParentTypeId Key to parent sigs. If provided will append to new type signatures
			*/
			static void RegisterSignatures(size_t TypeId, shared_ptr<deque<Signature>> NewTypeSignatures, size_t ParentTypeId = -1);

			/**
			 * @brief Returns prescribed attribute signatures of the class associated with the given key
			 * @param TypeId The RTTI type ID of the class
			 * @return The prescribed attribute signatures of the class associated with the given key. Nullptr 
			 * if no such key found.
			*/
			static const shared_ptr<deque<Signature>> GetSignaturesOfType(size_t TypeId);


		private:

			/**
			 * @brief Stores the prescribed attribute signatures of a class, using the class' RTTI type ID as a key
			*/
			inline static std::unordered_map<size_t, shared_ptr<deque<Signature>>> _TypeSignatures {};
		};

	}
}