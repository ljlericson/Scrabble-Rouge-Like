#pragma once


#include <SDL3/SDL.h>

#include "../../Core/AssetManager/AssetManager.hpp"
#include "../../Core/SDL2Backend/Renderer.hpp"
#include "../../Core/SDL2Backend/Texture.hpp"
#include "../EventSystem/BasicEventObserver.hpp"

namespace App
{
	namespace UIComponents
	{
		class Button : public EventSystem::BasicEventObserver
		{
		public:
			Button() = default;
			Button(const Core::SDLBackend::Renderer& renderer, SDL_FRect rect);

			void render(const Core::SDLBackend::Renderer& renderer);

			void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;
			
		private:
			Core::SDLBackend::Texture* m_tex;
			SDL_FRect m_texRect;
			SDL_FRect m_texSrcRect;

			bool m_pressed = false;
		};
	}
}