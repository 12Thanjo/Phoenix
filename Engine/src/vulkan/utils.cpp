#include "utils.h"

#include "Logging.h"


#include <format>

namespace ph{
	namespace vulkan{
		
		auto result_fail(VkResult result, const char* function) noexcept -> bool {
			if(result == VK_SUCCESS) return false;

			#if defined(PH_CONFIG_DEBUG)
				#define PRINT_RESULT_ERROR_MESSAGE(code, description) PH_ERROR(std::format("{}() | {} : {}", function, code, description))
			#else
				#define PRINT_RESULT_ERROR_MESSAGE(code, description) PH_ERROR(std::format("{}() | {}", function, code))
			#endif

			switch(result){
				case VK_NOT_READY: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_NOT_READY",
						"A fence or query has not yet completed"
					);
				} break;

				case VK_TIMEOUT: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_TIMEOUT",
						"A wait operation has not completed in the specified time"
					);
				} break;

				case VK_EVENT_SET: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_EVENT_SET",
						"An event is signaled"
					);
				} break;

				case VK_EVENT_RESET: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_EVENT_RESET",
						"An event is unsignaled"
					);
				} break;

				case VK_INCOMPLETE: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_INCOMPLETE",
						"A return array was too small for the result"
					);
				} break;

				case VK_ERROR_OUT_OF_HOST_MEMORY: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_OUT_OF_HOST_MEMORY",
						"A host memory allocation has failed."
					);
				} break;

				case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_OUT_OF_DEVICE_MEMORY",
						"A device memory allocation has failed."
					);
				} break;

				case VK_ERROR_INITIALIZATION_FAILED: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_INITIALIZATION_FAILED",
						"Initialization of an object could not be completed for implementation-specific reasons."
					);
				} break;

				case VK_ERROR_DEVICE_LOST: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_DEVICE_LOST",
						"The logical or physical device has been lost. See Lost Device"
					);
				} break;

				case VK_ERROR_MEMORY_MAP_FAILED: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_MEMORY_MAP_FAILED",
						"Mapping of a memory object has failed."
					);
				} break;

				case VK_ERROR_LAYER_NOT_PRESENT: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_LAYER_NOT_PRESENT",
						"A requested layer is not present or could not be loaded."
					);
				} break;

				case VK_ERROR_EXTENSION_NOT_PRESENT: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_EXTENSION_NOT_PRESENT",
						"A requested extension is not supported."
					);
				} break;

				case VK_ERROR_FEATURE_NOT_PRESENT: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_FEATURE_NOT_PRESENT",
						"A requested feature is not supported."
					);
				} break;

				case VK_ERROR_INCOMPATIBLE_DRIVER: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_INCOMPATIBLE_DRIVER",
						"The requested version of Vulkan is not supported by the driver "
							"or is otherwise incompatible for implementation-specific reasons."
					);
				} break;

				case VK_ERROR_TOO_MANY_OBJECTS: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_TOO_MANY_OBJECTS",
						"Too many objects of the type have already been created."
					);
				} break;

				case VK_ERROR_FORMAT_NOT_SUPPORTED: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_FORMAT_NOT_SUPPORTED",
						"A requested format is not supported on this device."
					);
				} break;

				case VK_ERROR_FRAGMENTED_POOL: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_FRAGMENTED_POOL",
						"A pool allocation has failed due to fragmentation of the pool’s memory."
							"This must only be returned if no attempt to allocate host or device memory was made to accommodate the new allocation."
							" This should be returned in preference to VK_ERROR_OUT_OF_POOL_MEMORY, "
							"but only if the implementation is certain that the pool allocation failure was due to fragmentation."
					);
				} break;

				case VK_ERROR_UNKNOWN: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_UNKNOWN",
						"An unknown error has occurred; either the application has provided invalid input, or an implementation failure has occurred."
					);
				} break;

				case VK_ERROR_OUT_OF_POOL_MEMORY: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_OUT_OF_POOL_MEMORY",
						"A pool memory allocation has failed. This must only be returned if no attempt to allocate host "
							"or device memory was made to accommodate the new allocation. "
							"If the failure was definitely due to fragmentation of the pool, VK_ERROR_FRAGMENTED_POOL should be returned instead."
					);
				} break;

				case VK_ERROR_INVALID_EXTERNAL_HANDLE: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_INVALID_EXTERNAL_HANDLE",
						"An external handle is not a valid handle of the specified type."
					);
				} break;

				case VK_ERROR_FRAGMENTATION: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_FRAGMENTATION",
						"A descriptor pool creation has failed due to fragmentation."
					);
				} break;

				case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS",
						"A buffer creation or memory allocation failed because the requested address is not available. "
							"A shader group handle assignment failed because the requested shader group handle information is no longer valid."
					);
				} break;

				case VK_PIPELINE_COMPILE_REQUIRED: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_PIPELINE_COMPILE_REQUIRED",
						"A requested pipeline creation would have required compilation, "
							"but the application requested compilation to not be performed."
					);
				} break;

				case VK_ERROR_SURFACE_LOST_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_SURFACE_LOST_KHR",
						"A surface is no longer available."
					);
				} break;

				case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_NATIVE_WINDOW_IN_USE_KHR",
						"The requested window is already in use by Vulkan or another API in a manner which prevents it from being used again."
					);
				} break;

				case VK_SUBOPTIMAL_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_SUBOPTIMAL_KHR",
						"A swapchain no longer matches the surface properties exactly, but can still be used to present to the surface successfully."
					);
				} break;

				case VK_ERROR_OUT_OF_DATE_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_OUT_OF_DATE_KHR",
						"A surface has changed in such a way that it is no longer compatible with the swapchain, "
							"and further presentation requests using the swapchain will fail. "
							"Applications must query the new surface properties and recreate their swapchain "
							"if they wish to continue presenting to the surface."
					);
				} break;

				case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_INCOMPATIBLE_DISPLAY_KHR",
						"The display used by a swapchain does not use the same presentable image layout, "
							"or is incompatible in a way that prevents sharing an image."
					);
				} break;

				case VK_ERROR_VALIDATION_FAILED_EXT: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_VALIDATION_FAILED_EXT",
						"A command failed because invalid usage was detected by the implementation or a validation-layer."
					);
				} break;

				case VK_ERROR_INVALID_SHADER_NV: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_INVALID_SHADER_NV",
						"One or more shaders failed to compile or link. "
							"More details are reported back to the application via VK_EXT_debug_report if enabled."
					);
				} break;

				case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR",
						"The requested VkImageUsageFlags are not supported."
					);
				} break;

				case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR",
						"The requested video picture layout is not supported."
					);
				} break;

				case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR",
						"A video profile operation specified via VkVideoProfileInfoKHR::videoCodecOperation is not supported."
					);
				} break;

				case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR",
						"Format parameters in a requested VkVideoProfileInfoKHR chain are not supported."
					);
				} break;

				case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR",
						"Codec-specific parameters in a requested VkVideoProfileInfoKHR chain are not supported."
					);
				} break;

				case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR",
						"The specified video Std header version is not supported."
					);
				} break;

				case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT",
						"https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_EXT_image_drm_format_modifier.html"
					);
				} break;

				case VK_ERROR_NOT_PERMITTED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_NOT_PERMITTED_KHR",
						"https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#VK_KHR_global_priority"
					);
				} break;

				case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT",
						"An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed "
							"as it did not have exclusive full-screen access. "
							"This may occur due to implementation-dependent reasons, outside of the application’s control."
					);
				} break;

				case VK_THREAD_IDLE_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_THREAD_IDLE_KHR",
						"A deferred operation is not complete but there is currently no work for this thread to do at the time of this call."
					);
				} break;

				case VK_THREAD_DONE_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_THREAD_DONE_KHR",
						"A deferred operation is not complete but there is no work remaining to assign to additional threads."
					);
				} break;

				case VK_OPERATION_DEFERRED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_OPERATION_DEFERRED_KHR",
						"A deferred operation was requested and at least some of the work was deferred."
					);
				} break;

				case VK_OPERATION_NOT_DEFERRED_KHR: {
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_OPERATION_NOT_DEFERRED_KHR",
						"A deferred operation was requested and no operations were deferred."
					);
				} break;

				#if defined(VK_ENABLE_BETA_EXTENSIONS)
					case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: {
						PRINT_RESULT_ERROR_MESSAGE(
							"VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR",
							"The specified Video Std parameters do not adhere to the syntactic "
								"or semantic requirements of the used video compression standard, "
								"or values derived from parameters according to the rules defined by the used video compression standard "
								"do not adhere to the capabilities of the video compression standard or the implementation."
						);
					} break;
				#endif

				case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:{
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_COMPRESSION_EXHAUSTED_EXT",
						"An image creation failed because internal resources required for compression are exhausted. "
						"This must only be returned when fixed-rate compression is requested."
					);
				} break;

				case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:{
					PRINT_RESULT_ERROR_MESSAGE(
						"VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT",
						"The provided binary shader code is not compatible with this device."
					);
				} break;

			};

			return true;
		};




		auto find_memory_type(
			const VkPhysicalDevice& physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties
		) noexcept -> std::optional<uint32_t> {
			
			VkPhysicalDeviceMemoryProperties memory_properties;
			vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

			for(uint32_t i = 0; i < memory_properties.memoryTypeCount; i++){
				if( ((type_filter & (1 << i)) != 0) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties ){
					return i;
				}
			}

			PH_FATAL("Failed to find compatable Vulkan memory type");
			return std::nullopt;
		};



		auto find_queue_families(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface) noexcept -> QueueFamilyIndices {
			auto indices = QueueFamilyIndices{};

			uint32_t queue_family_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

			auto queue_families = std::vector<VkQueueFamilyProperties>(queue_family_count);
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());


			// TODO: maybe better picking function?
			for(int i = 0; i < queue_families.size(); i+=1){
				// graphics
				if(queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
					indices.graphics_family = i;
				}

				// transfer
				if(queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT){
					indices.transfer_family = i;
				}

				// present
				VkBool32 supports_present = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &supports_present);
				if(supports_present){
					indices.present_family = i;
				}

				if(indices.is_complete()){
					break;
				}
			}

			return indices;
		};



		auto print_physical_device_properties(const VkPhysicalDevice& physical_device) noexcept -> void {
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(physical_device, &properties);

			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(physical_device, &features);

			VkPhysicalDeviceMemoryProperties memory_properties;
			vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);


			PH_INFO(std::format("Selected Vulkan Device: {}", properties.deviceName));


			switch(properties.deviceType){
				case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
					PH_INFO("\tGPU Type:       Discrete");
					break;

				case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
					PH_INFO("\tGPU Type:       Integrated");
					break;

				case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
					PH_INFO("\tGPU Type:       CPU");
					break;

				case VK_PHYSICAL_DEVICE_TYPE_CPU:
					PH_INFO("\tGPU Type:       Virtual");
					break;

				default:
				case VK_PHYSICAL_DEVICE_TYPE_OTHER:
					PH_INFO("\tGPU Type:       Other / Unknown");
					break;
			};


			PH_INFO( std::format(
				"\tAPI Version:    {}.{}.{}",
				VK_VERSION_MAJOR(properties.apiVersion),
				VK_VERSION_MINOR(properties.apiVersion),
				VK_VERSION_PATCH(properties.apiVersion)
			) );


			


			if(properties.vendorID == 0x10de){
				// NVIDIA
				PH_INFO( std::format(
					"\tDriver Version: {}.{}.{}.{}",
					(properties.driverVersion >> 22) & 0x3ff,
					(properties.driverVersion >> 14) & 0x0ff,
					(properties.driverVersion >> 6) & 0x0ff,
					properties.driverVersion & 0x003f
				) );

			#if defined(PH_PLATFORM_WINDOWS)
				}else if(properties.vendorID == 0x8086){
					// Intel
					PH_INFO( std::format(
						"\tDriver Version: ??.??.{}.{} (unformatted: {})",
						(properties.driverVersion >> 14) & 0xff,
						properties.driverVersion & 0x3fff,
						properties.driverVersion
					) );
					PH_TRACE("\t(Intel Driver version formatting is really weird. The first values cannot be obtained through Vulkan)");

			#endif

			}else{
				// Default
				PH_INFO( std::format(
					"\tDriver Version: {}.{}.{} (unformatted: {})",
					VK_VERSION_MAJOR(properties.driverVersion),
					VK_VERSION_MINOR(properties.driverVersion),
					VK_VERSION_PATCH(properties.driverVersion),
					properties.apiVersion
				) );
			}



			auto vendor_id_name = std::string("Unknown");
			switch(properties.vendorID){
				break; case 0x10de: vendor_id_name = "NVIDIA";
				break; case 0x8086: vendor_id_name = "Intel";
				break; case 0x1002: vendor_id_name = "AMD";
				break; case 0x13B5: vendor_id_name = "ARM";
				break; case 0x5143: vendor_id_name = "Qualcomm";
			};


			PH_INFO( std::format("\tVendor ID:      {:x} ({})", properties.vendorID, vendor_id_name) );
			PH_INFO( std::format("\tDevice ID:      {:x}", properties.deviceID) );


			// memory information
			for(uint32_t i = 0; i < memory_properties.memoryHeapCount; i++){
				float memory_size_gib = float(memory_properties.memoryHeaps[i].size) / 1024.0f / 1024.0f / 1024.0f;

				if(memory_properties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT){
					PH_INFO( std::format("\tGPU memory heap <{}> (Local):  {} GiB", i, memory_size_gib) );
				}else{
					PH_INFO( std::format("\tGPU memory heap <{}> (Shared): {} GiB", i, memory_size_gib) );
				}
			}

		};



		auto find_supported_format(
			const VkPhysicalDevice& physical_device, const evo::ArrayProxy<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features
		) noexcept -> VkFormat {
			for(VkFormat format : candidates){
				VkFormatProperties props;
				vkGetPhysicalDeviceFormatProperties(physical_device, format, &props);

				if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
					return format;
				} else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
					return format;
				}
			}

			PH_FATAL_BREAK("Failed to find supported format");
		};


		auto find_depth_format(const VkPhysicalDevice& physical_device) noexcept -> VkFormat {
			return find_supported_format(
				physical_device,
				{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
				VK_IMAGE_TILING_OPTIMAL,
				VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
			);
		};


		auto format_has_stencil_component(VkFormat format) noexcept -> bool {
			return (format == VK_FORMAT_D32_SFLOAT_S8_UINT) || (format == VK_FORMAT_D24_UNORM_S8_UINT);
		};



		auto get_max_msaa_samples(const VkPhysicalDevice& physical_device) noexcept -> VkSampleCountFlagBits {
			VkPhysicalDeviceProperties physical_device_properties;
			vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);

			const VkSampleCountFlags counts = physical_device_properties.limits.framebufferColorSampleCounts
												& physical_device_properties.limits.framebufferDepthSampleCounts;
			
			if(counts & VK_SAMPLE_COUNT_64_BIT) return VK_SAMPLE_COUNT_64_BIT;
			if(counts & VK_SAMPLE_COUNT_32_BIT) return VK_SAMPLE_COUNT_32_BIT;
			if(counts & VK_SAMPLE_COUNT_16_BIT) return VK_SAMPLE_COUNT_16_BIT;
			if(counts & VK_SAMPLE_COUNT_8_BIT)  return VK_SAMPLE_COUNT_8_BIT;
			if(counts & VK_SAMPLE_COUNT_4_BIT)  return VK_SAMPLE_COUNT_4_BIT;
			if(counts & VK_SAMPLE_COUNT_2_BIT)  return VK_SAMPLE_COUNT_2_BIT;

			return VK_SAMPLE_COUNT_1_BIT;
		};


	};
};