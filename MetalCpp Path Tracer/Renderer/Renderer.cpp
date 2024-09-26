#include "Renderer.h"

#include <simd/simd.h>
#include "Scene.h"
#include "InputSystem.h"
#include "Camera.h"

using namespace MetalCppPathTracer;

struct UniformsData
{
    simd::float3 cameraPosition;

    simd::float2 screenSize;
    
    simd::float3 viewportU;
    simd::float3 viewportV;
    
    simd::float3 firstPixelPosition;
    
    simd::float3 randomSeed;
    
    uint64_t sphereCount;
    
    uint64_t frameCount = 0;
};

inline uint32_t bitm_random()
{
    static uint32_t current_seed = 92407235;
    const uint32_t state = current_seed * 747796405u + 2891336453u;
    const uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state);
    return (current_seed = (word >> 22u) ^ word);
}

inline float randomFloat()
{
    return (float)bitm_random() / (float)std::numeric_limits<uint32_t>::max();
}

Renderer::Renderer( MTL::Device* pDevice )
: _pDevice(pDevice->retain()), _pScene(new Scene(256))
{
    _pCommandQueue = _pDevice->newCommandQueue();
    
    Camera::reset();
    Camera::screenSize = {1280, 720};
    
    buildScene();
    buildShaders();
    buildBuffers();
    buildTextures();
    
    recalculateViewport();
}

Renderer::~Renderer()
{
    _pSphereBuffer->release();
    _pPSO->release();
    _pCommandQueue->release();
    _pDevice->release();
    
    _pSphereMaterialBuffer->release();
    _pUniformsBuffer->release();
    
    for (int i = 0; i < 2; i++)
        _accumulationTargets[i]->release();
    
    delete _pScene;
}

void Renderer::buildShaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Error* pError = nullptr;
    MTL::Library* pLibrary = _pDevice->newDefaultLibrary();
    
    if ( !pLibrary )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    MTL::Function* pVertexFn = pLibrary->newFunction( NS::String::string("vertexMain", UTF8StringEncoding) );
    MTL::Function* pFragFn = pLibrary->newFunction( NS::String::string("fragmentMain", UTF8StringEncoding) );

    MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    pDesc->setVertexFunction(pVertexFn);
    pDesc->setFragmentFunction(pFragFn);
    pDesc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatRGBA16Float);

    _pPSO = _pDevice->newRenderPipelineState( pDesc, &pError );
    if ( !_pPSO )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    pVertexFn->release();
    pFragFn->release();
    pDesc->release();
    pLibrary->release();
}

void Renderer::buildScene()
{
    for(size_t i = 0; i < 50; i++)
    {
        const float radius = randomFloat()*3 + 0.5;
        const simd::float3 position = simd::float3{randomFloat()*100-50, radius, randomFloat()*100-50};
        
        const simd::float3 albedo = {randomFloat(), randomFloat(), randomFloat()};
        const simd::float3 emissionColor = {randomFloat(), randomFloat(), randomFloat()};
        
        const float materialProbability = randomFloat();
        const float materialType = materialProbability < 0.3 ? -1 : (materialProbability < 0.8 ? 0 : 1.5);
        
        const float emissionPower = 0;//materialType == 0 ? simd::max(randomFloat() * 90 - 60, 0.0f) : 0;
        
        _pScene->addEntity({position, radius}, {albedo, materialType, emissionColor, emissionPower});
    }
    
    _pScene->addEntity({{0, -10000, 0}, 10000}, {{0.99, 0.65, 0.01}, 0, {0}, 0});
    _pScene->addEntity({{0, 100, -100}, 40}, {{0.99, 0.65, 0.01}, 0, {0.7, 0.3, 0.1}, 2});
    _pScene->addEntity({{0, 20, -100}, 10}, {{0.99, 0.65, 0.01}, 0, {0.5, 0.8, 0.5}, 20});
}


void Renderer::recalculateViewport()
{
    UniformsData &u = *((UniformsData*)_pUniformsBuffer->contents());
    
    const float radianFov = Camera::verticalFov * (M_PI / 180);
    
    const auto h = simd::tan(radianFov/2);
    
    const auto viewportHeight = 2 * h * Camera::focalLength;
    const auto viewportWidth = viewportHeight * (Camera::screenSize.x / Camera::screenSize.y);
    
    u.cameraPosition = Camera::position;
    
    u.screenSize = Camera::screenSize;

    u.viewportU = viewportWidth * simd::normalize(simd::cross(Camera::forward, Camera::up));
    u.viewportV = viewportHeight * -Camera::up;
    
    u.firstPixelPosition = Camera::position + Camera::forward*Camera::focalLength - u.viewportU/2 - u.viewportV/2;
    
    u.frameCount = 0;
}


