#ifndef QPL_WINSYS_HPP
#define QPL_WINSYS_HPP
#pragma once

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <qpl/qpldeclspec.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/pictures.hpp>
#include <string_view>
#include <span>
#include <string>

namespace qpl {
	namespace winsys {
		struct point {
			point(int x, int y) : x(x), y(y) {

			}
			point() {

			}

			int x = 0;
			int y = 0;
			QPLDLL std::string string();
			QPLDLL bool operator==(const point& other) const;
			QPLDLL bool operator!=(const point& other) const;
			QPLDLL point operator-(point point) const;
		};
		struct rect {
			rect() {

			}
			rect(int x1, int y1, int x2, int y2) {
				this->top_left.x = x1;
				this->top_left.y = y1;
				this->bottom_right.x = x2;
				this->bottom_right.y = y2;
			}
			rect(point top_left, point bottom_right) {
				this->top_left = top_left;
				this->bottom_right = bottom_right;
			}

			point top_left;
			point bottom_right;
			QPLDLL std::string string();
			QPLDLL bool unset() const;
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


		struct minitor_info {
			HMONITOR monitor;
			RECT     rect;

			minitor_info(HMONITOR monitor, RECT rect)
				: monitor(monitor)
				, rect(rect)
			{ }
		};

		namespace impl {
			QPLDLL BOOL CALLBACK monitor_enum_proc(HMONITOR hMonitor, HDC, RECT* prcMonitor, LPARAM dwData);
		}
		QPLDLL std::vector<minitor_info> get_info_for_all_monitors();




		struct monitor_capture {
			qpl::u32 index = 0u;
			bool initialized = false;
			qpl::winsys::rect rect;
			qpl::winsys::rect size;
			RECT rc_client;
			HDC hDesktopDC;
			HDC hCaptureDC;
			HBITMAP hCaptureBitmap;
			BITMAPINFOHEADER bmi;
			qpl::pixels pixels;

			~monitor_capture() {
				if (this->initialized) {
					DeleteObject(this->hDesktopDC);
					DeleteObject(this->hCaptureDC);
					DeleteObject(this->hCaptureBitmap);
				}
			}

			QPLDLL qpl::winsys::point relative_top_left_corner() const;
			QPLDLL bool needs_hdc_update() const;
			QPLDLL void set_rectangle(qpl::winsys::rect rect);
			QPLDLL void _update_hdc(HDC hdcMonitor, LPRECT lprcMonitor);
			QPLDLL void _init(HDC hdcMonitor, LPRECT lprcMonitor);
			QPLDLL void scan();
			QPLDLL void update();
			QPLDLL qpl::pixels scan_and_get_pixels();
			QPLDLL void scan_and_generate_bmp(std::string file_name);
			QPLDLL qpl::winsys::rect get_size();

			QPLDLL qpl::pixels get_pixels() const;
			QPLDLL void generate_bmp(std::string file_name) const;
		};

		namespace impl {
			extern process_list p_list;
			extern watch_list w_list;

			QPLDLL void set_process_information(process& proc, HWND hWnd);
			QPLDLL BOOL CALLBACK process_list_window_callback(HWND hWnd, LPARAM lparam);
			QPLDLL BOOL CALLBACK watch_list_window_callback(HWND hWnd, LPARAM lparam);
			QPLDLL BOOL CALLBACK capture_monitor_init_callback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
			QPLDLL BOOL CALLBACK capture_monitor_scan_callback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
			QPLDLL BOOL CALLBACK capture_monitor_scan_single_callback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);


			extern qpl::u32 update_monitor_index;
			extern bool looping_monitors;
			extern qpl::u32 loop_monitor_index;
			extern std::vector<monitor_capture> monitor_captures;
		}

		QPLDLL BOOL CALLBACK call_save_window_names(HWND hwnd, LPARAM lParam);
		QPLDLL std::vector<std::wstring> get_all_open_window_names();
		QPLDLL void print_all_open_window_names();
		QPLDLL bool find_window(std::string name);
		QPLDLL bool find_window(std::wstring name);
		QPLDLL void close_window(std::string name);
		QPLDLL void close_window(std::wstring name);
		QPLDLL bool set_window_position(std::string name, qpl::winsys::rect rectangle, bool on_top = true, bool show = true);
		QPLDLL bool set_window_position(std::wstring name, qpl::winsys::rect rectangle, bool on_top = true, bool show = true);
		QPLDLL qpl::winsys::rect get_window_rect(std::wstring name);
		QPLDLL qpl::winsys::rect get_window_rect(std::string name);

		QPLDLL process_list& get_process_list();

