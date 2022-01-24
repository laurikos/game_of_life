#include "Window.h"
#include "WindowContext.h"
#include <functional>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <assert.h>

static void glfwErrorCallback(int errCode, const char* description)
{
    fprintf(stderr, "GLFW Error[%d]: %s\n", errCode, description);
}


struct Window::PImpl {
    Window& m_parent;
    
    std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> m_window;
    std::unique_ptr<WindowContext> m_windowContext;

    struct WindowData {
        std::string title;
        std::uint32_t width;
        std::uint32_t height;
        bool vSync;
    } m_savedWindowCfg;
    
    PImpl(Window& parent);

    void init(const std::string& title,
              std::uint32_t width, std::uint32_t height);
};

Window::PImpl::PImpl(Window& parent)
    : m_parent(parent)
{

}

void Window::PImpl::init(const std::string& title,
                         std::uint32_t width, std::uint32_t height)
{
    m_savedWindowCfg.height = height;
    m_savedWindowCfg.width = width;
    m_savedWindowCfg.title = title;
    m_savedWindowCfg.vSync = true;

    int success = glfwInit();
    if (!success) {
        fprintf(stderr, "Could not initialize GFLW");
        std::exit(1);
    }

    glfwSetErrorCallback(glfwErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    printf("Creating window \"%s\" (%d x %d)\n",
           m_savedWindowCfg.title.c_str(), m_savedWindowCfg.height,
           m_savedWindowCfg.width);

    m_window = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>>(
        glfwCreateWindow(static_cast<std::int32_t>(m_savedWindowCfg.width),
                         static_cast<std::int32_t>(m_savedWindowCfg.height),
                         m_savedWindowCfg.title.c_str(),
                         nullptr, nullptr),        
        [](GLFWwindow* window) {
            glfwDestroyWindow(window);
        });

    m_windowContext = std::make_unique<WindowContext>(m_window.get());
    m_windowContext->init();

    // bool err = gladLoadGL(glfwGetProcAddress) == 0;
    bool err = gladLoadGL() == 0;
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        std::exit(1);
    }

    glfwSetWindowUserPointer(m_window.get(), &m_savedWindowCfg);
    m_parent.setVSync(true);

    /*
     * glfw window, key, cursor callbacks:
     */
}

Window::Window(const std::string& title,
               std::uint32_t width, std::uint32_t height)
    : m_pImpl(std::make_unique<PImpl>(*this))
{
    m_pImpl->init(title, width, height);
}

Window::~Window()
{
    m_pImpl->m_window.reset();
    glfwTerminate();
}

void Window::onUpdate()
{
    glfwPollEvents();
    m_pImpl->m_windowContext->swapBuffers();
}

void Window::setVSync(bool enabled)
{
    if (enabled) {
        glfwSwapInterval(1);        
    } else {
        glfwSwapInterval(0);
    }
    m_pImpl->m_savedWindowCfg.vSync = enabled;
}

bool Window::isVSync() const
{
    return m_pImpl->m_savedWindowCfg.vSync;
}

std::uint32_t Window::height() const
{
    return m_pImpl->m_savedWindowCfg.height;
}

std::uint32_t Window::width() const
{
    return m_pImpl->m_savedWindowCfg.width;
}

GLFWwindow* Window::window() const
{
    assert(m_pImpl->m_window && "Did not find GLFWwindow object");
    return m_pImpl->m_window.get();
}
