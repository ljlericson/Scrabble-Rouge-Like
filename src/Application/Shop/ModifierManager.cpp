#include "ModifierManager.hpp"

namespace App
{
	namespace Shop
	{
		ModifierManager::ModifierManager()
		{
			namespace fs = std::filesystem;

			std::string path = "./config/modifiers/config/";
			try
			{
				for (const auto& entry : fs::directory_iterator(path))
				{
					ModifierInfo newModifierInfo;
					std::ifstream file(entry.path());

					bool notSuccess = false;
					try
					{
						file >> newModifierInfo.json;
					}
					catch (const nlohmann::json::exception& e)
					{
						Console::ccout << "[config/modifiers/config JSON ERROR]:\n[FILE]: " << std::string(path) << "\n[JSON ERROR]: " << e.what() << "\nMODIFIER SKIPPED" << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						notSuccess = true;
					}

					if (notSuccess)
						continue;

					if (newModifierInfo.json.contains("id"))
						newModifierInfo.id = newModifierInfo.json["id"].get<std::string>();
					else
					{
						Console::ccout << "[config/modifiers/config ERROR] No id field specified for: " << entry.path().string() << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						continue;
					}

					if (newModifierInfo.json.contains("description"))
						newModifierInfo.description = newModifierInfo.json["description"].get<std::string>();
					else
					{
						Console::ccout << "[config/modifiers/config ERROR] No description field specified for: " << entry.path().string() << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						continue;
					}

					if (newModifierInfo.json.contains("cost"))
						newModifierInfo.cost = newModifierInfo.json["cost"].get<int>();
					else
					{
						Console::ccout << "[config/modifiers/config ERROR] No cost field specified for: " << entry.path().string() << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						continue;
					}

					if (newModifierInfo.json.contains("rarity"))
						newModifierInfo.rarity = newModifierInfo.json["rarity"].get<int>();
					else
					{
						Console::ccout << "[config/modifiers/config ERROR] No rarity field specified for: " << entry.path().string() << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						continue;
					}

					if (newModifierInfo.json.contains("stackable"))
						newModifierInfo.stackable = newModifierInfo.json["stackable"].get<bool>();
					else
					{
						Console::ccout << "[config/modifiers/config ERROR] No stackable field specified for: " << entry.path().string() << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						continue;
					}
					m_modifierInfo.insert(std::pair{
						newModifierInfo.id,
						newModifierInfo
					});
					m_indexToID.push_back(newModifierInfo.id);
				}
			}
			catch (const fs::filesystem_error& e)
			{
				Console::ccout << "[ModifierManager ERROR]: Filesystem error: " << e.what() << std::endl;
				auto [begin, end] = Console::cchat.getMessageIterators();
				size_t elem = std::distance(begin, end) - 1;
				Console::Message& mes = Console::cchat.getMessage(elem);
				mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
			}
		}

		int ModifierManager::getBonusPoints(const std::vector<std::string>& words, int points, const char* event, const int numRemainingTiles, const int wordDelta) const
		{
			int add = 0;
			float addMult = 0;
			float mulMult = 0;

			for (const auto& [key, modifier] : m_modifiers)
			{
				auto bonusPoints = modifier->getBonusRoundPoints({ .event = event, .words = words, .points = points, .ch = ' ', .numRemainingTiles = numRemainingTiles, .wordDelta = wordDelta });
				add += bonusPoints.addScore;
				addMult += bonusPoints.addMult;
				mulMult += bonusPoints.mulMult;
			}

			if (mulMult == 0.0f) mulMult = 1.0f;
			if (addMult == 0.0f) addMult = 1.0f;

			float result = (points + add) * addMult * mulMult;
			return static_cast<int>(std::round(result - points));
		}

		int ModifierManager::getBonusPoints(char ch, int points, const char* event) const
		{
			int add = 0;
			float addMult = 0;
			float mulMult = 0;

			for (const auto& [key, modifier] : m_modifiers)
			{
				auto bonusPoints = modifier->getBonusRoundPoints({ .event = event, .words = {}, .points = points, .ch = ch });
				add += bonusPoints.addScore;
				addMult += bonusPoints.addMult;
				mulMult += bonusPoints.mulMult;
			}

			if (mulMult == 0.0f) mulMult = 1.0f;
			if (addMult == 0.0f) addMult = 1.0f;

			float result = (points + add) * addMult * mulMult;
			return result == points ? 0 : static_cast<int>(std::round(result));
		}


		float ModifierManager::getStaticPriceReduction(int points)
		{
			float reduced = 0;
			for (const auto& [key, modifier] : m_modifiers)
			{
				reduced -= (static_cast<float>(points) - (points * modifier->getStaticPriceReduction()));
			}
			return reduced;
		}

