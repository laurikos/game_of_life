#pragma once

#include "../Layer.h"
#include <memory>

class LayerManager;

enum class GameMode { Auto, Manual };

class GameOfLife : public Layer
{
public:
    GameOfLife(LayerManager* manager);
    virtual ~GameOfLife();

    virtual void init() override;
    virtual void onUpdate(float deltaTime) override;
    virtual void onRender() override;
    virtual void onEvent(Event& event) override;
    
    virtual bool isImGuiLayer() override { return false; }
    
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
