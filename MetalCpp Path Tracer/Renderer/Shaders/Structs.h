#ifndef STRUCTS_H
#define STRUCTS_H

#include <metal_stdlib>

struct v2f
{
    float4 position [[position]];
    float2 uv;
};

struct intersection
{
    float3 point;
    int sphereId;
    float3 normal;
    bool frontFace;
    float t = INFINITY;
};

struct UniformsData
{
    float3 cameraPosition;

    float2 screenSize;
    
    float3 viewportU;
    float3 viewportV;
    
    float3 firstPixelPosition;
    
    float3 randomSeed;
    
    uint64_t sphereCount;
    
    uint64_t frameCount;
};
#endif
