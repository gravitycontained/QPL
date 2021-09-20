#ifndef QPL_VULKAN_HPP
#define QPL_VULKAN_HPP
#pragma once

#if defined(QPL_USE_VULKAN) || defined(QPL_USE_ALL)

#include <qpl/qpldeclspec.hpp>
#include <qpl/objects.hpp>
#include <qpl/time.hpp>
#include <qpl/vardef.hpp>
#include <qpl/window.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <unordered_map>
#include <map>
#include <optional>


namespace qpl {
	namespace vk {
		struct queue_family_indices {
			std::optional<qpl::u32> graphics_family;
			std::optional<qpl::u32> present_family;

			QPLDLL bool is_complete() const;
		};
		struct swapchain_support_details {
			swapchain_support_details() {
				this->capabilities = VkSurfaceCapabilitiesKHR{};
			}
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> present_modes;

			QPLDLL VkSurfaceFormatKHR choose_swap_surface_format() const;
			QPLDLL VkPresentModeKHR choose_swap_present_mode() const;
			QPLDLL VkExtent2D choose_swap_extent(glm::uvec2 dimension) const;
		};


		QPLDLL const extern std::vector<const char*> validation_layers;
		QPLDLL const extern std::vector<const char*> device_extensions;


		QPLDLL void result_check(VkResult result, const std::string& message);
		QPLDLL qpl::u32 rate_physical_device(VkPhysicalDevice physical_device);
		QPLDLL std::vector<const char*> get_required_extensions();
		QPLDLL qpl::vk::queue_family_indices find_queue_families(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
		QPLDLL bool has_device_extensions_support(VkPhysicalDevice physical_device);
		QPLDLL bool is_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
		QPLDLL qpl::vk::swapchain_support_details query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
		QPLDLL VkShaderModule create_shader_module(VkDevice device, const std::string& code);
		QPLDLL qpl::u32 find_memory_type(VkPhysicalDevice physical_device, qpl::u32 type_filter, VkMemoryPropertyFlags properties);
		QPLDLL void create_buffer(VkDevice logical_device, VkPhysicalDevice physical_device, VkBuffer& buffer, VkDeviceMemory& buffer_memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		QPLDLL void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size, VkCommandPool command_pool, VkDevice logical_device, VkQueue present_queue);
		QPLDLL VkCommandBuffer begin_single_time_commands(VkDevice logical_device, VkCommandPool command_pool);
		QPLDLL void end_single_time_commands(VkDevice logical_device, VkQueue present_queue, VkCommandPool command_pool, VkCommandBuffer& command_buffer);
		QPLDLL void create_image(VkDevice logical_device, VkPhysicalDevice physical_device, VkImage& image, VkDeviceMemory& image_memory, qpl::u32 width, qpl::u32 height, qpl::u32 mip_levels, VkSampleCountFlagBits num_samples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
		QPLDLL void transition_image_layout(VkDevice logical_device, VkCommandPool command_pool, VkQueue present_queue, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout, qpl::u32 mip_levels);
		QPLDLL void copy_buffer_to_image(VkDevice logical_device, VkCommandPool command_pool, VkQueue present_queue, VkBuffer buffer, VkImage image, qpl::u32 width, qpl::u32 height);
		QPLDLL VkImageView create_image_view(VkDevice logical_device, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags, qpl::u32 mip_levels);
		VkFormat find_supported_format(VkPhysicalDevice physical_device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		QPLDLL VkFormat find_depth_format(VkPhysicalDevice physical_device);
		QPLDLL void generate_mipmaps(VkPhysicalDevice physical_device, VkDevice logical_device, VkQueue present_queue, VkCommandPool command_pool, VkImage image, VkFormat imageFormat, int32_t tex_width, int32_t tex_height, uint32_t mip_levels);
		QPLDLL VkSampleCountFlagBits get_max_usable_sample_count(VkPhysicalDevice physical_device);


		class validation_layer;
		class instance {
		public:
			VkInstance vk_instance;

			instance() {
				this->construct();
			}

			QPLDLL std::vector<VkExtensionProperties> get_extensions() const;
			QPLDLL void create(const qpl::vk::validation_layer& validation_layer);
			QPLDLL bool is_created() const;
			QPLDLL void destroy();

		private:
			QPLDLL void construct();

			bool m_created;
		};


		class validation_layer {
		public:
			VkDebugUtilsMessengerEXT vk_debug_messenger;
			validation_layer() {
				this->construct();
			}

			constexpr static bool is_enabled() {
				return

#ifdef _DEBUG
					true;
#else
					false;
#endif
			}
			QPLDLL static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
				VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
				VkDebugUtilsMessageTypeFlagsEXT message_type,
				const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
				void* p_user_data);


			QPLDLL static std::vector<VkLayerProperties> get_available_validation_layers();
			QPLDLL bool check_support() const;

			QPLDLL bool is_created() const;
			QPLDLL void create(const qpl::vk::instance& instance);
			QPLDLL void destroy(const qpl::vk::instance& instance);

		private:
			QPLDLL void construct();
			bool m_created;
		};


		class surface {
		public:
			VkSurfaceKHR vk_surface;

			surface() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::instance& instance);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::instance& instance);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class physical_device {
		public:
			VkPhysicalDevice vk_device;
			VkSampleCountFlagBits msaa_samples;
			bool multisampling;

			physical_device() {
				this->construct();
			}
			QPLDLL void create(const qpl::vk::instance& instance, const qpl::vk::surface& surface);
			QPLDLL bool is_created() const;
			QPLDLL void destroy();

			QPLDLL void enable_multisampling();
			QPLDLL void disable_multisampling();
			QPLDLL bool is_multisampling_enabled() const;

			QPLDLL void print_physical_devices() const;
			QPLDLL queue_family_indices get_queue_family_indices(const qpl::vk::surface& surface) const;

		private:
			QPLDLL void construct();
			bool m_created;
			std::multimap<qpl::u32, VkPhysicalDevice> m_physical_devices;
		};


		class present_queue {
		public:
			VkQueue vk_queue;
			std::vector<VkDeviceQueueCreateInfo> vk_queue_create_infos;
			present_queue() {
				this->construct();
			}


			QPLDLL void create(const qpl::vk::physical_device& physical_device, const qpl::vk::surface& surface);
			QPLDLL bool is_created() const;
			QPLDLL void destroy();
		private:
			QPLDLL void construct();
			bool m_created;
		};

		class logical_device {
		public:
			VkDevice vk_device;
			VkQueue vk_graphics_queue;

			logical_device() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::physical_device& physical_device,
				const qpl::vk::validation_layer& validation_layer,
				const qpl::vk::surface& surface,
				qpl::vk::present_queue& present_queue);

			QPLDLL bool is_created() const;
			QPLDLL void destroy();
		private:
			QPLDLL void construct();
			bool m_created;
		};


