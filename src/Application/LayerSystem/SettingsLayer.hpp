#pragma once
#include <memory>

#include "BasicLayer.hpp"

#include "../GameComponents/Board.hpp"
#include "../GameComponents/GameplayManager.hpp"
#include "../UIComponents/Button.hpp"
#include "../Shop/Shop.hpp"
#include "../Shop/ModifierManager.hpp"
#include "../Console/ChatStream.h"

#include "../../Core/SDLBackend/Renderer.hpp"
#include "../../Core/SDLBackend/Window.hpp"
#include "../../Core/AssetManager/AssetManager.hpp"

namespace App
{
	namespace LayerSystem
	{
		class SettingsLayer : public BasicLayer
		{
		public:
			SettingsLayer(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer);

			void render(const Core::SDLBackend::Renderer& renderer) override;

			~SettingsLayer() override;
		private:
			bool m_fadedIn = false;

			std::shared_ptr<Core::SDLBackend::Texture> m_backgroundTex;

			EventSystem::EventDispatcher& mr_eventDispatcher;
		};
		
	}
}