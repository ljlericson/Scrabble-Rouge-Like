#include "Modifier.hpp"

namespace App
{
	namespace Shop
	{
		Modifier::Modifier(std::string name,
						std::string description,
						int globalPriceReduction,
						int pointsScoreMultiplier,
						int rounStartingPoints,
						bool stackable,
						std::unique_ptr<LuaScripting::Script> script)
			: m_name(name),
			  m_description(description),
			  m_globalPriceReduction(globalPriceReduction),
			  m_pointsScoredMultiplier(pointsScoreMultiplier),
			  m_roundStartingPoints(rounStartingPoints),
			  m_stackable(stackable)
		{		
			if (script)
				m_script = std::move(script);
			else
				m_script = nullptr;
		}

		int Modifier::getPoints(const Context& context)
		{
			int scriptBonus = 0;
			if(m_script)
			{
				sol::table contextTable = LuaScripting::Script::getState().create_table();
				contextTable["event"] = context.event;
				contextTable["words"] = context.words;
				auto result = m_script->run(contextTable);
				if (result.has_value())
					scriptBonus = result.value()["addScore"];				
			}

			return ((context.points * m_pointsScoredMultiplier) - context.points)
				+ scriptBonus;
		}

		int Modifier::getRarity() const
		{
			return m_rarity;
		}

		int Modifier::getCost() const
		{
			return m_cost;
		}

		int Modifier::getStartPointsBonus() const
		{
			return m_roundStartingPoints;
		}

		bool Modifier::getStackable() const
		{
			return m_stackable;
		}
	}
}