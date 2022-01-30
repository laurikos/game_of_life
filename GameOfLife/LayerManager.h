#pragma once

#include "Layer.h"
#include "Events.h"

#include <vector>
#include <memory>

class Window;
struct GLFWwindow;

class LayerManager
{
public:
    LayerManager(const std::shared_ptr<Window>& window = nullptr);
    ~LayerManager();

    std::uint32_t getWindowHeight() const;
    std::uint32_t getWindowWidth() const;

    GLFWwindow* getWindow() const;
    
    Layer* insertNewLayer(std::unique_ptr<Layer> layer);
    void updateLayers(float deltaTime);
    void renderLayers();
    
    void sendEventToLayer(Layer* layer, Event& event);

    Layer* settingUpDone(const std::vector<std::vector<std::uint32_t>>& initState);
    bool isGameLayerInitialized() const;
    Layer* gameLayer();
    
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
