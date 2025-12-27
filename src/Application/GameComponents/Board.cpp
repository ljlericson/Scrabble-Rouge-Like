#include "Board.hpp"

namespace App
{
	namespace GameComponents
	{
		Board::Board(const Core::SDLBackend::Renderer& renderer, const Core::SDLBackend::Window& window)
			: m_spellChecker("./assets/dictionaries/en_AU.aff", "./assets/dictionaries/en_AU.dic")
		{
			m_tex = Core::AssetManager::textureManager->newTexture("Board", renderer.getRendHand(), "./assets/textures/board.png");
			auto [w, h] = Utils::getWindowSize();

			m_texRectShaking.h = static_cast<float>(h);
			m_texRectShaking.w = static_cast<float>(h);
			m_texRectShaking.x = 0.0f;
			m_texRectShaking.y = 0.0f;

			m_texRect = m_texRectShaking;

			std::ifstream file("./config/tiles/tilePoints.json");
			file >> m_letterScores;
		}

		void Board::render(const Core::SDLBackend::Renderer& renderer)
		{
			if (!m_active)
			{
				if (m_texRect.x < -m_texRect.w)
					return;
				else
					m_texRect.x -= 10;
			}

			auto [w, h] = Utils::getWindowSize();
			m_texRect.h = static_cast<float>(h);
			m_texRect.w = static_cast<float>(h);

			renderer.render(*m_tex, m_texRectShaking);
			m_texRectShaking = m_texRect;
		}
		
		void Board::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
		{
			if (keyboardState[SDL_SCANCODE_G])
				m_texRect.x += 1.0f;

			switch (e)
			{
			case EventType::screenShake:
				m_texRectShaking.x += Utils::getRandomInt(0, 10);
				m_texRectShaking.y += Utils::getRandomInt(0, 10);
				break;
			case EventType::gameStart:
				m_texRect.x = 0;
				m_active = true;
				break;
			case EventType::gameEnd:
				m_active = false;
				m_tiles.clear();
				break;
			case EventType::enterDevMode:
				m_devMode = true;
			}

		}


		void Board::addTileToBoard(Tile* tile)
		{
			if (!m_active)
				return;

			constexpr float snapMargin = 10.0f;

			const float minX = m_texRect.x;
			const float minY = m_texRect.y;
			const float maxX = m_texRect.x + m_texRect.w;
			const float maxY = m_texRect.y + m_texRect.h;

			// --- compute tile center (input is top-left) ---
			const float tileSize = m_texRect.w / static_cast<float>(m_numTiles);
			const glm::vec2 centerPos{
				tile->pos.x + tileSize * 0.5f,
				tile->pos.y + tileSize * 0.5f
			};

			// --- bounds check ---
			if (centerPos.x < minX - snapMargin || centerPos.x > maxX + snapMargin ||
				centerPos.y < minY - snapMargin || centerPos.y > maxY + snapMargin)
			{
				auto it = std::find(m_tiles.begin(), m_tiles.end(), tile);
				if (it != m_tiles.end())
					m_tiles.erase(it);

				tile->snapToTile(SIZE_MAX);
				return;
			}

			// --- convert to board-local ---
			const float localX = centerPos.x - minX;
			const float localY = centerPos.y - minY;

			const int tileX = static_cast<int>(std::floor(localX / tileSize));
			const int tileY = static_cast<int>(std::floor(localY / tileSize));

			// --- validate ---
			if (tileX < 0 || tileX >= static_cast<int>(m_numTiles) ||
				tileY < 0 || tileY >= static_cast<int>(m_numTiles))
			{
				auto it = std::find(m_tiles.begin(), m_tiles.end(), tile);
				if (it != m_tiles.end())
					m_tiles.erase(it);

				tile->snapToTile(SIZE_MAX);
				return;
			}

			const size_t index = static_cast<size_t>(tileY) * m_numTiles + tileX;

			// --- reject if already occupied ---
			for (const Tile* t : m_tiles)
			{
				if (t->getIndex() == index)
				{
					auto it = std::find(m_tiles.begin(), m_tiles.end(), tile);
					if (it != m_tiles.end())
						m_tiles.erase(it);

					tile->snapToTile(SIZE_MAX);
					return;
				}
			}

			// --- adjacency check ---
			auto occupiedAt = [&](int x, int y) -> bool
			{
				if (x < 0 || x >= static_cast<int>(m_numTiles) ||
					y < 0 || y >= static_cast<int>(m_numTiles))
					return false;

				const size_t idx = static_cast<size_t>(y) * m_numTiles + x;
				for (const Tile* t : m_tiles)
					if (t->getIndex() == idx)
						return true;

				return false;
			};

			const bool hasAdjacent =
				occupiedAt(tileX - 1, tileY) ||
				occupiedAt(tileX + 1, tileY) ||
				occupiedAt(tileX, tileY - 1) ||
				occupiedAt(tileX, tileY + 1);

			if (m_tiles.empty())
			{
				// first tile MUST be center
				if (index != (m_numTiles / 2) * m_numTiles + (m_numTiles / 2))
				{
					auto it = std::find(m_tiles.begin(), m_tiles.end(), tile);
					if (it != m_tiles.end())
						m_tiles.erase(it);

					tile->snapToTile(SIZE_MAX);
					return;
				}
			}
			else if (!hasAdjacent)
			{
				tile->snapToTile(SIZE_MAX);
				return;
			}

			// --- success ---
			tile->snapToTile(index);
			m_tiles.push_back(tile);
		}

