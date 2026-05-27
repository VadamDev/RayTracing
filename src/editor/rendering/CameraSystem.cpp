#include "CameraSystem.h"

#include "RenderingCanvas.h"

namespace editor
{
    static constexpr glm::vec3 RIGHT(1, 0, 0);
    static constexpr glm::vec3 UP   (0, 1, 0);

    void CameraSystem::processInputs(const float deltaTime) const
    {
        if (controller == nullptr || !isCameraPresent())
            return;

        controller->processInputs(deltaTime);
    }

    void CameraSystem::findPrimaryCamera()
    {
        if (!sceneHandler->isSceneOpened() || !isAnyPresent(sceneHandler->getOpenedScene()))
            return;

        CameraRef result = {};

        engine::Scene *scene = sceneHandler->getOpenedScene();
        for (const auto &entityHandle : scene->registry.view<CameraComponent>())
        {
            const engine::Entity entity = { entityHandle, scene };
            if (!entity.hasComponent<TransformComponent>())
                continue;

            auto &cameraCp = entity.getComponent<CameraComponent>();
            auto &transformCp = entity.getComponent<TransformComponent>();

            if (!result || cameraCp.primary)
            {
                result.entity = entity;

                result.camera = &cameraCp;
                result.transform = &transformCp;
            }
        }

        primaryCamera = result;
        updateLocalToWorldMatrix();
        updateProjectionMatrix();
    }

    void CameraSystem::updateLocalToWorldMatrix()
    {
        if (!isCameraPresent())
            return;

        const TransformComponent *transform = primaryCamera.transform;

        localToWorldMat = glm::identity<glm::mat4>();
        localToWorldMat = glm::translate(localToWorldMat, transform->position);
        localToWorldMat = glm::rotate(localToWorldMat, glm::radians(transform->rotation.y), UP);
        localToWorldMat = glm::rotate(localToWorldMat, glm::radians(transform->rotation.x), RIGHT);
    }

    void CameraSystem::updateProjectionMatrix()
    {
        if (!isCameraPresent())
            return;

        const CameraComponent *camera = primaryCamera.camera;
        projectionMat = glm::perspective(glm::atan(glm::tan(glm::radians(camera->fov / 2.0f)) / 2) * 2, canvas->getAspectRatio(), camera->focalPlane, 1000.f); //TODO: change me!
    }
}
