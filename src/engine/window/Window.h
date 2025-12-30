#pragma once

#include <glad/glad.h> //Prevent the compiler from bitching, GLAD need to be loaded before GLFW
#include <GLFW/glfw3.h>
#include <string>
#include <utility>

namespace engine
{
    class Window
    {

    public:
        explicit Window(const int width, const int height, std::string title)
            : width(width), height(height), title(std::move(title)) {}
        ~Window();

        /*
           Window Management
         */

        void create();

        void pushFrame() noexcept;
        void popFrame() const noexcept;

        /*
           Getters
         */

        bool shouldClose() const;

        int getWidth() const { return width; }
        int getHeight() const { return height; }
        float getAspectRatio() const { return (float) width / height; }
        std::string getTitle() const { return title; }

        /*
           Setters
         */

        void setTitle(std::string title);

    private:
        int width, height;
        std::string title;

        GLFWwindow *window = nullptr;

        bool resized = true;

        void setupCallbacks();
    };
}
