#pragma once
#ifndef QGL_SHADER_HPP
#define QGL_SHADER_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_GLEW_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/system.hpp>
#include <qpl/memory.hpp>

#include <qpl/QGL/glew.hpp>
#include <qpl/QGL/attributes.hpp>

#include <string>
#include <array>
#include <tuple>

namespace qgl {
	enum class shader_type : qpl::u8 {
		vertex = 0u,
		fragment,
		geometry,
		count
	};
	enum class uniform_type : qpl::u8 {
		transform_pvm = 0u,
		count
	};
	enum class vertex_attribute : qpl::u8 {
		position = 0u,
		color,
		tex_coord,
		count
	};


	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
	std::string get_vertex_shader(qpl::u32 version = 330u) {
		constexpr auto pos3 = qgl::has_pos3<Attributes...>();
		constexpr auto pos2 = qgl::has_pos2<Attributes...>();
		constexpr auto rgb = qgl::has_rgb<Attributes...>() || qgl::has_frgb<Attributes...>();
		constexpr auto rgba = qgl::has_rgba<Attributes...>() || qgl::has_frgba<Attributes...>();
		constexpr auto tex = qgl::has_tex<Attributes...>();

		std::ostringstream stream;
		stream << "#version " << version << '\n';

		if constexpr (pos3) {
			stream << "layout (location = 0) in vec3 position;\n";
		}
		else if constexpr (pos2) {
			stream << "layout (location = 0) in vec2 position;\n";
		}

		if constexpr (rgb) {
			stream << "layout (location = 1) in vec3 color;\n";
		}
		else if constexpr (rgba) {
			stream << "layout (location = 1) in vec4 color;\n";
		}

		if constexpr (tex) {
			stream << "layout (location = 2) in vec2 tex_coord;\n";
		}


		if constexpr (rgb) {
			stream << "out vec3 frag_color;\n";
		}
		else if constexpr (rgba) {
			stream << "out vec4 frag_color;\n";
		}

		stream << "uniform mat4 pvm;\n";

		if constexpr (tex) {
			stream << "out vec2 uv;\n";
		}


		stream << "void main() {\n";

		if constexpr (pos2) {
			stream << "	gl_Position = pvm * vec4(position, 1.0f, 1.0f);\n";
		}
		else if constexpr (pos3) {
			stream << "	gl_Position = pvm * vec4(position, 1.0f);\n";
		}
		else {
			stream << "	gl_Position = pvm * vec4(1.0f);\n";
		}
		if constexpr (tex) {
			stream << "	uv = tex_coord;\n";
		}
		if constexpr (rgb || rgba) {
			stream << "	frag_color = color;\n";
		}
		stream << "}\n";

		return stream.str();
	}
	template<typename ... Attributes> requires(qgl::is_vertex_attribute<Attributes>() && ...)
	constexpr std::string get_fragment_shader(qpl::u32 version = 330u) {
		constexpr auto pos3 = qgl::has_pos3<Attributes...>();
		constexpr auto pos2 = qgl::has_pos2<Attributes...>();
		constexpr auto rgb = qgl::has_rgb<Attributes...>() || qgl::has_frgb<Attributes...>();
		constexpr auto rgba = qgl::has_rgba<Attributes...>() || qgl::has_frgba<Attributes...>();
		constexpr auto tex = qgl::has_tex<Attributes...>();

		std::ostringstream stream;
		stream << "#version " << version << '\n';

		if constexpr (tex) {
			stream << "uniform sampler2D texture;\n";
			stream << "in vec2 uv;\n";
		}
		if constexpr (rgb) {
			stream << "in vec3 frag_color;\n";
		}
		else if constexpr (rgba) {
			stream << "in vec4 frag_color;\n";
		}
		stream << "void main() {\n";

		if constexpr (tex) {
			stream << "	vec4 tex_color = texture2D(texture, uv);\n";
		}
		if constexpr (rgb || rgba) {
			stream << "	gl_FragColor = ";

			if constexpr (tex) {
				stream << "tex_color * ";
			}
			if constexpr (rgb) {
				stream << "vec4(frag_color, 0.0f);\n";
			}
			else if constexpr (rgba) {
				stream << "frag_color;\n";
			}
		}
		stream << "}\n";

		return stream.str();
	}


	QPLDLL void check_shader_error(GLuint shader, GLuint flag, bool program, const std::string& error);
	QPLDLL GLuint build_shader(const std::string& src, qpl::u32 type);

	namespace detail {
		struct shader_save_string {
			QPLDLL const std::string& get_vertex_shader() const;
			QPLDLL const std::string& get_fragment_shader() const;
			QPLDLL const std::string& get_geometry_shader() const;

		protected:
			std::string vertex;
			std::string fragment;
			std::string geometry;
		};

