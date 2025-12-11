#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

namespace window
{
    class Window
    {

    public:
        Window(const int width, const int height, std::string title)
            : width(width), height(height), title(std::move(title)) {}

        ~Window();

        GLFWwindow* create();
        void update();

        /*
           Getters
         */

        bool shouldClose();

        int getWidth() const
        {
            return width;
        }

        int getHeight() const
        {
            return height;
        }

        std::string getTitle()
        {
            return title;
        }
    private:
        int width, height;
        std::string title;

        GLFWwindow *window = nullptr;
    };
}

#endif
