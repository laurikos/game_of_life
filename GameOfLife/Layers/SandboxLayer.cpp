#include "SandboxLayer.h"
#include "../LayerManager.h"
#include "../TemporaryRenderer.h"
#include "../Camera.h"
#include "../Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct SandboxLayer::PImpl {
    SandboxLayer& m_parent;
    LayerManager* m_manager;

    // Should move to Render class or something
    // lets see..
    struct RendererData
    {
        std::uint32_t vertexArray;
        std::uint32_t vertexBuffer;
        std::uint32_t indexBuffer;
        std::unique_ptr<Shader> shader;
    };

    std::unique_ptr<RendererData> m_rendererData;
    
    PImpl(SandboxLayer& parent, LayerManager* manager);
    ~PImpl();

    void init();
    void onUpdate(float deltaTime);
    void onRender();

    void startSceneUI();
    void endSceneUI();
};

SandboxLayer::PImpl::PImpl(SandboxLayer& parent, LayerManager* manager)
    : m_parent(parent),
      m_manager(manager),
      m_rendererData(std::make_unique<RendererData>())
{

}

SandboxLayer::PImpl::~PImpl()
{
}

void SandboxLayer::PImpl::init()
{
    // auto uiLayer = layerManager->insertNewLayer(std::make_unique<UILayer>(layerManager));
    // assert(uiLayer->isImGuiLayer() && "should've been UI layer");
    
    glGenVertexArrays(1, &m_rendererData->vertexArray);
    glBindVertexArray(m_rendererData->vertexArray);

    glGenBuffers(1, &m_rendererData->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererData->vertexBuffer);

    float vertices[3 * 3] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &m_rendererData->indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererData->indexBuffer);

    std::uint32_t indices[3] = { 0, 1, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::string vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
}

)";

    std::string fragmentShaderSource = R"(
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.22, 0.44, 0.33, 1.0);
}

)";
    
    m_rendererData->shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    
}

void SandboxLayer::PImpl::onUpdate(float deltaTime)
{
    TemporaryRenderer::setClearColor();
    TemporaryRenderer::clear();

    m_rendererData->shader->bind();
    glBindVertexArray(m_rendererData->vertexArray);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::PImpl::onRender()
{
}


SandboxLayer::SandboxLayer(LayerManager* manager)
    : m_pImpl(std::make_unique<PImpl>(*this, manager))
{
    
}

SandboxLayer::~SandboxLayer()
{

}

void SandboxLayer::init()
{
    m_pImpl->init();
}

void SandboxLayer::onUpdate(float deltaTime)
{
    m_pImpl->onUpdate(deltaTime);
}

void SandboxLayer::onRender()
{
    m_pImpl->onRender();
}

/*
struct RendererData
{
    std::uint32_t vertexArray;
    std::uint32_t vertexBuffer;
    std::uint32_t indexBuffer;
    std::unique_ptr<Shader> shader;

    RendererData() {}
};

void run(Window* window, LayerManager* layerManager)
{
    auto uiLayer = layerManager->insertNewLayer(std::make_unique<UILayer>(layerManager));
    assert(uiLayer->isImGuiLayer() && "should've been UI layer");

    std::unique_ptr<RendererData> rendererData = std::make_unique<RendererData>();
    
    glGenVertexArrays(1, &rendererData->vertexArray);
    glBindVertexArray(rendererData->vertexArray);

    glGenBuffers(1, &rendererData->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, rendererData->vertexBuffer);

    float vertices[3 * 3] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &rendererData->indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererData->indexBuffer);

    std::uint32_t indices[3] = { 0, 1, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::string vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
}

)";

    std::string fragmentShaderSource = R"(
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.22, 0.44, 0.33, 1.0);
}

)";
    
    rendererData->shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    
    float lastTime = 0.0f;
    while (isRunning) {
        float time = static_cast<float>(glfwGetTime());
        // handle timestep:
        deltaTime = time - lastTime;
        lastTime = time;

        TemporaryRenderer::setClearColor();
        TemporaryRenderer::clear();

        rendererData->shader->bind();
        glBindVertexArray(rendererData->vertexArray);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        if (glfwWindowShouldClose(window->window())) {
            isRunning = false;
        }
        
        layerManager->updateLayers(deltaTime);
        layerManager->renderLayers();
        window->onUpdate();
    }
}

 */