		QPLDLL void clear_watchlist();
		QPLDLL bool add_to_watchlist(std::wstring window_name);
		QPLDLL watch_list& get_watchlist();

		QPLDLL point get_mouse_position();
		QPLDLL bool mouse_left_clicked();

		QPLDLL void set_cursor_hand();
		QPLDLL void set_cursor_normal();

		QPLDLL qpl::pixels get_screen_pixels(qpl::winsys::rect rectangle);
		QPLDLL qpl::pixels get_screen_pixels();
		QPLDLL std::string get_screen_pixels_bmp_string();

		QPLDLL monitor_capture& get_capture_monitor(qpl::size index);
		QPLDLL void scan_monitor(qpl::u32 index);
		QPLDLL void init_monitor_captures();
		QPLDLL void scan_monitor_captures();
		QPLDLL void screen_shot_monitors();

		QPLDLL qpl::size monitor_capture_size();
		QPLDLL void enable_utf();
		QPLDLL void disable_utf();

		QPLDLL std::wstring read_utf_file(const std::wstring& path);
		QPLDLL std::wstring read_utf_file(const std::string& path);
	}
	QPLDLL void screen_shot(const std::string& file_name);
	QPLDLL void screen_shot(const std::string& file_name, qpl::winsys::rect rectangle);
	QPLDLL void screen_shot_stream(const std::string& file_name);


	QPLDLL qpl::winsys::point get_screen_dimension();
	QPLDLL void clear_console();

	QPLDLL void copy_to_clipboard(const std::string& s);
	QPLDLL void copy_to_clipboard(const std::wstring& s);


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


	QPLDLL std::ostream& operator<<(std::ostream& os, color color);
	QPLDLL std::ostream& operator<<(std::ostream& os, foreground foreground);
	QPLDLL std::ostream& operator<<(std::ostream& os, background background);

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

	struct shared_memory {
		HANDLE hMapFile;
		void* ptr = nullptr;
		bool array = false;

		~shared_memory() {
			this->destroy();
		}

		template <typename T>
		T* get() {
			return reinterpret_cast<T*>(this->ptr);
		}
		template <typename T>
		std::span<T> get_array() {
			auto p = reinterpret_cast<T*>(reinterpret_cast<char*>(this->ptr) + qpl::bytes_in_type<qpl::size>());
			auto size = this->array_byte_size() / sizeof(T);
			std::span<T> result(p, p + size);
			return result;
		}
		QPLDLL void* get();
		QPLDLL qpl::size array_byte_size() const;
		QPLDLL bool is_array() const;
		QPLDLL bool create(const std::string& name, qpl::size size);
		QPLDLL bool create_array(const std::string& name, qpl::size size);
		QPLDLL bool open(const std::string& name, qpl::size size);
		QPLDLL bool open_array(const std::string& name);
		QPLDLL void destroy();
	};

	namespace detail {
		extern std::unordered_map<std::string, shared_memory> shared_memories;
	}

	QPLDLL qpl::size get_shared_memory_array_byte_size(std::string name);
	QPLDLL void create_shared_memory_array(std::string name, qpl::size size);
	QPLDLL void open_shared_memory_array(std::string name);
	QPLDLL void create_shared_memory(std::string name, qpl::size size);
	QPLDLL void open_shared_memory(std::string name, qpl::size size);
	QPLDLL bool find_shared_memory(std::string name);
	template <typename T>
	T* create_shared_memory(std::string name) {
		qpl::create_shared_memory(name, sizeof(T));
		return qpl::detail::shared_memories[name].get<T>();
	}
	template <typename T>
	std::span<T> create_shared_memory_array(std::string name, qpl::size size) {
		qpl::create_shared_memory_array(name, sizeof(T) * size);
		return qpl::detail::shared_memories[name].get_array<T>();
	}
	template <typename T>
	T* get_shared_memory(std::string name) {
		if (qpl::detail::shared_memories.find(name) == qpl::detail::shared_memories.cend()) {
			if (qpl::find_shared_memory(name)){
				qpl::open_shared_memory(name, sizeof(T));
				return qpl::detail::shared_memories[name].get<T>();
			}
			else {
				return qpl::create_shared_memory<T>(name);
			}
		}
		return qpl::detail::shared_memories[name].get<T>();
	}

	template <typename T>
	std::span<T> get_shared_memory_array(std::string name) {
		if (qpl::detail::shared_memories.find(name) == qpl::detail::shared_memories.cend()) {
			if (qpl::find_shared_memory(name)) {
				qpl::open_shared_memory_array(name);
			}
			else {
				return {};
			}
		}
		return qpl::detail::shared_memories[name].get_array<T>();
	}
}


#endif
#endif