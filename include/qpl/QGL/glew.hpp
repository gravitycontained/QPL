#pragma once
#ifndef QGL_GLEW_HPP
#define QGL_GLEW_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_GLEW_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

namespace qgl {
	//GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
	enum class primitive_type : qpl::u8 {
		points = 0,
		lines = 1,
		line_strip = 3,
		triangles,
		triangle_strip,
		triangle_fan,
		quads,
		quad_strip
	};

	namespace gl {
		QPLDLL void generate_vertex_array(GLuint& id);
		QPLDLL void bind_vertex_array(GLuint id);
		QPLDLL void unbind_vertex_array();
		QPLDLL void buffer_vertex_data(GLuint vertexVBO, qpl::size size, void* data);
		QPLDLL void generate_vertex_buffer(GLuint& vertexVBO, qpl::size size, void* data);
		QPLDLL void generate_index_buffer(GLuint& indexVBO, qpl::size size, void* data);
		QPLDLL void delete_buffer(GLuint& id);
		QPLDLL void delete_vertex_array(GLuint& id);
		QPLDLL void draw_triangles(qpl::size size);
		QPLDLL void draw_elements(primitive_type type, qpl::size size);

		QPLDLL void attach_shader(GLuint program, GLuint shader);
		QPLDLL void create_program(GLuint& program);
		QPLDLL void link_program(GLuint program);
		QPLDLL void validate_program(GLuint program);
		QPLDLL void detach_shader(GLuint program, GLuint shader);
		QPLDLL void delete_shader(GLuint shader);

		QPLDLL void enable_vertex_attribute(GLuint position, qpl::size elements, qpl::size stride, qpl::size offset);

		QPLDLL void enable(GLenum mode);
		QPLDLL void cull_face(GLenum mode);
		QPLDLL void front_face(GLenum mode);
		QPLDLL void enable_glew_experimental();

		QPLDLL void viewport(GLsizei x0, GLsizei y0, GLsizei x1, GLsizei y1);
		QPLDLL void clear(GLbitfield mask);
		QPLDLL void clear_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	}
}

#endif
#endif