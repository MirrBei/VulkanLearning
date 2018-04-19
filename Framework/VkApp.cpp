#include "VkApp.h"

void VkApp::init()
{
    VkApplicationInfo appInfo = {};
    VkInstanceCreateInfo instanceCreateInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_0;
    appInfo.applicationVersion = 1;
    appInfo.pApplicationName = "VulkanLearning";

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance_);
    if (result == VK_SUCCESS)
    {
        uint32_t deviceCount = 0;
        result = vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
        if (result == VK_SUCCESS)
        {
            physicalDevices_.resize(deviceCount);
            vkEnumeratePhysicalDevices(instance_, &deviceCount, physicalDevices_.data());
        }
    }
}

void VkApp::queryPhysicalDeviceProperties(const int physicalDeviceIndex)
{
    if (physicalDeviceIndex < 0 || physicalDeviceIndex >= physicalDevices_.size())
        return;
    const VkPhysicalDevice physicalDevice = physicalDevices_[physicalDeviceIndex];
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
}

void VkApp::createLogicDevice(const int physicalDeviceIndex)
{
    if (physicalDeviceIndex < 0 || physicalDeviceIndex >= physicalDevices_.size())
        return;

    const VkPhysicalDevice physicalDevice = physicalDevices_[physicalDeviceIndex];

    // set features
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
    VkPhysicalDeviceFeatures requiredFeatures = {};
    requiredFeatures.multiDrawIndirect = supportedFeatures.multiDrawIndirect;
    requiredFeatures.tessellationShader = VK_TRUE;
    requiredFeatures.geometryShader = VK_TRUE;

    // set create info
    VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.queueCount = 1;
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = &requiredFeatures;

    vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device_);
}

void VkApp::queryInstanceLayers()
{
    uint32_t propertyCount = 0;
    vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
    if (propertyCount > 0)
    {
        std::vector<VkLayerProperties> layerProperties(propertyCount);
        vkEnumerateInstanceLayerProperties(&propertyCount, layerProperties.data());
    }
}

void VkApp::queryDeviceLayers(const int physicalDeviceIndex)
{
    if (physicalDeviceIndex < 0 || physicalDeviceIndex >= physicalDevices_.size())
        return;
    const VkPhysicalDevice physicalDevice = physicalDevices_[physicalDeviceIndex];
    uint32_t propertyCount = 0;
    vkEnumerateDeviceLayerProperties(physicalDevice, &propertyCount, nullptr);
    if (propertyCount > 0)
    {
        std::vector<VkLayerProperties> layerProperties(propertyCount);
        vkEnumerateDeviceLayerProperties(physicalDevice, &propertyCount, layerProperties.data());
    }
}

void VkApp::queryInstanceExtensions()
{
    uint32_t propertyCount;
    const VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
    if (result == VK_SUCCESS && propertyCount > 0)
    {
        std::vector<VkExtensionProperties> instanceExtensionProperties(propertyCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, instanceExtensionProperties.data());
    }
}

void VkApp::createBuffer()
{
    VkBufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = 1024*1024;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkBuffer buffer = VK_NULL_HANDLE;
    vkCreateBuffer(device_, &createInfo, nullptr, &buffer);
}

void VkApp::createImage()
{
    VkImageCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    createInfo.extent = {1024, 1024, 1};
    createInfo.mipLevels = 10;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImage image;
    vkCreateImage(device_, &createInfo, nullptr, &image);
}
