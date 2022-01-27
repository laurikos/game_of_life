#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
 * OpenGL context for Window class
 */
class OpenGLContext
{
public:
    OpenGLContext(GLFWwindow* window);
    ~OpenGLContext();

    /*
     * Load 3.x > version API of OpenGL
     * via glad
     */
    void init();
    void swapBuffers();

private:
    GLFWwindow* m_windowHandle;
};
