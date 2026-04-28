#pragma once

#include <filesystem>
#include <string>
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <vector>

#include "../bvh/BVHTriangle.h"

namespace application
{
    struct TriangleMesh
    {
        int rootBVHNodeIndex;
    };

    class ModelManager
    {

    public:
        void loadAll();
        void load(const std::filesystem::path &path);

        std::vector<RaytracedTriangle> getAllTriangles() const { return allTriangles; }
        std::vector<RaytracedBVHNode> getAllBVHNodes() const { return allNodes; }

        std::unordered_map<std::string, TriangleMesh> getAllMeshes() const { return allMeshes; }
    private:
        static BVHTriangle parseTriangle(const size_t &indiceIndex, const std::vector<tinyobj::index_t> &indices, const std::vector<tinyobj::real_t> &vertices, const std::vector<tinyobj::real_t> &normals);

        std::vector<RaytracedTriangle> allTriangles;
        std::vector<RaytracedBVHNode> allNodes;

        std::unordered_map<std::string, TriangleMesh> allMeshes;
    };
}
