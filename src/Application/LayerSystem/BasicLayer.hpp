#pragma once

#include "../../Core/SDLBackend/Renderer.hpp"
#include "../EventSystem/EventDispatcher.hpp"

namespace App
{
	namespace LayerSystem
	{
		class BasicLayer
		{
		public:
			virtual ~BasicLayer() = default;

			virtual void attach(EventSystem::EventDispatcher& eventDispatcher) = 0;

			virtual void dettach(EventSystem::EventDispatcher& eventDispatcher) = 0;

			virtual void render(const Core::SDLBackend::Renderer& renderer) = 0;
		};
	}
}