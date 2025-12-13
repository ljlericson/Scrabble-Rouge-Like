#include "SdlTemplate.hpp"

namespace App
{
	Application::Application()
	{
        if (!SDL_Init(SDL_INIT_EVENTS))
            std::cerr << "SDL events failed to init, error: " << SDL_GetError() << '\n';

        if (!SDL_Init(SDL_INIT_VIDEO))
            std::cerr << "SDL video failed to init, error: " << SDL_GetError() << '\n';

		SDL_Log("SDL version: %d", SDL_GetVersion());
		
		m_window = std::make_unique<Core::SDLBackend::Window>("sdltemplate", 1280, 720);
		m_renderer = std::make_unique<Core::SDLBackend::Renderer>(*m_window);

		m_scrabbleBoard = GameComponents::Board(*m_renderer);
		m_inputListener.attach(m_scrabbleBoard);

		// imgui init
		/*IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsLight();*/
		//ImGui_ImplSDL3_InitForSDLRenderer(m_window->getWHand(), m_renderer->getRendHand());
	}

	void Application::run()
	{
		while (!m_window->shouldClose())
		{
			m_window->pollEvents();
			m_renderer->preRender();

			m_scrabbleBoard.render(*m_renderer);

			m_renderer->postRender();
			m_inputListener.poll();

			/*ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Test");
			ImGui::Text("This is a test");
			ImGui::End();

			ImGui::Render();
			ImGui_ImplSDL3_Render*/

			SDL_Delay(10);
		}
	}

	Application::~Application()
	{
		SDL_Quit();
	}
}