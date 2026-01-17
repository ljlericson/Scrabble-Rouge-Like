#include "ShopLayer.hpp"

namespace App
{
	namespace LayerSystem
	{
		ShopLayer::ShopLayer(EventSystem::EventDispatcher& eventDispatcher, Shop::ModifierManager* modifierManager)
			: mr_eventDispatcher(eventDispatcher), m_shop(std::make_unique<Shop::Shop>(modifierManager))
		{		}

		void ShopLayer::render(const Core::SDLBackend::Renderer& renderer)
		{
			if(m_shop->isEmpty())
				m_shop->populateShop();

			if(m_shop->doneShopping())
				mr_eventDispatcher.queueEvent(EventType::shopPhaseEnd);

			m_shop->render(m_active ? Shop::Shop::UIDisabled::false_ : Shop::Shop::UIDisabled::true_);
		}

		ShopLayer::~ShopLayer()
		{
			
		}
	}
}