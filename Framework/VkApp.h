#pragma once
#include "vulkan/vulkan.hpp"

class VkApp
{
public:
    VkResult init();
    void queryPhysicalDeviceProperties(int physicalDeviceIndex);

private:
    VkInstance instance_ = nullptr;
    std::vector<VkPhysicalDevice> physicalDevices_;
};
