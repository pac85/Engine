#include "vkMain.h"

vkMain::vkMain( VkApplicationInfo app_info,
                GLFWwindow * window,
                vector<const char*> device_extensions,
                VkSurfaceFormatKHR surface_format,
                VkPresentModeKHR present_mode,
                VkExtent2D extent)
{

    const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    cout << "creating instance" << endl;
    instance  = new vkInstance(app_info, true, &validationLayers);

    cout << "creating surface" << endl;
    surface = new vkSurface(window, *instance);

    cout << "creating physical device" << endl;
    physical_device = new vkPhysicalDevice(*instance, *surface, &device_extensions);

    cout << "creating logical device" << endl;
    device = new vkDevice(*instance, *physical_device, &device_extensions);


    cout << "creating swapchain" << endl;

    vkSwapChainDesiredProperties dprops(surface_format, present_mode, extent);

    vkSurfaceInfo surface_info(physical_device->physical_device, *surface);

    swap_chain = new vkSwapChain(*physical_device, *device, *surface, surface_info, dprops, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
}


vkMain::~vkMain()
{
    delete(swap_chain);
    delete(device);
    delete(physical_device);
    delete(surface);
    delete(instance);
}

void vkMain::recreate_swap_chain(VkSurfaceFormatKHR surface_format, VkPresentModeKHR present_mode, VkExtent2D extent)
{
    //makes shure that we don't destroy anything that is still being used
    get_device().wait_idle();

    delete(swap_chain);

    //recreates the swap chain
    vkSwapChainDesiredProperties dprops(surface_format, present_mode, extent);
    vkSurfaceInfo surface_info(physical_device->physical_device, *surface);
    swap_chain = new vkSwapChain(*physical_device, *device, *surface, surface_info, dprops, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
}

/*
    get_device().wait_idle();

    delete(instance);
    delete(surface);
    delete(physical_device);
    delete(device);
    delete(swap_chain);

    vkMain(app_info, window, device_extensions, surface_format, present_mode, extent);
*/
