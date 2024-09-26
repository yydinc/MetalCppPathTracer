#ifndef INTERSECT_H
#define INTERSECT_H

#include <metal_stdlib>
#include <metal_raytracing>

#include "Structs.h"

inline float sphereIntersection(thread const ray &ray, device const float4 &sphere)
{
    const float3 sphereCenter = sphere.xyz;
    const float r = sphere.w;

    float3 originToCenter = sphereCenter - ray.origin;
    float a = length_squared(ray.direction);
    float h = dot(ray.direction, originToCenter);
    float c = length_squared(originToCenter) - r*r;

    float discriminant = h*h - a*c;

    if(discriminant < 0) return INFINITY;

    float sqrtDiscriminant = sqrt(discriminant);

    float root = (h - sqrtDiscriminant) / a;

    if(ray.min_distance > root || ray.max_distance < root)
    {
        root = (h + sqrtDiscriminant) / a;
        if(ray.max_distance >= root || ray.max_distance <= root)
        {
            return INFINITY;;
        }
    }
    
    return root;
}


#endif
