#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "BasicGameComponent.hpp"
#include "../../Core/AssetManager/AssetManager.hpp"
#include "../../Core/SDLBackend/Renderer.hpp"
#include "../../Core/SDLBackend/Texture.hpp"
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

			Tile(const Core::SDLBackend::Renderer& renderer, size_t numTiles, char tileChar);

			void render(const Core::SDLBackend::Renderer& renderer);

			void glideToStartPos();

			void snapToTile(size_t index);

			void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;

			PressState handlePress();

			glm::vec2& getStartPos();

			size_t getIndex() const;

			const char getTileChar() const;

			void shuffleChar(const Core::SDLBackend::Renderer& renderer);

			// playerhand state stuff

			void setInactive();

			static TTF_Font* getStaticFont();

		public:
			glm::vec2 pos;
			int tileSpotIndex = 0;
			bool addRedTint = false;
		private:
			std::shared_ptr<Core::SDLBackend::Texture> m_tex;
			SDL_FRect m_texRect;
			SDL_FRect m_texSrcRect;
			glm::vec2 m_startPos;

			bool m_ctrPressed = false;
			bool m_tilePressed = false;
			bool m_glidingToStart = false;
			bool m_firstGlide = true;

			bool m_tileActive = true;

			size_t m_index = SIZE_MAX;
			size_t m_numTilesOnBoard = 0;

			char m_char;

			inline static size_t sm_numTiles = 0;
			inline static bool sm_tilePressEngaged = false;
			inline static TTF_Font* sm_font = nullptr;
		};
	}
}