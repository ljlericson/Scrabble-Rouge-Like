#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <functional>
#include <algorithm>


#include "BasicGameComponent.hpp"
#include "Tile.hpp"
#include "Board.hpp"
#include "TileHighlighter.hpp"
#include "../EventSystem/EventDispatcher.hpp"
#include "../../Core/SDLBackend/Renderer.hpp"
#include "../../Core/SDLBackend/Text.hpp"
#include "../Console/ChatStream.h"


namespace App
{
	namespace GameComponents
	{
		class PlayerHand : public BasicGameComponent
		{
		public:
			PlayerHand(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer, size_t numTiles, size_t numTilesPerRound);

			void render(Board& scrabbleBoard, const Core::SDLBackend::Renderer& renderer);

			void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;

			size_t getNumTilesPerRound() const;

		private:
			std::vector<std::unique_ptr<Tile>> m_tiles;
			std::vector<std::reference_wrapper<std::unique_ptr<Tile>>> m_activeTiles;
			std::vector<std::reference_wrapper<std::unique_ptr<Tile>>> m_inactiveTiles;
			

			GameComponents::TileHighlighter m_highlighter;
			std::vector<size_t> m_badWordIndexes;
			std::string m_scoreTextStr;
			Core::SDLBackend::Text m_scoreText;
			Core::SDLBackend::Text m_scoreTextOverall;

			const Core::SDLBackend::Renderer& mr_renderer;
			EventSystem::EventDispatcher& mr_eventDispatcher;

			int m_score = 0;
			size_t m_scoreOverall = 0;
			uint8_t m_numRounds = 0;

			// config items
			size_t m_numTilesPerRound = 0;
			const size_t m_numTiles = 0;
		};
	}
}