		class swapchain {
		public:
			VkSwapchainKHR vk_swapchain;
			std::vector<VkImage> vk_images;
			VkFormat vk_image_format;
			VkExtent2D vk_extent;
			VkPresentModeKHR vk_present_mode;

			swapchain() {
				this->contruct();
			}

			QPLDLL void create(const qpl::vk::physical_device& physical_device,
				qpl::vk::logical_device& logical_device,
				const qpl::vk::surface& surface,
				glm::ivec2 dimension);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
			QPLDLL void enable_vsync();
			QPLDLL void disable_vsync();
			QPLDLL bool is_vsync_enabled() const;
		private:
			QPLDLL void contruct();

			bool m_created;
		};


		class frame_buffer_attachment {
		public:
			VkImage vk_image;
			VkDeviceMemory vk_memory;
			VkImageView vk_image_view;
			VkFormat vk_format;

			frame_buffer_attachment() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::logical_device& logical_device, const qpl::vk::physical_device& physical_device, const qpl::vk::swapchain& swapchain, VkImageUsageFlagBits usage_flag, VkFormat format);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};
		struct attachment {
			frame_buffer_attachment color, depth;
		};
		using attachments = std::vector<qpl::vk::attachment>;


		class image_views {
		public:
			std::vector<VkImageView> vk_image_views;
			image_views() {
				this->construct();
			}

			QPLDLL void create(qpl::vk::logical_device& logical_device, const qpl::vk::swapchain& swapchain);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class shader {
		public:
			VkShaderModule vk_module;
			shader() {
				this->construct();
			}

			QPLDLL void set_path(const std::string& path);
			QPLDLL std::string get_path() const;
			QPLDLL void create(const qpl::vk::logical_device& logical_device);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
			std::string m_path;
		};

		class render_pass {
		public:
			VkRenderPass vk_render_pass;
			//qpl::vk::attachments attachments;
			render_pass() {
				this->construct();
			}


