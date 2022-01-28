#include "Framebuffer.h"

Framebuffer::Framebuffer(const FBSettings& settings)
    : m_settings(settings)
{
}

Framebuffer::~Framebuffer()
{

}

const FBSettings& Framebuffer::settings() const
{
    return m_settings;
}
