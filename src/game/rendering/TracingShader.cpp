#include "TracingShader.h"

#include <spdlog/spdlog.h>

namespace application
{
    void TracingShader::sendViewParams(Camera *camera) const
    {
        const float planeHeight = camera->nearClipPlane * tan(glm::radians(camera->fov / 2)) * 2;
        const float planeWidth = planeHeight * camera->getAspectRatio();

        screenParams->set2i(camera->getWindowWidth(), camera->getWindowHeight());
        viewParams->set3f(planeWidth, planeHeight, camera->nearClipPlane);
        cameraPos->set3f(camera->position);

        localToWorldMatrix->setMatrix4f(camera->getLocalToWorldMatrix());
    }

    void TracingShader::setupUniforms()
    {
        currentFrameTime = accessUniform("currentFrameTime");
        screenParams = accessUniform("screenParams");
        viewParams = accessUniform("viewParams");
        cameraPos = accessUniform("cameraPos");

        localToWorldMatrix = accessUniform("localToWorld");

        numSpheres = accessUniform("numSpheres");
        maxBounces = accessUniform("maxBounces");
        raysPerPixel = accessUniform("raysPerPixel");
    }
}
