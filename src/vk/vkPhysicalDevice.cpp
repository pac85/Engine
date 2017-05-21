#include "vkPhysicalDevice.h"

#include "vkSurfaceInfo.h"

vkPhysicalDevice::vkPhysicalDevice(vkInstance &instance, vkSurface &surface, vector<const char*> * requiered_device_extensions)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance.instance, &deviceCount, nullptr);

    if(deviceCount == 0)
    {
        cerr<<"no device with vulkan support found"<<endl;
        return;
    }

    device_extensions = *requiered_device_extensions;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance.instance, &deviceCount, devices.data());

    if(!select_device(devices, physical_device, &queue_families, surface, requiered_device_extensions))
    {
        cerr<<"no suitable device found"<<endl;
        return;
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physical_device, &deviceProperties);

    cout << "chosen device:" << deviceProperties.deviceName << endl;
}

vkPhysicalDevice::~vkPhysicalDevice()
{
    //dtor
}

//choses the best device it can find, has to be changed to something
// that lets the user chose the hardware to be used
bool vkPhysicalDevice::select_device
(
    std::vector<VkPhysicalDevice> &devices,
    VkPhysicalDevice &chosen_device,
    vkQueueFamilies * queue_families,
    vkSurface &surface,
    vector<const char*> * device_extensions
)
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
                queue_families->graphics.index = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.surface, &presentSupport);

            if (!present_found && queueFamily.queueCount > 0 && presentSupport)
            {
                present_found = true;
                queue_families->present.index = i;
            }

            queue_families->transfer.index = queue_families->graphics.index;

            if(present_found && graphics_found){break;}//if it finds both there is no point in keeping looking for queue families

            i++;
        }
        if(!(graphics_found && present_found)){continue;}//if it didn't find both cues go to next device

        /*checking for device specific extnsions support*/
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(device_extensions->begin(), device_extensions->end());
        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        if(!requiredExtensions.empty()){continue;}

        /*checks that swap chains are supported*/
        bool swapChainAdequate = false;

        vkSurfaceInfo swapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();


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

