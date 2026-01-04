#pragma once

#include "../rendering/Renderable.h"
#include <unordered_map>

namespace engine
{
    enum class RenderType
    {
        ARRAYS = 0, STRIP = 1, ELEMENTS = 2
    };

    class VertexArrayObject : public Renderable
    {

    public:
        ~VertexArrayObject() override;

        /*
         * Create
         */

        void create(int verticesCount, RenderType renderType);
        void create(const int verticesCount)
        {
            create(verticesCount, RenderType::ARRAYS);
        }

        void createAndBind(const int verticesCount, const RenderType renderType)
        {
            create(verticesCount, renderType);
            bind();
        }

        void createAndBind(const int verticesCount)
        {
            createAndBind(verticesCount, RenderType::ARRAYS);
        }

        /*
         * Bind / Unbind
         */

        void bind() const;
        static void unbind();

        /*
         * Buffers
         */

        void genElementBuffer(signed long long int size, const void *data);
        unsigned int genBuffer(signed long long int size, const void *data, unsigned int usage, int vaSize, unsigned int vaType);

        /*
         * Attrib Pointer
         */

        void enableVertexAttribArrays() const;
        void disableVertexAttribArrays() const;

        /*
         * Lock
         */

        void ready();

        /*
         * Rendering
         */

        void render() override;

        void drawArrays() const;
        void drawStrip() const;
        void drawElements() const;

    private:
        RenderType renderType = RenderType::ARRAYS;
        unsigned int vao = 0, ebo = 0;

        int verticesCount = 0;

        std::unordered_map<unsigned int, unsigned int> buffersMap;
        unsigned int vertexAttributeIndex = 0;

        bool bReady = false;
    };
}
