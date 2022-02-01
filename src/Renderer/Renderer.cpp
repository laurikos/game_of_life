#include "Renderer.h"
#include "OpenGLObjects.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

static std::string vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main()
{
    v_Color = vec4(a_Color, 1.0);
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

)";

static std::string fragmentShaderSource = R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}

)";

struct QuadVertex
{
    glm::vec3 position;
    glm::vec3 color;
};


struct RendererData
{
    const std::uint32_t maxQuads = 10000;
    const std::uint32_t maxVertices = maxQuads * 4;
    const std::uint32_t maxIndices = maxQuads * 6;
    static const std::uint32_t maxTextureSlots = 32; // remove if not needed.

    std::shared_ptr<VAO> vao;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<Shader> shader;
    
    std::uint32_t quadIndicesCount = 0;

    std::unique_ptr<QuadVertex[]> quadVertices = nullptr;
    QuadVertex* pQuadVertices = nullptr;

    glm::vec4 vertexPositions[4];
};

static RendererData s_data;

void Renderer::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    
    s_data.vao = std::make_shared<VAO>();
    s_data.vbo = std::make_shared<VBO>(s_data.maxVertices * sizeof(QuadVertex));

    s_data.vbo->setBuffer("a_Position", 4 * 3);
    s_data.vbo->setBuffer("a_Color", 4 * 3);
    s_data.vbo->uploadBuffers();
    
    s_data.vao->uploadVBO(s_data.vbo);

    s_data.quadVertices = std::make_unique<QuadVertex[]>(s_data.maxVertices);

    auto quadIndices = std::make_unique<std::uint32_t[]>(s_data.maxIndices);

    std::size_t offset = 0;
    for (std::size_t i = 0; i < s_data.maxIndices; i += 6) {

        // first triangle of quad:
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        // second triangle of quad:
        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    std::shared_ptr<IBO> quadIBO = std::make_shared<IBO>(quadIndices.get(), s_data.maxIndices);
    s_data.vao->setIBO(quadIBO);

    s_data.shader = std::make_shared<Shader>(vertexShaderSource, fragmentShaderSource);
    s_data.shader->bind();

    s_data.vertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f };
    s_data.vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    s_data.vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
    s_data.vertexPositions[3] = {-0.5f,  0.5f, 0.0f, 1.0f };
}

void Renderer::close() {}

void Renderer::setClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       
}

void Renderer::handleResize(std::uint32_t width, std::uint32_t height)
{
    Renderer::setViewport(0, 0, width, height);
}

void Renderer::setViewport(std::uint32_t x, std::uint32_t y,
                            std::uint32_t width, std::uint32_t height)
{
    glViewport(x, y, width, height);
}

void Renderer::start(Camera& camera)
{
    s_data.shader->bind();
    s_data.shader->uploadUniformMat4("u_ViewProjection", camera.viewProjectionMatrix());

    s_data.quadIndicesCount = 0;
    s_data.pQuadVertices = s_data.quadVertices.get();
}

void Renderer::end()
{
    std::uint32_t dataSize = reinterpret_cast<std::uint8_t*>(s_data.pQuadVertices)
        - reinterpret_cast<std::uint8_t*>(s_data.quadVertices.get());

    s_data.vbo->setData(s_data.quadVertices.get(), dataSize);

    execute(s_data.vao, s_data.quadIndicesCount);    
}

void Renderer::drawQuad(const glm::vec3& position,
                                 const glm::vec2 size, const glm::vec4& color)
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    // Bottom left
    s_data.pQuadVertices->position = transform * s_data.vertexPositions[0];
    s_data.pQuadVertices->color = {color.r, color.g, color.b };
    s_data.pQuadVertices++;

    // Bottom right
    s_data.pQuadVertices->position = transform * s_data.vertexPositions[1];
    s_data.pQuadVertices->color = {color.r, color.g, color.b };
    s_data.pQuadVertices++;

    // Top right
    s_data.pQuadVertices->position = transform * s_data.vertexPositions[2];
    s_data.pQuadVertices->color = {color.r, color.g, color.b };
    s_data.pQuadVertices++;

    // Top left
    s_data.pQuadVertices->position = transform * s_data.vertexPositions[3];
    s_data.pQuadVertices->color = {color.r, color.g, color.b };
    s_data.pQuadVertices++;

    // quad has 6 indices 0-1-2 & 2-3-0
    s_data.quadIndicesCount += 6;
}

void Renderer::execute(const std::shared_ptr<VAO>& vao,
                                std::uint32_t indexCount)
{
    std::uint32_t count = !indexCount ?
        vao->getIBO()->indicesCount() :
        indexCount;

    vao->bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

