#pragma once
#ifndef QGL_SHAPES_HPP
#define QGL_SHAPES_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_GLEW_USE

#include <qpl/QGL/vertex.hpp>
#include <qpl/QSF/drawables.hpp>

#include <qpl/vector.hpp>
#include <qpl/color.hpp>

#include <array>

namespace qgl {

	constexpr auto get_cube() {
		constexpr auto array = std::array{
				qgl::make_vertex(qpl::vec(0, 0, 0), qpl::tex(0, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 0, 0), qpl::tex(1, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 1, 0), qpl::tex(1, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 1, 0), qpl::tex(0, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 1, 0), qpl::tex(1, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 1, 1), qpl::tex(0, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 0, 1), qpl::tex(0, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 0, 0), qpl::tex(1, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 0, 1), qpl::tex(0, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 0, 1), qpl::tex(1, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 1, 1), qpl::tex(1, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 1, 1), qpl::tex(0, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 0, 1), qpl::tex(0, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 0, 0), qpl::tex(1, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 1, 0), qpl::tex(1, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 1, 1), qpl::tex(0, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 0, 1), qpl::tex(0, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 0, 1), qpl::tex(1, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 0, 0), qpl::tex(1, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 0, 0), qpl::tex(0, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 1, 0), qpl::tex(0, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 1, 0), qpl::tex(1, 1), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(1, 1, 1), qpl::tex(1, 0), qpl::frgb::white),
				qgl::make_vertex(qpl::vec(0, 1, 1), qpl::tex(0, 0), qpl::frgb::white),
		};
		constexpr auto indices = std::array<qpl::u32, 36> {
				0, 1, 2, 0, 2, 3,
				6, 5, 4, 7, 6, 4,
				10, 9, 8, 11, 10, 8,
				12, 13, 14, 12, 14, 15,
				18, 17, 16, 19, 18, 16,
				20, 21, 22, 20, 22, 23 };
		
		return qgl::make_vertex_index_array(array, indices);
	}

	struct cuboid {
		cuboid();

		qpl::vec3 position;
		qpl::vec3 dimension;
		qpl::rgb color;
		mutable bool changed = true;
		mutable qgl::vertex_index_array vertex_array;

		QPLDLL void apply() const;

		QPLDLL void set_position(qpl::vec3 position);
		QPLDLL void set_dimension(qpl::vec3 dimension);
		QPLDLL void set_center(qpl::vec3 position);

		QPLDLL void move(qpl::vec3 delta);

		QPLDLL void set_color(qpl::rgb color);

		QPLDLL qpl::vec3 get_center() const;

		QPLDLL void draw(qsf::draw_object_gl& draw) const;
	};
}

#endif
#endif