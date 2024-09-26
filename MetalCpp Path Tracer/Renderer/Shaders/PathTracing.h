#ifndef PATH_TRACING_H
#define PATH_TRACING_H

#include <metal_stdlib>
#include <metal_raytracing>

#include "Random.h"
#include "Structs.h"

#include "Intersect.h"
#include "Scatter.h"

inline intersection firstHit(thread const ray &ray, device const float4* spheres, uint32_t sphereCount)
{
    intersection in;

    for(uint32_t i = 0; i < sphereCount; i++)
    {
        float root = sphereIntersection(ray, spheres[i]);
        
        if(root > in.t || root == INFINITY) continue;

        in.sphereId = i;
        in.t = root;
    }

    in.point = ray.origin + (in.t * ray.direction);
    in.normal = (in.point - spheres[in.sphereId].xyz) / spheres[in.sphereId].w;
    
    in.frontFace = dot(in.normal, ray.direction) < 0.0;

    in.normal = in.frontFace ? in.normal : -in.normal;

    return in;
}

inline float4 rayColor(ray ray , device const float4* spheres, device const float4* sphereMaterials, uint32_t sphereCount, thread uint32_t &seed)
{
    constexpr size_t maxRayDepth = 32;

    intersection in;
    
    float4 absorption = float4(1);
    float4 light = float4(0);
    size_t rayDepth = 0;
    
    while(rayDepth < maxRayDepth)
    {
        in = firstHit(ray, spheres, sphereCount);
        
        if(in.t == INFINITY) break;
        
        light += absorption * sphereMaterials[2*in.sphereId+1] * sphereMaterials[2*in.sphereId+1].w;
        
        absorption *= sphereMaterials[2*in.sphereId];
            
        scatter(ray, in, sphereMaterials[2*in.sphereId].w, seed);
        ray.origin = in.point;
        rayDepth++;
    }
    
    if(rayDepth == maxRayDepth) return float4(0);

    return light;
}

#endif
