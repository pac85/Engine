/**************************************************************************/

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
                                                                  
                                                                      
/**************************************************************************/#ifndef VK_UTILS_H_INCLUDED
#define VK_UTILS_H_INCLUDED

#include "../common.h"
#include"vkDeleter.hpp"

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    vector<VkSurfaceFormatKHR> formats;
    vector<VkPresentModeKHR> presentModes;
};

struct vulkan_device
{
    public:
        VkPhysicalDevice physical_device;
        VDeleter<VkDevice> device;
        int graphics_queue_family_index;
        VkQueue graphics_queue;
        int present_queue_family_index;
        VkQueue present_queue;
};

extern GLFWwindow* window;

extern VDeleter<VkInstance> instance;

extern VDeleter<VkDevice> device;

extern VDeleter<VkSurfaceKHR> surface;

extern VDeleter<VkSwapchainKHR> swap_chain;

extern vector<VDeleter<VkImageView>> swapChainImageViews;

extern bool use_validation_layers;

extern const std::vector<const char*> validationLayers;

extern VkPhysicalDevice physical_device;

extern int graphics_queue_family_index;
extern VkQueue graphics_queue;

extern int present_queue_family_index;
extern VkQueue present_queue;

extern void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
extern VDeleter<VkDebugReportCallbackEXT> callback;

extern vector<const char*> device_extensions;

extern vector<VkImage> swapChainImages;

extern VkFormat swap_chain_image_format;

extern VkExtent2D swap_chain_extent;

extern bool check_validation_layer_support();

extern std::vector<const char*> getRequiredExtensions();

extern VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData
);

extern VkResult create_debug_report_callbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

extern bool select_device(std::vector<VkPhysicalDevice> &devices, VkPhysicalDevice &chosen_device);

extern void chose_physical_device();

extern void create_logical_device();

extern void create_surface();

extern SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);

extern VkSurfaceFormatKHR chose_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats);

extern VkPresentModeKHR chose_prentation_mode(const std::vector<VkPresentModeKHR> availablePresentModes, VkPresentModeKHR preferred_mode);

extern VkExtent2D chose_surface_extent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height);

extern void create_swap_chain(VkPresentModeKHR preferred_modeunsigned, unsigned int width, unsigned int height);

extern void create_image_views();

extern void create_shader_module(string file, VDeleter<VkShaderModule> &shaderModule);

extern void create_graphics_pipeline();

#endif // VK_UTILS_H_INCLUDED


