#pragma once

#include <string>
#include "glm/glm.hpp"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#define MIN_ALLOC 10
#define AUTO_RESIZE_FACTOR 2

#define INVALID_TYPE_MSG "Invalid datum type"


namespace Fiea
{

	namespace GameEngine
	{
		class Scope;
		class RTTI;


		class Datum
		{
		public:

			/**
			 * @brief Enum used to specify a datums homogeneous type
			*/
			enum Type
			{
				UKNOWN,
				SCOPE,
				POINTER,
				INT,
				FLOAT,
				STRING,
				VEC4,
				MAT4X4
			};

			/**
			 * @brief Polymorphic pointer to supported data types
			*/
			union DataUnion
			{
				void* Unknown = nullptr;
				Scope* Scope;
				RTTI* RTTI;
				int* Int;
				float* Float;
				std::string* String;
				glm::vec4* Vec4;
				glm::mat4x4* Mat4x4;
			};


			#pragma region === Constructors & Destructors ===

			/**
			 * @brief Default constructor: creates datum with unknown type
			*/
			Datum();

			/**
			 * @brief Creates empty datum with initial type.
			*/
			Datum(const Type InitType);

			/**
			 * @brief Copy constructor: copies other data (item-wise deep copy), size, and type. 
			*/
			Datum(const Datum& Other);

			/**
			 * @brief Move constructor: transfers data ownership from other to this.
			 * @param Other Another datum
			*/
			Datum(Datum&& Other) noexcept;

			/**
			 * @brief Frees pointed-to data and array of DataUnions.
			*/
			~Datum();

			#pragma endregion


			#pragma region === Operators ===

			/**
			 * @brief Copies other data (item-wise deep copy), size, and type. 
			 * @return This datum.
			*/
			Datum& operator= (const Datum& Other);

			/**
			 * @brief Move assignment: Clears previous data and transfers owner ships of other data to this datum
			 * @param Other Another datum
			*/
			Datum& operator= (Datum&& Other);

			/**
			 * @brief Checks if two datums are equal
			*/
			bool operator== (const Datum& Other) const;

			/**
			 * @brief Checks if two datums are NOT equal
			*/
			bool operator!= (const Datum& Other) const;

			/**
			 * @brief Gets the stored scope at the given index. Throws if incorrect type.
			*/
			Scope& operator[](const unsigned int Index);

			#pragma endregion


			#pragma region === PushBack ===

			/**
			 * @brief Adds a new item to the end of the container. Checks that datum type is correct first
			 * @param NewItem Scope*
			 * @return If successfully added item
			*/
			bool PushBack(Scope* NewItem);

			/**
			 * @brief Adds a new item to the end of the container. Checks that datum type is correct first
			 * @param NewItem RTTI*
			 * @return If successfully added item
			*/
			bool PushBack(RTTI* NewItem);

			/**
			 * @brief Adds a new item to the end of the container. Checks that datum type is correct first
			 * @param NewItem Int
			 * @return If successfully added item
			*/
			bool PushBack(const int NewItem);

			/**
			 * @brief Adds a new item to the end of the container. Checks that datum type is correct first
			 * @param NewItem FLOAT
			 * @return If successfully added item
			*/
			bool PushBack(const float NewItem);

			/**
			 * @brief Adds a new item to the end of the container. Checks that datum type is correct first
			 * @param NewItem STRING
			 * @return If successfully added item
			*/
			bool PushBack(const std::string NewItem);

			/**
			 * @brief Adds a new item to the end of the container. Checks that datum type is correct first
			 * @param NewItem vector4
			 * @return If successfully added item
			*/
			bool PushBack(const glm::vec4 NewItem);

			/**
			 * @brief Adds a new item to the end of the container. Checks that datum type is correct first
			 * @param NewItem 4x4 matrix
			 * @return If successfully added item
			*/
			bool PushBack(const glm::mat4x4 NewItem);

			#pragma endregion


			/**
			 * @brief Removes the last item in the container, decrementing size
			 * @return If successfully removed last item
			*/
			bool PopBack();

			/**
			 * @brief Removes the scope at the index from the array. Decrements size
			 * @return The removed scope
			*/
			[[nodiscard]] Scope* RemoveScopeAt(const unsigned int Index);

			/**
			 * @brief Removes all items from container
			*/
			void Clear();

			/**
			 * @brief Sets the size and capacity of the array to NewSize.
			 * @return If successfully resized
			*/
			bool Resize(const unsigned int NewSize);


			#pragma region === Store External Data ===

			/**
			 * @brief Convert this datum into a wrapper of external unsigned ints 
			 * @param Size Number of items to wrap
			*/
			bool StoreExternal(int* ExternalItems, const unsigned int Size = 1);

			/**
			 * @brief Convert this datum into a wrapper of external floats
			 * @param Size Number of items to wrap
			*/
			bool StoreExternal(float* ExternalItems, const unsigned int Size = 1);

			/**
			 * @brief Convert this datum into a wrapper of external strings
			 * @param Size Number of items to wrap
			*/
			bool StoreExternal(std::string* ExternalItems, const unsigned int Size = 1);
			
