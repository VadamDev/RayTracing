#pragma once

#include <filesystem>
#include <string>
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>

#include "../scene/RaytracedObjects.h"

namespace application
{
    struct TriangleMesh
    {
        unsigned int triIndex;
        unsigned int numTri;

        glm::vec3 boxMin, boxMax;
    };

    struct AABB;

    class ModelManager
    {

    public:
        void loadAll();
        void load(const std::filesystem::path &path);

        std::vector<RaytracedTriangle> getAllTriangles() const { return allTriangles; }
        std::unordered_map<std::string, TriangleMesh> getAllMeshes() const { return allMeshes; }
    private:
        static RaytracedTriangle parseTriangle(const size_t &indiceIndex, const std::vector<tinyobj::index_t> &indices, const std::vector<tinyobj::real_t> &vertices, const std::vector<tinyobj::real_t> &normals);
        static AABB calculateMeshAABB(const std::vector<RaytracedTriangle> &triangles);

        std::vector<RaytracedTriangle> allTriangles;
        std::unordered_map<std::string, TriangleMesh> allMeshes;
    };
}
