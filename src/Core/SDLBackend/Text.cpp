#include "Text.hpp"

namespace Core
{
	namespace SDLBackend
	{
		/*class Text
		{
		public:
			Text(glm::vec2 pos, int w, int h, const std::string& fontKey, SDL_Color color, const std::string& text);

			void changeColor(SDL_Color color);

			void setText(const std::string& newText);

			void render(const Renderer& renderer);

		public:
			glm::vec2 pos;
		private:
			std::string m_text;
			Texture* m_tex = nullptr;
			SDL_FRect m_texRect;
			bool m_textSet = true;

			inline static std::unordered_map<std::string, TTF_Font*> sm_fonts;
		};*/

		Text::Text(glm::vec2 pos, float w, float h, const std::string& fontFPath, SDL_Color color, const std::string& text, WidthGrowthSide growthSide)
			:
			m_gorwthSide(growthSide),
			m_charWidth(w),
			pos(pos),
			m_texRect(pos.x, pos.y, 0.0f, static_cast<float>(h)),
			m_col(color),
			m_text(text)
		{
			if (sm_fonts.contains(fontFPath))
				m_font = sm_fonts.at(fontFPath).hnd;
			else
			{
				sm_fonts.emplace(std::pair{
					fontFPath,
					fontFPath // implicit construction of Text::Font struct
				});
				m_font = sm_fonts.at(fontFPath).hnd;
			}
		}

		void Text::changeColor(SDL_Color color)
		{
			m_reloadNeeded = true;
			m_col = color;
		}

		void Text::setText(const std::string& newText)
		{
			m_reloadNeeded = true;
			m_text = newText;
		}

		void Text::render(const Renderer& renderer)
		{
			if (m_reloadNeeded)
			{
				SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, m_text.c_str(), m_text.length(), m_col);
				if (!textSurface)
					std::cout << "TEXT ERR: " << SDL_GetError() << '\n';

				if(Core::AssetManager::textureManager->contains(m_text))
				{
					m_tex = Core::AssetManager::textureManager->overwriteTexture(
						m_text,
						renderer.getRendHand(),
						SDL_CreateTextureFromSurface(renderer.getRendHand(), textSurface)
					);
				}
				else
				{
					m_tex = Core::AssetManager::textureManager->newTexture(
						m_text,
						renderer.getRendHand(),
						SDL_CreateTextureFromSurface(renderer.getRendHand(), textSurface)
					);
				}

				SDL_DestroySurface(textSurface);

				m_reloadNeeded = false;
			}

			if(!m_manualRectMode)
			{
				if(m_gorwthSide == WidthGrowthSide::left)
					pos.x -= ((m_charWidth * static_cast<int>(m_text.length())) - m_texRect.w);

				m_texRect.x = pos.x;
				m_texRect.y = pos.y;
				m_texRect.w = static_cast<float>(m_charWidth * static_cast<int>(m_text.length()));
			}

			renderer.render(*m_tex, m_texRect);
		}

		SDL_FRect& Text::getRectToSetPosManually()
		{
			m_manualRectMode = true;
			return m_texRect;
		}

		SDL_Color Text::getTextColor() const
		{
			return m_col;
		}

		const std::string& Text::getText() const
		{
			return m_text;
		}
	}
}