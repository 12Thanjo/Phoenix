#include "context.h"

#include "Logging.h"

#include "vulkan/utils.h"

#include <format>

// TODO: remove
#include <set>

namespace ph{
	namespace vulkan{


		//////////////////////////////////////////////////////////////////////
		// debug messenger callback

		#if defined(PH_CONFIG_DEBUG)

				static VKAPI_ATTR auto VKAPI_CALL vk_debug_callback(
					VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
					VkDebugUtilsMessageTypeFlagsEXT message_type,
					const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
					[[maybe_unused]] void* user_data
				) -> VkBool32 {

					// get rid of message having a '\n' in it
					// auto message = std::string(callback_data->pMessage);
					// if(message[message.size() - 1] == '\n'){
					// 	message = message.substr(0, message.size() - 2);
					// }

					const char* message_type_str;

					switch(message_type){
						break; case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: message_type_str = "GENERAL";
						break; case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: message_type_str = "VALIDATION";
						break; case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: message_type_str = "PERFORMANCE";
						break; case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: message_type_str = "DEVICE ADDRESS BINDING";
					};


					const std::string message = std::format("{{{}}} {}", message_type_str, callback_data->pMessage);
					

					switch(message_severity){
						break;case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
							PH_ERROR(message);
							
							#if defined(PH_BUILD_DEBUG)
								PH_DEBUG_BREAK();
							#endif
						}
						break;case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: PH_WARNING(message);
						break;case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:    PH_INFO(message);
						break;case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: PH_TRACE(message);
						break;default: PH_FATAL_BREAK("Debug callback recieved unknown Severity type");
					};

					return false;
				};


		#endif



		//////////////////////////////////////////////////////////////////////
		// instance


		auto create_instance(const std::string& app_name) noexcept -> std::optional<VkInstance> {

			// TODO: make more general
			auto app_info = VkApplicationInfo{ .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
				.pApplicationName   = app_name.c_str(),
				.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
				.pEngineName        = "Phoenix Engine",
				.engineVersion      = VK_MAKE_VERSION(1, 0, 0),
				.apiVersion         = VK_API_VERSION_1_0,
			};



			///////////////////////////////////
			// layers

			auto required_validation_layers = evo::StaticVector<const char*, 1>{};

			#if defined(PH_CONFIG_DEBUG) 
				required_validation_layers.push_back("VK_LAYER_KHRONOS_validation");


				// TODO: error checking
				auto available_layers = std::vector<VkLayerProperties>();
				{
					uint32_t layer_count;
					vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

					available_layers.resize(layer_count);
					vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
				}



				// check that all required layers are available
				PH_TRACE("Checking that all required Vulkan layers are available");
				for(const char* required_layer : required_validation_layers){

					bool found = false;
					for(auto& available_layer : available_layers){
						if(evo::stringsEqual(required_layer, available_layer.layerName)){
							found = true;
							PH_TRACE( std::format("\tFound validation layer: '{}'", required_layer) );
							break;
						}
					}


					if(!found){
						PH_FATAL(std::format("Failed to find required Vulkan validation layer: '{}'", required_layer));
						return std::nullopt;
					}

				}

				PH_TRACE("All required Vulkan layers are available");


			#endif



			///////////////////////////////////
			// extensions

			// call GLFW::get_required_instance_extensions() ?

			auto required_extensions = evo::StaticVector<const char*, 3>{};

			required_extensions.push_back("VK_KHR_surface");

			#if defined(PH_PLATFORM_WINDOWS)
				required_extensions.push_back("VK_KHR_win32_surface");
			#elif PH_PLATFORM_LINUX
				required_extensions.push_back("VK_KHR_xcb_surface");
			#endif

			#if defined(PH_CONFIG_DEBUG)
				required_extensions.push_back("VK_EXT_debug_utils");

				#if defined(PH_CONFIG_TRACE)
					PH_TRACE("Required Vulkan Extensions:");
					for(const char* ext : required_extensions){
						PH_TRACE(std::format("\t'{}'", ext));
					}
				#endif
			#endif


			///////////////////////////////////
			// startup debug messenger

			#if defined(PH_CONFIG_DEBUG)
				const auto instance_messenger = VkDebugUtilsMessengerCreateInfoEXT{ .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
					.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
										| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
					                  	| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
					                  	| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,

					.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
										| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
										| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
										| VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,

					.pfnUserCallback = vk_debug_callback,
					.pUserData       = nullptr,
				};
			#endif



			///////////////////////////////////
			// create instance

			const auto create_info = VkInstanceCreateInfo{ .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				#if defined(PH_CONFIG_DEBUG)
					.pNext = &instance_messenger,
				#endif

				.pApplicationInfo        = &app_info,

				.enabledLayerCount       = static_cast<uint32_t>(required_validation_layers.size()),
				.ppEnabledLayerNames     = required_validation_layers.data(),

				.enabledExtensionCount   = static_cast<uint32_t>(required_extensions.size()),
				.ppEnabledExtensionNames = required_extensions.data(),
			};



			VkInstance instance;
			const VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
			if(vulkan::result_fail(result, "vkCreateInstance")) return std::nullopt;


			PH_DEBUG("Created: Vulkan instance");
			return std::move(instance);
		};



