#include "LayerStack.hpp"
#include "GameLayer.hpp"
#include "StartLayer.hpp"

namespace App
{
    namespace LayerSystem
    {
        LayerStack::LayerStack(EventSystem::EventDispatcher& eventDispatcher)
        : mr_eventDispatcher(eventDispatcher) {  }

        template<typename T> requires std::is_base_of_v<BasicLayer, T>
        void LayerStack::popLayer()
        {
            std::type_index ti = typeid(T);
            if(m_layers.contains(ti))
            {
                m_layers.erase(ti);
            }
        }

        void LayerStack::render(const Core::SDLBackend::Renderer& renderer)
        {
            for(auto& [___, layer] : m_layers)
                layer->render(renderer);
        }

        template void LayerStack::popLayer<GameLayer>();
        template void LayerStack::popLayer<StartLayer>();

    }
}