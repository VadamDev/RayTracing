#pragma once

namespace engine
{
    class IGameLifecycle
    {

    public:
        virtual ~IGameLifecycle() = default;

        virtual void init() = 0;

        virtual void update() = 0;
        virtual void processInputs(float deltaTime) = 0;
        virtual void render(float deltaTime) = 0;

        virtual void destroy() noexcept = 0;
    };
}
