#pragma once

#include "EventSystem/BasicEventObserver.hpp"
#include "EventSystem/EventDispatcher.hpp"

#include "LayerSystem/LayerStack.hpp"
#include "LayerSystem/EndLayer.hpp"
#include "LayerSystem/GameLayer.hpp"
#include "LayerSystem/StartLayer.hpp"
#include "LayerSystem/SettingsLayer.hpp"
#include "LayerSystem/ShopLayer.hpp"


#include "../Core/SDLBackend/Renderer.hpp"
#include "../Core/SDLBackend/Window.hpp"


namespace App
{
	class StateManager : public EventSystem::BasicEventObserver
	{
	public:
		StateManager(EventSystem::EventDispatcher& eventDispatcher, LayerSystem::LayerStack& layerStack, Core::SDLBackend::Renderer& r, Core::SDLBackend::Window& w);
		~StateManager();

		void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override;

	private:
		enum class State
		{
			startScreen,
			game,
			settingsMenu,
			endScreen
		};
	private:
		EventSystem::EventDispatcher& mr_eventDispatcher;
		LayerSystem::LayerStack& mr_layerStack;

		Core::SDLBackend::Window& mr_window;
		Core::SDLBackend::Renderer& mr_renderer;

		State m_state = State::startScreen;
		bool m_prevEscPressed = false;
	};
}