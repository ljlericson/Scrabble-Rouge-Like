#pragma once
#include <imgui.h>

#include "BasicLayer.hpp"

#include "../../Core/SDLBackend/Renderer.hpp"
#include "../../Core/SDLBackend/Window.hpp"
#include "../../Core/AssetManager/AssetManager.hpp"

#include "../../Utils/Utils.hpp"

namespace App
{
	namespace LayerSystem
	{
		class StartLayer : public BasicLayer
		{
		public:
			StartLayer(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer);

			void attach(EventSystem::EventDispatcher& eventDispatcher) override;

			void dettach(EventSystem::EventDispatcher& eventDispatcher) override;

			void render(const Core::SDLBackend::Renderer& renderer) override;

			~StartLayer() override;

		private:
			std::shared_ptr<Core::SDLBackend::Texture> m_backgroundTex;
			EventSystem::EventDispatcher& mr_eventDispatcher;
		};
	}
}