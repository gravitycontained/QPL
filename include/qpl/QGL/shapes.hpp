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
#include <map>
#include <tuple>

namespace qgl {

	constexpr auto get_cube() {
		constexpr auto array = std::array{
				qgl::make_vertex(qpl::vec(0, 0, 0), qpl::tex(0, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 0, 0), qpl::tex(1, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 1, 0), qpl::tex(1, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 1, 0), qpl::tex(0, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 1, 0), qpl::tex(1, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 1, 1), qpl::tex(0, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 0, 1), qpl::tex(0, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 0, 0), qpl::tex(1, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 0, 1), qpl::tex(0, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 0, 1), qpl::tex(1, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 1, 1), qpl::tex(1, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 1, 1), qpl::tex(0, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 0, 1), qpl::tex(0, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 0, 0), qpl::tex(1, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 1, 0), qpl::tex(1, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 1, 1), qpl::tex(0, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 0, 1), qpl::tex(0, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 0, 1), qpl::tex(1, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 0, 0), qpl::tex(1, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 0, 0), qpl::tex(0, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 1, 0), qpl::tex(0, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 1, 0), qpl::tex(1, 1), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(1, 1, 1), qpl::tex(1, 0), qpl::frgb::white()),
				qgl::make_vertex(qpl::vec(0, 1, 1), qpl::tex(0, 0), qpl::frgb::white()),
		};
		constexpr auto indices = std::array<qpl::u32, 36> {
				0, 1, 2, 0, 2, 3,
				6, 5, 4, 7, 6, 4,
				10, 9, 8, 11, 10, 8,
				12, 13, 14, 12, 14, 15,
				18, 17, 16, 19, 18, 16,
				20, 21, 22, 20, 22, 23 };
		
		return qgl::make_va_index(array, indices);
	}

	constexpr auto get_ico_sphere() {
		constexpr qpl::f32 A = 0.850650808352039932f;
		constexpr qpl::f32 B = 0.525731112119133606f;
		constexpr qpl::f32 C = 0.f;

		constexpr auto array = std::array{
			qgl::make_vertex(qpl::vec(-B,  C,  A)),
			qgl::make_vertex(qpl::vec( B,  C,  A)),
			qgl::make_vertex(qpl::vec(-B,  C, -A)),
			qgl::make_vertex(qpl::vec( B,  C, -A)),
			qgl::make_vertex(qpl::vec( C,  A,  B)),
			qgl::make_vertex(qpl::vec( C,  A, -B)),
			qgl::make_vertex(qpl::vec( C, -A,  B)),
			qgl::make_vertex(qpl::vec( C, -A, -B)),
			qgl::make_vertex(qpl::vec( A,  B,  C)),
			qgl::make_vertex(qpl::vec(-A,  B,  C)),
			qgl::make_vertex(qpl::vec( A, -B,  C)),
			qgl::make_vertex(qpl::vec(-A, -B,  C)),
		};
		constexpr auto indices = std::array<qpl::u32, 60> {
			0,4,1,  0,9,4,  9,5,4,  4,5,8,  4,8,1,
			8,10,1, 8,3,10, 5,3,8,  5,2,3,  2,7,3,
			7,10,3, 7,6,10, 7,11,6, 11,0,6, 0,1,6,
			6,1,10, 9,0,11, 9,11,2, 9,2,5,  7,2,11,
		};

		return qgl::make_va_index(array, indices);
	}

	using Lookup = std::map<std::pair<qpl::u32, qpl::u32>, qpl::u32>;

	template<typename ... Attributes>
	constexpr unsigned vertex_for_edge(Lookup& lookup, std::vector<qgl::vertex<Attributes...>>& vertices, qpl::u32 first, qpl::u32 second) {
		Lookup::key_type key(first, second);

		if (key.first > key.second)
			std::swap(key.first, key.second);

		auto inserted = lookup.insert({ key, qpl::u32_cast(vertices.size()) });
		if (inserted.second) {
			auto edge0 = vertices[first].position;
			auto edge1 = vertices[second].position;
			auto point = (edge0 + edge1).normalized();
			
			vertices.push_back(point);
		}

		return inserted.first->second;
	}

	template<typename ... Attributes>
	constexpr std::vector<unsigned> subdivide(std::vector<qgl::vertex<Attributes...>>& vertices, const std::vector<qpl::u32>& indices) {
		Lookup lookup;
		std::vector<qpl::u32> result;

		for (qpl::size i = 0u; i < indices.size(); i += 3u) {
			std::array<unsigned, 3u> mid;
			for (qpl::size edge = 0u; edge < 3u; ++edge) {
				mid[edge] = vertex_for_edge(lookup, vertices, indices[i + edge], indices[i + ((edge + 1) % 3)]);
			}
			result.push_back(indices[i + 0]);
			result.push_back(mid[0]);
			result.push_back(mid[2]);
			result.push_back(indices[i + 1]);
			result.push_back(mid[1]);
			result.push_back(mid[0]);
			result.push_back(indices[i + 2]);
			result.push_back(mid[2]);
			result.push_back(mid[1]);
			result.push_back(mid[0]);
			result.push_back(mid[1]);
			result.push_back(mid[2]);
		}

		return result;
	}

	constexpr auto make_ico_sphere(qpl::size subdivisions) {
		auto sphere = get_ico_sphere();

		std::vector<qgl::vertex<qgl::pos3>> vertices;
		std::vector<qpl::u32> indices;

		vertices.resize(sphere.vertices.size());
		std::copy(sphere.vertices.begin(), sphere.vertices.end(), vertices.begin());
		indices.resize(sphere.indices.size());
		std::copy(sphere.indices.begin(), sphere.indices.end(), indices.begin());

		for (qpl::size i = 0u; i < subdivisions; ++i) {
			indices = subdivide(vertices, indices);
		}

		return qgl::va_index_vector_initialization{ vertices, indices };
	}

	struct cuboid {
		cuboid();

		qpl::vec3 position;
		qpl::vec3 dimension;
		qpl::rgb color;
		mutable bool changed = true;
		mutable qgl::va_index vertex_array;

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