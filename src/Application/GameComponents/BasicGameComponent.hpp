#pragma once
#include "../EventSystem/BasicEventObserver.hpp"
#include "../../Core/SDL2Backend/Renderer.hpp"

namespace App
{
	namespace GameComponents
	{
		class BasicGameComponent : public EventSystem::BasicEventObserver
		{
		public:
			virtual ~BasicGameComponent() = default;

			virtual void onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events) override = 0;
		};
	}
}