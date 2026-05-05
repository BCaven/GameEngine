#include "bvh.cuh"

__device__ bool slabsBoxTest(
    float3 p0, float3 p1,
    float3 rayOrigin, float3 invRayDir,
    float rayTmin, float rayTmax
)
{
    float3 tLower;
    tLower.x = (p0.x - rayOrigin.x) * invRayDir.x;
    tLower.y = (p0.y - rayOrigin.y) * invRayDir.y;
    tLower.z = (p0.z - rayOrigin.z) * invRayDir.z;

    float3 tUpper;
    tUpper.x = (p1.x - rayOrigin.x) * invRayDir.x;
    tUpper.y = (p1.y - rayOrigin.y) * invRayDir.y;
    tUpper.z = (p1.z - rayOrigin.z) * invRayDir.z;
    
    float3 minLU = min(tLower, tUpper);
    float3 maxLU = max(tLower, tUpper);
    float4 tMins = make_float4(minLU.x, minLU.y, minLU.z, rayTmin);
    float4 tMaxes = make_float4(maxLU.x, maxLU.y, maxLU.z, rayTmax);

    return false;
}


__device__ void traverseRecursive(CollisionList& list,
    const BVH& bvh,
    const AABB& queryAABB,
    int            queryObjectIdx,
    NodePtr        node)
{
    // Allocate traversal stack from thread-local memory,
    // and push NULL to indicate that there are no postponed nodes.
    NodePtr stack[64];
    NodePtr* stackPtr = stack;
    *stackPtr++ = NULL; // push

    // Traverse nodes starting from the root.
    NodePtr node = bvh.getRoot();
    do
    {
        // Check each child node for overlap.
        NodePtr childL = bvh.getLeftChild(node);
        NodePtr childR = bvh.getRightChild(node);
        bool overlapL = (checkOverlap(queryAABB,
            bvh.getAABB(childL)));
        bool overlapR = (checkOverlap(queryAABB,
            bvh.getAABB(childR)));

        if (bvh.getRightmostLeafInLeftSubtree(node) <= queryLeaf)
            overlapL = false;

        if (bvh.getRightmostLeafInRightSubtree(node) <= queryLeaf)
            overlapR = false;

        // Query overlaps a leaf node => report collision.
        if (overlapL && bvh.isLeaf(childL))
            list.add(queryObjectIdx, bvh.getObjectIdx(childL));

        if (overlapR && bvh.isLeaf(childR))
            list.add(queryObjectIdx, bvh.getObjectIdx(childR));

        // Query overlaps an internal node => traverse.
        bool traverseL = (overlapL && !bvh.isLeaf(childL));
        bool traverseR = (overlapR && !bvh.isLeaf(childR));

        if (!traverseL && !traverseR)
        {
            node = *--stackPtr; // pop
        }
        else
        {
            node = (traverseL) ? childL : childR;
            if (traverseL && traverseR)
                *stackPtr++ = childR; // push
        }
    } while (node != NULL);
}

__global__ void findPotentialCollisions(
    CollisionList list,
    BVH           bvh)
{
    int idx = threadIdx.x + blockDim.x * blockIdx.x;
    if (idx < bvh.getNumLeaves())
    {
        NodePtr leaf = bvh.getLeaf(idx);
        traverseIterative(list, bvh,
            bvh.getAABB(leaf),
            bvh.getObjectIdx(leaf));
    }
}

// Morton code generation

// Expands a 10-bit integer into 30 bits
// by inserting 2 zeros after each bit.
unsigned int expandBits(unsigned int v)
{
    v = (v * 0x00010001u) & 0xFF0000FFu;
    v = (v * 0x00000101u) & 0x0F00F00Fu;
    v = (v * 0x00000011u) & 0xC30C30C3u;
    v = (v * 0x00000005u) & 0x49249249u;
    return v;
}

// Calculates a 30-bit Morton code for the
// given 3D point located within the unit cube [0,1].
unsigned int morton3D(float x, float y, float z)
{
    x = min(max(x * 1024.0f, 0.0f), 1023.0f);
    y = min(max(y * 1024.0f, 0.0f), 1023.0f);
    z = min(max(z * 1024.0f, 0.0f), 1023.0f);
    unsigned int xx = expandBits((unsigned int)x);
    unsigned int yy = expandBits((unsigned int)y);
    unsigned int zz = expandBits((unsigned int)z);
    return xx * 4 + yy * 2 + zz;
}

Node* generateHierarchy(unsigned int* sortedMortonCodes,
    int* sortedObjectIDs,
    int           first,
    int           last)
{
    // Single object => create a leaf node.

    if (first == last)
        return new LeafNode(&sortedObjectIDs[first]);

    // Determine where to split the range.

    int split = findSplit(sortedMortonCodes, first, last);

    // Process the resulting sub-ranges recursively.

    Node* childA = generateHierarchy(sortedMortonCodes, sortedObjectIDs,
        first, split);
    Node* childB = generateHierarchy(sortedMortonCodes, sortedObjectIDs,
        split + 1, last);
    return new InternalNode(childA, childB);
}

int findSplit(unsigned int* sortedMortonCodes,
    int           first,
    int           last)
{
    // Identical Morton codes => split the range in the middle.

    unsigned int firstCode = sortedMortonCodes[first];
    unsigned int lastCode = sortedMortonCodes[last];

    if (firstCode == lastCode)
        return (first + last) >> 1;

    // Calculate the number of highest bits that are the same
    // for all objects, using the count-leading-zeros intrinsic.

    int commonPrefix = __clz(firstCode ^ lastCode);

    // Use binary search to find where the next bit differs.
    // Specifically, we are looking for the highest object that
    // shares more than commonPrefix bits with the first one.

    int split = first; // initial guess
    int step = last - first;

    do
    {
        step = (step + 1) >> 1; // exponential decrease
        int newSplit = split + step; // proposed new position

        if (newSplit < last)
        {
            unsigned int splitCode = sortedMortonCodes[newSplit];
            int splitPrefix = __clz(firstCode ^ splitCode);
            if (splitPrefix > commonPrefix)
                split = newSplit; // accept proposal
        }
    } while (step > 1);

    return split;
}