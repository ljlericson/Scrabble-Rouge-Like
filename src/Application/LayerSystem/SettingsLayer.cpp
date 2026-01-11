#include "SettingsLayer.hpp"
#include "../../Utils/CmdParser/include/ljl/Cmd.hpp"

namespace App
{
	namespace LayerSystem
	{
		SettingsLayer::SettingsLayer(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer)
			: mr_eventDispatcher(eventDispatcher)
		{
			m_backgroundTex = Core::AssetManager::textureManager->newTexture("settingsBackground", renderer.getRendHand(), "./assets/Textures/GameComponents/Tint.png");
		}

		void SettingsLayer::render(const Core::SDLBackend::Renderer& renderer)
		{
			using namespace std::chrono_literals;
			if(!m_fadedIn)
			{
				auto animationState = renderer.fadeIn(*m_backgroundTex, SDL_FRect(0.0f, 0.0f, (float)Utils::getWindowSize().first, (float)Utils::getWindowSize().second), 200ms);
				if(animationState == Core::SDLBackend::Renderer::AnimationState::finished)
					m_fadedIn = true;
			}
			else
			{
				renderer.render(*m_backgroundTex, SDL_FRect(0.0f, 0.0f, (float)Utils::getWindowSize().first, (float)Utils::getWindowSize().second));
			}

			if(m_active)
				ImGui::Begin("Settings Menu", nullptr, 
					ImGuiWindowFlags_AlwaysAutoResize |
					ImGuiWindowFlags_NoBackground | 
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize | 
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoMove);
			else
				ImGui::Begin("Settings Menu", nullptr,
					ImGuiWindowFlags_AlwaysAutoResize |
					ImGuiWindowFlags_NoBackground |
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoInputs);

			static int langCurrentItem = 0;
			const char* langItems[] = { "EnUS", "EnNZ", "EnAU" };

			if (ImGui::BeginListBox("##languageSelection", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) 
			{
				for (int n = 0; n < IM_ARRAYSIZE(langItems); n++)
				{
					const bool isSelected = (langCurrentItem == n);

					if (ImGui::Selectable(langItems[n], isSelected))
						langCurrentItem = n;

					if (isSelected) 
						ImGui::SetItemDefaultFocus();

				}
				ImGui::EndListBox();
			}

			if (ImGui::Button("Apply"))
			{
				switch (langCurrentItem)
				{
				case 0:
					mr_eventDispatcher.queueEvent(EventType::switchTo_USEng_Lang);
					break;
				case 1:
					mr_eventDispatcher.queueEvent(EventType::switchTo_NZEng_Lang);
					break;
				case 2:
					mr_eventDispatcher.queueEvent(EventType::switchTo_AUEng_Lang);
					break;
				}
			}


			ImGui::End();
		}

		SettingsLayer::~SettingsLayer()
		{

		}
	}
}