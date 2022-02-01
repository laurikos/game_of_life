#pragma once
#include "Events.h"

#include <memory>
#include <string>

struct GLFWwindow;

/*
 * Window is main window for the application.
 * Polls the events from GLFWwindow and is queried by application for events.
 *
 * Also initializes GLFW and glad. And in OpenGLContext.cpp we initialize
 * the OpenGL version > 3.x with glad loader.
 */
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
    std::pair<Event, bool> fetchWindowEvent(); // use std::reference_wrapper ?    
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
