#pragma once

#include "Events.h"

#include <memory>

class LayerManager;

/*
 * Layer is a class that the actual Game that we run must implement.
 *
 * "onRender()" should probably be named to, like onUIRender()
 * since we only use it for rendering ImGui windows.
 * onUpdate() actually does the OpenGL calls to render all the game objects.
 */
class Layer
{
public:
    Layer(LayerManager* manager = nullptr) {}
    virtual ~Layer() = default;

    virtual void init() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onRender() {}
    virtual void onEvent(Event& event) {}
       
    virtual bool isImGuiLayer() { return false; }
    virtual void startSceneUI() {}
    virtual void endSceneUI() {}
};
