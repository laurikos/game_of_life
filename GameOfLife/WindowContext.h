#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowContext
{
public:
    WindowContext(GLFWwindow* window);
    ~WindowContext();

    void init();
    void swapBuffers();

private:
    GLFWwindow* m_windowHandle;
};
