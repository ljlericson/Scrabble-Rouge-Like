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
			nlohmann::json json;
			std::string id;
			std::string description;
			int rarity = 0;
			int cost = 0;
			bool stackable = false;
		};

		class ModifierManager
		{
		public:
			ModifierManager();

			int getBonusPoints(const std::vector<std::string>& words, int points) const;

			void /*std::array<const std::reference_wrapper<ModifierInfo>, 3>*/ getShopOptions() const;

			void selectOption(const std::string& id);
			 
		private:
			std::unordered_map<std::string, ModifierInfo> m_modifierInfo;
			std::unordered_map<std::string, std::unique_ptr<Modifier>> m_modifiers;
		};
	}
}