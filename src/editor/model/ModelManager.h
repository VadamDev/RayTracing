#pragma once

#include <tiny_obj_loader.h>

#include "../../engine/assets/AssetManager.h"
#include "../scene/RaytracedObjects.h"
#include "bvh/BoundingVolumeHierarchy.h"

namespace editor
{
    class RaytracedMesh
    {
        friend class ModelManager;

    public:
        explicit RaytracedMesh(const int numTri, const int numNodes, const int triOffset, const int nodeOffset, const std::function<void(RaytracedMesh*)> &unloadFunc)
            : numTri(numTri), numNodes(numNodes), triOffset(triOffset), nodeOffset(nodeOffset), unloadFunc(unloadFunc) {}
        ~RaytracedMesh();

        int getRootBvhNodeIndex() const { return nodeOffset; }

    private:
        int numTri, numNodes;
        int triOffset, nodeOffset;

        std::function<void(RaytracedMesh*)> unloadFunc;
    };

    class ModelManager : public engine::AssetManager<RaytracedMesh>
    {

    public:
        ModelManager()
            : AssetManager("resources/meshes/", [this](const std::string &path) { return meshLoader(path); }) {}
        ~ModelManager();

        void handleMeshUnload(const RaytracedMesh *mesh);

        std::vector<RaytracedTriangle>& getAllTriangles() { return allTriangles; }
        std::vector<RaytracedBVHNode>& getAllBvhNodes() { return allBvhNodes; }

    private:
        std::vector<RaytracedTriangle> allTriangles;
        std::vector<RaytracedBVHNode> allBvhNodes;

        bool destroyed = false;

        std::shared_ptr<RaytracedMesh> meshLoader(const std::string &path);
        static BVHTriangle parseTriangle(const size_t &indiceIndex, const std::vector<tinyobj::index_t> &indices, const std::vector<tinyobj::real_t> &vertices, const std::vector<tinyobj::real_t> &normals);
    };
}
