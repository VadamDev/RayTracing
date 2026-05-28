#include "RaytraceShader.h"

namespace editor
{
    void RaytraceShader::create()
    {
        ComputeShader::create();

        spheres.create();
        triangles.create();
        nodes.create();
        meshes.create();
    }

    void RaytraceShader::updateFrameIndex(const unsigned int frameIndex)
    {
        this->frameIndex->set1ui(frameIndex);
    }

    void RaytraceShader::updateViewData(CameraSystem *cameraSystem, const float aspectRatio) const
    {
        if (!cameraSystem->isCameraPresent())
            return;

        const CameraComponent *camera = cameraSystem->getPrimaryCamera().camera;

        const float planeHeight = camera->focalPlane * tan(glm::radians(camera->fov / 2));
        const float planeWidth = planeHeight * aspectRatio;

        viewParams->set3f(planeWidth, planeHeight, camera->focalPlane);
        localToWorldMat->setMatrix4f(cameraSystem->getLocalToWorldMatrix());
    }

    void RaytraceShader::updateRaytracerSettings(const RaytracerSettings &settings) const
    {
        accumulate->setBool(settings.accumulate);
        maxBounces->set1i(settings.maxBounces);
        raysPerPixel->set1i(settings.raysPerPixel);
        environmentLight->setBool(settings.environmentalLight);
        divergeStrength->set1f(settings.divergeStrength);
        defocusStrength->set1f(settings.defocusStrength);
    }

    void RaytraceShader::updateRaytracerDebugSettings(const DebugSettings &debugSettings) const
    {
        drawDebugMode->set1i(static_cast<int>(debugSettings.drawMode));
        statsThresholds->set2f(debugSettings.statsThreshold);
    }

    void RaytraceShader::updateSpheresBuffer(const std::vector<RaytracedSphere> &spheres)
    {
        this->spheres.update(spheres, 0, GL_DYNAMIC_DRAW);
    }

    void RaytraceShader::updateMeshDataBuffers(const std::vector<RaytracedTriangle> &triangles, const std::vector<RaytracedBVHNode> &nodes)
    {
        this->triangles.update(triangles, 1, GL_DYNAMIC_DRAW);
        this->nodes.update(nodes, 2, GL_DYNAMIC_DRAW);
    }

    void RaytraceShader::updateMeshesBuffer(const std::vector<TriangleMeshData> &meshes)
    {
        this->meshes.update(meshes, 3, GL_DYNAMIC_DRAW);
    }

    void RaytraceShader::unbindBuffers()
    {
        spheres.unbind();
        triangles.unbind();
        nodes.unbind();
        meshes.unbind();
    }

    void RaytraceShader::setupUniforms()
    {
        frameIndex = accessUniform("frameIndex");

        viewParams = accessUniform("viewParams");
        localToWorldMat = accessUniform("localToWorld");

        accumulate = accessUniform("accumulate");
        maxBounces = accessUniform("maxBounces");
        raysPerPixel = accessUniform("raysPerPixel");
        environmentLight = accessUniform("environmentLight");
        divergeStrength = accessUniform("divergeStrength");
        defocusStrength = accessUniform("defocusStrength");

        drawDebugMode = accessUniform("drawDebugMode");
        statsThresholds = accessUniform("statsThresholds");
    }
}
