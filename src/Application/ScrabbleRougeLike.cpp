#include "ScrabbleRougeLike.hpp"
#include "../Utils/CmdParser/include/ljl/Cmd.hpp"


namespace App
{
	Application::Application()
		: mf_ImGuiEventCallback([](SDL_Event* e) { ImGui_ImplSDL3_ProcessEvent(e); })
	{
		namespace fs = std::filesystem;
		std::ifstream file("path.json");
		if (file)
		{
			try
			{
				nlohmann::json data = nlohmann::json::parse(file);

				fs::path relativePath = data.at("path").get<std::string>();
				fs::current_path(relativePath);

				std::cout << "INFO: Moved to: " << fs::current_path() << '\n';
			}
			catch (const nlohmann::json::parse_error& e1)
			{
				std::cerr << "ERROR: JSON parse error: " << e1.what() << '\n';
			}
			catch (const nlohmann::json::exception& e2)
			{
				std::cerr << "ERROR: JSON error: " << e2.what() << '\n';
			}
			catch (const fs::filesystem_error& e3)
			{
				std::cerr << "ERROR: Filesystem error: " << e3.what() << '\n';
			}
		}

		Console::cchat.loadJson("./config/console/args.json");

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

		m_eventDispatcher = std::make_unique<EventSystem::EventDispatcher>();
		m_layerStack = std::make_unique<LayerSystem::LayerStack>(*m_eventDispatcher);
		m_stateManager = std::make_unique<StateManager>(*m_eventDispatcher, *m_layerStack, *m_renderer, *m_window);

		m_eventDispatcher->queueEvent(EventType::transferToStartState);
	}

	void Application::run()
	{
		while (!m_window->shouldClose())
		{
			 // update window size
			Utils::updateWindowSize(m_window->getWHand());
			// custom callback for ImGui
			m_window->pollEvents(mf_ImGuiEventCallback);
			// poll engine events && updates stateManager
			m_eventDispatcher->poll(*m_window);

			// start pre-render
			m_renderer->preRender();
			this->ImGuiPreRender();

			// render layers
			m_layerStack->render(*m_renderer);			

			// start post-render
			this->ImGuiPostRender();
			m_renderer->postRender();

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






	// helper functions

	void Application::ImGuiPreRender()
	{
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		this->ImGuiTheme();
	}

	void Application::ImGuiPostRender()
	{
		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer->getRendHand());
	}

	void Application::ImGuiTheme()
	{
		// AdobeInspired style by nexacopic from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6000000238418579f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 4.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.ChildRounding = 4.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 4.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 4.0f;
		style.FrameBorderSize = 1.0f;
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 14.0f;
		style.ScrollbarRounding = 4.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 20.0f;
		style.TabRounding = 4.0f;
		style.TabBorderSize = 1.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
		style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.1630901098251343f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.08627451211214066f, 0.08627451211214066f, 0.08627451211214066f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8784313797950745f, 0.8784313797950745f, 0.8784313797950745f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.9803921580314636f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.1490196138620377f, 0.1490196138620377f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2470588237047195f, 0.2470588237047195f, 0.2470588237047195f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3294117748737335f, 0.3294117748737335f, 0.3294117748737335f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.3098039329051971f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.800000011920929f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7490196228027344f, 0.7490196228027344f, 0.7490196228027344f, 0.7803921699523926f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7490196228027344f, 0.7490196228027344f, 0.7490196228027344f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 0.6705882549285889f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.9490196108818054f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.2235294133424759f, 0.2235294133424759f, 0.2235294133424759f, 0.8627451062202454f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.321568638086319f, 0.800000011920929f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.2745098173618317f, 0.2745098173618317f, 0.2745098173618317f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 0.9725490212440491f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.4235294163227081f, 0.4235294163227081f, 0.4235294163227081f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
	}
}