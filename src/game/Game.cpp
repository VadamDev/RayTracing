#include "Game.h"

#include "../engine/graphics/shader/ShaderProgram.h"
#include "../engine/graphics/shader/exceptions/ShaderException.h"
#include <spdlog/spdlog.h>

#include "TestShader.h"
#include "../engine/graphics/mesh/VertexArrayObject.h"

namespace game
{
    void Game::init()
    {
        try
        {
            shader = std::make_unique<TestShader>();
            shader->create();
        }
        catch (engine::exceptions::ProgramException &e)
        {
            spdlog::critical(e.what());
        }
        catch (engine::exceptions::ShaderException &e)
        {
            spdlog::critical(e.what());
        }

        vao = std::make_unique<engine::VertexArrayObject>();
        vao->createAndBind(4, engine::RenderType::STRIP);

        const auto *vertices = new float[]{ -1, 1, -1, -1, 1, 1, 1, -1 };
        vao->genBuffer(sizeof(float) * 2 * 4, vertices, GL_STATIC_DRAW, 2, GL_FLOAT);

        delete vertices;

        vao->ready();
    }

    void Game::update()
    {

    }

    void Game::processInputs(float deltaTime)
    {
        const auto inputManager = getWindow().getInputsManager();

        auto& window = getWindow();
        if (inputManager->isMouseButtonDown(mouse_buttons::BUTTON_1) && !window.isGrabbed())
            window.setGrabbed(true);
        else if (inputManager->isKeyDown(keyboard_keys::KEY_ESCAPE) && window.isGrabbed())
            window.setGrabbed(false);
    }

    void Game::render(float deltaTime)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->bind();

        shader->frameTime->set1f(getWindow().getFrameTimeF());
        vao->render();

        TestShader::unbind();
    }

    void Game::destroy() noexcept
    {

    }

}
