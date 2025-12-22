#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <expected>

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
			enum class StaticModifierType
			{
				globalPriceReduction,
				pointsScoreMultiplier,
				roundStartingPoints
			};
		public:
			Modifier(const std::unordered_map<StaticModifierType, int>& staticModifiers,
					 bool stackable,
				     std::unique_ptr<LuaScripting::Script> script = nullptr);
			
			int getBonusRoundPoints(const Context& context);

			int getStartPointsBonus() const;

			static auto stringToStaticModifer(const std::string& str) -> std::expected<StaticModifierType, std::string>;

		private:
			std::unique_ptr<LuaScripting::Script> m_script;

			// config items
			std::unordered_map<StaticModifierType, int> m_staticModifiers;
			bool m_stackable = false;
		};
	}
}