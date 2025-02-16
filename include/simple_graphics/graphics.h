#ifndef SG_GRAPHICS_H_
#define SG_GRAPHICS_H_

#include <vulkan/vulkan.h>

VkResult sgVkCreateInstance(VkInstance* vkInstance);

void sgVkDestroyInstance(VkInstance vkInstance);

void sgVkDestroySurface(VkInstance vkInstance, VkSurfaceKHR vkSurface);

#endif // !SG_GRAPHICS_H_