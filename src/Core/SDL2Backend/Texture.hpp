#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace Core
{
	namespace SDLBackend
	{
		struct Texture
		{
			Texture() = default;
			Texture(SDL_Renderer* renderer, const std::string& fpath);
			~Texture();
			
			SDL_Texture* texHand = nullptr;
		};
	}
}