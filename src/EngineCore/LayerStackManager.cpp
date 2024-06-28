#include "LayerStackManager.h"

Engine::LayerStackManager::~LayerStackManager()
{
    ClearLayers();
}

void Engine::LayerStackManager::AttacheLayer(const std::shared_ptr<BasicLayer>& layer)
{
    DetacheLayer(layer);
    m_layers.emplace(layer);
    layer->OnAttache();
}

bool Engine::LayerStackManager::DetacheLayer(const std::shared_ptr<BasicLayer>& layer)
{
    auto existingLayerIt = std::find(m_layers.begin(), m_layers.end(), layer);
    if (existingLayerIt != m_layers.end())
    {
        m_layers.erase(existingLayerIt);
        layer->OnDettache();
        return true;
    }
    return false;
}

bool Engine::LayerStackManager::UpdateLayers(float dt)
{
    for (auto& layer : m_layers)
    {
        if (!layer->Update(dt))
        {
            return false;
        }
    }

    for (auto& layer : m_layers)
    {
        layer->Draw();
    }

    return true;
}

void Engine::LayerStackManager::ClearLayers()
{
    while (!m_layers.empty())
    {
        m_layers.begin()->get()->OnDettache();
        m_layers.erase(m_layers.begin());
    }
}
