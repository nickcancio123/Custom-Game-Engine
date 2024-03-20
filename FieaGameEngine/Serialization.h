#pragma once

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"



namespace Fiea
{
	namespace GameEngine
	{

		inline static std::string SerializeInt(const int Item)
		{
			return std::to_string(Item);
		}

		inline static std::string SerializeFloat(const float Item)
		{
			return std::to_string(Item);
		}

		inline static std::string SerializeString(const std::string Item)
		{
			return Item;
		}

		inline static std::string SerializeVec4(const glm::vec4 Item)
		{
			std::string Out = SerializeFloat(Item.x);
			Out.append(", ");
			Out.append(SerializeFloat(Item.y));
			Out.append(", ");
			Out.append(SerializeFloat(Item.z));
			Out.append(", ");
			Out.append(SerializeFloat(Item.w));

			return Out;
		}

		inline static std::string SerializeMat4x4(const glm::mat4x4 M)
		{
			auto v1 = M[0];
			auto v2 = M[1];
			auto v3 = M[2];
			auto v4 = M[3];

			std::string Out = SerializeVec4(glm::vec4(v1[0], v1[1], v1[2], v1[3]));
			Out.append(", ");
			Out.append(SerializeVec4(glm::vec4(v2[0], v2[1], v2[2], v2[3])));
			Out.append(", ");
			Out.append(SerializeVec4(glm::vec4(v3[0], v3[1], v3[2], v3[3])));
			Out.append(", ");
			Out.append(SerializeVec4(glm::vec4(v4[0], v4[1], v4[2], v4[3])));

			return Out;
		}



		inline static int DeserializeInt(std::string String)
		{
			unsigned int UInt = 0;
			int ParsedItems = sscanf_s(String.c_str(), "%u", &UInt);

			if (ParsedItems < 1)
			{
				throw std::runtime_error("Failed to deserialize");
			}

			return UInt;
		}

		inline static float DeserializeFloat(std::string String)
		{
			float Float = 0;
			int ParsedItems = sscanf_s(String.c_str(), "%f", &Float);

			if (ParsedItems < 1)
			{
				throw std::runtime_error("Failed to deserialize");
			}

			return Float;
		}

		inline static std::string DeserializeString(std::string String)
		{
			return String;
		}

		inline static glm::vec4 DeserializeVec4(std::string String)
		{
			float x;
			float y;
			float z;
			float w;

			int ParsedItems = sscanf_s(String.c_str(), "%f %*s %f %*s %f %*s %f", &x, &y, &z, &w);

			if (ParsedItems < 4)
			{
				throw std::runtime_error("Failed to deserialize");
			}

			return glm::vec4(x, y, z, w);
		}

		inline static glm::mat4x4 DeserializeMat4x4(std::string String)
		{
			glm::vec4 v1;
			glm::vec4 v2;
			glm::vec4 v3;
			glm::vec4 v4;

			int ParsedItems = sscanf_s(
				String.c_str(),
				"%f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f %*s %f",
				&v1.x, &v1.y, &v1.z, &v1.w,
				&v2.x, &v2.y, &v2.z, &v2.w,
				&v3.x, &v3.y, &v3.z, &v3.w,
				&v4.x, &v4.y, &v4.z, &v4.w
			);

			if (ParsedItems < 16)
			{
				throw std::runtime_error("Failed to deserialize");
			}

			return glm::mat4x4(v1, v2, v3, v4);
		}


	}
}