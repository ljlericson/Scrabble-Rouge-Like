#include "GameLayer.hpp"
#include "../../Utils/CmdParser/include/ljl/Cmd.hpp"

namespace App
{
	namespace LayerSystem
	{
		/*class GameLayer : public BasicLayer
		{
		public:
			GameLayer();

			void attach(const EventSystem::EventDispatcher& eventDispatcher) override;

			virtual void render(const Core::SDLBackend::Renderer& renderer) override;

		private:
			std::unique_ptr<Shop::Shop> m_shop;
			std::unique_ptr<Shop::ModifierManager> m_modifierManager;
			std::unique_ptr<GameComponents::Board> m_scrabbleBoard;
			std::unique_ptr<GameComponents::GameplayManager> m_gameplayManager;
			std::unique_ptr<UIComponents::Button> m_button;

			std::shared_ptr<Core::SDLBackend::Texture> m_backgroundTex;
		};*/

		GameLayer::GameLayer(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Window& window, const Core::SDLBackend::Renderer& renderer)
			: mr_eventDispatcher(eventDispatcher)
		{
			m_modifierManager = std::make_unique<Shop::ModifierManager>();
			m_shop = std::make_unique<Shop::Shop>(m_modifierManager.get());
			m_scrabbleBoard = std::make_unique<GameComponents::Board>(renderer, window);
			m_gameplayManager = std::make_unique<GameComponents::GameplayManager>(eventDispatcher, renderer, *m_modifierManager, *m_scrabbleBoard, m_scrabbleBoard->getNumTiles(), 21);
			m_button = std::make_unique<UIComponents::Button>(renderer, SDL_FRect{ .x = 1280 - 200, .y = 200, .w = 111.0f, .h = 55.0f }, "Dummy Button");

			m_backgroundTex = Core::AssetManager::textureManager->newTexture("background", renderer.getRendHand(), "./assets/Textures/GameComponents/Background.png");
		}

		void GameLayer::attach(EventSystem::EventDispatcher& eventDispatcher)
		{
			eventDispatcher.attach(*m_gameplayManager);
			eventDispatcher.attach(*m_scrabbleBoard);
			eventDispatcher.attach(*m_button);
		}

		void GameLayer::dettach(EventSystem::EventDispatcher& eventDispatcher)
		{
			eventDispatcher.dettach(*m_gameplayManager);
			eventDispatcher.dettach(*m_scrabbleBoard);
			eventDispatcher.dettach(*m_button);
		}

		void GameLayer::render(const Core::SDLBackend::Renderer& renderer)
		{
			if (m_button->pressed())
			{
				/*m_eventDispatcher.queueEvent(EventType::roundStart);
				m_eventDispatcher.queueEvent(EventType::wordConfirmed);*/
				Console::ccout << "BUTTON PRESSED" << std::endl;
			}
			
			// background texture
			renderer.render(*m_backgroundTex, SDL_FRect(0.0f, 0.0f, (float)Utils::getWindowSize().first, (float)Utils::getWindowSize().second));
			// game components
			m_scrabbleBoard->render(renderer);
			m_gameplayManager->render(renderer);
			m_button->render(renderer);
			m_shop->render();


			// ---------------------------------------------------------
			// ---------------------------------------------------------
			// ---------------------------------------------------------
			// ---------------------------------------------------------


			// IMGUI RENDERING
			ImGui::Begin("CONTROLS");
			if (ImGui::CollapsingHeader("GAME EVENTS"))
			{
				if (ImGui::Button("Populate Shop"))
					m_shop->populateShop();
				if (ImGui::Button("Confirm Word"))
					mr_eventDispatcher.queueEvent(EventType::wordConfirmed);
				if (ImGui::Button("End Game"))
					mr_eventDispatcher.queueEvent(EventType::roundEnd);
				if (ImGui::Button("Start Game"))
					mr_eventDispatcher.queueEvent(EventType::roundStart);
				if (ImGui::Button("Run TM Garbage Collector"))
					Core::AssetManager::textureManager->runGarbargeCollector();
			}
			if (ImGui::CollapsingHeader("Dev Mode"))
			{
				ImGui::Text("Number of tiles left: %d", m_gameplayManager->getNumTilesLeft());
				if (!m_devMode)
					ImGui::OpenPopup("Dev Mode");
			}
			// This code must be called every frame, outside the button's if statement
			if (ImGui::BeginPopupModal("Dev Mode", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				std::string input;

				if (ImGui::InputText("##Password", &input, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_Password))
				{
					if (input == "password")
					{
						m_devMode = true;
						mr_eventDispatcher.queueEvent(EventType::enterDevMode);
						Console::ccout << "DEVMODE ACTIVE" << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(0.0f, 255.0f, 0.0f, 255.0f);
						ImGui::CloseCurrentPopup();
					}
					else
					{
						Console::ccout << "INCORRECT DEVMODE PASSWORD" << std::endl;
						auto [begin, end] = Console::cchat.getMessageIterators();
						size_t elem = std::distance(begin, end) - 1;
						Console::Message& mes = Console::cchat.getMessage(elem);
						mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
			ImGui::End();

			ImGui::Begin("Menu");
			if (ImGui::Button("Go to menu"))
				mr_eventDispatcher.queueEvent(EventType::transferToMenuLayer);
			ImGui::End();

			if (m_devMode)
			{
				if (ljl::cmdparser* cmd = Console::cchat.draw())
				{
					auto& cmdr = *cmd;
					if (cmdr.is(ljl::cmdparser::type::query))
						cmdr.respond();
					else if (cmdr.is(ljl::cmdparser::type::command))
					{
						if (cmdr["mod"])
							m_modifierManager->selectOption(cmdr.get_value<std::string>("mod", "-new"));
						if (cmdr["listMods"])
							m_modifierManager->listModifiersInChat();
						if (cmdr["listActiveMods"])
							m_modifierManager->listActiveModifiersInChat();
					}
				}
			}

			// END IMGUI RENDERING
		}

		GameLayer::~GameLayer()
		{
			mr_eventDispatcher.dettach(*m_gameplayManager);
			mr_eventDispatcher.dettach(*m_scrabbleBoard);
			mr_eventDispatcher.dettach(*m_button);
		}
	}
}