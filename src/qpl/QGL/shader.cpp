
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

	std::unordered_map<qpl::u8, qgl::shader> shaders;

	void qgl::shader::set_pvm(const std::array<GLfloat, 16>& matrix) {
		const auto& uniform = this->uniforms[qpl::size_cast(qgl::uniform_type::transform_pvm)];
		if (uniform >= 0) {
			glUniformMatrix4fv(qpl::u32_cast(uniform), 1, GL_FALSE, matrix.data());
		}
	}
	void qgl::shader::set_pvm(const qpl::f32* matrix) {
		const auto& uniform = this->uniforms[qpl::size_cast(qgl::uniform_type::transform_pvm)];
		if (uniform >= 0) {
			glUniformMatrix4fv(qpl::u32_cast(uniform), 1, GL_FALSE, matrix);
		}
	}
	void qgl::shader::bind() {
		glUseProgram(this->program);
	}
	void qgl::shader::unbind() {
		glUseProgram(0);
	}
	void qgl::shader::destroy() {
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
}

#endif