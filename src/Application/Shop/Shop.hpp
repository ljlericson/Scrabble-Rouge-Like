#pragma once
#include <vector>
#include <functional>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "../Console/ChatStream.h"
#include "../EventSystem/BasicEventObserver.hpp"
#include "ModifierManager.hpp"

namespace App
{
	namespace Shop
	{
		class Shop : public EventSystem::BasicEventObserver
		{
		public:
			enum class UIDisabled
			{
				true_,
				false_
			};
		public:
			Shop(ModifierManager* manager);

			void populateShop();

			void render(UIDisabled active);

			void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;

		private:
			std::vector<std::reference_wrapper<const ModifierInfo>> m_modifierInfo;
			ModifierManager* mrp_manager;

			bool m_render = false;
		};
	}
}