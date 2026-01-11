#pragma once
#include <vector>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <iostream>

#include "../../Core/SDLBackend/Renderer.hpp"
#include "BasicLayer.hpp"

namespace App
{
	namespace LayerSystem
	{
		class LayerStack
		{
		public:
			LayerStack(EventSystem::EventDispatcher& eventDispatcher);

			template<typename T, typename... Args> requires std::is_base_of_v<BasicLayer, T>
			void pushLayer(Args&&... args)
			{
				auto it = std::find_if(m_layers.begin(), m_layers.end(), [](const std::unique_ptr<BasicLayer>& layer)
				{
					return typeid(*layer) == typeid(T);
				});

				if (it == m_layers.end())
				{
					if(m_layers.size() > 0)
						m_layers.front()->toggleInactive(false);

					m_layers.push_back(std::make_unique<T>(std::forward<Args>(args)...));
				}
				else
				{
					std::cout << "WARNING: Layer of same type pushed twice\n";
				}
			}

			template<typename T> requires std::is_base_of_v<BasicLayer, T>
			void popLayer();

			template<typename T> requires std::is_base_of_v<BasicLayer, T>
			bool layerActive();

			void render(const Core::SDLBackend::Renderer& renderer);

			void clear();

		private:
			// fixed order required
			std::vector<std::unique_ptr<BasicLayer>> m_layers;
			EventSystem::EventDispatcher& mr_eventDispatcher;
		};
	}
}