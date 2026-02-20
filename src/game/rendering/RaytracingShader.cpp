#include "RaytracingShader.h"

#include <glad/glad.h>

namespace application
{
    void RaytracingShader::create()
    {
        ComputeShader::create();
        spheresBuffer.create();
        boxesBuffer.create();
    }

    void RaytracingShader::sendViewParams(Camera *camera) const
    {
        const float planeHeight = camera->nearClipPlane * tan(glm::radians(camera->fov / 2));
        const float planeWidth = planeHeight * camera->getAspectRatio();

        viewParams->set3f(planeWidth, planeHeight, camera->nearClipPlane);
        cameraPos->set3f(camera->position);

        localToWorldMatrix->setMatrix4f(camera->getLocalToWorldMatrix());
    }

    void RaytracingShader::setupUniforms()
    {
        frameIndex = accessUniform("frameIndex");
        viewParams = accessUniform("viewParams");
        cameraPos = accessUniform("cameraPos");

        localToWorldMatrix = accessUniform("localToWorld");

        accumulate = accessUniform("accumulate");
        maxBounces = accessUniform("maxBounces");
        raysPerPixel = accessUniform("raysPerPixel");
    }

    void RaytracingShader::updateSpheresBuffer(const std::vector<RaytracedSphereComponent> &spheres)
    {
        spheresBuffer.update(spheres, 0, GL_DYNAMIC_DRAW);
    }

    void RaytracingShader::updateBoxesBuffer(const std::vector<RaytracedBoxComponent> &boxes)
    {
        boxesBuffer.update(boxes, 1, GL_DYNAMIC_DRAW);
    }
}
