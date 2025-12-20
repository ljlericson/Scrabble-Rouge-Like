#include "EventDispatcher.hpp"
#include "../GameComponents/Board.hpp"
#include "../GameComponents/Tile.hpp"
#include "../GameComponents/PlayerHand.hpp"
#include "../UIComponents/Button.hpp"

namespace App
{
	namespace EventSystem
	{
		/*class InputListener
		{
			InputListener() = default;

			void start();
			void stop();

		private:
			void wt_listenForInputs();
		private:
			std::vector<InputObserver> m_observers;
			std::thread m_listenerThread;
		};*/

		template<class T> requires std::is_base_of_v<BasicEventObserver, T>
		void EventDispatcher::attach(T& observer)
		{
			m_observers.push_back(&observer);
		}

		template<class T> requires std::is_base_of_v<BasicEventObserver, T>
		void EventDispatcher::dettach(T& observer)
		{
			auto it = std::find(m_observers.begin(), m_observers.end(), &observer);
			if (it != m_observers.end())
				m_observers.erase(it);
			else
				std::cout << "WARNING: No observer found InputListener::dettach()\n";
		}

		void EventDispatcher::reserveObserverVectorCapacity(size_t numObservers)
		{
			if (m_observers.capacity() < numObservers)
				m_observers.reserve(numObservers);
		}

		auto EventDispatcher::getObserverVectorIterators()
			-> std::pair<std::vector<BasicEventObserver*>::iterator, std::vector<BasicEventObserver*>::iterator> const
		{
			return { m_observers.begin(), m_observers.end() };
		}

		void EventDispatcher::queueEvent(EventType e)
		{
			m_eventQueue.push_back(e);
		}

		void EventDispatcher::poll(const Core::SDLBackend::Window& window)
		{
			SDL_PumpEvents();
			const bool* keyboardState = SDL_GetKeyboardState(nullptr);

			if(!m_eventQueue.empty())
			{
				// avoid invalid iterators from playerhand
				// increasing m_observers on certain update
				for (size_t i = 0; i < m_observers.size(); i++)
					m_observers.at(i)->onInput(keyboardState, m_eventQueue.front(), window.getUnhandledEvents());

				m_eventQueue.erase(m_eventQueue.begin());
			}
			else
			{
				for (auto* observer : m_observers)
					observer->onInput(keyboardState, EventType::noEvent, window.getUnhandledEvents());
			}
		}
	}

	template void EventSystem::EventDispatcher::attach<App::GameComponents::Board>(App::GameComponents::Board&);
	template void EventSystem::EventDispatcher::dettach<App::GameComponents::Board>(App::GameComponents::Board&);

	template void EventSystem::EventDispatcher::attach<App::GameComponents::Tile>(App::GameComponents::Tile&);
	template void EventSystem::EventDispatcher::dettach<App::GameComponents::Tile>(App::GameComponents::Tile&);

	template void EventSystem::EventDispatcher::attach<App::GameComponents::PlayerHand>(App::GameComponents::PlayerHand&);
	template void EventSystem::EventDispatcher::dettach<App::GameComponents::PlayerHand>(App::GameComponents::PlayerHand&);

	template void EventSystem::EventDispatcher::attach<App::UIComponents::Button>(App::UIComponents::Button&);
	template void EventSystem::EventDispatcher::dettach<App::UIComponents::Button>(App::UIComponents::Button&);
}