#include "BoundingVolumeHierarchy.h"

#include <spdlog/spdlog.h>

namespace application {
    void BoundingVolumeHierarchy::build(std::vector<BVHTriangle> &triangles)
    {
        const auto before = std::chrono::steady_clock::now();

        BVHNode rootNode;
        rootNode.triCount = triangles.size();
        rootNode.growToInclude(triangles);

        nodes.push_back(rootNode);
        splitNode(triangles, 0, 0);
        nodes.shrink_to_fit();

        stats.buildTimeMs = duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - before).count() / 1e6f;
        collectStats();
    }

    std::tuple<int, float> BoundingVolumeHierarchy::chooseSplit(const BoundingBox &bounds)
    {
        const glm::vec3 &size = bounds.size;

        const int splitAxis = size.x > std::max(size.y, size.z) ? 0 : size.y > size.z ? 1 : 2;
        const float splitPos = bounds.center[splitAxis];

        return std::make_tuple(splitAxis, splitPos);
    }

    void BoundingVolumeHierarchy::splitNode(std::vector<BVHTriangle> &triangles, const int parentNodeIdx, int currentDepth)
    {
        BVHNode &parent = nodes[parentNodeIdx];
        if (currentDepth >= maxDepth || parent.triCount <= 2)
            return;

        const auto [splitAxis, splitPos] = chooseSplit(parent.bounds);

        int i = parent.triIndex;
        int j = parent.triIndex + parent.triCount - 1;
        while (i <= j)
        {
            if (triangles[i].centroid[splitAxis] < splitPos)
                i++;
            else
                std::swap(triangles[i], triangles[j--]);
        }

        const int leftCount = i - parent.triIndex;
        if (leftCount == 0 || leftCount == parent.triCount)
            return;

        BVHNode leftNode;
        leftNode.triIndex = parent.triIndex;
        leftNode.triCount = leftCount;

        BVHNode rightNode;
        rightNode.triIndex = i;
        rightNode.triCount = parent.triCount - leftCount;

        leftNode.growToInclude(triangles);
        rightNode.growToInclude(triangles);

        const int leftChildIdx = nodes.size();
        parent.leftChildIdx = leftChildIdx;
        parent.triCount = 0;

        nodes.push_back(leftNode);
        nodes.push_back(rightNode);

        splitNode(triangles, leftChildIdx, currentDepth + 1);
        splitNode(triangles, leftChildIdx + 1, currentDepth + 1);
    }

    void BoundingVolumeHierarchy::emplace(const std::vector<BVHTriangle> &triangles, std::vector<RaytracedTriangle> &allTriangles, std::vector<RaytracedBVHNode> &allNodes)
    {
        const int nodeIdxOffset = allNodes.size();
        const int triIdxOffset = allTriangles.size();
        for (BVHNode &node : nodes)
        {
            node.leftChildIdx += nodeIdxOffset;
            node.triIndex += triIdxOffset;
        }

        allTriangles.reserve(allTriangles.size() + triangles.size());
        allTriangles.insert(allTriangles.end(), triangles.begin(), triangles.end());

        allNodes.reserve(allNodes.size() + nodes.size());
        allNodes.insert(allNodes.end(), nodes.begin(), nodes.end());
    }

    void BoundingVolumeHierarchy::collectStats()
    {
        stats.nodeCount = nodes.size();

        for (const auto &node : nodes)
        {
            stats.triCount += node.triCount;

            if (node.triCount > 0)
                stats.leafNodeCount++;
        }
    }
}
