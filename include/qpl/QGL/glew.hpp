#pragma once
#ifndef QGL_GLEW_HPP
#define QGL_GLEW_HPP

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>

#include <qpl/defines.hpp>

#if defined QPL_INTERN_GLEW_USE
#include <GL/glew.h>
#include <GL/gl.h>
#else
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83

#define GL_DEPTH_TEST 0x0B71
#define GL_CULL_FACE 0x0B44
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_FRONT_AND_BACK 0x0408
#define GL_CW 0x0900
#define GL_CCW 0x0901

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100

#define GL_TRUE 1
#define GLEW_OK 0
#endif


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