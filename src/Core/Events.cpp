#include "Events.h"

Event::Event(EventType type, int xInt, int yInt, int zInt, float xFloat, float yFloat)
{
        m_type = type;
        
        if (type == EventType::WindowClose) {
            // TODO: handle if needed.
        }
        else if (type == EventType::WindowResize) {
            m_width = xInt;
            m_height = yInt;
        }
        else if (type == EventType::KeyPressed) {
            m_key = xInt;
        }
        else if (type == EventType::KeyReleased) {
            m_key = xInt;
        }
        else if (type == EventType::MouseMoved || type == EventType::MouseScrolled) {
            m_mouseX = xFloat;
            m_mouseY = yFloat;
        } else if (type == EventType::MouseClicked) {
            m_key = xInt;
        }
}

Event::~Event()
{

}

std::pair<int, int> Event::windowValues()
{
    return std::make_pair<int, int>(std::move(m_width), std::move(m_height));
}

int Event::keyValue() const
{
    return m_key;
}

std::pair<float, float> Event::mouseValues()
{
    return std::make_pair<float, float>(std::move(m_mouseX), std::move(m_mouseY));
}

const EventType& Event::type() const
{
    return m_type;
}
