#ifndef SG_GRAPHICS_H_
#define SG_GRAPHICS_H_

#include <vulkan/vulkan.h>

VkResult sgVkCreateInstance(VkInstance* vkInstance, uint32_t vulkanTargetVersion);

void sgVkDestroyInstance(VkInstance vkInstance);

void sgVkDestroySurface(VkInstance vkInstance, VkSurfaceKHR vkSurface);

VkResult sgVkGetRecommendedGpu(VkInstance vkInstance, uint32_t targetVulkanVersion, VkPhysicalDevice* pGpu);

VkResult sgVkFindQueueFamily(VkInstance vkInstance, VkPhysicalDevice vkGpu, VkSurfaceKHR vkSurface, uint32_t* pQueueFamilyIndex);


#endif // !SG_GRAPHICS_H_