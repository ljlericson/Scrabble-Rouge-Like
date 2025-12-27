#pragma once
#include <memory>

#include "Tile.hpp"
#include "../../Core/AssetManager/AssetManager.hpp"

namespace App
{
	namespace GameComponents
	{
		class Recycler
		{
		public:
			Recycler(const Core::SDLBackend::Renderer& renderer);

			bool inRecycler(const Tile& tile);

			void render(const Core::SDLBackend::Renderer& renderer);

			bool hide();

			void hideHard();

		private:
			std::shared_ptr<Core::SDLBackend::Texture> m_tex;
			SDL_FRect m_texRect;
			glm::vec2 m_posShown;
		};
	}
}