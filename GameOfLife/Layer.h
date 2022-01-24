#pragma once

#include <memory>

class LayerManager;

class Layer
{
public:
    Layer(LayerManager* manager = nullptr) {}
    virtual ~Layer() = default;

    virtual void init() {};
    virtual void onUpdate() {};
    virtual void onRender() {};

    virtual bool isImGuiLayer() { return false; }
};
