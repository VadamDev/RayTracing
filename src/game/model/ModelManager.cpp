#include "ModelManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <glm/common.hpp>

#include "tiny_obj_loader.h"

#include <spdlog/spdlog.h>

namespace application
{
    static const std::string MESHES_DIR = "resources/assets/meshes";

    using namespace std::filesystem;
    using namespace tinyobj;

    void ModelManager::loadAll()
    {
        for (const auto &entry : directory_iterator(MESHES_DIR))
        {
            if (!entry.is_regular_file())
                continue;

            const path &path = entry.path();
            if (!path.has_extension() || path.extension() != ".obj")
                continue;

            try { load(path); }
            catch (std::exception &e) //TODO: custom exceptions
            {
                spdlog::error("", e.what());
            }
        }
    }

    struct AABB
    {
        glm::vec3 min, max;
    };

    void ModelManager::load(const path &path)
    {
        const std::string name = path.stem().string();

        ObjReaderConfig readerConfig;
        readerConfig.triangulate = true;
        readerConfig.triangulation_method = "earcut";
        readerConfig.vertex_color = false;

        ObjReader reader;
        if (!reader.ParseFromFile(path.string()))
        {
            std::string error = "Unknown error occurred during model parsing";
            if (!reader.Error().empty())
                error = reader.Error();

            spdlog::error(error); //TODO: custom exceptions
            return;
        }

        auto &attribs = reader.GetAttrib();

        std::vector<Triangle> triangles;
        for (const shape_t &shape : reader.GetShapes())
        {
            const auto &indices = shape.mesh.indices;

            //If input doesn't have triangle faces we are fucked
            for (size_t indiceIndex = 0; indiceIndex < indices.size(); indiceIndex += 3)
                triangles.push_back(parseTriangle(indiceIndex, indices, attribs.vertices, attribs.normals));
        }

        auto [boxMin, boxMax] = calculateMeshAABB(triangles);
        TriangleMesh tri {
            .triIndex = (unsigned int) allTriangles.size(),
            .numTri = (unsigned int) triangles.size(),

            .boxMin = boxMin,
            .boxMax = boxMax
        };

        allTriangles.reserve(allTriangles.size() + triangles.size());
        allTriangles.insert(allTriangles.end(), triangles.begin(), triangles.end());

        allMeshes[name] = tri;
    }

    Triangle ModelManager::parseTriangle(const size_t &indiceIndex, const std::vector<index_t> &indices, const std::vector<real_t> &vertices, const std::vector<real_t> &normals)
    {
        glm::vec3 triPositions[3];
        glm::vec3 triNormals[3];

        for (int i = 0; i < 3; i++)
        {
            const index_t &indice = indices[indiceIndex + i];

            //Vertex Pos
            triPositions[i] = {
                vertices[3 * indice.vertex_index],
                vertices[3 * indice.vertex_index + 1],
                vertices[3 * indice.vertex_index + 2]
            };

            //Vertex Normal
            if (indice.normal_index >= 0)
            {
                triNormals[i] = {
                    normals[3 * indice.normal_index],
                    normals[3 * indice.normal_index + 1],
                    normals[3 * indice.normal_index + 2]
                };
            }
            else
            {
                //Missing normal (⌐■_■)
                triNormals[i] = glm::vec3(0, 1, 0);
            }
        }

        const Triangle tri {
            .posA = triPositions[0],
            .posB = triPositions[1],
            .posC = triPositions[2],

            .normalA = triNormals[0],
            .normalB = triNormals[1],
            .normalC = triNormals[2]
        };

        return tri;
    }

    AABB ModelManager::calculateMeshAABB(const std::vector<Triangle> &triangles)
    {
        AABB aabb {
            .min = glm::vec3(std::numeric_limits<float>::infinity()),
            .max = glm::vec3(-std::numeric_limits<float>::infinity())
        };

        for (auto &tri : triangles)
        {
            aabb.min = glm::min(aabb.min, glm::vec3(tri.posA));
            aabb.min = glm::min(aabb.min, glm::vec3(tri.posB));
            aabb.min = glm::min(aabb.min, glm::vec3(tri.posC));

            aabb.max = glm::max(aabb.max, glm::vec3(tri.posA));
            aabb.max = glm::max(aabb.max, glm::vec3(tri.posB));
            aabb.max = glm::max(aabb.max, glm::vec3(tri.posC));
        }

        spdlog::info("{}, {}, {} | {}, {}, {}", aabb.min.x, aabb.min.y, aabb.min.z, aabb.max.x, aabb.max.y, aabb.max.z);

        return aabb;
    }
}
