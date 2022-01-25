#pragma once

#include "../Layer.h"
#include <memory>

class LayerManager;

class SandboxLayer : public Layer
{
public:
    SandboxLayer(LayerManager* manager);
    virtual ~SandboxLayer();

    virtual void init() override;
    virtual void onUpdate(float deltaTime) override;
    virtual void onRender() override;

    virtual bool isImGuiLayer() override { return false; }
    
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
