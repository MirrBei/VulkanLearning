#include "VkAllocator.h"

VkAllocator::VkAllocator()
{
    allocatorCallbacks_.pUserData = static_cast<void*>(const_cast<VkAllocator*>(this));
    allocatorCallbacks_.pfnAllocation = &VkAllocator::Allocation;
    allocatorCallbacks_.pfnReallocation = &VkAllocator::Reallocation;
    allocatorCallbacks_.pfnFree = &VkAllocator::Free;
}

void* VkAllocator::Allocation(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return static_cast<VkAllocator*>(pUserData)->Allocation(size, alignment, allocationScope);
}

void* VkAllocator::Reallocation(void* pUserData, void* pOriginal, size_t size, size_t alignment,
                                VkSystemAllocationScope allocationScope)
{
    return static_cast<VkAllocator*>(pUserData)->Reallocation(pOriginal, size, alignment, allocationScope);
}

void VkAllocator::Free(void* pUserData, void* pMemory)
{
    return static_cast<VkAllocator*>(pUserData)->Free(pMemory);
}

void* VkAllocator::Allocation(size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return _aligned_malloc(size, alignment);
}

void* VkAllocator::Reallocation(void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return _aligned_realloc(pOriginal, size, alignment);
}

void VkAllocator::Free(void* pMemory)
{
    _aligned_free(pMemory);
}
