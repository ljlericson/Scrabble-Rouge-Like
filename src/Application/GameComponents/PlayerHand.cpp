#include "PlayerHand.hpp"

namespace App
{
	namespace GameComponents
	{
		PlayerHand::PlayerHand(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer, Shop::ModifierManager& modifierManager, int numTiles, int numTilesPerGame)
			: m_highlighter(SDL_Color{ .r = 255, .g = 0, .b = 0, .a = 100 }, numTiles),
			  m_numTilesLeft(numTilesPerGame),
			  mr_renderer(renderer),
			  mr_eventDispatcher(eventDispatcher),
			  mr_modifierManager(modifierManager),
			  m_numTiles(numTiles),
			  m_scoreText(glm::vec2(Utils::getWindowSize().first - 75.0f, 0.0f), 32, 32, "./assets/font.ttf", SDL_Color(0, 255, 0, 255), "0"),
			  m_scoreTextOverall(glm::vec2(Utils::getWindowSize().first - 75.0f, 32), 32, 32, "./assets/font.ttf", SDL_Color(255, 0, 0, 255), "0"),
			  m_tileSlots({false, false, false, false, false, false, false}),
			  m_tileRecycler(renderer)

		{
			// open file for tile letter selection
			std::ifstream file("./config/tiles/tileLetters.json");
			file >> m_vowelsAndCons;
			eventDispatcher.attach(*this);
		}

		void PlayerHand::render(Board& scrabbleBoard, const Core::SDLBackend::Renderer& renderer)
		{
			// only render tile recycler when the game is running
			if(m_gameRunning)
				m_tileRecycler.render(renderer);

			// tile render loop
			for (size_t i = 0; i < m_activeTiles.size(); ++i)
			{
				std::unique_ptr<Tile>& tile = m_activeTiles[i].get();
				auto result = tile->handlePress();

				// press hand logic
				if (result == GameComponents::Tile::PressState::justReleased)
				{
					// if the tile is released in the recycler then
					// shuffle the character and send it back to the
					// start pos (to give illusion that it is a new
					// tile)
					if (m_numTilesLeft > 0 && m_tileRecycler.inRecycler(*tile))
					{
						tile->shuffleChar(renderer);
						auto [w, h] = Utils::getWindowSize();
						tile->pos.x = w + 50.0f;
						tile->pos.y = h + 50.0f;
						tile->addRedTint = false;
						--m_numTilesLeft;
					}

					scrabbleBoard.addTileToBoard(tile.get());
					int score;
					std::tie(m_badWordIndexes, score) = scrabbleBoard.getBadWordIndexesAndScore(mr_modifierManager);

					m_score += score;
					m_score += mr_modifierManager.getBonusPoints(scrabbleBoard.getWordsOnBoard(), score, "wordScored", m_numTilesLeft);
					m_hideRecyclerAnimation = true;
				}
				else if (result == GameComponents::Tile::PressState::pressed)
				{
					m_hideRecyclerAnimation = false;

					if (m_tileRecycler.inRecycler(*tile))
						tile->addRedTint = true;
					else
						tile->addRedTint = false;

					// render the highlighter that shows where the tile
					// is going to go on the board
					auto [w, h] = Utils::getWindowSize();
					const float numTiles = static_cast<float>(scrabbleBoard.getNumTiles());
					const float tileSize = h / static_cast<float>(numTiles);

					// continuity correction for snapping in the middle of
					// the tile as opposed to the left top corner
					size_t tileIndex = scrabbleBoard.getSnapTileIndex({
						tile->pos.x + ((h / numTiles / 2.0f)),
						tile->pos.y + ((h / numTiles / 2.0f))
						});

					// SIZE_MAX indicates the tile is not on the board
					if (tileIndex != SIZE_MAX)
						m_highlighter.render(renderer, tileIndex);
				}
				else if (m_hideRecyclerAnimation)
				{
					m_hideRecyclerAnimation = m_tileRecycler.hideAnimation();
				}


				tile->render(renderer);
			} // ~ tile render loop

			// inactive tile renderering
			for (auto& tileReference : m_inactiveTiles) tileReference.get()->render(renderer);
			// red tint rendering for misspelled words on the board
			for (size_t badTileIndex : m_badWordIndexes) m_highlighter.render(renderer, badTileIndex);
			
			// update the temp score text renderering
			m_scoreText.setText(std::to_string(m_score));


			m_scoreText.render(renderer);
			m_scoreTextOverall.render(renderer);
		}

