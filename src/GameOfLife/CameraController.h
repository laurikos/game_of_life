#pragma once

#include "../Core/Events.h"

#include <memory>

class Camera;

class CameraController
{
public:
    CameraController(float aspectRatio, float zoomLevel);
    ~CameraController();

    Camera& getCamera();
    const Camera& getCamera() const;
    
    void onUpdate(float deltaTime);
    void onEvent(Event& e);
    
    float zoomLevel();
    void setZoomLevel(float level);
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};

