#ifndef SCATTER_H
#define SCATTER_H

#include <metal_stdlib>
#include <metal_raytracing>

#include "Random.h"
#include "Structs.h"

inline bool mirrorAngle(float refractionIndex, thread const float3 &normal, thread const float3 &rayDir, thread uint32_t &seed)
{
    const float cosTheta = dot(-1*rayDir, normal);
    const float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
    
    auto r0 =  (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0*r0;
    const float reflectance = r0 + (1-r0)*pow((1 - cosTheta),5);

    return ((refractionIndex * sinTheta > 1.0) || (reflectance > randomFloat(seed)));
}

inline void scatter(thread ray &r, thread const intersection &i, float materialType, thread uint32_t &seed)
{
    if(!materialType)
    {
        r.direction = i.normal + normalize(randomFloat3(seed));
    }
    else if(materialType < 0)
    {
        r.direction = reflect(r.direction, i.normal);
    }
    else
    {
        float rIndex = materialType;
        rIndex = i.frontFace ? 1 / rIndex : rIndex;

        r.direction = mirrorAngle(rIndex, i.normal, r.direction, seed) ?
                                    reflect(r.direction, i.normal) :
                                    refract(r.direction, i.normal, rIndex);
    }

    r.direction = normalize(r.direction);
}

#endif
