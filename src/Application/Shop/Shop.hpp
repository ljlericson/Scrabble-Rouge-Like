#pragma once
#include <vector>
#include <functional>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "../Console/ChatStream.h"
#include "ModifierManager.hpp"

namespace App
{
	namespace Shop
	{
		class Shop
		{
		public:
			Shop(ModifierManager* manager);

			void populateShop();

			void render();

		private:
			std::vector<std::reference_wrapper<const ModifierInfo>> m_modifierInfo;
			ModifierManager* mrp_manager;
		};
	}
}