#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "BasicGameComponent.hpp"
#include "../../Core/AssetManager/AssetManager.hpp"
#include "../../Core/SDL2Backend/Renderer.hpp"
#include "../../Core/SDL2Backend/Texture.hpp"
#include "../../Utils/Utils.hpp"

namespace App
{
	namespace GameComponents
	{
		class Tile : public BasicGameComponent
		{
		public:
			enum class PressState
			{
				notPressed,
				justReleased,
				pressed
			};

			Tile() = default;
			~Tile() = default;

			Tile(const Core::SDLBackend::Renderer& renderer, size_t numTiles);

			void render(const Core::SDLBackend::Renderer& renderer);

			void glideToStartPos();

			void snapToTile(size_t index);

			void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;

			PressState handlePress();

			glm::vec2 getStartPos() const;

			size_t getIndex() const;

			const char getTileChar() const;

			void shuffleChar(const Core::SDLBackend::Renderer& renderer);

			// playerhand state stuff

			void setInactive();

			static void setSidePadding(size_t num);

			static void incrementSidePadding();

		public:
			glm::vec2 pos;
		private:
			Core::SDLBackend::Texture* m_tex;
			SDL_FRect m_texRect;
			glm::vec2 m_startPos;

			bool m_ctrPressed = false;
			bool m_tilePressed = false;
			bool m_glidingToStart = false;
			bool m_firstGlide = true;

			bool m_tileActive = true;

			size_t m_index = SIZE_MAX;
			size_t m_numTilesOnBoard = 0;

			std::string m_char;
			Core::SDLBackend::Texture* m_textTexture;

			inline static size_t sm_numTiles = 0;
			inline static bool sm_tilePressEngaged = false;
			inline static TTF_Font* sm_font = nullptr;
		};
	}
}