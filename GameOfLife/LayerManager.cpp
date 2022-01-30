#include "LayerManager.h"
#include "Window.h"
#include "Layers/UILayer.h"
#include "Layers/GameOfLife.h"

#include <vector>

struct LayerManager::PImpl {
    LayerManager& m_parent;
    std::shared_ptr<Window> m_window;

    std::vector<std::unique_ptr<Layer>> m_layers;    
    UILayer* m_uiLayer = nullptr;
    Layer* m_gameLayer = nullptr;
    bool m_isGameLayerInitialized;

    PImpl(LayerManager& parent, const std::shared_ptr<Window> window);
    ~PImpl();

    Layer* addLayer(std::unique_ptr<Layer> layer);

};

LayerManager::PImpl::PImpl(LayerManager& parent, const std::shared_ptr<Window> window)
    : m_parent(parent),
      m_window(window),
      m_isGameLayerInitialized(false)
{
}

LayerManager::PImpl::~PImpl()
{
    m_layers.clear();
}

Layer* LayerManager::PImpl::addLayer(std::unique_ptr<Layer> layer)
{
    m_layers.emplace_back(std::move(layer));
    Layer* insertedLayer = m_layers.back().get();
    if (insertedLayer->isImGuiLayer()) {
        m_uiLayer = static_cast<UILayer*>(insertedLayer);
    }
    return insertedLayer;
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

Layer* LayerManager::insertNewLayer(std::unique_ptr<Layer> layer)
{
    auto insertedLayer = m_pImpl->addLayer(std::move(layer));
    insertedLayer->init();
    return insertedLayer;
}

void LayerManager::updateLayers(float deltaTime)
{
    for (const auto& l : m_pImpl->m_layers) {
        l->onUpdate(deltaTime);
    }
}

void LayerManager::renderLayers()
{
    if (!m_pImpl->m_uiLayer) { return; }
    
    m_pImpl->m_uiLayer->startSceneUI();

    for (const auto& l : m_pImpl->m_layers) {
        l->onRender();
    }

    m_pImpl->m_uiLayer->endSceneUI(); 
}

void LayerManager::sendEventToLayer(Layer* layer, Event& event)
{
    layer->onEvent(event);
}

Layer* LayerManager::settingUpDone(const std::vector<std::vector<std::uint32_t>>& initState)
{
    m_pImpl->m_isGameLayerInitialized = true;
    m_pImpl->m_gameLayer = insertNewLayer(std::make_unique<GameOfLife>(this, initState));
    return m_pImpl->m_gameLayer;
}

bool LayerManager::isGameLayerInitialized() const
{
    return m_pImpl->m_isGameLayerInitialized;
}

Layer* LayerManager::gameLayer()
{
    return m_pImpl->m_gameLayer;
}
