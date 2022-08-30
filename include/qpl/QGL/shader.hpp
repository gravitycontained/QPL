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


	template<typename ... Attributes> requires(qgl::is_attribute<Attributes>() && ...)
	std::string get_vertex_shader(qpl::u32 version = 330u) {
		constexpr auto pos3 = qgl::has_pos3<Attributes...>();
		constexpr auto pos2 = qgl::has_pos2<Attributes...>();
		constexpr auto rgb = qgl::has_rgb<Attributes...>();
		constexpr auto rgba = qgl::has_rgba<Attributes...>();
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
	template<typename ... Attributes> requires(qgl::is_attribute<Attributes>() && ...)
	std::string get_compact_vertex_shader(qpl::u32 version = 330u) {
		constexpr auto pos3 = qgl::has_pos3<Attributes...>();
		constexpr auto pos2 = qgl::has_pos2<Attributes...>();
		constexpr auto rgb = qgl::has_rgb<Attributes...>();
		constexpr auto rgba = qgl::has_rgba<Attributes...>();
		constexpr auto tex = qgl::has_tex<Attributes...>();

		std::ostringstream stream;
		stream << "#version " << version << '\n';

		if constexpr (pos3) {
			stream << "layout(location=0) in vec3 position;";
		}
		else if constexpr (pos2) {
			stream << "layout(location=0) in vec2 position;";
		}

		if constexpr (rgb) {
			stream << "layout(location=1) in vec3 color;";
		}
		else if constexpr (rgba) {
			stream << "layout(location=1) in vec4 color;";
		}

		if constexpr (tex) {
			stream << "layout(location=2) in vec2 tex_coord;";
		}


		if constexpr (rgb) {
			stream << "out vec3 frag_color;";
		}
		else if constexpr (rgba) {
			stream << "out vec4 frag_color;";
		}

		stream << "uniform mat4 pvm;";

		if constexpr (tex) {
			stream << "out vec2 uv;";
		}


		stream << "void main(){";

		if constexpr (pos2) {
			stream << "gl_Position=pvm*vec4(position,1,1);";
		}
		else if constexpr (pos3) {
			stream << "gl_Position=pvm*vec4(position,1);";
		}
		else {
			stream << "gl_Position=pvm*vec4(1);";
		}
		if constexpr (tex) {
			stream << "uv=tex_coord;";
		}
		if constexpr (rgb || rgba) {
			stream << "frag_color=color;";
		}
		stream << "}";

		return stream.str();
	}

	template<typename ... Attributes> requires(qgl::is_attribute<Attributes>() && ...)
	std::string get_fragment_shader(qpl::u32 version = 330u) {
		constexpr auto pos3 = qgl::has_pos3<Attributes...>();
		constexpr auto pos2 = qgl::has_pos2<Attributes...>();
		constexpr auto rgb = qgl::has_rgb<Attributes...>();
		constexpr auto rgba = qgl::has_rgba<Attributes...>();
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
	template<typename ... Attributes> requires(qgl::is_attribute<Attributes>() && ...)
	std::string get_compact_fragment_shader(qpl::u32 version = 330u) {
		constexpr auto pos3 = qgl::has_pos3<Attributes...>();
		constexpr auto pos2 = qgl::has_pos2<Attributes...>();
		constexpr auto rgb = qgl::has_rgb<Attributes...>();
		constexpr auto rgba = qgl::has_rgba<Attributes...>();
		constexpr auto tex = qgl::has_tex<Attributes...>();

		std::ostringstream stream;
		stream << "#version " << version << '\n';

		if constexpr (tex) {
			stream << "uniform sampler2D texture;";
			stream << "in vec2 uv;";
		}
		if constexpr (rgb) {
			stream << "in vec3 frag_color;";
		}
		else if constexpr (rgba) {
			stream << "in vec4 frag_color;";
		}
		stream << "void main(){";

		if constexpr (tex) {
			stream << "vec4 tex_color=texture2D(texture,uv);";
		}
		if constexpr (rgb || rgba) {
			stream << "gl_FragColor=";

			if constexpr (tex) {
				stream << "tex_color*";
			}
			if constexpr (rgb) {
				stream << "vec4(frag_color,0);";
			}
			else if constexpr (rgba) {
				stream << "frag_color;";
			}
		}
		stream << "}";

		return stream.str();
	}

	constexpr auto shader_string_data = std::array {
		std::make_tuple(0b1, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633220706f736974696f6e3b756e69666f726d206d6174342070766d3b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c312c31293b7d",
		"2376657273696f6e203333300a766f6964206d61696e28297b7d"),
		std::make_tuple(0b10, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633320706f736974696f6e3b756e69666f726d206d6174342070766d3b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c31293b7d",
		"2376657273696f6e203333300a766f6964206d61696e28297b7d"),
		std::make_tuple(0b100, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d312920696e207665633320636f6c6f723b6f7574207665633320667261675f636f6c6f723b756e69666f726d206d6174342070766d3b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a766563342831293b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a696e207665633320667261675f636f6c6f723b766f6964206d61696e28297b676c5f46726167436f6c6f723d7665633428667261675f636f6c6f722c30293b7d"),
		std::make_tuple(0b1000, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d312920696e207665633420636f6c6f723b6f7574207665633420667261675f636f6c6f723b756e69666f726d206d6174342070766d3b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a766563342831293b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a696e207665633420667261675f636f6c6f723b766f6964206d61696e28297b676c5f46726167436f6c6f723d667261675f636f6c6f723b7d"),
		std::make_tuple(0b10000, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a766563342831293b75763d7465785f636f6f72643b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b7d"),
		std::make_tuple(0b101, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633220706f736974696f6e3b6c61796f7574286c6f636174696f6e3d312920696e207665633320636f6c6f723b6f7574207665633320667261675f636f6c6f723b756e69666f726d206d6174342070766d3b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c312c31293b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a696e207665633320667261675f636f6c6f723b766f6964206d61696e28297b676c5f46726167436f6c6f723d7665633428667261675f636f6c6f722c30293b7d"),
		std::make_tuple(0b1001, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633220706f736974696f6e3b6c61796f7574286c6f636174696f6e3d312920696e207665633420636f6c6f723b6f7574207665633420667261675f636f6c6f723b756e69666f726d206d6174342070766d3b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c312c31293b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a696e207665633420667261675f636f6c6f723b766f6964206d61696e28297b676c5f46726167436f6c6f723d667261675f636f6c6f723b7d"),
		std::make_tuple(0b10001, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633220706f736974696f6e3b6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c312c31293b75763d7465785f636f6f72643b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b7d"),
		std::make_tuple(0b110, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633320706f736974696f6e3b6c61796f7574286c6f636174696f6e3d312920696e207665633320636f6c6f723b6f7574207665633320667261675f636f6c6f723b756e69666f726d206d6174342070766d3b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c31293b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a696e207665633320667261675f636f6c6f723b766f6964206d61696e28297b676c5f46726167436f6c6f723d7665633428667261675f636f6c6f722c30293b7d"),
		std::make_tuple(0b1010, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633320706f736974696f6e3b6c61796f7574286c6f636174696f6e3d312920696e207665633420636f6c6f723b6f7574207665633420667261675f636f6c6f723b756e69666f726d206d6174342070766d3b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c31293b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a696e207665633420667261675f636f6c6f723b766f6964206d61696e28297b676c5f46726167436f6c6f723d667261675f636f6c6f723b7d"),
		std::make_tuple(0b10010, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633320706f736974696f6e3b6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c31293b75763d7465785f636f6f72643b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b7d"),
		std::make_tuple(0b10100, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d312920696e207665633320636f6c6f723b6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b6f7574207665633320667261675f636f6c6f723b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a766563342831293b75763d7465785f636f6f72643b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b696e207665633320667261675f636f6c6f723b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b676c5f46726167436f6c6f723d7465785f636f6c6f722a7665633428667261675f636f6c6f722c30293b7d"),
		std::make_tuple(0b11000, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d312920696e207665633420636f6c6f723b6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b6f7574207665633420667261675f636f6c6f723b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a766563342831293b75763d7465785f636f6f72643b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b696e207665633420667261675f636f6c6f723b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b676c5f46726167436f6c6f723d7465785f636f6c6f722a667261675f636f6c6f723b7d"),
		std::make_tuple(0b10101, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633220706f736974696f6e3b6c61796f7574286c6f636174696f6e3d312920696e207665633320636f6c6f723b6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b6f7574207665633320667261675f636f6c6f723b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c312c31293b75763d7465785f636f6f72643b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b696e207665633320667261675f636f6c6f723b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b676c5f46726167436f6c6f723d7465785f636f6c6f722a7665633428667261675f636f6c6f722c30293b7d"),
		std::make_tuple(0b11001, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633220706f736974696f6e3b6c61796f7574286c6f636174696f6e3d312920696e207665633420636f6c6f723b6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b6f7574207665633420667261675f636f6c6f723b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c312c31293b75763d7465785f636f6f72643b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b696e207665633420667261675f636f6c6f723b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b676c5f46726167436f6c6f723d7465785f636f6c6f722a667261675f636f6c6f723b7d"),
		std::make_tuple(0b11010, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633320706f736974696f6e3b6c61796f7574286c6f636174696f6e3d312920696e207665633420636f6c6f723b6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b6f7574207665633420667261675f636f6c6f723b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c31293b75763d7465785f636f6f72643b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b696e207665633420667261675f636f6c6f723b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b676c5f46726167436f6c6f723d7465785f636f6c6f722a667261675f636f6c6f723b7d"),
		std::make_tuple(0b10110, "2376657273696f6e203333300a6c61796f7574286c6f636174696f6e3d302920696e207665633320706f736974696f6e3b6c61796f7574286c6f636174696f6e3d312920696e207665633320636f6c6f723b6c61796f7574286c6f636174696f6e3d322920696e2076656332207465785f636f6f72643b6f7574207665633320667261675f636f6c6f723b756e69666f726d206d6174342070766d3b6f757420766563322075763b766f6964206d61696e28297b676c5f506f736974696f6e3d70766d2a7665633428706f736974696f6e2c31293b75763d7465785f636f6f72643b667261675f636f6c6f723d636f6c6f723b7d",
		"2376657273696f6e203333300a756e69666f726d2073616d706c6572324420746578747572653b696e20766563322075763b696e207665633320667261675f636f6c6f723b766f6964206d61696e28297b76656334207465785f636f6c6f723d74657874757265324428746578747572652c7576293b676c5f46726167436f6c6f723d7465785f636f6c6f722a7665633428667261675f636f6c6f722c30293b7d"),
	};

	template<typename ... Attributes> requires(qgl::is_attribute<Attributes>() && ...)
	consteval auto get_shader_string_data() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		for (auto& i : shader_string_data) {
			if (std::get<0>(i) == flag) {
				return i;
			}
		}
	}
	template<typename ... Attributes> requires(qgl::is_attribute<Attributes>() && ...)
	std::string get_vertex_shader_string() {
		return qpl::from_hex_string(std::get<1>(get_shader_string_data<Attributes...>()));
	}
	template<typename ... Attributes> requires(qgl::is_attribute<Attributes>() && ...)
	std::string get_fragment_shader_string() {
		return qpl::from_hex_string(std::get<2>(get_shader_string_data<Attributes...>()));
	}

	QPLDLL void check_shader_error(GLuint shader, GLuint flag, bool program, const std::string& error);
	QPLDLL GLuint build_shader(const std::string& src, qpl::u32 type);



	struct shader {
		shader() {
			this->types.fill(0u);
			this->uniforms.fill(0);
			this->program = 0;
			this->loaded = false;
		}

		std::array<GLuint, qpl::size_cast(qgl::shader_type::count)> types;
		std::array<GLint, qpl::size_cast(qgl::uniform_type::count)> uniforms;
		GLuint program = 0;
		bool loaded = false;

		template<typename ... Attributes> requires (qgl::is_attribute<Attributes>() && ...)
		void load(qgl::shader_type type) {
			std::string shader_string;
			if (type == qgl::shader_type::vertex) {
				//shader_string = qgl::get_compact_vertex_shader<Attributes...>();
				shader_string = qgl::get_vertex_shader_string<Attributes...>();
			}
			else if (type == qgl::shader_type::fragment) {
				//shader_string = qgl::get_compact_fragment_shader<Attributes...>();
				shader_string = qgl::get_fragment_shader_string<Attributes...>();
			}

			if (shader_string.empty()) {
				return;
			}

			auto utype = qpl::u32_cast(type);
			if (this->types[utype]) {
				qgl::gl::detach_shader(this->program, this->types[utype]);
				qgl::gl::delete_shader(this->types[utype]);
			}

			switch (type) {
			case qgl::shader_type::vertex:
				this->types[utype] = qgl::build_shader(shader_string, GL_VERTEX_SHADER);
				break;
			case qgl::shader_type::fragment:
				this->types[utype] = qgl::build_shader(shader_string, GL_FRAGMENT_SHADER);
				break;
			case qgl::shader_type::geometry:
				this->types[utype] = qgl::build_shader(shader_string, GL_GEOMETRY_SHADER);
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

		QPLDLL void set_pvm(const std::array<GLfloat, 16>& matrix);
		QPLDLL void set_pvm(const qpl::f32* matrix);
		QPLDLL void bind();
		QPLDLL void unbind();
		QPLDLL void destroy();
	};

	QPLDLL extern std::unordered_map<qpl::u8, shader> shaders;

	template<typename ... Attributes> requires (qgl::is_attribute<Attributes>() && ...)
	void bind_shader() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		auto& shader = qgl::shaders[flag];
		shader.bind();
	}
	template<typename ... Attributes> requires (qgl::is_attribute<Attributes>() && ...)
	void unbind_shader() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		auto& shader = qgl::shaders[flag];
		shader.unbind();
	}
	template<typename ... Attributes> requires (qgl::is_attribute<Attributes>() && ...)
	void check_shader() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		auto& shader = qgl::shaders[flag];
		if (!shader.loaded) {
			shader.load<Attributes...>(qgl::shader_type::vertex);
			shader.load<Attributes...>(qgl::shader_type::fragment);
		}
	}

	template<typename ... Attributes> requires (qgl::is_attribute<Attributes>() && ...)
	qgl::shader& get_shader() {
		constexpr auto flag = qgl::attribute_flag<Attributes...>();
		return qgl::shaders[flag];
	}
}

#endif
#endif