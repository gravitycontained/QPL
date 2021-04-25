#include <qpl/vulkan.hpp>
#ifdef QPL_USE_VULKAN
#include <qpl/filesys.hpp>
#include <qpl/string.hpp>
#include <algorithm>
#include <set>
#include <functional>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#pragma warning(push)
#pragma warning(disable : 26812)

namespace qpl {
	bool qpl::vk::queue_family_indices::is_complete() const {
		return this->graphics_family.has_value() && this->present_family.has_value();
	}
	VkSurfaceFormatKHR qpl::vk::swapchain_support_details::choose_swap_surface_format() const {
		for (const auto& format : this->formats) {
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return format;
			}
		}

		return this->formats.front();
	}
	VkPresentModeKHR qpl::vk::swapchain_support_details::choose_swap_present_mode() const {
		for (const auto& present_mode : this->present_modes) {
			if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return present_mode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}
	VkExtent2D qpl::vk::swapchain_support_details::choose_swap_extent(glm::uvec2 dimension) const {
		if (this->capabilities.currentExtent.width != UINT32_MAX) {
			return this->capabilities.currentExtent;
		}
		else {
			VkExtent2D actual_extent = { dimension.x, dimension.y };

			actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
			actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actual_extent.height));

			return actual_extent;
		}
	}

	const std::vector<const char*> qpl::vk::validation_layers = {
	"VK_LAYER_LUNARG_standard_validation"
	};
	const std::vector<const char*> qpl::vk::device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	void qpl::vk::result_check(VkResult result, const std::string& message) {
		if (result != VK_SUCCESS) {
			std::string error_message;
			switch (result) {
			case VK_SUCCESS: error_message = "VK_SUCCESS"; break;
			case VK_NOT_READY: error_message = "VK_NOT_READY"; break;
			case VK_TIMEOUT: error_message = "VK_TIMEOUT"; break;
			case VK_EVENT_SET: error_message = "VK_EVENT_SET"; break;
			case VK_EVENT_RESET: error_message = "VK_EVENT_RESET"; break;
			case VK_INCOMPLETE: error_message = "VK_INCOMPLETE | VK_RESULT_END_RANGE"; break;
			case VK_ERROR_OUT_OF_HOST_MEMORY: error_message = "VK_ERROR_OUT_OF_HOST_MEMORY"; break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY: error_message = "VK_ERROR_OUT_OF_DEVICE_MEMORY"; break;
			case VK_ERROR_INITIALIZATION_FAILED: error_message = "VK_ERROR_INITIALIZATION_FAILED"; break;
			case VK_ERROR_DEVICE_LOST: error_message = "VK_ERROR_DEVICE_LOST"; break;
			case VK_ERROR_MEMORY_MAP_FAILED: error_message = "VK_ERROR_MEMORY_MAP_FAILED"; break;
			case VK_ERROR_LAYER_NOT_PRESENT: error_message = "VK_ERROR_LAYER_NOT_PRESENT"; break;
			case VK_ERROR_EXTENSION_NOT_PRESENT: error_message = "VK_ERROR_EXTENSION_NOT_PRESENT"; break;
			case VK_ERROR_FEATURE_NOT_PRESENT: error_message = "VK_ERROR_FEATURE_NOT_PRESENT"; break;
			case VK_ERROR_INCOMPATIBLE_DRIVER: error_message = "VK_ERROR_INCOMPATIBLE_DRIVER"; break;
			case VK_ERROR_TOO_MANY_OBJECTS: error_message = "VK_ERROR_TOO_MANY_OBJECTS"; break;
			case VK_ERROR_FORMAT_NOT_SUPPORTED: error_message = "VK_ERROR_FORMAT_NOT_SUPPORTED"; break;
			case VK_ERROR_FRAGMENTED_POOL: error_message = "VK_ERROR_FRAGMENTED_POOL | VK_RESULT_BEGIN_RANGE"; break;
			case VK_ERROR_OUT_OF_POOL_MEMORY: error_message = "VK_ERROR_OUT_OF_POOL_MEMORY | VK_ERROR_OUT_OF_POOL_MEMORY_KHR"; break;
			case VK_ERROR_INVALID_EXTERNAL_HANDLE: error_message = "VK_ERROR_INVALID_EXTERNAL_HANDLE | VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR"; break;
			case VK_ERROR_SURFACE_LOST_KHR: error_message = "VK_ERROR_SURFACE_LOST_KHR"; break;
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: error_message = "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"; break;
			case VK_SUBOPTIMAL_KHR: error_message = "VK_SUBOPTIMAL_KHR"; break;
			case VK_ERROR_OUT_OF_DATE_KHR: error_message = "VK_ERROR_OUT_OF_DATE_KHR"; break;
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: error_message = "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"; break;
			case VK_ERROR_VALIDATION_FAILED_EXT: error_message = "VK_ERROR_VALIDATION_FAILED_EXT"; break;
			case VK_ERROR_INVALID_SHADER_NV: error_message = "VK_ERROR_INVALID_SHADER_NV"; break;
			case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: error_message = "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"; break;
			case VK_ERROR_FRAGMENTATION_EXT: error_message = "VK_ERROR_FRAGMENTATION_EXT"; break;
			case VK_ERROR_NOT_PERMITTED_EXT: error_message = "VK_ERROR_NOT_PERMITTED_EXT"; break;
			case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT: error_message = "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT"; break;
			case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: error_message = "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT"; break;
			case VK_RESULT_RANGE_SIZE: error_message = "VK_RESULT_RANGE_SIZE"; break;
			default: error_message = "unkown"; break;
			}
			throw std::runtime_error(qpl::to_string(message, "\n[", error_message, "]").c_str());
		}
	}
	qpl::u32 qpl::vk::rate_physical_device(VkPhysicalDevice physical_device) {
		VkPhysicalDeviceProperties device_properties;
		VkPhysicalDeviceFeatures device_features;
		vkGetPhysicalDeviceProperties(physical_device, &device_properties);
		vkGetPhysicalDeviceFeatures(physical_device, &device_features);

		qpl::u32 score = 0u;

		if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000u;
		}

		score += device_properties.limits.maxImageDimension2D;

		if (!device_features.geometryShader) {
			return 0u;
		}

		return score;
	}
	std::vector<const char*> qpl::vk::get_required_extensions() {
		qpl::u32 glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

		if constexpr (qpl::vk::validation_layer::is_enabled()) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
	bool qpl::vk::has_device_extensions_support(VkPhysicalDevice physical_device) {
		qpl::u32 extension_count;
		vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

		std::vector<VkExtensionProperties> available_extensions(extension_count);
		vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

		std::set<std::string> required_extensions(qpl::vk::device_extensions.begin(), qpl::vk::device_extensions.end());

		for (const auto& extension : available_extensions) {
			required_extensions.erase(extension.extensionName);
		}

		return required_extensions.empty();
	}
	qpl::vk::queue_family_indices qpl::vk::find_queue_families(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
		qpl::vk::queue_family_indices indices;

		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

		qpl::u32 i = 0u;
		for (const auto& queueFamily : queue_families) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphics_family = i;
			}

			VkBool32 preset_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &preset_support);

			if (preset_support) {
				indices.present_family = i;
			}

			if (indices.is_complete()) {
				break;
			}
			++i;
		}

		return indices;
	}
	bool qpl::vk::is_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
		auto queue_family = qpl::vk::find_queue_families(physical_device, surface);

		auto has_support = qpl::vk::has_device_extensions_support(physical_device);

		bool swap_chain_adequate = false;
		if (has_support) {
			auto swap_chain_support = qpl::vk::query_swapchain_support(physical_device, surface);
			swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
		}

		VkPhysicalDeviceFeatures supported_features;
		vkGetPhysicalDeviceFeatures(physical_device, &supported_features);

		return queue_family.is_complete() && has_support && swap_chain_adequate && supported_features.samplerAnisotropy;
	}
	qpl::vk::swapchain_support_details qpl::vk::query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
		qpl::vk::swapchain_support_details details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

		qpl::u32 format_count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);

		if (format_count != 0u) {
			details.formats.resize(format_count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, details.formats.data());
		}

		qpl::u32 present_mode_count;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);

		if (present_mode_count != 0u) {
			details.present_modes.resize(present_mode_count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, details.present_modes.data());
		}

		return details;
	}
	VkShaderModule qpl::vk::create_shader_module(VkDevice device, const std::string& code) {
		VkShaderModuleCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = code.size();
		create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shader_module;
		if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shader_module;
	}
	qpl::u32 qpl::vk::find_memory_type(VkPhysicalDevice physical_device, qpl::u32 type_filter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties mem_properties;
		vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

		for (qpl::u32 i = 0u; i < mem_properties.memoryTypeCount; ++i) {
			if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}
	void qpl::vk::create_buffer(VkDevice logical_device, VkPhysicalDevice physical_device, VkBuffer& buffer, VkDeviceMemory& buffer_memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
		VkBufferCreateInfo buffer_info = {};
		buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_info.size = size;
		buffer_info.usage = usage;
		buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(logical_device, &buffer_info, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		VkMemoryRequirements mem_requirements;
		vkGetBufferMemoryRequirements(logical_device, buffer, &mem_requirements);

		VkMemoryAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize = mem_requirements.size;
		alloc_info.memoryTypeIndex = qpl::vk::find_memory_type(physical_device, mem_requirements.memoryTypeBits, properties);

		if (vkAllocateMemory(logical_device, &alloc_info, nullptr, &buffer_memory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(logical_device, buffer, buffer_memory, 0);

	}
	void qpl::vk::copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size, VkCommandPool command_pool, VkDevice logical_device, VkQueue present_queue) {
		VkCommandBufferAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandPool = command_pool;
		alloc_info.commandBufferCount = 1;

		VkCommandBuffer command_buffer;
		vkAllocateCommandBuffers(logical_device, &alloc_info, &command_buffer);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(command_buffer, &begin_info);

		VkBufferCopy copy_region = {};
		copy_region.size = size;
		vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

		vkEndCommandBuffer(command_buffer);

		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;

		vkQueueSubmit(present_queue, 1, &submit_info, VK_NULL_HANDLE);
		vkQueueWaitIdle(present_queue);

		vkFreeCommandBuffers(logical_device, command_pool, 1, &command_buffer);
	}
	VkCommandBuffer qpl::vk::begin_single_time_commands(VkDevice logical_device, VkCommandPool command_pool) {
		VkCommandBufferAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandPool = command_pool;
		alloc_info.commandBufferCount = 1;

		VkCommandBuffer command_buffer;
		vkAllocateCommandBuffers(logical_device, &alloc_info, &command_buffer);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(command_buffer, &begin_info);

		return command_buffer;
	}
	void qpl::vk::end_single_time_commands(VkDevice logical_device, VkQueue present_queue, VkCommandPool command_pool, VkCommandBuffer& command_buffer) {
		vkEndCommandBuffer(command_buffer);

		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;

		vkQueueSubmit(present_queue, 1, &submit_info, VK_NULL_HANDLE);
		vkQueueWaitIdle(present_queue);

		vkFreeCommandBuffers(logical_device, command_pool, 1, &command_buffer);
	}
	void qpl::vk::create_image(VkDevice logical_device, VkPhysicalDevice physical_device, VkImage& image, VkDeviceMemory& image_memory, qpl::u32 width, qpl::u32 height, qpl::u32 mip_levels, VkSampleCountFlagBits num_samples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) {
		VkImageCreateInfo image_info = {};
		image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.extent.width = width;
		image_info.extent.height = height;
		image_info.extent.depth = 1;
		image_info.mipLevels = mip_levels;
		image_info.arrayLayers = 1;
		image_info.format = format;
		image_info.tiling = tiling;
		image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_info.usage = usage;
		image_info.samples = num_samples;
		image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


		if (vkCreateImage(logical_device, &image_info, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements mem_requirements;
		vkGetImageMemoryRequirements(logical_device, image, &mem_requirements);

		VkMemoryAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize = mem_requirements.size;
		alloc_info.memoryTypeIndex = qpl::vk::find_memory_type(physical_device, mem_requirements.memoryTypeBits, properties);

		if (vkAllocateMemory(logical_device, &alloc_info, nullptr, &image_memory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(logical_device, image, image_memory, 0);
	}
	void qpl::vk::transition_image_layout(VkDevice logical_device, VkCommandPool command_pool, VkQueue present_queue, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout, qpl::u32 mip_levels) {
		VkCommandBuffer command_buffer = qpl::vk::begin_single_time_commands(logical_device, command_pool);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = old_layout;
		barrier.newLayout = new_layout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mip_levels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags source_stage;
		VkPipelineStageFlags destination_stage;

		if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			command_buffer,
			source_stage, destination_stage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		qpl::vk::end_single_time_commands(logical_device, present_queue, command_pool, command_buffer);
	}
	void qpl::vk::copy_buffer_to_image(VkDevice logical_device, VkCommandPool command_pool, VkQueue present_queue, VkBuffer buffer, VkImage image, qpl::u32 width, qpl::u32 height) {
		VkCommandBuffer command_buffer = qpl::vk::begin_single_time_commands(logical_device, command_pool);

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		qpl::vk::end_single_time_commands(logical_device, present_queue, command_pool, command_buffer);
	}
	VkImageView qpl::vk::create_image_view(VkDevice logical_device, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags, qpl::u32 mip_levels) {
		VkImageViewCreateInfo view_info = {};
		view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.image = image;
		view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view_info.format = format;
		view_info.subresourceRange.aspectMask = aspect_flags;
		view_info.subresourceRange.baseMipLevel = 0;
		view_info.subresourceRange.levelCount = mip_levels;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount = 1;

		VkImageView image_view;
		if (vkCreateImageView(logical_device, &view_info, nullptr, &image_view) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return image_view;
	}
	VkFormat qpl::vk::find_supported_format(VkPhysicalDevice physical_device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physical_device, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		throw std::runtime_error("failed to find supported format!");
	}
	VkFormat qpl::vk::find_depth_format(VkPhysicalDevice physical_device) {
		return qpl::vk::find_supported_format(physical_device,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}
	void qpl::vk::generate_mipmaps(VkPhysicalDevice physical_device, VkDevice logical_device, VkQueue present_queue, VkCommandPool command_pool, VkImage image, VkFormat imageFormat, int32_t tex_width, int32_t tex_height, uint32_t mip_levels) {
		// Check if image format supports linear blitting
		VkFormatProperties format_properties;
		vkGetPhysicalDeviceFormatProperties(physical_device, imageFormat, &format_properties);

		if (!(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
			throw std::runtime_error("texture image format does not support linear blitting!");
		}

		VkCommandBuffer command_buffer = qpl::vk::begin_single_time_commands(logical_device, command_pool);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mip_width = tex_width;
		int32_t mip_height = tex_height;

		//VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL

		for (uint32_t i = 1; i < mip_levels; ++i) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(command_buffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit = {};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mip_width, mip_height, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(command_buffer,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(command_buffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if (mip_width > 1) mip_width /= 2;
			if (mip_height > 1) mip_height /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mip_levels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(command_buffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		qpl::vk::end_single_time_commands(logical_device, present_queue, command_pool, command_buffer);
	}
	VkSampleCountFlagBits qpl::vk::get_max_usable_sample_count(VkPhysicalDevice physical_device) {
		VkPhysicalDeviceProperties phyiscal_device_properties;
		vkGetPhysicalDeviceProperties(physical_device, &phyiscal_device_properties);

		VkSampleCountFlags counts = phyiscal_device_properties.limits.framebufferColorSampleCounts & phyiscal_device_properties.limits.framebufferDepthSampleCounts;
		if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}


	std::vector<VkExtensionProperties> qpl::vk::instance::get_extensions() const {
		qpl::u32 extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
		return extensions;
	}
	void qpl::vk::instance::create(const qpl::vk::validation_layer& validation_layer) {
		if (this->is_created()) {
			throw std::runtime_error("instance already created!");
			return;
		}

		if constexpr (validation_layer::is_enabled()) {
			auto result = validation_layer.check_support();
			if (!result) {
				throw std::runtime_error("asked for validation layer; didn't find it");
			}
		}

		VkApplicationInfo application_info = {};
		application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pApplicationName = "Hello Triangle";
		application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.pEngineName = "No Engine";
		application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &application_info;

		auto extensions = get_required_extensions();
		create_info.enabledExtensionCount = static_cast<qpl::u32>(extensions.size());
		create_info.ppEnabledExtensionNames = extensions.data();

		if constexpr (validation_layer::is_enabled()) {
			create_info.enabledLayerCount = static_cast<qpl::u32>(qpl::vk::validation_layers.size());
			create_info.ppEnabledLayerNames = qpl::vk::validation_layers.data();
		}
		else {
			create_info.enabledLayerCount = 0u;
		}

		if (vkCreateInstance(&create_info, nullptr, &this->vk_instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
		this->m_created = true;
	}
	bool qpl::vk::instance::is_created() const {
		return this->m_created;
	}
	void qpl::vk::instance::construct() {
		this->m_created = false;
		this->vk_instance = 0;
	}
	void qpl::vk::instance::destroy() {
		if (this->is_created()) {
			vkDestroyInstance(this->vk_instance, nullptr);
			this->m_created = false;
		}
	}

	std::vector<VkLayerProperties> qpl::vk::validation_layer::get_available_validation_layers() {
		qpl::u32 layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> available_layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
		return available_layers;

	}
	bool qpl::vk::validation_layer::check_support() const {
		auto available_layers = this->get_available_validation_layers();

		for (const auto& i : qpl::vk::validation_layers) {
			for (const auto& j : available_layers) {
				if (std::strcmp(i, j.layerName) == 0) {
					return true;
				}
			}
		}
		return false;
	}
	bool qpl::vk::validation_layer::is_created() const {
		return this->m_created;
	}
	VKAPI_ATTR VkBool32 VKAPI_CALL qpl::vk::validation_layer::debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type,
		const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
		void* p_user_data) {

		qpl::println("validation layer: ", p_callback_data->pMessage);
		return VK_FALSE;
	}
	void qpl::vk::validation_layer::create(const qpl::vk::instance& instance) {
		if (!qpl::vk::validation_layer::is_enabled()) return;
		if (this->is_created()) {
			throw std::runtime_error("validation layer already created!");
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT create_info;

		create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		create_info.pfnUserCallback = this->debug_callback;

		VkResult result;
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.vk_instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			result = func(instance.vk_instance, &create_info, nullptr, &this->vk_debug_messenger);
		}
		else {
			result = VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}
		this->m_created = true;
	}
	void qpl::vk::validation_layer::destroy(const qpl::vk::instance& instance) {
		if (this->is_created()) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.vk_instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(instance.vk_instance, this->vk_debug_messenger, nullptr);
			}

			this->m_created = false;
		}
	}
	void qpl::vk::validation_layer::construct() {
		this->m_created = false;
	}



	void qpl::vk::surface::create(const qpl::vk::instance& instance) {
		if (this->is_created()) {
			throw std::runtime_error("surface already created!");
			return;
		}
		if (glfwCreateWindowSurface(instance.vk_instance, qpl::window::p_window, nullptr, &this->vk_surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vulkan surface!");
		}
		this->m_created = true;
	}
	bool qpl::vk::surface::is_created() const {
		return this->m_created;
	}
	void qpl::vk::surface::construct() {
		this->m_created = false;
	}
	void qpl::vk::surface::destroy(const qpl::vk::instance& instance) {
		if (this->is_created()) {
			vkDestroySurfaceKHR(instance.vk_instance, this->vk_surface, nullptr);
			this->m_created = false;
		}
	}


	void qpl::vk::physical_device::construct() {
		this->m_created = false;
		this->vk_device = VK_NULL_HANDLE;
		this->msaa_samples = VK_SAMPLE_COUNT_1_BIT;
		this->multisampling = true;
	}
	void qpl::vk::physical_device::create(const qpl::vk::instance& instance, const qpl::vk::surface& surface) {
		if (this->is_created()) {
			throw std::runtime_error("physical device already created!");
			return;
		}
		qpl::u32 physical_device_count = 0;
		vkEnumeratePhysicalDevices(instance.vk_instance, &physical_device_count, nullptr);

		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		vkEnumeratePhysicalDevices(instance.vk_instance, &physical_device_count, physical_devices.data());

		for (const auto& physical_device : physical_devices) {
			auto score = qpl::vk::rate_physical_device(physical_device);
			this->m_physical_devices.insert(std::make_pair(score, physical_device));
		}

		auto find = std::find_if(this->m_physical_devices.rbegin(), this->m_physical_devices.rend(), [&](const auto& pd) {
			return qpl::vk::is_device_suitable(pd.second, surface.vk_surface);
			});
		if (find != this->m_physical_devices.rend()) {
			this->vk_device = find->second;
			if (this->is_multisampling_enabled()) {
				this->msaa_samples = qpl::vk::get_max_usable_sample_count(this->vk_device);
			}
		}
		else {
			throw std::runtime_error("failed to find a suitable GPU!");
		}

		this->m_created = true;
	}
	bool qpl::vk::physical_device::is_created() const {
		return this->m_created;
	}
	void qpl::vk::physical_device::destroy() {
		this->m_created = false;
	}
	void qpl::vk::physical_device::enable_multisampling() {
		this->multisampling = true;
		if (this->is_created()) {
			this->msaa_samples = qpl::vk::get_max_usable_sample_count(this->vk_device);
		}

	}
	void qpl::vk::physical_device::disable_multisampling() {
		this->multisampling = false;
		if (this->is_created()) {
			this->msaa_samples = VK_SAMPLE_COUNT_1_BIT;
		}
	}
	bool qpl::vk::physical_device::is_multisampling_enabled() const {
		return this->multisampling;
	}
	void qpl::vk::physical_device::print_physical_devices() const {

		qpl::strform_builder builder;
		for (const auto& device : this->m_physical_devices) {
			VkPhysicalDeviceProperties device_properties;
			vkGetPhysicalDeviceProperties(device.second, &device_properties);

			builder.add_entry(qpl::strform_fixed("name: "));
			builder.add_entry(device_properties.deviceName);
			builder.add_entry(qpl::strform_fixed(" score: "));
			builder.add_entry(device.first);
			builder.complete_line();
		}
		qpl::println(builder.string());
	}
	qpl::vk::queue_family_indices qpl::vk::physical_device::get_queue_family_indices(const qpl::vk::surface& surface) const {
		return qpl::vk::find_queue_families(this->vk_device, surface.vk_surface);
	}


	void qpl::vk::present_queue::create(const qpl::vk::physical_device& physical_device, const qpl::vk::surface& surface) {
		if (this->is_created()) {
			throw std::runtime_error("present queue already created!");
			return;
		}
		auto indices = qpl::vk::find_queue_families(physical_device.vk_device, surface.vk_surface);

		std::set<qpl::u32> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

		float queue_priority = 1.0f;
		for (qpl::u32 queue_family : unique_queue_families) {
			VkDeviceQueueCreateInfo queue_create_info = {};
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.queueFamilyIndex = queue_family;
			queue_create_info.queueCount = 1;
			queue_create_info.pQueuePriorities = &queue_priority;
			this->vk_queue_create_infos.push_back(queue_create_info);
		}

		this->m_created = true;
	}
	bool qpl::vk::present_queue::is_created() const {
		return this->m_created;
	}
	void qpl::vk::present_queue::destroy() {
		if (this->is_created()) {
			this->vk_queue_create_infos.clear();
			this->m_created = false;
		}
	}
	void qpl::vk::present_queue::construct() {
		this->m_created = false;
	}


	void qpl::vk::logical_device::create(const qpl::vk::physical_device& physical_device,
		const qpl::vk::validation_layer& validation_layer,
		const qpl::vk::surface& surface,
		qpl::vk::present_queue& present_queue) {

		if (this->is_created()) {
			throw std::runtime_error("logical device already created!");
			return;
		}
		auto indices = qpl::vk::find_queue_families(physical_device.vk_device, surface.vk_surface);
		if (!indices.graphics_family.has_value()) {
			throw std::runtime_error("failed to find a suitable GPU with fitting family queue!");
		}

		//VkDeviceQueueCreateInfo queue_create_info = {};
		//queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		//queue_create_info.queueFamilyIndex = indices.graphics_family.value();
		//queue_create_info.queueCount = 1u;
		//
		//float queue_priority = 1.0f;
		//queue_create_info.pQueuePriorities = &queue_priority;

		VkPhysicalDeviceFeatures device_features = {};
		device_features.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		create_info.queueCreateInfoCount = static_cast<qpl::u32>(present_queue.vk_queue_create_infos.size());
		create_info.pQueueCreateInfos = present_queue.vk_queue_create_infos.data();


		create_info.pEnabledFeatures = &device_features;

		create_info.enabledExtensionCount = static_cast<qpl::u32>(qpl::vk::device_extensions.size());
		create_info.ppEnabledExtensionNames = qpl::vk::device_extensions.data();

		if constexpr (qpl::vk::validation_layer::is_enabled()) {
			create_info.enabledLayerCount = static_cast<qpl::u32>(qpl::vk::validation_layers.size());
			create_info.ppEnabledLayerNames = qpl::vk::validation_layers.data();
		}
		else {
			create_info.enabledLayerCount = 0u;
		}
		qpl::vk::result_check(vkCreateDevice(physical_device.vk_device, &create_info, nullptr, &this->vk_device), "failed to create logical device!");

		vkGetDeviceQueue(this->vk_device, indices.graphics_family.value(), 0u, &this->vk_graphics_queue);
		vkGetDeviceQueue(this->vk_device, indices.present_family.value(), 0u, &present_queue.vk_queue);

		this->m_created = true;
	}
	bool qpl::vk::logical_device::is_created() const {
		return this->m_created;
	}
	void qpl::vk::logical_device::destroy() {
		if (this->is_created()) {
			vkDestroyDevice(this->vk_device, nullptr);
			this->m_created = false;
		}
	}
	void qpl::vk::logical_device::construct() {
		this->m_created = false;
	}


	void qpl::vk::swapchain::create(
		const qpl::vk::physical_device& physical_device,
		qpl::vk::logical_device& logical_device,
		const qpl::vk::surface& surface,
		glm::ivec2 dimension) {

		if (this->is_created()) {
			throw std::runtime_error("swapchain already created!");
			return;
		}
		auto support_details = qpl::vk::query_swapchain_support(physical_device.vk_device, surface.vk_surface);
		auto surface_format = support_details.choose_swap_surface_format();
		this->vk_extent = support_details.choose_swap_extent(dimension);

		if (this->is_vsync_enabled()) {
			this->vk_present_mode = VK_PRESENT_MODE_FIFO_KHR;
		}
		else {
			this->vk_present_mode = support_details.choose_swap_present_mode();
		}

		qpl::u32 image_count = support_details.capabilities.minImageCount + 1;
		if (support_details.capabilities.maxImageCount > 0 && image_count > support_details.capabilities.maxImageCount) {
			image_count = support_details.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.surface = surface.vk_surface;

		create_info.minImageCount = image_count;
		create_info.imageFormat = surface_format.format;
		create_info.imageColorSpace = surface_format.colorSpace;
		create_info.imageExtent = this->vk_extent;
		create_info.imageArrayLayers = 1;
		create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		auto indices = qpl::vk::find_queue_families(physical_device.vk_device, surface.vk_surface);
		qpl::u32 queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };

		if (indices.graphics_family != indices.present_family) {
			create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			create_info.queueFamilyIndexCount = 2;
			create_info.pQueueFamilyIndices = queue_family_indices;
		}
		else {
			create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		create_info.preTransform = support_details.capabilities.currentTransform;
		create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		create_info.presentMode = this->vk_present_mode;

		create_info.clipped = VK_TRUE;

		create_info.oldSwapchain = VK_NULL_HANDLE;

		qpl::vk::result_check(vkCreateSwapchainKHR(logical_device.vk_device, &create_info, nullptr, &this->vk_swapchain), "failed to create swap chain!");

		vkGetSwapchainImagesKHR(logical_device.vk_device, this->vk_swapchain, &image_count, nullptr);
		this->vk_images.resize(image_count);
		vkGetSwapchainImagesKHR(logical_device.vk_device, this->vk_swapchain, &image_count, this->vk_images.data());

		this->vk_image_format = surface_format.format;
		this->m_created = true;
	}
	bool qpl::vk::swapchain::is_created() const {
		return this->m_created;
	}
	void qpl::vk::swapchain::contruct() {
		this->m_created = false;
		this->enable_vsync();
	}
	void qpl::vk::swapchain::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroySwapchainKHR(logical_device.vk_device, this->vk_swapchain, nullptr);
			this->m_created = false;
		}
	}

	void qpl::vk::swapchain::enable_vsync() {
		if (this->is_created() && !this->is_vsync_enabled()) {
			throw std::runtime_error("trying to enable vsync but swapchain was already created!");
			return;
		}
		this->vk_present_mode = VK_PRESENT_MODE_MAX_ENUM_KHR;
	}
	void qpl::vk::swapchain::disable_vsync() {
		if (this->is_created() && this->is_vsync_enabled()) {
			throw std::runtime_error("trying to disable vsync but swapchain was already created!");
			return;
		}
		this->vk_present_mode = VK_PRESENT_MODE_FIFO_KHR;
	}
	bool qpl::vk::swapchain::is_vsync_enabled() const {
		if (this->is_created()) {
			return this->vk_present_mode != VK_PRESENT_MODE_MAILBOX_KHR;
		}
		return this->vk_present_mode == VK_PRESENT_MODE_MAX_ENUM_KHR;
	}


	void qpl::vk::frame_buffer_attachment::create(const qpl::vk::logical_device& logical_device, const qpl::vk::physical_device& physical_device, const qpl::vk::swapchain& swapchain, VkImageUsageFlagBits usage_flag, VkFormat format) {
		if (this->is_created()) {
			throw std::runtime_error("depth resources already created!");
			return;
		}

		this->vk_format = format;
		qpl::vk::create_image(logical_device.vk_device, physical_device.vk_device, this->vk_image, this->vk_memory, swapchain.vk_extent.width, swapchain.vk_extent.height, 1u, VK_SAMPLE_COUNT_1_BIT, this->vk_format, VK_IMAGE_TILING_OPTIMAL, usage_flag, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		this->vk_image_view = qpl::vk::create_image_view(logical_device.vk_device, this->vk_image, this->vk_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1u);

		this->m_created = true;
	}
	bool qpl::vk::frame_buffer_attachment::is_created() const {
		return this->m_created;
	}
	void qpl::vk::frame_buffer_attachment::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyImageView(logical_device.vk_device, this->vk_image_view, nullptr);
			vkDestroyImage(logical_device.vk_device, this->vk_image, nullptr);
			vkFreeMemory(logical_device.vk_device, this->vk_memory, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::frame_buffer_attachment::construct() {
		this->m_created = false;
	}


	void qpl::vk::image_views::create(qpl::vk::logical_device& logical_device, const qpl::vk::swapchain& swapchain) {
		if (this->is_created()) {
			throw std::runtime_error("image views already created!");
			return;
		}
		this->vk_image_views.resize(swapchain.vk_images.size());

		for (size_t i = 0; i < swapchain.vk_images.size(); i++) {
			this->vk_image_views[i] = qpl::vk::create_image_view(logical_device.vk_device, swapchain.vk_images[i], swapchain.vk_image_format, VK_IMAGE_ASPECT_COLOR_BIT, 1u);
		}
		this->m_created = true;
	}
	bool qpl::vk::image_views::is_created() const {
		return this->m_created;
	}
	void qpl::vk::image_views::destroy(qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			for (auto image_view : this->vk_image_views) {
				vkDestroyImageView(logical_device.vk_device, image_view, nullptr);
			}

			//...

			this->m_created = false;
		}
	}
	void qpl::vk::image_views::construct() {
		this->m_created = false;
	}


	void qpl::vk::shader::set_path(const std::string& path) {
		this->m_path = path;
	}

	std::string qpl::vk::shader::get_path() const {
		return this->m_path;
	}
	void qpl::vk::shader::create(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			throw std::runtime_error("shader already created!");
			return;
		}
		if (this->m_path.empty()) {
			throw std::runtime_error("shader was given no path!");
		}
		auto shader_code = qpl::filesys::read_file(this->m_path);

		this->vk_module = qpl::vk::create_shader_module(logical_device.vk_device, shader_code);
		this->m_created = true;
	}
	bool qpl::vk::shader::is_created() const {
		return this->m_created;
	}
	void qpl::vk::shader::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyShaderModule(logical_device.vk_device, this->vk_module, nullptr);
			this->m_created = false;
		}
	}
	void qpl::vk::shader::construct() {
		this->m_created = false;
	}


	void qpl::vk::render_pass::create(const qpl::vk::logical_device& logical_device, const qpl::vk::physical_device& physical_device, const qpl::vk::swapchain& swapchain) {
		if (this->is_created()) {
			throw std::runtime_error("render pass already created!");
			return;
		}
		VkAttachmentDescription color_attachment = {};
		color_attachment.format = swapchain.vk_image_format;
		color_attachment.samples = physical_device.msaa_samples;
		color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depth_attachment = {};
		depth_attachment.format = qpl::vk::find_depth_format(physical_device.vk_device);
		depth_attachment.samples = physical_device.msaa_samples;
		depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription color_attachment_resolve = {};
		color_attachment_resolve.format = swapchain.vk_image_format;
		color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
		color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment_ref = {};
		color_attachment_ref.attachment = 1;
		color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference color_attachment_resolve_ref = {};
		color_attachment_resolve_ref.attachment = 0;
		color_attachment_resolve_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depth_attachment_ref = {};
		depth_attachment_ref.attachment = 2;
		depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		std::array<VkAttachmentReference, 2>  input_references{};
		input_references[0].attachment = 1;
		input_references[0].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		input_references[1].attachment = 2;
		input_references[1].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		std::array<VkSubpassDescription, 1> subpasses = {};
		subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpasses[0].colorAttachmentCount = 1;
		subpasses[0].pColorAttachments = &color_attachment_ref;
		subpasses[0].pDepthStencilAttachment = &depth_attachment_ref;
		subpasses[0].pResolveAttachments = &color_attachment_resolve_ref;

		//subpasses[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		//subpasses[1].colorAttachmentCount = 1;
		//subpasses[1].pColorAttachments = &color_attachment_refs[0];
		//subpasses[1].pDepthStencilAttachment = &depth_attachment_ref;
		//subpasses[1].inputAttachmentCount = 2;
		//subpasses[1].pInputAttachments = input_references.data();
		//
		std::vector<VkSubpassDependency> dependencies(1); //todo make variable!;

		//dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		//dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		//dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		//dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		//dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = 0;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		//dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
		//dependencies[1].dstSubpass = 0;
		//dependencies[1].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		//dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		//dependencies[1].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		//dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		//dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


		std::array<VkAttachmentDescription, 3> attachments = { color_attachment_resolve, color_attachment, depth_attachment };
		//std::array<VkAttachmentDescription, 3> attachments = { color_attachment, depth_attachment,color_attachment_resolve };
		VkRenderPassCreateInfo render_pass_info = {};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = static_cast<uint32_t>(attachments.size());
		render_pass_info.pAttachments = attachments.data();
		render_pass_info.subpassCount = 1;// subpasses.size();
		render_pass_info.pSubpasses = subpasses.data();
		render_pass_info.dependencyCount = static_cast<qpl::u32>(dependencies.size());
		render_pass_info.pDependencies = dependencies.data();

		if (vkCreateRenderPass(logical_device.vk_device, &render_pass_info, nullptr, &this->vk_render_pass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}

		this->m_created = true;
	}
	bool qpl::vk::render_pass::is_created() const {
		return this->m_created;
	}
	void qpl::vk::render_pass::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyRenderPass(logical_device.vk_device, this->vk_render_pass, nullptr);
			//for (auto i = 0; i < this->attachments.size(); ++i) {
			//	this->attachments[i].color.destroy(logical_device);
			//	this->attachments[i].depth.destroy(logical_device);
			//}

			this->m_created = false;
		}
	}
	void qpl::vk::render_pass::construct() {
		this->m_created = false;
	}


	void qpl::vk::descriptor_set_layout::create(const qpl::vk::logical_device& logical_device, const qpl::vk::uniform_buffers_proxy& ubo_proxy) {
		if (this->is_created()) {
			throw std::runtime_error("descriptor set layout already created!");
			return;
		}


		std::vector<VkDescriptorSetLayoutBinding> bindings;
		if (ubo_proxy) {
			bindings.push_back(VkDescriptorSetLayoutBinding{});
			if (ubo_proxy->uniform_buffer_object.binding == ~qpl::u32{}) {
				throw std::runtime_error(qpl::to_string("uniform buffer: ", ubo_proxy.name(), " binding value undefined").c_str());
			}
			bindings.back().binding = ubo_proxy->uniform_buffer_object.binding;
			bindings.back().descriptorCount = 1;
			bindings.back().descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			bindings.back().pImmutableSamplers = nullptr;
			auto flags = qpl::u32{};
			if (ubo_proxy->uniform_buffer_object.vertex_shader) {
				flags |= VK_SHADER_STAGE_VERTEX_BIT;
			}
			if (ubo_proxy->uniform_buffer_object.fragment_shader) {
				flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
			}
			if (flags == qpl::u32{}) {
				throw std::runtime_error(qpl::to_string("uniform buffer: ", ubo_proxy.name(), " deactivated on both vertex and fragment shader").c_str());
			}
			bindings.back().stageFlags = flags;
		}
		bindings.push_back(VkDescriptorSetLayoutBinding{});
		bindings.back().binding = this->binding;
		bindings.back().descriptorCount = 1;
		bindings.back().descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		bindings.back().pImmutableSamplers = nullptr;
		bindings.back().stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutCreateInfo layout_info = {};
		layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
		layout_info.pBindings = bindings.data();

		qpl::vk::result_check(vkCreateDescriptorSetLayout(logical_device.vk_device, &layout_info, nullptr, &this->vk_descriptor_set_layout), "failed to create descriptor set layout!");

		this->m_created = true;
	}
	bool qpl::vk::descriptor_set_layout::is_created() const {
		return this->m_created;
	}
	void qpl::vk::descriptor_set_layout::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyDescriptorSetLayout(logical_device.vk_device, this->vk_descriptor_set_layout, nullptr);

			this->m_created = false;
		}
	}

	void qpl::vk::descriptor_set_layout::set_binding(qpl::u32 binding) {
		this->binding = binding;
	}
	qpl::u32 qpl::vk::descriptor_set_layout::get_binding() const {
		return this->binding;
	}
	void qpl::vk::descriptor_set_layout::construct() {
		this->m_created = false;
	}

	void qpl::vk::graphics_pipeline::create(
		const qpl::vk::physical_device& physical_device,
		const qpl::vk::logical_device& logical_device,
		const qpl::vk::swapchain& swapchain,
		const qpl::vk::render_pass& render_pass,
		const qpl::vk::descriptor_set_layout& descriptor_set_layout,
		qpl::vk::shader& vertex_shader,
		qpl::vk::shader& fragment_shader,
		bool enable_vertices) {
		if (this->m_created) {
			throw std::runtime_error("graphics pipeline already created!");
			return;
		}

		vertex_shader.create(logical_device);
		fragment_shader.create(logical_device);

		VkPipelineShaderStageCreateInfo vert_shader_stage_info = {};
		vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vert_shader_stage_info.module = vertex_shader.vk_module;
		vert_shader_stage_info.pName = "main";

		VkPipelineShaderStageCreateInfo frag_shader_stage_info = {};
		frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		frag_shader_stage_info.module = fragment_shader.vk_module;
		frag_shader_stage_info.pName = "main";

		VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info };

		auto binding_descriptions = qpl::vk::get_binding_descriptions();
		auto attribute_descriptions = qpl::vk::get_attribute_descriptions();

		VkPipelineVertexInputStateCreateInfo vertex_input_info = {};
		if (enable_vertices) {
			vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;


			vertex_input_info.vertexBindingDescriptionCount = static_cast<qpl::u32>(binding_descriptions.size());
			vertex_input_info.vertexAttributeDescriptionCount = static_cast<qpl::u32>(attribute_descriptions.size());
			vertex_input_info.pVertexBindingDescriptions = binding_descriptions.data();
			vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();
		}


		VkPipelineInputAssemblyStateCreateInfo input_assembly = {};
		input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapchain.vk_extent.width);
		viewport.height = static_cast<float>(swapchain.vk_extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = swapchain.vk_extent;

		VkPipelineViewportStateCreateInfo viewport_state = {};
		viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state.viewportCount = 1;
		viewport_state.pViewports = &viewport;
		viewport_state.scissorCount = 1;
		viewport_state.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = physical_device.msaa_samples;

		VkPipelineDepthStencilStateCreateInfo depth_stencil = {};
		depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth_stencil.depthTestEnable = VK_TRUE;
		depth_stencil.depthWriteEnable = VK_TRUE;
		depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depth_stencil.depthBoundsTestEnable = VK_FALSE;
		depth_stencil.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment = {};
		color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo color_blending = {};
		color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blending.logicOpEnable = VK_FALSE;
		color_blending.logicOp = VK_LOGIC_OP_COPY;
		color_blending.attachmentCount = 1;
		color_blending.pAttachments = &color_blend_attachment;
		color_blending.blendConstants[0] = 0.0f;
		color_blending.blendConstants[1] = 0.0f;
		color_blending.blendConstants[2] = 0.0f;
		color_blending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipeline_layout_info = {};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 1;
		pipeline_layout_info.pSetLayouts = &descriptor_set_layout.vk_descriptor_set_layout;

		qpl::vk::result_check(vkCreatePipelineLayout(logical_device.vk_device, &pipeline_layout_info, nullptr, &this->vk_pipeline_layout), "failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo pipeline_info = {};
		pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.stageCount = 2;
		pipeline_info.pStages = shader_stages;
		if (enable_vertices) {
			pipeline_info.pVertexInputState = &vertex_input_info;
		}
		pipeline_info.pInputAssemblyState = &input_assembly;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterizer;
		pipeline_info.pMultisampleState = &multisampling;
		pipeline_info.pDepthStencilState = &depth_stencil;
		pipeline_info.pColorBlendState = &color_blending;
		pipeline_info.layout = this->vk_pipeline_layout;
		pipeline_info.renderPass = render_pass.vk_render_pass;
		pipeline_info.subpass = 0;
		pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

		qpl::vk::result_check(vkCreateGraphicsPipelines(logical_device.vk_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &this->vk_pipeline), "failed to create graphics pipeline!");

		vertex_shader.destroy(logical_device);
		fragment_shader.destroy(logical_device);

		this->m_created = true;
	}
	bool qpl::vk::graphics_pipeline::is_created() const {
		return this->m_created;
	}
	void qpl::vk::graphics_pipeline::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyPipelineLayout(logical_device.vk_device, this->vk_pipeline_layout, nullptr);
			vkDestroyPipeline(logical_device.vk_device, this->vk_pipeline, nullptr);
			this->m_created = false;
		}
	}
	void qpl::vk::graphics_pipeline::construct() {
		this->m_created = false;
	}


	void qpl::vk::command_pool::create(
		const qpl::vk::physical_device& physical_device,
		const qpl::vk::surface& surface,
		const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			throw std::runtime_error("command pool already created!");
			return;
		}

		auto queue_family_indices = qpl::vk::find_queue_families(physical_device.vk_device, surface.vk_surface);

		VkCommandPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

		qpl::vk::result_check(vkCreateCommandPool(logical_device.vk_device, &pool_info, nullptr, &this->vk_command_pool), "failed to create command pool!");

		this->m_created = true;
	}
	bool qpl::vk::command_pool::is_created() const {
		return this->m_created;
	}
	void qpl::vk::command_pool::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyCommandPool(logical_device.vk_device, this->vk_command_pool, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::command_pool::construct() {
		this->m_created = false;
	}


	void qpl::vk::color_resources::create(const qpl::vk::logical_device& logical_device, const qpl::vk::physical_device& physical_device, const qpl::vk::swapchain& swapchain) {
		if (this->is_created()) {
			throw std::runtime_error("depth resources already created!");
			return;
		}

		VkFormat depth_format = swapchain.vk_image_format;

		qpl::vk::create_image(logical_device.vk_device, physical_device.vk_device, this->vk_image, this->vk_memory, swapchain.vk_extent.width, swapchain.vk_extent.height, 1u, physical_device.msaa_samples, depth_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		this->vk_image_view = qpl::vk::create_image_view(logical_device.vk_device, this->vk_image, depth_format, VK_IMAGE_ASPECT_COLOR_BIT, 1u);

		this->m_created = true;
	}
	bool qpl::vk::color_resources::is_created() const {
		return this->m_created;
	}
	void qpl::vk::color_resources::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyImageView(logical_device.vk_device, this->vk_image_view, nullptr);
			vkDestroyImage(logical_device.vk_device, this->vk_image, nullptr);
			vkFreeMemory(logical_device.vk_device, this->vk_memory, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::color_resources::construct() {
		this->m_created = false;
	}

	void qpl::vk::depth_resources::create(const qpl::vk::logical_device& logical_device, const qpl::vk::physical_device& physical_device, const qpl::vk::swapchain& swapchain) {
		if (this->is_created()) {
			throw std::runtime_error("depth resources already created!");
			return;
		}

		VkFormat depth_format = qpl::vk::find_depth_format(physical_device.vk_device);

		qpl::vk::create_image(logical_device.vk_device, physical_device.vk_device, this->vk_image, this->vk_memory, swapchain.vk_extent.width, swapchain.vk_extent.height, 1u, physical_device.msaa_samples, depth_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		this->vk_image_view = qpl::vk::create_image_view(logical_device.vk_device, this->vk_image, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1u);

		this->m_created = true;
	}
	bool qpl::vk::depth_resources::is_created() const {
		return this->m_created;
	}
	void qpl::vk::depth_resources::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyImageView(logical_device.vk_device, this->vk_image_view, nullptr);
			vkDestroyImage(logical_device.vk_device, this->vk_image, nullptr);
			vkFreeMemory(logical_device.vk_device, this->vk_memory, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::depth_resources::construct() {
		this->m_created = false;
	}

	void qpl::vk::framebuffers::create(
		const qpl::vk::logical_device& logical_device,
		const qpl::vk::swapchain& swapchain,
		const qpl::vk::render_pass& render_pass,
		const qpl::vk::image_views& image_views,
		const qpl::vk::color_resources& color_resources,
		const qpl::vk::depth_resources& depth_resources) {

		if (this->is_created()) {
			throw std::runtime_error("framebuffers already created!");
			return;
		}

		this->vk_framebuffers.resize(image_views.vk_image_views.size());

		for (size_t i = 0; i < image_views.vk_image_views.size(); i++) {
			//std::array<VkImageView, 3> attachments = {
			//	image_views.vk_image_views[i],
			//	render_pass.attachments[i].color.vk_image_view,
			//	render_pass.attachments[i].depth.vk_image_view
			//};
			std::array<VkImageView, 3> attachments = {
				//color_resources.vk_image_view,
				//depth_resources.vk_image_view,
				//image_views.vk_image_views[i],
				image_views.vk_image_views[i],
				color_resources.vk_image_view,
				depth_resources.vk_image_view
			};

			VkFramebufferCreateInfo framebuffer_info = {};
			framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_info.renderPass = render_pass.vk_render_pass;
			framebuffer_info.attachmentCount = static_cast<qpl::u32>(attachments.size());
			framebuffer_info.pAttachments = attachments.data();
			framebuffer_info.width = swapchain.vk_extent.width;
			framebuffer_info.height = swapchain.vk_extent.height;
			framebuffer_info.layers = 1;

			qpl::vk::result_check(vkCreateFramebuffer(logical_device.vk_device, &framebuffer_info, nullptr, &this->vk_framebuffers[i]), "failed to create framebuffer!");

		}

		this->m_created = true;
	}
	bool qpl::vk::framebuffers::is_created() const {
		return this->m_created;
	}
	void qpl::vk::framebuffers::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			for (auto framebuffer : this->vk_framebuffers) {
				vkDestroyFramebuffer(logical_device.vk_device, framebuffer, nullptr);
			}

			this->m_created = false;
		}
	}
	void qpl::vk::framebuffers::construct() {
		this->m_created = false;
	}


	void qpl::vk::texture::create(
		const qpl::vk::physical_device& physical_device,
		const qpl::vk::logical_device& logical_device,
		const qpl::vk::command_pool& command_pool,
		const qpl::vk::present_queue& present_queue
	) {

		if (this->is_created()) {
			throw std::runtime_error("texture already created!");
			return;
		}
		if (this->m_path.empty()) {
			throw std::runtime_error("texture path empty!");
			return;
		}

		int tex_width, tex_height, tex_channels;
		stbi_uc* pixels = stbi_load(m_path.data(), &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);
		VkDeviceSize image_size = static_cast<VkDeviceSize>(tex_width)* tex_height * 4;
		this->mip_levels = static_cast<uint32_t>(std::floor(std::log2(std::max(tex_width, tex_height)))) + 1;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}


		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_memory;
		qpl::vk::create_buffer(logical_device.vk_device, physical_device.vk_device, staging_buffer, staging_buffer_memory, image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		vkMapMemory(logical_device.vk_device, staging_buffer_memory, 0, image_size, 0, &data);
		memcpy(data, pixels, static_cast<qpl::size>(image_size));
		vkUnmapMemory(logical_device.vk_device, staging_buffer_memory);

		stbi_image_free(pixels);

		qpl::vk::create_image(logical_device.vk_device, physical_device.vk_device, this->vk_image, this->vk_memory, tex_width, tex_height, this->mip_levels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		qpl::vk::transition_image_layout(logical_device.vk_device, command_pool.vk_command_pool, present_queue.vk_queue, this->vk_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, this->mip_levels);
		qpl::vk::copy_buffer_to_image(logical_device.vk_device, command_pool.vk_command_pool, present_queue.vk_queue, staging_buffer, this->vk_image, static_cast<uint32_t>(tex_width), static_cast<uint32_t>(tex_height));
		//qpl::vk::transition_image_layout(logical_device.vk_device, command_pool.vk_command_pool, present_queue.vk_queue, this->vk_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, this->mip_levels);

		vkDestroyBuffer(logical_device.vk_device, staging_buffer, nullptr);
		vkFreeMemory(logical_device.vk_device, staging_buffer_memory, nullptr);

		qpl::vk::generate_mipmaps(physical_device.vk_device, logical_device.vk_device, present_queue.vk_queue, command_pool.vk_command_pool, this->vk_image, VK_FORMAT_R8G8B8A8_UNORM, tex_width, tex_height, this->mip_levels);

		this->m_created = true;
	}
	void qpl::vk::texture::set_path(qpl::string_view path) {
		this->m_path = path;
	}
	bool qpl::vk::texture::is_created() const {
		return this->m_created;
	}
	void qpl::vk::texture::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyImage(logical_device.vk_device, this->vk_image, nullptr);
			vkFreeMemory(logical_device.vk_device, this->vk_memory, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::texture::construct() {
		this->m_created = false;
	}


	void qpl::vk::texture_view::create(const qpl::vk::logical_device& logical_device, const qpl::vk::texture& texture) {
		if (this->is_created()) {
			throw std::runtime_error("texture view already created!");
			return;
		}

		this->vk_image_view = qpl::vk::create_image_view(logical_device.vk_device, texture.vk_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, texture.mip_levels);

		VkSamplerCreateInfo sampler_info = {};
		sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler_info.magFilter = VK_FILTER_LINEAR;
		sampler_info.minFilter = VK_FILTER_LINEAR;
		sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.anisotropyEnable = VK_TRUE;
		sampler_info.maxAnisotropy = 16;
		sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		sampler_info.unnormalizedCoordinates = VK_FALSE;
		sampler_info.compareEnable = VK_FALSE;
		sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
		sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_info.minLod = 0;
		sampler_info.maxLod = static_cast<float>(texture.mip_levels);
		sampler_info.mipLodBias = 0;

		qpl::vk::result_check(vkCreateSampler(logical_device.vk_device, &sampler_info, nullptr, &this->vk_sampler), "failed to create texture sampler!");

		this->m_created = true;
	}
	bool qpl::vk::texture_view::is_created() const {
		return this->m_created;
	}
	void qpl::vk::texture_view::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroySampler(logical_device.vk_device, this->vk_sampler, nullptr);
			vkDestroyImageView(logical_device.vk_device, this->vk_image_view, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::texture_view::construct() {
		this->m_created = false;
	}

	void qpl::vk::vertex_buffer::create(
		const qpl::vk::physical_device& physical_device,
		const qpl::vk::logical_device& logical_device,
		const qpl::vk::command_pool& command_pool,
		const qpl::vk::present_queue& present_queue) {

		if (this->is_created()) {
			throw std::runtime_error("vertex buffer already created!");
			return;
		}

		VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size();

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_memory;
		qpl::vk::create_buffer(logical_device.vk_device, physical_device.vk_device, staging_buffer, staging_buffer_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		vkMapMemory(logical_device.vk_device, staging_buffer_memory, 0, buffer_size, 0, &data);
		memcpy(data, vertices.data(), static_cast<qpl::size>(buffer_size));
		vkUnmapMemory(logical_device.vk_device, staging_buffer_memory);

		qpl::vk::create_buffer(logical_device.vk_device, physical_device.vk_device, this->vk_buffer, this->vk_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		qpl::vk::copy_buffer(staging_buffer, this->vk_buffer, buffer_size, command_pool.vk_command_pool, logical_device.vk_device, present_queue.vk_queue);

		vkDestroyBuffer(logical_device.vk_device, staging_buffer, nullptr);
		vkFreeMemory(logical_device.vk_device, staging_buffer_memory, nullptr);

		this->m_created = true;
	}


	qpl::f32& qpl::vk::vertex_buffer::operator[](qpl::size index) {
		return this->vertices[index];
	}
	const qpl::f32& qpl::vk::vertex_buffer::operator[](qpl::size index) const {
		return this->vertices[index];
	}


	void qpl::vk::vertex_buffer::add(const qpl::vertex_2D& vertex) {
		this->vertices.reserve(this->size() + qpl::vertex_2D::size());
		this->vertices.push_back(vertex.position.x);
		this->vertices.push_back(vertex.position.y);
		this->vertices.push_back(vertex.color.r);
		this->vertices.push_back(vertex.color.g);
		this->vertices.push_back(vertex.color.b);
	}
	void qpl::vk::vertex_buffer::add(const qpl::vertex_3D& vertex) {
		this->vertices.reserve(this->size() + qpl::vertex_3D::size());
		this->vertices.push_back(vertex.position.x);
		this->vertices.push_back(vertex.position.y);
		this->vertices.push_back(vertex.position.z);
		this->vertices.push_back(vertex.color.r);
		this->vertices.push_back(vertex.color.g);
		this->vertices.push_back(vertex.color.b);
	}
	void qpl::vk::vertex_buffer::add(const qpl::vertex_2D_tex& vertex) {
		this->vertices.reserve(this->size() + qpl::vertex_2D_tex::size());
		this->vertices.push_back(vertex.position.x);
		this->vertices.push_back(vertex.position.y);
		this->vertices.push_back(vertex.color.r);
		this->vertices.push_back(vertex.color.g);
		this->vertices.push_back(vertex.color.b);
		this->vertices.push_back(vertex.tex_coord.x);
		this->vertices.push_back(vertex.tex_coord.y);

	}
	void qpl::vk::vertex_buffer::add(const qpl::vertex_3D_tex& vertex) {
		this->vertices.reserve(this->size() + qpl::vertex_3D_tex::size());
		this->vertices.push_back(vertex.position.x);
		this->vertices.push_back(vertex.position.y);
		this->vertices.push_back(vertex.position.z);
		this->vertices.push_back(vertex.color.r);
		this->vertices.push_back(vertex.color.g);
		this->vertices.push_back(vertex.color.b);
		this->vertices.push_back(vertex.tex_coord.x);
		this->vertices.push_back(vertex.tex_coord.y);
	}


	void qpl::vk::vertex_buffer::clear() {
		this->vertices.clear();
	}
	void qpl::vk::vertex_buffer::resize(qpl::size size) {
		this->vertices.resize(size);
	}
	void qpl::vk::vertex_buffer::reserve(qpl::size size) {
		this->vertices.reserve(size);
	}
	qpl::size qpl::vk::vertex_buffer::vertex_size() const {
		switch (this->vertex_binding) {
		case qpl::vk::vertex_binding::vertex_2D: return qpl::vertex_2D::size(); break;
		case qpl::vk::vertex_binding::vertex_3D: return qpl::vertex_3D::size(); break;
		case qpl::vk::vertex_binding::vertex_2D_tex: return qpl::vertex_2D_tex::size(); break;
		case qpl::vk::vertex_binding::vertex_3D_tex: return qpl::vertex_3D_tex::size(); break;
		default: return qpl::size{}; break;
		}
	}
	qpl::size qpl::vk::vertex_buffer::size() const {
		return this->vertices.size();
	}
	bool qpl::vk::vertex_buffer::empty() const {
		return this->vertices.empty();
	}
	qpl::size qpl::vk::vertex_buffer::capacity() const {
		return this->vertices.capacity();
	}
	void qpl::vk::vertex_buffer::set_vertex_binding(qpl::vk::vertex_binding vertex_binding) {
		this->vertex_binding = vertex_binding;
	}
	qpl::vk::vertex_binding qpl::vk::vertex_buffer::get_vertex_binding() const {
		return this->vertex_binding;
	}
	bool qpl::vk::vertex_buffer::is_created() const {
		return this->m_created;
	}
	void qpl::vk::vertex_buffer::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyBuffer(logical_device.vk_device, this->vk_buffer, nullptr);
			vkFreeMemory(logical_device.vk_device, this->vk_memory, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::vertex_buffer::construct() {
		this->m_created = false;
		this->vertex_binding = qpl::vk::vertex_binding::vertex_3D;
	}


	void qpl::vk::index_buffer::create(
		const qpl::vk::physical_device& physical_device,
		const qpl::vk::logical_device& logical_device,
		const qpl::vk::command_pool& command_pool,
		const qpl::vk::present_queue& present_queue) {

		if (this->is_created()) {
			throw std::runtime_error("index buffer already created!");
			return;
		}

		VkDeviceSize buffer_size = sizeof(indices[0]) * indices.size();

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_memory;
		qpl::vk::create_buffer(logical_device.vk_device, physical_device.vk_device, staging_buffer, staging_buffer_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		vkMapMemory(logical_device.vk_device, staging_buffer_memory, 0, buffer_size, 0, &data);
		memcpy(data, indices.data(), static_cast<qpl::size>(buffer_size));
		vkUnmapMemory(logical_device.vk_device, staging_buffer_memory);

		qpl::vk::create_buffer(logical_device.vk_device, physical_device.vk_device, this->vk_buffer, this->vk_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		qpl::vk::copy_buffer(staging_buffer, this->vk_buffer, buffer_size, command_pool.vk_command_pool, logical_device.vk_device, present_queue.vk_queue);

		vkDestroyBuffer(logical_device.vk_device, staging_buffer, nullptr);
		vkFreeMemory(logical_device.vk_device, staging_buffer_memory, nullptr);

		this->m_created = true;
	}

	qpl::u32& qpl::vk::index_buffer::operator[](qpl::size index) {
		return this->indices[index];
	}
	const qpl::u32& qpl::vk::index_buffer::operator[](qpl::size index) const {
		return this->indices[index];
	}
	qpl::u32& qpl::vk::index_buffer::back() {
		return this->indices.back();
	}
	const qpl::u32& qpl::vk::index_buffer::back() const {
		return this->indices.back();
	}
	void qpl::vk::index_buffer::add(qpl::size index) {
		this->indices.reserve(this->size() + 1ull);
		this->indices.push_back(index);
	}
	void qpl::vk::index_buffer::clear() {
		this->indices.clear();
	}
	void qpl::vk::index_buffer::resize(qpl::size size) {
		this->indices.resize(size);
	}
	void qpl::vk::index_buffer::reserve(qpl::size size) {
		this->indices.reserve(size);
	}
	qpl::size qpl::vk::index_buffer::size() const {
		return this->indices.size();
	}
	bool qpl::vk::index_buffer::empty() const {
		return this->indices.empty();
	}
	qpl::size qpl::vk::index_buffer::capacity() const {
		return this->indices.capacity();
	}
	bool qpl::vk::index_buffer::is_created() const {
		return this->m_created;
	}
	void qpl::vk::index_buffer::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyBuffer(logical_device.vk_device, this->vk_buffer, nullptr);
			vkFreeMemory(logical_device.vk_device, this->vk_memory, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::index_buffer::construct() {
		this->m_created = false;
	}



	qpl::size qpl::vk::uniform_buffer_object::size() const {
		return this->bytes.size();
	}

	bool qpl::vk::uniform_buffer_object::empty() const {
		return this->bytes.empty();
	}

	void qpl::vk::uniform_buffer_object::update_references() {
		for (auto i : this->references) {
			memcpy(this->bytes.data() + this->access_map[i.first].index, i.second, this->access_map[i.first].size);
		}
	}
	std::vector<std::string> qpl::vk::uniform_buffer_object::get_entry_list() const {
		std::vector<std::string> result;
		for (const auto& i : this->access_map) {
			result.push_back(i.first);
		}
		return result;
	}


	void qpl::vk::uniform_buffers::update_uniform_buffer_object(const qpl::vk::logical_device& logical_device, qpl::u32 current_image) {
		if (this->uniform_buffer_object.empty()) {
			return;
		}
		this->uniform_buffer_object.update_references();

		void* data;

		vkMapMemory(logical_device.vk_device, this->vk_uniform_buffers_memory[current_image], 0, this->uniform_buffer_object.size(), 0, &data);
		memcpy(data, this->uniform_buffer_object.bytes.data(), this->uniform_buffer_object.size());
		vkUnmapMemory(logical_device.vk_device, this->vk_uniform_buffers_memory[current_image]);
	}
	void qpl::vk::uniform_buffers::create(
		const qpl::vk::physical_device& physical_device,
		const qpl::vk::logical_device& logical_device,
		const qpl::vk::swapchain& swapchain) {
		if (this->is_created()) {
			throw std::runtime_error("uniform buffers already created!");
			return;
		}

		VkDeviceSize buffer_size = this->uniform_buffer_object.size();

		this->vk_uniform_buffers.resize(swapchain.vk_images.size());
		this->vk_uniform_buffers_memory.resize(swapchain.vk_images.size());

		for (size_t i = 0; i < swapchain.vk_images.size(); i++) {
			qpl::vk::create_buffer(logical_device.vk_device, physical_device.vk_device, this->vk_uniform_buffers[i], this->vk_uniform_buffers_memory[i], buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		}
		this->m_created = true;
	}
	bool qpl::vk::uniform_buffers::is_created() const {
		return this->m_created;
	}
	void qpl::vk::uniform_buffers::destroy(const qpl::vk::logical_device& logical_device, const qpl::vk::swapchain& swapchain) {
		if (this->is_created()) {
			for (size_t i = 0; i < swapchain.vk_images.size(); i++) {
				vkDestroyBuffer(logical_device.vk_device, this->vk_uniform_buffers[i], nullptr);
				vkFreeMemory(logical_device.vk_device, this->vk_uniform_buffers_memory[i], nullptr);
			}

			this->m_created = false;
		}
	}
	void qpl::vk::uniform_buffers::construct() {
		this->m_created = false;
	}


	void qpl::vk::descriptor_pool::create(const qpl::vk::logical_device& logical_device, const qpl::vk::swapchain& swapchain) {
		if (this->is_created()) {
			throw std::runtime_error("descriptor set layout already created!");
			return;
		}

		std::array<VkDescriptorPoolSize, 2> pool_sizes = {};
		pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		pool_sizes[0].descriptorCount = static_cast<uint32_t>(swapchain.vk_images.size());

		pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		pool_sizes[1].descriptorCount = static_cast<uint32_t>(swapchain.vk_images.size());

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount = static_cast<qpl::u32>(pool_sizes.size());
		pool_info.pPoolSizes = pool_sizes.data();
		pool_info.maxSets = static_cast<uint32_t>(swapchain.vk_images.size());

		qpl::vk::result_check(vkCreateDescriptorPool(logical_device.vk_device, &pool_info, nullptr, &this->vk_descriptor_pool), "failed to create descriptor pool!");

		this->m_created = true;
	}
	bool qpl::vk::descriptor_pool::is_created() const {
		return this->m_created;
	}
	void qpl::vk::descriptor_pool::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyDescriptorPool(logical_device.vk_device, this->vk_descriptor_pool, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::descriptor_pool::construct() {
		this->m_created = false;
	}


	void qpl::vk::descriptor_sets::create(
		const qpl::vk::logical_device& logical_device,
		const qpl::vk::swapchain& swapchain,
		const qpl::vk::descriptor_set_layout& descriptor_set_layout,
		const qpl::vk::descriptor_pool& descriptor_pool,
		const qpl::vk::uniform_buffers_proxy& uniform_buffers_proxy,
		const qpl::vk::texture_view& texture_view) {
		if (this->is_created()) {
			throw std::runtime_error("descriptor set layout already created!");
			return;
		}

		std::vector<VkDescriptorSetLayout> layouts(swapchain.vk_images.size(), descriptor_set_layout.vk_descriptor_set_layout);
		VkDescriptorSetAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool = descriptor_pool.vk_descriptor_pool;
		alloc_info.descriptorSetCount = static_cast<uint32_t>(swapchain.vk_images.size());
		alloc_info.pSetLayouts = layouts.data();

		this->vk_descriptor_sets.resize(swapchain.vk_images.size());


		qpl::vk::result_check(vkAllocateDescriptorSets(logical_device.vk_device, &alloc_info, this->vk_descriptor_sets.data()), "failed to allocate descriptor sets!");

		for (size_t i = 0; i < swapchain.vk_images.size(); i++) {
			VkDescriptorImageInfo image_info = {};
			image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			image_info.imageView = texture_view.vk_image_view;
			image_info.sampler = texture_view.vk_sampler;

			std::vector<VkWriteDescriptorSet> descriptor_writes = {};
			if (uniform_buffers_proxy) {
				VkDescriptorBufferInfo buffer_info = {};

				buffer_info.buffer = uniform_buffers_proxy->vk_uniform_buffers[i];
				buffer_info.offset = 0;
				buffer_info.range = uniform_buffers_proxy->uniform_buffer_object.size();

				descriptor_writes.push_back(VkWriteDescriptorSet{});
				descriptor_writes.back().sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptor_writes.back().dstSet = this->vk_descriptor_sets[i];
				descriptor_writes.back().dstBinding = uniform_buffers_proxy->uniform_buffer_object.binding;
				descriptor_writes.back().dstArrayElement = 0;
				descriptor_writes.back().descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptor_writes.back().descriptorCount = 1;
				descriptor_writes.back().pBufferInfo = &buffer_info;
			}

			descriptor_writes.push_back(VkWriteDescriptorSet{});
			descriptor_writes.back().sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptor_writes.back().dstSet = this->vk_descriptor_sets[i];
			descriptor_writes.back().dstBinding = 1;
			descriptor_writes.back().dstArrayElement = 0;
			descriptor_writes.back().descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptor_writes.back().descriptorCount = 1;
			descriptor_writes.back().pImageInfo = &image_info;

			vkUpdateDescriptorSets(logical_device.vk_device, static_cast<uint32_t>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
		}

		this->m_created = true;
	}
	bool qpl::vk::descriptor_sets::is_created() const {
		return this->m_created;
	}
	void qpl::vk::descriptor_sets::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {


			this->m_created = false;
		}
	}
	void qpl::vk::descriptor_sets::construct() {
		this->m_created = false;
	}


	void qpl::vk::command_buffers::create(
		const qpl::vk::logical_device& logical_device,
		const qpl::vk::swapchain& swapchain,
		const qpl::vk::render_pass& render_pass,
		const qpl::vk::framebuffers& framebuffers,
		const qpl::vk::command_pool& command_pool,
		const qpl::vk::scene_collection& scenes,
		qpl::vk::vulkan& vulkan
	) {
		if (this->is_created()) {
			throw std::runtime_error("command buffers already created!");
			return;
		}

		this->vk_command_buffers.resize(framebuffers.vk_framebuffers.size());

		VkCommandBufferAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandPool = command_pool.vk_command_pool;
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandBufferCount = static_cast<qpl::u32>(this->vk_command_buffers.size());

		qpl::vk::result_check(vkAllocateCommandBuffers(logical_device.vk_device, &alloc_info, this->vk_command_buffers.data()), "failed to allocate command buffers!");

		for (size_t i = 0; i < this->vk_command_buffers.size(); i++) {
			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			//begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			qpl::vk::result_check(vkBeginCommandBuffer(this->vk_command_buffers[i], &begin_info), "failed to begin recording command buffer!");


			VkRenderPassBeginInfo render_pass_info = {};
			render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			render_pass_info.renderPass = render_pass.vk_render_pass;
			render_pass_info.framebuffer = framebuffers.vk_framebuffers[i];
			render_pass_info.renderArea.offset = { 0, 0 };
			render_pass_info.renderArea.extent = swapchain.vk_extent;

			std::array<VkClearValue, 3> clear_values = {};
			clear_values[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
			clear_values[1].color = { { 0.0f, 0.0f, 0.1f, 1.0f } };
			clear_values[2].depthStencil = { 1.0f, 0 };

			render_pass_info.clearValueCount = static_cast<qpl::u32>(clear_values.size());
			render_pass_info.pClearValues = clear_values.data();

			vkCmdBeginRenderPass(this->vk_command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

			bool delay = false;
			for (const auto& s : scenes) {
				if (delay) {
					//vkCmdNextSubpass(this->vk_command_buffers[i], VK_SUBPASS_CONTENTS_INLINE);
				}
				VkBuffer vertex_buffers[1] = {};
				VkDeviceSize offsets[] = { 0 };

				if (vulkan.get_vertex_buffer(s.second.vertex_buffer_name)) {
					vertex_buffers[0] = vulkan.get_vertex_buffer(s.second.vertex_buffer_name)->vk_buffer;
					vkCmdBindVertexBuffers(this->vk_command_buffers[i], static_cast<qpl::u32>(vulkan.get_vertex_buffer(s.second.vertex_buffer_name)->get_vertex_binding()), 1u, vertex_buffers, offsets);
					//vkCmdBindVertexBuffers(this->vk_command_buffers[i], 0, 1u, vertex_buffers, offsets);
				}
				if (vulkan.get_index_buffer(s.second.index_buffer_name)) {
					vkCmdBindIndexBuffer(this->vk_command_buffers[i], vulkan.get_index_buffer(s.second.index_buffer_name)->vk_buffer, 0, VK_INDEX_TYPE_UINT32);
				}
				if (!vulkan.get_descriptor_sets(s.second.descriptor_sets_name)) {
					throw std::runtime_error(qpl::to_string("couldn't find descriptor set \"", s.second.descriptor_sets_name, "\""));
				}
				vkCmdBindDescriptorSets(this->vk_command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, s.second.graphics_pipeline.vk_pipeline_layout, 0, 1, &vulkan.get_descriptor_sets(s.second.descriptor_sets_name)->vk_descriptor_sets[i], 0, nullptr);
				vkCmdBindPipeline(this->vk_command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, s.second.graphics_pipeline.vk_pipeline);

				if (vulkan.get_index_buffer(s.second.index_buffer_name)) {
					vkCmdDrawIndexed(this->vk_command_buffers[i], static_cast<uint32_t>(vulkan.get_index_buffer(s.second.index_buffer_name)->indices.size()), 1, 0, 0, 0);
				}
				else {
					//qpl::println("vkCmdDraw [", s.first, "]");
					vkCmdDraw(this->vk_command_buffers[i], 3, 1, 0, 0);
				}


				delay = true;
			}


			vkCmdEndRenderPass(this->vk_command_buffers[i]);

			qpl::vk::result_check(vkEndCommandBuffer(this->vk_command_buffers[i]), "failed to record command buffer!");

		}
		this->m_created = true;
	}
	bool qpl::vk::command_buffers::is_created() const {
		return this->m_created;
	}
	void qpl::vk::command_buffers::destroy(const qpl::vk::logical_device& logical_device, const qpl::vk::command_pool& command_pool) {
		if (this->is_created()) {
			vkFreeCommandBuffers(logical_device.vk_device, command_pool.vk_command_pool, static_cast<uint32_t>(this->vk_command_buffers.size()), this->vk_command_buffers.data());

			this->m_created = false;
		}
	}
	void qpl::vk::command_buffers::construct() {
		this->m_created = false;
	}


	void qpl::vk::semaphore::create(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			throw std::runtime_error("semaphore already created!");
			return;
		}

		VkSemaphoreCreateInfo semaphore_info = {};
		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		qpl::vk::result_check(vkCreateSemaphore(logical_device.vk_device, &semaphore_info, nullptr, &this->vk_semaphore), "failed to create semaphore!");

		this->m_created = true;
	}
	bool qpl::vk::semaphore::is_created() const {
		return this->m_created;
	}
	void qpl::vk::semaphore::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroySemaphore(logical_device.vk_device, this->vk_semaphore, nullptr);

			this->m_created = false;
		}
	}
	void qpl::vk::semaphore::construct() {
		this->m_created = false;
	}


	void qpl::vk::fence::create(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			return;
		}
		VkFenceCreateInfo fence_info = {};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		qpl::vk::result_check(vkCreateFence(logical_device.vk_device, &fence_info, nullptr, &this->vk_fence), "failed to create fence!");

		this->m_created = true;
	}
	bool qpl::vk::fence::is_created() const {
		return this->m_created;
	}
	void qpl::vk::fence::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			vkDestroyFence(logical_device.vk_device, this->vk_fence, nullptr);
		}

		this->m_created = false;
	}
	void qpl::vk::fence::construct() {
		this->m_created = false;
	}


	void qpl::vk::sync_objects::wait_for_fence(const qpl::vk::logical_device& logical_device) {
		vkWaitForFences(logical_device.vk_device, 1, &this->get_current_in_flight_fence().vk_fence, VK_TRUE, UINT64_MAX);
	}
	void qpl::vk::sync_objects::reset_fence(const qpl::vk::logical_device& logical_device) {
		vkResetFences(logical_device.vk_device, 1, &this->get_current_in_flight_fence().vk_fence);
	}
	qpl::vk::fence& qpl::vk::sync_objects::get_current_in_flight_fence() {
		return this->in_flight_fences[this->get_current_frame()];
	}
	const qpl::vk::fence& qpl::vk::sync_objects::get_current_in_flight_fence() const {
		return this->in_flight_fences[this->get_current_frame()];
	}
	const qpl::vk::semaphore& qpl::vk::sync_objects::get_current_image_available_semaphore() const {
		return this->image_available_semaphores[this->get_current_frame()];
	}
	qpl::vk::semaphore& qpl::vk::sync_objects::get_current_image_available_semaphore() {
		return this->image_available_semaphores[this->get_current_frame()];
	}
	const qpl::vk::semaphore& qpl::vk::sync_objects::get_current_render_finished_semaphore() const {
		return this->render_finished_semaphores[this->get_current_frame()];
	}
	qpl::vk::semaphore& qpl::vk::sync_objects::get_current_render_finished_semaphore() {
		return this->render_finished_semaphores[this->get_current_frame()];
	}

	qpl::u32 qpl::vk::sync_objects::get_current_frame() const {
		return this->m_frame;
	}
	void qpl::vk::sync_objects::advance_frame() {
		this->m_frame = (this->m_frame + qpl::u32{ 1 }) % static_cast<qpl::u32>(this->get_number_of_frames());
	}
	void qpl::vk::sync_objects::set_number_of_frames(qpl::size size) {
		this->m_size = size;
	}
	qpl::size qpl::vk::sync_objects::get_number_of_frames() const {
		return this->m_size;
	}
	void qpl::vk::sync_objects::create(const qpl::vk::logical_device& logical_device, const qpl::vk::swapchain& swapchain) {
		if (this->is_created()) {
			throw std::runtime_error("sync objects already created!");
			return;
		}

		this->image_available_semaphores.resize(this->get_number_of_frames());
		this->render_finished_semaphores.resize(this->get_number_of_frames());
		this->in_flight_fences.resize(this->get_number_of_frames());
		this->images_in_flight.resize(swapchain.vk_images.size());

		for (auto i = qpl::size{}; i < this->get_number_of_frames(); ++i) {
			this->image_available_semaphores[i].create(logical_device);
			this->render_finished_semaphores[i].create(logical_device);
			this->in_flight_fences[i].create(logical_device);
		}

		for (auto& i : this->images_in_flight) {
			i.vk_fence = VK_NULL_HANDLE;
		}

		this->m_created = true;
	}
	bool qpl::vk::sync_objects::is_created() const {
		return this->m_created;
	}
	void qpl::vk::sync_objects::destroy(const qpl::vk::logical_device& logical_device) {
		if (this->is_created()) {
			for (auto i = qpl::size{}; i < this->get_number_of_frames(); ++i) {
				this->image_available_semaphores[i].destroy(logical_device);
				this->render_finished_semaphores[i].destroy(logical_device);
				this->in_flight_fences[i].destroy(logical_device);
			}

			this->m_created = false;
		}

	}
	void qpl::vk::sync_objects::construct() {
		this->m_created = false;
		this->m_size = qpl::size{ 2u };
		this->m_frame = qpl::u32{};
	}

	void qpl::vk::scene_proxy::disable_vertex_buffer() {
		(*this)->vertex_buffer_name = "";
	}
	bool qpl::vk::scene_proxy::is_vertex_buffer_enabled() const {
		return !(*this)->vertex_buffer_name.empty();
	}
	bool qpl::vk::scene_proxy::is_vertex_buffer_disabled() const {
		return (*this)->vertex_buffer_name.empty();
	}
	void qpl::vk::scene_proxy::set_vertex_buffer(const std::string& vertex_buffer_name) {
		(*this)->vertex_buffer_name = vertex_buffer_name;
	}

	void qpl::vk::scene_proxy::disable_index_buffer() {
		(*this)->index_buffer_name = "";
	}
	bool qpl::vk::scene_proxy::is_index_buffer_enabled() const {
		return !(*this)->index_buffer_name.empty();
	}
	bool qpl::vk::scene_proxy::is_index_buffer_disabled() const {
		return (*this)->index_buffer_name.empty();
	}
	void qpl::vk::scene_proxy::set_index_buffer(const std::string& index_buffer_name) {
		(*this)->index_buffer_name = index_buffer_name;
	}
	void qpl::vk::scene_proxy::set_vertex_shader(const std::string& vertex_shader_path) {
		(*this)->vertex_shader.set_path(vertex_shader_path);
	}
	void qpl::vk::scene_proxy::set_fragment_shader(const std::string& fragment_shader_path) {
		(*this)->fragment_shader.set_path(fragment_shader_path);
	}

	void qpl::vk::scene_proxy::disable_descriptor_sets() {
		(*this)->descriptor_sets_name = "";
	}
	bool qpl::vk::scene_proxy::is_descriptor_sets_enabled() const {
		return !(*this)->descriptor_sets_name.empty();
	}
	bool qpl::vk::scene_proxy::is_descriptor_sets_disabled() const {
		return (*this)->descriptor_sets_name.empty();
	}
	void qpl::vk::scene_proxy::set_descriptor_sets(const std::string& descriptor_sets_name) {
		this->ptr->get_descriptor_sets(descriptor_sets_name);
		(*this)->descriptor_sets_name = descriptor_sets_name;
	}
	void qpl::vk::scene_proxy::disable_uniform_buffers() {
		(*this)->uniform_buffers_name = "";
	}
	bool qpl::vk::scene_proxy::is_uniform_buffers_enabled() const {
		return !(*this)->uniform_buffers_name.empty();
	}
	bool qpl::vk::scene_proxy::is_uniform_buffers_disabled() const {
		return (*this)->uniform_buffers_name.empty();
	}
	void qpl::vk::scene_proxy::set_uniform_buffers(const std::string& uniform_buffers_name) {
		this->ptr->get_uniform_buffers(uniform_buffers_name);
		(*this)->uniform_buffers_name = uniform_buffers_name;
	}
	qpl::vk::scene* qpl::vk::scene_proxy::operator->() {
		auto it = this->ptr->scene_collection.find(this->scene_name);
		if (it == this->ptr->scene_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find scene ", this->scene_name).c_str());
		}
		return &it->second;
	}
	const qpl::vk::scene* qpl::vk::scene_proxy::operator->() const {
		auto it = this->ptr->scene_collection.find(this->scene_name);
		if (it == this->ptr->scene_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find scene ", this->scene_name).c_str());
		}
		return &it->second;
	}
	qpl::vk::scene& qpl::vk::scene_proxy::operator*() {
		return *this->operator->();
	}
	const qpl::vk::scene& qpl::vk::scene_proxy::operator*() const {
		return *this->operator->();
	}
	qpl::vk::scene_proxy::operator bool() const {
		auto it = this->ptr->scene_collection.find(this->scene_name);
		return it != this->ptr->scene_collection.end();
	}
	bool qpl::vk::scene_proxy::operator!() const {
		return !this->operator bool();
	}
	std::string qpl::vk::scene_proxy::name() const {
		return this->scene_name;
	}

	void qpl::vk::uniform_buffers_proxy::set_uniform_binding(qpl::u32 binding) {
		(*this)->uniform_buffer_object.binding = binding;
	}
	void qpl::vk::uniform_buffers_proxy::activate_on_vertex_shader() {
		(*this)->uniform_buffer_object.vertex_shader = true;
	}
	void qpl::vk::uniform_buffers_proxy::deactivate_on_vertex_shader() {
		(*this)->uniform_buffer_object.vertex_shader = false;
	}
	bool qpl::vk::uniform_buffers_proxy::is_activated_on_vertex_shader() const {
		return (*this)->uniform_buffer_object.vertex_shader;
	}
	void qpl::vk::uniform_buffers_proxy::activate_on_fragment_shader() {
		(*this)->uniform_buffer_object.fragment_shader = true;
	}
	void qpl::vk::uniform_buffers_proxy::deactivate_on_fragment_shader() {
		(*this)->uniform_buffer_object.fragment_shader = false;
	}
	bool qpl::vk::uniform_buffers_proxy::is_activated_on_fragment_shader() const {
		return (*this)->uniform_buffer_object.fragment_shader;
	}

	qpl::vk::uniform_buffers* qpl::vk::uniform_buffers_proxy::operator->() {
		auto it = this->ptr->uniform_buffers_collection.find(this->uniform_buffers_name);
		if (it == this->ptr->uniform_buffers_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find uniform buffers ", this->uniform_buffers_name).c_str());
		}
		return &it->second;
	}

	const qpl::vk::uniform_buffers* qpl::vk::uniform_buffers_proxy::operator->() const {
		auto it = this->ptr->uniform_buffers_collection.find(this->uniform_buffers_name);
		if (it == this->ptr->uniform_buffers_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find uniform buffers ", this->uniform_buffers_name).c_str());
		}
		return &it->second;
	}
	qpl::vk::uniform_buffers& qpl::vk::uniform_buffers_proxy::operator*() {
		return *this->operator->();
	}
	const qpl::vk::uniform_buffers& qpl::vk::uniform_buffers_proxy::operator*() const {
		return *this->operator->();
	}
	qpl::vk::uniform_buffers_proxy::operator bool() const {
		auto it = this->ptr->uniform_buffers_collection.find(this->uniform_buffers_name);
		return it != this->ptr->uniform_buffers_collection.end();
	}
	bool qpl::vk::uniform_buffers_proxy::operator!() const {
		return !this->operator bool();
	}
	std::string qpl::vk::uniform_buffers_proxy::name() const {
		return this->uniform_buffers_name;
	}

	qpl::vk::vertex_buffer* qpl::vk::vertex_buffer_proxy::operator->() {
		auto it = this->ptr->vertex_buffer_collection.find(this->vertex_buffer_name);
		if (it == this->ptr->vertex_buffer_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find vertex buffer ", this->vertex_buffer_name).c_str());
		}
		return &it->second;
	}

	const qpl::vk::vertex_buffer* qpl::vk::vertex_buffer_proxy::operator->() const {
		auto it = this->ptr->vertex_buffer_collection.find(this->vertex_buffer_name);
		if (it == this->ptr->vertex_buffer_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find vertex buffer ", this->vertex_buffer_name).c_str());
		}
		return &it->second;
	}
	qpl::vk::vertex_buffer& qpl::vk::vertex_buffer_proxy::operator*() {
		return *this->operator->();
	}
	const qpl::vk::vertex_buffer& qpl::vk::vertex_buffer_proxy::operator*() const {
		return *this->operator->();
	}
	qpl::vk::vertex_buffer_proxy::operator bool() const {
		auto it = this->ptr->vertex_buffer_collection.find(this->vertex_buffer_name);
		return it != this->ptr->vertex_buffer_collection.end();
	}
	bool qpl::vk::vertex_buffer_proxy::operator!() const {
		return !this->operator bool();
	}
	std::string qpl::vk::vertex_buffer_proxy::name() const {
		return this->vertex_buffer_name;
	}

	qpl::vk::index_buffer* qpl::vk::index_buffer_proxy::operator->() {
		auto it = this->ptr->index_buffer_collection.find(this->index_buffer_name);
		if (it == this->ptr->index_buffer_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find index buffer ", this->index_buffer_name).c_str());
		}
		return &it->second;
	}

	const qpl::vk::index_buffer* qpl::vk::index_buffer_proxy::operator->() const {
		auto it = this->ptr->index_buffer_collection.find(this->index_buffer_name);
		if (it == this->ptr->index_buffer_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find index buffer ", this->index_buffer_name).c_str());
		}
		return &it->second;
	}
	qpl::vk::index_buffer& qpl::vk::index_buffer_proxy::operator*() {
		return *this->operator->();
	}
	const qpl::vk::index_buffer& qpl::vk::index_buffer_proxy::operator*() const {
		return *this->operator->();
	}
	qpl::vk::index_buffer_proxy::operator bool() const {
		auto it = this->ptr->index_buffer_collection.find(this->index_buffer_name);
		return it != this->ptr->index_buffer_collection.end();
	}
	bool qpl::vk::index_buffer_proxy::operator!() const {
		return !this->operator bool();
	}
	std::string qpl::vk::index_buffer_proxy::name() const {
		return this->index_buffer_name;
	}

	qpl::vk::descriptor_sets* qpl::vk::descriptor_sets_proxy::operator->() {
		auto it = this->ptr->descriptor_sets_collection.find(this->descriptor_sets_name);
		if (it == this->ptr->descriptor_sets_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find descriptor sets ", this->descriptor_sets_name).c_str());
		}
		return &it->second;
	}

	const qpl::vk::descriptor_sets* qpl::vk::descriptor_sets_proxy::operator->() const {
		auto it = this->ptr->descriptor_sets_collection.find(this->descriptor_sets_name);
		if (it == this->ptr->descriptor_sets_collection.end()) {
			throw std::runtime_error(qpl::to_string("couldn't find descriptor sets ", this->descriptor_sets_name).c_str());
		}
		return &it->second;
	}
	qpl::vk::descriptor_sets& qpl::vk::descriptor_sets_proxy::operator*() {
		return *this->operator->();
	}
	const qpl::vk::descriptor_sets& qpl::vk::descriptor_sets_proxy::operator*() const {
		return *this->operator->();
	}
	qpl::vk::descriptor_sets_proxy::operator bool() const {
		auto it = this->ptr->descriptor_sets_collection.find(this->descriptor_sets_name);
		return it != this->ptr->descriptor_sets_collection.end();
	}
	bool qpl::vk::descriptor_sets_proxy::operator!() const {
		return !this->operator bool();
	}
	std::string qpl::vk::descriptor_sets_proxy::name() const {
		return this->descriptor_sets_name;
	}

	void qpl::vk::vulkan::create() {
		if (this->is_print_stuff_enabled()) qpl::println("vulkan [setup] . . . ");

		this->create_window();
		this->create_instance();
		this->create_surface();
		this->create_validation_layer();
		this->create_physical_device();
		this->create_present_queue();
		this->create_logical_device();
		this->create_swapchain();
		this->create_image_views();
		this->create_render_pass();
		this->create_command_pool();
		this->create_color_resources();
		this->create_depth_resources();
		this->create_framebuffers();
		this->create_texture();
		this->create_texture_view();
		this->create_descriptor_pool();
		this->create_descriptor_set_layouts();
		this->create_graphics_pipelines();
		this->create_vertex_buffers();
		this->create_index_buffers();
		this->create_uniform_buffers();
		this->create_descriptor_sets();
		this->create_command_buffers();
		this->create_sync_objects();

		if (this->is_print_stuff_enabled()) qpl::println("vulkan [setup done]");
		if (this->is_print_stuff_enabled()) qpl::println_repeat('-', 40);
	}
	void qpl::vk::vulkan::destroy() {
		if (this->is_print_stuff_enabled()) qpl::println("vulkan [cleanup] . . . ");

		this->wait_for_device();

		this->destroy_sync_objects();
		this->destroy_command_buffers();
		this->destroy_descriptor_sets();
		this->destroy_uniform_buffers();
		this->destroy_index_buffer();
		this->destroy_vertex_buffers();
		this->destroy_graphics_pipelines();
		this->destroy_descriptor_set_layout();
		this->destroy_descriptor_pool();
		this->destroy_depth_resources();
		this->destroy_color_resources();
		this->destroy_texture_view();
		this->destroy_texture();
		this->destroy_framebuffers();
		this->destroy_command_pool();
		this->destroy_render_pass();
		this->destroy_image_views();
		this->destroy_swapchain();
		this->destroy_logical_device();
		this->destroy_validation_layer();
		this->destroy_present_queue();
		this->destroy_physical_device();
		this->destroy_surface();
		this->destroy_instance();
		this->destroy_window();


		if (this->is_print_stuff_enabled()) qpl::println("vulkan [cleanup done]");
		if (this->is_print_stuff_enabled()) qpl::println_repeat('-', 40);
	}
	void qpl::vk::vulkan::recreate() {
		this->wait_for_device();
		this->destroy();
		this->create();
	}
	void qpl::vk::vulkan::recreate_swapchain() {
		if (this->is_print_stuff_enabled()) qpl::println("vulkan [recreating swapchain] . . . ");

		qpl::window::consider_minimize();
		this->wait_for_device();


		this->destroy_framebuffers();
		this->destroy_depth_resources();
		this->destroy_color_resources();
		this->destroy_command_buffers();
		this->destroy_graphics_pipelines();
		this->destroy_render_pass();
		this->destroy_image_views();
		this->destroy_swapchain();
		this->destroy_uniform_buffers();
		this->destroy_descriptor_sets();
		this->destroy_descriptor_pool();


		this->create_swapchain();
		this->create_image_views();
		this->create_render_pass();
		this->create_color_resources();
		this->create_depth_resources();
		this->create_graphics_pipelines();
		this->create_framebuffers();
		this->create_uniform_buffers();
		this->create_descriptor_pool();
		this->create_descriptor_sets();
		this->create_command_buffers();


		if (this->is_print_stuff_enabled()) qpl::println("vulkan [recreating swapchain done]");
		if (this->is_print_stuff_enabled()) qpl::println_repeat('-', 40);
	}
	void qpl::vk::vulkan::recreate_buffers() {
		this->wait_for_device();
		this->destroy_command_buffers();
		this->destroy_vertex_buffers();
		this->destroy_index_buffer();

		this->create_index_buffers();
		this->create_vertex_buffers();
		this->create_command_buffers();
	}

	void qpl::vk::vulkan::add_scene(const std::string& scene_name) {
		this->scene_collection.insert(std::make_pair(scene_name, qpl::vk::scene{}));
	}
	qpl::vk::scene_proxy qpl::vk::vulkan::get_scene(const std::string& scene_name) {
		return qpl::vk::scene_proxy(scene_name, *this);
	}
	void qpl::vk::vulkan::add_descriptor_sets(const std::string& descriptor_sets_name) {
		this->descriptor_sets_collection.insert(std::make_pair(descriptor_sets_name, qpl::vk::descriptor_sets{}));
	}
	qpl::vk::descriptor_sets_proxy qpl::vk::vulkan::get_descriptor_sets(const std::string& descriptor_sets_name) {
		return qpl::vk::descriptor_sets_proxy(descriptor_sets_name, *this);
	}

	void qpl::vk::vulkan::add_uniform_buffers(const std::string& uniform_buffers_name) {
		this->uniform_buffers_collection.insert(std::make_pair(uniform_buffers_name, qpl::vk::uniform_buffers{}));
	}
	qpl::vk::uniform_buffers_proxy qpl::vk::vulkan::get_uniform_buffers(const std::string& uniform_buffers_name) {
		return qpl::vk::uniform_buffers_proxy(uniform_buffers_name, *this);
	}

	void qpl::vk::vulkan::add_vertex_buffer(const std::string& vertex_buffer_name) {
		this->vertex_buffer_collection.insert(std::make_pair(vertex_buffer_name, qpl::vk::vertex_buffer{}));
	}
	qpl::vk::vertex_buffer_proxy qpl::vk::vulkan::get_vertex_buffer(const std::string& vertex_buffer_name) {
		return qpl::vk::vertex_buffer_proxy(vertex_buffer_name, *this);
	}

	void qpl::vk::vulkan::add_index_buffer(const std::string& index_buffer_name) {
		this->index_buffer_collection.insert(std::make_pair(index_buffer_name, qpl::vk::index_buffer{}));
	}
	qpl::vk::index_buffer_proxy qpl::vk::vulkan::get_index_buffer(const std::string& index_buffer_name) {
		return qpl::vk::index_buffer_proxy(index_buffer_name, *this);
	}


	qpl::u32 qpl::vk::vulkan::get_current_frame() const {
		return this->sync_objects.get_current_frame();
	}
	void qpl::vk::vulkan::enable_vsync() {
		this->swapchain.enable_vsync();
	}
	void qpl::vk::vulkan::disable_vsync() {
		this->swapchain.disable_vsync();
	}
	bool qpl::vk::vulkan::is_vsync_enabled() const {
		return this->swapchain.is_vsync_enabled();
	}

	void qpl::vk::vulkan::enable_multisampling() {
		this->physical_device.enable_multisampling();
	}
	void qpl::vk::vulkan::disable_multisampling() {
		this->physical_device.disable_multisampling();
	}
	bool qpl::vk::vulkan::ismultisampling_enabled() const {
		return this->physical_device.is_multisampling_enabled();
	}
	bool qpl::vk::vulkan::is_open() const {
		return qpl::window::is_open();
	}
	void qpl::vk::vulkan::close() {
		qpl::window::close();
	}
	void qpl::vk::vulkan::draw() {
		if (!qpl::window::is_open()) {
			return;
		}
		this->sync_objects.wait_for_fence(this->logical_device);

		qpl::u32 image_index;
		auto result = vkAcquireNextImageKHR(this->logical_device.vk_device, this->swapchain.vk_swapchain, UINT64_MAX, this->sync_objects.get_current_image_available_semaphore().vk_semaphore, VK_NULL_HANDLE, &image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			this->recreate_swapchain();
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		for (auto& i : this->uniform_buffers_collection) {
			i.second.update_uniform_buffer_object(this->logical_device, image_index);
		}

		if (this->sync_objects.images_in_flight[image_index].vk_fence != VK_NULL_HANDLE) {
			vkWaitForFences(this->logical_device.vk_device, 1, &this->sync_objects.images_in_flight[image_index].vk_fence, VK_TRUE, UINT64_MAX);
		}
		this->sync_objects.images_in_flight[image_index].vk_fence = this->sync_objects.get_current_in_flight_fence().vk_fence;

		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore wait_semaphores[] = { this->sync_objects.get_current_image_available_semaphore().vk_semaphore };

		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = wait_semaphores;
		submit_info.pWaitDstStageMask = wait_stages;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &this->command_buffers.vk_command_buffers[image_index];

		VkSemaphore signal_semaphores[] = { this->sync_objects.get_current_render_finished_semaphore().vk_semaphore };
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = signal_semaphores;

		this->sync_objects.reset_fence(this->logical_device);

		qpl::vk::result_check(vkQueueSubmit(this->present_queue.vk_queue, 1, &submit_info, this->sync_objects.get_current_in_flight_fence().vk_fence), "failed to submit draw command buffer!");


		VkPresentInfoKHR present_info = {};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = signal_semaphores;

		VkSwapchainKHR swap_chains[] = { this->swapchain.vk_swapchain };
		present_info.swapchainCount = 1;
		present_info.pSwapchains = &this->swapchain.vk_swapchain;
		present_info.pImageIndices = &image_index;
		present_info.pResults = nullptr;


		result = vkQueuePresentKHR(this->present_queue.vk_queue, &present_info);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			this->recreate_swapchain();
		}
		else {
			qpl::vk::result_check(result, "failed to present swap chain image!");
		}


		this->sync_objects.advance_frame();
	}

	void qpl::vk::vulkan::wait_for_device() {
		if (this->logical_device.is_created()) {
			vkDeviceWaitIdle(this->logical_device.vk_device);
		}
	}

	void qpl::vk::vulkan::create_window() {
		if (!qpl::window::is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - window [setup] . . . ");
			qpl::window::create();
			if (this->is_print_stuff_enabled()) qpl::println(" - window [setup done]");
		}
	}
	void qpl::vk::vulkan::create_instance() {
		if (!this->instance.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan instance [setup] . . . ");
			this->instance.create(this->validation_layer);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan instance [setup done]");
		}

	}
	void qpl::vk::vulkan::create_surface() {
		if (!this->surface.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan surface [setup] . . . ");
			this->surface.create(this->instance);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan surface [setup done]");
		}
	}
	void qpl::vk::vulkan::create_validation_layer() {
		if constexpr (validation_layer::is_enabled()) {
			if (!this->validation_layer.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan validation layer [setup] . . . ");
				this->validation_layer.create(this->instance);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan validation layer [setup done]");
			}
		}
	}
	void qpl::vk::vulkan::create_physical_device() {
		if (!this->physical_device.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan physical device [setup] . . . ");
			this->physical_device.create(this->instance, this->surface);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan physical device [setup done]");
		}
	}
	void qpl::vk::vulkan::create_present_queue() {
		if (!this->present_queue.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan present queue [setup] . . . ");
			this->present_queue.create(this->physical_device, this->surface);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan present queue [setup done]");
		}
	}
	void qpl::vk::vulkan::create_logical_device() {
		if (!this->logical_device.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan logical device [setup] . . . ");
			this->logical_device.create(this->physical_device, this->validation_layer, this->surface, this->present_queue);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan logical device [setup done]");
		}
	}
	void qpl::vk::vulkan::create_swapchain() {
		if (!this->swapchain.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan swapchain [setup] . . . ");
			this->swapchain.create(this->physical_device, this->logical_device, this->surface, qpl::window::get_dimension());
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan swapchain [setup done]");
		}
	}
	void qpl::vk::vulkan::create_image_views() {
		if (!this->image_views.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan image view [setup] . . . ");
			this->image_views.create(this->logical_device, this->swapchain);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan image view [setup done]");
		}
	}
	void qpl::vk::vulkan::create_render_pass() {
		if (!this->render_pass.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan render pass [setup] . . . ");
			this->render_pass.create(this->logical_device, this->physical_device, this->swapchain);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan render pass [setup done]");
		}
	}
	void qpl::vk::vulkan::create_command_pool() {
		if (!this->command_pool.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan command pool [setup] . . . ");
			this->command_pool.create(this->physical_device, this->surface, this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan command pool [setup done]");
		}
	}
	void qpl::vk::vulkan::create_color_resources() {
		if (!this->color_resources.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan color resources [setup] . . . ");
			this->color_resources.create(this->logical_device, this->physical_device, this->swapchain);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan color resources [setup done]");
		}
	}
	void qpl::vk::vulkan::create_depth_resources() {
		if (!this->depth_resources.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan depth resources [setup] . . . ");
			this->depth_resources.create(this->logical_device, this->physical_device, this->swapchain);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan depth resources [setup done]");
		}
	}
	void qpl::vk::vulkan::create_framebuffers() {
		if (!this->framebuffers.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan framebuffers [setup] . . . ");
			this->framebuffers.create(this->logical_device, this->swapchain, this->render_pass, this->image_views, this->color_resources, this->depth_resources);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan framebuffers [setup done]");
		}
	}
	void qpl::vk::vulkan::create_texture() {
		if (!this->texture.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan texture [setup] . . . ");
			this->texture.create(this->physical_device, this->logical_device, this->command_pool, this->present_queue);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan texture [setup done]");
		}
	}
	void qpl::vk::vulkan::create_texture_view() {
		if (!this->texture_view.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan texture view [setup] . . . ");
			this->texture_view.create(this->logical_device, this->texture);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan texture view [setup done]");
		}
	}
	void qpl::vk::vulkan::create_descriptor_pool() {
		for (auto& i : this->scene_collection) {
			if (!i.second.descriptor_pool.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor pool layout [", i.first, " setup] . . . ");
				i.second.descriptor_pool.create(this->logical_device, this->swapchain);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor pool layout [", i.first, " setup done]");
			}
		}
	}
	void qpl::vk::vulkan::create_descriptor_set_layouts() {
		for (auto& i : this->scene_collection) {
			if (!i.second.descriptor_set_layout.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor set layout [", i.first, " setup] . . . ");
				i.second.descriptor_set_layout.create(this->logical_device, this->get_uniform_buffers(i.second.uniform_buffers_name));
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor set layout [", i.first, " setup done]");
			}
		}
	}
	void qpl::vk::vulkan::create_graphics_pipelines() {
		for (auto& i : this->scene_collection) {
			if (!i.second.graphics_pipeline.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan graphics pipeline [", i.first, " setup] . . . ");
				i.second.graphics_pipeline.create(
					this->physical_device,
					this->logical_device,
					this->swapchain,
					this->render_pass,
					i.second.descriptor_set_layout,
					i.second.vertex_shader,
					i.second.fragment_shader,
					this->get_vertex_buffer(i.second.vertex_buffer_name)
				);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan graphics pipeline [", i.first, " setup done]");
			}
		}
	}
	void qpl::vk::vulkan::create_vertex_buffers() {
		for (auto& i : this->scene_collection) {
			if (this->get_vertex_buffer(i.second.vertex_buffer_name) && !this->get_vertex_buffer(i.second.vertex_buffer_name)->is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan vertex buffer [", i.first, " setup] . . . ");
				this->get_vertex_buffer(i.second.vertex_buffer_name)->create(
					this->physical_device,
					this->logical_device,
					this->command_pool,
					this->present_queue
				);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan vertex buffer [", i.first, " setup done]");
			}
		}
	}
	void qpl::vk::vulkan::create_index_buffers() {
		for (auto& i : this->scene_collection) {
			if (this->get_index_buffer(i.second.index_buffer_name) && !this->get_index_buffer(i.second.index_buffer_name)->is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan index buffer [", i.first, " setup] . . . ");
				this->get_index_buffer(i.second.index_buffer_name)->create(
					this->physical_device,
					this->logical_device,
					this->command_pool,
					this->present_queue
				);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan index buffer [", i.first, " setup done]");
			}
		}
	}
	void qpl::vk::vulkan::create_uniform_buffers() {
		for (auto& i : this->scene_collection) {
			if (this->get_uniform_buffers(i.second.uniform_buffers_name) && !this->get_uniform_buffers(i.second.uniform_buffers_name)->is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan uniform buffers [", i.first, " setup] . . . ");
				this->get_uniform_buffers(i.second.uniform_buffers_name)->create(this->physical_device, this->logical_device, this->swapchain);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan uniform buffers [", i.first, " setup done]");
			}
		}
	}
	void qpl::vk::vulkan::create_descriptor_sets() {
		for (auto& i : this->scene_collection) {
			if (this->get_descriptor_sets(i.second.descriptor_sets_name) && !this->get_descriptor_sets(i.second.descriptor_sets_name)->is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor sets [", i.first, " setup] . . . ");
				this->get_descriptor_sets(i.second.descriptor_sets_name)->create(
					this->logical_device,
					this->swapchain,
					i.second.descriptor_set_layout,
					i.second.descriptor_pool,
					this->get_uniform_buffers(i.second.uniform_buffers_name),
					this->texture_view);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor sets [", i.first, " setup done]");
			}
		}
	}
	void qpl::vk::vulkan::create_command_buffers() {
		if (!this->command_buffers.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan command buffers [setup] . . . ");
			this->command_buffers.create(
				this->logical_device,
				this->swapchain,
				this->render_pass,
				this->framebuffers,
				this->command_pool,
				this->scene_collection,
				*this
			);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan command buffers [setup done]");
		}
	}
	void qpl::vk::vulkan::create_sync_objects() {
		if (!this->sync_objects.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan sync objects [setup] . . . ");
			this->sync_objects.create(this->logical_device, this->swapchain);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan sync objects [setup done]");
		}
	}

	void qpl::vk::vulkan::destroy_sync_objects() {
		if (this->sync_objects.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan sync objects [cleanup]. . . ");
			this->sync_objects.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan sync objects [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_command_buffers() {
		if (this->command_buffers.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan command buffers [cleanup]. . . ");
			this->command_buffers.destroy(this->logical_device, this->command_pool);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan command buffers [cleanup done]");
		}
	}



	void qpl::vk::vulkan::destroy_descriptor_sets() {
		for (auto& i : this->descriptor_sets_collection) {
			if (i.second.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan uniform buffers [", i.first, " cleanup]. . . ");
				i.second.destroy(this->logical_device);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan uniform buffers [", i.first, " cleanup done]");
			}
		}
	}
	void qpl::vk::vulkan::destroy_uniform_buffers() {
		for (auto& i : this->uniform_buffers_collection) {
			if (i.second.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor sets [", i.first, " cleanup]. . . ");
				i.second.destroy(this->logical_device, this->swapchain);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor sets [", i.first, " cleanup done]");
			}
		}
	}
	void qpl::vk::vulkan::destroy_index_buffer() {
		for (auto& i : this->index_buffer_collection) {
			if (i.second.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan index buffer [", i.first, " cleanup]. . . ");
				i.second.destroy(this->logical_device);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan index buffer [", i.first, " cleanup done]");
			}
		}
	}
	void qpl::vk::vulkan::destroy_vertex_buffers() {
		for (auto& i : this->vertex_buffer_collection) {
			if (i.second.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan vertex buffer [", i.first, " cleanup]. . . ");
				i.second.destroy(this->logical_device);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan vertex buffer [", i.first, " cleanup done]");
			}
		}
	}
	void qpl::vk::vulkan::destroy_graphics_pipelines() {
		for (auto& i : this->scene_collection) {
			if (i.second.graphics_pipeline.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan graphics pipeline [", i.first, " cleanup]. . . ");
				i.second.graphics_pipeline.destroy(this->logical_device);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan graphics pipeline [", i.first, " cleanup done]");
			}
		}
	}
	void qpl::vk::vulkan::destroy_descriptor_set_layout() {
		for (auto& i : this->scene_collection) {
			if (i.second.descriptor_set_layout.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor set layout [", i.first, " cleanup]. . . ");
				i.second.descriptor_set_layout.destroy(this->logical_device);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor set layout [", i.first, " cleanup done]");
			}
		}
	}

	void qpl::vk::vulkan::destroy_descriptor_pool() {
		for (auto& i : this->scene_collection) {
			if (i.second.descriptor_pool.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor pool [", i.first, " cleanup]. . . ");
				i.second.descriptor_pool.destroy(this->logical_device);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan descriptor pool [", i.first, " cleanup done]");
			}
		}
	}
	void qpl::vk::vulkan::destroy_texture_view() {
		if (this->texture_view.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan texture view [cleanup]. . . ");
			this->texture_view.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan texture view[cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_texture() {
		if (this->texture.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan texture [cleanup]. . . ");
			this->texture.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan texture [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_framebuffers() {
		if (this->framebuffers.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan framebuffers [cleanup]. . . ");
			this->framebuffers.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan framebuffers [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_depth_resources() {
		if (this->depth_resources.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan depth resources [cleanup]. . . ");
			this->depth_resources.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan depth resources [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_color_resources() {
		if (this->color_resources.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan color resources [cleanup]. . . ");
			this->color_resources.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan color resources [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_command_pool() {
		if (this->command_pool.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan command pool [cleanup]. . . ");
			this->command_pool.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan command pool [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_render_pass() {
		if (this->render_pass.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan render pass [cleanup]. . . ");
			this->render_pass.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan render pass [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_image_views() {
		if (this->image_views.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan image view [cleanup]. . . ");
			this->image_views.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan image view [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_swapchain() {
		if (this->swapchain.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan swapchain [cleanup]. . . ");
			this->swapchain.destroy(this->logical_device);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan swapchain [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_logical_device() {
		if (this->logical_device.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan logical device [cleanup]. . . ");
			this->logical_device.destroy();
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan logical device [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_validation_layer() {
		if constexpr (validation_layer::is_enabled()) {
			if (this->validation_layer.is_created()) {
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan validation layer [cleanup] . . . ");
				this->validation_layer.destroy(this->instance);
				if (this->is_print_stuff_enabled()) qpl::println(" - vulkan validation layer [cleanup done]");
			}
		}
	}
	void qpl::vk::vulkan::destroy_present_queue() {
		if (this->present_queue.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan present queue [cleanup] . . . ");
			this->present_queue.destroy();
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan present queue [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_physical_device() {
		if (this->physical_device.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan physical device [cleanup] . . . ");
			this->physical_device.destroy();
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan physical device [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_surface() {
		if (this->surface.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan surface [cleanup]. . . ");
			this->surface.destroy(this->instance);
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan surface [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_instance() {
		if (this->instance.is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan instance [cleanup]. . . ");
			this->instance.destroy();
			if (this->is_print_stuff_enabled()) qpl::println(" - vulkan instance [cleanup done]");
		}
	}
	void qpl::vk::vulkan::destroy_window() {
		if (qpl::window::is_created()) {
			if (this->is_print_stuff_enabled()) qpl::println(" - window [cleanup]. . . ");
			qpl::window::destroy();
			if (this->is_print_stuff_enabled()) qpl::println(" - window [cleanup done]");
		}
	}


	void qpl::vk::vulkan::enable_print_stuff() {
		this->m_print_stuff = true;
	}
	void qpl::vk::vulkan::disable_print_stuff() {
		this->m_print_stuff = false;
	}
	bool qpl::vk::vulkan::is_print_stuff_enabled() const {
		return this->m_print_stuff;
	}
	void qpl::vk::vulkan::construct() {
		this->m_print_stuff = true;
	}
	void qpl::vk::application::create() {
		this->vulkan.create();
	}
	void qpl::vk::application::destroy() {
		this->vulkan.destroy();
	}
	void qpl::vk::application::recreate() {
		this->vulkan.recreate();
	}
	void qpl::vk::application::recreate_swapchain() {
		this->vulkan.recreate_swapchain();
	}
	void qpl::vk::application::recreate_buffers() {
		this->vulkan.recreate_buffers();
	}
	void qpl::vk::application::update() {
		static glm::vec2 mouse_pos = this->mouse_position();
		static qpl::clock clock;
		this->m_frame_time = clock.elapsed_f_reset();
		qpl::window::poll_events();
		if (qpl::window::dimension_changed()) {
			this->recreate_swapchain();
			return;
		}
		this->m_delta_mouse = this->mouse_position() - mouse_pos;
		mouse_pos = this->mouse_position();
	}


	bool qpl::vk::application::is_open() const {
		return qpl::window::is_open();
	}
	void qpl::vk::application::close() {
		qpl::window::close();
	}
	void qpl::vk::application::draw() {
		this->vulkan.draw();
		++this->m_number_of_frames;
	}

	void qpl::vk::application::wait_for_device() {
		this->vulkan.wait_for_device();
	}
	void qpl::vk::application::set_dimension(glm::ivec2 dim) const {
		qpl::window::set_dimension(dim);
	}
	glm::ivec2 qpl::vk::application::get_dimension() const {
		return qpl::window::get_dimension();
	}
	void qpl::vk::application::set_title(qpl::string_view title) const {
		qpl::window::set_title(title);
	}
	qpl::string_view qpl::vk::application::get_title() const {
		return qpl::window::get_title();
	}

	qpl::time qpl::vk::application::get_run_time() const {
		return this->m_run_time_clock.elapsed();
	}


	glm::vec2 qpl::vk::application::mouse_position() const {
		return qpl::window::mouse_position();
	}
	glm::vec2 qpl::vk::application::delta_mouse_position() const {
		return this->m_delta_mouse;
	}
	bool qpl::vk::application::left_mouse_clicked() const {
		return qpl::window::left_mouse_clicked();
	}
	bool qpl::vk::application::left_mouse_holding() const {
		return qpl::window::left_mouse_holding();
	}
	bool qpl::vk::application::left_mouse_released() const {
		return qpl::window::left_mouse_released();
	}
	bool qpl::vk::application::right_mouse_clicked() const {
		return qpl::window::right_mouse_clicked();
	}
	bool qpl::vk::application::right_mouse_holding() const {
		return qpl::window::right_mouse_holding();
	}
	bool qpl::vk::application::right_mouse_released() const {
		return qpl::window::right_mouse_released();
	}
	bool qpl::vk::application::scrolled_up() const {
		return qpl::window::scrolled_up();
	}
	bool qpl::vk::application::scrolled_down() const {
		return qpl::window::scrolled_down();
	}
	bool qpl::vk::application::key_pressed() const {
		return qpl::window::key_pressed();
	}
	bool qpl::vk::application::key_released() const {
		return qpl::window::key_released();
	}
	bool qpl::vk::application::key_pressed(qpl::i32 key) const {
		return qpl::window::key_pressed(key);
	}
	bool qpl::vk::application::key_pressed_utf(qpl::u32 key) const {
		return qpl::window::key_pressed_utf(key);
	}
	bool qpl::vk::application::key_released(qpl::i32 key) const {
		return qpl::window::key_pressed(key);
	}
	bool qpl::vk::application::key_holding() const {
		return qpl::window::key_holding();
	}
	bool qpl::vk::application::key_holding(qpl::i32 key) const {
		return qpl::window::key_holding(key);
	}

	qpl::f64 qpl::vk::application::get_frame_time() const {
		return this->m_frame_time;
	}
	qpl::u32 qpl::vk::application::get_current_frame() const {
		return this->vulkan.get_current_frame();
	}
	qpl::size qpl::vk::application::get_number_of_frames() const {
		return this->m_number_of_frames;
	}

	void qpl::vk::application::enable_print_stuff() {
		this->vulkan.enable_print_stuff();
	}
	void qpl::vk::application::disable_print_stuff() {
		this->vulkan.disable_print_stuff();
	}
	bool qpl::vk::application::is_print_stuff_enabled() const {
		return this->vulkan.is_print_stuff_enabled();
	}
	void qpl::vk::application::hide_cursor() {
		glfwSetInputMode(qpl::window::p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	void qpl::vk::application::show_cursor() {
		glfwSetInputMode(qpl::window::p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}




	void qpl::vk::application::construct() {
		this->m_number_of_frames = qpl::size{};
		this->m_frame_time = 0.0;
		this->m_delta_mouse = { 0, 0 };
	}

}
#pragma warning(pop) 

#endif



