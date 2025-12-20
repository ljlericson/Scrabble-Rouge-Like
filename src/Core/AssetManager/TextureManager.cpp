#include "TextureManager.hpp"
#include "../SDL2Backend/Texture.hpp"

namespace Core
{
	namespace AssetManager
	{
		SDLBackend::Texture* TextureManager::newTexture(const std::string& key, SDL_Renderer* renderer, const std::string& fpath)
		{
			if (!mrp_renderer)
				mrp_renderer = renderer;

			// avoid texture creation if texture exists
			if(m_textures.contains(key))
				return m_textures.at(key).get();

			auto tex = std::make_unique<SDLBackend::Texture>(renderer, fpath);

			if (tex->texHand)
			{
				m_textures.insert({ key, std::move(tex) });
				
				return m_textures.at(key).get();
			}
			// texture is invalid so we return the invalid tex
			else
			{
				return this->getInvalidTex();
			}
		}

		SDLBackend::Texture* TextureManager::newTexture(const std::string& key, SDL_Renderer* renderer, SDL_Texture* texture)
		{
			if (!mrp_renderer)
				mrp_renderer = renderer;

			// avoid texture creation if texture exists
			if (m_textures.contains(key))
				return m_textures.at(key).get();

			auto tex = std::make_unique<SDLBackend::Texture>(texture);

			if (tex->texHand)
			{
				m_textures.insert({ key, std::move(tex) });

				return m_textures.at(key).get();
			}
			// texture is invalid so we return the invalid tex
			else
			{
				return this->getInvalidTex();
			}
		}

		SDLBackend::Texture* TextureManager::getTexture(const std::string& key)
		{
			if (m_textures.contains(key))
				return m_textures.at(key).get();
			
			else
			{
				std::cout << "WARNING: NO TEXTURE FOUND TextureManager::getTexture()\n";
				return this->getInvalidTex();
			}
		}

		SDLBackend::Texture* TextureManager::getInvalidTex()
		{
			if (!m_invalidTex)
				m_invalidTex = std::make_unique<SDLBackend::Texture>(mrp_renderer, "./assets/textures/no_texture.png");

			return m_invalidTex.get();
		}
	}
}