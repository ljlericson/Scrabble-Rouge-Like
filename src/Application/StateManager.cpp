#include "StateManager.hpp"

namespace App
{
	StateManager::StateManager(EventSystem::EventDispatcher& eventDispatcher, LayerSystem::LayerStack& layerStack, Core::SDLBackend::Renderer& r, Core::SDLBackend::Window& w)
		: mr_eventDispatcher(eventDispatcher), mr_layerStack(layerStack), mr_renderer(r), mr_window(w)
	{
		mr_eventDispatcher.attach(*this);
	}

	StateManager::~StateManager()
	{
		mr_eventDispatcher.dettach(*this);
	}

	void StateManager::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
	{
		// settings logic
		if(mr_layerStack.layerActive<LayerSystem::GameLayer>())
		{
			bool escPressed = keyboardState[SDL_SCANCODE_ESCAPE];
			bool escJustPressed = escPressed && !m_prevEscPressed;
			m_prevEscPressed = escPressed;
			if (escJustPressed)
			{
				if (m_state == State::game)
				{
					mr_layerStack.pushLayer<LayerSystem::SettingsLayer>(mr_eventDispatcher, mr_renderer);
					m_state = State::settingsMenu;
				}
				else if (m_state == State::settingsMenu)
				{
					mr_layerStack.popLayer<LayerSystem::SettingsLayer>();
					m_state = State::game;
				}
			}
		}

		switch (e)
		{
		case EventType::roundEnd:
			{
				auto result = mr_layerStack.getLayer<LayerSystem::GameLayer>();
				if(result)
					mr_layerStack.pushLayer<LayerSystem::ShopLayer>(mr_eventDispatcher, result.value().get().getModifierManager());
				else
					std::cout << result.error();
			}
			break;

		case EventType::shopPhaseEnd:
			mr_layerStack.popLayer<LayerSystem::ShopLayer>();
			mr_eventDispatcher.queueEvent(EventType::roundStart);
			mr_eventDispatcher.queueEvent(EventType::wordConfirmed);
			break;

		case EventType::transferToGameState:
			mr_layerStack.clear();
			mr_layerStack.pushLayer<LayerSystem::GameLayer>(mr_eventDispatcher, mr_window, mr_renderer, "");
			m_state = State::game;
			break;

		case EventType::transferToStartState:
			mr_layerStack.clear();
			mr_layerStack.pushLayer<LayerSystem::StartLayer>(mr_eventDispatcher, mr_renderer);
			m_state = State::startScreen;
			break;

		case EventType::transferToEndState:
			mr_layerStack.clear();
			mr_layerStack.pushLayer<LayerSystem::EndLayer>(mr_eventDispatcher, mr_renderer);
			m_state = State::startScreen;
			break;
		}
	}
}