		template<bool save_string>
		using shader_save_string_conditional = qpl::conditional<qpl::if_true<save_string>, shader_save_string, qpl::empty_type>;
	}

	template<bool save_string>
	struct shader_impl : detail::shader_save_string_conditional<save_string> {
		shader_impl() {
			this->types.fill(0u);
			this->uniforms.fill(0);
			this->program = 0;
			this->loaded = false;
		}

		static constexpr bool using_saved_string() {
			return save_string;
		}

		std::array<GLuint, qpl::size_cast(qgl::shader_type::count)> types;
		std::array<GLint, qpl::size_cast(qgl::uniform_type::count)> uniforms;
		GLuint program = 0;
		bool loaded = false;

		void set(const std::string& shader, qgl::shader_type type) {
			if (shader.empty()) {
				return;
			}

			auto utype = qpl::u32_cast(type);
			if (this->types[utype]) {
				qgl::gl::detach_shader(this->program, this->types[utype]);
				qgl::gl::delete_shader(this->types[utype]);
			}

			switch (type) {
			case qgl::shader_type::vertex:
				if constexpr (using_saved_string()) {
					this->vertex = shader;
				}
				this->types[utype] = qgl::build_shader(shader, GL_VERTEX_SHADER);
				break;
			case qgl::shader_type::fragment:
				if constexpr (using_saved_string()) {
					this->fragment = shader;
				}
				this->types[utype] = qgl::build_shader(shader, GL_FRAGMENT_SHADER);
				break;
			case qgl::shader_type::geometry:
				if constexpr (using_saved_string()) {
					this->geometry = shader;
				}
				this->types[utype] = qgl::build_shader(shader, GL_GEOMETRY_SHADER);
				break;
			default:
				break;
			}

			if (this->program == 0) {
				qgl::gl::create_program(this->program);
			}

			qgl::gl::attach_shader(this->program, this->types[utype]);

			qgl::gl::link_program(this->program);
			qgl::check_shader_error(this->program, GL_LINK_STATUS, true, "Shader link error:");

			qgl::gl::validate_program(this->program);
			qgl::check_shader_error(this->program, GL_VALIDATE_STATUS, true, "Invalid shader:");

			this->loaded = true;
		}
		void set_fragment(const std::string& shader) {
			this->set(shader, qgl::shader_type::fragment);
		}
		void set_vertex(const std::string& shader) {
			this->set(shader, qgl::shader_type::vertex);
		}
		void set_geometry(const std::string& shader) {
			this->set(shader, qgl::shader_type::geometry);
		}
		void set_pvm(const std::array<GLfloat, 16>& matrix) {
			const auto& uniform = this->uniforms[qpl::size_cast(qgl::uniform_type::transform_pvm)];
			if (uniform >= 0) {
				glUniformMatrix4fv(qpl::u32_cast(uniform), 1, GL_FALSE, matrix.data());
			}
		}
		void set_pvm(const qpl::f32* matrix) {
			const auto& uniform = this->uniforms[qpl::size_cast(qgl::uniform_type::transform_pvm)];
			if (uniform >= 0) {
				glUniformMatrix4fv(qpl::u32_cast(uniform), 1, GL_FALSE, matrix);
			}
		}
		void bind() const {
			qgl::gl::bind_shader(this->program);
		}
		void unbind() const {
			qgl::gl::unbind_shader();
		}
		void destroy() {
			for (auto& type : this->types) {
				glDetachShader(this->program, type);
				glDeleteShader(type);
				type = 0;
			}
			for (auto& uniform : this->uniforms) {
				uniform = -1;
			}
			glDeleteProgram(this->program);
			this->program = 0;
		}
	};

	using shader = shader_impl<true>;
	using shader_small = shader_impl<true>;

	QPLDLL extern std::unordered_map<qpl::u8, shader_small> shaders;

	template<typename ... Attributes> requires (qgl::is_vertex_attribute<Attributes>() && ...)
	void bind_shader() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		auto& shader = qgl::shaders[flag];
		shader.bind();
	}
	template<typename ... Attributes> requires (qgl::is_vertex_attribute<Attributes>() && ...)
	void unbind_shader() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		auto& shader = qgl::shaders[flag];
		shader.unbind();
	}
	template<typename ... Attributes> requires (qgl::is_vertex_attribute<Attributes>() && ...)
	void check_shader() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		auto& shader = qgl::shaders[flag];
		if (!shader.loaded) {
			shader.set_vertex(qgl::get_vertex_shader<Attributes...>());
			shader.set_fragment(qgl::get_fragment_shader<Attributes...>());
		}
	}

	template<typename ... Attributes> requires (qgl::is_vertex_attribute<Attributes>() && ...)
	qgl::shader_small& get_shader() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		return qgl::shaders[flag];
	}
}

#endif
#endif