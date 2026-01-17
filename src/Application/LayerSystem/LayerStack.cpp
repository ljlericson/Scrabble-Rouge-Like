#include "LayerStack.hpp"
#include "GameLayer.hpp"
#include "StartLayer.hpp"
#include "SettingsLayer.hpp"
#include "ShopLayer.hpp"


namespace App
{
    namespace LayerSystem
    {
        LayerStack::LayerStack(EventSystem::EventDispatcher& eventDispatcher)
        : mr_eventDispatcher(eventDispatcher) {  }

        template<typename T> requires std::is_base_of_v<BasicLayer, T>
        void LayerStack::popLayer()
        {
            auto it = std::find_if(m_layers.begin(), m_layers.end(), [](const std::unique_ptr<BasicLayer>& layer)
                {
                    return typeid(*layer) == typeid(T);
                });

            if (it != m_layers.end())
            {
                m_layers.erase(it);
                if(m_layers.size() > 0)
                    m_layers.front()->toggleInactive(true);
            }
        }

        template<typename T> requires std::is_base_of_v<BasicLayer, T>
        bool LayerStack::layerActive()
        {
            auto it = std::find_if(m_layers.begin(), m_layers.end(), [](const std::unique_ptr<BasicLayer>& layer)
            {
                return typeid(*layer) == typeid(T);
            });

            return it != m_layers.end();
        }

        template<typename T> requires std::is_base_of_v<BasicLayer, T>
        std::expected<std::reference_wrapper<T>, std::string> LayerStack::getLayer()
        {
            auto it = std::find_if(m_layers.begin(), m_layers.end(), [](const std::unique_ptr<BasicLayer>& layer)
            {
                return typeid(*layer) == typeid(T);
            });

            if(it != m_layers.end())
                return static_cast<T&>(*it->get());

             return std::unexpected<std::string>("ERROR: No layer found");
        }

        void LayerStack::render(const Core::SDLBackend::Renderer& renderer)
        {
            for(auto& layer : m_layers)
                layer->render(renderer);
        }

        void LayerStack::clear()
        {
            m_layers.clear();
        }

        template void LayerStack::popLayer<GameLayer>();
        template void LayerStack::popLayer<StartLayer>();
        template void LayerStack::popLayer<SettingsLayer>();
        template void LayerStack::popLayer<ShopLayer>();

        template bool LayerStack::layerActive<GameLayer>();
        template bool LayerStack::layerActive<StartLayer>();
        template bool LayerStack::layerActive<SettingsLayer>();
        template bool LayerStack::layerActive<ShopLayer>();

        template std::expected<std::reference_wrapper<GameLayer>, std::string> LayerStack::getLayer<GameLayer>();
        template std::expected<std::reference_wrapper<StartLayer>, std::string> LayerStack::getLayer<StartLayer>();
        template std::expected<std::reference_wrapper<SettingsLayer>, std::string> LayerStack::getLayer<SettingsLayer>();
        template std::expected<std::reference_wrapper<ShopLayer>, std::string> LayerStack::getLayer<ShopLayer>();
    }
}