#pragma once

#include <memory>

#include "../engine/AbstractGame.h"
#include "controller/CameraController.h"
#include "gui/Gui.h"
#include "rendering/Renderer.h"

namespace game
{
    class Game : public engine::AbstractGame
    {

    public:
        using AbstractGame::AbstractGame;

        void init() override;

        void update() override;
        void processInputs(float deltaTime) override;
        void render(float deltaTime) override;

        void destroy() noexcept override;

    private:
        std::shared_ptr<Renderer> renderer;
        std::unique_ptr<CameraController> cameraController;

        std::shared_ptr<Gui> gui;
    };
}