			QPLDLL void create(const qpl::vk::logical_device& logical_device, const qpl::vk::physical_device& physical_device, const qpl::vk::swapchain& swapchain);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class vulkan;
		class uniform_buffers_proxy;

		struct uniform_buffer_object;
		class descriptor_set_layout {
		public:
			VkDescriptorSetLayout vk_descriptor_set_layout;
			qpl::u32 binding;
			descriptor_set_layout() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::logical_device& logical_device, const qpl::vk::uniform_buffers_proxy& ubo_proxy);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
			QPLDLL void set_binding(qpl::u32 binding);
			QPLDLL qpl::u32 get_binding() const;
		private:

			QPLDLL void construct();

			bool m_created;
		};

		class vertex_buffer;

		class graphics_pipeline {
		public:
			VkPipelineLayout vk_pipeline_layout;
			VkPipeline vk_pipeline;

			graphics_pipeline() {
				this->construct();
			}

			QPLDLL void create(
				const qpl::vk::physical_device& physical_device,
				const qpl::vk::logical_device& logical_device,
				const qpl::vk::swapchain& swapchain,
				const qpl::vk::render_pass& render_pass,
				const qpl::vk::descriptor_set_layout& descriptor_set_layout,
				qpl::vk::shader& vertex_shader,
				qpl::vk::shader& fragment_shader,
				bool enable_vertices
			);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};
		class command_pool {
		public:
			VkCommandPool vk_command_pool;

			command_pool() {
				this->construct();
			}

			QPLDLL void create(
				const qpl::vk::physical_device& physical_device,
				const qpl::vk::surface& surface,
				const qpl::vk::logical_device& logical_device);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class color_resources {
		public:
			VkImage vk_image;
			VkDeviceMemory vk_memory;
			VkImageView vk_image_view;

			color_resources() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::logical_device& logical_device, const qpl::vk::physical_device& physical_device, const qpl::vk::swapchain& swapchain);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class depth_resources {
		public:
			VkImage vk_image;
			VkDeviceMemory vk_memory;
			VkImageView vk_image_view;

			depth_resources() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::logical_device& logical_device, const qpl::vk::physical_device& physical_device, const qpl::vk::swapchain& swapchain);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class framebuffers {
		public:
			std::vector<VkFramebuffer> vk_framebuffers;

			framebuffers() {
				this->construct();
			}
			QPLDLL void create(
				const qpl::vk::logical_device& logical_device,
				const qpl::vk::swapchain& swapchain,
				const qpl::vk::render_pass& render_pass,
				const qpl::vk::image_views& image_views,
				const qpl::vk::color_resources& color_resources,
				const qpl::vk::depth_resources& depth_resources);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};


		class texture {
		public:
			qpl::u32 mip_levels;
			VkImage vk_image;
			VkDeviceMemory vk_memory;

			texture() {
				this->construct();
			}

			QPLDLL void create(
				const qpl::vk::physical_device& physical_device,
				const qpl::vk::logical_device& logical_device,
				const qpl::vk::command_pool& command_pool,
				const qpl::vk::present_queue& present_queue
			);
			QPLDLL void set_path(std::string_view path);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
			std::string_view m_path;
		};

		class texture_view {
		public:
			VkImageView vk_image_view;
			VkSampler vk_sampler;

