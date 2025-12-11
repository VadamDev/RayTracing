#include <iostream>

#include "Window.h"

namespace window
{
    Window::~Window()
    {
        if(window != nullptr)
            glfwDestroyWindow(window);

        glfwTerminate(); //Have no effect when GLFW is not initialized so no need to add a check
    }

    GLFWwindow* Window::create()
    {
        //Initializing GLFW
        glfwSetErrorCallback([](int error, const char* description) {
            std::cerr << "Caught a GLFW error [" << error << "]: " << description << std::endl;
        });

        if (!glfwInit())
            return nullptr;

        //Creating the window
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            return nullptr;
        }

        //Centering the window
        const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(window, (vidMode->width - width) / 2, (vidMode->height - height) / 2);

        //Set context current
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);

        //Show the window
        glfwShowWindow(window);

        return window;
    }

    void Window::update()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool Window::shouldClose()
    {
        return glfwWindowShouldClose(window);
    }
}