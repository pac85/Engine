#include "vkSurface.h"
#include"../settings.h"

vkSurface:: vkSurface(GLFWwindow* window, vkInstance &instance) : surface{instance.instance, vkDestroySurfaceKHR}
{
    if(glfwCreateWindowSurface(instance.instance, window, nullptr, surface.replace()) != VK_SUCCESS )
    {
        cout << "could't create surface" << endl;
    }
}

vkSurface::~vkSurface()
{
    //nothing to do here, VDeleter takes care of everything
}
