#pragma once
#ifndef QGL_VERTEX_HPP
#define QGL_VERTEX_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE
#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/vector.hpp>
#include <qpl/color.hpp>

#include <array>
#include <tuple>


#include <qpl/QGL/glew.hpp>
#include <qpl/QGL/attributes.hpp>
#include <qpl/QGL/shader.hpp>

#include <qpl/QSF/drawables.hpp>

namespace qgl {


	namespace detail {
		struct vertex_pos2 { qpl::vec2 position = qpl::vec2{}; };
		struct vertex_pos3 { qpl::vec3 position = qpl::vec3{}; };
		struct vertex_rgb { qpl::frgb color = qpl::frgb{}; };
		struct vertex_rgba { qpl::frgba color = qpl::frgba{}; };
		struct vertex_tex { qpl::vec2 tex_coords = qpl::vec2{}; };


		template<typename T>
		using vertex_type = qpl::conditional<
			qpl::if_true<qpl::is_same<T, qgl::pos2>()>, qgl::detail::vertex_pos2,
			qpl::if_true<qpl::is_same<T, qgl::pos3>()>, qgl::detail::vertex_pos3,
			qpl::if_true<qpl::is_same<T, qgl::rgb>()>, qgl::detail::vertex_rgb,
			qpl::if_true<qpl::is_same<T, qgl::rgba>()>, qgl::detail::vertex_rgba,
			qpl::if_true<qpl::is_same<T, qgl::tex>()>, qgl::detail::vertex_tex,
			qpl::default_type, qpl::error_type>;
		
		template<typename T>
		using vertex_type_map = qpl::conditional<
			qpl::if_true<qpl::is_vectorN<T, 2>()>,  qgl::pos2,
			qpl::if_true<qpl::is_vectorN<T, 3>()>,  qgl::pos3,
			qpl::if_true<qpl::is_same<T, qpl::rgb>()>,  qgl::rgb,
			qpl::if_true<qpl::is_same<T, qpl::frgb>()>, qgl::rgb,
			qpl::if_true<qpl::is_same<T, qpl::rgba>()>,  qgl::rgba,
			qpl::if_true<qpl::is_same<T, qpl::frgba>()>, qgl::rgba,
			qpl::if_true<qpl::is_texN<T, 2>()>, qgl::tex,
			qpl::default_type, qpl::error_type>;

		template<typename... T>
		constexpr bool is_valid_vertex_attributes() {
			return
				!(qpl::is_any_type_equal_to<qgl::rgb, T...>() && qpl::is_any_type_equal_to<qgl::rgba, T...>()) &&
				!(qpl::is_any_type_equal_to<qgl::pos2, T...>() && qpl::is_any_type_equal_to<qgl::pos3, T...>()) &&
				qpl::are_all_types_unique<T...>() &&
				(qgl::is_attribute<T>() && ...);
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
		constexpr static bool has_tex() {
			return qpl::is_any_type_equal_to<qgl::tex, Attributes...>();
		}
		constexpr static bool has_position() {
			return has_pos2() || has_pos3();
		}
		constexpr static bool has_color() {
			return has_rgb() || has_rgba();
		}

		using position_type = qpl::conditional_t<qpl::if_true<has_pos2()>, qpl::vec2, qpl::if_true<has_pos3()>, qpl::vec3, qpl::default_error>;
		using color_type = qpl::conditional_t<qpl::if_true<has_rgb()>, qpl::frgb, qpl::if_true<has_rgba()>, qpl::frgba, qpl::default_error>;
		using tex_type = qpl::conditional_t<qpl::if_true<has_tex()>, qpl::vec2, qpl::default_error>;


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

		template<typename ... Ts>
		constexpr vertex& operator=(const std::tuple<Ts...>& values) {
			auto tuple = this->get_ref_tuple();

			constexpr auto N = qpl::tuple_size<decltype(tuple)>();
			qpl::constexpr_iterate<N>([&](auto e) {
				constexpr auto i = e.i;
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
					this->position = position_type{};
				}
			}
			if constexpr (has_color()) {
				if constexpr (type::has_color()) {
					this->color = other.color;
				}
				else {
					this->color = color_type{};
				}
			}
			if constexpr (has_tex()) {
				if constexpr (type::has_tex()) {
					this->tex_coords = other.tex_coords;
				}
				else {
					this->tex_coords = tex_type{};
				}
			}
			return *this;
		}

	};

	template<typename... Ts>
		constexpr auto make_vertex(Ts... ts) {
		return vertex<qgl::detail::vertex_type_map<Ts>...>(std::make_tuple(ts...));
	}


