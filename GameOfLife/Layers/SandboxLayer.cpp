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
    std::unique_ptr<Camera> m_camera;
    // Should move to Render class or something
    // lets see..
    // struct RendererData
    // {
    //     std::uint32_t vertexArray;
    //     std::uint32_t vertexBuffer;
    //     std::uint32_t indexBuffer;
    //     std::unique_ptr<Shader> shader;
    // };

    // std::unique_ptr<RendererData> m_rendererData;
    
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
      // m_rendererData(std::make_unique<RendererData>()),
      m_camera(std::make_unique<Camera>(
                   -1 * (1200.0f / 720.0f) * 3.0f,
                   (1200.0f / 720.0f) * 3.0f,
                   -1 * 3.0f,
                   3.0f))
{
    m_camera->setPosition({ 0.0f, 0.0f, 0.0f });
}

SandboxLayer::PImpl::~PImpl()
{
}

void SandboxLayer::PImpl::init()
{
    // TemporaryRenderer::init();
    // TemporaryRenderer::setViewport(0, 0, 1200, 720);
#ifdef FIRST_DRAFT
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
#endif
}

void SandboxLayer::PImpl::onUpdate(float deltaTime)
{
    TemporaryRenderer::setClearColor();
    TemporaryRenderer::clear();

    TemporaryRenderer::start(*(m_camera.get()));

    for (int i = -50; i < 50; ++i) {

        for (int j = -50; j < 50; ++j) {
            
            TemporaryRenderer::drawQuad({ (j/10.0f), (i/10.0f), 0.0f },
                                        { 0.08f, 0.08f },
                                        { 0.67f, 0.17f, 0.27f, 1.0f});
        }
        
    }
    
    TemporaryRenderer::end();

#ifdef FIRST_DRAFT
    m_rendererData->shader->bind();
    glBindVertexArray(m_rendererData->vertexArray);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
#endif
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
