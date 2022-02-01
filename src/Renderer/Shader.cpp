#include "Shader.h"

#include <assert.h>
#include <vector>
#include <stdio.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

struct Shader::PImpl {
    Shader& m_parent;
    std::uint32_t m_shaderProgram;

    PImpl(Shader& parent, const std::string& vertexSrc,
          const std::string& fragmentSrc);
    ~PImpl();
};

Shader::PImpl::PImpl(Shader& parent, const std::string& vertexSrc,
                     const std::string& fragmentSrc)
    : m_parent(parent)
{
    /*
     * **********************************************************
     * Code below is copied from 
     * https://www.khronos.org/opengl/wiki/Shader_Compilation
     * **********************************************************
     */
    
    // Read our shaders into the appropriate buffers
    const char*  vertexSource = vertexSrc.c_str();
    const char* fragmentSource = fragmentSrc.c_str();

        // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = (const GLchar*)vertexSource;
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[ 0 ]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        fprintf(stderr, "Vertex shader compile errored: %s\n", infoLog.data());
        assert(false);
        // In this simple program, we'll just leave
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar*)fragmentSource;
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength,
                           &infoLog[ 0 ]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        fprintf(stderr, "Fragment shader compile errored: %s\n", infoLog.data());
        assert(false);

        // In this simple program, we'll just leave
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_shaderProgram = glCreateProgram(); 
    GLuint program = m_shaderProgram;

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[ 0 ]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Use the infoLog as you see fit.
        fprintf(stderr, "Linking shaders errored: %s\n", infoLog.data());
        assert(false);

        // In this simple program, we'll just leave
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

Shader::PImpl::~PImpl()
{
    glDeleteProgram(m_shaderProgram);
}

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_pImpl(std::make_unique<PImpl>(*this, vertexSrc, fragmentSrc))
{

}

Shader::~Shader()
{
    
}

void Shader::bind() const
{
    glUseProgram(m_pImpl->m_shaderProgram);
}

void Shader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_pImpl->m_shaderProgram, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_pImpl->m_shaderProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uploadUniformInt(const std::string& name, std::int32_t value)
{
    GLint location = glGetUniformLocation(m_pImpl->m_shaderProgram, name.c_str());
    glUniform1i(location, value);
}

void Shader::uploadUniformIntArray(const std::string& name,
                                         std::int32_t* values, std::uint32_t count)
{
    GLint location = glGetUniformLocation(m_pImpl->m_shaderProgram, name.c_str());
    glUniform1iv(location, count, values);
}
    
void Shader::uploadUniformFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(m_pImpl->m_shaderProgram, name.c_str());
    glUniform1f(location, value);
}

void Shader::uploadUniformFloat2(const std::string& name, const glm::vec2& vec)
{
    GLint location = glGetUniformLocation(m_pImpl->m_shaderProgram, name.c_str());
    glUniform2f(location, vec.x, vec.y);
}

void Shader::uploadUniformFloat3(const std::string& name, const glm::vec3& vec)
{
    GLint location = glGetUniformLocation(m_pImpl->m_shaderProgram, name.c_str());
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::uploadUniformFloat4(const std::string& name, const glm::vec4& vec)
{
    GLint location = glGetUniformLocation(m_pImpl->m_shaderProgram, name.c_str());
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}
