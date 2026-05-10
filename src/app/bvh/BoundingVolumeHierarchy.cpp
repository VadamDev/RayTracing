#include "BoundingVolumeHierarchy.h"

#include <limits>
#include <spdlog/spdlog.h>

namespace application {
    static constexpr int SAH_TEST_PER_AXIS = 5;

    void BoundingVolumeHierarchy::build(std::vector<BVHTriangle> &triangles)
    {
        const auto before = std::chrono::steady_clock::now();

        BVHNode rootNode;
        rootNode.triCount = triangles.size();
        rootNode.growToInclude(triangles);

        nodes.push_back(rootNode);
        splitNode(triangles, 0, 0);
        nodes.shrink_to_fit();

        stats.buildTimeMs = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - before).count() / 1e6f;
        collectStats();
    }

    float BoundingVolumeHierarchy::evaluateSAH(const BVHNode &node, const std::vector<BVHTriangle> &triangles, const int axis, const float splitPos)
    {
        BoundingBox left, right;
        int leftTriCount = 0, rightTriCount = 0;

        for (int i = node.triIndex; i < node.triIndex + node.triCount; i++)
        {
            const BVHTriangle &tri = triangles[i];

            const bool isInLeft = tri.centroid[axis] < splitPos;
            BoundingBox &boxToGrow = isInLeft ? left : right;
            boxToGrow.growToInclude(tri.minPos, tri.maxPos);

            if (isInLeft)
                leftTriCount++;
            else
                rightTriCount++;
        }

        const float cost = left.getArea() * leftTriCount + right.getArea() * rightTriCount;
        return cost > 0 ? cost : std::numeric_limits<float>::infinity(); // A cost of 0 implicitly mean that the split failed
    }

    std::tuple<int, float, float> BoundingVolumeHierarchy::chooseSplit(const BVHNode &node, const std::vector<BVHTriangle> &triangles)
    {
        int bestAxis = 0;
        float bestPos = 0, bestCost = std::numeric_limits<float>::infinity();

        for (int axis = 0; axis < 3; axis++)
        {
            const float boxStart = node.bounds.boxMin[axis];
            const float boxEnd = node.bounds.boxMax[axis];

            for (int i = 1; i <= SAH_TEST_PER_AXIS; i++)
            {
                const float splitT = i / (SAH_TEST_PER_AXIS + 1.0f);
                const float candidatePos = std::lerp(boxStart, boxEnd, splitT);

                const float cost = evaluateSAH(node, triangles, axis, candidatePos);
                if (cost >= bestCost)
                    continue;

                bestAxis = axis;
                bestPos = candidatePos;
                bestCost = cost;
            }
        }

        return std::make_tuple(bestAxis, bestPos, bestCost);
    }

    void BoundingVolumeHierarchy::splitNode(std::vector<BVHTriangle> &triangles, const int parentNodeIdx, const int currentDepth)
    {
        BVHNode &parent = nodes[parentNodeIdx];
        if (currentDepth >= maxDepth || parent.triCount <= 1)
            return;

        const auto [splitAxis, splitPos, sahCost] = chooseSplit(parent, triangles);
        if (sahCost >= parent.getSAHCost())
            return;

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

        int minDepth = std::numeric_limits<int>::max();
        int maxDepth = std::numeric_limits<int>::min();
        findMinMaxDepth(0, 0, minDepth, maxDepth);

        int minTriCount = std::numeric_limits<int>::max();
        int maxTriCount = std::numeric_limits<int>::min();
        for (const auto &node : nodes)
        {
            const int triCount = node.triCount;
            if (triCount <= 0)
                continue;

            stats.triCount += triCount;
            stats.leafNodeCount++;

            if (minTriCount > triCount)
                minTriCount = triCount;

            if (maxTriCount < triCount)
                maxTriCount = triCount;
        }

        stats.minTriCount = minTriCount;
        stats.maxTriCount = maxTriCount;
        stats.minDepth = minDepth;
        stats.maxDepth = maxDepth;
    }

    void BoundingVolumeHierarchy::findMinMaxDepth(const int parentNodeIdx, const int currentDepth, int &minDepth, int &maxDepth)
    {
        const BVHNode &node = nodes[parentNodeIdx];
        if (node.triCount > 0)
        {
            if (currentDepth < minDepth)
                minDepth = currentDepth;

            if (currentDepth > maxDepth)
                maxDepth = currentDepth;

            return;
        }

        findMinMaxDepth(node.leftChildIdx, currentDepth + 1, minDepth, maxDepth);
        findMinMaxDepth(node.leftChildIdx + 1, currentDepth + 1, minDepth, maxDepth);
    }
}
