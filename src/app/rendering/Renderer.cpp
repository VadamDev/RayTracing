#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "../RaytracingApplication.h"
#include "../../engine/scene/Entity.h"
#include "../scene/Components.h"

namespace application
{
    void Renderer::init(engine::Window *window, RaytracingApplication *application)
    {
        this->window = window;
        this->application = application;

        engine::Messenger *globalMessenger = application->getGlobalMessenger();

        // Canvas
        canvas = std::make_unique<Canvas>();
        canvas->create();

        // Camera
        camera = std::make_unique<Camera>(90, 1, globalMessenger);

        // Shader
        shader = std::make_unique<RaytracingShader>();
        shader->create();

        // Reset frame accumulation on camera move
        globalMessenger->subscribe<CameraMovedEvent>([this](const CameraMovedEvent *) {
            resetAccumulation();
        });

        // Resize render canvas on viewport resize + reset accumulation
        globalMessenger->subscribe<CanvasResizeEvent>([this](const CanvasResizeEvent *event) {
            canvas->resize(event->newWidth, event->newHeight);
            resetAccumulation();
        });

        // Reset Frame Accumulation
        globalMessenger->subscribe<AccumulationResetEvent>([this](const AccumulationResetEvent *event) {
            frameIndex = event->newFrameIndex;
        });

        // Update buffer storage and reset accumulation on entity add/remove
        globalMessenger->subscribe<engine::AddEntityToSceneEvent>([globalMessenger](const engine::AddEntityToSceneEvent *) {
            UpdateShaderBuffersEvent event;
            globalMessenger->dispatch(event);
        });

        globalMessenger->subscribe<engine::RemoveEntityFromSceneEvent>([globalMessenger](const engine::RemoveEntityFromSceneEvent *) {
            UpdateShaderBuffersEvent event;
            globalMessenger->dispatch(event);
        });

        // Only update buffer storage if necessary, whole scene is sent to the GPU when this event is fired
        globalMessenger->subscribe<UpdateShaderBuffersEvent>([this](const UpdateShaderBuffersEvent *event) {
            if (event->resetAccumulation)
                resetAccumulation();

            shouldUpdateBuffers = true;
        });
    }

    void Renderer::render()
    {
        if (shouldUpdateBuffers)
        {
            updateSpheres();
            updateMeshDataBuffers();
            updateTriangleMeshes();

            shouldUpdateBuffers = false;
        }

        shader->bind();

        shader->frameIndex->set1ui(frameIndex++);
        shader->drawDebugMode->set1i(drawDebugMode);
        shader->statsThresholds->set2f(statsThresholds);
        shader->accumulate->setBool(accumulate);
        shader->maxBounces->set1i(maxBounces);
        shader->raysPerPixel->set1i(raysPerPixel);
        shader->environmentLight->setBool(environmentLight);
        shader->divergeStrength->set1f(divergeStrength);
        shader->defocusStrength->set1f(defocusStrength);
        shader->sendViewParams(camera.get());

        RaytracingShader::dispatchCompute(ceil(canvas->getWidth() / 8), ceil(canvas->getHeight() / 8), 1, GL_TEXTURE_FETCH_BARRIER_BIT);

        shader->unbindBuffers();
        shader->unbind();
    }

    void Renderer::updateSpheres() const
    {
        std::vector<RaytracedSphere> allSpheres;

        engine::Scene *currentScene = application->getActiveScene();
        for (auto &entityHandle : currentScene->registry.view<RaytracedMaterialComponent, RaytracedSphereComponent>())
        {
            const engine::Entity entity = { entityHandle, currentScene };

            auto &sphere = entity.getComponent<RaytracedSphereComponent>().sphere;
            if (entity.hasComponent<TransformComponent>())
            {
                const auto &transform = entity.getComponent<TransformComponent>();
                sphere.position = transform.position;
                sphere.radius = (transform.scale.x + transform.scale.y + transform.scale.z) / 3;
            }

            sphere.material = entity.getComponent<RaytracedMaterialComponent>().material;
            allSpheres.push_back(sphere);
        }

        shader->updateSpheresBuffer(allSpheres);
    }

    void Renderer::updateMeshDataBuffers() const
    {
        const ModelManager &modelManager = application->getModelManager();
        shader->updateMeshDataBuffers(modelManager.getAllTriangles(), modelManager.getAllBVHNodes());
    }

    void Renderer::updateTriangleMeshes() const
    {
        const auto &meshesMap = application->getModelManager().getAllMeshes();

        std::vector<TriangleMeshData> allMeshes;

        engine::Scene *currentScene = application->getActiveScene();
        for (auto &entityHandle : currentScene->registry.view<RaytracedMaterialComponent, RaytracedMeshComponent>())
        {
            const engine::Entity entity = { entityHandle, currentScene };

            const std::string &meshName = entity.getComponent<RaytracedMeshComponent>().name;
            if (!meshesMap.contains(meshName))
                continue;

            auto transformMatrix = glm::identity<glm::mat4>();
            if (entity.hasComponent<TransformComponent>())
                transformMatrix = entity.getComponent<TransformComponent>().toTransformMat();

            const TriangleMesh &mesh = meshesMap.at(meshName);
            const TriangleMeshData data {
                .rootBVHNodeIndex = mesh.rootBVHNodeIndex,

                .localToWorld = transformMatrix,
                .worldToLocal = glm::inverse(transformMatrix),

                .material = entity.getComponent<RaytracedMaterialComponent>().material
            };

            allMeshes.push_back(data);
        }

        shader->updateMeshesBuffer(allMeshes);
    }

    engine::Messenger* Renderer::getGlobalMessenger() const
    {
        return application->getGlobalMessenger();
    }

    void Renderer::resetAccumulation() const
    {
        AccumulationResetEvent e;
        application->getGlobalMessenger()->dispatch(e);
    }
}
