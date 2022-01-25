#include "Application.h"
#include "TemporaryRenderer.h"
#include "Layers/UILayer.h"
#include "Layers/SandboxLayer.h"
#include "Window.h"
#include "LayerManager.h"

#include <GLFW/glfw3.h>

Application::Application()
    : m_window(std::make_shared<Window>()),
      m_layerManager(std::make_unique<LayerManager>(m_window))
{
    TemporaryRenderer::init();
        
    m_isRunning = true;

    m_layerManager->insertNewLayer(std::make_unique<UILayer>(m_layerManager.get()));
    m_layerManager->insertNewLayer(std::make_unique<SandboxLayer>(m_layerManager.get()));
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

        TemporaryRenderer::setClearColor();
        TemporaryRenderer::clear();

        if (glfwWindowShouldClose(m_window->window())) {
            m_isRunning = false;
        }
        
        m_layerManager->updateLayers(deltaTime);
        m_layerManager->renderLayers();
        m_window->onUpdate();
    }
}
