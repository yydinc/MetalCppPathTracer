# MetalCpp Path Tracer

## Purpose

Purpose of this project is to build a path tracer using [Metal](https://developer.apple.com/metal/) API provided by Apple.

## Technical Details

Main goal of this path tracer is to calculate an approximation to [the rendering equation] (https://en.wikipedia.org/wiki/Rendering_equation). This calculation is done via simulating possible paths of the light rays that camera receives and aggregating them. 
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
