#include "StartLayer.hpp"

namespace App
{
	namespace LayerSystem
	{
		StartLayer::StartLayer(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer)
			: mr_eventDispatcher(eventDispatcher)
		{
			m_backgroundTex = Core::AssetManager::textureManager->newTexture("menu", renderer.getRendHand(), "./assets/Textures/GameComponents/Background2.jpg");
		}

		void StartLayer::attach(EventSystem::EventDispatcher& eventDispatcher)
		{
			
		}

		void StartLayer::dettach(EventSystem::EventDispatcher& eventDispatcher)
		{

		}

		void StartLayer::render(const Core::SDLBackend::Renderer& renderer)
		{
			renderer.render(*m_backgroundTex, SDL_FRect(0.0f, 0.0f, (float)Utils::getWindowSize().first, (float)Utils::getWindowSize().second));

			ImGui::Begin("Game");
			if (ImGui::Button("Start Game"))
				mr_eventDispatcher.queueEvent(EventType::transferToGameLayer);
			ImGui::End();
		}

		StartLayer::~StartLayer()
		{

		}
	}
}