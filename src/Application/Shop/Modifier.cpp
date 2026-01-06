#include "Modifier.hpp"

namespace App
{
	namespace Shop
	{
		Modifier::Modifier(const std::unordered_map<StaticModifierType, float>& staticModifiers,
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

		PointsReturn Modifier::getBonusRoundPoints(const Context& context)
		{
			PointsReturn points = { .addScore = 0, .addMult = 0, .mulMult = 0 };

			if (m_script)
			{
				sol::state& lua = LuaScripting::Script::getState();

				sol::table contextTable = lua.create_table();
				contextTable["event"] = context.event;
				contextTable["words"] = sol::as_table(context.words);
				contextTable["points"] = context.points;
				contextTable["char"] = context.ch;
				contextTable["numRemainingTiles"] = context.numRemainingTiles;
				contextTable["wordDelta"] = context.wordDelta;

				
				if (auto bonus = m_script->run(contextTable))
				{
					points.addScore += bonus.value().get<int>("addScore");
					points.addMult += bonus.value().get<float>("addMultScore");
					points.mulMult += bonus.value().get<float>("mulMultScore");
				}

			}

			if (m_staticModifiers.contains(StaticModifierType::pointsScoredMultiplier) && context.ch != ' ')
			{
				points.addMult += m_staticModifiers.at(StaticModifierType::pointsScoredMultiplier);
			}

			return points;
		}

		float Modifier::getStaticPriceReduction() const
		{
			if (m_staticModifiers.contains(StaticModifierType::globalPriceReduction))
			{
				return m_staticModifiers.at(StaticModifierType::globalPriceReduction);
			}
			return 1;
		}

		auto Modifier::stringToStaticModifier(const std::string& str) -> std::expected<StaticModifierType, std::string>
		{
			if (str == "globalPriceReduction")
				return StaticModifierType::globalPriceReduction;
			else if (str == "pointsScoredMultiplier")
				return StaticModifierType::pointsScoredMultiplier;
			else
				return std::unexpected(std::string("No enum found for string \"") + str + std::string("\""));
		}
	}
}