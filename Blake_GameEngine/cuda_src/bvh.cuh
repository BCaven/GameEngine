#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__device__ float3 max(float3 first, float3 second);
__device__ float3 min(float3 first, float3 second);