#include <TriangleMesh.hpp>
#include <Gpu.hpp>

#include <iostream>
#include <cassert>

struct Wrong
{
    std::string no_gpu_equivalent;
};

int main()
{
    auto sqr = TriangleMesh::square();
    auto gpu_sqr = reflect_on_gpu(sqr);

    assert(gpu_sqr.vertices == sqr.vertices.device_ptr);
    assert(gpu_sqr.indices == sqr.indices.device_ptr);
    assert(gpu_sqr.triangle_count == sqr.triangle_count);

    std::cout << "Triangles on GPU:" << std::endl;
    for (size_t i=0; i<gpu_sqr.triangle_count; i+=3)
    {
        const auto A = gpu_sqr.vertices[gpu_sqr.indices[i+0]];
        const auto B = gpu_sqr.vertices[gpu_sqr.indices[i+1]];
        const auto C = gpu_sqr.vertices[gpu_sqr.indices[i+2]];

        std::cout << "Triangle #" << i/3 << ":" << std::endl;
        std::cout << "    (" << A.x << "," << A.y << ")\n";
        std::cout << "    (" << B.x << "," << B.y << ")\n";
        std::cout << "    (" << C.x << "," << C.y << ")\n";
    }

    // auto gpu_w = reflect_on_gpu(Wrong{});
}
