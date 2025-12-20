#include "Window.hpp"

namespace Core
{
	namespace SDLBackend
	{
		/*class Window
		{
		public:
			Window();

			void pollEvents();
			void changeWinName(const std::string& name);
			bool shouldClose();

			bool getKeyDown(Uint32 keyCode);

		private:
			SDL_Window* m_whand;
			SDL_Event m_event;
		};*/

		Window::Window(const std::string& title, uint16_t w, uint16_t h)
			: m_w(w), m_h(h), m_event{}
		{
			m_whand = SDL_CreateWindow("3D Engine", m_w, m_h, SDL_WINDOW_RESIZABLE);
		}

		void Window::pollEvents()
		{
			m_eventsToHandle.clear();
			while (SDL_PollEvent(&m_event))
			{
				m_eventsToHandle.push_back(m_event.type);
			}
		}

		void Window::pollEvents(const std::function<void(SDL_Event* e)>& callback)
		{
			m_eventsToHandle.clear();
			while (SDL_PollEvent(&m_event))
			{
				callback(&m_event);
				m_eventsToHandle.push_back(m_event.type);
			}
		}

		void Window::changeWinName(const std::string& name)
		{
			SDL_SetWindowTitle(m_whand, name.c_str());
		}

		bool Window::shouldClose()
		{
			return std::find(m_eventsToHandle.begin(), m_eventsToHandle.end(), SDL_EVENT_QUIT) != m_eventsToHandle.end();
		}

		const std::vector<uint32_t>& Window::getUnhandledEvents() const
		{
			return m_eventsToHandle;
		}

		SDL_Window* Window::getWHand() const
		{
			return m_whand;
		}
	}
}