#include <qpl/QGL/glew.hpp>

#if defined QPL_INTERN_GLEW_USE
#include <qpl/exception.hpp>


namespace qgl {


	void qgl::gl::generate_vertex_array(GLuint& id) {
		glGenVertexArrays(1, &id);
	}
	void qgl::gl::bind_vertex_array(GLuint id) {
		glBindVertexArray(id);
	}
	void qgl::gl::unbind_vertex_array() {
		glBindVertexArray(0);
	}
	void qgl::gl::buffer_vertex_data(GLuint vertexVBO, qpl::size size, const void* data) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	void qgl::gl::buffer_vertex_sub_data(GLuint vertexVBO, qpl::size offset, qpl::size size, const void* data) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
	void qgl::gl::generate_vertex_buffer(GLuint& vertexVBO, qpl::size size, const void* data) {
		glGenBuffers(1, &vertexVBO);
		qgl::gl::buffer_vertex_data(vertexVBO, size, data);
	}
	void qgl::gl::generate_index_buffer(GLuint& indexVBO, qpl::size size, const void* data) {
		glGenBuffers(1, &indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	void qgl::gl::delete_buffer(GLuint& id) {
		glDeleteBuffers(1, &id);
	}
	void qgl::gl::delete_vertex_array(GLuint& id) {
		glDeleteVertexArrays(1, &id);
	}
	void qgl::gl::draw_elements(primitive_type type, qpl::size size) {
		auto mode = static_cast<GLenum>(type);
		glDrawElements(mode, static_cast<GLsizei>(size), GL_UNSIGNED_INT, 0);
	}
	void qgl::gl::draw_arrays(primitive_type type, qpl::size size) {
		auto mode = static_cast<GLenum>(type);
		glDrawArrays(mode, static_cast<GLint>(0), static_cast<GLsizei>(size));
	}
	std::string qgl::gl::error_description(GLenum error) {
		switch (error) {
		case GL_NO_ERROR:
			return "GL_NO_ERROR: No error has been recorded. The value of this symbolic constant is guaranteed to be 0. ";
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.  ";
		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.  ";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.  ";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return
				"GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete."
				"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded. . ";
		case GL_STACK_UNDERFLOW:
			return "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow. ";
		case GL_STACK_OVERFLOW:
			return "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow. ";
		default:
			return "No Description";
		}
	}


	void qgl::gl::print_errors() {
		qpl::u32 error = glGetError();
		//while (error != GL_NO_ERROR) {
			qpl::println(qgl::gl::error_description(error));
			//error = glGetError();
		//}
	}

	void qgl::gl::attach_shader(GLuint program, GLuint type) {
		glAttachShader(program, type);
	}
	void qgl::gl::create_program(GLuint& program) {
		program = glCreateProgram();
	}
	void qgl::gl::link_program(GLuint program) {
		glLinkProgram(program);
	}
	void qgl::gl::validate_program(GLuint program) {
		glValidateProgram(program);
	}
	void qgl::gl::detach_shader(GLuint program, GLuint type) {
		glDetachShader(program, type);
	}
	void qgl::gl::delete_shader(GLuint shader) {
		glDeleteShader(shader);
	}
	void qgl::gl::bind_shader(GLuint program) {
		glUseProgram(program);
	}
	void qgl::gl::unbind_shader() {
		glUseProgram(0);
	}

	void qgl::gl::enable_vertex_attribute(GLuint position, qpl::size elements, GLenum type, qpl::size stride, qpl::size offset) {
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, static_cast<GLint>(elements), type, GL_FALSE, static_cast<GLsizei>(stride), (void*)offset);
	}
	void qgl::gl::enable(GLenum mode) {
		glEnable(mode);
	}
	void qgl::gl::cull_face(GLenum mode) {
		glCullFace(mode);
	}
	void qgl::gl::front_face(GLenum mode) {
		glFrontFace(mode);
	}
	void qgl::gl::enable_glew_experimental() {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw qpl::exception("qgl::gl::enable_glew_experimental: glewInit() error.");
		}
	}
	void qgl::gl::viewport(GLsizei x0, GLsizei y0, GLsizei x1, GLsizei y1) {
		glViewport(x0, y0, x1, y1);
	}
	void qgl::gl::clear(GLbitfield mask) {
		glClear(mask);
	}
	void qgl::gl::clear_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
		glClearColor(r, g, b, a);
	}
}

#endif