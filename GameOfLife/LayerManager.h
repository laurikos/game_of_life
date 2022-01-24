#pragma once

#include "Layer.h"

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

    void insertNewLayer(std::unique_ptr<Layer> layer);
    void updateLayers();
    void renderLayers();
    
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
