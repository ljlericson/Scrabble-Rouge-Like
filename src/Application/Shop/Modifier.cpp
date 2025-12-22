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

				if (auto bonus = m_script->run(contextTable))
					bonusPoints += bonus.value().get<int>("addScore");
			}

			if (m_staticModifiers.contains(StaticModifierType::pointsScoreMultiplier))
			{
				bonusPoints += (context.points * m_staticModifiers.at(StaticModifierType::pointsScoreMultiplier)) -
								context.points;
			}

			return bonusPoints;
		}

		int Modifier::getStartPointsBonus() const
		{
			if (m_staticModifiers.contains(StaticModifierType::roundStartingPoints))
			{
				return m_staticModifiers.at(StaticModifierType::roundStartingPoints);
			}
			return -1;
		}

		auto Modifier::stringToStaticModifer(const std::string& str) -> std::expected<StaticModifierType, std::string>
		{
			if (str == "globalPriceReduction")
				return StaticModifierType::globalPriceReduction;
			else if (str == "pointsScoreMultiplier")
				return StaticModifierType::pointsScoreMultiplier;
			else if (str == "roundStartingPoints")
				return StaticModifierType::roundStartingPoints;
			else
				return std::unexpected("No enum found for string");
		}
	}
}