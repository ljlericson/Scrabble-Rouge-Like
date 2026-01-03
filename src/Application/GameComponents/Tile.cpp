#include "Tile.hpp"

namespace App
{
	namespace GameComponents
	{
		Tile::Tile(const Core::SDLBackend::Renderer& renderer, int numTiles, char tileChar)
			: m_startPos(0.0f), m_numTilesLengthWase(numTiles), m_char(tileChar)
		{
			if (!sm_font)
				sm_font = TTF_OpenFont("./assets/Fonts/font.ttf", 128);

			m_tex = Core::AssetManager::textureManager->newTexture(
				"Tile",
				renderer.getRendHand(),
				"./assets/Textures/GameComponents/Tiles.png"
			);

			auto [w, h] = Utils::getWindowSize();
			m_texRect.h = h / static_cast<float>(numTiles);
			m_texRect.w = h / static_cast<float>(numTiles);
			pos.x = w + 50.0f;
			pos.y = h + 50.0f;
			
			m_texSrcRect.h = 32.0f;
			m_texSrcRect.w = 32.0f;
			int index = int(m_char - 'A');
			m_texSrcRect.x = (index % 16) * 32.0f;
			m_texSrcRect.y = (index / 16) * 32.0f;
		}

		void Tile::render(const Core::SDLBackend::Renderer& renderer)
		{
			m_texRect.x = pos.x;
			m_texRect.y = pos.y;

			renderer.render(*m_tex, m_texRect, &m_texSrcRect);
			if(addRedTint)
				renderer.render(m_texRect, SDL_Color(255, 0, 0, 150), Core::SDLBackend::Renderer::DrawType::fill);
		}

		void Tile::glideToStartPos()
		{
			if (!m_tileActive)
				return;

			m_glidingToStart = true;
		}

		void Tile::snapToTile(size_t index)
		{
			if (!m_tileActive)
				return;

			m_index = index;

			if (m_index == SIZE_MAX)
			{
				this->glideToStartPos();
				return;
			}

			auto [w, h] = Utils::getWindowSize();
			const float tileSize = h / static_cast<float>(m_numTilesLengthWase);

			const size_t tileY = m_index / m_numTilesLengthWase;
			const size_t tileX = m_index % m_numTilesLengthWase;

			pos.x = tileX * tileSize;
			pos.y = tileY * tileSize;
		}

		void Tile::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
		{
			if (!m_tileActive)
				return;

			if(std::find(events.begin(), events.end(), SDL_EVENT_MOUSE_BUTTON_DOWN) != events.end())
				m_ctrPressed = true;
			else if (std::find(events.begin(), events.end(), SDL_EVENT_MOUSE_BUTTON_UP) != events.end())
				m_ctrPressed = false;
			

			switch (e)
			{
			case EventType::screenShake:
				std::cout << "SHAKING\n";
				break;
			default:
				break;
			}

			if (m_glidingToStart && pos != m_startPos)
			{
				pos.x += (getStartPos().x - pos.x) / 10.0f;
				pos.y += (getStartPos().y - pos.y) / 10.0f;
			}
			if(glm::distance(pos, m_startPos) < 10.0f)
			{
				pos = m_startPos;
				m_glidingToStart = false;
			}
		}

		Tile::PressState Tile::handlePress()
		{
			if (!m_tileActive)
				return PressState::notPressed;

			const float minX = pos.x;
			const float minY = pos.y;
			const float maxX = pos.x + m_texRect.w;
			const float maxY = pos.y + m_texRect.h;

			float mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			// I honestly have no clue how this if
			// statement got this complex
			if (
					// check that another tile isnt engaged and that control is pressed
				   (((m_ctrPressed) && (!sm_tilePressEngaged) && (!m_tilePressed)) ||
				   (((m_ctrPressed) && (sm_tilePressEngaged) && (m_tilePressed))))

										&&
					// check that the mouse curser is over the tile
					     (((mouseX > minX && mouseX < maxX) &&
					       (mouseY > minY && mouseY < maxY))     
						  || m_tilePressed) // or the tile is already pressed (bug fix)
				)
			{
				pos.x = mouseX - (m_texRect.w / 2.0f);
				pos.y = mouseY - (m_texRect.h / 2.0f);

				m_tilePressed = true;
				sm_tilePressEngaged = true;
				return PressState::pressed;
			}
			// control not pressed and tile engaged means
			// user has just released tile
			else if (!m_ctrPressed && m_tilePressed)
			{
				m_tilePressed = false;
				sm_tilePressEngaged = false;
				pos.x = mouseX - (m_texRect.w / 2.0f);
				pos.y = mouseY - (m_texRect.h / 2.0f);
				return PressState::justReleased;
			}

			// else tile isnt engaged
			return PressState::notPressed;
		}

		glm::vec2& Tile::getStartPos()
		{
			return m_startPos;
		}

		size_t Tile::getIndex() const
		{
			return m_index;
		}

		const char Tile::getTileChar() const
		{
			return m_char;
		}

		void Tile::shuffleChar(const Core::SDLBackend::Renderer& renderer)
		{
			m_char = char('A' + Utils::getRandomInt(0, 24));
			m_texSrcRect.h = 32.0f;
			m_texSrcRect.w = 32.0f;
			int index = int(m_char - 'A');
			m_texSrcRect.x = (index % 16) * 32.0f;
			m_texSrcRect.y = (index / 16) * 32.0f;
		}

		void Tile::setInactive()
		{
			m_tileActive = false;
		}

		TTF_Font* Tile::getStaticFont()
		{
			return sm_font;
		}
	}
}