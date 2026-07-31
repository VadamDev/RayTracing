#include "ModelManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <filesystem>
#include <ranges>
#include <spdlog/spdlog.h>

#include "../../engine/messenger/Messenger.hpp"
#include "bvh/BoundingVolumeHierarchy.h"
#include "../rendering/RenderingEvents.h"

namespace editor
{
    using namespace std::filesystem;
    using namespace tinyobj;

    static constexpr int MAX_BVH_DEPTH = 32;

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

    // Shift data after the mesh we removed to the left in triangle & node global arrays
    void ModelManager::handleMeshUnload(const RaytracedMesh *mesh)
    {
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

        UpdateMeshesDataBuffersEvent event;
        globalMessenger.dispatch(event);
    }

    std::shared_ptr<RaytracedMesh> ModelManager::meshLoader(const path &p)
    {
        path path = p;
        if (!path.has_extension())
            path.replace_extension(".obj");

        if (!exists(path) || !is_regular_file(path))
            return nullptr; // nullptr is automatically managed by the AssetManager class

        spdlog::info("Loading {}...", path.filename().string());

        // Load triangles in obj file
        ObjReaderConfig readerConfig;
        //readerConfig.triangulation_method = "earcut";
        readerConfig.vertex_color = false;

        ObjReader reader;
        if (!reader.ParseFromFile(path.string(), readerConfig))
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
            triangles.reserve(triangles.size() + shape.mesh.num_face_vertices.size());
            const auto &indices = shape.mesh.indices;

            for (size_t i = 0; i < indices.size(); i += 3)
                triangles.push_back(parseTriangle(i, indices, attribs.vertices, attribs.normals));
        }

        spdlog::info("-> Loaded {} triangles", triangles.size());

        // Build BVH & sort triangles
        BoundingVolumeHierarchy bvh(MAX_BVH_DEPTH);
        bvh.build(triangles);

        const BVHStats &bvhStats = bvh.getStats();
        spdlog::info("-> BVH built in {:.3f}ms, {} nodes ({} leaf | Tri min/max: {}, {}) (Depth min/max {}, {})", bvhStats.buildTimeMs, bvhStats.nodeCount, bvhStats.leafNodeCount, bvhStats.minTriCount, bvhStats.maxTriCount, bvhStats.minDepth, bvhStats.maxDepth);

        // Emplace data inside global triangle & nodes array
        std::vector<RaytracedTriangle> raytracedTriangles;
        std::vector<RaytracedBVHNode> raytracedNodes;
        const auto [triOffset, nodeOffset] = emplaceMesh(triangles, raytracedTriangles, bvh, raytracedNodes);

        spdlog::info("-> Done!");

        // TODO: This feels very hacky, The "destroyed" variable is set to true when the ModelManager destructor is called.
        // todo: because during the shutdown sequence, the ModelManager will be die before the meshes it contains calls their destructors.
        const auto unloadFunc = [this](RaytracedMesh *mesh) {
            if (destroyed)
                return;

            this->handleMeshUnload(mesh);
        };
        
        UpdateMeshesDataBuffersEvent event;
        globalMessenger.dispatch(event);

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

    std::tuple<int, int> ModelManager::emplaceMesh(const std::vector<BVHTriangle> &triangles, std::vector<RaytracedTriangle> &raytracedTriangles, BoundingVolumeHierarchy &bvh, std::vector<RaytracedBVHNode> &raytracedNodes)
    {
        // Shader compatible triangle list
        raytracedTriangles.reserve(triangles.size());
        std::ranges::copy(triangles, std::back_inserter(raytracedTriangles));

        const int triOffset = allTriangles.size();
        allTriangles.reserve(triOffset + raytracedTriangles.size());
        allTriangles.insert(allTriangles.end(), raytracedTriangles.begin(), raytracedTriangles.end());

        // Create shader compatible bvh node struct
        const std::vector<BVHNode> &nodes = bvh.getNodes();
        raytracedNodes.reserve(nodes.size());

        const int nodeOffset = allBvhNodes.size();
        for (BVHNode node : nodes)
        {
            node.leftChildIdx += nodeOffset;
            node.triIndex += triOffset;

            raytracedNodes.push_back(node);
        }

        // Insert bvh nodes to the global nodes list
        allBvhNodes.reserve(nodeOffset + raytracedNodes.size());
        allBvhNodes.insert(allBvhNodes.end(), raytracedNodes.begin(), raytracedNodes.end());

        return std::make_tuple(triOffset, nodeOffset);
    }
}