		auto destroy_instance(VkInstance& instance) noexcept -> void {
			PH_ASSERT(instance != VK_NULL_HANDLE, "Instance is not valid handle");

			vkDestroyInstance(instance, nullptr);
			instance = VK_NULL_HANDLE;

			PH_DEBUG("Destroyed: Vulkan instance");
		};




		#if defined(PH_CONFIG_DEBUG)


			auto create_debug_messenger(const VkInstance& instance) noexcept -> std::optional<VkDebugUtilsMessengerEXT> {
				const auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
					instance, "vkCreateDebugUtilsMessengerEXT"
				);

				if(vkCreateDebugUtilsMessengerEXT == nullptr){
					PH_ERROR("vkCreateDebugUtilsMessengerEXT() not found");
					return std::nullopt;
				}


				const auto create_info = VkDebugUtilsMessengerCreateInfoEXT{ .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
					.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
										| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
					                  	| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
					                  	| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,

					.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
										| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
										| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
										| VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,

					.pfnUserCallback = vk_debug_callback,
					.pUserData       = nullptr,
				};


				VkDebugUtilsMessengerEXT debug_messenger;
				const VkResult result = vkCreateDebugUtilsMessengerEXT(instance, &create_info, nullptr, &debug_messenger);
				if(vulkan::result_fail(result, "vkCreateDebugUtilsMessengerEXT")) return std::nullopt;

