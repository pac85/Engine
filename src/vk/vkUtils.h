#ifndef VKUTILS_H_INCLUDED
#define VKUTILS_H_INCLUDED

#include "../common.h"
#include "vkPhysicalDevice.h"
#include "vkDevice.h"

inline VkVertexInputBindingDescription make_VkVertexInputBindingDescription (uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
{
    VkVertexInputBindingDescription temp = {};
    temp.binding = binding;
    temp.stride = stride;
    temp.inputRate = inputRate;
    return temp;
}


extern uint32_t find_memory_type(vkPhysicalDevice * physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties);

extern void create_buffer(  vkDevice *device,
                            VkDeviceSize size,
                            VkBufferUsageFlags usage,
                            VkMemoryPropertyFlags properties,
                            VDeleter<VkBuffer> &buffer,
                            VDeleter<VkDeviceMemory> &bufferMemory);

//inspired from vulkan-tutorial.com
inline void copy_buffer(vkDevice * device, VkCommandPool command_pool, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size)
{
    //creates a command buffer to do the copy operation
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = command_pool;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device->device, &alloc_info, &command_buffer);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);

    //does the actual copy operation
    VkBufferCopy copy_region = {};
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);
    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(device->queues.transfer.queue, 1, &submit_info, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->queues.transfer.queue);

    vkFreeCommandBuffers(device->device, command_pool, 1, &command_buffer);
}

//creates and fills a  buffer using a staging buffer
inline void create_filled_buffer_staged(  vkDevice *device,
                                          VkCommandPool & command_pool,
                                          VkDeviceSize buffer_size,
                                          void * data, VkBufferUsageFlags usage,
                                          VDeleter<VkBuffer> &buffer,
                                          VDeleter<VkDeviceMemory> &buffer_memory)
{
    VDeleter<VkBuffer> staging_buffer{device->device, vkDestroyBuffer};
    VDeleter<VkDeviceMemory> staging_buffer_memory{device->device, vkFreeMemory};

    create_buffer(device,
                  buffer_size,
                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  staging_buffer,
                  staging_buffer_memory);

    void * staging_memory_ptr;
    vkMapMemory(device->device, staging_buffer_memory, 0, buffer_size, 0, &staging_memory_ptr);
    memcpy(staging_memory_ptr, data, (size_t)buffer_size);
    vkUnmapMemory(device->device, staging_buffer_memory);

    create_buffer(device,
                  buffer_size,
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                  buffer,
                  buffer_memory);

    copy_buffer(device, command_pool, staging_buffer, buffer, buffer_size);
}

#endif // VKUTILS_H_INCLUDED
