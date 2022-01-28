#pragma once

#include <cinttypes>

struct FBSettings
{
    std::uint32_t width;
    std::uint32_t height;

    bool bind = false;
};

class Framebuffer
{
public:
    Framebuffer(const FBSettings& settings);
    ~Framebuffer();

    const FBSettings& settings() const;
private:
    FBSettings m_settings;
};
