#ifndef RENDERER_H
#define RENDERER_H


#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <simd/simd.h>

#include "Scene.h"

namespace MetalCppPathTracer
{

class Renderer
{
 public:
    Renderer(MTL::Device *pDevice);
    ~Renderer();
    void buildScene();
    void buildShaders();
    void buildBuffers();
    void buildTextures();
    
    void recalculateViewport();
    void updateCamera();
    
    void updateUniforms();
    
    void draw(MTK::View *pView);
    void drawableSizeWillChange(MTK::View *pView, CGSize size);
    
 private:
    MTL::Device *_pDevice;
    MTL::CommandQueue *_pCommandQueue;
    MTL::RenderPipelineState *_pPSO;
    
    MTL::Buffer *_pSphereBuffer;
    MTL::Buffer *_pSphereMaterialBuffer;
    MTL::Buffer *_pUniformsBuffer;
    MTL::Texture *_accumulationTargets[2];
    
    Scene *_pScene;
};

};

#endif  //  RENDERER_H