void Renderer::buildBuffers()
{
    const size_t sphereCount = _pScene->getEntityCount();
    
    simd::float4 *sphereTransforms = _pScene->createTransformsBuffer();
    simd::float4 *sphereMaterials = _pScene->createMaterialsBuffer();
    
    const size_t spheresDataSize = sphereCount * sizeof( simd::float4 );
    const size_t sphereMaterialsDataSize = 2 * sphereCount * sizeof( simd::float4 );
    const size_t uniformsDataSize = sizeof( UniformsData );

    _pSphereBuffer = _pDevice->newBuffer( spheresDataSize, MTL::ResourceStorageModeManaged );;
    _pSphereMaterialBuffer = _pDevice->newBuffer( sphereMaterialsDataSize, MTL::ResourceStorageModeManaged );
    _pUniformsBuffer = _pDevice->newBuffer( uniformsDataSize, MTL::ResourceStorageModeManaged );

    memcpy(_pSphereBuffer->contents(), sphereTransforms, spheresDataSize );
    memcpy(_pSphereMaterialBuffer->contents(), sphereMaterials, sphereMaterialsDataSize );

    _pSphereBuffer->didModifyRange(NS::Range::Make(0, _pSphereBuffer->length() ) );
    _pSphereMaterialBuffer->didModifyRange(NS::Range::Make(0, _pSphereMaterialBuffer->length() ) );
    _pUniformsBuffer->didModifyRange(NS::Range::Make(0, _pUniformsBuffer->length() ) );
}

void Renderer::buildTextures()
{
    MTL::TextureDescriptor *textureDescriptor = MTL::TextureDescriptor::alloc()->init();

    textureDescriptor->setPixelFormat(MTL::PixelFormat::PixelFormatRGBA32Float);
    textureDescriptor->setTextureType(MTL::TextureType::TextureType2D);
    textureDescriptor->setWidth(Camera::screenSize.x);
    textureDescriptor->setHeight(Camera::screenSize.y);
    textureDescriptor->setStorageMode(MTL::StorageMode::StorageModePrivate);
    textureDescriptor->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);

    for (uint i = 0; i < 2; i++)
        _accumulationTargets[i] = _pDevice->newTexture(textureDescriptor);
}

void Renderer::updateCamera()
{
    const bool cameraUpdated = Camera::transformWithInputs();
    
    if(cameraUpdated) recalculateViewport();
}

void Renderer::updateUniforms()
{
    UniformsData &u = *((UniformsData*)_pUniformsBuffer->contents());
    
    u.frameCount += 1;
    u.randomSeed = {randomFloat(), randomFloat(), randomFloat()};
    u.sphereCount = _pScene->getEntityCount();
    
    updateCamera();
    
    _pUniformsBuffer->didModifyRange(NS::Range::Make(0, _pUniformsBuffer->length() ) );
}

void Renderer::draw( MTK::View* pView )
{
    {
        updateUniforms();
        MTL::Texture *tmp = _accumulationTargets[0];
        _accumulationTargets[0] = _accumulationTargets[1];
        _accumulationTargets[1] = tmp;
    }
    
    {
        NS::AutoreleasePool *pPool = NS::AutoreleasePool::alloc()->init();

        MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();
        MTL::RenderPassDescriptor *pRpd = pView->currentRenderPassDescriptor();
        MTL::RenderCommandEncoder *pEnc = pCmd->renderCommandEncoder(pRpd);

        pEnc->setRenderPipelineState(_pPSO);

        pEnc->setFragmentBuffer(_pSphereBuffer, 0, 0);
        pEnc->setFragmentBuffer(_pSphereMaterialBuffer, 0, 1);
        pEnc->setFragmentBuffer(_pUniformsBuffer, 0, 2);

        pEnc->setFragmentTexture(_accumulationTargets[0], 0);
        pEnc->setFragmentTexture(_accumulationTargets[1], 1);
        pEnc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0),
                             NS::UInteger(6));

        pEnc->endEncoding();
        pCmd->presentDrawable(pView->currentDrawable());
        pCmd->commit();

        pPool->release();
    }

}

void Renderer::drawableSizeWillChange(MTK::View *pView, CGSize size)
{
    for (uint i = 0; i < 2; i++)
        _accumulationTargets[i]->release();
    
    Camera::screenSize = {(float)size.width, (float)size.height};
    
    buildTextures();
    
    recalculateViewport();
}
