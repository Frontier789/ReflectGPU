#include <cstddef>
#include <cstdint>
#include <new>

template<typename T>
struct DeviceMemoryManager
{
    T *device_ptr;
    size_t num_elem;

    ~DeviceMemoryManager();

    static DeviceMemoryManager create(size_t element_count);
};

// NOTE: for this demo I am using RAM allocations
// for the purpose of simplicity. This could use
// cudaMalloc for example in a real setting.
inline std::byte *alloce_gpu_memory(size_t byte_count, size_t alignment)
{
    return new (std::align_val_t(alignment)) std::byte[byte_count];
}

void free_gpu_memory(std::byte *ptr)
{
    delete[] ptr;
}


template<typename T>
DeviceMemoryManager<T>::~DeviceMemoryManager()
{
    free_gpu_memory(reinterpret_cast<std::byte*>(device_ptr));
}

template<typename T>
DeviceMemoryManager<T> DeviceMemoryManager<T>::create(size_t element_count)
{
    const auto byte_count = sizeof(T) * element_count;
    const auto alignment = alignof(T);

    return DeviceMemoryManager<T>{
        .device_ptr = reinterpret_cast<T*>(alloce_gpu_memory(byte_count, alignment)),
        .num_elem = element_count,
    };
}
