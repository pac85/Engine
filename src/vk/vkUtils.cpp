#include "vkUtils.h"
//Inspired from from vulkan-tutorial.com
uint32_t find_memory_type(vkPhysicalDevice * physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device->physical_device, &memory_properties);

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
    {
        if ((type_filter & (1 << i))&&
            (memory_properties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

//Inspired from from vulkan-tutorial.com
void create_buffer( vkDevice *device,
                    VkDeviceSize size,
                    VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties,
                    VDeleter<VkBuffer> &buffer,
                    VDeleter<VkDeviceMemory> &bufferMemory)
{
    //creates a buffer for the vertices
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    //sets sharingMode based on whether the graphics and transfer queue families are the same
    buffer_info.sharingMode =   device->physical_device->queue_families.transfer.index ==
                                device->physical_device->queue_families.graphics.index ?

                                    VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;

    if (vkCreateBuffer(device->device, &buffer_info, nullptr, buffer.replace()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    //gets the memory requirements for
    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(device->device, buffer, &mem_requirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = mem_requirements.size;
    allocInfo.memoryTypeIndex = find_memory_type(device->physical_device, mem_requirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device->device, &allocInfo, nullptr, bufferMemory.replace()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    //this is needed to tell vulkan that the memory we just allocated is used
    //for the vertex buffer we also created
    vkBindBufferMemory(device->device, buffer, bufferMemory, 0);
}

