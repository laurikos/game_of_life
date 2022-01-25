#include "TemporaryRenderer.h"
#include <glad/glad.h>

void TemporaryRenderer::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void TemporaryRenderer::close() {}

void TemporaryRenderer::setClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void TemporaryRenderer::clear()
{
    // glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       
}

void TemporaryRenderer::handleResize(std::uint32_t width, std::uint32_t height)
{
}

void TemporaryRenderer::setViewport(std::uint32_t x, std::uint32_t y,
                            std::uint32_t width, std::uint32_t height)
{
    glViewport(x, y, width, height);
}

void TemporaryRenderer::start(const Camera& camera) {}

void TemporaryRenderer::end() {}

void TemporaryRenderer::drawQuad(const glm::vec2& position,
                                 const glm::vec2 size, const glm::vec4& color)
{
}

void TemporaryRenderer::execute()
{

}
