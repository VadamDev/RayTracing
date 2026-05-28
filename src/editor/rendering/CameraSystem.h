#pragma once

#include <glm/mat4x4.hpp>

#include "../../engine/scene/ComponentSystem.hpp"
#include "../controller/ICamController.h"
#include "../scene/Components.h"

namespace editor
{
    class RenderingCanvas;
    class SceneHandler;

    struct CameraRef
    {
        engine::Entity entity = {};

        CameraComponent *camera = nullptr;
        TransformComponent *transform = nullptr;

        operator bool() const { return entity && camera != nullptr && transform != nullptr; }
    };

    class CameraSystem : public engine::ComponentSystem<CameraComponent>
    {

    public:
        explicit CameraSystem(RenderingCanvas *canvas, SceneHandler *sceneHandler)
            : canvas(canvas), sceneHandler(sceneHandler) {}

        template<std::derived_from<ICamController> T, typename... Args>
        void registerController(Args&&... args)
        {
            controller = std::make_unique<T>(std::forward<Args>(args)...);
        }

        void processInputs(float deltaTime) const;
        void findPrimaryCamera();

        void updateLocalToWorldMatrix();
        void updateProjectionMatrix();

        CameraRef& getPrimaryCamera() { return primaryCamera; }
        bool isCameraPresent() const { return primaryCamera; }

        glm::mat4& getLocalToWorldMatrix() { return localToWorldMat; }
        glm::mat4& getProjectionMatrix() { return projectionMat; }

    private:
        RenderingCanvas *canvas;
        SceneHandler *sceneHandler;
        std::unique_ptr<ICamController> controller = nullptr;

        CameraRef primaryCamera = {};

        glm::mat4 localToWorldMat = glm::identity<glm::mat4>();
        glm::mat4 projectionMat = glm::identity<glm::mat4>();
    };
}
