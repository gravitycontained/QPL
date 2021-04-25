#ifndef QPL_PICTURES_HPP
#define QPL_PICTURES_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/span.hpp>
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

		constexpr static qpl::size pixel_bytes() {
			return qpl::size{ 3 };
		}
		QPLDLL std::string string() const;
	};

	struct pixels {
		QPLDLL void load(qpl::string_view sv);
		QPLDLL void load_bmp(qpl::string_view sv);
		QPLDLL void set_dimension(qpl::u32 width, qpl::u32 height);
		QPLDLL pixels rectangle(qpl::u32 x, qpl::u32 y, qpl::u32 width, qpl::u32 height) const;
		qpl::size size() const;

		QPLDLL std::string generate_bmp_string() const;
		QPLDLL void generate_bmp(std::string filename) const;
		
		std::vector<pixel_rgb> data;
		qpl::u32 width = 0u;
		qpl::u32 height = 0u;
	};

	namespace detail {
		constexpr auto file_header_size = 14;
		constexpr auto info_header_size = 40;

		QPLDLL std::array<char, info_header_size> create_bitmap_info_header(qpl::size width, qpl::size height);
		QPLDLL std::array<char, file_header_size> create_bitmap_file_header(qpl::size width, qpl::size height, qpl::size padding_size);
	}
}

#endif