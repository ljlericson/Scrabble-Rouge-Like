#pragma once
#include <expected>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <unordered_set>

#include <nlohmann/json.hpp>
#include <hunspell/hunspell.hxx>
#include <SDL3/SDL.h>

#include "BasicGameComponent.hpp"
#include "Tile.hpp"
#include "../EventSystem/EventDispatcher.hpp"
#include "../../Core/SDLBackend/Renderer.hpp"
#include "../../Core/SDLBackend/Window.hpp"
#include "../../Core/AssetManager/AssetManager.hpp"
#include "../../Core/SDLBackend/Texture.hpp"
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

			std::pair<std::vector<size_t>, int> getBadWordIndexesAndScore();

			size_t getSnapTileIndex(glm::vec2 pos);

			size_t getNumTiles() const;
			
			const std::vector<std::string>& getWordsOnBoard() const;

		private:
			// tiles
			std::vector<Tile*> m_tiles;
			std::vector<std::string> m_words;
			// board renderering
			std::shared_ptr<Core::SDLBackend::Texture> m_emptyTiles = nullptr;
			std::shared_ptr<Core::SDLBackend::Texture> m_tex = nullptr;
			SDL_FRect m_texRect;
			SDL_FRect m_texRectShaking;

			Hunspell m_spellChecker;
			nlohmann::json m_letterScores;

			int m_score = 0;
			
			bool m_devMode = false;
			bool m_active = true;

			// config items
			size_t m_numTiles = 15; // number of tiles lengthwise |--->|
		};
	}
}

