#ifndef SG_GRAPHICS_H_
#define SG_GRAPHICS_H_

#include <vulkan/vulkan.h>

/**
* @brief Creates a vulkan instance targeting vulkan 1.0. Doesn't require having installed the vulkan sdk
**/
VkResult sgVkCreateInstance(VkInstance* vkInstance);

/**
* @brief Destroys the passed vulkan instance. Doesn't require having installed the vulkan sdk
**/
void sgVkDestroyInstance(VkInstance vkInstance);

/**
* @brief Destroys the passed vulkan surface. Doesn't require having installed the vulkan sdk
**/
void sgVkDestroySurface(VkInstance vkInstance, VkSurfaceKHR vkSurface);

/**
* @brief Gets a gpu compatible with the passed instance and its graphics queue family. Prioritizes dedicated GPUs. Doesn't require having installed the vulkan sdk
**/
VkResult sgVkGetRecommendedGpu(VkInstance vkInstance, VkSurfaceKHR vkSurface, VkPhysicalDevice* pGpu, uint32_t* pQueueFamilyIndex);


#endif // !SG_GRAPHICS_H_