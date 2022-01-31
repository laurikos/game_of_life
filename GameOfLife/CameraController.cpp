#include "CameraController.h"
#include "Camera.h"

#include <GLFW/glfw3.h>

struct CameraController::PImpl {
    CameraController& m_parent;

    float m_aspectRatio;
    float m_zoomLevel;
    Camera m_camera;

    glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_cameraMoveSpd = 5.0f;

    bool m_moveLeft = false;
    bool m_moveRight = false;
    bool m_moveUp = false;
    bool m_moveDown = false;
    
    PImpl(CameraController& parent, float aspectRatio, float zoomLevel);
    ~PImpl();

    void onUpdate(float deltaTime);
    void onEvent(Event& e);

    void onWindowResized(Event& e);
    void onMouseScrolled(Event& e);
};

CameraController::PImpl::PImpl(CameraController& parent,
                               float aspectRatio, float zoomLevel)
    : m_parent(parent),
      m_aspectRatio(aspectRatio),
      m_zoomLevel(zoomLevel),
      m_camera(
          ((-1)* m_aspectRatio) * m_zoomLevel,
          m_aspectRatio * m_zoomLevel,
          (-1) * m_zoomLevel,
          m_zoomLevel)
{
#ifdef Debug
    printf("[CameraController] Initial zoom level set to: %.2f\n", m_zoomLevel);
#endif
}

CameraController::PImpl::~PImpl()
{

}

CameraController::CameraController(float aspectRatio, float zoomLevel)
    : m_pImpl(std::make_unique<PImpl>(*this, aspectRatio, zoomLevel))
{

}

CameraController::~CameraController()
{

}

void CameraController::PImpl::onMouseScrolled(Event& e)
{
    m_zoomLevel -= e.mouseValues().second * 0.25f;
    m_zoomLevel = std::max(m_zoomLevel, 0.5f);

    m_camera.setProjection(
        ((-1) * m_aspectRatio) * m_zoomLevel,
        m_aspectRatio * m_zoomLevel,
        (-1) * m_zoomLevel, m_zoomLevel);
}

void CameraController::PImpl::onWindowResized(Event& e)
{
    auto data = e.windowValues();
    m_aspectRatio = static_cast<float>(data.first) / static_cast<float>(data.second);
    m_camera.setProjection(
        ((-1) * m_aspectRatio) * m_zoomLevel,
        m_aspectRatio * m_zoomLevel,
        (-1) * m_zoomLevel, m_zoomLevel);
}

void CameraController::PImpl::onUpdate(float deltaTime)
{
    // Had to rotate came because rendering was off be 90 degress.
    // and I could not find why in decent amount of time
    // Commented lines are as they should be.
    if (m_moveLeft) {
        // m_cameraPosition.x -= m_cameraMoveSpd * deltaTime;
        m_cameraPosition.y -= m_cameraMoveSpd * deltaTime;
    }
    if (m_moveRight)
    {
        // m_cameraPosition.x += m_cameraMoveSpd * deltaTime;
        m_cameraPosition.y += m_cameraMoveSpd * deltaTime;
    }

    if (m_moveUp)
    {
        // m_cameraPosition.y += m_cameraMoveSpd * deltaTime;
        m_cameraPosition.x -= m_cameraMoveSpd * deltaTime;
    }
    if (m_moveDown) {
        m_cameraPosition.x += m_cameraMoveSpd * deltaTime;
        // m_cameraPosition.y -= m_cameraMoveSpd * deltaTime;
    }

    m_camera.setPosition(m_cameraPosition);
    m_cameraMoveSpd = m_zoomLevel;
}

void CameraController::PImpl::onEvent(Event& e)
{
    if (e.type() == EventType::MouseScrolled) {
        onMouseScrolled(e);
    }

    if (e.type() == EventType::WindowResize) {
        onWindowResized(e);
    }
    
    if (e.type() == EventType::KeyPressed) {
        
        if (e.keyValue() == GLFW_KEY_DOWN) {
            m_moveDown = true;
        }
        else if (e.keyValue() == GLFW_KEY_UP) {
            m_moveUp = true;
        }

        if (e.keyValue() == GLFW_KEY_RIGHT) {
            m_moveRight = true;
        }
        if (e.keyValue() == GLFW_KEY_LEFT) {
            m_moveLeft = true;
        }
    }

    if (e.type() == EventType::KeyReleased) {
        if (e.keyValue() == GLFW_KEY_DOWN) {
            m_moveDown = false;
        }
        else if (e.keyValue() == GLFW_KEY_UP) {
            m_moveUp = false;
        }

        if (e.keyValue() == GLFW_KEY_RIGHT) {
            m_moveRight = false;
        }
        if (e.keyValue() == GLFW_KEY_LEFT) {
            m_moveLeft = false;
        }
    }
}

Camera& CameraController::getCamera()
{
    return m_pImpl->m_camera;
}

const Camera& CameraController::getCamera() const
{
    return m_pImpl->m_camera;
}

void CameraController::onUpdate(float deltaTime)
{
    m_pImpl->onUpdate(deltaTime);
}

void CameraController::onEvent(Event& e)
{
    m_pImpl->onEvent(e);
}

void CameraController::setZoomLevel(float level)
{
    m_pImpl->m_zoomLevel = level;
}

float CameraController::zoomLevel()
{
    return m_pImpl->m_zoomLevel;
}
