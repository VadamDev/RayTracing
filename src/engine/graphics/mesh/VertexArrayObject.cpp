#include "VertexArrayObject.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace engine
{
    VertexArrayObject::~VertexArrayObject()
    {
        if (vao == 0)
            return;

        bind();
        disableVertexAttribArrays();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        for (const auto& [bufferId, _] : buffersMap)
            glDeleteBuffers(1, &bufferId);

        if (ebo != 0)
            glDeleteBuffers(1, &ebo);

        unbind();
        glDeleteVertexArrays(1, &vao);

        buffersMap.clear();
        vao = 0;
    }

    /*
     * Create
     */

    void VertexArrayObject::create(const int verticesCount, const RenderType renderType)
    {
        this->renderType = renderType;
        this->verticesCount = verticesCount;

        glGenVertexArrays(1, &vao);
    }

    /*
     * Bind / Unbind
     */

    void VertexArrayObject::bind() const
    {
        glBindVertexArray(vao);
    }

    void VertexArrayObject::unbind()
    {
        glBindVertexArray(0);
    }

    /*
     * Buffers
     */

    /**
     * Generate a new element (indices) buffer, and link it to the VAO
     *
     * @param size Specifies the size in bytes of the buffer object's new data store.
     * @param data Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
     */
    void VertexArrayObject::genElementBuffer(const signed long long int size, const void *data)
    {
        if (bReady)
            throw std::runtime_error("VAO was set as ready, you can only modify existing buffers");

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    /**
     * Generate a new buffer and link it to the VAO
     *
     * @param size Specifies the size in bytes of the buffer object's new data store.
     * @param data Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
     * @param usage Specifies the expected usage pattern of the data store. The symbolic constant must be GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
     * @param vaSize Specifies the number of components per generic vertex attribute.
     * @param vaType Specifies the data type of each component in the array. The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted
     * @return The generated buffer id
     */
    unsigned int VertexArrayObject::genBuffer(const signed long long int size, const void *data, const unsigned int usage, const int vaSize, const unsigned int vaType)
    {
        if (bReady)
            throw std::runtime_error("VAO was set as ready, you can only modify existing buffers");

        unsigned int bufferId = 0;
        glGenBuffers(1, &bufferId);

        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);

        glEnableVertexAttribArray(vertexAttributeIndex);
        glVertexAttribPointer(vertexAttributeIndex, vaSize, vaType, false, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        buffersMap[bufferId] = vertexAttributeIndex;
        vertexAttributeIndex++;

        return bufferId;
    }


    /*
     * Attrib Pointer
     */

    void VertexArrayObject::enableVertexAttribArrays() const
    {
        for(const auto& [_, attribPointer] : buffersMap)
            glEnableVertexAttribArray(attribPointer);
    }

    void VertexArrayObject::disableVertexAttribArrays() const
    {
        for(const auto& [_, attribPointer] : buffersMap)
            glDisableVertexAttribArray(attribPointer);
    }

    /*
     * Lock
     */

    void VertexArrayObject::ready()
    {
        unbind();
        this->bReady = true;
    }

    /*
     * Rendering
     */

    void VertexArrayObject::render()
    {
        if (!bReady)
            return;

        bind();
        enableVertexAttribArrays();

        switch (renderType)
        {
            using enum RenderType;

            case ARRAYS:
                drawArrays(); break;
            case STRIP:
                drawStrip(); break;
            case ELEMENTS:
                drawElements(); break;
            default:
                break;
        }

        disableVertexAttribArrays();
        unbind();
    }

    void VertexArrayObject::drawArrays() const
    {
        glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    }

    void VertexArrayObject::drawStrip() const
    {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, verticesCount);
    }

    void VertexArrayObject::drawElements() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
