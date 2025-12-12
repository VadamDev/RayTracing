#include <iostream>

#include "Window.h"

namespace engine
{
    Window::~Window()
    {
        if(window != nullptr)
            glfwDestroyWindow(window);

        glfwTerminate(); //Have no effect when GLFW is not initialized so no need to add a check, I think... Maybe? Eh...
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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

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

        //Black magic pointer shit that I DO NOT understand
        glfwSetWindowUserPointer(window, this);

        //Setup callbacks
        setupCallbacks();

        //Centering the window
        const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(window, (vidMode->width - width) / 2, (vidMode->height - height) / 2);

        //Set context current
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);

        //Show the window
        glfwShowWindow(window);

        //Create OpenGL capabilities
        //TODO: glad

        return window;
    }

    void Window::setupCallbacks()
    {
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
            auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));

            self->width = width;
            self->height = height;

            self->resized = true;
        });

        //TODO: actually setup callbacks
    }

    void Window::update()
    {
        if (resized)
        {
            glViewport(0, 0, width, height);

            std::cout << "Resized! | dimensions: " << width << " / " << height << std::endl;
            resized = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
    }

    void Window::swapBuffers() const
    {
        glfwSwapBuffers(window);
    }


    /*
       Getters
     */

    bool Window::shouldClose() const
    {
        return window != nullptr ? glfwWindowShouldClose(window) : true;
    }

    /*
       Setters
     */

    void Window::setTitle(std::string title)
    {
        this->title = std::move(title);
        glfwSetWindowTitle(window, this->title.c_str());
    }
}