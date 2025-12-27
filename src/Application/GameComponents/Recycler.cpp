#include "Recycler.hpp"

namespace App
{
	namespace GameComponents
	{
		Recycler::Recycler(const Core::SDLBackend::Renderer& renderer)
		{
			m_tex = Core::AssetManager::textureManager->newTexture("RECYCLER", renderer.getRendHand(), "./assets/textures/recycler.png");
			m_texRect.h = 100.0f;
			m_texRect.w = 500.0f;
			m_texRect.y = static_cast<float>(Utils::getWindowSize().second) - 100.0f;
			m_texRect.x = static_cast<float>(Utils::getWindowSize().first) - 500.0f;
			m_posShown = { m_texRect.x, m_texRect.y };
			m_texRect.y = m_posShown.y + 500.0f;
		}

		bool Recycler::inRecycler(const Tile& tile)
		{
			return (tile.pos.x > m_texRect.x && tile.pos.x < (m_texRect.x + m_texRect.w)
				 && tile.pos.y > m_texRect.y && tile.pos.y < (m_texRect.y + m_texRect.h));
		}

		void Recycler::render(const Core::SDLBackend::Renderer& renderer)
		{
			if (glm::distance(glm::vec2(m_texRect.x, m_texRect.y), m_posShown) < 1.0f)
			{
				m_texRect.y = m_posShown.y;
				renderer.render(*m_tex, m_texRect);
			}
			else
			{
				m_texRect.y -= (m_texRect.y - m_posShown.y) / 5.0f;
				renderer.render(*m_tex, m_texRect);
			}
		}

		bool Recycler::hide()
		{
			if (std::fabs(((m_texRect.y - (m_posShown.y + 500.0f)))) < 1.0f)
			{
				m_texRect.y = m_posShown.y + 500.0f;
				return false;
			}
			else
			{
				m_texRect.y += 6.0f;
				return true;
			}
		}

		void Recycler::hideHard()
		{
			m_texRect.y = m_posShown.y + 500.0f;
		}
	}
}