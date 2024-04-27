#pragma once

#include "Scope.h"
#include "Signature.h"
#include <deque>
#include <memory>


namespace Fiea
{
	namespace GameEngine
	{
		using std::shared_ptr;
		using std::deque;

		class Attributed : public Scope
		{
			RTTI_DECLARATIONS(Attributed, Scope);

		public:

			#pragma region === Constructors & Destructors ===

			Attributed(size_t TypeId);

			/**
			 * @brief Copy constructor: performs a deep copy of another attributed
			 * @param Other The other attributed
			*/
			Attributed(const Attributed& Other);

			/**
			 * @brief Move constructor: steals ownership from the other attributed
			 * @param Other The other attributed
			*/
			Attributed(Attributed&& Other) noexcept;

			virtual ~Attributed() override = default;

			#pragma endregion


			#pragma region === Operators ===

			/**
			 * @brief If this attributed does not have the same attributes as other, minus "this" attribute
			 * @param Other Other attributed
			*/
			bool operator!=(const Attributed& Other) const;

			/**
			 * @brief Copy assignment: deep copies the other attributed
			 * @param Other The other attributed
			 * @return This
			*/
			Attributed& operator=(const Attributed& Other);

			/**
			 * @brief Move assignment: steals ownership from other attributed
			 * @param Other The other attributed
			 * @return This
			*/
			Attributed& operator=(Attributed&& Other) noexcept;

			#pragma endregion

			/**
			 * @brief Returns a new attributed that is a deep copy of this
			 * @return The new attributed
			*/
			virtual Attributed* Clone() const override = 0;

			/**
			 * @brief Attempts to append a datum with the given name. Checks if there already exists a 
			 * prescribed attribute with given name, if so throws runtime error.
			 * @param Name Name of attribute
			 * @return Appended datum
			*/
			Datum& AppendAuxilliaryAttribute(const std::string Name);


			#pragma region === Attribute Checks ===

			/**
			 * @brief Checks if this scope contains a datum with the given name.
			 * @param Name Name of attribute
			*/
			bool IsAttribute(const std::string& Name);

			/**
			 * @brief Checks if there is a prescribed attribute signature with the given name
			 * @param Name Name of attribute
			*/
			bool IsPrescribedAttribute(const std::string& Name);

			/**
			 * @brief Checks if this scope contains a datum with the given name that is NOT a prescribed attribute
			 * @param Name Name of attribute
			*/
			bool IsAuxilliaryAttribute(const std::string& Name);

			#pragma endregion


		protected:

			/**
			 * @brief The names of auxilliary attributes, guaranteed to be made. Is a container of names and not signatures, 
			 * because no way of knowing what attribute type will be
			*/
			std::vector<std::string> _AuxilliaryAttributeNames;

			/**
			 * @brief Appends datums based on prescribed attribute signatures
			*/
			void CreatePrescribedAttributes(size_t TypeId);
		};
	}
}