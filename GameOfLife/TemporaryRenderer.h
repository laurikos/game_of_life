#pragma once

#include "Camera.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

/*
 *
 * This should represent something that the final product will be
 * 
 */
struct VBO
{
    std::uint32_t m_programID;
    std::unordered_map<std::string, std::pair<std::uint32_t, std::uint32_t>> m_data;
    std::uint32_t m_stride;
    
    VBO(std::uint32_t size)
    {
        glCreateBuffers(1, &m_programID);
        glBindBuffer(GL_ARRAY_BUFFER, m_programID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }
    
    VBO(float* vertices, std::uint32_t size)
    {
        glCreateBuffers(1, &m_programID);
        glBindBuffer(GL_ARRAY_BUFFER, m_programID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    ~VBO()
    {
        glDeleteBuffers(1, &m_programID);
    }

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_programID);
    }

    void unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);        
    }

    void setBuffer(const std::string& shaderAttrib, std::uint32_t size)
    {
        m_data.insert({ shaderAttrib, { size, 0 } });
    }
    
    void uploadBuffers()
    {
        std::uint32_t offset = 0;
        m_stride = 0;
        for (auto& [ attrib, bufData ] : m_data) {
            bufData.second = offset;
            offset += bufData.first;
            m_stride += bufData.first;
        }        
    }

    const std::unordered_map<std::string, std::pair<std::uint32_t, std::uint32_t>>&
    getBufferData() const
    {
        return m_data;
    }

    std::uint32_t getStride() const { return m_stride; }

    void setData(const void* data, std::uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_programID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
};

struct IBO
{
    std::uint32_t m_count;
    std::uint32_t m_programID;
    
    IBO(std::uint32_t* indices, std::uint32_t count)
        : m_count(count)
    {
        glCreateBuffers(1, &m_programID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_programID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (count * sizeof(std::uint32_t)),
                     indices, GL_STATIC_DRAW);
    }
    
    ~IBO()
    {
        glDeleteBuffers(1, &m_programID);
    }

    void bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_programID);
    }

    std::uint32_t indicesCount() const { return m_count; }
};

struct VAO
{
    std::uint32_t m_programID;
    std::vector<std::shared_ptr<VBO>> m_vertexBuffers;
    std::shared_ptr<IBO> m_indexBuffer;
    
    VAO()
    {
        glCreateVertexArrays(1, &m_programID);
    }
    
    ~VAO()
    {
        glDeleteVertexArrays(1, &m_programID);
    }

    void bind() const
    {
        glBindVertexArray(m_programID);
    }

    void unbind() const
    {
        glBindVertexArray(0);
    }

    void uploadVBO(const std::shared_ptr<VBO>& vbo)
    {
        glBindVertexArray(m_programID);
        vbo->bind();

        std::uint32_t bufIdx = 0;
        const auto& bufData = vbo->getBufferData();
        
        // TODO: This needs some refactoring.. 
        // too much hardcoded stuff but let's see how it works out.
        for (const auto& [ attrib, data ] : bufData) {
            
            glEnableVertexAttribArray(bufIdx);
            glVertexAttribPointer(bufIdx,
                                  3, /* count (i.e. glm::vec3 has count 3 and so on */
                                  GL_FLOAT, /* gl type hard coded for now...*/
                                  GL_FALSE, /* normalized -- hard coded for now..*/
                                  vbo->getStride(),
                                  reinterpret_cast<const void*>(data.second));
            bufIdx++;
        }

        m_vertexBuffers.push_back(vbo);
    }

    void setIBO(const std::shared_ptr<IBO>& ibo)
    {
        glBindVertexArray(m_programID);
        ibo->bind();

        m_indexBuffer = ibo;
    }

    const std::vector<std::shared_ptr<VBO>>& getVBOs() const
    {
        return m_vertexBuffers;
    }

    const std::shared_ptr<IBO>& getIBO() const
    {
        return m_indexBuffer;
    }
};

class TemporaryRenderer
{
public:
    static void init();
    static void close();    

    static void handleResize(std::uint32_t width, std::uint32_t height);
    static void setViewport(std::uint32_t x, std::uint32_t y,
                            std::uint32_t width, std::uint32_t height);

    static void setClearColor(const glm::vec4& color = { 0.1f, 0.1f, 0.1f, 1.0f });
    static void clear();
    
    static void start(Camera& camera);
    static void end();

    static void drawQuad(const glm::vec3& position, const glm::vec2 size,
                         const glm::vec4& color);

    static void execute(const std::shared_ptr<VAO>& vao,
                        std::uint32_t indexCount);

private:

};
