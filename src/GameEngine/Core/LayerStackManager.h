#pragma once

#include <GameEngine/Core/BasicLayer.h>

#include <set>
#include <memory>

namespace Engine
{
    /*!
     * @brief Implements application abstraction in layers
    */
    class LayerStackManager
    {
        public:
            ~LayerStackManager();

            /*!
             * @brief Attaches the given layer
             * @param layer Layer to attaches
            */
            void AttacheLayer(const std::shared_ptr<BasicLayer>& layer);

            /*!
             * @brief Detaches the given layer
             * @param layer Layer to detaches
             * @return True if layer was detached. False means layer not in stack.
            */
            bool DetacheLayer(const std::shared_ptr<BasicLayer>& layer);

            /*!
             * @brief Updates all layers. First runs "update" then "draw"
             * @param dt Delta time
             * @return True when all updates where successfully
            */
            bool UpdateLayers(float dt);

            /*!
             * @brief Detaches all layers
            */
            void ClearLayers();

        private:
            /*!
             * @brief Struct for comparing layers priorities
            */
            struct CompareStruct
            {
                bool operator()(const std::shared_ptr<BasicLayer>& lhs, const std::shared_ptr<BasicLayer>& rhs) const
                {
                    std::less<int>;
                    return lhs->GetLayerIndex() < rhs->GetLayerIndex();
                }
            };

        private:
            std::set<std::shared_ptr<BasicLayer>, CompareStruct> m_layers;
    };
}
