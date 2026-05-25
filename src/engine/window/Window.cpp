#include "Window.h"

#include <ranges>
#include <spdlog/spdlog.h>

#include "IRenderLayer.h"
#include "../messenger/Messenger.hpp"

namespace engine
{
    Window::~Window()
    {
        if (window != nullptr)
        {
            glfwDestroyWindow(window);

            for (const auto &layer : renderLayers)
                layer->onDestroy();
        }

        glfwTerminate();
    }

    void Window::create()
    {
        // Init GLFW
        glfwSetErrorCallback([](int error, const char* description) {
            spdlog::error("Caught a GLFW error [{}]: \n{}", error, description);
        });

        if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

        // Create the window
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
        glfwSetWindowUserPointer(window, this);

        // Setup callbacks
        setupCallbacks();

        // Centering the window
        const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(window, (vidMode->width - width) / 2, (vidMode->height - height) / 2);

        // Set context current
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);

        //Create OpenGL capabilities
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw std::runtime_error("Failed to initialize OpenGL context");

        // Log OpenGL version
        spdlog::info("OpenGL version {}", std::string_view(reinterpret_cast<const char*>(glGetString(GL_VERSION))));

        //Show the window
        glfwShowWindow(window);

        for (const auto &layer : renderLayers)
            layer->onInit(window);
    }

    void Window::setupCallbacks() const
    {
        // Window Resize
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, const int newWidth, const int newHeight) {
            auto *self = retrieveWindow(window);

            self->width = newWidth;
            self->height = newHeight;

            self->resized = true;
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow *windowHandle, const int button, const int action, const int mods) {
            retrieveMouse(windowHandle).onMouseButton(button, action, mods);
        });

        glfwSetScrollCallback(window, [](GLFWwindow *windowHandle, const double xOffset, const double yOffset) {
            retrieveMouse(windowHandle).onScroll(xOffset, yOffset);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow *windowHandle, const double xPos, const double yPos) {
            retrieveMouse(windowHandle).onCursorPosition(xPos, yPos);
        });

        glfwSetKeyCallback(window, [](GLFWwindow *windowHandle, const int key, const int scancode, const int action, const int mods) {
            retrieveKeyboard(windowHandle).onKey(key, scancode, action, mods);
        });
    }

    void Window::pushAndPop(const float deltaTime)
    {
        if (resized)
        {
            glViewport(0, 0, width, height);
            resized = false;

            WindowResizeEvent event(width, height);
            messenger->dispatch(event);
        }

        frameTime = static_cast<float>(glfwGetTime());

        if (grabbed)
            inputsManager.getMouse().processDeltas();

        glClear(GL_COLOR_BUFFER_BIT);
        auto activeLayers = renderLayers | std::views::filter([](const auto &layer) { return layer->canRender(); });
        // We're doing this separately to be versatile as possible. (For exemple, imgui need to be need to be rendered after geometry)
        for (const auto &layer : activeLayers)
            layer->onFramePush(deltaTime);

        for (const auto &layer : activeLayers)
            layer->onFramePop();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    /*
     * Getters
     */

    bool Window::shouldClose() const
    {
        return window != nullptr ? glfwWindowShouldClose(window) : true;
    }

    /*
     * Setters
     */

    void Window::setTitle(std::string title)
    {
        this->title = std::move(title);
        glfwSetWindowTitle(window, title.c_str());
    }

    void Window::setGrabbed(const bool grabbed)
    {
        glfwSetInputMode(window, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        this->grabbed = grabbed;
    }
}