			texture_view() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::logical_device& logical_device, const qpl::vk::texture& texture);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class vertex_buffer {
		public:
			VkBuffer vk_buffer;
			VkDeviceMemory vk_memory;
			std::vector<qpl::f32> vertices;
			qpl::vk::vertex_binding vertex_binding;
			qpl::size offset;

			vertex_buffer() {
				this->construct();
			}

			QPLDLL void create(
				const qpl::vk::physical_device& physical_device, 
				const qpl::vk::logical_device& logical_device,
				const qpl::vk::command_pool& command_pool,
				const qpl::vk::present_queue& present_queue
			);
			QPLDLL qpl::f32& operator[](qpl::size index);
			QPLDLL const qpl::f32& operator[](qpl::size index) const;

			QPLDLL void add(const qpl::vertex_2D& vertex);
			QPLDLL void add(const qpl::vertex_3D& vertex);
			QPLDLL void add(const qpl::vertex_2D_tex& vertex);
			QPLDLL void add(const qpl::vertex_3D_tex& vertex);

			QPLDLL void clear();
			QPLDLL void resize(qpl::size size);
			QPLDLL void reserve(qpl::size size);
			QPLDLL qpl::size vertex_size() const;
			QPLDLL qpl::size size() const;
			QPLDLL bool empty() const;
			QPLDLL qpl::size capacity() const;
			QPLDLL void set_vertex_binding(qpl::vk::vertex_binding vertex_binding);
			QPLDLL qpl::vk::vertex_binding get_vertex_binding() const;
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class index_buffer {
		public:
			VkBuffer vk_buffer;
			VkDeviceMemory vk_memory;

			std::vector<qpl::u32> indices;

			index_buffer() {
				this->construct();
			}

			QPLDLL void create(
				const qpl::vk::physical_device& physical_device,
				const qpl::vk::logical_device& logical_device,
				const qpl::vk::command_pool& command_pool,
				const qpl::vk::present_queue& present_queue
			);
			QPLDLL qpl::u32& operator[](qpl::size index);
			QPLDLL const qpl::u32& operator[](qpl::size index) const;
			QPLDLL qpl::u32& back();
			QPLDLL const qpl::u32& back() const;

			QPLDLL void add(qpl::size index);

			QPLDLL void clear();
			QPLDLL void resize(qpl::size size);
			QPLDLL void reserve(qpl::size size);
			QPLDLL qpl::size size() const;
			QPLDLL bool empty() const;
			QPLDLL qpl::size capacity() const;
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		using vertex_buffer_collection = std::unordered_map<std::string, qpl::vk::vertex_buffer>;
		using index_buffer_collection = std::unordered_map<std::string, qpl::vk::index_buffer>;


		struct projection_object {
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};

		struct uniform_buffer_object {
			struct range {
				qpl::size index;
				qpl::size size;
			};
			std::vector<std::byte> bytes;

			std::unordered_map<std::string, qpl::vk::uniform_buffer_object::range> access_map;
			std::unordered_map<std::string, void*> references;
			qpl::u32 binding = ~qpl::u32{};
			bool fragment_shader = false;
			bool vertex_shader = false;
			
			template<class C>
			void add_type(const std::string& access_name) {
				if (this->access_map.find(access_name) != this->access_map.cend()) {
					throw std::runtime_error("trying to add a type to uniform_buffer_object but the type is already contained");
					return;
				}
				this->access_map[access_name].index = this->bytes.size();
				this->access_map[access_name].size = sizeof(C);
				this->bytes.resize(this->bytes.size() + sizeof(C));
			}

			template<class C>
			void set_value(const std::string& access_name, const C& value) {
				if (this->access_map.find(access_name) == this->access_map.cend()) {
					this->add_type<C>(access_name);
				}
				memcpy(this->bytes.data() + this->access_map[access_name].index, &value, this->access_map[access_name].size);
			}
			template<class C>
			void add_reference(const std::string& access_name, C& value) {
				if (this->access_map.find(access_name) == this->access_map.cend()) {
					this->add_type<C>(access_name);
				}
				this->references[access_name] = reinterpret_cast<void*>(&value);
				memcpy(this->bytes.data() + this->access_map[access_name].index, &value, this->access_map[access_name].size);
			}
			template<class C>
			C get_value(const std::string& access_name) {
				C value;
				if (this->access_map.find(access_name) == this->access_map.cend()) {
					this->add_type<C>(access_name);
				}
				memcpy(&value, this->bytes.data() + this->access_map[access_name].index, this->access_map[access_name].size);
				return value;
			}
			QPLDLL void update_references();
			QPLDLL std::vector<std::string> get_entry_list() const;

			QPLDLL qpl::size size() const;
			QPLDLL bool empty() const;
		};


		class uniform_buffers {
		public:
			std::vector<VkBuffer> vk_uniform_buffers;
			std::vector<VkDeviceMemory> vk_uniform_buffers_memory;
			qpl::vk::uniform_buffer_object uniform_buffer_object;


			uniform_buffers() {
				this->construct();
			}

			QPLDLL void create(
				const qpl::vk::physical_device& physical_device, 
				const qpl::vk::logical_device& logical_device,
				const qpl::vk::swapchain& swapchain
			);
			QPLDLL void update_uniform_buffer_object(const qpl::vk::logical_device& logical_device, qpl::u32 current_image);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device, const qpl::vk::swapchain& swapchain);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		using uniform_buffers_collection = std::unordered_map<std::string, uniform_buffers>;

		class descriptor_pool {
		public:
			VkDescriptorPool vk_descriptor_pool;

			descriptor_pool() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::logical_device& logical_device, const qpl::vk::swapchain& swapchain);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};


		class descriptor_sets {
		public:
			std::vector<VkDescriptorSet> vk_descriptor_sets;

			descriptor_sets() {
				this->construct();
			}

			QPLDLL void create(
				const qpl::vk::logical_device& logical_device,
				const qpl::vk::swapchain& swapchain,
				const qpl::vk::descriptor_set_layout& descriptor_set_layout,
				const qpl::vk::descriptor_pool& descriptor_pool,
				const qpl::vk::uniform_buffers_proxy& uniform_buffers_proxy,
				const qpl::vk::texture_view& texture_view
			);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		using descriptor_sets_collection = std::unordered_map<std::string, descriptor_sets>;

		struct scene {
			qpl::vk::shader fragment_shader;
			qpl::vk::shader vertex_shader;
			qpl::vk::descriptor_pool descriptor_pool;
			qpl::vk::descriptor_set_layout descriptor_set_layout;
			qpl::vk::graphics_pipeline graphics_pipeline;
			std::string vertex_buffer_name;
			std::string index_buffer_name;
			std::string uniform_buffers_name;
			std::string descriptor_sets_name;
		};

		using scene_collection = std::unordered_map<std::string, scene>;

		struct scene_build {
			std::vector<qpl::vk::shader> fragment_shader;
			std::vector<qpl::vk::shader> vertex_shader;
			std::vector<qpl::vk::uniform_buffers> uniform_buffers;
			std::vector<qpl::vk::graphics_pipeline> pipelines;
			std::vector<qpl::vk::descriptor_set_layout> descriptor_set_layouts;

		};


		class command_buffers {
		public:
			std::vector<VkCommandBuffer> vk_command_buffers;
			command_buffers() {
				this->construct();
			}

			QPLDLL void create(
				const qpl::vk::logical_device& logical_device,
				const qpl::vk::swapchain& swapchain,
				const qpl::vk::render_pass& render_pass,
				const qpl::vk::framebuffers& framebuffers,
				const qpl::vk::command_pool& command_pool,
				const qpl::vk::scene_collection& scenes,
				qpl::vk::vulkan& vulkan
				);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device, const qpl::vk::command_pool& command_pool);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class semaphore {
		public:
			VkSemaphore vk_semaphore;
			semaphore() {
				this->construct();
			}

			QPLDLL void create(const qpl::vk::logical_device& logical_device);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class fence {
		public:
			VkFence vk_fence;
			fence() {
				this->construct();
			}


			QPLDLL void create(const qpl::vk::logical_device& logical_device);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			bool m_created;
		};

		class sync_objects {
		public:
			std::vector<qpl::vk::semaphore> image_available_semaphores;
			std::vector<qpl::vk::semaphore> render_finished_semaphores;
			std::vector<qpl::vk::fence> in_flight_fences;
			std::vector<qpl::vk::fence> images_in_flight;
			sync_objects() {
				this->construct();
			}

			QPLDLL void wait_for_fence(const qpl::vk::logical_device& logical_device);
			QPLDLL void reset_fence(const qpl::vk::logical_device& logical_device);
			QPLDLL qpl::vk::fence& get_current_in_flight_fence();
			QPLDLL const qpl::vk::fence& get_current_in_flight_fence() const;
			QPLDLL const qpl::vk::semaphore& get_current_image_available_semaphore() const;
			QPLDLL qpl::vk::semaphore& get_current_image_available_semaphore();
			QPLDLL const qpl::vk::semaphore& get_current_render_finished_semaphore() const;
			QPLDLL qpl::vk::semaphore& get_current_render_finished_semaphore();
			QPLDLL qpl::u32 get_current_frame() const;
			QPLDLL void advance_frame();
			QPLDLL void set_number_of_frames(qpl::size size);
			QPLDLL qpl::size get_number_of_frames() const;

			QPLDLL void create(const qpl::vk::logical_device& logical_device, const qpl::vk::swapchain& swapchain);
			QPLDLL bool is_created() const;
			QPLDLL void destroy(const qpl::vk::logical_device& logical_device);
		private:
			QPLDLL void construct();

			qpl::u32 m_frame;
			qpl::size m_size;
			bool m_created;
		};


		class scene_proxy {
		public:
			scene_proxy(const std::string& name, vulkan& vulkan) : scene_name(name), ptr(&vulkan) {

			}
			QPLDLL void disable_vertex_buffer();
			QPLDLL bool is_vertex_buffer_enabled() const;
			QPLDLL bool is_vertex_buffer_disabled() const;
			QPLDLL void set_vertex_buffer(const std::string& vertex_buffer_name);

			QPLDLL void disable_index_buffer();
			QPLDLL bool is_index_buffer_enabled() const;
			QPLDLL bool is_index_buffer_disabled() const;
			QPLDLL void set_index_buffer(const std::string& index_buffer_name);

			QPLDLL void set_vertex_shader(const std::string& vertex_shader_path);
			QPLDLL void set_fragment_shader(const std::string& fragment_shader_path);

			QPLDLL void disable_descriptor_sets();
			QPLDLL bool is_descriptor_sets_enabled() const;
			QPLDLL bool is_descriptor_sets_disabled() const;
			QPLDLL void set_descriptor_sets(const std::string& descriptor_sets_name);

			QPLDLL void disable_uniform_buffers();
			QPLDLL bool is_uniform_buffers_enabled() const;
			QPLDLL bool is_uniform_buffers_disabled() const;
			QPLDLL void set_uniform_buffers(const std::string& uniform_buffers_name);

			QPLDLL qpl::vk::scene* operator->();
			QPLDLL const qpl::vk::scene* operator->() const;
			QPLDLL qpl::vk::scene& operator*();
			QPLDLL const qpl::vk::scene& operator*() const;

			QPLDLL operator bool() const;
			QPLDLL bool operator!() const;

			QPLDLL std::string name() const;
		private:
			std::string scene_name;
			vulkan* const ptr;
		};

		class uniform_buffers_proxy {
		public:
			uniform_buffers_proxy(const std::string& name, vulkan& vulkan) : uniform_buffers_name(name), ptr(&vulkan) {

			}

			template<class C>
			void add_uniform_type(const std::string& access_name) {
				(*this)->uniform_buffer_object.add_type<C>(access_name);
			}
			template<class C>
			void set_uniform_value(const std::string& access_name, const C& value) {
				(*this)->uniform_buffer_object.set_value(access_name, value);
			}
			template<class C>
			C get_uniform_value(const std::string& access_name) {
				return (*this)->uniform_buffer_object.get_value<C>(access_name);
			}
			template<class C>
			void add_uniform_reference(const std::string& access_name, C& value) {
				(*this)->uniform_buffer_object.add_reference(access_name, value);
			}
			QPLDLL void set_uniform_binding(qpl::u32 binding);
			QPLDLL void activate_on_vertex_shader();
			QPLDLL void deactivate_on_vertex_shader();
			QPLDLL bool is_activated_on_vertex_shader() const;
			QPLDLL void activate_on_fragment_shader();
			QPLDLL void deactivate_on_fragment_shader();
			QPLDLL bool is_activated_on_fragment_shader() const;


			QPLDLL qpl::vk::uniform_buffers* operator->();
			QPLDLL const qpl::vk::uniform_buffers* operator->() const;
			QPLDLL qpl::vk::uniform_buffers& operator*();
			QPLDLL const qpl::vk::uniform_buffers& operator*() const;

			QPLDLL operator bool() const;
			QPLDLL bool operator!() const;

			QPLDLL std::string name() const;
		private:
			std::string uniform_buffers_name;
			vulkan* const ptr;
		};

		class vertex_buffer_proxy {
		public:
			vertex_buffer_proxy(const std::string& name, vulkan& vulkan) : vertex_buffer_name(name), ptr(&vulkan) {

			}

			QPLDLL qpl::vk::vertex_buffer* operator->();
			QPLDLL const qpl::vk::vertex_buffer* operator->() const;
			QPLDLL qpl::vk::vertex_buffer& operator*();
			QPLDLL const qpl::vk::vertex_buffer& operator*() const;

			QPLDLL operator bool() const;
			QPLDLL bool operator!() const;

			QPLDLL std::string name() const;
		private:
			std::string vertex_buffer_name;
			vulkan* const ptr;
		};

		class index_buffer_proxy {
		public:
			index_buffer_proxy(const std::string& name, vulkan& vulkan) : index_buffer_name(name), ptr(&vulkan) {

			}

			QPLDLL qpl::vk::index_buffer* operator->();
			QPLDLL const qpl::vk::index_buffer* operator->() const;
			QPLDLL qpl::vk::index_buffer& operator*();
			QPLDLL const qpl::vk::index_buffer& operator*() const;

			QPLDLL operator bool() const;
			QPLDLL bool operator!() const;

			QPLDLL std::string name() const;
		private:
			std::string index_buffer_name;
			vulkan* const ptr;
		};
		class descriptor_sets_proxy {
		public:
			descriptor_sets_proxy(const std::string& name, vulkan& vulkan) : descriptor_sets_name(name), ptr(&vulkan) {

			}

			QPLDLL qpl::vk::descriptor_sets* operator->();
			QPLDLL const qpl::vk::descriptor_sets* operator->() const;
			QPLDLL qpl::vk::descriptor_sets& operator*();
			QPLDLL const qpl::vk::descriptor_sets& operator*() const;

			QPLDLL operator bool() const;
			QPLDLL bool operator!() const;

			QPLDLL std::string name() const;
		private:
			std::string descriptor_sets_name;
			vulkan* const ptr;
		};

		class vulkan {
		public:
			vulkan() {
				this->construct();
			}


			QPLDLL void create();
			QPLDLL void destroy();
			QPLDLL void recreate();
			QPLDLL void recreate_swapchain();
			QPLDLL void recreate_buffers();

			QPLDLL void add_scene(const std::string& scene_name);
			QPLDLL qpl::vk::scene_proxy get_scene(const std::string& scene_name);

			QPLDLL void add_descriptor_sets(const std::string& descriptor_sets_name);
			QPLDLL qpl::vk::descriptor_sets_proxy get_descriptor_sets(const std::string& descriptor_sets_name);

			QPLDLL void add_uniform_buffers(const std::string& uniform_buffers_name);
			QPLDLL qpl::vk::uniform_buffers_proxy get_uniform_buffers(const std::string& uniform_buffers_name);

			QPLDLL void add_vertex_buffer(const std::string& vertex_buffer_name);
			QPLDLL qpl::vk::vertex_buffer_proxy get_vertex_buffer(const std::string& vertex_buffer_name);

			QPLDLL void add_index_buffer(const std::string& index_buffer_name);
			QPLDLL qpl::vk::index_buffer_proxy get_index_buffer(const std::string& index_buffer_name);


			QPLDLL qpl::u32 get_current_frame() const;
			QPLDLL void enable_vsync();
			QPLDLL void disable_vsync();
			QPLDLL bool is_vsync_enabled() const;

			QPLDLL void enable_multisampling();
			QPLDLL void disable_multisampling();
			QPLDLL bool ismultisampling_enabled() const;

			QPLDLL bool is_open() const;
			QPLDLL void close();
			QPLDLL void draw();
			QPLDLL void wait_for_device();

			QPLDLL void create_window();
			QPLDLL void create_instance();
			QPLDLL void create_surface();
			QPLDLL void create_validation_layer();
			QPLDLL void create_physical_device();
			QPLDLL void create_present_queue();
			QPLDLL void create_logical_device();
			QPLDLL void create_swapchain();
			QPLDLL void create_image_views();
			QPLDLL void create_render_pass();
			QPLDLL void create_framebuffers();
			QPLDLL void create_command_pool();
			QPLDLL void create_texture();
			QPLDLL void create_texture_view();
			QPLDLL void create_descriptor_pool();
			QPLDLL void create_color_resources();
			QPLDLL void create_depth_resources();
			QPLDLL void create_descriptor_set_layouts();
			QPLDLL void create_graphics_pipelines();
			QPLDLL void create_vertex_buffers();
			QPLDLL void create_index_buffers();
			QPLDLL void create_uniform_buffers();
			QPLDLL void create_descriptor_sets();
			QPLDLL void create_command_buffers();
			QPLDLL void create_sync_objects();


			QPLDLL void destroy_sync_objects();
			QPLDLL void destroy_command_buffers();
			QPLDLL void destroy_descriptor_sets();
			QPLDLL void destroy_uniform_buffers();
			QPLDLL void destroy_index_buffer();
			QPLDLL void destroy_vertex_buffers();
			QPLDLL void destroy_graphics_pipelines();
			QPLDLL void destroy_descriptor_set_layout();
			QPLDLL void destroy_descriptor_pool();
			QPLDLL void destroy_depth_resources();
			QPLDLL void destroy_color_resources();
			QPLDLL void destroy_command_pool();
			QPLDLL void destroy_texture_view();
			QPLDLL void destroy_texture();
			QPLDLL void destroy_framebuffers();
			QPLDLL void destroy_render_pass();
			QPLDLL void destroy_image_views();
			QPLDLL void destroy_swapchain();
			QPLDLL void destroy_logical_device();
			QPLDLL void destroy_validation_layer();
			QPLDLL void destroy_present_queue();
			QPLDLL void destroy_physical_device();
			QPLDLL void destroy_surface();
			QPLDLL void destroy_instance();
			QPLDLL void destroy_window();

			QPLDLL void enable_print_stuff();
			QPLDLL void disable_print_stuff();
			QPLDLL bool is_print_stuff_enabled() const;

			qpl::vk::instance instance;
			qpl::vk::validation_layer validation_layer;
			qpl::vk::physical_device physical_device;
			qpl::vk::logical_device logical_device;
			qpl::vk::surface surface;
			qpl::vk::present_queue present_queue;
			qpl::vk::swapchain swapchain;
			qpl::vk::image_views image_views;
			qpl::vk::render_pass render_pass;
			qpl::vk::framebuffers framebuffers;
			qpl::vk::command_pool command_pool;
			qpl::vk::color_resources color_resources;
			qpl::vk::depth_resources depth_resources;
			qpl::vk::texture texture;
			qpl::vk::texture_view texture_view;

			qpl::vk::vertex_buffer_collection vertex_buffer_collection;
			qpl::vk::index_buffer_collection index_buffer_collection;
			qpl::vk::uniform_buffers_collection uniform_buffers_collection;
			qpl::vk::descriptor_sets_collection descriptor_sets_collection;
			qpl::vk::scene_collection scene_collection;

			qpl::vk::command_buffers command_buffers;
			qpl::vk::sync_objects sync_objects;


		private:
			QPLDLL void construct();

			bool m_print_stuff;

		};

		class application {
		public:
			application() {
				this->construct();
			}
			~application() {
				this->destroy();
			}

			QPLDLL void create();
			QPLDLL void destroy();
			QPLDLL void recreate();
			QPLDLL void recreate_swapchain();
			QPLDLL void recreate_buffers();

			QPLDLL bool is_open() const;
			QPLDLL void close();
			QPLDLL void update();
			QPLDLL void draw();
			QPLDLL void wait_for_device();

			QPLDLL void set_dimension(glm::ivec2 dim) const;
			QPLDLL glm::ivec2 get_dimension() const;
			QPLDLL void set_title(std::string_view title) const;
			QPLDLL std::string_view get_title() const;

			QPLDLL qpl::time get_run_time() const;

			QPLDLL glm::vec2 mouse_position() const;
			QPLDLL glm::vec2 delta_mouse_position() const;
			QPLDLL bool left_mouse_clicked() const;
			QPLDLL bool left_mouse_holding() const;
			QPLDLL bool left_mouse_released() const;
			QPLDLL bool right_mouse_clicked() const;
			QPLDLL bool right_mouse_holding() const;
			QPLDLL bool right_mouse_released() const;
			QPLDLL bool scrolled_up() const;
			QPLDLL bool scrolled_down() const;
			QPLDLL bool key_pressed() const;
			QPLDLL bool key_released() const;
			QPLDLL bool key_pressed(qpl::i32 key) const;
			QPLDLL bool key_pressed_utf(qpl::u32 key) const;
			QPLDLL bool key_released(qpl::i32 key) const;
			QPLDLL bool key_holding() const;
			QPLDLL bool key_holding(qpl::i32 key) const;		
			template<typename ...args>
			bool key_holding(qpl::i32 key, args... more) {
				return qpl::window::key_holding(key, more...);
			}

			QPLDLL qpl::f64 get_frame_time() const;
			QPLDLL qpl::u32 get_current_frame() const;
			QPLDLL qpl::size get_number_of_frames() const;
			QPLDLL void enable_print_stuff();
			QPLDLL void disable_print_stuff();
			QPLDLL bool is_print_stuff_enabled() const;
			QPLDLL void hide_cursor();
			QPLDLL void show_cursor();



		//private:
			QPLDLL void construct();

			qpl::vk::vulkan vulkan;

			qpl::size m_number_of_frames;
			qpl::f64 m_frame_time;
			glm::vec2 m_delta_mouse;
			qpl::clock m_run_time_clock;
		};
	}
}

#endif
#endif