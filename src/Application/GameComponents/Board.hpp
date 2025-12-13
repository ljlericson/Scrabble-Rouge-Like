#pragma once

#include <SDL3/SDL.h>

#include "BasicGameComponent.hpp"
#include "../../Core/SDL2Backend/Renderer.hpp"
#include "../../Core/AssetManager/AssetManager.hpp"
#include "../../Core/SDL2Backend/Texture.hpp"

namespace App
{
	namespace GameComponents
	{
		class Board : public BasicGameComponent
		{
		public:
			Board() = default;

			Board(const Core::SDLBackend::Renderer& renderer);

			void render(const Core::SDLBackend::Renderer& renderer) const override;

			void onInput(const bool* keyboardState) override;

		private:
			Core::SDLBackend::Texture* m_tex = nullptr;
			SDL_FRect m_texRect;
		};
	}
}

