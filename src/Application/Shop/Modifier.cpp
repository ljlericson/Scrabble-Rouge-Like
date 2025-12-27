#include "Modifier.hpp"

namespace App
{
	namespace Shop
	{
		Modifier::Modifier(const std::unordered_map<StaticModifierType, int>& staticModifiers,
						bool stackable,
						std::unique_ptr<LuaScripting::Script> script)
			: m_staticModifiers(staticModifiers),
			  m_stackable(stackable)
		{		
			if (script)
				m_script = std::move(script);
			else
				m_script = nullptr;
		}

		int Modifier::getBonusRoundPoints(const Context& context) const
		{
			int bonusPoints = 0;

			if (m_script)
			{
				sol::state& lua = LuaScripting::Script::getState();

				sol::table contextTable = lua.create_table();
				contextTable["event"] = context.event;
				contextTable["words"] = sol::as_table(context.words);
				contextTable["points"] = context.points;
				contextTable["char"] = context.ch;

				if (auto bonus = m_script->run(contextTable))
				{
					bonusPoints += bonus.value().get<int>("addScore");
				}

			}

			if (m_staticModifiers.contains(StaticModifierType::pointsScoredMultiplier) && context.ch == ' ')
			{
				bonusPoints += ((context.points * m_staticModifiers.at(StaticModifierType::pointsScoredMultiplier)) -
								context.points) * static_cast<int>(context.words.size() - m_words.size());
				m_words = context.words;
			}

			return bonusPoints;
		}

		int Modifier::getStaticStartPointsBonus() const
		{
			if (m_staticModifiers.contains(StaticModifierType::roundStartingPoints))
			{
				return m_staticModifiers.at(StaticModifierType::roundStartingPoints);
			}
			return 0;
		}

		int Modifier::getStaticPriceReduction() const
		{
			if (m_staticModifiers.contains(StaticModifierType::globalPriceReduction))
			{
				return m_staticModifiers.at(StaticModifierType::globalPriceReduction);
			}
			return 1;
		}

		auto Modifier::stringToStaticModifer(const std::string& str) -> std::expected<StaticModifierType, std::string>
		{
			if (str == "globalPriceReduction")
				return StaticModifierType::globalPriceReduction;
			else if (str == "pointsScoredMultiplier")
				return StaticModifierType::pointsScoredMultiplier;
			else if (str == "roundStartingPoints")
				return StaticModifierType::roundStartingPoints;
			else
				return std::unexpected("No enum found for string");
		}
	}
}