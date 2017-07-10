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
#include "vkInstance.h"

vkInstance::vkInstance(VkApplicationInfo app_info, bool _use_validation_layers, const std::vector<const char*> * _validationLayers)
{
    bUsing_validation_layers = _use_validation_layers, validationLayers = _validationLayers;

    std::vector<const char*> extensions = getRequiredExtensions();

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &app_info;
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    if(bUsing_validation_layers)
    {
        //sets up validation layers
        if(!check_validation_layer_support())
        {
            cerr<<"unavailable validation layers"<<endl;
        }
        createInfo.enabledLayerCount = validationLayers->size();
        createInfo.ppEnabledLayerNames = validationLayers->data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if(vkCreateInstance(&createInfo, nullptr, instance.replace()) != VK_SUCCESS)
    {
        cerr<<"unable to initialize vulkan"<<endl;
    }

    if(bUsing_validation_layers)
    {
        //sets up debug callback
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        createInfo.pfnCallback = debugCallback;

        VkResult Callback_result = create_debug_report_callbackEXT(instance, &createInfo, nullptr, callback.replace());

        if (Callback_result != VK_SUCCESS)
        {
            cerr<<"unable to set up debug callback "<< Callback_result <<endl;
        }
    }
}

vkInstance::~vkInstance()
{
    //dtor
}

bool vkInstance::using_validation_layers()
{
    return bUsing_validation_layers;
}

const std::vector<const char*> * vkInstance::get_validation_layers()
{
    return validationLayers;
}

std::vector<const char*> vkInstance::getRequiredExtensions()
{
    std::vector<const char*> extensions;

    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++)
    {
        extensions.push_back(glfwExtensions[i]);
    }

    if (bUsing_validation_layers)
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

bool vkInstance::check_validation_layer_support()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : *validationLayers)
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

//this is th debug callback, called whenever an an error occurs with the error itself given as a parameter
VKAPI_ATTR VkBool32 VKAPI_CALL vkInstance::debugCallback
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

VkResult vkInstance::create_debug_report_callbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
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

void vkInstance::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr)
    {
        func(instance, callback, pAllocator);
    }
}
