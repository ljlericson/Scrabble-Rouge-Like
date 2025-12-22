#pragma once
#include <memory>
#include <vector>
#include <string>

#include <sol/sol.hpp>

#include "../LuaScripting/Script.hpp"

namespace App
{
	namespace Shop
	{
		struct Context
		{
			const char* event;
			std::vector<std::string> words;
			int points;
		};

		class Modifier
		{
		public:
			Modifier(std::string name,
					std::string description,
					int globalPriceReduction,
					int pointsScoreMultiplier,
					int rounStartingPoints,
					bool stackable,
				    std::unique_ptr<LuaScripting::Script> script = nullptr);
			
			int getPoints(const Context& context);

			int getRarity() const;

			int getCost() const;

			int getStartPointsBonus() const;

			bool getStackable() const;

		private:
			std::unique_ptr<LuaScripting::Script> m_script;

			// config items
			std::string m_name;
			std::string m_description;
			int m_rarity = 0;
			int m_cost = 0;
			int m_globalPriceReduction = 0;
			int m_pointsScoredMultiplier = 0;
			int m_roundStartingPoints = 0;
			bool m_stackable = false;
		};
	}
}