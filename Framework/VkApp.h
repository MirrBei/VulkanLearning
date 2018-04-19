#pragma once
#include "vulkan/vulkan.hpp"

class VkApp
{
public:
    void init();
    void queryPhysicalDeviceProperties(int physicalDeviceIndex);
    void createLogicDevice(const int physicalDeviceIndex);
    static void queryInstanceLayers();
    void queryDeviceLayers(int physicalDeviceIndex);
    void queryInstanceExtensions();
    void createBuffer();
    void createImage();

private:
    VkInstance instance_ = nullptr;
    std::vector<VkPhysicalDevice> physicalDevices_;
    VkDevice device_;
};
