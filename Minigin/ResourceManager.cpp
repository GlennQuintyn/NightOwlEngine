#include "NightOwlEnginePCH.h"
#include "ResourceManager.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Texture2D.h"
#include "Font.h"

#include "Renderer.h"

using namespace dae;

void ResourceManager::Init(const std::string_view dataPath)
{
	if (dataPath.empty())
	{
		Logger::GetInstance().LogError("ResourceManager:\tNO DATA PATH WAS GIVEN!");
		return;
	}

	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file)
{
	//TODO: check member container if texture was already loaded before if not make new one if it does give copy of sharedptr

	//checks if the texture already exits in the map if so return the shared ptr to it,
	//if not, create new texture and store its path as key and shared ptr as value
	if (const auto texture = m_TextureMap.find(file); texture != m_TextureMap.end())
	{
		return (*texture).second;
	}
	else
	{
		const auto fullPath = m_DataPath + file;
		auto newTexture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());

		if (newTexture == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
		}

		auto pNewTexture{ std::make_shared<Texture2D>(newTexture) };
		m_TextureMap[file] = pNewTexture;
		return pNewTexture;
	}
}

std::shared_ptr<Font> ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	//TODO: check member container if font was already loaded before if not make new one if it does give copy of sharedptr
	size;

	if (const auto font = m_FontMap.find({ file, size }); font != m_FontMap.end())
	{
		return (*font).second;
	}
	else
	{
		auto newFont{ std::make_shared<Font>(m_DataPath + file, size) };
		m_FontMap[{file, size}] = newFont;
		return newFont;
	}
}
