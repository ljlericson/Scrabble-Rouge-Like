#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <functional>
#include <fstream>
#include <algorithm>
#include <array>

#include <nlohmann/json.hpp>

#include "BasicGameComponent.hpp"
#include "Tile.hpp"
#include "Recycler.hpp"
#include "Board.hpp"
#include "TileHighlighter.hpp"
#include "../Shop/ModifierManager.hpp"
#include "../EventSystem/EventDispatcher.hpp"
#include "../../Core/SDLBackend/Renderer.hpp"
#include "../../Core/SDLBackend/Text.hpp"
#include "../Console/ChatStream.h"


namespace App
{
	namespace GameComponents
	{
		class GameplayManager : public BasicGameComponent
		{
		public:
			GameplayManager(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer, Shop::ModifierManager& modifierManager, Board& board, int numTiles, int numTilesPerGame);
			~GameplayManager();

			void render(const Core::SDLBackend::Renderer& renderer);

			void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;

			int getNumTilesLeft() const;

		private:
			// list of all tiles within the game
			std::vector<std::unique_ptr<Tile>> m_tiles;
			// list of active tiles on the board or in the players hand,
			// events are polled as usual
			std::vector<std::reference_wrapper<std::unique_ptr<Tile>>> m_activeTiles;
			// list of inactive tiles that are on the board and do not take
			// events because their only job is to be rendered;
			std::vector<std::reference_wrapper<std::unique_ptr<Tile>>> m_inactiveTiles;
			// free tile slots
			std::array<bool, 7> m_tileSlots;
			
			// tile highlighter for both 
			GameComponents::TileHighlighter m_highlighter;
			std::vector<size_t> m_badWordIndexes;
			Core::SDLBackend::Text m_scoreText;
			Core::SDLBackend::Text m_tilesLeftText;
			Core::SDLBackend::Text m_targetScoreText;

			Board& mr_board;
			Shop::ModifierManager& mr_modifierManager;
			const Core::SDLBackend::Renderer& mr_renderer;
			EventSystem::EventDispatcher& mr_eventDispatcher;

			Recycler m_tileRecycler;
			bool m_hideRecyclerAnimation = false;
			
			bool m_gameRunning = false;

			nlohmann::json m_vowelsAndCons;

			int m_score = 0;
			uint8_t m_numRounds = 0;
			int m_nextTileIndex = 0;
			int m_numPreviousWords = 0;
			
			bool m_devMode = false;

			// num tiles total for the entire game
			int m_numTilesLeft = 0;
			// num tiles lengthwise across the board
			const int m_numTiles = 0;
			// num tiles across the entire game
			const int m_numTilesTotal;

			int m_numGames = 1;
			size_t m_targetScore = 30;
		};
	}
}