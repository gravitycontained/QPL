#pragma once
#ifndef QGL_ATTRIBUTES_HPP
#define QGL_ATTRIBUTES_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_GLEW_USE

#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>

namespace qgl {
	struct pos2 {};
	struct pos3 {};
	struct rgb {};
	struct rgba {};
	struct frgb {};
	struct frgba {};
	struct tex {};

	constexpr qpl::u8 flag_bit_index = 0b1 << 0;
	constexpr qpl::u8 flag_bit_shader = 0b1 << 1;
	constexpr qpl::u8 flag_bit_primitive_type = 0b1 << 2;
	constexpr qpl::u8 flag_bit_autoupdate = 0b1 << 3;

	constexpr qpl::u8 flag_default = 0b0;


	template<typename T>
	constexpr bool is_vertex_attribute() {
		return qpl::is_any_type_equal_to<T, pos2, pos3, rgb, rgba, frgb, frgba, tex>();
	}


	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
	constexpr bool has_pos2() {
		return qpl::is_any_type_equal_to<qgl::pos2, Attributes...>();
	}
	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
	constexpr bool has_pos3() {
		return qpl::is_any_type_equal_to<qgl::pos3, Attributes...>();
	}
	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
	constexpr bool has_rgb() {
		return qpl::is_any_type_equal_to<qgl::rgb, Attributes...>();
	}
	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
	constexpr bool has_rgba() {
		return qpl::is_any_type_equal_to<qgl::rgba, Attributes...>();
	}
	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
		constexpr bool has_frgb() {
		return qpl::is_any_type_equal_to<qgl::frgb, Attributes...>();
	}
	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
		constexpr bool has_frgba() {
		return qpl::is_any_type_equal_to<qgl::frgba, Attributes...>();
	}
	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
	constexpr bool has_tex() {
		return qpl::is_any_type_equal_to<qgl::tex, Attributes...>();
	}

	template<typename Attribute> requires(qgl::is_vertex_attribute<Attribute>())
		constexpr qpl::u32 attribute_flag_bit() {
		if constexpr (qgl::has_pos2<Attribute>()) {
			return (1 << 0);
		}
		else if constexpr (qgl::has_pos3<Attribute>()) {
			return (1 << 1);
		}
		else if constexpr (qgl::has_rgb<Attribute>()) {
			return (1 << 2);
		}
		else if constexpr (qgl::has_rgba<Attribute>()) {
			return (1 << 3);
		}
		else if constexpr (qgl::has_frgb<Attribute>()) {
			return (1 << 4);
		}
		else if constexpr (qgl::has_frgba<Attribute>()) {
			return (1 << 5);
		}
		else if constexpr (qgl::has_tex<Attribute>()) {
			return (1 << 6);
		}
	}

	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
	constexpr qpl::u8 attribute_flag() {
		return (attribute_flag_bit<Attributes>() | ...);
	}



}

#endif
#endif