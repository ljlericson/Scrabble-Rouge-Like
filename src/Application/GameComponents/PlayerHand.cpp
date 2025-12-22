#include "PlayerHand.hpp"

namespace App
{
	namespace GameComponents
	{
		PlayerHand::PlayerHand(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer, size_t numTiles, size_t numTilesPerRound)
			: m_highlighter(SDL_Color{ .r = 255, .g = 0, .b = 0, .a = 50 }, numTiles),
			  m_numTilesPerRound(numTilesPerRound),
			  mr_renderer(renderer),
			  mr_eventDispatcher(eventDispatcher),
			  m_numTiles(numTiles),
			  m_scoreText(glm::vec2(Utils::getWindowSize().first - 75.0f, 0.0f), 32, 32, "./assets/font.ttf", SDL_Color(0, 255, 0, 255), "0"),
			  m_scoreTextOverall(glm::vec2(Utils::getWindowSize().first - 75.0f, 32), 32, 32, "./assets/font.ttf", SDL_Color(255, 0, 0, 255), "0")

		{
			eventDispatcher.attach(*this);
		}

		void PlayerHand::render(Board& scrabbleBoard, const Core::SDLBackend::Renderer& renderer)
		{
			for (auto& tileReference : m_activeTiles)
			{
				std::unique_ptr<Tile>& tile = tileReference.get();

				auto result = tile->handlePress();

				if (result == GameComponents::Tile::PressState::justReleased)
				{
					scrabbleBoard.addTileToBoard(tile.get());
					int score;
					std::tie(m_badWordIndexes, score) = scrabbleBoard.getBadWordIndexesAndScore();

					m_score += score;
				}
				else if (result == GameComponents::Tile::PressState::pressed)
				{
					auto [w, h] = Utils::getWindowSize();
					const float numTiles = static_cast<float>(scrabbleBoard.getNumTiles());
					const float tileSize = h / static_cast<float>(numTiles);

					size_t tileIndex = scrabbleBoard.getSnapTileIndex({
						tile->pos.x + ((h / numTiles / 2.0f)),
						tile->pos.y + ((h / numTiles / 2.0f))
						});

					if (tileIndex != SIZE_MAX)
						m_highlighter.render(renderer, tileIndex);
				}

				tile->render(renderer);
			}
			for (auto& tileReference : m_inactiveTiles)
			{
				tileReference.get()->render(renderer);
			}
			for (size_t badTileIndes : m_badWordIndexes)
				m_highlighter.render(renderer, badTileIndes);

			m_scoreText.setText(std::to_string(m_score));
			m_scoreText.render(renderer);
			m_scoreTextOverall.render(renderer);
		}

		void PlayerHand::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
		{
			switch (e)
			{
			case EventType::gameStart:
				m_score = 20;
				// mr_eventDispatcher.reserveObserverVectorCapacity(m_numTilesPerRound);
				for (size_t i = 0; i <= m_numTilesPerRound; i++)
				{
					/*auto [beginIt, endIt] = mr_eventDispatcher.getObserverVectorIterators();
					size_t distance = std::distance(endIt - i, endIt);*/
					m_tiles.push_back(std::make_unique<Tile>(mr_renderer, m_numTiles));
					mr_eventDispatcher.attach(*m_tiles.back());
				}
				break;
			case EventType::gameEnd:
				for (auto& tile : m_tiles)
				{
					mr_eventDispatcher.dettach(*tile);
				}

				m_scoreOverall += m_score;
				m_score = 0;
				m_scoreTextOverall.setText(std::to_string(m_scoreOverall));
				m_activeTiles.clear();
				m_inactiveTiles.clear();
				m_tiles.clear();
				m_badWordIndexes.clear();
				break;
			case EventType::roundStart:
				if (m_inactiveTiles.size() == m_numTilesPerRound)
				{
					Console::ccout << "NO MORE TILES" << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
					break;
				}
				else if(m_activeTiles.size() > 0)
				{
					Console::ccout << "ROUND ALREADY STARTED\n";
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
					break;
				}

				Tile::setSidePadding(0);
				for (size_t i = 0; (i < 7) && (m_activeTiles.size() <= m_numTilesPerRound); i++)
				{
					Tile::incrementSidePadding();
					m_activeTiles.push_back(m_tiles.at(i + (m_numRounds * 7)));
					m_activeTiles.at(i).get()->glideToStartPos();
				}
				break;
			case EventType::roundEnd:
				if (m_badWordIndexes.size() > 0)
				{
					Console::ccout << "ALL WORDS MUST BE CORRECTLY SPELLED" << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
					return;
				}
				for (auto& tile : m_activeTiles)
				{
					const auto& tilePtr = tile.get();
					if (tilePtr->getIndex() == SIZE_MAX)
					{
						Console::ccout << "ALL TILES MUST BE USED BEFORE ENDING TURN" << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						return;
					}
				}
				for (auto& tile : m_activeTiles)
				{
					tile.get()->setInactive();
					m_inactiveTiles.push_back(tile);
				}

				m_activeTiles.clear();
				m_numRounds++;
				break;
			case EventType::shuffleTiles:

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

				for (auto& tile : m_activeTiles)
				{
					const auto& tilePtr = tile.get();
					if (tilePtr->getIndex() == SIZE_MAX)
					{
						tilePtr->shuffleChar(mr_renderer);
					}
				}
			}
		}

		size_t PlayerHand::getNumTilesPerRound() const
		{
			return m_numTilesPerRound;
		}
	}
}