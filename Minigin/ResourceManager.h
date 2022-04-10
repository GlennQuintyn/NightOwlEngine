#pragma once
#include "Singleton.h"
#include <string>
#include <map>

namespace dae
{
	class Texture2D;
	class Font;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string_view data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size);
	private:
		//TODO: add container with loaded textures AND fonts
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;

		std::map<std::string, std::shared_ptr<Texture2D>> m_TextureMap;
		std::map<std::pair<std::string, unsigned int>, std::shared_ptr<Font>> m_FontMap;
	};
}
