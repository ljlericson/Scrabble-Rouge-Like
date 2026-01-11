#include "EndLayer.hpp"

namespace App
{
	namespace LayerSystem
	{
		EndLayer::EndLayer(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer)
			: mr_eventDispatcher(eventDispatcher)
		{
			m_backgroundTex = Core::AssetManager::textureManager->newTexture("menu", renderer.getRendHand(), "./assets/Textures/GameComponents/Background2.jpg");
		}

		void EndLayer::render(const Core::SDLBackend::Renderer& renderer)
		{
			renderer.render(*m_backgroundTex, SDL_FRect(0.0f, 0.0f, (float)Utils::getWindowSize().first, (float)Utils::getWindowSize().second));

			if (m_active)
				ImGui::Begin("GAME OVER");
			else
				ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoInputs);

			ImGui::BeginDisabled(!m_active);

			if (ImGui::Button("Start Game"))
				mr_eventDispatcher.queueEvent(EventType::transferToGameState);

			ImGui::EndDisabled();
			ImGui::End();
		}

		EndLayer::~EndLayer()
		{

		}
	}
}