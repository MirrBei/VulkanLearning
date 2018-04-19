#pragma once
#include <vulkan/vulkan.hpp>

class VkAllocator
{
public:
    VkAllocator();
    operator const VkAllocationCallbacks*() const
    {
        return &allocatorCallbacks_;
    }

private:
    VkAllocationCallbacks allocatorCallbacks_{};

    static void* VKAPI_CALL Allocation(void* pUserData, size_t size, size_t alignment,
                                       VkSystemAllocationScope allocationScope);
    static void* VKAPI_CALL Reallocation(void* pUserData, void* pOriginal, size_t size, size_t alignment,
                                         VkSystemAllocationScope allocationScope);
    static void VKAPI_CALL Free(void* pUserData, void* pMemory);

    void* Allocation(size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void* Reallocation(void* pOriginal, size_t size, size_t alignment,
                       VkSystemAllocationScope allocationScope);
    void Free(void* pMemory);
};
