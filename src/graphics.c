#include <simple_graphics/graphics.h>
#include <simple_graphics/platform.h>
#include <stdlib.h>

VkResult sgVkCreateInstance(VkInstance* pInstance) {
	const VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = NULL,
		.pApplicationName = NULL,
		.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
		.pEngineName = NULL,
		.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0
	};

	const VkInstanceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = INSTANCE_EXTENSIONS_COUNT,
		.ppEnabledExtensionNames = INSTANCE_EXTENSIONS
	};

	PFN_vkCreateInstance createInstance = (PFN_vkCreateInstance)sgGetVulkanInstanceFunction(NULL, "vkCreateInstance");
	return createInstance(&createInfo, NULL, pInstance);
}

void sgVkDestroyInstance(VkInstance vkInstance) {
	PFN_vkDestroyInstance destroyInstance = (PFN_vkDestroyInstance)sgGetVulkanInstanceFunction(vkInstance, "vkDestroyInstance");
	destroyInstance(vkInstance, NULL);
}

void sgVkDestroySurface(VkInstance vkInstance, VkSurfaceKHR vkSurface) {
	PFN_vkDestroySurfaceKHR destroySurface = (PFN_vkDestroySurfaceKHR)sgGetVulkanInstanceFunction(vkInstance, "vkDestroySurfaceKHR");
	destroySurface(vkInstance, vkSurface, NULL);
}

VkResult sgVkGetRecommendedGpu(VkInstance vkInstance, VkSurfaceKHR vkSurface, VkPhysicalDevice* pGpu, uint32_t* pQueueFamilyIndex) {
	PFN_vkEnumeratePhysicalDevices enumerateGpus = (PFN_vkEnumeratePhysicalDevices)sgGetVulkanInstanceFunction(vkInstance, "vkEnumeratePhysicalDevices");

	uint32_t gpuCount = 0;
	if (enumerateGpus(vkInstance, &gpuCount, NULL) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	if (gpuCount < 1) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkPhysicalDevice* gpus = malloc(sizeof(VkPhysicalDevice) * gpuCount);
	if (!gpus) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	if (enumerateGpus(vkInstance, &gpuCount, gpus) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	PFN_vkGetPhysicalDeviceQueueFamilyProperties getGpuQueueFamilies = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)sgGetVulkanInstanceFunction(vkInstance, "vkGetPhysicalDeviceQueueFamilyProperties");
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR getGpuSurfaceSupport = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)sgGetVulkanInstanceFunction(vkInstance, "vkGetPhysicalDeviceSurfaceSupportKHR");
	PFN_vkGetPhysicalDeviceProperties getGpuProperties = (PFN_vkGetPhysicalDeviceProperties)sgGetVulkanInstanceFunction(vkInstance, "vkGetPhysicalDeviceProperties");

	VkPhysicalDevice recommendedGpu = VK_NULL_HANDLE;
	uint32_t queueFamilyIndex = 0;
	for (uint32_t i = 0; i < gpuCount; i++) {
		VkPhysicalDevice gpu = gpus[i];

		uint32_t gpuQueueFamilyCount = 0;
		getGpuQueueFamilies(gpu, &gpuQueueFamilyCount, NULL);

		VkQueueFamilyProperties* queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * gpuQueueFamilyCount);
		if (!queueFamilies) {
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}
		getGpuQueueFamilies(gpu, &gpuQueueFamilyCount, queueFamilies);

		bool isQueueFamilyFound = false;
		for (uint32_t i = 0; i < gpuQueueFamilyCount; i++) {
			VkQueueFamilyProperties queueFamily = queueFamilies[i];
			VkBool32 isSurfaceSupported = VK_FALSE;
			if (getGpuSurfaceSupport(gpu, i, vkSurface, &isSurfaceSupported) != VK_SUCCESS) {
				continue;
			}

			bool isCapableOfGraphics = queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
			if (isCapableOfGraphics && isSurfaceSupported) {
				isQueueFamilyFound = true;
				recommendedGpu = gpu;
				queueFamilyIndex = i;
				break;
			}

		}
		free(queueFamilies);

		if (!isQueueFamilyFound) {
			continue;
		}

		VkPhysicalDeviceProperties gpuProperties;
		getGpuProperties(gpu, &gpuProperties);

		if (gpuProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			break;
		}

	}
	free(gpus);

	if (recommendedGpu == VK_NULL_HANDLE) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	*pGpu = recommendedGpu;
	return VK_SUCCESS;
}