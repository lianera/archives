
LtRay
=============

# Introduction #

A simple implemention of ray tracing by using C++.

# Features #

* Shape: sphere, cone, plane, cylinder, disk, triangle, polygon
* Customed particial shape
* Source supported: own format, ply
* Output supported: ppm file, point cloud output(ply),glfw window preview
* Light: direction light, point light, spot light
* Shadow
* Mirror
* Refraction
* Camera: fov, orthogonal view, perspective view
* Grid acceleration
* Transformation: translation, scale, rotation
* Shader: phong, depth map, normal map, global spherical harmonics lighting, albedo map
* Anti-aliasing

# Samples #

![room.png](samples/room.png)
![sample.jpg](samples/sample.jpg)
![bunny.png](samples/bunny.png)
![laurana.jpg](samples/laurana.jpg)

# Build #

Visual Studio 2013

# Run #

`Release\ltray data\world.toml output.png`