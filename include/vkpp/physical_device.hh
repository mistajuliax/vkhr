#ifndef VKPP_PHYSICAL_DEVICE_HH
#define VKPP_PHYSICAL_DEVICE_HH

#include <vkpp/extension.hh>
#include <vkpp/surface.hh>

#include <vulkan/vulkan.h>

#include <string>
#include <unordered_set>
#include <vector>

namespace vkpp {
    class Surface; // forward.
    class PhysicalDevice final {
    public:
        PhysicalDevice(const VkPhysicalDevice& physical_device);

        operator VkPhysicalDevice() const;

        bool operator==(const PhysicalDevice& other) const;
        bool operator!=(const PhysicalDevice& other) const;

        VkPhysicalDevice& get_handle();

        enum class Type {
            Other = VK_PHYSICAL_DEVICE_TYPE_OTHER,
            IntegratedGpu = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
            DiscreteGpu = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
            VirtualGpu = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
            Cpu = VK_PHYSICAL_DEVICE_TYPE_CPU
        };

        using Features = VkPhysicalDeviceFeatures;

        bool is_discrete_gpu() const;
        bool is_integrated_gpu() const;
        bool is_virtual_gpu() const;

        bool is_gpu() const;

        std::uint32_t get_memory_heap() const;
        VkDeviceSize  get_memory_size() const;

        const VkPhysicalDeviceFeatures& get_features() const;
        const VkPhysicalDeviceMemoryProperties& get_memory_properties() const;
        const std::vector<VkQueueFamilyProperties>& get_queue_family_properties() const;
        const std::vector<Extension>& get_available_extensions() const;
        const VkPhysicalDeviceProperties& get_properties() const;

        bool has_present_queue(Surface& surface) const;
        void assign_present_queue_indices(Surface& surface);

        const std::unordered_set<std::int32_t>& get_queue_family_indices() const;

        bool has_every_queue() const;

        bool has_compute_queue() const;
        bool has_graphics_queue() const;
        bool has_transfer_queue() const;

        std::int32_t get_compute_queue_family_index() const;
        std::int32_t get_graphics_queue_family_index() const;
        std::int32_t get_transfer_queue_family_index() const;
        std::int32_t get_present_queue_family_index() const;

        std::string get_details() const;
        const std::string& get_name() const;
        Type get_type() const;
        std::string get_type_string() const;

    private:
        std::string name;

        Type type;

        VkPhysicalDeviceFeatures features;
        VkPhysicalDeviceMemoryProperties memory_properties;
        VkPhysicalDeviceProperties properties;

        void find_device_memory_heap_index();

        std::vector<VkQueueFamilyProperties> queue_families;

        std::int32_t device_heap_index { -1 };

        void assign_queue_family_indices();
        void locate_queue_family_indices();

        std::int32_t graphics_queue_family_index { -1 };
        std::int32_t compute_queue_family_index  { -1 };
        std::int32_t transfer_queue_family_index { -1 };
        std::int32_t present_queue_family_index  { -1 };

        std::unordered_set<std::int32_t> queue_family_indices;

        std::vector<Extension> available_extensions;

        VkPhysicalDevice handle { VK_NULL_HANDLE };
    };
}

#endif
