#include "ModifierManager.hpp"

namespace App
{
	namespace Shop
	{
		ModifierManager::ModifierManager()
		{
			namespace fs = std::filesystem;

			std::string path = "./config/modifiers/config/"; // Path to the directory (e.g., current directory)
			try
			{
				for (const auto& entry : fs::directory_iterator(path))
				{
					ModifierInfo newModifierInfo;
					std::ifstream file(entry.path());
					file >> newModifierInfo.json;

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

		int ModifierManager::getBonusPoints(const std::vector<std::string>& words, int points) const
		{
			int bonusPoints = 0;
			for (const auto& [key, modifier] : m_modifiers)
			{
				bonusPoints += modifier->getBonusRoundPoints({ .event = "", .words = words, .points = points });
			}
			return bonusPoints;
		}

		int ModifierManager::getStaticPriceReduction(int points)
		{
			int reduced = 0;
			for (const auto& [key, modifier] : m_modifiers)
			{
				reduced -= (points - (points * modifier->getStaticPriceReduction()));
			}
			return reduced;
		}

		void /*std::array<const std::reference_wrapper<ModifierInfo>, 3>*/ ModifierManager::getShopOptions() const
		{

		}

		void ModifierManager::selectOption(const std::string& id)
		{
			if (m_modifierInfo.contains(id) && m_modifierInfo.at(id).stackable)
			{
				const ModifierInfo& modifierInfo = m_modifierInfo.at(id);
				bool hasScript = false;
				if (!modifierInfo.json.contains("script"))
				{
					Console::ccout << "[config/modifiers/config ERROR] No script field specified. Did you mean \"script\": false? (ID): " << id << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
				}
				else
					hasScript = (modifierInfo.json["script"].get<std::string>() != "<NO-SCRIPT>");

				std::unordered_map<Modifier::StaticModifierType, int> staticModifierInfo;
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
						auto result = Modifier::stringToStaticModifer(key);
						if (result)
						{
							staticModifierInfo.insert(std::pair{
								result.value(),
								val.get<int>()
								});
						}
						else
						{
							Console::ccout << "[config/modifiers/config ERROR] Invalid modifier: " << result.error() << " (ID) : " << id << std::endl;
							auto [begin, end] = Console::cchat.getMessageIterators();
							size_t elem = std::distance(begin, end) - 1;
							Console::Message& mes = Console::cchat.getMessage(elem);
							mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						}
					}
				}

				m_modifiers.emplace(std::pair{
					id,
					std::make_unique<Modifier>(
						staticModifierInfo,				
						true,					
						(hasScript ? std::make_unique<LuaScripting::Script>(modifierInfo.json["script"].get<std::string>()) : nullptr)
					)
				});
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
	}
}