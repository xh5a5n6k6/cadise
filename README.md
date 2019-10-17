# Cadise Renderer
<img src="./gallery/cover-image/lowpoly_Snowman.png">
<p align="center"><i>Snowman Scene Rendered with Cadise (based on Lowpoly Snowman scene by sumo12 from BlendSwap)</i></p>

## Introduction
Cadise is a physically based renderer written in C++17, taking some other renderers ([PBRT-v3](https://github.com/mmp/pbrt-v3), [Photon-v2](https://github.com/TzuChieh/Photon-v2), [Tungsten](https://github.com/tunabrain/tungsten), etc) as references. The main purpose of a renderer is to solve the integral problem described in [rendering equation](https://en.wikipedia.org/wiki/Rendering_equation), which is more often known as light transport simulation.

Cadise is still been developing and more features will be implemented.

[![Build Status](https://travis-ci.com/xh5a5n6k6/cadise.svg?branch=master)](https://travis-ci.com/xh5a5n6k6/cadise)

## Scene Description
Cadise Renderer Scene Description (CRSD) developed by Chia-Yu Chou is a scene description and readable by Cadise renderer. To generate a CRSD file, you can find some example scenes in the `./scene/` folder for reference, or export scenes from other 3D softwares (it currently only supports Blender exporter, please see the following repository for further information).

- [**Cadise for Blender**](https://github.com/xh5a5n6k6/cadise-blender)

## Feature Lists
- ### Rendering Technique
    - Whitted Ray Tracing
    - Unidirectional Path Tracing
    - Unidirectional Path Tracing with Next Event Estimation
- ### Accelerator
    - BVH (equal)
- ### BSDF
    - Lambertian Diffuse
    - Specular Reflection
    - Specular Transmission
    - Perfect Dielectric
    - Blinn-Phong
- ### Primitive
    - Sphere
    - Triangle
    - Rectangle
- ### Light
    - Point Light
    - Area Light (rectangle)
- ### Texture
    - Constant
    - Checkerboard
- ### Camera
    - Pinhole Perspective
- ### Sampling
    - Random
    - Stratified
- ### Reconstruction Filter
    - Box
    - Cone
    - Gaussian
    - Mitchell

## License
Cadise is under the [MIT](https://opensource.org/licenses/MIT) license. 

It also includes some third-party libraries in the `./source/third-party` folder with their own licenses, you can see the `LICENSE.txt` for more details.