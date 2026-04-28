#include "RaytracingShader.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace application
{
    void RaytracingShader::create()
    {
        ComputeShader::create();

        spheresBuffer.create();
        boxesBuffer.create();
        trianglesBuffer.create();
        bvhNodesBuffer.create();
        meshesBuffer.create();
    }

    void RaytracingShader::sendViewParams(Camera *camera) const
    {
        const float planeHeight = camera->nearClipPlane * tan(glm::radians(camera->fov / 2));
        const float planeWidth = planeHeight * camera->getAspectRatio();

        viewParams->set3f(planeWidth, planeHeight, camera->nearClipPlane);
        localToWorldMatrix->setMatrix4f(camera->getLocalToWorldMatrix());
    }

    void RaytracingShader::setupUniforms()
    {
        frameIndex = accessUniform("frameIndex");
        viewParams = accessUniform("viewParams");
        localToWorldMatrix = accessUniform("localToWorld");
        drawDebugMode = accessUniform("drawDebugMode");
        statsThresholds = accessUniform("statsThresholds");

        accumulate = accessUniform("accumulate");
        maxBounces = accessUniform("maxBounces");
        raysPerPixel = accessUniform("raysPerPixel");
        environmentLight = accessUniform("environmentLight");
        divergeStrength = accessUniform("divergeStrength");
        defocusStrength = accessUniform("defocusStrength");
    }

    void RaytracingShader::updateSpheresBuffer(const std::vector<RaytracedSphere> &spheres)
    {
        spheresBuffer.update(spheres, 0, GL_DYNAMIC_DRAW);
    }

    void RaytracingShader::updateBoxesBuffer(const std::vector<RaytracedBox> &boxes)
    {
        boxesBuffer.update(boxes, 1, GL_DYNAMIC_DRAW);
    }

    void RaytracingShader::updateMeshDataBuffers(const std::vector<RaytracedTriangle> &triangles, const std::vector<RaytracedBVHNode> &bvhNodes)
    {
        trianglesBuffer.update(triangles, 2, GL_DYNAMIC_DRAW);
        bvhNodesBuffer.update(bvhNodes, 3, GL_DYNAMIC_DRAW);
    }

    void RaytracingShader::updateMeshesBuffer(const std::vector<TriangleMeshData> &meshes)
    {
        meshesBuffer.update(meshes, 4, GL_DYNAMIC_DRAW);
    }

    void RaytracingShader::unbindBuffers()
    {
        spheresBuffer.unbind();
        boxesBuffer.unbind();
        trianglesBuffer.unbind();
        bvhNodesBuffer.unbind();
        meshesBuffer.unbind();
    }
}
