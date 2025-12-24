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

		Button::Button(const Core::SDLBackend::Renderer& renderer, SDL_FRect rect, const char* title)
			: m_tex(Core::AssetManager::textureManager->newTexture("BUTTON", renderer.getRendHand(), "./assets/textures/button.png")),
			m_texRect(rect)
		{
			m_texSrcRect.h = 104.0f;
			m_texSrcRect.w = 111.0f;
			m_texSrcRect.x = 0;
			m_texSrcRect.y = 0;
			
			m_text = Core::SDLBackend::Text(
				glm::vec2{ m_texSrcRect.x, m_texSrcRect.y }, 
				m_texSrcRect.w, 
				m_texSrcRect.h, 
				"./assets/font.ttf",
				SDL_Color(255, 0, 0, 255), 
				title
			);
		}

		void Button::render(const Core::SDLBackend::Renderer& renderer) 
		{
			if (m_pressed)
				m_texSrcRect = { 0, 0, 111.0f, 104.0f / 2 };
			else
				m_texSrcRect = { 0, 104.0f / 2, 111.0f, 104.0f / 2 };

			renderer.render(*m_tex, m_texRect, &m_texSrcRect);
			m_text.getRectToSetPosManually() = SDL_FRect{ .x = m_texRect.x + ((m_texRect.w - m_texRect.w * 0.9f) / 2.0f), 
												   .y = m_texRect.y + (m_texRect.h / 4) + (m_pressed ? 2.0f : 0.0f),
												   .w = m_texRect.w * 0.9f, 
												   .h = (m_texRect.h / 2) };
			m_text.render(renderer);
		}

		void Button::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
		{
			if (keyboardState[SDL_SCANCODE_N])
			{
				SDL_Color col = m_text.getTextColor();
				m_text.changeColor(SDL_Color(col.r, col.g + 10, col.b, col.a));
			}

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
					m_handled = false;
				}
			}
			else if (std::find(events.begin(), events.end(), SDL_EVENT_MOUSE_BUTTON_UP) != events.end())
				m_pressed = false;
		}

		void Button::setButtonText(const std::string& newText)
		{
			m_text.setText(newText);
		}

		bool Button::pressed() const
		{
			if (!m_handled)
			{
				m_handled = true;
				return m_pressed;
			}

			return false;
		}

	}
}