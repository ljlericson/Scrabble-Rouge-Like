#include "Renderer.hpp"

namespace Core
{
	namespace SDLBackend
	{
		Renderer::Renderer(const Window& window)
		{
			m_rendHand = SDL_CreateRenderer(window.getWHand(), nullptr);
			if (!m_rendHand)
				std::cerr << "ERROR: Renderer failed to create\n";
		}

		void Renderer::preRender()
		{
			SDL_SetRenderDrawColor(m_rendHand, 0, 0, 0, 255);
			SDL_RenderClear(m_rendHand);
		}

		void Renderer::render(const Texture& tex, SDL_FRect texRect, SDL_FRect* srcRect) const
		{
			if (!SDL_RenderTexture(m_rendHand, tex.texHand, srcRect, &texRect))
				std::cerr << "ERROR: Texture failed to render " << SDL_GetError() << '\n';
		}

		void Renderer::render(SDL_FRect rect, SDL_Color col, DrawType type) const
		{
			SDL_SetRenderDrawBlendMode(m_rendHand, SDL_BLENDMODE_BLEND);
			if (type == DrawType::fill)
			{
				SDL_SetRenderDrawColor(m_rendHand, col.r, col.g, col.b, col.a);
				SDL_RenderFillRect(m_rendHand, &rect);
			}
			else
			{
				SDL_SetRenderDrawColor(m_rendHand, col.r, col.g, col.b, col.a);
				SDL_RenderRect(m_rendHand, &rect);
			}
		}

		void Renderer::postRender()
		{
			SDL_RenderPresent(m_rendHand);
		}


		SDL_Renderer* Renderer::getRendHand() const
		{
			return m_rendHand;
		}

		Renderer::~Renderer()
		{
			SDL_DestroyRenderer(m_rendHand);
		}
	}
}