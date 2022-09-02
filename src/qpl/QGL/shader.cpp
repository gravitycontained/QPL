
#include <qpl/QGL/shader.hpp>

#if defined QPL_INTERN_GLEW_USE

#include <GL/glew.h>
#include <GL/GL.h>
#include <qpl/system.hpp>

namespace qgl {

	void qgl::check_shader_error(GLuint shader, GLuint flag, bool program, const std::string& error) {
		GLint success = 0;
		GLchar error_buffer[1024] = { 0 };
		if (program) {
			glGetProgramiv(shader, flag, &success);
		}
		else {
			glGetShaderiv(shader, flag, &success);
		}

		if (!success) {
			if (program) {
				glGetProgramInfoLog(shader, sizeof(error_buffer), nullptr, error_buffer);
			}
			else {
				glGetShaderInfoLog(shader, sizeof(error_buffer), nullptr, error_buffer);
			}

			qpl::println(error, " : ", error_buffer);
		}
	}

	GLuint qgl::build_shader(const std::string& src, qpl::u32 type) {
		GLuint shaderID = glCreateShader(type);
		if (!shaderID) {
			qpl::println("Bad shader type!");
			return 0;
		}
		const GLchar* sources[1];
		GLint lengths[1];
		sources[0] = src.c_str();
		lengths[0] = static_cast<GLint>(src.length());
		glShaderSource(shaderID, 1, sources, lengths);
		glCompileShader(shaderID);
		qgl::check_shader_error(shaderID, GL_COMPILE_STATUS, false, "Shader compile error: ");
		return shaderID;
	}

	std::unordered_map<qpl::u8, qgl::shader_small> shaders;


	const std::string& qgl::detail::shader_save_string::get_vertex_shader() const {
		return this->vertex;
	}
	const std::string& qgl::detail::shader_save_string::get_fragment_shader() const {
		return this->fragment;
	}
	const std::string& qgl::detail::shader_save_string::get_geometry_shader() const {
		return this->geometry;
	}
}

#endif