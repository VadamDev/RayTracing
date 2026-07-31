#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "inputs/InputsManager.h"

namespace engine
{
    class Messenger;
    class IRenderLayer;

    struct WindowResizeEvent
    {
        explicit WindowResizeEvent(const int newWidth, const int newHeight)
            : newWidth(newWidth), newHeight(newHeight), newAspectRatio(static_cast<float>(newWidth) / static_cast<float>(newHeight)) {}

        int newWidth, newHeight;
        float newAspectRatio;
    };

    class Window
    {
        friend class Application;

    public:
        Window(const int width, const int height, std::string title, const bool vsync = false)
            : width(width), height(height), title(std::move(title)), vsync(vsync) {}
        ~Window();

        /*
         * Management
         */

        void create();

        template<std::derived_from<IRenderLayer> T, typename... Args>
        std::shared_ptr<T> registerLayer(Args&&... args)
        {
            if (window != nullptr)
                throw std::runtime_error("Attempted to register a render layer to an already created window");

            auto layer = std::make_shared<T>(std::forward<Args>(args)...);
            renderLayers.push_back(layer);

            return layer;
        }

        void pushAndPop(float deltaTime);

        /*
         * Getters
         */

        bool shouldClose() const;

        float getFrameTime() const { return frameTime; }

        bool isGrabbed() const { return grabbed; }
        bool wasGrabbedBefore() const { return wasGrabbed; }

        int getWidth() const { return width; }
        int getHeight() const { return height; }
        float getAspectRatio() const { return (float) width / height; }
        std::string getTitle() const { return title; }
        bool isVsync() const { return vsync; }

        InputsManager& getInputsManager() { return inputsManager; }

        /*
         * Setters
         */

        void setVsync(bool vsync);
        void setTitle(std::string title);
        void setGrabbed(bool grabbed);
        void hideCursor(bool hidden);

    private:
        int width, height;
        std::string title;
        bool vsync = false;

        GLFWwindow *window = nullptr;
        InputsManager inputsManager;
        Messenger *messenger = nullptr;

        bool resized = true, grabbed = false, wasGrabbed = false, cursorHidden = false;
        float frameTime = 0;

        std::vector<std::shared_ptr<IRenderLayer>> renderLayers;

        void setupCallbacks() const;

        /*
         * Static Utility
         * retrieve objects used inside GLFW callbacks from window user pointer
         */

        static Mouse& retrieveMouse(GLFWwindow *glfwWindow)
        {
            return retrieveWindow(glfwWindow)->getInputsManager().getMouse();
        }

        static Keyboard& retrieveKeyboard(GLFWwindow *glfwWindow)
        {
            return retrieveWindow(glfwWindow)->getInputsManager().getKeyboard();
        }

        static Window* retrieveWindow(GLFWwindow *glfwWindow)
        {
            return static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        }
    };
}
