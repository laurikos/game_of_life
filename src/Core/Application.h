#pragma once

#include "Events.h"

class Window;
class LayerManager;
class Layer;

#include <memory>

/*
 * Application creates and initializes everything that we need for a game
 * Also handles the game loop and polls events from Window which then forwards to
 * layers, if needed.
 */
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
    Layer* m_gameSetupLayer = nullptr;
    
    std::shared_ptr<Window> m_window;
    std::unique_ptr<LayerManager> m_layerManager;
};
