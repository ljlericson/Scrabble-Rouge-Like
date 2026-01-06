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
#include "../Shop/ModifierManager.hpp"
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

			// returns a vector of bad indexes for tile highlighting
			std::vector<size_t> validateWords();

			int getBaseScore(const Shop::ModifierManager& modifierManager);

			void clearMWords();

			void clearMTilesReferences();

			size_t getSnapTileIndex(glm::vec2 pos);

			int getNumTiles() const;
			
			const std::vector<std::string>& getWordsOnBoard() const;

		private:
			// tiles
			std::vector<Tile*> m_tiles;
			std::vector<std::string> m_words;
			std::vector<std::string> m_allWords;

			// board renderering
			std::shared_ptr<Core::SDLBackend::Texture> m_emptyTiles = nullptr;
			std::shared_ptr<Core::SDLBackend::Texture> m_tex = nullptr;
			SDL_FRect m_texRect;
			SDL_FRect m_texRectShaking;

			Hunspell m_spellChecker;
			// mj for memeber json
			nlohmann::json mj_letterScores;

			int m_score = 0;
			
			bool m_devMode = false;
			bool m_active = true;

			// config items
			int m_numTiles = 15; // number of tiles lengthwise |--->|
		};
	}
}

