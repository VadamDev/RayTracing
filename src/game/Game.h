#pragma once

#include "../engine/AbstractGame.h"

namespace game
{
    class Game : public engine::AbstractGame
    {

    public:
        using AbstractGame::AbstractGame;

        void processInputs(float deltaTime) override;
        void update() override;
        void render(float deltaTime) override;
    };
}

