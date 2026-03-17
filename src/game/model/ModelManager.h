#pragma once

#include <filesystem>
#include <string>
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>

namespace application
{
    struct alignas(16) Triangle
    {
        glm::vec3 posA; float pad0 = 0;
        glm::vec3 posB; float pad1 = 0;
        glm::vec3 posC; float pad2 = 0;

        glm::vec3 normalA; float pad3 = 0;
        glm::vec3 normalB; float pad4 = 0;
        glm::vec3 normalC;
    };

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

        std::vector<Triangle> getAllTriangles() const { return allTriangles; }
        std::unordered_map<std::string, TriangleMesh> getAllMeshes() const { return allMeshes; }
    private:
        static Triangle parseTriangle(const size_t &indiceIndex, const std::vector<tinyobj::index_t> &indices, const std::vector<tinyobj::real_t> &vertices, const std::vector<tinyobj::real_t> &normals);
        static AABB calculateMeshAABB(const std::vector<Triangle> &triangles);

        std::vector<Triangle> allTriangles;
        std::unordered_map<std::string, TriangleMesh> allMeshes;
    };
}
