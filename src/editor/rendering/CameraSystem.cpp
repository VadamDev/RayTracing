#include "CameraSystem.h"

#include "RenderingCanvas.h"
#include "RenderingEvents.h"
#include "../../engine/messenger/Messenger.hpp"
#include "../scene/SceneHandler.h"

namespace editor
{
    void CameraSystem::processInputs(const float deltaTime) const
    {
        if (controller == nullptr || !isCameraPresent())
            return;

        if (controller->processInputs(deltaTime))
        {
            PrimaryCameraMovedEvent event;
            globalMessenger.dispatch(event);
        }
    }

    void CameraSystem::findPrimaryCamera()
    {
        if (!sceneHandler->isSceneOpened() || !isAnyPresent(sceneHandler->getOpenedScene()))
            return;

        CameraRef result = {};

        engine::Scene *scene = sceneHandler->getOpenedScene();
        for (const auto &entityHandle : scene->registry.view<CameraComponent, TransformComponent>())
        {
            const engine::Entity entity = { entityHandle, scene };

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

        localToWorldMat = primaryCamera.transform->toTransformMat();
    }

    void CameraSystem::updateProjectionMatrix()
    {
        if (!isCameraPresent())
            return;

        const CameraComponent *camera = primaryCamera.camera;

        const float halfHeight = glm::tan(glm::radians(camera->fov / 2.0f)) / 2.0f;
        const float fov = glm::atan(halfHeight) * 2;
        projectionMat = glm::perspective(fov, canvas->getAspectRatio(), camera->focalPlane, 1000.f);
    }
}
