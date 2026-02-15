#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "../RaytracingApplication.h"
#include "../../engine/scene/Entity.h"

namespace application
{
    void Renderer::init(engine::Window &window, RaytracingApplication *application)
    {
        this->application = application;

        //Screen Mesh
        canvasMesh = std::make_unique<engine::VertexArrayObject>();
        canvasMesh->createAndBind(4, engine::RenderType::STRIP);

        const std::vector<float> vertices = { -1, 1, -1, -1, 1, 1, 1, -1 };
        canvasMesh->genBuffer(sizeof(float) * 2 * 4, vertices.data(), GL_STATIC_DRAW, 2, GL_FLOAT);
        canvasMesh->ready();

        //Shader
        shader = std::make_unique<TracingShader>();
        shader->create();

        //Framebuffer
        framebuffer = std::make_shared<engine::Framebuffer>(window.getWidth(), window.getHeight(), &window);
        framebuffer->create();

        camera = std::make_unique<Camera>(75, 1, this);

        spheresBuffer.create();
    }

    void Renderer::render()
    {
        framebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();
        updateSpheres();
        shader->currentFrameTime->set1f(application->getWindow().getFrameTimeF());
        shader->maxBounces->set1i(maxBounces);
        shader->raysPerPixel->set1i(raysPerPixel);
        shader->sendViewParams(camera.get());

        canvasMesh->render();

        shader->unbind();
        framebuffer->unbind();
    }

    void Renderer::onViewportResize(const std::function<void(engine::WindowResizeEvent &)> &callback)
    {
        viewportResizeDispatcher.subscribe(callback);
    }

    void Renderer::dispatchViewportResize(engine::WindowResizeEvent event)
    {
        viewportResizeDispatcher.dispatch(event);
    }

    /*
     * TODO: this is heavily temporary, we do not need update every frame, just check for changes (with imgui?), and update the buffer if necessary
     * this requires some serious rework, so just leave it as it is for now
     * also this approach might seems a bit stupid, since data is duplicated between components. Only RaytracedSphereComponent is actually sent to the renderer
     *
     */
    void Renderer::updateSpheres()
    {
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

        spheresBuffer.update(allSpheres);
        shader->numSpheres->set1i(spheresBuffer.size());
    }
}
