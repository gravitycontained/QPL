#ifndef QPL_OBJECTS_HPP
#define QPL_OBJECTS_HPP
#pragma once

#if defined(QPL_USE_VULKAN) || defined(QPL_USE_ALL)
#include <glm/glm.hpp>
#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

namespace qpl {

	namespace vk {
		enum class vertex_binding : qpl::u32 {
			vertex_3D_tex = 0u,
			vertex_2D = 1u,
			vertex_3D = 2u,
			vertex_2D_tex = 3u,
			count = 4u
		};
	}

	struct vertex_2D {
		glm::vec2 position;
		glm::vec3 color;

		static constexpr qpl::vk::vertex_binding vertex_binding() {
			return qpl::vk::vertex_binding::vertex_2D;
		}
		static constexpr qpl::size size() {
			return qpl::size{ 5 };
		}
	};
	struct vertex_3D {
		glm::vec3 position;
		glm::vec3 color;

		static constexpr qpl::vk::vertex_binding vertex_binding() {
			return qpl::vk::vertex_binding::vertex_3D;
		}
		static constexpr qpl::size size() {
			return qpl::size{ 6 };
		}
	};

	struct vertex_2D_tex {
		glm::vec2 position;
		glm::vec3 color;
		glm::vec2 tex_coord;

		static constexpr qpl::vk::vertex_binding vertex_binding() {
			return qpl::vk::vertex_binding::vertex_2D_tex;
		}
		static constexpr qpl::size size() {
			return qpl::size{ 7 };
		}
	};
	struct vertex_3D_tex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 tex_coord;

		static constexpr qpl::vk::vertex_binding vertex_binding() {
			return qpl::vk::vertex_binding::vertex_3D_tex;
		}
		static constexpr qpl::size size() {
			return qpl::size{ 8 };
		}
	};
	namespace vk {
		QPLDLL std::array<VkVertexInputBindingDescription, static_cast<qpl::size>(qpl::vk::vertex_binding::count)> get_binding_descriptions();
		QPLDLL std::array<VkVertexInputAttributeDescription, 3> get_attribute_descriptions();
	}
}

#endif
#endif