#ifndef QPL_PICTURES_HPP
#define QPL_PICTURES_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <vector>
#include <array>
#include <string>

namespace qpl {
	struct pixel_rgba {
		qpl::u8 a;
		qpl::u8 b;
		qpl::u8 g;
		qpl::u8 r;

		explicit pixel_rgba() noexcept : r(0), g(0), b(0), a(qpl::u8_max) {

		}

		explicit pixel_rgba(qpl::u8 r, qpl::u8 g, qpl::u8 b, qpl::u8 a = qpl::u8_max) noexcept : r(r), g(g), b(b), a(a) {

		}

		constexpr static qpl::size bytes() {
			return qpl::size{ 4 };
		}
	};

	struct pixel_rgb {
		qpl::u8 b;
		qpl::u8 g;
		qpl::u8 r;


		explicit pixel_rgb() noexcept : r(0), g(0), b(0) {

		}
		explicit pixel_rgb(qpl::u8 r, qpl::u8 g, qpl::u8 b) noexcept : r(r), g(g), b(b) {

		}

		constexpr static qpl::size bytes() {
			return qpl::size{ 3 };
		}
	};

	namespace detail {
		constexpr auto file_header_size = 14;
		constexpr auto info_header_size = 40;

		QPLDLL std::array<char, info_header_size> create_bitmap_info_header(qpl::size width, qpl::size height);
		QPLDLL std::array<char, file_header_size> create_bitmap_file_header(qpl::size width, qpl::size height, qpl::size padding_size);
	}
	QPLDLL std::string generate_bmp_string(const std::vector<qpl::pixel_rgb>& image, qpl::size width, qpl::size height);
	QPLDLL void generate_bmp(const std::vector<qpl::pixel_rgb>& image, qpl::size width, qpl::size height, const std::string& filename);
}

#endif