		std::pair<std::vector<size_t>, int> Board::getBadWordIndexesAndScore(const Shop::ModifierManager& modifierManager)
		{
			if (!m_active)
				return { {}, 0 };

			m_words.clear();

			const size_t N = m_numTiles;
			int newScore = 0;

			// Build board grid
			std::vector<char> grid(N * N, '\0');
			for (const Tile* tile : m_tiles)
			{
				if (tile->getIndex() != SIZE_MAX)
					grid[tile->getIndex()] = tile->getTileChar();
			}

			std::unordered_set<size_t> badIndexes;

			auto processWord = [&](const std::string& word,
				const std::vector<size_t>& indexes)
				{
					if (word.length() < 2)
						return;

					if (!m_spellChecker.spell(word))
					{
						badIndexes.insert(indexes.begin(), indexes.end());
						return;
					}

					// Valid word
					if (std::find(m_words.begin(), m_words.end(), word) == m_words.end())
						m_words.push_back(word);

					for (char c : word)
					{
						int bonusScore = modifierManager.getBonusPoints(c, 0, "charScored");
						if (bonusScore == 0)
						{
							std::string key(1, c);
							newScore += m_letterScores["tileBasePoints"].at(key).get<int>();
						}
						else
							newScore += bonusScore;
							
					}
				};

			// --------------------
			// Horizontal scan
			// --------------------
			for (size_t y = 0; y < N; ++y)
			{
				std::string word;
				std::vector<size_t> indexes;

				for (size_t x = 0; x < N; ++x)
				{
					size_t idx = y * N + x;
					char c = grid[idx];

					if (c != '\0')
					{
						word.push_back(c);
						indexes.push_back(idx);
					}
					else
					{
						processWord(word, indexes);
						word.clear();
						indexes.clear();
					}
				}

				processWord(word, indexes);
			}

			// --------------------
			// Vertical scan
			// --------------------
			for (size_t x = 0; x < N; ++x)
			{
				std::string word;
				std::vector<size_t> indexes;

				for (size_t y = 0; y < N; ++y)
				{
					size_t idx = y * N + x;
					char c = grid[idx];

					if (c != '\0')
					{
						word.push_back(c);
						indexes.push_back(idx);
					}
					else
					{
						processWord(word, indexes);
						word.clear();
						indexes.clear();
					}
				}

				processWord(word, indexes);
			}

			// Finalize score
			int delta = newScore - m_score;
			m_score = newScore;

			return {
				std::vector<size_t>(badIndexes.begin(), badIndexes.end()),
				delta
			};
		}

		size_t Board::getSnapTileIndex(glm::vec2 pos)
		{
			if (!m_active)
				return SIZE_MAX;

			constexpr float snapMargin = 10.0f;

			const float minX = m_texRect.x;
			const float minY = m_texRect.y;
			const float maxX = m_texRect.x + m_texRect.w;
			const float maxY = m_texRect.y + m_texRect.h;

			// Reject if too far from board
			if (pos.x < minX - snapMargin || pos.x > maxX + snapMargin ||
				pos.y < minY - snapMargin || pos.y > maxY + snapMargin)
			{
				return SIZE_MAX;
			}

			const float tileSize = m_texRect.w / static_cast<float>(m_numTiles);

			// Convert to board-local space
			const float localX = pos.x - minX;
			const float localY = pos.y - minY;

			// Tile index based on top-left corner
			const int tileX = static_cast<int>(std::floor(localX / tileSize));
			const int tileY = static_cast<int>(std::floor(localY / tileSize));

			// Validate indices
			if (tileX < 0 || tileX >= static_cast<int>(m_numTiles) ||
				tileY < 0 || tileY >= static_cast<int>(m_numTiles))
			{
				return SIZE_MAX;
			}

			const size_t index = static_cast<size_t>(tileY) * m_numTiles + tileX;
			return index;
		}

		size_t Board::getNumTiles() const
		{
			return m_numTiles;
		}

		const std::vector<std::string>& Board::getWordsOnBoard() const
		{
			return m_words;
		}
	}
}