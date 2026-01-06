#pragma once
#include <unordered_map>
#include <type_traits>
#include <typeindex>
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
				std::type_index ti = typeid(T);
				if (!m_layers.contains(ti))
				{
					m_layers.insert(std::pair{
						ti,
						std::make_unique<T>(std::forward<Args>(args)...)
						});
					m_layers.at(typeid(T))->attach(mr_eventDispatcher);
				}
				else
				{
					std::cout << "WARNING: Layer of same type pushed twice\n";
				}
			}

			template<typename T> requires std::is_base_of_v<BasicLayer, T>
			void popLayer();

			void render(const Core::SDLBackend::Renderer& renderer);

		private:
			std::unordered_map<std::type_index, std::unique_ptr<BasicLayer>> m_layers;
			EventSystem::EventDispatcher& mr_eventDispatcher;
		};
	}
}