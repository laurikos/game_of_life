#include "Window.h"
#include "../Renderer/OpenGLContext.h"

#include <stdio.h>
#include <assert.h>
#include <queue>
#include <functional>
#include <tuple>

static void glfwErrorCallback(int errCode, const char* description)
{
    fprintf(stderr, "GLFW Error[%d]: %s\n", errCode, description);
}


struct Window::PImpl {
    Window& m_parent;
    
    std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> m_window;
    std::unique_ptr<OpenGLContext> m_windowContext;

    struct WindowConfigs {
        std::string title;
        std::uint32_t width;
        std::uint32_t height;
        bool vSync;
        std::function<void(Event&)> eventCallback;
    } m_savedWindowCfg;

    std::unique_ptr<std::queue<Event>> m_events;
    
    PImpl(Window& parent);

    void init(const std::string& title,
              std::uint32_t width, std::uint32_t height);
    void eventCallBackFunc(Event& e);
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

    m_events = std::make_unique<std::queue<Event>>();
    
    m_savedWindowCfg.eventCallback =
        std::bind(&PImpl::eventCallBackFunc, this, std::placeholders::_1);

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

    m_windowContext = std::make_unique<OpenGLContext>(m_window.get());
    m_windowContext->init();

    // Was for glad2:
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
    glfwSetWindowSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height) {
        Event e(EventType::WindowResize, width, height);
        auto& winUsrPtr = *static_cast<WindowConfigs*>(glfwGetWindowUserPointer(window));
        winUsrPtr.eventCallback(e);        
    });

    glfwSetWindowCloseCallback(m_window.get(), [](GLFWwindow* window){
        auto& winUsrPtr = *static_cast<WindowConfigs*>(glfwGetWindowUserPointer(window));
        Event e(EventType::WindowClose);
        winUsrPtr.eventCallback(e);
    });

    glfwSetKeyCallback(m_window.get(), [](GLFWwindow* window, int key,
                                          int scancode, int action, int mods) {
        auto& winUsrPtr =
            *static_cast<WindowConfigs*>(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                Event e(EventType::KeyPressed, key);
                winUsrPtr.eventCallback(e);
                break;
            }
            case GLFW_RELEASE: {
                Event e(EventType::KeyReleased, key);
                winUsrPtr.eventCallback(e);
                break;
            }
            case GLFW_REPEAT: {
                break;
            }
        }
    });

    glfwSetScrollCallback(m_window.get(), [](GLFWwindow* window, double xOff, double yOff){
        auto& winUsrPtr =
            *static_cast<WindowConfigs*>(glfwGetWindowUserPointer(window));

        Event e(EventType::MouseScrolled, 0, 0, 0,
                static_cast<float>(xOff), static_cast<float>(yOff));
        winUsrPtr.eventCallback(e);
    });

    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPos, double yPos){
        auto& winUsrPtr =
            *static_cast<WindowConfigs*>(glfwGetWindowUserPointer(window));

        Event e(EventType::MouseMoved, 0, 0, 0,
                static_cast<float>(xPos), static_cast<float>(yPos));
        winUsrPtr.eventCallback(e);
    });

    glfwSetMouseButtonCallback(
        m_window.get(),
        [](GLFWwindow* window, int button, int action, int modes) {
            auto& winUsrPtr =
                *static_cast<WindowConfigs*>(glfwGetWindowUserPointer(window));

            if (action == GLFW_PRESS) {
                Event e(EventType::MouseClicked, button);
                winUsrPtr.eventCallback(e);
            }
        });
}

void Window::PImpl::eventCallBackFunc(Event& e)
{
    m_events->push(e);
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

std::pair<Event, bool> Window::fetchWindowEvent()
{
    if (!m_pImpl->m_events->size()) {
        return std::make_pair<Event, bool>(EventType::NEvents, true);
    }

    Event e = m_pImpl->m_events->front();
    m_pImpl->m_events->pop();
    return std::make_pair<Event, bool>(std::move(e), false);
}
