#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>


/*
 * VertexBufferObject
 * set up vertices and derive the offset and stride for vertex array to
 * build up the batched render call to OpenGL
 */
struct VBO
{
    std::uint32_t m_programID;
    std::unordered_map<std::string, std::pair<std::uint32_t, std::uint32_t>> m_data;
    std::uint32_t m_stride;
    
    VBO(std::uint32_t size);
    VBO(float* vertices, std::uint32_t size);

    ~VBO();

    void bind() const;
    void unbind() const;

    void setBuffer(const std::string& shaderAttrib, std::uint32_t size);
    void uploadBuffers();
    void setData(const void* data, std::uint32_t size);
    
    const std::unordered_map<std::string, std::pair<std::uint32_t, std::uint32_t>>&
        getBufferData() const;
    
    inline std::uint32_t getStride() const { return m_stride; }
};

/*
 * IndexBufferObject
 * Holds the indices data for quads, that we draw.
 */
struct IBO
{
    std::uint32_t m_count;
    std::uint32_t m_programID;
    
    IBO(std::uint32_t* indices, std::uint32_t count);
    ~IBO();
    
    void bind() const;

    inline std::uint32_t indicesCount() const { return m_count; }
};

/*
 * VertexArrayObject
 * Set up the VBOs and IBO for the quad drawing.
 */
struct VAO
{
    std::uint32_t m_programID;
    std::vector<std::shared_ptr<VBO>> m_vertexBuffers;
    std::shared_ptr<IBO> m_indexBuffer;
    
    VAO();
    ~VAO();

    void bind() const;
    void unbind() const;

    void uploadVBO(const std::shared_ptr<VBO>& vbo);
    void setIBO(const std::shared_ptr<IBO>& ibo);

    const std::vector<std::shared_ptr<VBO>>& getVBOs() const;
    const std::shared_ptr<IBO>& getIBO() const;
};
