#pragma once

#include "Camera.h"

#include <glm/glm.hpp>


class TemporaryRenderer
{
public:
    static void init();
    static void close();    

    static void handleResize(std::uint32_t width, std::uint32_t height);

    static void setClearColor();
    static void clear();
    
    static void start(const Camera& camera);
    static void end();

    static void drawQuad(const glm::vec2& position, const glm::vec2 size,
                         const glm::vec4& color);

    static void execute();
};
