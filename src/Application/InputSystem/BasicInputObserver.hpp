#pragma once
#include <vector>
#include <functional>
#include <SDL3/SDL.h>

namespace App
{
	namespace InputSystem
	{
		class BasicInputObserver
		{
		public:
			virtual ~BasicInputObserver() = default;

			virtual void onInput(const bool* keyboardState) = 0;
		};
	}
}