		void PlayerHand::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
		{
			// local stack variable
			size_t nextTileIndex = static_cast<size_t>(m_numRounds) * 7;

			switch (e)
			{
			case EventType::gameStart:
				m_gameRunning = true;

				for (size_t i = 0; i < m_numTilesLeft; i++)
				{
					m_tileRecycler.hide();
					char c;
					int chance = Utils::getRandomInt(0, 2);

					if (chance == 0)
					{
						c = m_vowelsAndCons["vowels"].at(Utils::getRandomInt(0, (int32_t)m_vowelsAndCons["vowels"].size() - 1)).get<std::string>()[0];
					}
					else
					{
						c = m_vowelsAndCons["consonants"].at(Utils::getRandomInt(0, (int32_t)m_vowelsAndCons["consonants"].size() - 1)).get<std::string>()[0];
					}
					

					m_tiles.push_back(std::make_unique<Tile>(mr_renderer, m_numTiles, c));
					mr_eventDispatcher.attach(*m_tiles.back());
				}

				break; // ~ gameStart
		
			case EventType::gameEnd: // ----------------
				m_gameRunning = false;

				for (auto& tile : m_tiles)
				{
					mr_eventDispatcher.dettach(*tile);
				}
				for (int i = 0; i < 7; i++)
				{
					m_tileSlots[i] = false;
				}

				m_score = 0;
				m_numRounds = 0;
				m_activeTiles.clear();
				m_inactiveTiles.clear();
				m_tiles.clear();
				m_badWordIndexes.clear();

				break; // ~ gameEnd

			case EventType::wordConfirmed: // ----------------

				m_scoreOverall += m_score;
				m_scoreTextOverall.setText(std::to_string(m_scoreOverall));
				m_score = 0;

				if (!m_devMode && m_badWordIndexes.size() > 0)
				{
					Console::ccout << "WORD MUST BE SPELT CORRECTLY" << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
					return;
				}
				if (m_inactiveTiles.size() == m_numTilesLeft)
				{
					Console::ccout << "NO MORE TILES" << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
					break;
				}

				for (int i = (int)m_activeTiles.size() - 1; i >= 0; --i)
				{
					auto& tile = m_activeTiles[i].get();
					if (tile->getIndex() != SIZE_MAX)
					{
						m_tileSlots[tile->tileSpotIndex] = false;
						tile->setInactive();
						m_inactiveTiles.push_back(tile);
						m_activeTiles.erase(m_activeTiles.begin() + i);
					}
				}

				while (true)
				{
					auto slotIt = std::find(m_tileSlots.begin(), m_tileSlots.end(), false);
					if (slotIt == m_tileSlots.end())
						break;

					if (m_activeTiles.size() >= m_tiles.size())
						break;

					if (nextTileIndex >= m_tiles.size())
						break;

					auto& tile = m_tiles[nextTileIndex++];
					m_activeTiles.push_back(tile);

					int slotIndex = static_cast<int>(std::distance(m_tileSlots.begin(), slotIt));
					tile->tileSpotIndex = slotIndex;
					m_tileSlots[slotIndex] = true;

					glm::vec2& pos = tile->getStartPos();
					pos.y = static_cast<float>(Utils::getWindowSize().second) - 200;
					pos.x = static_cast<float>(Utils::getWindowSize().first) - (64.0f + slotIndex * 55.0f);

					tile->glideToStartPos();
					--m_numTilesLeft;
				}
				m_numRounds++;

				break; // ~ wordConfirmed

			case EventType::shuffleTiles: // ----------------
				if(!m_devMode)
				{
					int numTilesToShuffle = 0;
					for (auto& tile : m_activeTiles)
					{
						const auto& tilePtr = tile.get();
						if (tilePtr->getIndex() == SIZE_MAX)
						{
							numTilesToShuffle++;
						}
					}
					if (m_score < numTilesToShuffle * 5)
					{
						Console::ccout << "You too broke for that shit :(" << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						break;
					}

					m_score -= numTilesToShuffle * 5;
				}

				for (auto& tile : m_activeTiles)
				{
					const auto& tilePtr = tile.get();
					if (tilePtr->getIndex() == SIZE_MAX)
					{
						tilePtr->shuffleChar(mr_renderer);
					}
				}

				break; // ~shuffleTiles

			case EventType::enterDevMode:
				m_devMode = true;
				break;
			}
		}

		int PlayerHand::getNumTilesLeft() const
		{
			return m_numTilesLeft;
		}
	}
}