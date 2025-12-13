#include "Board.hpp"

namespace App
{
	namespace GameComponents
	{
		Board::Board(const Core::SDLBackend::Renderer& renderer)
		{
			m_tex = Core::AssetManager::textureManager->newTexture("Board", renderer.getRendHand(), "./assets/textures/board.png");
			m_texRect.h = 500.0f;
			m_texRect.w = 500.0f;
			m_texRect.x = 0.0f;
			m_texRect.y = 0.0f;
		}

		void Board::render(const Core::SDLBackend::Renderer& renderer) const
		{
			renderer.render(*m_tex, m_texRect);
		}

		void Board::onInput(const bool* keyboardState)
		{
			if (keyboardState[SDL_SCANCODE_G])
				m_texRect.x += 1.0f;
		}
	}
}