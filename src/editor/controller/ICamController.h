#pragma once

namespace editor
{
    class CameraSystem;

    class ICamController
    {

    public:
        explicit ICamController(CameraSystem *cameraSystem)
            : cameraSystem(cameraSystem) {}
        virtual ~ICamController() = default;

        virtual void processInputs(float deltaTime) = 0;

    protected:
        CameraSystem *cameraSystem;
    };
}
