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
			char ch;
			const int numRemainingTiles;
		};

		class Modifier
		{
		public:
			enum class StaticModifierType
			{
				globalPriceReduction,
				pointsScoredMultiplier,
				roundStartingPoints
			};
		public:
			Modifier(const std::unordered_map<StaticModifierType, int>& staticModifiers,
					 bool stackable,
				     std::unique_ptr<LuaScripting::Script> script = nullptr);
			
			int getBonusRoundPoints(const Context& context) const;

			int getStaticStartPointsBonus() const;

			int getStaticPriceReduction() const;

			static auto stringToStaticModifier(const std::string& str) -> std::expected<StaticModifierType, std::string>;

		private:
			std::unique_ptr<LuaScripting::Script> m_script;
			mutable std::vector<std::string> m_words;
			// config items
			std::unordered_map<StaticModifierType, int> m_staticModifiers;
			bool m_stackable = false;
		};
	}
}