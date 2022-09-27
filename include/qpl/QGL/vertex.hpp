#pragma once
#ifndef QGL_VERTEX_HPP
#define QGL_VERTEX_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_GLEW_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/vector.hpp>
#include <qpl/color.hpp>

#include <qpl/QGL/glew.hpp>
#include <qpl/QGL/attributes.hpp>
#include <qpl/QGL/shader.hpp>

#include <qpl/QSF/drawables.hpp>

#include <array>
#include <tuple>
#include <iostream>
#include <vector>

namespace qgl {


	namespace detail {
		struct vertex_pos2 {  qpl::vec2 position = qpl::vec2{}; };
		struct vertex_pos3 {  qpl::vec3 position = qpl::vec3{}; };
		struct vertex_frgb {  qpl::frgb  color = qpl::frgb{}; };
		struct vertex_frgba { qpl::frgba color = qpl::frgba{}; };
		struct vertex_rgb {   qpl::rgb   color = qpl::rgb{}; };
		struct vertex_rgba {  qpl::rgba  color = qpl::rgba{}; };
		struct vertex_tex {   qpl::vec2 tex_coords = qpl::vec2{}; };

		template<typename T>
		using vertex_type = qpl::conditional<
			qpl::if_true<qpl::is_same<T, qgl::pos2>()>, qgl::detail::vertex_pos2,
			qpl::if_true<qpl::is_same<T, qgl::pos3>()>, qgl::detail::vertex_pos3,
			qpl::if_true<qpl::is_same<T, qgl::rgb>()>, qgl::detail::vertex_rgb,
			qpl::if_true<qpl::is_same<T, qgl::rgba>()>, qgl::detail::vertex_rgba,
			qpl::if_true<qpl::is_same<T, qgl::frgb>()>, qgl::detail::vertex_frgb,
			qpl::if_true<qpl::is_same<T, qgl::frgba>()>, qgl::detail::vertex_frgba,
			qpl::if_true<qpl::is_same<T, qgl::tex>()>, qgl::detail::vertex_tex,
			qpl::default_type, qpl::error_type>;

		template<typename T>
		using vertex_type_map = qpl::conditional<
			qpl::if_true<qpl::is_vectorN<T, 2>()>, qgl::pos2,
			qpl::if_true<qpl::is_vectorN<T, 3>()>, qgl::pos3,
			qpl::if_true<qpl::is_same<T, qpl::rgb>()>, qgl::rgb,
			qpl::if_true<qpl::is_same<T, qpl::rgba>()>, qgl::rgba,
			qpl::if_true<qpl::is_same<T, qpl::frgb>()>, qgl::frgb,
			qpl::if_true<qpl::is_same<T, qpl::frgba>()>, qgl::frgba,
			qpl::if_true<qpl::is_texN<T, 2>()>, qgl::tex,
			qpl::default_type, qpl::error_type>;

		template<typename... T>
		constexpr bool is_valid_vertex_attributes() {
			return
				qpl::type_match_count<std::tuple<qgl::rgb, qgl::frgb, qgl::rgba, qgl::frgba>, T...>() <= 1 &&
				qpl::type_match_count<std::tuple<qgl::pos2, qgl::pos3>, T...>() <= 1 &&
				qpl::are_all_types_unique<T...>() &&
				(qgl::is_vertex_attribute<T>() && ...);
		}
	}



	template<typename... Attributes> requires(qgl::detail::is_valid_vertex_attributes<Attributes...>())
	struct vertex : qgl::detail::vertex_type<Attributes>... {
		constexpr static GLsizei stride() {
			return sizeof(vertex);
		}
		constexpr static bool has_pos2() {
			return qpl::is_any_type_equal_to<qgl::pos2, Attributes...>();
		}
		constexpr static bool has_pos3() {
			return qpl::is_any_type_equal_to<qgl::pos3, Attributes...>();
		}
		constexpr static bool has_rgb() {
			return qpl::is_any_type_equal_to<qgl::rgb, Attributes...>();
		}
		constexpr static bool has_rgba() {
			return qpl::is_any_type_equal_to<qgl::rgba, Attributes...>();
		}
		constexpr static bool has_frgb() {
			return qpl::is_any_type_equal_to<qgl::frgb, Attributes...>();
		}
		constexpr static bool has_frgba() {
			return qpl::is_any_type_equal_to<qgl::frgba, Attributes...>();
		}
		constexpr static bool has_tex() {
			return qpl::is_any_type_equal_to<qgl::tex, Attributes...>();
		}
		constexpr static bool has_position() {
			return has_pos2() || has_pos3();
		}
		constexpr static bool has_color() {
			return has_rgb() || has_rgba() || has_frgb() || has_frgba();
		}

