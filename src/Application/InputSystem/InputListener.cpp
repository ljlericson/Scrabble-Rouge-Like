#include "InputListener.hpp"
#include "../GameComponents/Board.hpp"

namespace App
{
	namespace InputSystem
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

		template<class T> requires std::is_base_of_v<BasicInputObserver, T>
		void InputListener::attach(T& observer)
		{
			m_observers.push_back(&observer);
		}

		template<class T> requires std::is_base_of_v<BasicInputObserver, T>
		void InputListener::dettach(T& observer)
		{
			auto it = std::find(m_observers.begin(), m_observers.end(), &observer);
			if (it != m_observers.end())
				m_observers.erase(it);
			else
				std::cout << "WARNING: No observer found InputListener::dettach()\n";
		}

		void InputListener::poll()
		{
			// REQUIRED: pumps SDL input state
			SDL_PumpEvents();

			const bool* keyboardState = SDL_GetKeyboardState(nullptr);

			for (auto* observer : m_observers)
			{
				observer->onInput(keyboardState);
			}
		}
	}

	template void InputSystem::InputListener::attach<App::GameComponents::Board>(App::GameComponents::Board&);
	template void InputSystem::InputListener::dettach<App::GameComponents::Board>(App::GameComponents::Board&);
}