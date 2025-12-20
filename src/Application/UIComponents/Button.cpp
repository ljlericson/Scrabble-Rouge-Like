#include "Button.hpp"

namespace App
{
	namespace UIComponents
	{
		/*
		class Button : public EventSystem::BasicEventObserver
		{
		public:
			Button() = default;
			Button(const Core::SDLBackend::Renderer* renderer);

			void render(const Core::SDLBackend::Renderer* renderer);
			
		private:
			Core::SDLBackend::Texture* m_tex;
			SDL_FRect m_texRect;
			SDL_FRect m_texSrcRect;
		};
		*/

		Button::Button(const Core::SDLBackend::Renderer& renderer, SDL_FRect rect)
			: m_tex(Core::AssetManager::textureManager->newTexture("BUTTON", renderer.getRendHand(), "./assets/textures/button.png")),
			m_texRect(rect)
		{
			m_texSrcRect.h = 104.0f;
			m_texSrcRect.w = 111.0f;
			m_texSrcRect.x = 0;
			m_texSrcRect.y = 0;
		}

		void Button::render(const Core::SDLBackend::Renderer& renderer) 
		{
			if (m_pressed)
				m_texSrcRect = { 0, 0, 111.0f, 104.0f / 2 };
			else
				m_texSrcRect = { 0, 104.0f / 2, 111.0f, 104.0f / 2 };

			renderer.render(*m_tex, m_texRect, &m_texSrcRect);
		}

		void Button::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
		{
			if (std::find(events.begin(), events.end(), SDL_EVENT_MOUSE_BUTTON_DOWN) != events.end())
			{
				float x, y;
				const float minX = m_texRect.x;
				const float minY = m_texRect.y;
				const float maxX = m_texRect.x + m_texRect.w;
				const float maxY = m_texRect.y + m_texRect.h;

				SDL_GetMouseState(&x, &y);
				if ((x > minX && x < maxX) &&
					(y > minY && y < maxY))
				{
					m_pressed = true;
				}
			}
			else if (std::find(events.begin(), events.end(), SDL_EVENT_MOUSE_BUTTON_UP) != events.end())
				m_pressed = false;
		}

	}
}