#pragma once
#include <iostream>
#include <memory>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <imgui.h>
//#include <imgui_impl_sdl3.h>

#include "../Core/AssetManager/TextureManager.hpp"
#include "../Core/SDL2Backend/Window.hpp"
#include "../Core/SDL2Backend/Texture.hpp"
#include "../Core/SDL2Backend/Renderer.hpp"
#include "InputSystem/InputListener.hpp"
#include "InputSystem/BasicInputObserver.hpp"

#include "GameComponents/Board.hpp"




namespace App
{
	class Application
	{
	public:
		Application();
		void run();
		~Application();

	private:
		std::unique_ptr<Core::SDLBackend::Window> m_window;
		std::unique_ptr<Core::SDLBackend::Renderer> m_renderer;
		InputSystem::InputListener m_inputListener;

		GameComponents::Board m_scrabbleBoard;
	};
}