#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <algorithm>
#include <type_traits>

#include <SDL3/SDL.h>

#include "BasicInputObserver.hpp"

namespace App
{
	namespace InputSystem
	{
		class InputListener
		{
		public:
			InputListener() = default;

			template<class T> requires std::is_base_of_v<BasicInputObserver, T>
			void attach(T& observer);

			template<class T> requires std::is_base_of_v<BasicInputObserver, T>
			void dettach(T& observer);

			void poll();

		private:
			std::vector<BasicInputObserver*> m_observers;
		};
	}
}