			/**
			 * @brief Convert this datum into a wrapper of external vector4s
			 * @param Size Number of items to wrap
			*/
			bool StoreExternal(glm::vec4* ExternalItems, const unsigned int Size = 1);

			/**
			 * @brief Convert this datum into a wrapper of external 4x4 matrices
			 * @param Size Number of items to wrap
			*/
			bool StoreExternal(glm::mat4x4* ExternalItems, const unsigned int Size = 1);

			#pragma endregion


			#pragma region === Setters ===

			/**
			 * @brief Sets Datum type.
			 * @param NewType The new type this datum will contain.
			 * @return If successfully set type
			*/
			bool SetType(const Type NewType);

			/**
			 * @brief Set the value of an unsigned int at index
			 * @return If successfully set item
			*/
			bool Set(Scope* Value, const unsigned int ItemIndex = 0);

			/**
			 * @brief Set the value of a pointer to an RTTI
			 * @return If successfully set item
			*/
			bool Set(RTTI* Value, const unsigned int ItemIndex = 0);

			/**
			 * @brief Set the value of an unsigned int at index
			 * @return If successfully set item
			*/
			bool Set(const int Value, const unsigned int ItemIndex = 0);

			/**
			 * @brief Set the value of a float at index
			 * @return If successfully set item
			*/
			bool Set(const float Value, const unsigned int ItemIndex = 0);

			/**
			 * @brief Set the value of a string at index
			 * @return If successfully set item
			*/
			bool Set(const std::string Value, const unsigned int ItemIndex = 0);

			/**
			 * @brief Set the value of a vector4 at index
			 * @return If successfully set item
			*/
			bool Set(const glm::vec4 Value, const unsigned int ItemIndex = 0);

			/**
			 * @brief Set the value of a 4x4 matrix at index
			 * @return If successfully set item
			*/
			bool Set(const glm::mat4x4 Value, const unsigned int ItemIndex = 0);


			/**
			 * @brief Set an item of set datum type by passing a serialized version of it.
			 * @param SerializedItem Item in string form
			 * @return If successfully set item
			*/
			bool SetSerializedItem(std::string SerializedItem, const unsigned int ItemIndex = 0);
			
			#pragma endregion


			#pragma region === Accessors ===

			/**
			 * @brief Gets item at index. Checks if correct datum type and valid index.
			 * @param ItemIndex Ordinal location of item
			 * @return Scope at index
			*/
			Scope* GetScope(const unsigned int ItemIndex = 0) const;

			/**
			 * @brief Gets item at index. Checks if correct datum type and valid index.
			 * @param ItemIndex Ordinal location of item
			 * @return RTTI pointer at index
			*/
			RTTI* GetPointer(const unsigned int ItemIndex = 0) const;

			/**
			 * @brief Gets item at index. Checks if correct datum type and valid index.
			 * @param ItemIndex Ordinal location of item 
			 * @return Unsigned int at index
			*/
			int GetInt(const unsigned int ItemIndex = 0) const;

			/**
			 * @brief Gets item at index. Checks if correct datum type and valid index.
			 * @param ItemIndex Ordinal location of item
			 * @return Float at index
			*/
			float GetFloat(const unsigned int ItemIndex = 0) const;

			/**
			 * @brief Gets item at index. Checks if correct datum type and valid index.
			 * @param ItemIndex Ordinal location of item
			 * @return String at index
			*/
			std::string GetString(const unsigned int ItemIndex = 0) const;

			/**
			 * @brief Gets item at index. Checks if correct datum type and valid index.
			 * @param ItemIndex Ordinal location of item
			 * @return Vector4 at index
			*/
			glm::vec4 GetVec4(const unsigned int ItemIndex = 0) const;

			/**
			 * @brief Gets item at index. Checks if correct datum type and valid index.
			 * @param ItemIndex Ordinal location of item
			 * @return Matrix4x4 at index
			*/
			glm::mat4x4 GetMat4x4(const unsigned int ItemIndex = 0) const;

			/**
			 * @brief Gets item at index in serialized form.
			 * @param ItemIndex Ordinal location of item
			*/
			std::string GetSerializedItem(const unsigned int ItemIndex = 0) const;

			/**
			 * @brief The number of items in the container
			*/
			unsigned int Size() const;

			/**
			 * @brief Get the Datum type
			*/
			Type GetType() const;

			#pragma endregion


		private:
			Type _Type = Type::UKNOWN;

			DataUnion* _Data = nullptr;

			unsigned int _Size = 0;
			unsigned int _Capacity = 0;

			/**
			 * @brief Whether this datum is storing (wrapping) external data
			*/
			bool _External = false;

			/**
			 * @brief Increases the capacity of the container.
			 * @param NewCapacity The new capacity to set. Must be greater than current capacity.
			*/
			void Reserve(const unsigned int NewCapacity);

			/**
			 * @brief Reserves new capacity by a factor of current capacity or a minimum capacity if 
			 * current capacity is zero.
			*/
			void AutoResize();

			/**
			 * @brief Copy constructor for when Other datum is external
			*/
			void CopyConstructExternal(const Datum& Other);

			/**
			 * @brief Assignment operator for when Other datum is external
			*/
			Datum& AssignmentOpExternal(const Datum& Other);
		};

	}
}
