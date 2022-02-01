#pragma once

#include "Camera.h"
#include "OpenGLObjects.h"

#include <glm/glm.hpp>

/*
 * Renderer (2D) for GameOfLife.
 * Fully static class
 *
 * Initialize before use
 * Usage in layers "onUpdate()":
 * 1. start() -- starts the scene
 * 2. drawQuad() -- draw X number of quads (builds the batch of vertices to draw)
 * 3. end() -- flushes the resulted VAO to GPU and executes the draw call.
 *
 * Also handles the auxiliary rendering commands like clear/clearcolor
 */
class Renderer
{
public:
    static void init();
    static void close();    

    static void handleResize(std::uint32_t width, std::uint32_t height);
    static void setViewport(std::uint32_t x, std::uint32_t y,
                            std::uint32_t width, std::uint32_t height);

    static void setClearColor(const glm::vec4& color = { 0.1f, 0.1f, 0.1f, 1.0f });
    static void clear();
    
    static void start(Camera& camera);
    static void end();

    static void drawQuad(const glm::vec3& position, const glm::vec2 size,
                         const glm::vec4& color);

    static void execute(const std::shared_ptr<VAO>& vao,
                        std::uint32_t indexCount);
};
