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

		int Modifier::getBonusRoundPoints(const Context& context)
		{
			int scriptBonus = 0;
			if (m_script)
			{
				sol::table contextTable = LuaScripting::Script::getState().create_table();
				contextTable["event"] = context.event;
				contextTable["words"] = context.words;
				auto result = m_script->run(contextTable);
				if (result.has_value())
					scriptBonus = result.value()["addScore"];
			}
			if (m_staticModifiers.contains(StaticModifierType::pointsScoreMultiplier))
			{
				scriptBonus += ((context.points * m_staticModifiers.at(StaticModifierType::pointsScoreMultiplier))
					- context.points);
			}
			return scriptBonus;
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