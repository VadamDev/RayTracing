#pragma once

#include <memory>

#include "../../engine/graphics/mesh/VertexArrayObject.h"
#include "camera/Camera.h"
#include "shader/TracingShader.h"

namespace game
{
    class Renderer : public engine::IRenderable
    {

    public:
        void init(engine::Window &window);
        void render() override;

        Camera* getCamera() const { return camera.get(); }

    private:
        std::unique_ptr<engine::VertexArrayObject> canvasMesh;
        std::unique_ptr<TracingShader> shader;

        std::unique_ptr<Camera> camera;
    };
}
