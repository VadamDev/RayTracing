#pragma once

#include <string>
#include <utility>
#include <GLFW/glfw3.h>

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

        GLFWwindow* create();

        void update();
        void swapBuffers() const;

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