	namespace detail {
		template<typename Tuple> requires(qpl::is_tuple<Tuple>())
		constexpr auto vertex_from_tuple_convert() {
			constexpr auto unpack_tuple = []<typename Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return qgl::vertex<std::tuple_element_t<Ints, Tuple>...>();
			};
			return unpack_tuple.template operator()<Tuple>(std::make_index_sequence<qpl::tuple_size<Tuple>()>());
		}


		template<typename Tuple> requires(qpl::is_tuple<Tuple>())
		constexpr bool is_valid_vertex_attributes_tuple() {
			constexpr auto unpack_tuple = []<typename Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return qgl::detail::is_valid_vertex_attributes<std::tuple_element_t<Ints, Tuple>...>();
			};
			return unpack_tuple.template operator()<Tuple>(std::make_index_sequence<qpl::tuple_size<Tuple>()>());
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
		constexpr bool is_valid_vao_attributes() {
			if constexpr (qpl::is_integer<qpl::variadic_type_back<Attributes...>>()) {
				return qgl::detail::is_valid_vertex_attributes_tuple<qpl::variadic_type_splice_back<1, Attributes...>>();
			}
			else {
				return qgl::detail::is_valid_vertex_attributes<Attributes...>();
			}
		}

		struct vao_tex {
			const sf::Texture* texture = nullptr;

			void set_texture(const sf::Texture& texture) {
				this->texture = &texture;
			}
			void bind() const {
				if (this->texture == nullptr) {
					throw qpl::exception("qgl::vao : texture not set");
				}
				sf::Texture::bind(this->texture);
			}
			void unbind() const {
				sf::Texture::bind(nullptr);
			}
		};
		template<typename... Attributes>
		using vao_attributes = qpl::conditional<qpl::if_true<qpl::is_any_type_equal_to<qgl::tex, Attributes...>()>, vao_tex, qpl::empty_type>;
	}
	template<typename V, qpl::size N, typename I, qpl::size M>
	struct vertex_index_array_initalization {
		std::array<V, N> vertices;
		std::array<I, M> indices;

	};
	template<typename V, qpl::size N, typename I, qpl::size M>
	constexpr auto make_vertex_index_array(const std::array<V, N>& vertices, const std::array<I, M>& indices) {
		return vertex_index_array_initalization<V, N, I, M>{ vertices, indices };
	}


	template<typename V, qpl::size N, typename I, qpl::size M>
	struct vertex_array_initalization {
		std::array<V, N> vertices;

	};
	template<typename V, qpl::size N>
	constexpr auto make_vertex_array(const std::array<V, N>& vertices) {
		return vertex_array_initalization<V, N>{ vertices };
	}


	//last attribute can be a integer for indices type. example vao<qgs::pos3, qgs::rgb, qpl::u32> is valid
	//if last attribute is not an integer, qpl::u32 will be defaulted. example vao<qgs::pos3, qgs::rgb>
	template<typename... Attributes> requires(qgl::detail::is_valid_vao_attributes<Attributes...>())
	struct vertex_index_array_type : detail::vao_attributes<Attributes...> {

		constexpr static bool using_custom_indices_uint = qpl::is_integer<qpl::variadic_type_back<Attributes...>>();

		using index_type = typename qpl::conditional_t<qpl::if_true<using_custom_indices_uint>, qpl::variadic_type_back<Attributes...>, qpl::u32>::type;
		using vertex_type = typename decltype(qgl::detail::vertex_type_convert<Attributes...>());


		std::vector<vertex_type> vertices;
		std::vector<index_type> indices;

