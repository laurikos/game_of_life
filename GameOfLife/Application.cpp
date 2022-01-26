#include "Application.h"
#include "TemporaryRenderer.h"
#include "Layers/UILayer.h"
#include "Layers/SandboxLayer.h"
#include "Layers/GameOfLife.h"
#include "Window.h"
#include "LayerManager.h"

#include <GLFW/glfw3.h>

Application::Application()
    : m_window(std::make_shared<Window>()),
      m_layerManager(std::make_unique<LayerManager>(m_window))
{
    TemporaryRenderer::init();
    TemporaryRenderer::setViewport(0, 0, m_window->width(), m_window->height());
    
    m_isRunning = true;

    m_layerManager->insertNewLayer(std::make_unique<UILayer>(m_layerManager.get()));
#ifdef SANDBOX_APP
    m_layerManager->insertNewLayer(std::make_unique<SandboxLayer>(m_layerManager.get()));
#endif
    m_layerManager->insertNewLayer(std::make_unique<GameOfLife>(m_layerManager.get()));
}

Application::~Application()
{
    TemporaryRenderer::close();
}

void Application::run()
{
    while (m_isRunning) {
        float time = static_cast<float>(glfwGetTime());
        float deltaTime = time - m_lastTime;
        m_lastTime = time;

        if (glfwWindowShouldClose(m_window->window())) {
            m_isRunning = false;
        }
        
        m_layerManager->updateLayers(deltaTime);
        m_layerManager->renderLayers();
        
        m_window->onUpdate();
    }
}
