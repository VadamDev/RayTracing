#pragma once

#include "BoundingBox.hpp"
#include "BVHTriangle.h"
#include "../scene/RaytracedObjects.h"

namespace application {
    struct BVHNode
    {
        BoundingBox bounds = BoundingBox(glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity()));
        int leftChildIdx = -2;
        int triIndex = 0, triCount = 0;

        void growToInclude(const std::vector<BVHTriangle> &triangles)
        {
            for (int i = triIndex; i < triIndex + triCount; i++)
            {
                const BVHTriangle &tri = triangles[i];
                bounds.growToInclude(tri.minPos, tri.maxPos);
            }
        }

        explicit operator RaytracedBVHNode() const
        {
            return {
                .boxMin = bounds.boxMin,
                .pad0 = 0,
                .boxMax = bounds.boxMax,

                .leftChildIdx = leftChildIdx,
                .triIndex = triIndex,
                .triCount = triCount
            };
        }
    };

    struct BVHStats
    {
        float buildTimeMs = 0;
        int triCount = 0;
        int nodeCount = 0, leafNodeCount = 0;

        int minDepth = 0, maxDepth = 0;
        int minTriCount = 0, maxTriCount = 0;
    };

    class BoundingVolumeHierarchy {

    public:
        explicit BoundingVolumeHierarchy(const int maxDepth)
            : maxDepth(maxDepth) {}

        void build(std::vector<BVHTriangle> &triangles);
        void emplace(const std::vector<BVHTriangle> &triangles, std::vector<RaytracedTriangle> &allTriangles, std::vector<RaytracedBVHNode> &allNodes);

        int getMaxDepth() const { return maxDepth; }
        std::vector<BVHNode>& getNodes() { return nodes; }
        BVHStats getStats() const { return stats; }

    private:
        int maxDepth;
        std::vector<BVHNode> nodes;
        BVHStats stats = {};

        static std::tuple<int, float> chooseSplit(const BoundingBox &bounds);
        void splitNode(std::vector<BVHTriangle> &triangles, int parentNodeIdx, int currentDepth);

        void collectStats();
        void findMinMaxDepth(int parentNodeIdx, int currentDepth, int &minDepth, int &maxDepth);
    };
}
