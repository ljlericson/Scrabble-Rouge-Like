#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include "Window.hpp"
#include "Texture.hpp"

namespace Core
{
	namespace SDLBackend
	{
		class Renderer
		{
		public:
			Renderer(const Window& window);

			void preRender();

			void render(const Texture& tex, SDL_FRect texRect) const;

			void postRender();

			SDL_Renderer* getRendHand() const;

			~Renderer();

		private:
			SDL_Renderer* m_rendHand;
		};
	}
}