#include "GameplayManager.hpp"

namespace App
{
	namespace GameComponents
	{
		GameplayManager::GameplayManager(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer, Shop::ModifierManager& modifierManager, Board& board, int numTiles, int numTilesPerGame)
			: m_highlighter(SDL_Color{ .r = 0, .g = 0, .b = 255, .a = 100 }, numTiles),
			  m_numTilesLeft(numTilesPerGame),
			  m_numTilesTotal(numTilesPerGame),
			  mr_renderer(renderer),
			  mr_eventDispatcher(eventDispatcher),
			  mr_modifierManager(modifierManager),
			  mr_board(board),
			  m_numTiles(numTiles),
			  m_scoreText(glm::vec2(Utils::getWindowSize().first - 10.0f, 0.0f), 20, 20, "./assets/Fonts/font.ttf", SDL_Color(0, 255, 0, 255), "Points: "),
			  m_tilesLeftText(glm::vec2(Utils::getWindowSize().first - 10.0f, 20.0f), 20, 20, "./assets/Fonts/font.ttf", SDL_Color(255, 0, 0, 255), "Number of Tiles: "),
			  m_targetScoreText(glm::vec2(Utils::getWindowSize().first - 10.0f, 40.0f), 20, 20, "./assets/Fonts/font.ttf", SDL_Color(235, 201, 52, 255), "Target Points: "),
			  m_tileSlots({false, false, false, false, false, false, false}),
			  m_tileRecycler(renderer)

		{
			// open file for tile letter selection
			std::ifstream file("./config/tiles/tileLetters.json");
			file >> mj_vowelsAndCons;
		}

		GameplayManager::~GameplayManager()
		{
			for (auto& tile : m_tiles)
			{
				mr_eventDispatcher.dettach(*tile);
			}
		}

