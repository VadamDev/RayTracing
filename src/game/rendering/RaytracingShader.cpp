#include "RaytracingShader.h"

#include <glad/glad.h>

namespace application
{
    void RaytracingShader::create()
    {
        ComputeShader::create();
        spheresBuffer.create();
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
        currentFrameTime = accessUniform("currentFrameTime");
        viewParams = accessUniform("viewParams");
        cameraPos = accessUniform("cameraPos");

        localToWorldMatrix = accessUniform("localToWorld");

        maxBounces = accessUniform("maxBounces");
        raysPerPixel = accessUniform("raysPerPixel");
    }

    void RaytracingShader::updateSpheresBuffer(const std::vector<RaytracedSphereComponent> &spheres)
    {
        spheresBuffer.update(spheres, GL_DYNAMIC_DRAW);
    }
}
