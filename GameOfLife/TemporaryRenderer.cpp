#include "TemporaryRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void TemporaryRenderer::init() {}

void TemporaryRenderer::close() {}

void TemporaryRenderer::setClearColor()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void TemporaryRenderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void TemporaryRenderer::handleResize(std::uint32_t width, std::uint32_t height)
{
}

void TemporaryRenderer::start(const Camera& camera) {}

void TemporaryRenderer::end() {}

void TemporaryRenderer::drawQuad(const glm::vec2& position,
                                 const glm::vec2 size, const glm::vec4& color)
{
}
