#include "Game.h"

namespace game
{
    void Game::init()
    {
        //Renderer
        renderer = std::make_shared<Renderer>();
        renderer->init(window);

        //Controller
        cameraController = std::make_unique<CameraController>(renderer->getCamera(), window.getInputsManager());

        //Gui
        gui = std::make_unique<Gui>(this, renderer.get());
        window.registerImGuiWindow(gui);
    }

    void Game::update()
    {

    }

    void Game::processInputs(float deltaTime)
    {
        const auto inputManager = window.getInputsManager();

        if (inputManager->isMouseButtonDown(engine::MouseButton::BUTTON_1) && !window.isGrabbed() && !engine::Window::wantCapturePeripherals())
            window.setGrabbed(true);
        else if (inputManager->isKeyDown(engine::KeyboardKeys::KEY_ESCAPE) && window.isGrabbed())
            window.setGrabbed(false);

        if (!window.isGrabbed())
            return;

        cameraController->processInputs(deltaTime);
    }

    void Game::render(const float deltaTime)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        renderer->render();
    }

    void Game::destroy() noexcept
    {

    }
}
