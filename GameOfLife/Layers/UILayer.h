#pragma once

#include "../Layer.h"
#include <memory>

class LayerManager;

class UILayer : public Layer
{
public:
    UILayer(LayerManager* manager);
    virtual ~UILayer();

    virtual void init() override;
    virtual void onUpdate(float deltaTime) override;
    virtual void onRender() override;

    virtual bool isImGuiLayer() override { return true; }
    
    void startSceneUI();
    void endSceneUI();
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
