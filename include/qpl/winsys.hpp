#ifndef QPL_WINSYS_HPP
#define QPL_WINSYS_HPP
#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <vector>
#include <string_view>

#include <qpl/qpldeclspec.hpp>
#include <qpl/pictures.hpp>

namespace qpl {
	namespace winsys {
		struct point {
			int x = 0;
			int y = 0;
			QPLDLL std::string string();
			QPLDLL bool operator==(const point& other) const;
			QPLDLL bool operator!=(const point& other) const;
			QPLDLL point operator-(point point) const;
		};
		struct rect {
			point top_left;
			point bottom_right;
			QPLDLL std::string string();
			QPLDLL int width() const;
			QPLDLL int height() const;
			QPLDLL bool operator==(const rect& other) const;
			QPLDLL bool operator!=(const rect& other) const;
			QPLDLL bool contains(point point) const;
		};

		struct process {
			std::wstring process_name;
			std::wstring window_name;
			bool has_window = false;
			bool is_visible = false;
			bool is_on_screen = false;
			DWORD process_ID = 0;
			rect window_dimension;

			HWND hwnd = nullptr;
		};
		using process_list = std::vector<process>;
		using watch_list = std::vector<process>;

		namespace impl {
			extern process_list p_list;
			extern watch_list w_list;

			QPLDLL void set_process_information(process& proc, HWND hWnd);
			QPLDLL BOOL process_list_window_callback(HWND hWnd, LPARAM lparam);
			QPLDLL BOOL watch_list_window_callback(HWND hWnd, LPARAM lparam);
		}
		QPLDLL process_list& get_process_list();

		QPLDLL void clear_watchlist();
		QPLDLL bool add_to_watchlist(std::wstring window_name);
		QPLDLL watch_list& get_watchlist();

		QPLDLL point get_mouse_position();
		QPLDLL bool mouse_left_clicked();

		QPLDLL std::vector<qpl::pixel_rgb> get_screen_pixels(qpl::winsys::rect rectangle);
		QPLDLL std::vector<qpl::pixel_rgb> get_screen_pixels();
	}
	QPLDLL void screen_shot(const std::string& file_name);
	QPLDLL qpl::winsys::point get_screen_dimension();
	QPLDLL void clear_console();

	enum class color : unsigned {
		black = 0,
		blue = 1,
		green = 2,
		aqua = 3,
		red = 4,
		purple = 5,
		yellow = 6,
		white = 7,
		gray = 8,
		light_blue = 9,
		light_green = 10,
		light_aqua = 11,
		light_red = 12,
		light_purple = 13,
		light_yellow = 14,
		bright_white = 15,
		size = 16,
	};
	enum class foreground : unsigned {
		black = 0,
		blue = 1,
		green = 2,
		aqua = 3,
		red = 4,
		purple = 5,
		yellow = 6,
		white = 7,
		gray = 8,
		light_blue = 9,
		light_green = 10,
		light_aqua = 11,
		light_red = 12,
		light_purple = 13,
		light_yellow = 14,
		bright_white = 15,
		size = 16,
	};
	enum class background : unsigned {
		black = 0,
		blue = 1,
		green = 2,
		aqua = 3,
		red = 4,
		purple = 5,
		yellow = 6,
		white = 7,
		gray = 8,
		light_blue = 9,
		light_green = 10,
		light_aqua = 11,
		light_red = 12,
		light_purple = 13,
		light_yellow = 14,
		bright_white = 15,
		size = 16,
	};

	struct cc {
		cc() {
			this->foreground = qpl::foreground::white;
			this->background = qpl::background::black;
		}
		cc(const cc& other) {
			*this = other;
		}
		cc(qpl::color foreground, qpl::color background) {
			this->foreground = static_cast<qpl::foreground>(foreground);
			this->background = static_cast<qpl::background>(background);
		}
		cc(qpl::color foreground) {
			this->foreground = static_cast<qpl::foreground>(foreground);
			this->background = qpl::background::black;
		}
		cc(qpl::foreground foreground, qpl::background background) {
			this->foreground = foreground;
			this->background = background;
		}
		cc(qpl::background background, qpl::foreground foreground) {
			this->foreground = foreground;
			this->background = background;
		}
		cc(qpl::foreground foreground) {
			this->foreground = foreground;
			this->background = qpl::background::black;
		}
		cc(qpl::background background) {
			this->foreground = qpl::foreground::white;
			this->background = background;
		}
		cc& operator=(const cc& other) {
			this->foreground = other.foreground;
			this->background = other.background;
			return *this;
		}
		bool operator==(const cc& other) const {
			return this->foreground == other.foreground && this->background == other.background;
		}

		QPLDLL static qpl::cc def();
		QPLDLL static qpl::cc random();
		QPLDLL static qpl::cc random_foreground();
		QPLDLL static qpl::cc random_background();

		qpl::foreground foreground;
		qpl::background background;
	};

	QPLDLL qpl::cc get_random_console_color();
	QPLDLL qpl::foreground get_random_foreground();
	QPLDLL qpl::background get_random_background();
	QPLDLL void set_console_color(qpl::color foreground);
	QPLDLL void set_console_color(qpl::color foreground, qpl::color background);
	QPLDLL void set_console_color(qpl::foreground foreground, qpl::background background);
	QPLDLL void set_console_color(qpl::background background, qpl::foreground foreground);
	QPLDLL void set_console_color(qpl::foreground foreground);
	QPLDLL void set_console_color(qpl::background background);
	QPLDLL void set_console_color(qpl::cc color);
	QPLDLL void set_console_color_default();
	QPLDLL void copy_to_clipboard(const std::string& string);

}

#endif