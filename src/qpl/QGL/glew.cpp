#include <qpl/QGL/glew.hpp>

#include <qpl/exception.hpp>

#if defined QPL_INTERN_GLEW_USE

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
	void qgl::gl::buffer_vertex_data(GLuint vertexVBO, qpl::size size, void* data) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	void qgl::gl::generate_vertex_buffer(GLuint& vertexVBO, qpl::size size, void* data) {
		glGenBuffers(1, &vertexVBO);
		qgl::gl::buffer_vertex_data(vertexVBO, size, data);
	}
	void qgl::gl::generate_index_buffer(GLuint& indexVBO, qpl::size size, void* data) {
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
	void qgl::gl::draw_triangles(qpl::size size) {
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(size), GL_UNSIGNED_INT, 0);
	}
	void qgl::gl::draw_elements(primitive_type type, qpl::size size) {
		auto mode = static_cast<GLenum>(type);
		glDrawArrays(mode, static_cast<GLint>(0), static_cast<GLsizei>(size));
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

	void qgl::gl::enable_vertex_attribute(GLuint position, qpl::size elements, qpl::size stride, qpl::size offset) {
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, static_cast<GLint>(elements), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), (void*)offset);
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