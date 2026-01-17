#pragma once
#include <memory>
#include <imgui.h>

#include "BasicLayer.hpp"

#include "../Shop/Shop.hpp"
#include "../Shop/ModifierManager.hpp"

#include "../../Core/SDLBackend/Renderer.hpp"
#include "../../Core/SDLBackend/Window.hpp"

#include "../../Utils/Utils.hpp"

namespace App
{
	namespace LayerSystem
	{
		class ShopLayer : public BasicLayer
		{
		public:
			ShopLayer(EventSystem::EventDispatcher& eventDispatcher, Shop::ModifierManager* modifierManager);

			void render(const Core::SDLBackend::Renderer& renderer) override;

			~ShopLayer() override;

		private:
			std::unique_ptr<Shop::Shop> m_shop;
			EventSystem::EventDispatcher& mr_eventDispatcher;
		};
	}
}