		using position_type = qpl::conditional_identity<qpl::if_true<has_pos2()>, qpl::vec2, qpl::if_true<has_pos3()>, qpl::vec3, qpl::default_error>;
		using color_type = qpl::conditional_identity<
			qpl::if_true<has_rgb()>, qpl::rgb,
			qpl::if_true<has_rgba()>, qpl::rgba,
			qpl::if_true<has_frgb()>, qpl::frgb,
			qpl::if_true<has_frgba()>, qpl::frgba,
			qpl::default_error>;
		using tex_type = qpl::conditional_identity<qpl::if_true<has_tex()>, qpl::vec2, qpl::default_error>;


		constexpr auto get_tuple() const {
			return std::tuple<std::decay_t<decltype(qpl::access_member(std::declval<qgl::detail::vertex_type<Attributes>>()))>...>(qpl::access_member(static_cast<qgl::detail::vertex_type<Attributes>>(*this))...);
		}
		constexpr auto get_const_ref_tuple() const {
			return std::tuple<decltype(qpl::access_member(std::declval<qgl::detail::vertex_type<Attributes>>()))...>(qpl::access_member(static_cast<qgl::detail::vertex_type<Attributes> const&>(*this))...);
		}
		constexpr auto get_ref_tuple() {
			return std::tuple<decltype(qpl::access_member(std::declval<qgl::detail::vertex_type<Attributes>&>()))...>(qpl::access_member(static_cast<qgl::detail::vertex_type<Attributes>&>(*this))...);
		}

		constexpr vertex() {

		}
		template<typename ... Ts>
		constexpr vertex(Ts... values) {
			if constexpr (sizeof...(Ts) == 1 && qpl::is_tuple<qpl::variadic_type_front<Ts...>>()) {
				*this = qpl::tuple_value<0u>(values...);
			}
			else {
				*this = std::make_tuple(values...);
			}
		}

		constexpr vertex(qgl::detail::vertex_type<Attributes>... values) {
			*this = std::make_tuple(values...);
		}

		template<typename T> requires(qpl::is_tuple<T>())
		constexpr vertex(const T& tuple) {
			*this = tuple;
		}		
		template<typename ...Ts> requires(qgl::detail::is_valid_vertex_attributes<Ts...>())
		constexpr vertex(const qgl::vertex<Ts...>& other) {
			*this = other;
		}

		template<typename ... Ts>
		constexpr vertex& operator=(const std::tuple<Ts...>& values) {
			auto tuple = this->get_ref_tuple();

			constexpr auto N = qpl::tuple_size<decltype(tuple)>();
			qpl::constexpr_iterate<N>([&](auto i) {
				std::get<i>(tuple) = static_cast<std::decay_t<std::tuple_element_t<i, decltype(tuple)>>>(std::get<i>(values));
			});

			return *this;
		}

		template<typename ...Ts> requires(qgl::detail::is_valid_vertex_attributes<Ts...>())
		constexpr vertex& operator=(const qgl::vertex<Ts...>& other) {
			using type = qgl::vertex<Ts...>;
			if constexpr (has_position()) {
				if constexpr (type::has_position()) {
					this->position = other.position;
				}
				else {
					this->position = decltype(this->position){};
				}
			}
			if constexpr (has_color()) {
				if constexpr (type::has_color()) {
					this->color = other.color;
				}
				else {
					this->color = decltype(this->color){};
				}
			}
			if constexpr (has_tex()) {
				if constexpr (type::has_tex()) {
					this->tex_coords = other.tex_coords;
				}
				else {
					this->tex_coords = decltype(this->tex_coords){};
				}
			}
			return *this;
		}

