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

        //Canvas
        canvas = std::make_unique<Canvas>();
        canvas->create();

        //Camera
        camera = std::make_unique<Camera>(90, 1, this);

        //Shader
        shader = std::make_unique<RaytracingShader>();
        shader->create();

        camera->onCameraMove([this](CameraMovedEvent &_) {
            frameIndex = 1;
        });
    }

    void Renderer::render()
    {
        shader->bind();

        updateSpheres();
        shader->frameIndex->set1i(frameIndex++);
        shader->maxBounces->set1i(maxBounces);
        shader->raysPerPixel->set1i(raysPerPixel);
        shader->sendViewParams(camera.get());

        shader->dispatchCompute(ceil(canvas->getWidth() / 8), ceil(canvas->getHeight() / 4), 1, GL_ALL_BARRIER_BITS);

        shader->unbind();
    }

    void Renderer::onCanvasResize(const std::function<void(engine::WindowResizeEvent &)> &callback)
    {
        viewportResizeDispatcher.subscribe(callback);
    }

    void Renderer::dispatchCanvasResize(engine::WindowResizeEvent event)
    {
        canvas->resize(event.getNewWidth(), event.getNewHeight());
        frameIndex = 1;

        viewportResizeDispatcher.dispatch(event);
    }

    /*
     * TODO: this is heavily temporary, we do not need update every frame, just check for changes (with imgui?), and update the buffer if necessary
     * this requires some serious rework, so just leave it as it is for now
     * also this approach might seems a bit stupid, since data is duplicated between components. Only RaytracedSphereComponent is actually sent to the renderer
     */
    void Renderer::updateSpheres() const
    {
        //TODO: if any changes are detected here, we need to reset the frame index to 1

        std::vector<RaytracedSphereComponent> allSpheres;

        engine::Scene &currentScene = application->getActiveScene();
        for (auto &entityHandle : currentScene.registry.view<RaytracedMaterialComponent, RaytracedSphereComponent>())
        {
            const engine::Entity entity = { entityHandle, &currentScene };

            auto &sphere = entity.getComponent<RaytracedSphereComponent>();
            if (entity.hasComponent<TransformComponent>())
            {
                const auto &transform = entity.getComponent<TransformComponent>();
                sphere.position = transform.position;
                sphere.radius = transform.scale;
            }

            sphere.material = entity.getComponent<RaytracedMaterialComponent>();

            allSpheres.push_back(sphere);
        }

        shader->updateSpheresBuffer(allSpheres);
    }
}
