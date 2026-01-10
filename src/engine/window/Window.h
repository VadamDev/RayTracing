#pragma once

#include <glad/glad.h> //Prevent the compiler from bitching, GLAD need to be loaded before GLFW
#include <GLFW/glfw3.h>
#include <string>
#include <utility>

#include "../event/EventDispatcher.h"
#include "event/WindowResizeEvent.h"
#include "input/InputsManager.h"

namespace engine
{
    class Window
    {

    public:
        explicit Window(const int width, const int height, std::string title)
            : width(width), height(height), title(std::move(title)) {}
        ~Window();

        /*
         * Window Management
         */

        void create();

        void pushFrame() noexcept;
        void popFrame() const noexcept;

        void whenResized(const std::function<void(WindowResizeEvent&)>& callback);

        /*
         * Getters
         */

        bool shouldClose() const;

        double getFrameTime() const { return dFrameTime; }
        float getFrameTimeF() const { return fFrameTime; }

        bool isGrabbed() const { return grabbed; }

        int getWidth() const { return width; }
        int getHeight() const { return height; }
        float getAspectRatio() const { return (float) width / height; }
        std::string getTitle() const { return title; }

        std::shared_ptr<InputsManager> getInputsManager() const { return inputManager; }

        /*
         * Setters
         */

        void setTitle(std::string title);
        void setGrabbed(bool grabbed);

        /*
         * Static Utils
         */

        static int getMonitorRefreshRate();

    private:
        int width, height;
        std::string title;

        GLFWwindow *window = nullptr;

        std::shared_ptr<InputsManager> inputManager;
        EventDispatcher<WindowResizeEvent> resizeDispatcher;

        bool resized = true, grabbed = false;
        double dFrameTime = 0, fFrameTime = 0;

        void setupCallbacks();

        /*
         * Static Utility
         * retrieve objects used inside GLFW callbacks from window user pointer
         */

        static Mouse& retrieveMouse(GLFWwindow *glfwWindow)
        {
            return retrieveWindow(glfwWindow)->getInputsManager()->getMouse();
        }

        static Keyboard& retrieveKeyboard(GLFWwindow *glfwWindow)
        {
            return retrieveWindow(glfwWindow)->getInputsManager()->getKeyboard();
        }

        static Window* retrieveWindow(GLFWwindow *glfwWindow)
        {
            return static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        }
    };
}
