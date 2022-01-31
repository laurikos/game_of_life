#include "Application.h"
#include "TemporaryRenderer.h"
#include "Layers/UILayer.h"
#include "Layers/SandboxLayer.h"
#include "Layers/GameOfLifeSetup.h"
#include "Layers/GameOfLife.h"
#include "Window.h"
#include "LayerManager.h"

#include <GLFW/glfw3.h>

Application::Application()
    : m_window(std::make_shared<Window>("Game of Life", 1280, 720)),
      m_layerManager(std::make_unique<LayerManager>(m_window))
{
    TemporaryRenderer::init();
    TemporaryRenderer::setViewport(0, 0, m_window->width(), m_window->height());
    
    m_isRunning = true;

#ifdef DEBUG_UI_LAYER
    m_layerManager->insertNewLayer(std::make_unique<UILayer>(m_layerManager.get()));
#endif
    
#ifdef SANDBOX_APP
    m_gameLayer = m_layerManager->insertNewLayer(
        std::make_unique<SandboxLayer>(m_layerManager.get()));
#endif

    m_gameSetupLayer =
        m_layerManager->insertNewLayer(std::make_unique<GameOfLifeSetup>(m_layerManager.get()));
}

Application::~Application()
{
    TemporaryRenderer::close();
}

void Application::run()
{
    bool hasEvents = true;
    while (m_isRunning) {
        float time = static_cast<float>(glfwGetTime());
        float deltaTime = time - m_lastTime;
        m_lastTime = time;

        hasEvents = true;
        
        if (glfwWindowShouldClose(m_window->window())) {
            m_isRunning = false;
        }

        if (m_layerManager->isGameLayerInitialized()) {
            m_gameLayer = m_layerManager->gameLayer();
        }
        
        m_layerManager->updateLayers(deltaTime);
        m_layerManager->renderLayers();

        while (hasEvents) {

            auto fetchedEvent = m_window->fetchWindowEvent();            
            if (fetchedEvent.second) { hasEvents = false; }
            onEvent(fetchedEvent.first);
        }
        
        m_window->onUpdate();

    }
}

void Application::onEvent(Event& e)
{
    if (e.type() == EventType::WindowResize) {
        auto values = e.windowValues();
        TemporaryRenderer::handleResize(values.first, values.second);
        m_layerManager->sendEventToLayer(m_gameSetupLayer, e);
    }

    // This is (at least for now) implented since glfwWindowShouldClose
    // works for now.
    if (e.type() == EventType::WindowClose) {
    }

    if (e.type() == EventType::KeyPressed) {
        if (e.keyValue() == GLFW_KEY_ESCAPE) {
            m_isRunning = false;
        }
    }
    
    if (m_gameLayer) {
        m_layerManager->sendEventToLayer(m_gameLayer, e);
    }
}