		std::vector<std::reference_wrapper<const ModifierInfo>> ModifierManager::getShopOptions(size_t num) const
		{
			std::vector<std::reference_wrapper<const ModifierInfo>> list;
			for (size_t i = 0; i < num; i++)
			{
				bool unique = false;
				do 
				{
					size_t index = Utils::getRandomInt(0, static_cast<int>(m_modifierInfo.size()) - 1);
					const ModifierInfo& potentialModifier = m_modifierInfo.at(m_indexToID.at(index));

					list.push_back(std::cref(potentialModifier));

					if (list.size() > 1) 
					{
						if (list.back().get().id == list.at(list.size() - 2).get().id) 
						{
							list.pop_back();
							unique = false;
						}
						else 
							unique = true;
					}
					else 
						unique = true;

				} while (!unique);
			}

			return list;
		}

		void ModifierManager::selectOption(const std::string& id)
		{
			if (m_modifierInfo.contains(id))
			{
				const ModifierInfo& modifierInfo = m_modifierInfo.at(id);
				bool hasScript = false;
				if (!modifierInfo.json.contains("script"))
				{
					Console::ccout << "[config/modifiers/config ERROR] No script field specified. Did you mean \"script\": \"<NO-SCRIPT>\"? (ID): " << id << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
				}
				else
					hasScript = (modifierInfo.json["script"].get<std::string>() != "<NO-SCRIPT>");

				std::unordered_map<Modifier::StaticModifierType, float> staticModifierInfo;
				if (!modifierInfo.json.contains("staticModifiers"))
				{
					Console::ccout << "[config/modifiers/config ERROR] No staticModifiers field specified, staticModifiers field is REQUIRED (ID): " << id << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
					return;
				}
				else
				{
					for (const auto& [key, val] : modifierInfo.json["staticModifiers"].items())
					{
						auto result = Modifier::stringToStaticModifier(key);
						if (result)
						{
							staticModifierInfo.insert(std::pair{
								result.value(),
								val.get<float>()
								});
						}
						else
						{
							Console::ccout << "[config/modifiers/config ERROR] Invalid staticModifier: " << result.error() << " (ID) : " << id << std::endl;
							auto [begin, end] = Console::cchat.getMessageIterators();
							size_t elem = std::distance(begin, end) - 1;
							Console::Message& mes = Console::cchat.getMessage(elem);
							mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
							return;
						}
					}
				}

				Console::ccout << "[config/modifiers/config INFO] Modifier (ID): " << id << " successfully added" << std::endl;
				auto [begin, end] = Console::cchat.getMessageIterators();
				size_t elem = std::distance(begin, end) - 1;
				Console::Message& mes = Console::cchat.getMessage(elem);
				mes.color = ImVec4(0.0f, 255.0f, 0.0f, 255.0f);

				m_modifiers.insert(std::pair{
					id,
					std::make_unique<Modifier>(
						staticModifierInfo,				
						true,					
						(hasScript ? std::make_unique<LuaScripting::Script>(modifierInfo.json["script"].get<std::string>()) : nullptr)
					)
				});

				if (m_modifierInfo.at(id).stackable)
				{
					m_indexToID.erase(std::find(m_indexToID.begin(), m_indexToID.end(), id));
					m_modifierInfo.erase(id);
				}
			}
			else
			{
				Console::ccout << "[config/modifiers/config ERROR] No modifier exists: " << " (ID) : " << id << std::endl;
				auto [begin, end] = Console::cchat.getMessageIterators();
				size_t elem = std::distance(begin, end) - 1;
				Console::Message& mes = Console::cchat.getMessage(elem);
				mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
			}
		}

		void ModifierManager::listActiveModifiersInChat() const
		{
			Console::ccout << "[config/modifiers/config INFO] Listing active modifiers..." << std::endl;
			auto [begin, end] = Console::cchat.getMessageIterators();
			size_t elem = std::distance(begin, end) - 1;
			Console::Message& mes = Console::cchat.getMessage(elem);
			mes.color = ImVec4(0.0f, 0.0f, 255.0f, 255.0f);
			for (const auto& [id, ____] : m_modifiers)
			{
				Console::ccout << "[config/modifiers/config INFO] Modifier (ID): " << id << std::endl;
				auto [begin, end] = Console::cchat.getMessageIterators();
				size_t elem = std::distance(begin, end) - 1;
				Console::Message& mes = Console::cchat.getMessage(elem);
				mes.color = ImVec4(0.0f, 0.0f, 255.0f, 255.0f);
			}
		}

		void ModifierManager::listModifiersInChat() const
		{
			Console::ccout << "[config/modifiers/config INFO] Listing all modifiers..." << std::endl;
			auto [begin, end] = Console::cchat.getMessageIterators();
			size_t elem = std::distance(begin, end) - 1;
			Console::Message& mes = Console::cchat.getMessage(elem);
			mes.color = ImVec4(0.0f, 0.0f, 255.0f, 255.0f);
			for (const auto& [____, modInfo] : m_modifierInfo)
			{
				Console::ccout << "[config/modifiers/config INFO] Modifier (ID): " << modInfo.id << std::endl;
				auto [begin, end] = Console::cchat.getMessageIterators();
				size_t elem = std::distance(begin, end) - 1;
				Console::Message& mes = Console::cchat.getMessage(elem);
				mes.color = ImVec4(0.0f, 0.0f, 255.0f, 255.0f);
			}
		}
	}
}