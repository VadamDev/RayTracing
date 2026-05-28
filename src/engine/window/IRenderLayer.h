#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine
{
    class IRenderLayer
    {

    public:
        virtual ~IRenderLayer() = default;

        virtual void onInit(GLFWwindow *window) = 0;

        virtual bool canRender() const = 0;
        virtual void onFramePush(float deltaTime) = 0;
        virtual void onFramePop() = 0;

        virtual void onDestroy() noexcept = 0;
    };
}
