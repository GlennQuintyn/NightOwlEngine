#pragma once
//disabling all warnings that come from this math library
//#pragma warning(push)
//#pragma warning( disable : 26495 )
//#pragma warning( disable : 26812 )
//#pragma warning( disable : 26819 )
//#pragma warning( disable : 4201 )
//#include <glm/glm.hpp>
//#include <SDL.h>
//#pragma warning (pop)
#include <json.hpp>

namespace dae
{
	class EntityLoader
	{
	public:
		EntityLoader(const std::string& filePath);
		~EntityLoader() = default;

		SDL_Rect GetCollider(const std::string& colliderName);


		EntityLoader(const EntityLoader& other) = delete;
		EntityLoader(EntityLoader&& other) = delete;
		EntityLoader& operator=(const EntityLoader& other) = delete;
		EntityLoader& operator=(EntityLoader&& other) = delete;
	private:

		nlohmann::json m_JsonData;
	};
}
