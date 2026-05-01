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

    struct UpdateShaderBuffersEvent
    {
        bool resetAccumulation = true;
    };

    class Renderer : public engine::IRenderable
    {

    public:
        void init(engine::Window *window, RaytracingApplication *application);

        void render() override;

        Canvas* getCanvas() const { return canvas.get(); }
        Camera* getCamera() const { return camera.get(); }
        engine::Messenger* getGlobalMessenger() const;

        int drawDebugMode = 0;
        glm::vec2 statsThresholds = glm::vec2(1);
        bool accumulate = true;
        int maxBounces = 4;
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

        bool shouldUpdateBuffers = true;

        void updateSpheres() const;
        void updateBoxes() const;
        void updateMeshDataBuffers() const;
        void updateTriangleMeshes() const;

        void resetAccumulation() const;
    };
}
