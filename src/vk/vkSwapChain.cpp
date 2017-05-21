#include "vkSwapChain.h"
#include "vkExceptions.h"

//since VkSurfaceFormatKHR doesn't have operator ==
inline bool VkSurfaceFormatKHR_cmp(VkSurfaceFormatKHR a, VkSurfaceFormatKHR b)
{
    return a.colorSpace == b.colorSpace && a.format == b.format;
}

vkSwapChainDesiredProperties::vkSwapChainDesiredProperties(VkSurfaceFormatKHR _surface_format, VkPresentModeKHR _present_mode, VkExtent2D _extent)
{
    surface_format = _surface_format;
    present_mode = _present_mode;
    extent = _extent;
}

vkSwapChain::vkSwapChain(vkPhysicalDevice physical_device,
                         vkDevice &device,
                         vkSurface &surface,
                         vkSurfaceInfo surface_info,
                         vkSwapChainDesiredProperties desired_properties,
                         VkImageUsageFlags image_usage) : swap_chain{device.device, vkDestroySwapchainKHR}
{
    surface_format = get_surface_format(surface_info.formats, desired_properties);
    present_mode = get_present_mode(surface_info.present_modes, desired_properties);
    extent = get_2d_extent(surface_info.capabilities, desired_properties);
    image_count = get_image_count(surface_info.capabilities);

    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface.surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = image_usage;
    //sets the VkSwapchainCreateInfoKHR which are dependent on the queue families indices
    if(physical_device.queue_families.graphics.index != physical_device.queue_families.present.index)
    {
        //if the graphics and the present queue families are the same
        uint32_t queue_family_indices[] =
        {
            (uint32_t) physical_device.queue_families.graphics.index,
            (uint32_t) physical_device.queue_families.present.index
        };
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else
    {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = surface_info.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device.device, &create_info, nullptr, swap_chain.replace()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    //image_count already contains the amount of images in the swapchain but
    //the driver is allowed to create more images
    vkGetSwapchainImagesKHR(device.device, swap_chain, &image_count, nullptr);
    swap_chain_images.resize(image_count);
    vkGetSwapchainImagesKHR(device.device, swap_chain, &image_count, swap_chain_images.data());

    swap_chain_image_views = create_image_views(device);
}

VkSurfaceFormatKHR vkSwapChain::get_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats, vkSwapChainDesiredProperties desired_properties)
{
    //VK_FORMAT_UNDEFINED means tha any format can be used so the function will just return the desired format
    if(available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED)
    {
        return desired_properties.surface_format;
    }

    //otherwise it tries to find the desired format in the list of available formats
    for(auto surface_format : available_formats)
    {
        if(VkSurfaceFormatKHR_cmp(surface_format, desired_properties.surface_format))
        {
            return desired_properties.surface_format;
        }
    }

    //at that point, since the function hasn't still returned, it means
    //that the desired format is not available and throws an error
    throw vkUnavailableFeature("unavailable format");
}

VkPresentModeKHR vkSwapChain::get_present_mode(const std::vector<VkPresentModeKHR> available_present_modes, vkSwapChainDesiredProperties desired_properties)
{
    //tries to find the desired present mode in the list of available present modes
    for(auto present_mode : available_present_modes)
    {
        if(present_mode == desired_properties.present_mode)
        {
            return desired_properties.present_mode;
        }
    }

    //at that point, since the function hasn't still returned, it means
    //that the desired present mode is not available and throws an error
    throw vkUnavailableFeature("unavailable present mode");
}

VkExtent2D vkSwapChain::get_2d_extent(const VkSurfaceCapabilitiesKHR& capabilities, vkSwapChainDesiredProperties desired_properties)
{
    if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    //if currentExtent.width or currentExtent.height are set to the maximum value of uint32
    //the program id free to chose different resolutions than the window's one
    else
    {
        desired_properties.extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, desired_properties.extent.width));
        desired_properties.extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, desired_properties.extent.height));

        return desired_properties.extent;
    }
}

vector<VDeleter<VkImageView>> vkSwapChain::create_image_views(vkDevice &device)
{
    vector<VDeleter<VkImageView>> temp_swap_chain_image_views;
    temp_swap_chain_image_views.resize(swap_chain_images.size(), VDeleter<VkImageView>{device.device, vkDestroyImageView});
    for(unsigned int i = 0;i < temp_swap_chain_image_views.size();i++)
    {
        VkImageViewCreateInfo image_view_info;

        image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_info.pNext = nullptr;
        image_view_info.flags = 0;
        image_view_info.image = swap_chain_images[i];
        image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_info.format = surface_format.format;
        image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_info.subresourceRange.baseMipLevel = 0;
        image_view_info.subresourceRange.levelCount = 1;
        image_view_info.subresourceRange.baseArrayLayer = 0;
        image_view_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device.device, &image_view_info, nullptr, temp_swap_chain_image_views[i].replace()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views!");
        }
    }

    return temp_swap_chain_image_views;
}

vkSwapChain::~vkSwapChain()
{
    //dtor
}


