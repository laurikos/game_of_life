#pragma once

#include "Events.h"

class Window;
class LayerManager;
class Layer;

#include <memory>

class Application
{
public:
    Application();
    ~Application();

    void run();

    void onEvent(Event& e);
    
private:
    bool m_isRunning;
    float m_lastTime = 0.0f;

    Layer* m_gameLayer = nullptr;
    
    std::shared_ptr<Window> m_window;
    std::unique_ptr<LayerManager> m_layerManager;
};
