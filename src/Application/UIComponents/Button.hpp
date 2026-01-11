#pragma once
#include <string.h>
#include <memory>

#include <SDL3/SDL.h>

#include "../../Core/AssetManager/AssetManager.hpp"
#include "../../Core/SDLBackend/Renderer.hpp"
#include "../../Core/SDLBackend/SpriteSheet.hpp"
#include "../../Core/SDLBackend/Text.hpp"
#include "../EventSystem/BasicEventObserver.hpp"

namespace App
{
	namespace UIComponents
	{
		class Button : public EventSystem::BasicEventObserver
		{
		public:
			Button() = default;
			Button(const Core::SDLBackend::Renderer& renderer, SDL_FRect rect, const char* title);

			void render(const Core::SDLBackend::Renderer& renderer);

			void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;

			void setButtonText(const std::string& newText);

			bool pressed() const;

			void setActive(bool toggle) { m_active = toggle; };
			
		private:
			Core::SDLBackend::SpriteSheet m_tex;
			Core::SDLBackend::Text m_text;
			SDL_FRect m_texRect;
			SDL_FRect m_texSrcRect;

			bool m_pressed = false;
			mutable bool m_handled = false;
			bool m_active = true;
		};
	}
}