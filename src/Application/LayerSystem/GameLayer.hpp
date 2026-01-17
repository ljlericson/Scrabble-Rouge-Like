#pragma once
#include <string_view>

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
		class GameLayer : public BasicLayer
		{
		public:
			GameLayer(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Window& window, const Core::SDLBackend::Renderer& renderer, std::string_view savePath);

			void render(const Core::SDLBackend::Renderer& renderer) override;

			Shop::ModifierManager* getModifierManager();

			~GameLayer() override;

		private:
			std::unique_ptr<Shop::Shop> m_shop;
			std::unique_ptr<Shop::ModifierManager> m_modifierManager;
			std::unique_ptr<GameComponents::Board> m_scrabbleBoard;
			std::unique_ptr<GameComponents::GameplayManager> m_gameplayManager;
			std::unique_ptr<UIComponents::Button> m_button;

			std::shared_ptr<Core::SDLBackend::Texture> m_backgroundTex;

			EventSystem::EventDispatcher& mr_eventDispatcher;

			bool m_devMode = false;
		};
	}
}