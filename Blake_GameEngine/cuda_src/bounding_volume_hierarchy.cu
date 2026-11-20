
__device__ void traverseRecursive( CollisionList& list,
                                   const BVH&     bvh, 
                                   const AABB&    queryAABB,
                                   int            queryObjectIdx,
                                   NodePtr        node)
{