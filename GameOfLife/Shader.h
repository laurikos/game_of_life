#pragma once

#include <string>
#include <memory>

class Shader
{
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void bind() const;
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
