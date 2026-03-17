#pragma once

#include "../../engine/graphics/mesh/VertexArrayObject.h"
#include "Camera.h"
#include "Canvas.h"
#include "RaytracingShader.h"
#include "../../engine/window/Window.h"

namespace engine
{
    class ProfilerEntry;
}

namespace application
{
    class RaytracingApplication;

    class Renderer : public engine::IRenderable
    {

    public:
        void init(engine::Window *window, RaytracingApplication *application);

        void render() override;

        void onCanvasResize(const std::function<void(engine::WindowResizeEvent&)> &callback);
        void dispatchCanvasResize(engine::WindowResizeEvent event);

        Canvas* getCanvas() const { return canvas.get(); }
        Camera* getCamera() const { return camera.get(); }

        bool accumulate = true;
        int maxBounces = 8;
        int raysPerPixel = 1;
        bool environmentLight = true;
    private:
        unsigned int frameIndex = 1;
        bool accumulatedBefore = true; //TODO: hack, until i add an event for accumulation reset

        engine::Window *window = nullptr;
        RaytracingApplication *application = nullptr;

        std::unique_ptr<Canvas> canvas;
        std::unique_ptr<Camera> camera;
        std::unique_ptr<RaytracingShader> shader;

        engine::EventDispatcher<engine::WindowResizeEvent> viewportResizeDispatcher;

        void updateBuffers() const;

        void updateSpheres() const;
        void updateBoxes() const;
        void updateTriangles() const;
        void updateTriangleMeshes() const;
    };
}
