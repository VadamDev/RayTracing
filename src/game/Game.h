#pragma once

#include <memory>

#include "TestShader.h"
#include "../engine/AbstractGame.h"
#include "../engine/graphics/mesh/VertexArrayObject.h"

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
        std::unique_ptr<engine::VertexArrayObject> vao;
        std::unique_ptr<TestShader> shader;
    };
}

