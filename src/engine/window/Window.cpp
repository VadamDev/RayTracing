#include "Window.h"

#include "spdlog/spdlog.h"

namespace engine
{
    Window::~Window()
    {
        if(window != nullptr)
            glfwDestroyWindow(window);

        glfwTerminate(); //Have no effect when GLFW is not initialized so no need to add a check, I think... Maybe? Eh...
    }

    void Window::create()
    {
        //Initializing GLFW
        glfwSetErrorCallback([](int error, const char* description) {
            spdlog::error("Caught a GLFW error [{}]: \n{}", error, description);
        });

        if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

        //Creating the window
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
            throw std::runtime_error("Failed to create a GLFW window");

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

        //Create OpenGL capabilities
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            throw std::runtime_error("Failed to initialize OpenGL context");

        //Show the window
        glfwShowWindow(window);

        spdlog::info("OpenGL version is {}", std::string_view(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
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

    void Window::pushFrame() noexcept
    {
        if (resized)
        {
            glViewport(0, 0, width, height);
            resized = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    void Window::popFrame() const noexcept
    {
        glfwPollEvents();
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