#pragma once

#include "../engine/AbstractGame.h"

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
    };
}

