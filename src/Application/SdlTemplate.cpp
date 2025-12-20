#include "SdlTemplate.hpp"

namespace App
{
	void Application::ImGuiRender()
	{
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("TEST");
		if (ImGui::Button("End Round"))
			m_eventDispatcher.queueEvent(EventType::roundEnd);
		if (ImGui::Button("Start Round"))
			m_eventDispatcher.queueEvent(EventType::roundStart);
		if (ImGui::Button("End Game"))
			m_eventDispatcher.queueEvent(EventType::gameEnd);
		if (ImGui::Button("Start Game"))
			m_eventDispatcher.queueEvent(EventType::gameStart);
		if (ImGui::Button("Shuffle unused tiles"))
			m_eventDispatcher.queueEvent(EventType::shuffleTiles);
		ImGui::End();
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
		
		m_window = std::make_unique<Core::SDLBackend::Window>("sdltemplate", 1280, 720);
		m_renderer = std::make_unique<Core::SDLBackend::Renderer>(*m_window);

		Utils::updateWindowSize(m_window->getWHand());

		m_scrabbleBoard = std::make_unique<GameComponents::Board>(*m_renderer, *m_window);
		m_playerHand = std::make_unique<GameComponents::PlayerHand>(m_eventDispatcher, *m_renderer, m_scrabbleBoard->getNumTiles(), 21);
		m_button = UIComponents::Button(*m_renderer, SDL_FRect{ .x = 1280 - 200, .y = 200, .w = 111.0f, .h = 55.0f});

		m_eventDispatcher.attach(*m_scrabbleBoard);
		m_eventDispatcher.attach(m_button);

		// 2. Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplSDL3_InitForSDLRenderer(m_window->getWHand(), m_renderer->getRendHand());
		ImGui_ImplSDLRenderer3_Init(m_renderer->getRendHand());
	}

	void Application::run()
	{
		while (!m_window->shouldClose())
		{
			// update systems
			Utils::updateWindowSize(m_window->getWHand());
			// custom cullback for imgui
			m_window->pollEvents(mf_ImGuiEventCallback);
			m_eventDispatcher.poll(*m_window);
			// rendering
			m_renderer->preRender();
			m_scrabbleBoard->render(*m_renderer);
			m_playerHand->render(*m_scrabbleBoard, *m_renderer);
			m_button.render(*m_renderer);
			this->ImGuiRender();
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
}