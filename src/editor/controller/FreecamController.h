#pragma once

#include <glm/vec3.hpp>

#include "ICamController.h"
#include "../../engine/window/Window.h"

namespace editor
{
    class FreecamController : public ICamController
    {

    public:
        explicit FreecamController(CameraSystem *cameraSystem, engine::Window &window)
            : ICamController(cameraSystem), window(window), inputsManager(window.getInputsManager()) {}

        bool processInputs(float deltaTime) override;

        float sensitivity = 0.2f;
        float cameraSpeed = 5;
    private:
        engine::Window &window;
        engine::InputsManager &inputsManager;

        bool moved = false;

        void processMouse(glm::vec3 &cameraRot);
        void processKeyboard(glm::vec3 &cameraPos, const glm::vec3 &cameraRot, float deltaTime);

        static void moveCamera(const glm::vec3 &offset, glm::vec3 &cameraPos, const glm::vec3 &cameraRot);
    };
}
