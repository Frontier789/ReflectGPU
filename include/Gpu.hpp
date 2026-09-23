#include <stdexcept>
#include <format>
#include <string>
#include <meta>

consteval bool is_device_memory_manager(const std::meta::info &member_type)
{
    if (!std::meta::has_template_arguments(member_type)) {
        return false;
    }
    return std::meta::template_of(member_type) == ^^DeviceMemoryManager;
}

consteval std::meta::info device_memory_manager_type(const std::meta::info &member_type)
{
    auto args = std::meta::template_arguments_of(member_type);

    return args[0];
}

template<typename T>
struct GpuReflector
{
    struct Impl;

    consteval {
        auto ctx = std::meta::access_context::current();
        auto old_members = std::meta::nonstatic_data_members_of(^^T, ctx);
        std::vector<std::meta::info> new_members;
        
        for (const auto member : old_members) {
            auto type = std::meta::type_of(member);
            const auto identifier = std::meta::identifier_of(member);

            if (std::meta::is_arithmetic_type(type)) {}
            else if (is_device_memory_manager(type)) {
                type = std::meta::add_pointer(device_memory_manager_type(type));
            }
            else {
                throw std::runtime_error(
                    std::string("Type '") + std::meta::identifier_of(^^T) +
                    "' cannot be made into a gpu type, because member '" + identifier +
                    "' has unhandled type " + std::meta::display_string_of(type)
                );
            }
            
            std::meta::info new_member = std::meta::data_member_spec(type, {
                .name = identifier
            });
            
            new_members.push_back(new_member);
        }

        std::meta::define_aggregate(^^Impl, new_members);
    }
};

template<size_t N>
constexpr std::array<size_t, N> iota()
{
    std::array<size_t, N> arr;
    for (size_t i=0; i<arr.size(); ++i) {
        arr[i] = i;
    }
    return arr;
}

template<typename T>
using Gpu = GpuReflector<T>::Impl;

template<typename T>
Gpu<T> reflect_on_gpu(T &data)
{
    auto reflected = Gpu<T>{};

    static constexpr auto ctx = std::meta::access_context::current();
    static constexpr auto members = std::define_static_array(
        std::meta::nonstatic_data_members_of(^^T, ctx)
    );
    static constexpr auto members_gpu = std::define_static_array(
        std::meta::nonstatic_data_members_of(^^Gpu<T>, ctx)
    );

    template for (constexpr auto i : iota<members.size()>()) {
        if constexpr (is_device_memory_manager(std::meta::type_of(members[i]))) {
            reflected.[:members_gpu[i]:] = data.[:members[i]:].device_ptr;
        }
        else {
            reflected.[:members_gpu[i]:] = data.[:members[i]:];
        }
    }

    return reflected;
}
