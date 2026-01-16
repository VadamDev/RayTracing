#include "TracingShader.h"

namespace game
{
    void TracingShader::sendViewParams(Camera *camera) const
    {
        const float planeHeight = camera->nearClipPlane * glm::tan(glm::radians(camera->fov / 2)) * 2;
        const float planeWidth = planeHeight * camera->getAspectRatio();

        viewParams->set3f(planeWidth, planeHeight, camera->nearClipPlane);
        cameraPos->set3f(camera->position);

        localToWorldMatrix->setMatrix4f(camera->getLocalToWorldMatrix());
    }

    void TracingShader::setupUniforms()
    {
        viewParams = accessUniform("viewParams");
        cameraPos = accessUniform("cameraPos");

        localToWorldMatrix = accessUniform("localToWorld");
    }
}
