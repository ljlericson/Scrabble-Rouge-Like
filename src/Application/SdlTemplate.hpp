#pragma once
#include <iostream>
#include <memory>

#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "../Core/AssetManager/TextureManager.hpp"
#include "../Core/SDLBackend/Window.hpp"
#include "../Core/SDLBackend/Texture.hpp"
#include "../Core/SDLBackend/Renderer.hpp"
#include "EventSystem/EventDispatcher.hpp"

#include "Console/ChatStream.h"
#include "Console/ChatConsole.h"
#include "GameComponents/Board.hpp"
#include "GameComponents/PlayerHand.hpp"
#include "UIComponents/Button.hpp"
#include "Shop/Shop.hpp"

#include "../Utils/Utils.hpp"


namespace App
{
	class Application
	{
	public:
		Application();
		void run();
		~Application();

	private:
		void ImGuiRender();
		void ImGuiPostRender();
		void ImGuiTheme();
	private:
		std::unique_ptr<Core::SDLBackend::Window> m_window;
		std::unique_ptr<Core::SDLBackend::Renderer> m_renderer;

		std::unique_ptr<GameComponents::Board> m_scrabbleBoard;
		std::unique_ptr<GameComponents::PlayerHand> m_playerHand;
		UIComponents::Button m_button;

		std::function<void(SDL_Event* e)> mf_ImGuiEventCallback;
		EventSystem::EventDispatcher m_eventDispatcher;

		bool m_fullscreen = false;
	};
}