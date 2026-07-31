#pragma once

namespace engine
{
    class IAppLifecycle
    {

    public:
        virtual ~IAppLifecycle() = default;

        virtual void onInit() = 0; // Called before render layer & window init
        virtual void onPostInit() = 0; // Called AFTER render layer & window init
        virtual void onPreRender(float deltaTime) = 0; // Called BEFORE render layer pushFrame
        virtual void onDestroy() noexcept = 0;
    };
}
