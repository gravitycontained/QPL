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

		using position_type = qpl::conditional_type<qpl::if_true<has_pos2()>, qpl::vec2, qpl::if_true<has_pos3()>, qpl::vec3, qpl::default_error>;
		using color_type = qpl::conditional_type<
			qpl::if_true<has_rgb()>, qpl::rgb,
			qpl::if_true<has_rgba()>, qpl::rgba,
			qpl::if_true<has_frgb()>, qpl::frgb,
			qpl::if_true<has_frgba()>, qpl::frgba,
			qpl::default_error>;
		using tex_type = qpl::conditional_type<qpl::if_true<has_tex()>, qpl::vec2, qpl::default_error>;


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

	namespace detail {
		template<typename Tuple> requires(qpl::is_tuple<Tuple>())
		constexpr auto vertex_from_tuple_convert() {
			auto unpack_tuple = []<typename Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return qgl::vertex<std::tuple_element_t<Ints, Tuple>...>();
			};
			return unpack_tuple.template operator() < Tuple > (std::make_index_sequence<qpl::tuple_size<Tuple>()>());
		}

		template<typename Tuple> requires(qpl::is_tuple<Tuple>())
		constexpr bool is_valid_vertex_attributes_tuple() {
			auto unpack_tuple = []<typename Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return qgl::detail::is_valid_vertex_attributes<std::tuple_element_t<Ints, Tuple>...>();
			};
			return unpack_tuple.template operator() < Tuple > (std::make_index_sequence<qpl::tuple_size<Tuple>()>());
		}

		template<typename... Attributes>
		constexpr auto vertex_type_convert() {
			if constexpr (qpl::is_integer<qpl::variadic_type_back<Attributes...>>()) {
				return qgl::detail::vertex_from_tuple_convert<qpl::variadic_type_splice_back<1, Attributes...>>();
			}
			else {
				return qpl::declval<vertex<Attributes...>>();
			}
		}
		template<typename... Attributes>
		constexpr bool is_valid_va_attributes() {
			if constexpr (qpl::is_integer<qpl::variadic_type_back<Attributes...>>()) {
				return qgl::detail::is_valid_vertex_attributes_tuple<qpl::variadic_type_splice_back<1, Attributes...>>();
			}
			else {
				return qgl::detail::is_valid_vertex_attributes<Attributes...>();
			}
		}

		struct va_tex {
			const sf::Texture* texture = nullptr;

			QPLDLL void set_texture(const sf::Texture& texture);
			QPLDLL void bind() const;
			QPLDLL void unbind() const;
		};

		struct va_shader {
			qgl::shader shader;

			QPLDLL void set_vertex_shader(const std::string& vertex);
			QPLDLL void set_fragment_shader(const std::string& fragment);
			QPLDLL void set_geometry_shader(const std::string& geometry);

			QPLDLL const std::string& get_vertex_shader() const;
			QPLDLL const std::string& get_fragment_shader() const;
			QPLDLL const std::string& get_geometry_shader() const;
		};

		struct va_primitive_type {
			qgl::primitive_type primitive_type;

			QPLDLL void set_primitive_type(qgl::primitive_type primitive_type);
			QPLDLL qgl::primitive_type get_primitive_type() const;
		};

		struct va_autoupdate_type {
			mutable bool changed = true;
		};

		template<qpl::u8 info_flag>
		using va_inheritance = qpl::inheritance_flags<info_flag, va_tex, va_shader, va_primitive_type, va_autoupdate_type>;

		template<typename... Attributes>
		constexpr static bool va_using_custom_indices_uint() {
			return qpl::is_integer<qpl::variadic_type_back<Attributes...>>();
		}

		template<typename... Attributes> requires(qgl::detail::is_valid_va_attributes<Attributes...>())
		using va_index_type = typename qpl::conditional_type<qpl::if_true<va_using_custom_indices_uint<Attributes...>()>, qpl::variadic_type_back<Attributes...>, qpl::u32>::type;

		template<typename... Attributes> requires(qgl::detail::is_valid_va_attributes<Attributes...>())
		using va_vertex_type = decltype(qgl::detail::vertex_type_convert<Attributes...>());

		template<typename... Attributes>
		struct va_content_array_vertex_index {
			std::vector<va_vertex_type<Attributes...>> vertices;
			std::vector<va_index_type<Attributes...>> indices;
		};
		template<typename... Attributes>
		struct va_content_array_vertex {
			std::vector<va_vertex_type<Attributes...>> vertices;
		};

		template<qpl::u8 info_flag, typename... Attributes>
		using va_conditional_content = qpl::conditional<qpl::if_true<qpl::bool_cast(info_flag & qgl::flag_bit_index)>, 
			va_content_array_vertex_index<Attributes...>, 
			va_content_array_vertex<Attributes...>>;
	}

	template<typename V, qpl::size N, typename I, qpl::size M>
	struct va_index_initialization {
		std::array<V, N> vertices;
		std::array<I, M> indices;
	};
	template<typename V, typename I>
	struct va_index_vector_initialization {
		std::vector<V> vertices;
		std::vector<I> indices;
	};
	template<typename V, qpl::size N>
	struct va_initialization {
		std::array<V, N> vertices;
	};
	template<typename V>
	struct va_vector_initialization {
		std::vector<V> vertices;
	};

	template<typename V, qpl::size N, typename I, qpl::size M>
	constexpr auto make_va_index(const std::array<V, N>& vertices, const std::array<I, M>& indices) {
		return va_index_initialization<V, N, I, M>{ vertices, indices };
	}

	template<typename V, qpl::size N>
	constexpr auto make_va(const std::array<V, N>& vertices) {
		return va_initialization<V, N>{ vertices };
	}


	//first is a flag, you can enable indices by inserting qgl::flag_index bit, enable custom shader by inserting qgl::flag_shader
	//last attribute can be a integer for indices type. example vao<qgs::pos3, qgs::rgb, qpl::u32> is valid
	//if last attribute is not an integer, qpl::u32 will be defaulted. example vao<qgs::pos3, qgs::rgb>
	template<qpl::u8 info_flag, typename... Attributes> requires(qgl::detail::is_valid_va_attributes<Attributes...>())
	struct vertex_array : detail::va_inheritance<info_flag>, detail::va_conditional_content<info_flag, Attributes...> {

		constexpr static bool using_custom_indices_uint() {
			return qpl::bool_cast(qpl::is_integer<qpl::variadic_type_back<Attributes...>>());
		}
		constexpr static bool has_indices() {
			return qpl::bool_cast(info_flag & qgl::flag_bit_index);
		}
		constexpr static bool has_shader() {
			return qpl::bool_cast(info_flag & qgl::flag_bit_shader);
		}
		constexpr static bool has_primitive_type() {
			return qpl::bool_cast(info_flag & qgl::flag_bit_primitive_type);
		}
		constexpr static bool has_autoupdate() {
			return qpl::bool_cast(info_flag & qgl::flag_bit_autoupdate);
		}

		using index_type = typename qpl::conditional_type<qpl::if_true<using_custom_indices_uint()>, qpl::variadic_type_back<Attributes...>, qpl::u32>::type;
		using vertex_type = typename decltype(qgl::detail::vertex_type_convert<Attributes...>());

		~vertex_array() {
			if (this->generated()) {
				qgl::gl::delete_buffer(this->vertexVBO);
				qgl::gl::delete_buffer(this->indexVBO);
				qgl::gl::delete_vertex_array(this->id);

				this->vertexVBO = 0;
				this->indexVBO = 0;
				this->id = 0;
			}
		}

		bool generated() const {
			return qpl::bool_cast(this->id);
		}


		template<qpl::u8 info_flag2, typename ...Ts>
		constexpr vertex_array(const qgl::vertex_array<info_flag2, Ts...>& other) {
			*this = other;
		}
		constexpr vertex_array(const qgl::vertex_array<info_flag, Attributes...>& other) {
			*this = other;
		}
		template<typename V, qpl::size N, typename I, qpl::size M>
		constexpr vertex_array(const qgl::va_index_initialization<V, N, I, M>& other) {
			*this = other;
		}
		template<typename V, typename I>
		constexpr vertex_array(const qgl::va_index_vector_initialization<V, I>& other) {
			*this = other;
		}
		template<typename V, qpl::size N>
		constexpr vertex_array(const qgl::va_initialization<V, N>& other) {
			*this = other;
		}
		template<typename V>
		constexpr vertex_array(const qgl::va_vector_initialization<V>& other) {
			*this = other;
		}
		constexpr vertex_array() {

		}

		template<qpl::u8 info_flag2, typename ...Ts>
		constexpr qgl::vertex_array<info_flag, Attributes...>& operator=(const qgl::vertex_array<info_flag2, Ts...>& other) {
			
			if constexpr (has_indices()) {
				if constexpr (other.has_indices()) {
					this->indices.resize(other.indices.size());
					for (qpl::size i = 0u; i < this->indices.size(); ++i) {
						this->indices[i] = static_cast<index_type>(other.indices[i]);
					}
				}
				else {
					this->indices.clear();
				}
			}

			constexpr bool make_vertices_from_indices = !has_indices() && other.has_indices();

			if constexpr (make_vertices_from_indices) {
				this->vertices.resize(other.indices.size());
				for (qpl::size i = 0u; i < other.indices.size(); ++i) {
					this->vertices[i] = other.vertices[other.indices[i]];
				}
			}
			else {
				this->vertices.resize(other.vertices.size());
				for (qpl::size i = 0u; i < this->vertices.size(); ++i) {
					this->vertices[i] = other[i];
				}
			}

			this->id = other.id;
			this->vertexVBO = other.vertexVBO;
			this->indexVBO = other.indexVBO;
			return *this;
		}
		constexpr qgl::vertex_array<info_flag, Attributes...>& operator=(const qgl::vertex_array<info_flag, Attributes...>& other) {
			this->vertices = other.vertices;
			if constexpr (has_indices()) {
				this->indices = other.indices;
			}
			this->id = other.id;
			this->vertexVBO = other.vertexVBO;
			this->indexVBO = other.indexVBO;
			return *this;
		}
		template<typename V, qpl::size N, typename I, qpl::size M>
		constexpr qgl::vertex_array<info_flag, Attributes...>& operator=(const qgl::va_index_initialization<V, N, I, M>& init) {

			if constexpr (has_indices()) {
				this->vertices.resize(N);
				for (qpl::size i = 0u; i < N; ++i) {
					this->vertices[i] = init.vertices[i];
				}
				this->indices.resize(M);
				for (qpl::size i = 0u; i < M; ++i) {
					this->indices[i] = static_cast<index_type>(init.indices[i]);
				}			
			}
			else {
				this->vertices.resize(init.indices.size());
				for (qpl::size i = 0u; i < init.indices.size(); ++i) {
					this->vertices[i] = init.vertices[init.indices[i]];
				}
			}
			return *this;
		}
		template<typename V, typename I>
		constexpr qgl::vertex_array<info_flag, Attributes...>& operator=(const qgl::va_index_vector_initialization<V, I>& init) {
			if constexpr (has_indices()) {
				this->vertices.resize(init.vertices.size());
				for (qpl::size i = 0u; i < this->vertices.size(); ++i) {
					this->vertices[i] = init.vertices[i];
				}
				this->indices.resize(init.indices.size());
				for (qpl::size i = 0u; i < this->indices.size(); ++i) {
					this->indices[i] = static_cast<index_type>(init.indices[i]);
				}
			}
			else {
				this->vertices.resize(init.indices.size());
				for (qpl::size i = 0u; i < init.indices.size(); ++i) {
					this->vertices[i] = init.vertices[init.indices[i]];
				}
			}
			return *this;
		}
		template<typename V, qpl::size N>
		constexpr qgl::vertex_array<info_flag, Attributes...>& operator=(const qgl::va_initialization<V, N>& init) {
			this->vertices.resize(N);
			for (qpl::size i = 0u; i < N; ++i) {
				this->vertices[i] = init.vertices[i];
			}
			if constexpr (has_indices()) {
				this->indices.clear();
			}
			return *this;
		}
		template<typename V>
		constexpr qgl::vertex_array<info_flag, Attributes...>& operator=(const qgl::va_vector_initialization<V>& init) {
			this->vertices.resize(init.vertices.size());
			for (qpl::size i = 0u; i < this->vertices.size(); ++i) {
				this->vertices[i] = init.vertices[i];
			}
			if constexpr (has_indices()) {
				this->indices.clear();
			}
			return *this;
		}

		void update() const {
			if (!this->generated()) {
				this->generate();
			}
			qgl::gl::bind_vertex_array(this->id);
			qgl::gl::buffer_vertex_data(this->vertexVBO, this->draw_count() * vertex_type::stride(), this->vertices.data());
			qgl::gl::unbind_vertex_array();
		}
		void generate() const {
			if constexpr (has_shader()) {
				if (!this->shader.loaded) {
					this->shader.set_vertex(qgl::get_vertex_shader<Attributes...>());
					this->shader.set_fragment(qgl::get_fragment_shader<Attributes...>());
				}

				this->shader.bind();
			}
			else {
				qgl::check_shader<Attributes...>();
				qgl::bind_shader<Attributes...>();
			}

			qgl::gl::generate_vertex_array(this->id);
			qgl::gl::bind_vertex_array(this->id);
			qgl::gl::generate_vertex_buffer(this->vertexVBO, this->draw_count() * vertex_type::stride(), this->vertices.data());

			qpl::size offset = 0u;
			auto add_attribute = [&]<typename T>() {
				if constexpr (qpl::is_same<T, qgl::pos2>()) {
					qgl::gl::enable_vertex_attribute<qpl::f32>(static_cast<GLuint>(qgl::vertex_attribute::position), 2, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector2f);
				}
				else if constexpr (qpl::is_same<T, qgl::pos3>()) {
					qgl::gl::enable_vertex_attribute<qpl::f32>(static_cast<GLuint>(qgl::vertex_attribute::position), 3, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector3f);
				}
				else if constexpr (qpl::is_same<T, qgl::rgb>()) {
					qgl::gl::enable_vertex_attribute<qpl::u8>(static_cast<GLuint>(qgl::vertex_attribute::color), 3, vertex_type::stride(), offset);
					offset += sizeof(qpl::rgb);
				}
				else if constexpr (qpl::is_same<T, qgl::rgba>()) {
					qgl::gl::enable_vertex_attribute<qpl::u8>(static_cast<GLuint>(qgl::vertex_attribute::color), 4, vertex_type::stride(), offset);
					offset += sizeof(qpl::rgba);
				}
				else if constexpr (qpl::is_same<T, qgl::frgb>()) {
					qgl::gl::enable_vertex_attribute<qpl::f32>(static_cast<GLuint>(qgl::vertex_attribute::color), 3, vertex_type::stride(), offset);
					offset += sizeof(qpl::frgb);
				}
				else if constexpr (qpl::is_same<T, qgl::frgba>()) {
					qgl::gl::enable_vertex_attribute<qpl::f32>(static_cast<GLuint>(qgl::vertex_attribute::color), 4, vertex_type::stride(), offset);
					offset += sizeof(qpl::frgba);
				}
				else if constexpr (qpl::is_same<T, qgl::tex>()) {
					qgl::gl::enable_vertex_attribute<qpl::f32>(static_cast<GLuint>(qgl::vertex_attribute::tex_coord), 2, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector2f);
				}
			};

			(add_attribute.template operator() < Attributes > (), ...);

			if constexpr (has_indices()) {
				qgl::gl::generate_index_buffer(this->indexVBO, this->draw_count() * sizeof(index_type), this->indices.data());
			}
			qgl::gl::bind_vertex_array(0);

			if constexpr (has_shader()) {
				this->shader.unbind();
			}
			else {
				qgl::unbind_shader<Attributes...>();
			}
		}

		void draw(qsf::draw_object_gl& draw) const {
			draw.window->setActive(true);

			if (!this->generated()) {
				this->generate();
			}
			if constexpr (has_autoupdate()) {
				this->update();
			}
			auto view = draw.states.transform.getMatrix();

			if constexpr (vertex_type::has_tex()) {
				if (this->texture) {
					this->bind();
				}
			}

			if constexpr (has_shader()) {
				this->shader.bind();
				this->shader.set_pvm(view);
			}
			else {
				qgl::check_shader<Attributes...>();
				qgl::bind_shader<Attributes...>();
				qgl::get_shader<Attributes...>().set_pvm(view);
			}

			qgl::gl::bind_vertex_array(this->id);

			if constexpr (has_primitive_type()) {
				qgl::gl::draw_elements(this->primitive_type, this->draw_count());
			}
			else if constexpr (!has_indices()) {
				qgl::gl::draw_elements(qgl::primitive_type::triangles, this->draw_count());
			}
			else {
				qgl::gl::draw_triangles(this->draw_count());
			}

			qgl::gl::unbind_vertex_array();
			if constexpr (vertex_type::has_tex()) {
				if (this->texture) {
					this->unbind();
				}
			}

			draw.window->setActive(false);

			if constexpr (has_shader()) {
				this->shader.unbind();
			}
			else {
				qgl::unbind_shader<Attributes...>();
			}
		}

		void move(vertex_type::position_type::type position) requires(vertex_type::has_position()) {
			for (auto& i : this->vertices) {
				i.position += position;
			}
		}

		constexpr vertex_type& operator[](qpl::size index) {
			return this->vertices[index];
		}
		constexpr const vertex_type& operator[](qpl::size index) const {
			return this->vertices[index];
		}
		constexpr vertex_type& at(qpl::size index) {
			return this->vertices.at(index);
		}
		constexpr const vertex_type& at(qpl::size index) const {
			return this->vertices.at(index);
		}

		constexpr vertex_type& back() {
			return this->vertices.back();
		}
		constexpr const vertex_type& back() const {
			return this->vertices.back();
		}
		constexpr vertex_type& front() {
			return this->vertices.front();
		}
		constexpr const vertex_type& front() const {
			return this->vertices.front();
		}

		constexpr auto begin() {
			return this->vertices.begin();
		}
		constexpr const auto begin() const {
			return this->vertices.cbegin();
		}
		constexpr auto cbegin() {
			return this->vertices.cbegin();
		}
		constexpr auto end() {
			return this->vertices.end();
		}
		constexpr const auto end() const {
			return this->vertices.cend();
		}
		constexpr auto cend() {
			return this->vertices.cend();
		}
		constexpr auto rbegin() {
			return this->vertices.rbegin();
		}
		constexpr const auto rbegin() const {
			return this->vertices.crbegin();
		}
		constexpr auto crbegin() {
			return this->vertices.crbegin();
		}
		constexpr auto rend() {
			return this->vertices.rend();
		}
		constexpr const auto rend() const {
			return this->vertices.crend();
		}
		constexpr auto crend() {
			return this->vertices.crend();
		}

		constexpr void add(const vertex_type& vertex) {
			this->vertices.push_back(vertex);
		}


		template<qpl::u8 info_flag2, typename ...Ts>
		constexpr void add(const qgl::vertex_array<info_flag2, Ts...>& other) {
			if constexpr (has_indices()) {
				auto vertices_size = this->vertices.size();
				if constexpr (other.has_indices()) {
					auto indices_size = this->indices.size();
					this->indices.resize(indices_size + other.indices.size());
					for (qpl::size i = 0u; i < other.indices.size(); ++i) {
						this->indices[indices_size + i] = static_cast<index_type>(other.indices[i] + vertices_size);
					}
				}
				else {
					this->indices.clear();
				}
			}

			constexpr bool make_vertices_from_indices = !has_indices() && other.has_indices();

			if constexpr (make_vertices_from_indices) {
				auto vertices_size = this->vertices.size();
				this->vertices.resize(vertices_size + other.indices.size());
				for (qpl::size i = 0u; i < other.indices.size(); ++i) {
					this->vertices[vertices_size + i] = other.vertices[other.indices[i]];
				}
			}
			else {
				auto vertices_size = this->vertices.size();
				this->vertices.resize(vertices_size + other.vertices.size());
				for (qpl::size i = 0u; i < other.vertices.size(); ++i) {
					this->vertices[vertices_size + i] = other.vertices[i];
				}
			}
		}
		constexpr qpl::size size() const {
			return this->vertices.size();
		}
		constexpr void clear() {
			this->vertices.clear();
			if constexpr (has_indices()) {
				this->indices.clear();
			}
		}
		constexpr GLsizei draw_count() const {
			if constexpr (has_indices()) {
				return static_cast<GLsizei>(this->indices.size());
			}
			else {
				return static_cast<GLsizei>(this->vertices.size());
			}
		}
		template<qpl::u8 info_flag2, typename... Ts> requires(qgl::detail::is_valid_va_attributes<Ts...>())
		friend struct vertex_array_flag_type;

	private:
		mutable GLuint id = 0u;
		mutable GLuint vertexVBO = 0u;
		mutable GLuint indexVBO = 0u;
	};

	using va_index = qgl::vertex_array<qgl::flag_bit_index, qgl::pos3, qgl::tex, qgl::frgb>;
	using va_index_primitive = qgl::vertex_array<qgl::flag_bit_index, qgl::pos3, qgl::tex, qgl::frgb>;
	using va = qgl::vertex_array<qgl::flag_default, qgl::pos3, qgl::tex, qgl::frgb>;
}

#endif
#endif