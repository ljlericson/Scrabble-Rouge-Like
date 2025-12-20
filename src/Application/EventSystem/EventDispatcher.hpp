#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <algorithm>
#include <type_traits>

#include <SDL3/SDL.h>

#include "BasicEventObserver.hpp"
#include "EventType.hpp"
#include "../../Core/SDL2Backend/Window.hpp"

namespace App
{
	namespace EventSystem
	{
		class EventDispatcher
		{
		public:
			EventDispatcher() = default;


			template<class T> requires std::is_base_of_v<BasicEventObserver, T>
			void attach(T& observer);

			template<class T> requires std::is_base_of_v<BasicEventObserver, T>
			void dettach(T& observer);

			void reserveObserverVectorCapacity(size_t numObservers);

			auto getObserverVectorIterators()
				-> std::pair<std::vector<BasicEventObserver*>::iterator, std::vector<BasicEventObserver*>::iterator> const;
			
			void queueEvent(EventType e);

			void poll(const Core::SDLBackend::Window& window);

		private:
			std::vector<BasicEventObserver*> m_observers;
			std::vector<EventType> m_eventQueue;
		};
	}
}