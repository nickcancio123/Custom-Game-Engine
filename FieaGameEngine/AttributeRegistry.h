#pragma once

#include <unordered_map>
#include <vector>
#include "RTTI.h"
#include "Signature.h"


namespace Fiea
{
	namespace GameEngine
	{

		/**
		 * @brief A singleton class for registering Attributed classes' attribute signature lists
		*/
		class AttributeRegistry
		{
		public:
			/**
			 * @brief If the singleton instance is not yet made, heap instantiate a new one.
			*/
			static void Create();

			/**
			 * @brief Gets the singleton instance, creating one if not yet made.
			 * @return The singleton instance
			*/
			static AttributeRegistry* GetInstance();

			/**
			 * @brief Delete singleton instance.
			*/
			static void Destroy();

			/**
			 * @brief Inserts the given pair of ID and signature list into the registry map
			 * @param ID The type ID
			 * @param Signatures The list of attribute signatures of the type class
			 * @return The address of the added list of signatures
			*/
			const std::vector<Signature>* RegisterAttributeSignatures(RTTI::IdType ID, std::vector<Signature> Signatures);

			/**
			 * @brief Returns the list of attribute signatures associated with the given type ID, if any
			 * @param ID The type ID
			*/
			const std::vector<Signature>* GetAttributeSignatures(RTTI::IdType ID) const;


		private:
			inline static AttributeRegistry* Instance = nullptr;

			std::unordered_map<RTTI::IdType, std::vector<Signature>> TypeToSignatures;

			AttributeRegistry() = default;
		};
	}
}