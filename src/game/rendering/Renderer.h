#pragma once

#include "../../engine/graphics/mesh/VertexArrayObject.h"
#include "Camera.h"
#include "SpheresShaderBuffer.h"
#include "TracingShader.h"
#include "../../engine/graphics/Framebuffer.h"
#include "../../engine/window/Window.h"

namespace application
{
    class RaytracingApplication;

    class Renderer : public engine::IRenderable
    {

    public:
        void init(engine::Window &window, RaytracingApplication *application);
        void render() override;

        void onViewportResize(const std::function<void(engine::WindowResizeEvent&)> &callback);
        void dispatchViewportResize(engine::WindowResizeEvent event);

        Camera* getCamera() const { return camera.get(); }
        std::shared_ptr<engine::Framebuffer> getFramebuffer() const { return framebuffer; }

        int maxBounces = 8;
        int raysPerPixel = 16;
    private:
        RaytracingApplication *application = nullptr;

        std::unique_ptr<engine::VertexArrayObject> canvasMesh;
        std::unique_ptr<TracingShader> shader;

        std::shared_ptr<engine::Framebuffer> framebuffer;

        std::unique_ptr<Camera> camera;

        engine::EventDispatcher<engine::WindowResizeEvent> viewportResizeDispatcher;

        SpheresShaderBuffer spheresBuffer;

        void updateSpheres();
    };
}
