#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

struct Camera::PImpl
{
    Camera& m_parent;

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_viewProjectionMatrix;

    glm::vec3 m_position = glm::vec3(0.0f);
    float m_rotation = 0.0f;

    PImpl(Camera& parent, float left, float right, float bottom, float top);
    ~PImpl();
    
    void updateViewProjection();
};

Camera::PImpl::PImpl(Camera& parent, float left, float right, float bottom, float top)
    : m_parent(parent),
      m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
      m_viewMatrix(glm::mat4(1.0f))
{
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

Camera::PImpl::~PImpl()
{

}

void Camera::PImpl::updateViewProjection()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

    m_viewMatrix = glm::inverse(transform);
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;        
}

Camera::Camera(float left, float right, float bottom, float top)
    : m_pImpl(std::make_unique<PImpl>(*this, left, right, bottom, top))
{
    
}

Camera::~Camera()
{

}

void Camera::setProjection(float left, float right, float bottom, float top)
{
    m_pImpl->m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, -1.0f);
    m_pImpl->m_viewProjectionMatrix = m_pImpl->m_projectionMatrix * m_pImpl->m_viewMatrix;
}

void Camera::setPosition(const glm::vec3& position)
{
    m_pImpl->m_position = position;
    m_pImpl->updateViewProjection();
}

const glm::vec3& Camera::position() const
{
    return m_pImpl->m_position;
}

const glm::mat4& Camera::projectionMatrix() const
{
    return m_pImpl->m_projectionMatrix;
}

const glm::mat4& Camera::viewMatrix() const
{
    return m_pImpl->m_viewMatrix;
}

const glm::mat4& Camera::viewProjectionMatrix() const
{
    return m_pImpl->m_viewProjectionMatrix;
}
