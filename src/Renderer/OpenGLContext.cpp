#include "OpenGLContext.h"
#include <stdio.h>
#include <assert.h>
#include <cstdlib>

OpenGLContext::OpenGLContext(GLFWwindow* window)
    : m_windowHandle(window)
{

}

OpenGLContext::~OpenGLContext()
{
    
}

void OpenGLContext::init()
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
    printf("\n\n");
}

void OpenGLContext::swapBuffers()
{
    glfwSwapBuffers(m_windowHandle);
}
