#include "LayerManager.h"
#include "Window.h"
#include "Layers/UILayer.h"

#include <vector>

struct LayerManager::PImpl {
    LayerManager& m_parent;
    std::shared_ptr<Window> m_window;

    std::vector<std::unique_ptr<Layer>> m_layers;
    
    PImpl(LayerManager& parent, const std::shared_ptr<Window> window);
    ~PImpl();

    Layer* addLayer(std::unique_ptr<Layer> layer);
};

LayerManager::PImpl::PImpl(LayerManager& parent, const std::shared_ptr<Window> window)
    : m_parent(parent),
      m_window(window)
{

}

LayerManager::PImpl::~PImpl()
{
    m_layers.clear();
}

Layer* LayerManager::PImpl::addLayer(std::unique_ptr<Layer> layer)
{
    m_layers.emplace_back(std::move(layer));
    return m_layers.back().get();
}

LayerManager::LayerManager(const std::shared_ptr<Window>& window)
    : m_pImpl(std::make_unique<PImpl>(*this, window))
{

}

LayerManager::~LayerManager()
{

}

std::uint32_t LayerManager::getWindowHeight() const 
{
    return m_pImpl->m_window->height();
}

std::uint32_t LayerManager::getWindowWidth() const
{
    return m_pImpl->m_window->width();
}

GLFWwindow* LayerManager::getWindow() const
{
    return m_pImpl->m_window->window();
}

void LayerManager::insertNewLayer(std::unique_ptr<Layer> layer)
{
    auto insertedLayer = m_pImpl->addLayer(std::move(layer));
    insertedLayer->init();
}

void LayerManager::updateLayers()
{
    for (const auto& l : m_pImpl->m_layers) {
        l->onUpdate();
    }
}

void LayerManager::renderLayers()
{
    for (const auto& l : m_pImpl->m_layers) {

        if (l->isImGuiLayer()) {
            UILayer* uiLayer = static_cast<UILayer*>(l.get());
            uiLayer->startSceneUI();
        }
        
        l->onRender();
        
        if (l->isImGuiLayer()) {
            UILayer* uiLayer = static_cast<UILayer*>(l.get());
            uiLayer->endSceneUI();
        }

    }

}
