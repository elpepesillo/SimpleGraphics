#ifndef SG_GRAPHICS_H_
#define SG_GRAPHICS_H_

#include <vulkan/vulkan.h>

VkResult sgVkCreateInstance(VkInstance* vkInstance);

void sgVkDestroyInstance(VkInstance vkInstance);

void sgVkDestroySurface(VkInstance vkInstance, VkSurfaceKHR vkSurface);

VkResult sgVkGetRecommendedGpu(VkInstance vkInstance, VkSurfaceKHR vkSurface, uint32_t* pQueueFamilyIndex, VkPhysicalDevice* pGpu);


#endif // !SG_GRAPHICS_H_