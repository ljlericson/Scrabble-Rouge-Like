#include "Texture.hpp"

namespace Core
{
	namespace SDLBackend
	{
		Texture::Texture(SDL_Renderer* renderer, const std::string& fpath)
			: texHand(nullptr)
		{
            SDL_Surface* surface = IMG_Load(fpath.c_str());
            if (!surface)
            {
                SDL_Log("IMG_Load failed (%s): %s",
                    fpath.c_str(), SDL_GetError());
                return;
            }

            texHand = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);

            if (!texHand)
            {
                SDL_Log("SDL_CreateTextureFromSurface failed: %s",
                    SDL_GetError());
            }
		}

		Texture::~Texture()
		{
			if(texHand)
				SDL_DestroyTexture(texHand);
		}
	}
}