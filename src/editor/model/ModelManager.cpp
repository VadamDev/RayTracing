#include "ModelManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <filesystem>
#include <ranges>
#include <spdlog/spdlog.h>

#include "bvh/BoundingVolumeHierarchy.h"

namespace editor
{
    using namespace std::filesystem;
    using namespace tinyobj;

    /*
     * Raytraced Mesh
     */

    RaytracedMesh::~RaytracedMesh()
    {
        unloadFunc(this);
    }

    /*
     * Model Manager
     */

    ModelManager::~ModelManager()
    {
        destroyed = true;
    }

    void ModelManager::handleMeshUnload(const RaytracedMesh *mesh)
    {
        // Shift node data
        for (size_t i = mesh->nodeOffset + mesh->numNodes; i < allBvhNodes.size(); i++)
        {
            RaytracedBVHNode &node = allBvhNodes[i];

            if (node.triCount > 0)
                node.index -= mesh->numTri;
            else
                node.index -= mesh->numNodes;
        }

        for (const auto &value : std::views::values(assets))
        {
            if (value.expired())
                continue;

            int &nodeOffset = value.lock()->nodeOffset;
            if (nodeOffset <= mesh->nodeOffset)
                continue;

            nodeOffset -= mesh->numNodes;
        }

        allTriangles.erase(allTriangles.begin() + mesh->triOffset, allTriangles.begin() + (mesh->triOffset + mesh->numTri));
        allBvhNodes.erase(allBvhNodes.begin() + mesh->nodeOffset, allBvhNodes.begin() + (mesh->nodeOffset + mesh->numNodes));
    }

    std::shared_ptr<RaytracedMesh> ModelManager::meshLoader(const std::string &path)
    {
        const std::filesystem::path sysPath(path);
        if (path == pathPrefix || !exists(sysPath) || !is_regular_file(sysPath))
            return nullptr; // If the file doesn't even exist

        const std::string name = sysPath.stem().string();
        spdlog::info("Loading {}...", sysPath.filename().string());

        ObjReaderConfig readerConfig;
        //readerConfig.triangulation_method = "earcut";
        readerConfig.vertex_color = false;

        ObjReader reader;
        if (!reader.ParseFromFile(path, readerConfig))
        {
            std::string error = reader.Error();
            if (error.empty())
                error = "Unknown error occurred during model parsing";

            spdlog::error(error);
            return nullptr;
        }

        auto &attribs = reader.GetAttrib();

        std::vector<BVHTriangle> triangles;
        for (const shape_t &shape : reader.GetShapes())
        {
            const auto &indices = shape.mesh.indices;

            //If input doesn't have triangle faces we are fucked
            for (size_t i = 0; i < indices.size(); i += 3)
                triangles.push_back(parseTriangle(i, indices, attribs.vertices, attribs.normals));
        }

        spdlog::info("-> Loaded {} triangles", triangles.size());

        // Build BVH & sort triangles
        BoundingVolumeHierarchy bvh(32);
        bvh.build(triangles);

        const BVHStats &bvhStats = bvh.getStats();
        spdlog::info("-> BVH built in {:.3f}ms, {} nodes ({} leaf | Tri min/max: {}, {}) (Depth min/max {}, {})", bvhStats.buildTimeMs, bvhStats.nodeCount, bvhStats.leafNodeCount, bvhStats.minTriCount, bvhStats.maxTriCount, bvhStats.minDepth, bvhStats.maxDepth);

        // Create shader compatible triangle struct
        std::vector<RaytracedTriangle> raytracedTriangles;
        raytracedTriangles.reserve(triangles.size());
        std::ranges::copy(triangles, std::back_inserter(raytracedTriangles));

        // Insert mesh triangles to the global triangles list
        const int triOffset = allTriangles.size();
        allTriangles.reserve(triOffset + raytracedTriangles.size());
        allTriangles.insert(allTriangles.end(), raytracedTriangles.begin(), raytracedTriangles.end());

        // Create shader compatible bvh node struct
        std::vector<RaytracedBVHNode> raytracedNodes;
        raytracedNodes.reserve(bvh.getNodes().size());

        const int nodeOffset = allBvhNodes.size();
        for (BVHNode node : bvh.getNodes())
        {
            node.leftChildIdx += nodeOffset;
            node.triIndex += triOffset;

            raytracedNodes.push_back(node);
        }

        // Insert bvh nodes to the global nodes list
        allBvhNodes.reserve(nodeOffset + raytracedNodes.size());
        allBvhNodes.insert(allBvhNodes.end(), raytracedNodes.begin(), raytracedNodes.end());

        spdlog::info("-> Done!");

        // TODO: This feels very hacky, The "destroyed" variable is set to true when the ModelManager destructor is called.
        // todo: because during the shutdown sequence, the ModelManager will be die before the meshes it contains calls their destructors.
        const auto unloadFunc = [this](RaytracedMesh *mesh) {
            if (destroyed)
                return;

            this->handleMeshUnload(mesh);
        };

        return std::make_shared<RaytracedMesh>(raytracedTriangles.size(), raytracedNodes.size(), triOffset, nodeOffset, unloadFunc);
    }

    BVHTriangle ModelManager::parseTriangle(const size_t &indiceIndex, const std::vector<index_t> &indices, const std::vector<real_t> &vertices, const std::vector<real_t> &normals)
    {
        glm::vec3 triPositions[3] = {};
        glm::vec3 triNormals[3] = {};

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