		void GameplayManager::render(const Core::SDLBackend::Renderer& renderer, UpdateGameSystemOnRender renderType)
		{
			// only render tile recycler when the game is running
			if(m_gameRunning)
				m_tileRecycler.render(renderer);

			// tile render loop
			for (size_t i = 0; i < m_activeTiles.size(); ++i)
			{
				std::unique_ptr<Tile>& tile = m_activeTiles[i].get();

				// only update tiles if the game system is to be updated
				if(renderType == UpdateGameSystemOnRender::true_)
				{
					auto result = tile->handlePress();
					// press tile logic
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
							// decrement tiles left and increment next
							// tile index 
							--m_numTilesLeft;
							m_nextTileIndex++;
						}

						mr_board.addTileToBoard(tile.get());
						m_badWordIndexes = mr_board.validateWords();

						const auto& wordsOnBoard = mr_board.getWordsOnBoard();

						m_hideRecyclerAnimation = true;
						m_numPreviousWords = static_cast<int>(wordsOnBoard.size());
					}
					else if (result == GameComponents::Tile::PressState::pressed)
					{
						if (tile.get() != m_activeTiles.back().get().get())
						{
							auto it = std::find_if(m_activeTiles.begin(), m_activeTiles.end(), [&](const auto& t)
								{
									return t.get().get() == m_activeTiles[i].get().get();
								});

							if (it != m_activeTiles.end())
							{
								std::rotate(it, it + 1, m_activeTiles.end());
								i = 0; // sneaky lil trick to avoid flickering
							}
						}

						m_hideRecyclerAnimation = false;

						if (m_tileRecycler.inRecycler(*tile))
							tile->addRedTint = true;
						else
							tile->addRedTint = false;

						// render the highlighter that shows where the tile
						// is going to go on the board
						auto [w, h] = Utils::getWindowSize();
						const float numTiles = static_cast<float>(mr_board.getNumTiles());
						const float tileSize = h / static_cast<float>(numTiles);

						// continuity correction for snapping in the middle of
						// the tile as opposed to the left top corner
						size_t tileIndex = mr_board.getSnapTileIndex({
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
				}

				tile->render(renderer);
			} // ~ tile render loop

			// inactive tile rendering
			for (auto& tileReference : m_inactiveTiles) tileReference.get()->render(renderer);
			// red tint rendering for misspelled words on the board
			for (size_t badTileIndex : m_badWordIndexes) m_highlighter.render(renderer, badTileIndex);

			m_scoreText.render(renderer);

			m_tilesLeftText.setText("Number of Tiles: ");
			m_tilesLeftText.setText(m_tilesLeftText.getText().append(std::to_string(m_tiles.size() - m_nextTileIndex)));
			m_tilesLeftText.render(renderer);

			m_targetScoreText.render(renderer);
		}

		void GameplayManager::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
		{
			switch (e)
			{
			case EventType::roundStart:
				m_gameRunning = true;

				// local scope for switch block
				{
					m_targetScore = m_numGames * 30;
					std::string text = m_targetScoreText.getText();
					text.append(std::to_string(m_targetScore));
					m_targetScoreText.setText("Target Points: ");
					m_targetScoreText.setText(text);
				}

				for (size_t i = 0; i < m_numTilesLeft; i++)
				{
					m_tileRecycler.hide();
					char c;
					int chance = Utils::getRandomInt(0, 2);

					if (chance == 0)
					{
						c = mj_vowelsAndCons["vowels"].at(Utils::getRandomInt(0, (int32_t)mj_vowelsAndCons["vowels"].size() - 1)).get<std::string>()[0];
					}
					else
					{
						c = mj_vowelsAndCons["consonants"].at(Utils::getRandomInt(0, (int32_t)mj_vowelsAndCons["consonants"].size() - 1)).get<std::string>()[0];
					}
					

					m_tiles.push_back(std::make_unique<Tile>(mr_renderer, m_numTiles, c));
					mr_eventDispatcher.attach(*m_tiles.back());
				}

				break; // ~ gameStart
		
			case EventType::roundEnd: // ----------------
				m_gameRunning = false;

				for (auto& tile : m_tiles)
				{
					mr_eventDispatcher.dettach(*tile);
				}
				for (int i = 0; i < 7; i++)
				{
					m_tileSlots[i] = false;
				}		

				m_activeTiles.clear();
				m_inactiveTiles.clear();
				m_tiles.clear();
				m_badWordIndexes.clear();

				if(m_score < m_targetScore)
					mr_eventDispatcher.queueEvent(EventType::gameEnd);

				m_score = 0;
				m_numRounds = 0;
				m_numTilesLeft = m_numTilesTotal;
				m_nextTileIndex = 0;
				++m_numGames;

				break; // ~ gameEnd

			case EventType::wordConfirmed: // ----------------

				if (!m_devMode && m_badWordIndexes.size() > 0)
				{
					Console::ccout << "WORD MUST BE SPELT CORRECTLY" << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
					return;
				}

				// avoid skipping init of vars

				{
					int baseScore = mr_board.getBaseScore(mr_modifierManager);
					const auto& wordsOnBoard = mr_board.getWordsOnBoard();
					m_score += baseScore;
					m_score += mr_modifierManager.getBonusPoints(wordsOnBoard, baseScore, "wordScored", m_numTilesLeft, static_cast<int>(wordsOnBoard.size()) - m_numPreviousWords);
				}
				mr_board.clearMWords();
				m_badWordIndexes.clear();

				m_scoreText.setText("Points: ");
				m_scoreText.setText(m_scoreText.getText().append(std::to_string(m_score)));

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

				while(true)
				{
					auto slotIt = std::find(m_tileSlots.begin(), m_tileSlots.end(), false);
					if (slotIt == m_tileSlots.end()) 
						break;

					if (m_nextTileIndex > m_tiles.size() - 1) 
						break;

					auto& tile = m_tiles[m_nextTileIndex];
					m_activeTiles.push_back(tile);
					++m_nextTileIndex;

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

			case EventType::enterDevMode:
				m_devMode = true;
				break;
			}
		}

		int GameplayManager::getNumTilesLeft() const
		{
			return m_numTilesLeft;
		}
	}
}