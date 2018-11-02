#ifndef VKPP_DEVICE_HH
#define VKPP_DEVICE_HH

#include <vkpp/layer.hh>
#include <vkpp/physical_device.hh>
#include <vkpp/extension.hh>

#include <vkpp/queue.hh>

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace vkpp {
    class Device final {
    public:
        Device(PhysicalDevice& physical_device,
               const std::vector<Layer>& enabled_instance_layers,
               const std::vector<Extension>& required_extensions,
               const PhysicalDevice::Features& required_features);
        ~Device() noexcept;

        Device(Device&& device) noexcept;
        Device& operator=(Device&& device) noexcept;

        friend void swap(Device& lhs, Device& rhs);

        VkDevice& get_handle();

        PhysicalDevice& get_physical_device() const;

        template<typename T> std::vector<T> find(const std::vector<T>& requested,
                                                 const std::vector<T>& available) const;
        std::vector<Extension> find(const std::vector<Extension>& extensions) const;

        const PhysicalDevice::Features& get_enabled_features() const;
        const std::vector<Extension>& get_available_extensions() const;
        const std::vector<Extension>& get_enabled_extensions() const;

        bool has_compute_queue() const;
        bool has_graphics_queue() const;
        bool has_transfer_queue() const;
        bool has_present_queue() const;

        Queue* get_compute_queue();  // Warning: there may or may NOT be a queue of the
        Queue* get_graphics_queue(); // type you want, in that case make sure to check
        Queue* get_transfer_queue(); // if the resulting queue is nullptr. In most cases
        Queue* get_present_queue();  // this this will work, but you should be careful.

    private:
        template<typename T> static std::string collapse(const std::vector<T>& vector);

        void assign_queues();
        void assign_queue(std::int32_t index, Queue** queue);

        std::vector<Extension> enabled_extensions;
        PhysicalDevice::Features enabled_features;

        std::vector<Queue> queues;

        Queue* graphics_queue { nullptr };
        Queue* compute_queue { nullptr };
        Queue* transfer_queue { nullptr };
        Queue* present_queue { nullptr };

        PhysicalDevice* physical_device { nullptr };

        VkDevice handle { VK_NULL_HANDLE };
    };

    template<typename T>
    std::string Device::collapse(const std::vector<T>& vector) {
        std::string element_string;
        for (const auto element : vector)
            element_string.append(element.name + " ");
        return element_string;
    }

    template<typename T>
    std::vector<T> Device::find(const std::vector<T>& requested,
                                const std::vector<T>& available) const {
        std::vector<T> missing_requests;

        for (const auto request : requested) {
            bool request_found { false };
            for (const auto available_request : available) {
                if (request == available_request)
                    request_found = true;
            }

            if (!request_found)
                missing_requests.push_back(request);
        }

        return missing_requests;
    }
}

#endif
