#pragma once
#ifndef QGL_VERTEX_ARRAY_HPP
#define QGL_VERTEX_ARRAY_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_GLEW_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>

#include <qpl/type_traits.hpp>

#include <qpl/QGL/shader.hpp>
#include <qpl/QGL/vertex.hpp>
#include <string>

#include <qpl/QSF/drawables.hpp>

namespace qgl {
	namespace detail {
		template<typename Tuple> requires(qpl::is_tuple<Tuple>())
		constexpr auto vertex_from_tuple_convert() {
			return qpl::tuple_unpack<Tuple>([&](auto... pack) {
				return qgl::vertex<decltype(pack)...>();
			});
		}

		template<typename Tuple> requires(qpl::is_tuple<Tuple>())
		constexpr bool is_valid_vertex_attributes_tuple() {
			return qpl::tuple_unpack<Tuple>([&](auto... pack) {
				return qgl::detail::is_valid_vertex_attributes<decltype(pack)...>();
			});
		}
		template<typename... Attributes>
		constexpr auto vertex_type_convert() {
			if constexpr (qgl::is_primitive_type<qpl::variadic_type_front<Attributes...>>()) {
				return qpl::declval<qgl::primitive_vertex_type<qpl::variadic_type_front<Attributes...>>>();
			}
			else if constexpr (qpl::is_integer<qpl::variadic_type_back<Attributes...>>()) {
				return qgl::detail::vertex_from_tuple_convert<qpl::variadic_type_splice_back<1, Attributes...>>();
			}
			else {
				return qpl::declval<vertex<Attributes...>>();
			}
		}
		template<typename... Attributes>
		constexpr bool is_valid_va_attributes() {
			if constexpr (qgl::is_primitive_type<qpl::variadic_type_front<Attributes...>>()) {
				using tuple = qpl::rename_variadic<qpl::variadic_type_front<Attributes...>, std::tuple>;

				return qpl::tuple_unpack<tuple>([&](auto... pack) {
					return qgl::detail::is_valid_vertex_attributes<decltype(pack)...>();
				});
			}

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

		struct va_dynamic_primitive_type {
			qgl::primitive_type dynamic_primitive_type;

			QPLDLL void set_primitive_type(qgl::primitive_type primitive_type);
			QPLDLL qgl::primitive_type get_primitive_type() const;
		};

		struct va_autoupdate_type {
			mutable bool changed = true;
		};

		template<qpl::u8 info_flag>
		using va_inheritance = qpl::inheritance_flags<info_flag, va_tex, va_shader, va_dynamic_primitive_type, va_autoupdate_type>;

		template<typename... Attributes>
		constexpr static bool va_using_custom_indices_uint() {
			return qpl::is_integer<qpl::variadic_type_back<Attributes...>>();
		}

		template<typename... Attributes> requires(qgl::detail::is_valid_va_attributes<Attributes...>())
		using va_index_type = typename qpl::conditional_identity<qpl::if_true<va_using_custom_indices_uint<Attributes...>()>, qpl::variadic_type_back<Attributes...>, qpl::u32>::type;

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
		constexpr static bool is_static_primitive_type() {
			return qgl::is_primitive_type<qpl::variadic_type_front<Attributes...>>();
		}

		using index_type = typename qpl::conditional_identity<qpl::if_true<using_custom_indices_uint()>, qpl::variadic_type_back<Attributes...>, qpl::u32>::type;
		using vertex_type = typename decltype(qgl::detail::vertex_type_convert<Attributes...>());
		using vertex_tuple_type = qpl::rename_variadic_identity<vertex_type, std::tuple>::type;


		using primitive_type = qpl::variadic_type_front<Attributes...>;

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


		void generate() const {

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

			if constexpr (is_static_primitive_type()) {
				qpl::tuple_unpack<vertex_tuple_type>([&](auto... pack) {
					(add_attribute.template operator() < decltype(pack) > (), ...);
					});
			}
			else {
				(add_attribute.template operator() < Attributes > (), ...);
			}

			if constexpr (has_indices()) {
				qgl::gl::generate_index_buffer(this->indexVBO, this->draw_count() * sizeof(index_type), this->indices.data());
			}
			qgl::gl::bind_vertex_array(0);
			this->unbind_shader();
		}

		void update() const {
			if (!this->generated()) {
				this->generate();
			}
			qgl::gl::bind_vertex_array(this->id);
			qgl::gl::buffer_vertex_data(this->vertexVBO, this->draw_count() * vertex_type::stride(), this->vertices.data());
			qgl::gl::unbind_vertex_array();
		}
		void update_sub(qpl::size start, qpl::size end) const {
			if (!this->generated()) {
				this->generate();
			}
			qgl::gl::bind_vertex_array(this->id);
			qgl::gl::buffer_vertex_sub_data(this->vertexVBO, start * vertex_type::stride(), (end - start) * vertex_type::stride(), this->vertices.data());
			//qgl::gl::buffer_vertex_sub_data(this->vertexVBO, start * vertex_type::stride(), (end - start) * vertex_type::stride(), this->vertices.data());
			//qgl::gl::buffer_vertex_sub_data(this->vertexVBO, start * vertex_type::stride(), this->draw_count() * vertex_type::stride(), this->vertices.data());
			qgl::gl::unbind_vertex_array();
		}

		void bind_shader() const {
			if constexpr (has_shader()) {
				if (!this->shader.loaded) {
					if constexpr (is_static_primitive_type()) {
						qpl::tuple_unpack<vertex_tuple_type>([&](auto... pack) {
							this->shader.set_vertex(qgl::get_vertex_shader<decltype(pack)...>());
							this->shader.set_fragment(qgl::get_fragment_shader<decltype(pack)...>());
						});
					}
					else {
						this->shader.set_vertex(qgl::get_vertex_shader<Attributes...>());
						this->shader.set_fragment(qgl::get_fragment_shader<Attributes...>());
					}
				}

				this->shader.bind();
			}
			else {
				if constexpr (is_static_primitive_type()) {
					qpl::tuple_unpack<vertex_tuple_type>([&](auto... pack) {
						qgl::check_shader<decltype(pack)...>();
						qgl::bind_shader<decltype(pack)...>();
					});
				}
				else {
					qgl::check_shader<Attributes...>();
					qgl::bind_shader<Attributes...>();
				}
			}
		}
		void unbind_shader() const {
			if constexpr (has_shader()) {
				this->shader.unbind();
			}
			else {
				if constexpr (is_static_primitive_type()) {
					qpl::tuple_unpack<vertex_tuple_type>([&](auto... pack) {
						qgl::unbind_shader<decltype(pack)...>();
					});
				}
				else {
					qgl::unbind_shader<Attributes...>();
				}
			}
		}
		void draw(qsf::draw_object_gl& draw) const {
			draw.window->setActive(true);
			
			this->bind_shader();

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
				if constexpr (is_static_primitive_type()) {
					qpl::constexpr_iterate<qpl::tuple_size<vertex_tuple_type>()>([&](auto&&... pack) {
						qgl::check_shader<qpl::tuple_type<pack.i, vertex_tuple_type>...>();
						qgl::bind_shader<qpl::tuple_type<pack.i, vertex_tuple_type>...>();
						qgl::get_shader<qpl::tuple_type<pack.i, vertex_tuple_type>...>().set_pvm(view);
					});
				}
				else {
					qgl::check_shader<Attributes...>();
					qgl::bind_shader<Attributes...>();
					qgl::get_shader<Attributes...>().set_pvm(view);
				}
			}

			qgl::gl::bind_vertex_array(this->id);

			if constexpr (is_static_primitive_type()) {

				qgl::primitive_type type;
				if constexpr (qgl::is_primitive_points<primitive_type>()) {
					type = qgl::primitive_type::points;
				}
				else if constexpr (qgl::is_primitive_lines<primitive_type>()) {
					type = qgl::primitive_type::lines;
				}
				else if constexpr (qgl::is_primitive_triangles<primitive_type>()) {
					type = qgl::primitive_type::triangles;
				}
				else if constexpr (qgl::is_primitive_quads<primitive_type>()) {
					type = qgl::primitive_type::quads;
				}


				if constexpr (has_indices()) {
					qgl::gl::draw_elements(type, this->draw_count());
				}
				else {
					qgl::gl::draw_arrays(type, this->draw_count());
				}
			}
			else {
				if constexpr (has_primitive_type() && has_indices()) {
					qgl::gl::draw_elements(this->dynamic_primitive_type, this->draw_count());
				}
				else if constexpr (has_primitive_type() && !has_indices()) {
					qgl::gl::draw_arrays(this->dynamic_primitive_type, this->draw_count());
				}
				else if constexpr (!has_primitive_type() && has_indices()) {
					qgl::gl::draw_elements(qgl::primitive_type::triangles, this->draw_count());
				}
				else if constexpr (!has_primitive_type() && !has_indices()) {
					qgl::gl::draw_arrays(qgl::primitive_type::triangles, this->draw_count());
				}
			}

			qgl::gl::unbind_vertex_array();
			if constexpr (vertex_type::has_tex()) {
				if (this->texture) {
					this->unbind();
				}
			}

			draw.window->setActive(false);
			this->unbind_shader();
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
		template<typename C> requires(qpl::is_contiguous_container<C>())
		constexpr void add(const C& va) {
			auto size = this->vertices.size();
			this->vertices.resize(size + va.size());
			//if constexpr (qpl::is_same<qpl::container_subtype<C>, vertex_type>()) {
			//	std::memcpy(this->vertices.data() + size, va.data(), sizeof(vertex_type) * va.size());
			//}
			//else {
				for (qpl::size i = 0u; i < va.size(); ++i) {
					this->vertices[i + size] = va[i];
				}
			//}
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

				if constexpr (qpl::is_same<vertex_type, qgl::vertex_array<info_flag2, Ts...>::vertex_type>()) {
					std::memcpy(this->vertices.data() + vertices_size, other.vertices.data(), other.vertices.size() * sizeof(vertex_type));
				}
				else {
					for (qpl::size i = 0u; i < other.vertices.size(); ++i) {
						this->vertices[vertices_size + i] = other.vertices[i];
					}
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