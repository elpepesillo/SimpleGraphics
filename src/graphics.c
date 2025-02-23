#include <simple_graphics/graphics.h>
#include <simple_graphics/platform.h>
#include <stdlib.h>

VkResult sgVkCreateInstance(VkInstance* pInstance) {
	if (!isSgInitialized) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

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

	PFN_vkCreateInstance createInstance = (PFN_vkCreateInstance)instanceProcAddr(NULL, "vkCreateInstance");
	return createInstance(&createInfo, NULL, pInstance);
}

void sgVkDestroyInstance(VkInstance vkInstance) {
	if (!isSgInitialized) {
		return;
	}

	PFN_vkDestroyInstance destroyInstance = (PFN_vkDestroyInstance)instanceProcAddr(vkInstance, "vkDestroyInstance");
	destroyInstance(vkInstance, NULL);
}

void sgVkDestroySurface(VkInstance vkInstance, VkSurfaceKHR vkSurface) {
	if (!isSgInitialized) {
		return;
	}

	PFN_vkDestroySurfaceKHR destroySurface = (PFN_vkDestroySurfaceKHR)instanceProcAddr(vkInstance, "vkDestroySurfaceKHR");
	destroySurface(vkInstance, vkSurface, NULL);
}

VkResult sgVkGetRecommendedGpu(VkInstance vkInstance, VkPhysicalDevice* pGpu) {
	if (!isSgInitialized) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	PFN_vkEnumeratePhysicalDevices enumerateGpus = (PFN_vkEnumeratePhysicalDevices)instanceProcAddr(vkInstance, "vkEnumeratePhysicalDevices");

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

	PFN_vkGetPhysicalDeviceProperties getGpuProperties = (PFN_vkGetPhysicalDeviceProperties)instanceProcAddr(vkInstance, "vkGetPhysicalDeviceProperties");

	VkPhysicalDevice recommendedGpu = VK_NULL_HANDLE;
	for (uint32_t i = 0; i < gpuCount; i++) {
		recommendedGpu = gpus[i];

		VkPhysicalDeviceProperties gpuProperties;
		getGpuProperties(recommendedGpu, &gpuProperties);

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

VkResult sgVkFindQueueFamily(VkInstance vkInstance, VkPhysicalDevice vkGpu, VkSurfaceKHR vkSurface, uint32_t* pQueueFamilyIndex) {
	if (!isSgInitialized) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	PFN_vkGetPhysicalDeviceQueueFamilyProperties getGpuQueueFamilies = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)instanceProcAddr(vkInstance, "vkGetPhysicalDeviceQueueFamilyProperties");
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR getGpuSurfaceSupport = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)instanceProcAddr(vkInstance, "vkGetPhysicalDeviceSupportKHR");

	uint32_t gpuQueueFamilyCount = 0;
	getGpuQueueFamilies(vkGpu, &gpuQueueFamilyCount, NULL);

	VkQueueFamilyProperties* queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * gpuQueueFamilyCount);
	if (!queueFamilies) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	getGpuQueueFamilies(vkGpu, &gpuQueueFamilyCount, queueFamilies);

	uint32_t queueFamilyIndex = 0;
	bool isQueueFamilyFound = false;
	for (uint32_t i = 0; i < gpuQueueFamilyCount; i++) {
		VkQueueFamilyProperties queueFamily = queueFamilies[i];
		VkBool32 isSurfaceSupported = VK_FALSE;
		if (getGpuSurfaceSupport(vkGpu, i, vkSurface, &isSurfaceSupported) != VK_SUCCESS) {
			continue;
		}

		bool isCapableOfGraphics = queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
		if (isCapableOfGraphics && isSurfaceSupported) {
			isQueueFamilyFound = true;
			queueFamilyIndex = i;
			break;
		}

	}

	if (!isQueueFamilyFound) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	*pQueueFamilyIndex = queueFamilyIndex;
	return VK_SUCCESS;
}