#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <chrono>

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
			enum class DrawType
			{
				outline,
				fill
			};

			enum class AnimationState
			{
				finished,
				inProgress
			};
		public:

			Renderer(const Renderer&) = delete;
			Renderer& operator=(const Renderer&) = delete;

			Renderer(const Window& window);

			void preRender();

			void render(const Texture& tex, SDL_FRect texRect, SDL_FRect* srcRect = nullptr) const;

			AnimationState fadeIn(const Texture& tex, SDL_FRect texRect, std::chrono::milliseconds fadeInTime, SDL_FRect* srcRect = nullptr) const;

			void render(SDL_FRect rect, SDL_Color col, DrawType type) const;

			void postRender();

			SDL_Renderer* getRendHand() const;

			~Renderer();

		private:
			SDL_Renderer* m_rendHand;
			mutable std::unordered_map<const Texture*, std::chrono::steady_clock::time_point> m_texFadeStarts;
		};
	}
}