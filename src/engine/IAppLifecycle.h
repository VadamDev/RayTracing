#pragma once

namespace engine
{
    class IAppLifecycle
    {

    public:
        virtual ~IAppLifecycle() = default;

        virtual void onInit() = 0;
        virtual void onPostInit() = 0;
        virtual void onProcessInputs(float deltaTime) = 0;
        virtual void onDestroy() noexcept = 0;
    };
}
