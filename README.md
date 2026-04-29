# Cadise Renderer
<img src="./Gallery/CoverImage/living-room.jpg">
<p align="center"><i>Living Room Scene Rendered with Cadise (based on <a href="https://www.blendswap.com/blend/13491">The Modern Living Room</a> scene by Wig42)</i></p>

## Introduction
Cadise is a physically-based renderer written in C++ (up to C++20), taking some other renderers ([PBRT-v3](https://github.com/mmp/pbrt-v3), [Photon-v2](https://github.com/TzuChieh/Photon-v2), [Tungsten](https://github.com/tunabrain/tungsten), etc) as references. The main purpose of a renderer is to solve the integral problem described in [rendering equation](https://en.wikipedia.org/wiki/Rendering_equation), which is more often known as light transport simulation.

Cadise is still been developing and more features will be implemented.
(an overview of features is in the [FEATURE.md](FEATURE.md))

| Master branch | Develop branch |
|     :---:     |      :---:     |
| [![Build Status](https://travis-ci.com/xh5a5n6k6/cadise.svg?branch=master)](https://travis-ci.com/xh5a5n6k6/cadise)| [![Build Status](https://travis-ci.com/xh5a5n6k6/cadise.svg?branch=develop)](https://travis-ci.com/xh5a5n6k6/cadise) |

## Cadise Scene Description
Cadise Scene Description (CSD) developed by Chia-Yu Chou is a scene description readable by Cadise renderer. To generate a CSD file (with .csda file extension, a is short for ascii), you can find some example scenes in the [./Scene/](./Scene/) folder for reference, or export scenes from other 3D softwares. It currently only supports Blender exporter, please see the following repository for further information.

- [**Cadise for Blender**](https://github.com/xh5a5n6k6/cadise-blender)

## License
Cadise is under the [MIT](https://opensource.org/licenses/MIT) license. 

It also includes some third-party libraries in the [./Cadise/Source/ThirdParty/](./Cadise/Source/ThirdParty/) folder with their own licenses, you can see the [THIRDPARTY.txt](THIRDPARTY.txt) for more details.

<img src="./Gallery/Logo/cadise-logo-uhd.jpg">
<p align="center">The Logo of Cadise Renderer</p>

## Gallery
Here are some images rendered with Cadise.

<img src="./Gallery/Image/anger.jpg">
<p align="center">The above image is NOT used for any commercial purposes.</p>
<p align="center"><i>Anger Scene (based on <a href="https://www.blendswap.com/blend/16288">Anger (Furia)</a> scene by hfmm99)</i></p>
<img src="./Gallery/Image/iPhone.jpg">
<p align="center"><i>iPhone Scene (based on <a href="https://www.blendswap.com/blend/18439">iPhone 7 Plus</a> scene by sriniwasjha)</i></p>
<img src="./Gallery/Image/mike.jpg">
<p align="center"><i>Mike Scene (based on <a href="https://www.blendswap.com/blend/22596">Mike Rig</a> scene by lucky3d1)</i></p>
<img src="./Gallery/Image/chess.jpg">
<p align="center"><i>Chess Scene (based on <a href="https://www.blendswap.com/blend/18589">Standard Chess Set</a> scene by MiriamAHoyt; HDRI is <a href="https://hdrihaven.com/hdri/?c=artificial%20light&h=colorful_studio">Colorful Studio</a> by Sergej Majboroda)</i></p>
<img src="./Gallery/Image/pear.jpg">
<p align="center"><i>Pear Scene (based on <a href="https://www.blendswap.com/blend/18747">Pear</a> scene by MZiemys)</i></p>
<img src="./Gallery/Image/snowman.jpg">
<p align="center"><i>Snowman Scene (based on <a href="https://www.blendswap.com/blend/22629">Lowpoly Snowman</a> scene by sumo12)</i></p>
<img src="./Gallery/Image/cornell-box-sphere.jpg">
<p align="center"><i>Cornell Box Scene (based on original <a href="https://www.graphics.cornell.edu/online/box/data.html">Cornell Box Data</a>)</i></p>