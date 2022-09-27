#pragma once
#ifndef QGL_GLEW_HPP
#define QGL_GLEW_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_GLEW_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>

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
		QPLDLL void buffer_vertex_data(GLuint vertexVBO, qpl::size size, const void* data);
		QPLDLL void buffer_vertex_sub_data(GLuint vertexVBO, qpl::size ofset, qpl::size size, const void* data);
		QPLDLL void generate_vertex_buffer(GLuint& vertexVBO, qpl::size size, const void* data);
		QPLDLL void generate_index_buffer(GLuint& indexVBO, qpl::size size, const void* data);
		QPLDLL void delete_buffer(GLuint& id);
		QPLDLL void delete_vertex_array(GLuint& id);
		QPLDLL void draw_elements(primitive_type type, qpl::size size);
		QPLDLL void draw_arrays(primitive_type type, qpl::size size);
		QPLDLL std::string error_description(GLenum error);
		QPLDLL void print_errors();

		QPLDLL void attach_shader(GLuint program, GLuint shader);
		QPLDLL void create_program(GLuint& program);
		QPLDLL void link_program(GLuint program);
		QPLDLL void validate_program(GLuint program);
		QPLDLL void detach_shader(GLuint program, GLuint shader);
		QPLDLL void delete_shader(GLuint shader);
		QPLDLL void bind_shader(GLuint program);
		QPLDLL void unbind_shader();

		QPLDLL void enable_vertex_attribute(GLuint position, qpl::size elements, GLenum type, qpl::size stride, qpl::size offset);

		/*
		#define GL_BYTE 0x1400
		#define GL_UNSIGNED_BYTE 0x1401
		#define GL_SHORT 0x1402
		#define GL_UNSIGNED_SHORT 0x1403
		#define GL_INT 0x1404
		#define GL_UNSIGNED_INT 0x1405
		#define GL_FLOAT 0x1406
		#define GL_2_BYTES 0x1407
		#define GL_3_BYTES 0x1408
		#define GL_4_BYTES 0x1409
		#define GL_DOUBLE 0x140A
		*/
		template<typename T>
		void enable_vertex_attribute(GLuint position, qpl::size elements, qpl::size stride, qpl::size offset) {
			if constexpr (qpl::is_same<T, qpl::i8>()) {
				qgl::gl::enable_vertex_attribute(position, elements, GL_BYTE, stride, offset);
			}
			else if constexpr (qpl::is_same<T, qpl::u8>()) {
				qgl::gl::enable_vertex_attribute(position, elements, GL_UNSIGNED_BYTE, stride, offset);
			}
			if constexpr (qpl::is_same<T, qpl::i16>()) {
				qgl::gl::enable_vertex_attribute(position, elements, GL_SHORT, stride, offset);
			}
			else if constexpr (qpl::is_same<T, qpl::u16>()) {
				qgl::gl::enable_vertex_attribute(position, elements, GL_UNSIGNED_SHORT, stride, offset);
			}
			if constexpr (qpl::is_same<T, qpl::i32>()) {
				qgl::gl::enable_vertex_attribute(position, elements, GL_INT, stride, offset);
			}
			else if constexpr (qpl::is_same<T, qpl::u32>()) {
				qgl::gl::enable_vertex_attribute(position, elements, GL_UNSIGNED_INT, stride, offset);
			}
			else if constexpr (qpl::is_same<T, float>()) {
				qgl::gl::enable_vertex_attribute(position, elements, GL_FLOAT, stride, offset);
			}
			else if constexpr (qpl::is_same<T, double>()) {
				qgl::gl::enable_vertex_attribute(position, elements, GL_DOUBLE, stride, offset);
			}
		}



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