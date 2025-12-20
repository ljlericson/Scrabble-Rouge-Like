#pragma once
#include <expected>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

#include <hunspell/hunspell.hxx>
#include <SDL3/SDL.h>

#include "BasicGameComponent.hpp"
#include "Tile.hpp"
#include "../EventSystem/EventDispatcher.hpp"
#include "../../Core/SDL2Backend/Renderer.hpp"
#include "../../Core/SDL2Backend/Window.hpp"
#include "../../Core/AssetManager/AssetManager.hpp"
#include "../../Core/SDL2Backend/Texture.hpp"
#include "../../Utils/Utils.hpp"

namespace App
{
	namespace GameComponents
	{
		class Board : public BasicGameComponent
		{
		public:
			enum class SnapErrType
			{
				tooFarFromBoard,
				spotOccupied
			};

			Board() = default;

			Board(const Core::SDLBackend::Renderer& renderer, const Core::SDLBackend::Window& window);

			void render(const Core::SDLBackend::Renderer& renderer);

			void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;

			void addTileToBoard(Tile* tile);

			std::vector<size_t> getBadWordIndexes();

			size_t getSnapTileIndex(glm::vec2 pos);

			size_t getNumTiles() const;

		private:
			// tiles
			std::vector<Tile*> m_tiles;
			std::vector<std::string> m_words;
			// board renderering
			Core::SDLBackend::Texture* m_tex = nullptr;
			SDL_FRect m_texRect;
			SDL_FRect m_texRectShaking;

			Hunspell m_spellChecker;
			
			bool m_firstTile = true;

			// config items
			size_t m_numTiles = 15; // number of tiles lengthwise |--->|
		};
	}
}

