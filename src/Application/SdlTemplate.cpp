#include "SdlTemplate.hpp"
#include "../Utils/CmdParser/include/ljl/Cmd.hpp"


namespace App
{
	void Application::ImGuiTheme()
	{
		// Photoshop style by Derydoca from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6000000238418579f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 4.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 4.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 2.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 2.0f;
		style.FrameBorderSize = 1.0f;
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 13.0f;
		style.ScrollbarRounding = 12.0f;
		style.GrabMinSize = 7.0f;
		style.GrabRounding = 0.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 1.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.2627451121807098f, 0.2627451121807098f, 0.2627451121807098f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2745098173618317f, 0.2745098173618317f, 0.2745098173618317f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2980392277240753f, 0.2980392277240753f, 0.2980392277240753f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1560000032186508f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.3910000026226044f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.2627451121807098f, 0.2627451121807098f, 0.2627451121807098f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.6700000166893005f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.5843137502670288f, 0.5843137502670288f, 0.5843137502670288f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.1560000032186508f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5860000252723694f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5860000252723694f);
	}

	void Application::ImGuiPreRender()
	{
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		this->ImGuiTheme();
	}

	void Application::ImGuiRenderStartUI()
	{
		ImGui::Begin("Game");
		if (ImGui::Button("Start Game"))
			m_layer = Layer::Game;
		ImGui::End();
	}

	void Application::ImGuiRenderGameUI()
	{
		ImGui::Begin("CONTROLS");
		if(ImGui::CollapsingHeader("GAME EVENTS"))
		{
			if (ImGui::Button("Populate Shop"))
				m_shop->populateShop();
			if (ImGui::Button("Confirm Word"))
				m_eventDispatcher.queueEvent(EventType::wordConfirmed);
			if (ImGui::Button("End Game"))
				m_eventDispatcher.queueEvent(EventType::gameEnd);
			if (ImGui::Button("Start Game"))
				m_eventDispatcher.queueEvent(EventType::gameStart);
			if (ImGui::Button("Run TM Garbage Collector"))
				Core::AssetManager::textureManager->runGarbargeCollector();
			if (ImGui::Button("Toggle Fullscreen"))
			{
				m_fullscreen = m_fullscreen ? false : true;
				SDL_DisplayID displayID = SDL_GetDisplayForWindow(m_window->getWHand());
				SDL_DisplayMode closestMode;
				if (!SDL_GetClosestFullscreenDisplayMode(displayID, 1280, 720, 0, true, &closestMode))
				{
					Console::ccout << "WINDOW FAILED TO FULLSCREEN" << std::endl;
					auto [begin, end] = Console::cchat.getMessageIterators();
					size_t elem = std::distance(begin, end) - 1;
					Console::Message& mes = Console::cchat.getMessage(elem);
					mes.color = ImVec4(0.0f, 255.0f, 0.0f, 255.0f);
				}
				SDL_SetWindowFullscreen(m_window->getWHand(), m_fullscreen);
			}
			if (ImGui::Button("Main Menu"))
				m_layer = Layer::Start;
		}
		if (ImGui::CollapsingHeader("Dev Mode"))
		{
			ImGui::Text("Number of tiles left: %d", m_playerHand->getNumTilesLeft());
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
					m_eventDispatcher.queueEvent(EventType::enterDevMode);
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

		if(m_devMode)
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

	}

	void Application::ImGuiPostRender()
	{
		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer->getRendHand());
	}


	Application::Application()
		: mf_ImGuiEventCallback([](SDL_Event* e) { ImGui_ImplSDL3_ProcessEvent(e); })
	{

        if (!SDL_Init(SDL_INIT_EVENTS))
            std::cerr << "SDL events failed to init, error: " << SDL_GetError() << '\n';

        if (!SDL_Init(SDL_INIT_VIDEO))
            std::cerr << "SDL video failed to init, error: " << SDL_GetError() << '\n';

		if (!TTF_Init())
			std::cerr << "TTF failed to init, error: " << SDL_GetError() << '\n';

		SDL_Log("SDL version: %d", SDL_GetVersion());
		
		m_window = std::make_unique<Core::SDLBackend::Window>("Scrabble", 1280, 720);
		m_renderer = std::make_unique<Core::SDLBackend::Renderer>(*m_window);

		Utils::updateWindowSize(m_window->getWHand());

		// 2. Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplSDL3_InitForSDLRenderer(m_window->getWHand(), m_renderer->getRendHand());
		ImGui_ImplSDLRenderer3_Init(m_renderer->getRendHand());

		m_modifierManager = std::make_unique<Shop::ModifierManager>();
		m_shop = std::make_unique<Shop::Shop>(m_modifierManager.get());
		m_scrabbleBoard = std::make_unique<GameComponents::Board>(*m_renderer, *m_window);
		m_playerHand = std::make_unique<GameComponents::PlayerHand>(m_eventDispatcher, *m_renderer, *m_modifierManager, *m_scrabbleBoard, m_scrabbleBoard->getNumTiles(), 21);
		m_button = UIComponents::Button(*m_renderer, SDL_FRect{ .x = 1280 - 200, .y = 200, .w = 111.0f, .h = 55.0f }, "Start Game");

		m_backgroundTex = Core::AssetManager::textureManager->newTexture("background", m_renderer->getRendHand(), "./assets/textures/background.png");
		m_menuTex = Core::AssetManager::textureManager->newTexture("menu", m_renderer->getRendHand(), "./assets/textures/test.jpg");

		m_eventDispatcher.attach(*m_scrabbleBoard);
		m_eventDispatcher.attach(m_button);

		m_layer = Layer::Start; // should be Layer::Start but i start it in game for easier debug

	}

	void Application::run()
	{
		while (!m_window->shouldClose())
		{
			switch(m_layer)
			{
			case Layer::Start: // ----------------------------

				// update systems
				Utils::updateWindowSize(m_window->getWHand());
				m_window->pollEvents(mf_ImGuiEventCallback);

				m_renderer->preRender();
				m_renderer->render(*m_menuTex, SDL_FRect(0.0f, 0.0f, (float)Utils::getWindowSize().first, (float)Utils::getWindowSize().second));
				this->ImGuiPreRender();
				this->ImGuiRenderStartUI();
				this->ImGuiPostRender();
				m_renderer->postRender();

				break; // ~ Start

			case Layer::Game: // ----------------------------

				// update systems
				Utils::updateWindowSize(m_window->getWHand());
				if (m_button.pressed())
				{
					m_eventDispatcher.queueEvent(EventType::gameStart);
					m_eventDispatcher.queueEvent(EventType::wordConfirmed);
				}
				// custom callback for ImGui
				m_window->pollEvents(mf_ImGuiEventCallback);
				m_eventDispatcher.poll(*m_window);
				// rendering
				m_renderer->preRender();
				// background texture
				m_renderer->render(*m_backgroundTex, SDL_FRect(0.0f, 0.0f, (float)Utils::getWindowSize().first, (float)Utils::getWindowSize().second));
				// game components
				m_scrabbleBoard->render(*m_renderer);
				m_playerHand->render(*m_renderer);
				m_button.render(*m_renderer);

				this->ImGuiPreRender();
				this->ImGuiRenderGameUI();
				m_shop->render();
				this->ImGuiPostRender();
				m_renderer->postRender();


				break; // ~ Game

			} // ~ switch m_layer

			SDL_Delay(10); 
		}
	}

	Application::~Application()
	{
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
		SDL_Quit();
	}
}