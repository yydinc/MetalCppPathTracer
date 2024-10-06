# MetalCpp Path Tracer

## Purpose

Purpose of this project is to build a path tracer using [Metal](https://developer.apple.com/metal/) API provided by Apple.

## Some Render Samples


<img width="1275" alt="A blue sphere, a metal sphere, and a hollow glass sphere on top of a bigger orange sphere" src="https://github.com/user-attachments/assets/87b044b7-2825-4fbd-ac0a-13ac2fb06e59">

### A hollow glass sphere, a blue lambertian sphere, and a metal sphere on top of a bigger orange lambertian sphere with a warm lighting


<img width="1275" alt="Same scene from a different perspective." src="https://github.com/user-attachments/assets/2b0fbc3b-0e5c-4dfe-92e1-f9d216425acb">

### Same scene from a different perspective


![gpu-path-tracer-1](https://github.com/user-attachments/assets/7c0f36a9-3607-4390-85fc-0278dbdd31cd)
### Some randomly generated lambertian, glass, and metal spheres with two different light sources


## Technical Details

Main goal of this path tracer is to calculate an approximation to [the rendering equation](https://en.wikipedia.org/wiki/Rendering_equation). This calculation is done via simulating possible paths of the light rays that camera receives and aggregating them. 
The calculation done for each pixel on the screen is independent of one another. Hence, doing this calculations in parallel is possible. This application utilizes Metal API to program the Apple GPUs for this parallel processing task.

## Features to implement

- Better pseudo-random number generation
- Bounding Volume Hierarchy (BVH)
- Triangle rendering
- Mesh importing
- Bidirectional Reflectance Distribution Function (BRDF)
- Spectral rendering
- Importance sampling


## References

[_Physically Based Rendering: From Theory To Implementation_](https://www.pbr-book.org)

[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

[_The Cherno: Ray Tracing_](https://www.youtube.com/watch?v=gfW1Fhd9u9Q&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl)
