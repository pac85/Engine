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
#ifndef VKSWAPCHAIN_H
#define VKSWAPCHAIN_H

#include"../common.h"
#include"vkDeleter.hpp"
#include"vkDevice.h"
#include"vkSurfaceInfo.h"
#include"vkSurface.h"


struct vkSwapChainDesiredProperties
{
    vkSwapChainDesiredProperties(VkSurfaceFormatKHR _surface_format, VkPresentModeKHR _present_mode, VkExtent2D _extent);
    VkSurfaceFormatKHR surface_format;
    VkPresentModeKHR present_mode;
    VkExtent2D extent;

};

class vkSwapChain
{
    public:
        vkSwapChain(vkPhysicalDevice physical_device,
                    vkDevice &device,
                    vkSurface &surface,
                    vkSurfaceInfo surface_info,
                    vkSwapChainDesiredProperties desired_properties,
                    VkImageUsageFlags image_usage);
        virtual ~vkSwapChain();

    protected:

    private:

        VDeleter<VkSwapchainKHR> swap_chain;
        vector<VkImage> swap_chain_images;
        vector<VDeleter<VkImageView>> swap_chain_image_views;

        VkSurfaceFormatKHR surface_format;
        VkPresentModeKHR present_mode;
        VkExtent2D extent;
        uint32_t image_count;

        VkSurfaceFormatKHR get_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats, vkSwapChainDesiredProperties desired_properties);
        VkPresentModeKHR get_present_mode(const std::vector<VkPresentModeKHR> available_present_modes, vkSwapChainDesiredProperties desired_properties);
        VkExtent2D get_2d_extent(const VkSurfaceCapabilitiesKHR& capabilities, vkSwapChainDesiredProperties desired_properties);
        inline uint32_t get_image_count(VkSurfaceCapabilitiesKHR capabilities)
        {
            if (capabilities.maxImageCount > 0 && (capabilities.minImageCount + 1)  > capabilities.maxImageCount)
            {
                return capabilities.maxImageCount;
            }
            else
            {
                return capabilities.minImageCount + 1;
            }
        }

        vector<VDeleter<VkImageView>> create_image_views(vkDevice &device);//creates an image view for each Vkimage of the swapchain
};

#endif // VKSWAPCHAIN_H
