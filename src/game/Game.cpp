#include "Game.h"

#include "../engine/graphics/shader/ShaderProgram.h"
#include "../engine/graphics/shader/exceptions/ShaderException.h"
#include <spdlog/spdlog.h>

#include "TestShader.h"

namespace game
{
    void Game::init()
    {
        try
        {
            TestShader shader;
            shader.create();
        }
        catch (engine::exceptions::ProgramException &e)
        {
            spdlog::critical(e.what());
        }
        catch (engine::exceptions::ShaderException &e)
        {
            spdlog::critical(e.what());
        }
    }

    void Game::update()
    {

    }

    void Game::processInputs(float deltaTime)
    {

    }

    void Game::render(float deltaTime)
    {

    }

    void Game::destroy() noexcept
    {

    }

}
