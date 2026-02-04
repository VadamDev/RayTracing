#include "Renderer.h"

namespace application
{
    void Renderer::init(engine::Window &window)
    {
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

        camera = std::make_unique<Camera>(75, 1, &window);
    }

    void Renderer::render()
    {
        framebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();
        shader->sendViewParams(camera.get());
        canvasMesh->render();
        shader->unbind();

        framebuffer->unbind();
    }
}