		~vertex_index_array_type() {
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


		template<typename ...Ts>
		vertex_index_array_type(const qgl::vertex_index_array_type<Ts...>& other) {
			*this = other;
		}
		vertex_index_array_type(const qgl::vertex_index_array_type<Attributes...>& other) {
			*this = other;
		}
		vertex_index_array_type() {

		}

		template<typename ...Ts>
		constexpr qgl::vertex_index_array_type<Attributes...>& operator=(const qgl::vertex_index_array_type<Ts...>& other) {
			this->vertices.resize(other.vertices.size());
			for (qpl::size i = 0u; i < this->vertices.size(); ++i) {
				this->vertices[i] = other[i];
			}
			this->indices.resize(other.indices.size());
			for (qpl::size i = 0u; i < this->indices.size(); ++i) {
				this->indices[i] = static_cast<index_type>(other.indices[i]);
			}
			this->id = other.id;
			this->vertexVBO = other.vertexVBO;
			this->indexVBO = other.indexVBO;
			return *this;
		}
		constexpr qgl::vertex_index_array_type<Attributes...>& operator=(const qgl::vertex_index_array_type<Attributes...>& other) {
			this->vertices = other.vertices;
			this->indices = other.indices;
			this->id = other.id;
			this->vertexVBO = other.vertexVBO;
			this->indexVBO = other.indexVBO;
			return *this;
		}
		template<typename V, qpl::size N, typename I, qpl::size M>
		constexpr qgl::vertex_index_array_type<Attributes...>& operator=(const qgl::vertex_index_array_initalization<V, N, I, M>& init) {
			this->vertices.resize(N);
			for (qpl::size i = 0u; i < N; ++i) {
				this->vertices[i] = init.vertices[i];
			}
			this->indices.resize(M);
			for (qpl::size i = 0u; i < M; ++i) {
				this->indices[i] = static_cast<index_type>(init.indices[i]);
			}
			return *this;
		}
		void update() {
			qgl::gl::bind_vertex_array(this->id);
			qgl::gl::buffer_vertex_data(this->vertexVBO, this->draw_count() * vertex_type::stride(), this->vertices.data());
			qgl::gl::unbind_vertex_array();
		}

		void generate() {
			qgl::check_shader<Attributes...>();
			qgl::bind_shader<Attributes...>();

			qgl::gl::generate_vertex_array(this->id);
			qgl::gl::bind_vertex_array(this->id);
			qgl::gl::generate_vertex_buffer(this->vertexVBO, this->draw_count() * vertex_type::stride(), this->vertices.data());

			qpl::size offset = 0u;
			auto add_attribute = [&]<typename T>() {
				if constexpr (qpl::is_same<T, qgl::pos2>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::position), 2, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector2f);
				}
				else if constexpr (qpl::is_same<T, qgl::pos3>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::position), 3, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector3f);
				}
				else if constexpr (qpl::is_same<T, qgl::rgb>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::color), 3, vertex_type::stride(), offset);
					offset += sizeof(qpl::frgb);
				}
				else if constexpr (qpl::is_same<T, qgl::rgba>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::color), 4, vertex_type::stride(), offset);
					offset += sizeof(qpl::frgba);
				}
				else if constexpr (qpl::is_same<T, qgl::tex>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::tex_coord), 2, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector2f);
				}
			};

			(add_attribute.template operator()<Attributes>(), ...);

			qgl::gl::generate_index_buffer(this->indexVBO, this->draw_count() * sizeof(index_type), this->indices.data());
			qgl::gl::bind_vertex_array(0);
			qgl::unbind_shader<Attributes...>();
		}
		void draw(qsf::draw_object_gl& draw) const {

			draw.window->setActive(true);

			auto view = draw.states.transform.getMatrix();

			if constexpr (vertex_type::has_tex()) {
				if (this->texture) {
					this->bind();
				}
			}

			qgl::check_shader<Attributes...>();
			qgl::bind_shader<Attributes...>();

			qgl::gl::bind_vertex_array(this->id);

			qgl::get_shader<Attributes...>().set_pvm(view);
			qgl::gl::draw_triangles(this->draw_count());

			qgl::gl::unbind_vertex_array();
			if constexpr (vertex_type::has_tex()) {
				if (this->texture) {
					this->unbind();
				}
			}

			draw.window->setActive(false);
			qgl::unbind_shader<Attributes...>();
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

		constexpr qpl::size size() const {
			return this->vertices.size();
		}
		constexpr GLsizei draw_count() const {
			return static_cast<GLsizei>(this->indices.size());
		}
		template<typename... Ts> requires(qgl::detail::is_valid_vao_attributes<Ts...>())
		friend struct vertex_index_array_type;

		private:
			GLuint id = 0u;
			GLuint vertexVBO = 0u;
			GLuint indexVBO = 0u;

	};



	template<typename... Attributes> requires(qgl::detail::is_valid_vertex_attributes<Attributes...>())
	struct vertex_array_type : detail::vao_attributes<Attributes...> {

		using vertex_type = qgl::vertex<Attributes...>;

		std::vector<vertex_type> vertices;
		primitive_type primitive_type = primitive_type::triangles;

		~vertex_array_type() {
			if (this->generated()) {
				qgl::gl::delete_buffer(this->vertexVBO);
				qgl::gl::delete_vertex_array(this->id);

				this->vertexVBO = 0;
				this->id = 0;
			}
		}

		bool generated() const {
			return qpl::bool_cast(this->id);
		}


		template<typename ...Ts>
		vertex_array_type(const qgl::vertex_array_type<Ts...>& other) {
			*this = other;
		}
		vertex_array_type(const qgl::vertex_array_type<Attributes...>& other) {
			*this = other;
		}
		vertex_array_type() {

		}

		template<typename ...Ts>
		qgl::vertex_array_type<Attributes...>& operator=(const qgl::vertex_array_type<Ts...>& other) {
			this->vertices.resize(other.vertices.size());
			for (qpl::size i = 0u; i < this->vertices.size(); ++i) {
				this->vertices[i] = other[i];
			}
			this->id = other.id;
			this->vertexVBO = other.vertexVBO;
			return *this;
		}
		qgl::vertex_array_type<Attributes...>& operator=(const qgl::vertex_array_type<Attributes...>& other) {
			this->vertices = other.vertices;
			this->id = other.id;
			this->vertexVBO = other.vertexVBO;
			return *this;
		}
		template<typename V, qpl::size N, typename I, qpl::size M>
		qgl::vertex_array_type<Attributes...>& operator=(const qgl::vertex_array_initalization<V, N, I, M>& init) {
			this->vertices.resize(N);
			for (qpl::size i = 0u; i < N; ++i) {
				this->vertices[i] = init.vertices[i];
			}
			return *this;
		}
		void update() {
			qgl::gl::bind_vertex_array(this->id);
			qgl::gl::buffer_vertex_data(this->vertexVBO, this->size() * vertex_type::stride(), this->vertices.data());
			qgl::gl::unbind_vertex_array();
		}

		void generate() {
			qgl::check_shader<Attributes...>();
			qgl::bind_shader<Attributes...>();

			qgl::gl::generate_vertex_array(this->id);
			qgl::gl::bind_vertex_array(this->id);
			qgl::gl::generate_vertex_buffer(this->vertexVBO, this->size() * vertex_type::stride(), this->vertices.data());

			qpl::size offset = 0u;
			auto add_attribute = [&]<typename T>() {
				if constexpr (qpl::is_same<T, qgl::pos2>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::position), 2, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector2f);
				}
				else if constexpr (qpl::is_same<T, qgl::pos3>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::position), 3, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector3f);
				}
				else if constexpr (qpl::is_same<T, qgl::rgb>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::color), 3, vertex_type::stride(), offset);
					offset += sizeof(qpl::frgb);
				}
				else if constexpr (qpl::is_same<T, qgl::rgba>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::color), 4, vertex_type::stride(), offset);
					offset += sizeof(qpl::frgba);
				}
				else if constexpr (qpl::is_same<T, qgl::tex>()) {
					qgl::gl::enable_vertex_attribute(static_cast<GLuint>(qgl::vertex_attribute::tex_coord), 2, vertex_type::stride(), offset);
					offset += sizeof(qpl::vector2f);
				}
			};

			(add_attribute.template operator() < Attributes > (), ...);

			qgl::gl::unbind_vertex_array();
			qgl::unbind_shader<Attributes...>();
		}
		void draw(qsf::draw_object_gl& draw) const {
			draw.window->setActive(true);

			auto view = draw.states.transform.getMatrix();

			if constexpr (vertex_type::has_tex()) {
				if (this->texture) {
					this->bind();
				}
			}

			qgl::check_shader<Attributes...>();
			qgl::bind_shader<Attributes...>();

			qgl::gl::bind_vertex_array(this->id);

			std::array<float, 16> pvm;
			std::memcpy(pvm.data(), view, sizeof(pvm));

			qgl::get_shader<Attributes...>().set_pvm(pvm);
			qgl::gl::draw_elements(this->primitive_type, this->size());

			qgl::gl::unbind_vertex_array();
			if constexpr (vertex_type::has_tex()) {
				if (this->texture) {
					this->unbind();
				}
			}

			draw.window->setActive(false);
			qgl::unbind_shader<Attributes...>();
		}

		void move(vertex_type::position_type::type position) requires(vertex_type::has_position()) {
			for (auto& i : this->vertices) {
				i.position += position;
			}
		}

		constexpr void add(const vertex_type& vertex) {
			this->vertices.push_back(vertex);
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

		constexpr qpl::size size() const {
			return this->vertices.size();
		}

		template<typename... Ts> requires(qgl::detail::is_valid_vertex_attributes<Ts...>())
		friend struct vertex_array_type;

		private:
			GLuint id = 0u;
			GLuint vertexVBO = 0u;

	};

	using vertex_index_array = qgl::vertex_index_array_type<qgl::pos3, qgl::tex, qgl::rgb>;
	using vertex_array = qgl::vertex_array_type<qgl::pos3, qgl::tex, qgl::rgb>;
}

#endif
#endif