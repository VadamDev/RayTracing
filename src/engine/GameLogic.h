#pragma once

namespace engine
{
    class GameLogic {

    public:
        virtual ~GameLogic() = default;

        virtual void processInputs(float deltaTime) = 0;
        virtual void update() = 0;
        virtual void render(float deltaTime) = 0;
    };
}
