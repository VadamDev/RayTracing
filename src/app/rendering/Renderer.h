#pragma once

#include "Camera.h"
#include "Canvas.h"
#include "RaytracingShader.h"
#include "../../engine/graphics/IRenderable.h"
#include "../../engine/window/Window.h"

namespace engine
{
    class Messenger;
}

namespace application
{
    class RaytracingApplication;

    struct AccumulationResetEvent
    {
        unsigned int newFrameIndex = 1;
    };

    class Renderer : public engine::IRenderable
    {

    public:
        void init(engine::Window *window, RaytracingApplication *application);

        void render() override;

        Canvas* getCanvas() const { return canvas.get(); }
        Camera* getCamera() const { return camera.get(); }
        engine::Messenger* getGlobalMessenger() const;

        bool accumulate = true;
        int maxBounces = 8;
        int raysPerPixel = 1;
        bool environmentLight = true;
        float divergeStrength = 0.5f;
        float defocusStrength = 0;
    private:
        unsigned int frameIndex = 1;

        engine::Window *window = nullptr;
        RaytracingApplication *application = nullptr;

        std::unique_ptr<Canvas> canvas;
        std::unique_ptr<Camera> camera;
        std::unique_ptr<RaytracingShader> shader;

        void updateBuffers() const;

        void updateSpheres() const;
        void updateBoxes() const;
        void updateTriangles() const;
        void updateTriangleMeshes() const;
    };
}
