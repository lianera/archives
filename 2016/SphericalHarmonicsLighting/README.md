# 球谐光照

| ![](spherical-harmonics-lighting/samples/ball.jpg)  | ![](spherical-harmonics-lighting/samples/bunny.jpg)  | ![](spherical-harmonics-lighting/samples/teapot.jpg)  |
|---|---|---|

## 简介

给定环境贴图，对环境光进行采样，得到数据量很小的环境光参数。利用这些参数对场景或者模型加上环境光照，详细内容参考[Lianera的博客](http://lianera.github.io)

程序包含三个部分：

* [Framework](framework)：框架
* [Spherical Harmonics Sampler](sh_sampler)：球谐光照采样器
* [Spherical Harmonics Lighting](spherical-harmonics-lighting)：球谐光照渲染器

## 环境

* Visual Studio 2015
* 第三方库：OpenGL, Glfw3, Glew, FreeImage