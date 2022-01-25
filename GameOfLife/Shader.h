#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void bind() const;

    // probably just need position and color so float3 and float4 maybe
    // but just put them all here...
    void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    void uploadUniformInt(const std::string& name, std::int32_t value);
    void uploadUniformIntArray(const std::string& name,
                               std::int32_t* values, std::uint32_t count);

    
    void uploadUniformFloat(const std::string& name, float value);
    void uploadUniformFloat2(const std::string& name, const glm::vec2& vec);
    void uploadUniformFloat3(const std::string& name, const glm::vec3& vec);
    void uploadUniformFloat4(const std::string& name, const glm::vec4& vec);
    
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pImpl;
};
