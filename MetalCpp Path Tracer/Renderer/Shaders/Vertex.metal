#include "Structs.h"

v2f vertex vertexMain(uint vertexId [[vertex_id]])
{
    constexpr simd::float2 uv[6] = {
       {1.0f, -1.0f},
       {-1.0f, -1.0f},
       {1.0f, 1.0f},

       {1.0f, 1.0f},
       {-1.0f, -1.0f},
       {-1.0f, 1.0f}
   };
    
    v2f o;
    o.position = float4(uv[vertexId], 0.0, 1.0 );
    o.uv = (float2 (uv[vertexId].x, -uv[vertexId].y ) * 0.5) + 0.5;
    return o;
}
