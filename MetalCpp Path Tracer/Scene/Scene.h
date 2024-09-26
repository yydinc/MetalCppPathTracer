#ifndef SCENE_H
#define SCENE_H

#include "Transform.h"
#include "Material.h"

namespace MetalCppPathTracer {

class Scene
{
 public:
    Scene(size_t capacity) : materials(new Material[capacity]), transforms(new Transform[capacity]) {}
    
    inline size_t addEntity(const Transform &transform, const Material &mat)
    {
        materials[entityCount] = mat;
        transforms[entityCount] = transform;
        return entityCount++;
    }
    
    inline size_t addEntity(const Transform &&transform, const Material &&mat)
    {
        materials[entityCount] = mat;
        transforms[entityCount] = transform;
        return entityCount++;
    }
    
    inline simd::float4 *createMaterialsBuffer()
    {
        simd::float4 *buffer = new simd::float4[2 * entityCount];
        
        for(size_t i = 0; i < entityCount; i++)
        {
            const Material &mat = materials[i];
            
            buffer[2*i] = simd::make_float4(mat.albedo, mat.materialType);
            buffer[2*i + 1] = simd::make_float4(mat.emissionColor, mat.emissionPower);
        }
        
        return buffer;
    }
    
    inline simd::float4 *createTransformsBuffer()
    {
        simd::float4 *buffer = new simd::float4[entityCount];
        
        for(size_t i = 0; i < entityCount; i++)
        {
            const Transform &transform = transforms[i];
            
            buffer[i] = simd::make_float4(transform.position, transform.scale);
        }
        
        return buffer;
    }
    
    inline size_t getEntityCount()
    {
        return entityCount;
    }
    
 private:
    size_t entityCount = 0;
    size_t capacity;
    
    Material *materials;
    Transform *transforms;
};

}

#endif
