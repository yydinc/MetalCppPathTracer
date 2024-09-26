#ifndef RANDOM_H
#define RANDOM_H

#include <metal_stdlib>

inline uint32_t random(uint32_t current_seed)
{
    uint32_t state = current_seed * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state);
    return (word >> 22u) ^ word;
}

inline float randomFloat(uint32_t seed)
{
    return (float)random(seed) / (float)((uint32_t)-1);
}

inline float3 randomFloat3(uint32_t seed)
{
    const float x = randomFloat(seed)*2-1;
    seed = random(seed);

    const float y = randomFloat(seed)*2-1;
    seed = random(seed);

    const float z = randomFloat(seed)*2-1;
    seed = random(seed);
    
    return {x, y, z};
}

inline float random(float2 uv, float3 random)
{
    return fract(sin(dot(uv, random.xy)) * random.z);
}

inline float random(float2 uv)
{
    return 0.5 + 0.5 * fract(sin(dot(uv, float2(12.9898, 78.233)))* 43758.5453);
}

#endif
