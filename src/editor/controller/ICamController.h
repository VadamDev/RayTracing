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

        virtual bool processInputs(float deltaTime) = 0;

    protected:
        CameraSystem *cameraSystem;
    };
}
