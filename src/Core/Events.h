#pragma once

#include <utility>

enum class EventType {
    WindowClose = 0,
    WindowResize,
    KeyPressed,
    KeyReleased,
    MouseMoved,
    MouseScrolled,
    MouseClicked,
    NEvents
};


/*
 * Event is class that holds all the event types we propagate to layers.
 * Not my best planned class ever but handles the job well enough for now.
 */
class Event
{
public:
    Event(EventType type, int xInt = 0, int yInt = 0, int zInt = 0,
          float xFloat = 0.0f, float yFloat = 0.0f);
    ~Event();

    const EventType& type() const;
    std::pair<int, int> windowValues();
    int keyValue() const;
    std::pair<float, float> mouseValues();
    
private:
    EventType m_type;
    
    int m_width = 0;
    int m_height = 0;
    int m_key = 0;
    float m_mouseY = 0.0f;
    float m_mouseX = 0.0f;
};

