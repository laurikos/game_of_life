#pragma once

class Window;
class LayerManager;

#include <memory>

class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    bool m_isRunning;
    float m_lastTime = 0.0f;

    std::shared_ptr<Window> m_window;
    std::unique_ptr<LayerManager> m_layerManager;
};
