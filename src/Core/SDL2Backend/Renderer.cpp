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

		void Renderer::render(const Texture& tex)
		{
			SDL_FRect rect;
			rect.x = tex.pos.x;
			rect.y = tex.pos.y;
			rect.w = 500; // tex.pos.x;
			rect.h = 500; // tex.pos.y;


			if (!SDL_RenderTexture(m_rendHand, tex.texHand, NULL, &rect))
				std::cerr << "ERROR: Texture failed to render " << SDL_GetError() << '\n';
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