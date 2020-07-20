#ifndef QPL_RGb_HPP
#define QPL_RGb_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <string>

#include <glm/glm.hpp>

namespace qpl {
	class frgb {
	public:
		constexpr frgb() noexcept : m_rgb(0, 0, 0) {

		}
		constexpr frgb(const frgb& other) noexcept : m_rgb(other.m_rgb) {

		}
		constexpr frgb(const glm::vec3& other) noexcept : m_rgb(other) {

		}
		constexpr frgb(qpl::f32 r, qpl::f32 g, qpl::f32 b) noexcept : m_rgb(r, g, b) {

		}
		QPLDLL frgb& operator=(const qpl::frgb& other);
		QPLDLL frgb& operator=(const glm::vec3& other);
		QPLDLL bool operator==(const frgb& other) const;
		QPLDLL bool operator==(const glm::vec3& other) const;

		QPLDLL operator glm::vec3() const;
		QPLDLL glm::vec3 get() const;

		QPLDLL std::string string() const;


		QPLDLL void brighten(qpl::f32 delta);
		QPLDLL qpl::frgb brightened(qpl::f32 delta) const;

		QPLDLL void darken(qpl::f32 delta);
		QPLDLL qpl::frgb darkened(qpl::f32 delta) const;

		QPLDLL void light(qpl::f32 delta);
		QPLDLL qpl::frgb lighted(qpl::f32 delta) const;

		QPLDLL void intensify(qpl::f32 delta);
		QPLDLL qpl::frgb intensified(qpl::f32 delta) const;

		QPLDLL void grayify(qpl::f32 delta);
		QPLDLL qpl::frgb grayified(qpl::f32 delta) const;

		QPLDLL void saturate(qpl::f32 delta);
		QPLDLL qpl::frgb saturated(qpl::f32 delta) const;


		QPLDLL void black_gray_intense_white(qpl::f32 delta);
		QPLDLL qpl::frgb black_gray_intense_whited(qpl::f32 delta) const;

		QPLDLL void black_intense_white(qpl::f32 delta);
		QPLDLL qpl::frgb black_intense_whited(qpl::f32 delta) const;

		QPLDLL void interpolate(qpl::frgb other, qpl::f32 delta);
		QPLDLL qpl::frgb interpolated(qpl::frgb other, qpl::f32 delta) const;

		const static qpl::frgb red;
		const static qpl::frgb green;
		const static qpl::frgb blue;
		const static qpl::frgb black;
		const static qpl::frgb white;
		const static qpl::frgb yellow;
		const static qpl::frgb magenta;
		const static qpl::frgb cyan;

		const static qpl::frgb orange;
		const static qpl::frgb purple;
		const static qpl::frgb amaranth;
		const static qpl::frgb turquoise;
	private:
		glm::vec3 m_rgb;
	};

	QPLDLL qpl::frgb random_color();
}
#endif