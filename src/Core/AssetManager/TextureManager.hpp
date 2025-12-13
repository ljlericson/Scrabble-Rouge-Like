#pragma once
#include <unordered_map>
#include <memory>
#include <string>

#include <SDL3/SDL.h>

namespace Core
{
	namespace SDLBackend { struct Texture; }

	namespace AssetManager
	{
		class TextureManager
		{
		public:
			TextureManager() = default;

			SDLBackend::Texture* newTexture(const std::string& key, SDL_Renderer* renderer, const std::string& fpath);

			SDLBackend::Texture* getTexture(const std::string& key);

			SDLBackend::Texture* getInvalidTex();

		private:
			std::unordered_map<std::string, std::unique_ptr<SDLBackend::Texture>> m_textures;
			std::unique_ptr<SDLBackend::Texture> m_invalidTex;

			SDL_Renderer* mrp_renderer = nullptr; // REFERENCE POINTER NOT OWNED
		};
	}
}