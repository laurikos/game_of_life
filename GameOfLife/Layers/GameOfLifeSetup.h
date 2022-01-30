#pragma once

#include "../Layer.h"
#include <memory>

class LayerManager;

class GameOfLifeSetup : public Layer
{
public:
    GameOfLifeSetup(LayerManager* manager);
    virtual ~GameOfLifeSetup();

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
