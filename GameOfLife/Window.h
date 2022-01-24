#pragma once

#include <memory>
#include <string>

struct GLFWwindow;

class Window
{
public:
    Window(const std::string& title = "GameOfLife",
       std::uint32_t width = 1280, std::uint32_t height = 720);
    ~Window();

    void onUpdate();
    
    void setVSync(bool enabled);
    bool isVSync() const;
    std::uint32_t height() const;
    std::uint32_t width() const; 
    
    GLFWwindow* window() const;
    
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
