#include "BurgerTimePCH.h"
#include "EntityLoader.h"
#include <fstream>
#include <filesystem>

dae::EntityLoader::EntityLoader(const std::string& filePath)
	: m_JsonData{}
{
	try
	{
		if (std::filesystem::exists(filePath))
		{
			std::ifstream jsonFile{ filePath };
			jsonFile >> m_JsonData;
		}
	}
	catch (const std::exception& e)
	{
		//e.what();

		using namespace std::string_literals;
		Logger::GetInstance().LogError("ENTITYLOADER: FAILED TO LOAD OR PARSE JSON FILE!"s + e.what());
	}
}

SDL_Rect dae::EntityLoader::GetCollider(const std::string& colliderName)
{
	auto& collider = m_JsonData["ColliderBoxes"][colliderName];
	return SDL_Rect(collider[0], collider[1], collider[2], collider[3]);
}
