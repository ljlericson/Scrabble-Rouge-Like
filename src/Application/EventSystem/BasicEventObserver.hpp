#pragma once
#include <vector>
#include <functional>
#include <SDL3/SDL.h>

#include "EventType.hpp"

namespace App
{
	namespace EventSystem
	{
		class BasicEventObserver
		{
		public:
			virtual ~BasicEventObserver() = default;

			virtual void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) = 0;
		};
	}
}