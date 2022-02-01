#pragma once

#include <memory>
#include <glm/glm.hpp>

class Camera
{
public:
    Camera(float left, float right, float bottom, float top);
    ~Camera();

    void setProjection(float left, float right, float bottom, float top);
    void setPosition(const glm::vec3& position);

    const glm::vec3& position() const;
    const glm::mat4& projectionMatrix() const;
    const glm::mat4& viewMatrix() const;
    const glm::mat4& viewProjectionMatrix() const;

private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
