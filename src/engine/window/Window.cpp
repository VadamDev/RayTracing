#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "spdlog/spdlog.h"

namespace engine
{
    Window::~Window()
    {
        if(window != nullptr)
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            glfwDestroyWindow(window);
        }

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

        //Show OpenGL version
        spdlog::info("OpenGL version {}", std::string_view(reinterpret_cast<const char*>(glGetString(GL_VERSION))));

        //Show the window
        glfwShowWindow(window);

        //Setup Imgui
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void Window::setupCallbacks()
    {
        /*
         * Window Resize
         */

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, const int width, const int height) {
            auto *self = retrieveWindow(window);

            self->width = width;
            self->height = height;

            self->resized = true;
        });

        /*
         * Inputs
         */

        inputManager = std::make_shared<InputsManager>();

        /*
         * Mouse
         */

        glfwSetMouseButtonCallback(window, [](GLFWwindow *windowHandle, const int button, const int action, const int mods) {
            retrieveMouse(windowHandle).onMouseButton(button, action, mods);
        });

        glfwSetScrollCallback(window, [](GLFWwindow *windowHandle, const double xOffset, const double yOffset) {
            retrieveMouse(windowHandle).onScroll(xOffset, yOffset);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow *windowHandle, const double xPos, const double yPos) {
            retrieveMouse(windowHandle).onCursorPosition(xPos, yPos);
        });

        /*
         * Keyboard
         */

        glfwSetKeyCallback(window, [](GLFWwindow *windowHandle, const int key, const int scancode, const int action, const int mods) {
            retrieveKeyboard(windowHandle).onKey(key, scancode, action, mods);
        });
    }

    void Window::pushFrame() noexcept
    {
        if (resized)
        {
            glViewport(0, 0, width, height);
            resized = false;

            WindowResizeEvent event(width, height);
            resizeDispatcher.dispatch(event);
        }

        dFrameTime = glfwGetTime();
        fFrameTime = static_cast<float>(dFrameTime);

        inputManager->getMouse().processDeltas();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui is currently rendered before world rendering. We're always rendering to a framebuffer so it'll never cause issues
        for (auto &imguiWindow : imguiWindows)
            imguiWindow->draw();
    }

    void Window::popFrame() const noexcept
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    void Window::whenResized(const std::function<void(WindowResizeEvent&)> &callback)
    {
        resizeDispatcher.subscribe(callback);
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

    void Window::setGrabbed(const bool grabbed)
    {
        glfwSetInputMode(window, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        this->grabbed = grabbed;
    }

    /*
     * Static Utils
     */

    int Window::getMonitorRefreshRate()
    {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
    }

    bool Window::wantCapturePeripherals()
    {
        const ImGuiIO &io = ImGui::GetIO();
        return io.WantCaptureMouse || io.WantCaptureKeyboard;
    }
}
