#pragma once

#include <string>
#include <unordered_map>
#include "Factory.h"

namespace Fiea::GameEngine
{
	/**
	 * @brief Stores a map of concrete factories pointers, which it owns
	 * @tparam BaseClass The base class of which factories produce subclasses
	*/
	template<class BaseClass>
	class FactoryManager
	{
		
	public:
		/**
		 * @brief Adds a name-factory pair to concrete factory map
		 * @param ClassName The name of the class this new factory makes
		 * @param NewFactory The factory associated with the class with the given name
		*/
		static inline void AddFactory(const std::string& ClassName, Factory<BaseClass>& NewFactory)
		{
			ClassNameToFactory.insert({ ClassName, &NewFactory });
		}

		/**
		 * @brief Finds the concrete factory associated with the given class name
		 * @param ClassName The name a class
		 * @return A pointer to a concrete factory that makes objects of the given class name
		*/
		static inline Factory<BaseClass>* FindFactory(const std::string& ClassName)
		{
			auto FoundItr = ClassNameToFactory.find(ClassName);

			if (FoundItr == ClassNameToFactory.end())
			{
				return nullptr;
			}

			return FoundItr->second;
		}

		/**
		 * @brief Finds a factory associated with the given class name and creates an new heap instance
		 * @param ClassName The name of the class to instantiate
		 * @return Pointer to base class of new instance
		*/
		static inline BaseClass* Create(const std::string& ClassName)
		{
			Factory<BaseClass>* FoundFactory = FindFactory(ClassName);

			if (!FoundFactory)
			{
				return nullptr;
			}

			return FoundFactory->Create();
		}

		/**
		 * @brief Clears contents of static factory map
		 * Note: I'm using this to appease memory leak checks :)
		*/
		static inline void Clear()
		{
			while (!ClassNameToFactory.empty())
			{
				if (ClassNameToFactory.begin()->second)
				{
					delete ClassNameToFactory.begin()->second;
				}

				ClassNameToFactory.erase(ClassNameToFactory.begin());
			}
		}

	private:
		inline static std::unordered_map<std::string, Factory<BaseClass>*> ClassNameToFactory;
	};
}