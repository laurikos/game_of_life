#include "OpenGLObjects.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

VBO::VBO(std::uint32_t size)
{
    glCreateBuffers(1, &m_programID);
    glBindBuffer(GL_ARRAY_BUFFER, m_programID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}
    
VBO::VBO(float* vertices, std::uint32_t size)
{
    glCreateBuffers(1, &m_programID);
    glBindBuffer(GL_ARRAY_BUFFER, m_programID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &m_programID);
}

void VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_programID);
}

void VBO::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);        
}

void VBO::setBuffer(const std::string& shaderAttrib, std::uint32_t size)
{
    m_data.insert({ shaderAttrib, { size, 0 } });
}
    
void VBO::uploadBuffers()
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
VBO::getBufferData() const
{
    return m_data;
}

void VBO::setData(const void* data, std::uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_programID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

IBO::IBO(std::uint32_t* indices, std::uint32_t count)
    : m_count(count)
{
    glCreateBuffers(1, &m_programID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_programID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (count * sizeof(std::uint32_t)),
                 indices, GL_STATIC_DRAW);
}
    
IBO::~IBO()
{
    glDeleteBuffers(1, &m_programID);
}

void IBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_programID);
}

VAO::VAO()
{
    glCreateVertexArrays(1, &m_programID);
}
    
VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_programID);
}

void VAO::bind() const
{
    glBindVertexArray(m_programID);
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}

void VAO::uploadVBO(const std::shared_ptr<VBO>& vbo)
{
    glBindVertexArray(m_programID);
    vbo->bind();

    std::uint32_t bufIdx = 0;
    const auto& bufData = vbo->getBufferData();
        
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

void VAO::setIBO(const std::shared_ptr<IBO>& ibo)
{
    glBindVertexArray(m_programID);
    ibo->bind();

    m_indexBuffer = ibo;
}

const std::vector<std::shared_ptr<VBO>>& VAO::getVBOs() const
{
    return m_vertexBuffers;
}

const std::shared_ptr<IBO>& VAO::getIBO() const
{
    return m_indexBuffer;
}
