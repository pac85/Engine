/**************************************************************************/
/*
  Copyright (C) 2017 Antonino Maniscalco (alias pac85)

  This file is part of Engine.

  Engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Engine.  If not, see http://www.gnu.org/licenses/

*/
/**************************************************************************/
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

inline bool has_stencil_component(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

/**

void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (hasStencilComponent(format)) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    } else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    endSingleTimeCommands(commandBuffer);
}

*/

#endif // VKUTILS_H_INCLUDED
