/*
 * Should be OpenGLContext
 * Loads the glad as version > 3.x OpenGL API handler.
 */

#include "WindowContext.h"
#include <stdio.h>
#include <assert.h>
#include <cstdlib>

WindowContext::WindowContext(GLFWwindow* window)
    : m_windowHandle(window)
{

}

WindowContext::~WindowContext()
{
    
}

void WindowContext::init()
{
    assert(m_windowHandle && "failed to fetch GLFWwindow handle");
    glfwMakeContextCurrent(m_windowHandle);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize OpenGL context\n");
        std::exit(1);
    }

    printf("OpenGL Vendor:    %s\n", glGetString(GL_VENDOR));
    printf("OpenGL Renderer:  %s\n", glGetString(GL_RENDERER));
    printf("OpenGL Version:   %s\n", glGetString(GL_VERSION));
}

void WindowContext::swapBuffers()
{
    glfwSwapBuffers(m_windowHandle);
}
