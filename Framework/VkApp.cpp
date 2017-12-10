#include "VkApp.h"

VkResult VkApp::init()
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
            result = vkEnumeratePhysicalDevices(instance_, &deviceCount, physicalDevices_.data());
        }
    }
    return result;
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

VkResult VkApp::createLogicDevice(const int physicalDeviceIndex)
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

    VkDevice device;
    const VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);
    return result;
}
