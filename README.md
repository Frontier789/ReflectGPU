# Demo repo for using C++ reflections to reflect SoA types onto the GPU

This repo seeks to answer the question, can I automatically mirror my aggregating types to gpu equivalents using C++ reflections.

The complete write-up is available on my blog: [LINK](https://komaro.me/posts/2026_09_22_reflecting_to_the_gpu)

## Building

As of writing this, only GCC supports reflections on its mainline out of the major C++ compilers. Clang has a [fork](https://github.com/bloomberg/clang-p2996) from Bloomberg with experimental reflection support, and MSVC is completely lacking.

To build the project install g++ version 16, and pass in the appropriate C++ standards flag and enable the reflections feature:
```bash
g++-16 -Iinclude -std=c++26 -freflection main.cpp
```

I also added a super simple cmakelists file so cmake can be used too.

## Background

While working on my [path tracer](https://github.com/Frontier789/DevilRay) I created multiple structs of the form
```C++
struct TriangleMesh
{
    DeviceMemoryManager<Vec2> vertices;
    DeviceMemoryManager<int> indices;

    int triangle_count;
};
```

These types are following a model called [structre of arrays](https://en.wikipedia.org/wiki/AoS_and_SoA) to make memory loads coalesce better. The arrays, such as `points` above, come with some extra metadata, number of elements, cpu mapping, etc.

To pass in all this data to a GPU kernel in CUDA without the unneeded fluff, I found myself writing mirror structs by hand such as:
```C++
struct TriangleMeshGPU
{
    Vec2 *vertices;
    int *indices;

    int triangle_count;
};
```

And conversion functions such as:
```C++
TriangleMeshGPU mirror_to_gpu(TriangleMesh &data)
{
    return TriangleMeshGPU {
        .vertices = data.vertices.device_ptr,
        .indices = data.indices.device_ptr,
        .triangle_count = data.triangle_count,
    };
}
```

A big issue with this is that when I add new members, I have to update not only the GPU version of the struct but also the `mirror` function, in case I forget that, the new data member ends up uninitialized, which can cause weird behaviour or flat out crashes.

## The dream

Wouldn't it be nice to be able to just write
```C++
const auto tris_gpu = mirror_to_gpu(tris);
```

And have the reflections take care of creating the GPU mirror type and copying all members?

Read the rest of my endeavors with reflections on my blog: [LINK](https://komaro.me/posts/2026_09_22_reflecting_to_the_gpu) 

Tl;dr: it's possible to do this with reflections, but your editor won't like it.