				PH_DEBUG("Created: Vulkan debug messenger");
				return std::move(debug_messenger);
			};



			auto destroy_debug_messenger(const VkInstance& instance, VkDebugUtilsMessengerEXT& debug_messenger) noexcept -> bool {
				const auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
					instance, "vkDestroyDebugUtilsMessengerEXT"
				);

				if(vkDestroyDebugUtilsMessengerEXT == nullptr){
					PH_ERROR("vkDestroyDebugUtilsMessengerEXT() not found");
					return false;
				}

				vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
				debug_messenger = VK_NULL_HANDLE;

				PH_DEBUG("Destroyed: Vulkan debug messenger");
				return true;
			};


		#endif




		auto select_physical_device(const VkInstance& instance) noexcept -> std::optional<VkPhysicalDevice> {


			// TODO: error checking
			auto physical_devices = std::vector<VkPhysicalDevice>();
			{
				uint32_t physical_device_count;
				vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

				physical_devices.resize(physical_device_count);
				vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());
			}


			if(physical_devices.empty()){
				PH_FATAL("Found no devices that support Vulkan");
				return std::nullopt;
			}


			auto potential_devices = std::vector<VkPhysicalDevice>{};

			PH_TRACE("Selecting Physical Device:");

			for(auto& physical_device : physical_devices){

				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(physical_device, &properties);

				VkPhysicalDeviceFeatures features;
				vkGetPhysicalDeviceFeatures(physical_device, &features);

				// VkPhysicalDeviceMemoryProperties memory_properties;
				// vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);


				bool device_is_supported = true;

				if(features.samplerAnisotropy == VK_FALSE) device_is_supported = false;




				if(device_is_supported == false){
					PH_TRACE(std::format("\t- '{}': not supported", properties.deviceName));
					continue;
				}


				PH_TRACE(std::format("\t- '{}': supported", properties.deviceName));
				potential_devices.push_back(physical_device);

			}


			if(potential_devices.empty()){
				PH_ERROR("Found no Vulkan devices that support all required features");
				return std::nullopt;
			}

			//////////////////////////////////////////////////////////////////////
			// sort devices

			std::sort(potential_devices.begin(), potential_devices.end(), 
				[](const VkPhysicalDevice& lhs, const VkPhysicalDevice& rhs) noexcept -> bool {
					// return true if lhs is better
					// return false if rhs is better
					// return true if equal

					PH_ASSERT(lhs != rhs, "Physical device is in the list more than once");

					const auto score_gpu = [](const VkPhysicalDevice& physical_device) noexcept -> int {
						int score = 0;

						VkPhysicalDeviceProperties properties;
						vkGetPhysicalDeviceProperties(physical_device, &properties);

						switch(properties.deviceType){
							break; case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:   score += 5;
							break; case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:    score += 3;
							break; case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: score += 2;
							break; case VK_PHYSICAL_DEVICE_TYPE_CPU:            score += 1;
							// break; case VK_PHYSICAL_DEVICE_TYPE_OTHER:       score += 0;
						};

						return score;
					};


					return score_gpu(lhs) >= score_gpu(rhs);
				}
			);


			//////////////////////////////////////////////////////////////////////
			// done

			const VkPhysicalDevice picked_device = potential_devices[0];
			vulkan::print_physical_device_properties(picked_device);

			PH_TRACE("Picked vulkan physical device");
			return picked_device;
		};






		//////////////////////////////////////////////////////////////////////
		// logical device



		auto Device::create(const VkSurfaceKHR& surface, VkPhysicalDevice physical_device, bool use_sample_shading) noexcept -> bool {

			const vulkan::QueueFamilyIndices indices = vulkan::find_queue_families(physical_device, surface);
			if(indices.is_complete() == false) return false;
				

			auto queue_create_infos = std::vector<VkDeviceQueueCreateInfo>{};
			auto unique_queue_families = std::set<uint32_t>{ *indices.graphics_family, *indices.present_family };

			float queue_priority = 1.0f;
			for(uint32_t queue_family : unique_queue_families){
				auto queue_create_info = VkDeviceQueueCreateInfo{ .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.queueFamilyIndex = queue_family,
					.queueCount = 1,
					.pQueuePriorities = &queue_priority,
				};
				queue_create_infos.push_back(queue_create_info);
			}

			const auto device_features = VkPhysicalDeviceFeatures{
				.sampleRateShading = use_sample_shading ? VK_TRUE : VK_FALSE,
				.samplerAnisotropy   = VK_TRUE,
			};


			const auto device_extensions = std::to_array<const char*>({
				"VK_KHR_swapchain"
			});


			const auto create_info = VkDeviceCreateInfo{ .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_infos.size()),
				.pQueueCreateInfos       = queue_create_infos.data(),

				// depricated
				.enabledLayerCount       = 0,
				.ppEnabledLayerNames     = nullptr,

				.enabledExtensionCount   = static_cast<uint32_t>(device_extensions.size()),
				.ppEnabledExtensionNames = device_extensions.data(),

				.pEnabledFeatures        = &device_features,
			};

			const VkResult result = vkCreateDevice(physical_device, &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateDevice")) return false;

			vkGetDeviceQueue(this->handle, *indices.graphics_family, 0, &this->graphics_queue);
			vkGetDeviceQueue(this->handle, *indices.present_family, 0, &this->present_queue);
			vkGetDeviceQueue(this->handle, *indices.transfer_family, 0, &this->transfer_queue);

			this->physical = physical_device;


			PH_DEBUG("Created: Vulkan device");
			return true;
		};


		auto Device::destroy() noexcept -> void {
			vkDestroyDevice(this->handle, nullptr);
			this->handle = VK_NULL_HANDLE;

			this->physical = VK_NULL_HANDLE;

			PH_DEBUG("Destroyed: Vulkan device");			
		};



		auto Device::wait_idle() const noexcept -> bool {
			const VkResult result = vkDeviceWaitIdle(this->handle);
			if(vulkan::result_fail(result, "vkDeviceWaitIdle")) return false;

			return true;
		};

		
		
	};
};