		std::string string() const {
			auto tuple = this->get_const_ref_tuple();
			return qpl::to_string(tuple);
		}

		template<typename... Ts>  requires(qgl::detail::is_valid_vertex_attributes<Ts...>())
		friend std::ostream& operator<<(std::ostream& os, const vertex<Ts...>& vertex);
	};
	template<typename... Attributes>  requires(qgl::detail::is_valid_vertex_attributes<Attributes...>())
	std::ostream& operator<<(std::ostream& os, const vertex<Attributes...>& vertex) {
		return os << vertex.string();
	}

	template<typename... Ts>
	constexpr auto make_vertex(Ts... ts) {
		return vertex<qgl::detail::vertex_type_map<Ts>...>(std::make_tuple(ts...));
	}


	template<typename... Attributes> requires(qgl::detail::is_valid_vertex_attributes<Attributes...>())
	struct primitive_points {};

	template<typename... Attributes> requires(qgl::detail::is_valid_vertex_attributes<Attributes...>())
	struct primitive_lines {};

	template<typename... Attributes> requires(qgl::detail::is_valid_vertex_attributes<Attributes...>())
	struct primitive_triangles {};

	template<typename... Attributes> requires(qgl::detail::is_valid_vertex_attributes<Attributes...>())
	struct primitive_quads {};

	namespace detail {
		template<typename... Ts>
		constexpr auto primitive_signature(qgl::primitive_points<Ts...>) {
			return qpl::declval<qgl::vertex<Ts...>>();
		}
		template<typename... Ts>
		constexpr auto primitive_signature(qgl::primitive_lines<Ts...>) {
			return qpl::declval<qgl::vertex<Ts...>>();
		}
		template<typename... Ts>
		constexpr auto primitive_signature(qgl::primitive_triangles<Ts...>) {
			return qpl::declval<qgl::vertex<Ts...>>();
		}
		template<typename... Ts>
		constexpr auto primitive_signature(qgl::primitive_quads<Ts...>) {
			return qpl::declval<qgl::vertex<Ts...>>();
		}
		template<typename T>
		constexpr auto primitive_signature(T) {
			return std::false_type{};
		}


		template<typename... Ts>
		constexpr auto primitive_points_signature(qgl::primitive_points<Ts...>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto primitive_points_signature(T) {
			return std::false_type{};
		}

		template<typename... Ts>
		constexpr auto primitive_lines_signature(qgl::primitive_lines<Ts...>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto primitive_lines_signature(T) {
			return std::false_type{};
		}

		template<typename... Ts>
		constexpr auto primitive_triangles_signature(qgl::primitive_triangles<Ts...>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto primitive_triangles_signature(T) {
			return std::false_type{};
		}

		template<typename... Ts>
		constexpr auto primitive_quads_signature(qgl::primitive_quads<Ts...>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto primitive_quads_signature(T) {
			return std::false_type{};
		}
	}

	template<typename T>
	constexpr bool is_primitive_type() {
		return !qpl::is_same<decltype(qgl::detail::primitive_signature(qpl::declval<T>())), std::false_type>();
	}

	template<typename T> requires(is_primitive_type<T>())
	using primitive_vertex_type = decltype(qgl::detail::primitive_signature(qpl::declval<T>()));


	template<typename T>
	constexpr bool is_primitive_points() {
		return decltype(qgl::detail::primitive_points_signature(qpl::declval<T>()))::value;
	}
	template<typename T>
	constexpr bool is_primitive_lines() {
		return decltype(qgl::detail::primitive_lines_signature(qpl::declval<T>()))::value;
	}
	template<typename T>
	constexpr bool is_primitive_triangles() {
		return decltype(qgl::detail::primitive_triangles_signature(qpl::declval<T>()))::value;
	}
	template<typename T>
	constexpr bool is_primitive_quads() {
		return decltype(qgl::detail::primitive_quads_signature(qpl::declval<T>()))::value;
	}
}

#endif
#endif