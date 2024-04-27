#pragma once

#include <list>
#include <memory>
#include "json/json.h"
#include "RTTI.h"


namespace Fiea
{
	namespace GameEngine
	{
		class ParseHandler;


		class ParseCoordinator
		{

		public:
			
			class BaseWrapper : public RTTI
			{
				RTTI_DECLARATIONS(BaseWrapper, RTTI);

			public:

				virtual ~BaseWrapper() = default;

				/**
				 * @brief Initializes the state of the wrapper. Called when parsing begins.
				*/
				virtual void Initialize();

				/**
				 * @brief Cleans up the state of the wrapper. Called when parsing finishes.
				*/
				virtual void Cleanup();
			};

			/**
			 * @brief Initializes the wrapper reference
			 * @param Wrapper The wrapper reference
			*/
			ParseCoordinator(BaseWrapper& Wrapper);

			/**
			 * @brief Adds a parse handler to the end of the handler list
			 * @param NewHandler The handler reference
			*/
			void AddParseHandler(std::shared_ptr<ParseHandler> NewHandler);

			/**
			 * @brief Removes the given parse handler from the list of parse handlers
			 * @param HandlerToRemove The handler to remove
			*/
			void RemoveParseHandler(std::shared_ptr<ParseHandler> HandlerToRemove);


			#pragma region === Deserialization ===

			/**
			 * @brief Converts the string into a string stream and deserializes it as that
			 * @param StringObject String representation of a json object
			*/
			void DeserializeObject(const std::string& StringObject);

			/**
			 * @brief Opens a json file by the given path name and deserializes it as a fstream
			 * @param FileName The path of the json file
			*/
			void DeserializeObjectFromFile(const std::string& FileName);

			/**
			 * @brief Parses the json object using the list of parse handlers
			 * @param Stream The stream of data containing a json object
			*/
			void DeserializeObject(std::istream& Stream);

			#pragma endregion


			/**
			 * @brief Returns the wrapper reference contained by this
			*/
			BaseWrapper& GetWrapper();

			/**
			 * @brief Sets the contained wrapper reference, calling cleanup on the old wrapper and intialize on the new wrapper 
			 * @param Wrapper The new wrapper reference
			*/
			void SetWrapper(BaseWrapper& Wrapper);

		private:

			ParseCoordinator(const ParseCoordinator& Other) = delete;
			ParseCoordinator(ParseCoordinator&& Other) noexcept = delete;

			ParseCoordinator& operator=(const ParseCoordinator& Other) = delete;
			ParseCoordinator& operator=(ParseCoordinator&& Other) noexcept = delete;

			/**
			 * @brief Parses the members of the given json object
			 * @param JsonValue The json object whose members to parse 
			*/
			void ParseMembers(const Json::Value& JsonValue);

			/**
			 * @brief Looks for a parse handler to parse a json element
			 * @param Name The key of the json element
			 * @param JsonValue The json element
			*/
			void Parse(const std::string& Name, const Json::Value& JsonValue);

			/**
			 * @brief Reference to a wrapper
			*/
			BaseWrapper* _Wrapper = nullptr;
			
			/**
			 * @brief List of parse handlers forming a chain of responsibility
			*/
			std::list<std::shared_ptr<ParseHandler>> _ParseHandlers;
		};
	}
}