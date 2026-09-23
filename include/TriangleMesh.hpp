#include <DeviceMemoryManager.hpp>

#include <cstring>

struct Vec2
{
    float x;
    float y;
};

struct TriangleMesh
{
    DeviceMemoryManager<Vec2> vertices;
    DeviceMemoryManager<int> indices;

    int triangle_count;

    static TriangleMesh square();
};

inline TriangleMesh TriangleMesh::square()
{
    TriangleMesh m{
        .vertices = DeviceMemoryManager<Vec2>::create(4),
        .indices = DeviceMemoryManager<int>::create(6),
        .triangle_count = 6,
    };

    const Vec2 vertices[4] = {
        Vec2{.x=0, .y=0},
        Vec2{.x=1, .y=0},
        Vec2{.x=1, .y=1},
        Vec2{.x=0, .y=1}
    };

    const int indices[6] = {
        0,1,2, 1,2,3
    };

    // Imitates cudaMemcpy
    std::memcpy(m.vertices.device_ptr, vertices, sizeof(Vec2)*4);
    std::memcpy(m.indices.device_ptr, indices, sizeof(int)*6);

    return m;
}
