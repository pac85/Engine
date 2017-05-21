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
