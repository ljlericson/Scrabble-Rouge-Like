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
		class EndLayer : public BasicLayer
		{
		public:
			EndLayer(EventSystem::EventDispatcher& eventDispatcher, const Core::SDLBackend::Renderer& renderer);

			void render(const Core::SDLBackend::Renderer& renderer) override;

			~EndLayer() override;

		private:
			std::shared_ptr<Core::SDLBackend::Texture> m_backgroundTex;
			EventSystem::EventDispatcher& mr_eventDispatcher;
		};
	}
}