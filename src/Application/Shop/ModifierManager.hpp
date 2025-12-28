#pragma once
#include <memory>
#include <unordered_map>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>

#include <nlohmann/json.hpp>

#include "Modifier.hpp"

namespace App
{
	namespace Shop
	{
		struct ModifierInfo
		{
			ModifierInfo() { localID = s_numModifiers;  s_numModifiers++; }
			~ModifierInfo() { s_numModifiers--; }

			nlohmann::json json;
			std::string id;
			std::string description;
			int rarity = 0;
			int cost = 0;
			bool stackable = false;

			size_t localID = 0;

		private:
			static inline size_t s_numModifiers;
		};

		class ModifierManager
		{
		public:
			ModifierManager();

			int getBonusPoints(const std::vector<std::string>& words, int points, const char* event, const int numRemainingTiles) const;

			int getBonusPoints(char ch, int points, const char* event) const;

			int getStaticPriceReduction(int points);

			std::vector<std::reference_wrapper<const ModifierInfo>> getShopOptions(size_t num) const;

			void selectOption(const std::string& id);

			void listModifiersInChat() const;

			void listActiveModifiersInChat() const;
			 
		private:
			std::unordered_map<std::string, ModifierInfo> m_modifierInfo;
			std::unordered_map<std::string, std::unique_ptr<Modifier>> m_modifiers;
			std::vector<std::string> m_indexToID;
		};
	}
}