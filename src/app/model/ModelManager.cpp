#include "ModelManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <spdlog/spdlog.h>

#include "../bvh/BoundingVolumeHierarchy.h"

namespace application
{
    static constexpr int BVH_TARGET_DEPTH = 32;
    static const std::string MESHES_DIR = "resources/assets/meshes";

    using namespace std::filesystem;
    using namespace tinyobj;

    void ModelManager::loadAll()
    {
        spdlog::info("Target BVH depth is {}", BVH_TARGET_DEPTH);

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
                spdlog::error("Failed to load model at path {}: {}", path.string(), e.what());
            }
        }
    }

    void ModelManager::load(const path &path)
    {
        const std::string name = path.stem().string();
        spdlog::info("Loading model {}...", path.filename().string());

        ObjReaderConfig readerConfig;
        //readerConfig.triangulation_method = "earcut";
        readerConfig.vertex_color = false;

        ObjReader reader;
        if (!reader.ParseFromFile(path.string()))
        {
            std::string error = "Unknown error occurred during model parsing";
            if (!reader.Error().empty())
                error = reader.Error();

            spdlog::error(error);
            return;
        }

        auto &attribs = reader.GetAttrib();

        std::vector<BVHTriangle> triangles;
        for (const shape_t &shape : reader.GetShapes())
        {
            const auto &indices = shape.mesh.indices;

            //If input doesn't have triangle faces we are fucked
            for (size_t indiceIndex = 0; indiceIndex < indices.size(); indiceIndex += 3)
                triangles.push_back(parseTriangle(indiceIndex, indices, attribs.vertices, attribs.normals));
        }

        // Build bvh, also silently sort the provided triangle vector
        spdlog::info("-> Building BVH for {} triangles...", triangles.size());
        BoundingVolumeHierarchy bvh(BVH_TARGET_DEPTH);
        bvh.build(triangles);

        const int rootBVHNodeIndex = allNodes.size();
        bvh.emplace(triangles, allTriangles, allNodes);

        const BVHStats &bvhStats = bvh.getStats();
        spdlog::info("-> BVH built in {:.3f}ms, {} nodes ({} leaf | Tri min/max: {}, {}) (Depth min/max {}, {})", bvhStats.buildTimeMs, bvhStats.nodeCount, bvhStats.leafNodeCount, bvhStats.minTriCount, bvhStats.maxTriCount, bvhStats.minDepth, bvhStats.maxDepth);
        spdlog::info("-> Loaded {}", path.filename().string());

        allMeshes[name] = {
            .rootBVHNodeIndex = rootBVHNodeIndex
        };
    }

    BVHTriangle ModelManager::parseTriangle(const size_t &indiceIndex, const std::vector<index_t> &indices, const std::vector<real_t> &vertices, const std::vector<real_t> &normals)
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

        const BVHTriangle tri(
            triPositions[0],
            triPositions[1],
            triPositions[2],

            triNormals[0],
            triNormals[1],
            triNormals[2]
        );

        return tri;
    }
}
