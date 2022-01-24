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
    virtual void onUpdate() override;
    virtual void onRender() override;

    inline bool isImGuiLayer() override { return true; }
    
    void startSceneUI();
    void endSceneUI();
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
