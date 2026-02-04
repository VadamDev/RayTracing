#pragma once

#include "../../engine/graphics/mesh/VertexArrayObject.h"
#include "Camera.h"
#include "TracingShader.h"
#include "../../engine/graphics/Framebuffer.h"

namespace application
{
    class Renderer : public engine::IRenderable
    {

    public:
        void init(engine::Window &window);
        void render() override;

        Camera* getCamera() const { return camera.get(); }
        std::shared_ptr<engine::Framebuffer> getFramebuffer() const { return framebuffer; }

    private:
        std::unique_ptr<engine::VertexArrayObject> canvasMesh;
        std::unique_ptr<TracingShader> shader;

        std::shared_ptr<engine::Framebuffer> framebuffer;

        std::unique_ptr<Camera> camera;
    };
}
