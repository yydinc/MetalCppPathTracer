#ifndef CAMERA_H
#define CAMERA_H

#include <simd/simd.h>
#include "InputSystem.h"

namespace MetalCppPathTracer {

namespace Camera
{

inline simd::float3 position;
inline simd::float3 forward;
inline simd::float3 up;

inline float verticalFov;
inline float focalLength;
inline simd::float2 screenSize;

inline constexpr float movementSpeed = 0.1;
inline constexpr float rotationSpeed = 0.002;
inline constexpr float zoomSpeed = 0.1;

inline static void reset()
{
    position = {0, 0, 32};
    forward = {0, 0, -1};
    up = {0, 1, 0};
    
    verticalFov = 60.0;
    focalLength = 1.0;
}

inline bool move(simd::float3 movementDirection)
{
    if(simd::length_squared(movementDirection) == 0) return false;
    
    const simd::float3 cameraRight = simd::normalize(simd::cross(forward, simd::float3{0, 1, 0}));
    
    const simd::float3 forwardMovementDirection = simd::cross(simd::float3{0, 1, 0}, cameraRight);
    
    position += movementSpeed * simd::normalize(cameraRight*InputSystem::movementInput.x +
                                                simd::float3{0, 1, 0}*InputSystem::movementInput.y +
                                                forwardMovementDirection*InputSystem::movementInput.z);
    return true;
    
}

inline bool rotate(simd::float2 angles)
{
    if(simd::length_squared(angles) == 0) return false;
    
    simd::float3 cameraRight = simd::cross(forward, simd::float3{0, 1, 0});
    simd::quatf rotationUp = simd::quatf(-InputSystem::rotationInput.y*rotationSpeed, cameraRight);
    Camera::forward = simd::normalize(simd_act(rotationUp, forward));
    
    cameraRight = simd::cross(Camera::forward, simd::float3{0, 1, 0});
    up = simd::normalize(simd::cross(cameraRight, forward));
    simd::quatf rotationRight = simd::quatf(-InputSystem::rotationInput.x*rotationSpeed, up);
    forward = simd::normalize(simd_act(rotationRight, forward));
    
    return true;
}

inline bool zoom(float zoomAmount)
{
    if (zoomAmount == 0) return false;
    
    verticalFov = simd::clamp(verticalFov + zoomAmount * zoomSpeed, 30.0f, 120.0f);
    
    return true;
}

inline bool transformWithInputs()
{
    if(InputSystem::resetInput) reset();
    
    const bool reseted = InputSystem::resetInput;
    const bool moved = move(InputSystem::movementInput);
    const bool rotated = rotate(InputSystem::rotationInput);
    const bool zoomed = zoom(InputSystem::zoomInput);
    
    InputSystem::clearInputs();
    
    const bool transformed =  (reseted || moved || rotated || zoomed);
    
    return transformed;
}
};

};

#endif
