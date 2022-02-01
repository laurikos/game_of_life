#pragma once

#include "../Core/Events.h"
#include "../Core/Layer.h"
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
    virtual void onEvent(Event& e) override;
    
    virtual bool isImGuiLayer() override { return true; }
    
    virtual void startSceneUI() override;
    virtual void endSceneUI() override;
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
