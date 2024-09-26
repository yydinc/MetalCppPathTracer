#include <metal_stdlib>
#include <metal_raytracing>

using namespace metal;
using metal::raytracing::ray;

#include "PathTracing.h"

float4 fragment fragmentMain( v2f in [[stage_in]],
                             device float4* spheres [[buffer(0)]],
                             device const float4* sphereMaterials [[buffer(1)]],
                             device const UniformsData* uniforms [[buffer(2)]],
                             texture2d<float, access::read_write> lastFrame [[texture(0)]],
                             texture2d<float, access::read_write> currentFrame [[texture(1)]])
{
    const device UniformsData &u = *uniforms;
    
    if(u.frameCount == 0)
    {
        uint2 coord = uint2((uint)(in.uv.x*u.screenSize.x), (uint)(in.uv.y*u.screenSize.y));
        
        lastFrame.write(0, coord);
    }
    
    uint32_t seed = random(in.uv, u.randomSeed.xyz) * ((uint32_t)-1);
    
    float xOff = (randomFloat(seed)-0.5)/u.screenSize.x;
    
    seed = random(seed);

    float yOff = (randomFloat(seed)-0.5)/u.screenSize.y;
    seed = random(seed);

    float3 rayDir = (u.firstPixelPosition +
                     (in.uv.x+xOff) * u.viewportU +
                     (in.uv.y+yOff) * u.viewportV) - u.cameraPosition;
    

    ray r{u.cameraPosition, normalize(rayDir)};
    r.min_distance = 0.0001;
    
    float4 color = rayColor(r, spheres, sphereMaterials, u.sphereCount, seed);
    
    uint2 coord = uint2((uint)(in.uv.x*u.screenSize.x), (uint)(in.uv.y*u.screenSize.y));
     
    uint64_t frameCount = u.frameCount+1;
    
    color += lastFrame.read(coord) * (float)(frameCount-1);
    
    color /= frameCount;
    
    color = clamp(color, 0.0, 1.0);
    
    currentFrame.write(color, coord);

    color.w = 1;
    
    return color;
}


