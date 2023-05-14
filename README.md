# 3D Renderer

Simple software 3d-renderer written from scratch in C++ using SFML graphics library. Library is used only for primitive actions, such as draw pixels. Renderer supports surface color interpolation, textures mapping (from jpg/jpeg images), Gouraud shading, implements .obj model file parser.

## Setup

To setup after cloning repository use
```
./setup.sh
```

## Launch

Currently available 5 test render targets: test_cube, test_triangles, test_interpolation, test_teapot, test_naruto. To launch them run from the build directory:
```
./<target name>
```

## Examples
![Teapot example](https://github.com/hrustim25/renderer3d/blob/main/examples/teapot_example.gif)
