#include "vk/vkUtils.h"

GLFWwindow* window;

VDeleter<VkInstance> instance {vkDestroyInstance};

VDeleter<VkDevice> device{vkDestroyDevice};

VDeleter<VkSurfaceKHR> surface{instance, vkDestroySurfaceKHR};

VDeleter<VkSwapchainKHR> swap_chain{device, vkDestroySwapchainKHR};

vector<VDeleter<VkImageView>> swapChainImageViews;

bool use_validation_layers;

const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

VkPhysicalDevice physical_device = VK_NULL_HANDLE;

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
VDeleter<VkDebugReportCallbackEXT> callback{instance, DestroyDebugReportCallbackEXT};

int graphics_queue_family_index = -1;
VkQueue graphics_queue;

int present_queue_family_index = -1;
VkQueue present_queue;

vector<const char*> device_extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

vector<VkImage> swapChainImages;

VkFormat swap_chain_image_format;

VkExtent2D swap_chain_extent;

bool check_validation_layer_support()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

std::vector<const char*> getRequiredExtensions()
{
    std::vector<const char*> extensions;

    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++)
    {
        extensions.push_back(glfwExtensions[i]);
    }

    if (use_validation_layers)
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData
)
{
    std::cerr << "validation layer: " << msg << std::endl;

    return VK_FALSE;
}

VkResult create_debug_report_callbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr)
    {
        func(instance, callback, pAllocator);
    }
}

bool select_device(std::vector<VkPhysicalDevice> &devices, VkPhysicalDevice &chosen_device)
{
    bool suitable_dev_found = false;
    unsigned int best_score = 0;
    for (const auto& device : devices)
    {
        unsigned int score = 0;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){score+= 1000;}

        if(!deviceFeatures.geometryShader){score-= 1000000;}

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        /*checks that all the needed queue families are supported */
        bool graphics_found = false, present_found = false;
        int i = 0;
        for(const auto& queueFamily : queueFamilies)
        {
            if (!graphics_found && queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphics_found = true;
                graphics_queue_family_index = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (!present_found && queueFamily.queueCount > 0 && presentSupport)
            {
                present_found = true;
                present_queue_family_index = i;
            }

            if(present_found && graphics_found){break;}//if it finds both there is no point in keeping looking for queue families

            i++;
        }
        if(!(graphics_found && present_found)){continue;}//if it didn't find both cues go to next device

        /*checking for device specific extnsions support*/
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(device_extensions.begin(), device_extensions.end());
        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        if(!requiredExtensions.empty()){continue;}

        /*checks that swap chains are supported*/
        bool swapChainAdequate = false;

        SwapChainSupportDetails swapChainSupport = query_swap_chain_support(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();


        if(score >= best_score)
        {
            chosen_device = device;
            suitable_dev_found = true;
            best_score = score;
        }

        if(!swapChainAdequate){continue;}

    }
    return suitable_dev_found;   //if no device is found
}

void chose_physical_device()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount == 0)
    {
        cerr<<"no device with vulkan support found"<<endl;
        return;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    if(!select_device(devices, physical_device))
    {
        cerr<<"no suitable device found"<<endl;
        return;
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physical_device, &deviceProperties);

    cout << "chosen device:" << deviceProperties.deviceName << endl;
}

void create_logical_device()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {graphics_queue_family_index, present_queue_family_index};

    float queuePriority = 1.0f;
    for (int queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = (uint32_t) queueCreateInfos.size();
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = device_extensions.size();
    createInfo.ppEnabledExtensionNames = device_extensions.data();

    if (use_validation_layers)
    {
        createInfo.enabledLayerCount = validationLayers.size();
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physical_device, &createInfo, nullptr, device.replace()) != VK_SUCCESS)
    {
        cout << "failed to create logical device" << endl;
    }

    vkGetDeviceQueue(device, graphics_queue_family_index, 0, &graphics_queue);
    vkGetDeviceQueue(device, present_queue_family_index, 0, &present_queue);

}

void create_surface()
{
    if(glfwCreateWindowSurface(instance, window, nullptr, surface.replace()) != VK_SUCCESS )
    {
        cout << "could't create surface" << endl;
    }
}

SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR chose_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR chose_prentation_mode(const std::vector<VkPresentModeKHR> availablePresentModes, VkPresentModeKHR preferred_mode)
{
    //tries to find the preferred mode in the given list
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == preferred_mode)
        {
            return availablePresentMode;
        }
    }
    //if it doesn't find the preferred mode it returns VK_PRESENT_MODE_FIFO_KHR (aka vSync) which is always present
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chose_surface_extent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        VkExtent2D actualExtent = {width, height};

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

void create_swap_chain(VkPresentModeKHR preferred_mode, unsigned int width, unsigned int height)
{
    SwapChainSupportDetails swapChainSupport = query_swap_chain_support(physical_device);

    VkSurfaceFormatKHR surfaceFormat = chose_surface_format(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chose_prentation_mode(swapChainSupport.presentModes, preferred_mode);
    VkExtent2D extent = chose_surface_extent(swapChainSupport.capabilities, width, height);

    //may need that information later so it is saved in those variables
    swap_chain_image_format = surfaceFormat.format;
    swap_chain_extent = extent;

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {(uint32_t) graphics_queue_family_index, (uint32_t) present_queue_family_index};

    if (graphics_queue_family_index != present_queue_family_index)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, swap_chain.replace()) != VK_SUCCESS)
    {
        cout<<"unable to create the swap chain"<<endl;
    }

    vkGetSwapchainImagesKHR(device, swap_chain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swap_chain, &imageCount, swapChainImages.data());
}

void create_image_views()
{
    swapChainImageViews.resize(swapChainImages.size(), VDeleter<VkImageView>{device, vkDestroyImageView});

    for (uint32_t i = 0; i < swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swap_chain_image_format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;


        if (vkCreateImageView(device, &createInfo, nullptr, swapChainImageViews[i].replace()) != VK_SUCCESS)
        {
            cout<<"unable to create image views"<<endl;
        }

    }
}

void create_shader_module(string file, VDeleter<VkShaderModule> &shaderModule)
{
    //reads the file
    ifstream source(file, ios::ate | ios::binary);

    if (!source.is_open())
    {
        cout << "cannot open SPIR-V shader " << file << endl;
    }

    size_t fileSize = (size_t) source.tellg();
    vector<char> buffer(fileSize);
    source.seekg(0);
    source.read(buffer.data(), fileSize);

    source.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = fileSize;
    createInfo.pCode = (uint32_t*) buffer.data();

    if (vkCreateShaderModule(device, &createInfo, nullptr, shaderModule.replace()) != VK_SUCCESS)
    {
        cout << "failed to create shader module!"<< endl;
    }
}

void create_graphics_pipeline()
{

}
