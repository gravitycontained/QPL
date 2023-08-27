
#include <qpl/winsys.hpp>


#ifdef _WIN32
#include <qpl/random.hpp>
#include <qpl/string.hpp>
#include <qpl/codec.hpp>

#include <iostream>
#include <exception>
#include <sstream>
#include <regex>

#include <TlHelp32.h>
#include <Lmcons.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>

#include <io.h>
#include <fcntl.h>

#include <qpl/filesys.hpp>

#pragma comment(lib, "psapi")

namespace qpl {
	namespace winsys {
		std::string qpl::winsys::point::string() {
			std::ostringstream stream;
			stream << "(" << this->x << ", " << this->y << ")";
			return stream.str();
		}
		bool qpl::winsys::point::operator==(const point& other) const {
			return this->x == other.x && this->y == other.y;
		}
		bool qpl::winsys::point::operator!=(const point& other) const {
			return !(*this == other);
		}
		point qpl::winsys::point::operator-(point point) const {
			qpl::winsys::point result = *this;
			result.x -= point.x;
			result.y -= point.y;
			return result;
		}

		std::string qpl::winsys::rect::string() {
			std::ostringstream stream;
			stream << this->top_left.string() << ", " << this->bottom_right.string();
			return stream.str();
		}
		bool qpl::winsys::rect::unset() const {
			return this->top_left.x == 0 && this->top_left.y == 0 && this->bottom_right.x == 0 && this->bottom_right.y == 0;
		}
		int qpl::winsys::rect::width() const {
			return this->bottom_right.x - this->top_left.x;
		}
		int qpl::winsys::rect::height() const {
			return this->bottom_right.y - this->top_left.y;
		}
		bool qpl::winsys::rect::operator==(const rect& other) const {
			return this->top_left == other.top_left && this->bottom_right == other.bottom_right;
		}
		bool qpl::winsys::rect::operator!=(const rect& other) const {
			return !(*this == other);
		}
		bool qpl::winsys::rect::contains(point point) const {
			return point.x > this->top_left.x && point.x < this->bottom_right.x&& point.y > this->top_left.y && point.y < this->bottom_right.y;
		}


		BOOL CALLBACK qpl::winsys::impl::monitor_enum_proc(HMONITOR hMonitor, HDC, RECT* prcMonitor, LPARAM dwData) {
			//ASSERT(hMonitor != NULL);
			//ASSERT(prcMonitor != nullptr);
			//ASSERT(dwData != NULL);

			MONITORINFO mi;
			mi.cbSize = sizeof(mi);
			GetMonitorInfo(hMonitor, &mi);

			if ((mi.dwFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) == DISPLAY_DEVICE_MIRRORING_DRIVER) {
				// Skip mirroring drivers that do not correspond to display screens.
				return TRUE;
			}
			else {
				auto* const pInfo = reinterpret_cast<std::vector<minitor_info>*>(dwData);
				//ASSERT(pInfo != nullptr);

				pInfo->emplace_back(hMonitor, *prcMonitor);

				return TRUE;
			}
		}

		std::vector<minitor_info> qpl::winsys::get_info_for_all_monitors() {
			const int cMonitors = ::GetSystemMetrics(SM_CMONITORS);

			std::vector<minitor_info> result;
			result.reserve(cMonitors);

			::EnumDisplayMonitors(nullptr,
				nullptr,
				impl::monitor_enum_proc,
				reinterpret_cast<LPARAM>(&result));

			return result;
		}


		void qpl::winsys::monitor_capture::_init(HDC hdcMonitor, LPRECT lprcMonitor) {
			if (this->rect.unset()) {
				this->rect.bottom_right.x = lprcMonitor->right - lprcMonitor->left;
				this->rect.bottom_right.y = lprcMonitor->bottom - lprcMonitor->top;
			}
			this->_update_hdc(hdcMonitor, lprcMonitor);

			this->pixels.set_dimension(this->rect.width(), this->rect.height());

			this->bmi.biSize = sizeof(BITMAPINFOHEADER);
			this->bmi.biWidth = this->rect.width();
			this->bmi.biHeight = this->rect.height();
			this->bmi.biPlanes = 1;
			this->bmi.biBitCount = 24;
			this->bmi.biCompression = BI_RGB;
			this->bmi.biSizeImage = 0;
			this->bmi.biXPelsPerMeter = 0;
			this->bmi.biYPelsPerMeter = 0;
			this->bmi.biClrUsed = 0;
			this->bmi.biClrImportant = 0;


			this->initialized = true;
		}

		qpl::winsys::point qpl::winsys::monitor_capture::relative_top_left_corner() const {
			return qpl::winsys::point(this->rc_client.left, this->rc_client.top);
		}
		bool qpl::winsys::monitor_capture::needs_hdc_update() const {
			return !qpl::winsys::impl::looping_monitors;
		}
		void qpl::winsys::monitor_capture::set_rectangle(qpl::winsys::rect rect) {
			this->rect = rect;
			this->hCaptureBitmap = CreateCompatibleBitmap(this->hDesktopDC, this->rect.width(), this->rect.height());
			this->pixels.set_dimension(this->rect.width(), this->rect.height());
			this->bmi.biWidth = this->rect.width();
			this->bmi.biHeight = this->rect.height();
		}
		void qpl::winsys::monitor_capture::_update_hdc(HDC hdcMonitor, LPRECT lprcMonitor) {
			bool new_monitor = this->hDesktopDC != hdcMonitor;
			if (new_monitor || !this->hDesktopDC) {
				if (this->hDesktopDC) {
					DeleteObject(this->hDesktopDC);
				}
				DeleteObject(this->hCaptureDC);
				DeleteObject(this->hCaptureBitmap);
				this->hDesktopDC = hdcMonitor;
				this->rc_client = *lprcMonitor;
				this->hCaptureDC = CreateCompatibleDC(this->hDesktopDC);
				this->hCaptureBitmap = CreateCompatibleBitmap(this->hDesktopDC, this->rect.width(), this->rect.height());
			}
		}
		void qpl::winsys::monitor_capture::update() {
			qpl::winsys::scan_monitor(this->index);
		}
		void qpl::winsys::monitor_capture::scan() {
			if (!this->initialized) {
				throw std::exception("qpl::winsys::screen_pixels_stream was not initialized");
			}

			//if (this->needs_hdc_update()) {
			//	this->update();
			//}
			this->update();
			
			SelectObject(this->hCaptureDC, this->hCaptureBitmap);

			BitBlt(this->hCaptureDC, 0, 0, this->rect.width(), this->rect.height(), this->hDesktopDC, this->rc_client.left + this->rect.top_left.x, this->rc_client.top + this->rect.top_left.y, SRCCOPY);

			GetDIBits(this->hCaptureDC, this->hCaptureBitmap, 0, this->rect.height(), this->pixels.data.data(), (BITMAPINFO*)&this->bmi, DIB_RGB_COLORS);

		}
		qpl::pixels qpl::winsys::monitor_capture::scan_and_get_pixels() {
			this->scan();
			return this->pixels;
		}
		void qpl::winsys::monitor_capture::scan_and_generate_bmp(std::string file_name) {
			this->scan();
			this->generate_bmp(file_name);
		}
		qpl::winsys::rect qpl::winsys::monitor_capture::get_size() {
			if (this->size.unset()) {
				auto rect = qpl::winsys::get_info_for_all_monitors()[this->index].rect;
				this->size = qpl::winsys::rect(rect.left, rect.top, rect.right, rect.bottom);
			}
			return this->size;
		}
		qpl::pixels qpl::winsys::monitor_capture::get_pixels() const {
			return this->pixels;
		}
		void qpl::winsys::monitor_capture::generate_bmp(std::string file_name) const {
			this->pixels.generate_bmp(file_name);
		}

		process_list qpl::winsys::impl::p_list;
		watch_list qpl::winsys::impl::w_list;

		void qpl::winsys::impl::set_process_information(process& proc, HWND hWnd) {
			auto check = [](DWORD code, int n) {
				return;
				if (code == 0) {
					auto error_id = GetLastError();

					LPSTR messageBuffer = nullptr;
					size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, error_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

					std::string message(messageBuffer, size);

					//Free the buffer.
					LocalFree(messageBuffer);
					//std::cerr << "[" << n << "] failed " << message << '\n';
				}
			};

			proc.hwnd = hWnd;


			DWORD process_ID;
			proc.process_ID = GetWindowThreadProcessId(hWnd, &process_ID);

			HANDLE handle = OpenProcess(
				PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
				FALSE,
				process_ID
			);


			WCHAR tbuffer[MAX_PATH];
			check(GetModuleFileNameEx(handle, 0, tbuffer, MAX_PATH), 1);
			proc.process_name = tbuffer;

			const std::wregex wreg{ L".*\\\\(.*)" };
			std::wsmatch smatch;
			std::regex_match(proc.process_name, smatch, wreg);
			if (!smatch.str(1).empty()) {
				proc.process_name = smatch.str(1);
			}
			proc.process_name = qpl::string_to_wstring(qpl::wstring_to_string(proc.process_name));


			tagRECT lrect;
			GetWindowRect(hWnd, &lrect);
			proc.window_dimension.top_left = { lrect.left, lrect.top };
			proc.window_dimension.bottom_right = { lrect.right, lrect.bottom };

			proc.has_window = !(lrect.left == 0 && lrect.right == 0 && lrect.top == 0 && lrect.bottom == 0);
			proc.is_visible = IsWindowVisible(hWnd);
			proc.is_on_screen = proc.is_visible && !IsIconic(hWnd);

			int length = GetWindowTextLength(hWnd);
			WCHAR* wbuffer = new WCHAR[length + 1ull];
			check(GetWindowText(hWnd, wbuffer, length + 1), 0);
			proc.window_name = wbuffer;
			delete[] wbuffer;
		}
		BOOL CALLBACK qpl::winsys::impl::process_list_window_callback(HWND hWnd, LPARAM lparam) {
			qpl::winsys::impl::p_list.push_back(process{});
			qpl::winsys::impl::set_process_information(qpl::winsys::impl::p_list.back(), hWnd);
			return true;
		}
		BOOL CALLBACK qpl::winsys::impl::watch_list_window_callback(HWND hWnd, LPARAM lparam) {
			for (unsigned i = 0u; i < qpl::winsys::impl::w_list.size(); ++i) {
				if (qpl::winsys::impl::w_list[i].hwnd == hWnd) {
					qpl::winsys::impl::set_process_information(qpl::winsys::impl::w_list[i], hWnd);
					return true;
				}
			}
			return true;
		}
		BOOL CALLBACK qpl::winsys::impl::capture_monitor_init_callback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
			if (qpl::winsys::impl::monitor_captures.size() <= qpl::winsys::impl::loop_monitor_index) {
				qpl::winsys::impl::monitor_captures.push_back({});
			}
			if (!qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index].initialized) {
				qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index]._init(hdcMonitor, lprcMonitor);
			}
			qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index].index = qpl::winsys::impl::loop_monitor_index;
			
			++qpl::winsys::impl::loop_monitor_index;
			return true;
		}
		BOOL CALLBACK qpl::winsys::impl::capture_monitor_scan_callback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
			if (qpl::winsys::impl::monitor_captures.size() <= qpl::winsys::impl::loop_monitor_index) {
				qpl::winsys::impl::monitor_captures.push_back({});
			}
			if (!qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index].initialized) {
				qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index]._init(hdcMonitor, lprcMonitor);
			}
			qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index].index = qpl::winsys::impl::loop_monitor_index;
			qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index]._update_hdc(hdcMonitor, lprcMonitor);
			qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index].scan();


			++qpl::winsys::impl::loop_monitor_index;
			return true;
		}
		BOOL CALLBACK qpl::winsys::impl::capture_monitor_scan_single_callback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
			if (qpl::winsys::impl::loop_monitor_index == qpl::winsys::impl::update_monitor_index) {

				if (qpl::winsys::impl::monitor_captures.size() <= qpl::winsys::impl::loop_monitor_index) {
					qpl::winsys::impl::monitor_captures.push_back({});
				}
				if (!qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index].initialized) {
					qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index]._init(hdcMonitor, lprcMonitor);
				}
				qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index].index = qpl::winsys::impl::loop_monitor_index;
				qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index]._update_hdc(hdcMonitor, lprcMonitor);
				qpl::winsys::impl::monitor_captures[qpl::winsys::impl::loop_monitor_index].scan();
			}

			++qpl::winsys::impl::loop_monitor_index;
			return true;
		}


		qpl::u32 qpl::winsys::impl::update_monitor_index = 0u;
		qpl::u32 qpl::winsys::impl::loop_monitor_index = 0u;
		bool qpl::winsys::impl::looping_monitors = false;
		std::vector<monitor_capture> qpl::winsys::impl::monitor_captures;


		BOOL CALLBACK call_save_window_names(HWND hwnd, LPARAM lParam) {
			const DWORD TITLE_SIZE = 1024;
			WCHAR windowTitle[TITLE_SIZE];

			GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

			int length = ::GetWindowTextLength(hwnd);
			std::wstring title(&windowTitle[0]);
			if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") {
				return TRUE;
			}

			// Retrieve the pointer passed into this callback, and re-'type' it.
			// The only way for a C API to pass arbitrary data is by means of a void*.
			std::vector<std::wstring>& titles =
				*reinterpret_cast<std::vector<std::wstring>*>(lParam);
			titles.push_back(title);

			return TRUE;
		}

		std::vector<std::wstring> qpl::winsys::get_all_open_window_names() {
			std::vector<std::wstring> titles;
			EnumWindows(call_save_window_names, reinterpret_cast<LPARAM>(&titles));
			return titles;
		}
		void qpl::winsys::print_all_open_window_names() {
			auto titles = qpl::winsys::get_all_open_window_names();
			qpl::println(titles.size(), " windows open --- ");
			for (auto& i : titles) {
				qpl::println("\"", i, "\"");
			}
			qpl::println("--------");
		}
		bool qpl::winsys::find_window(std::string name) {
			return qpl::winsys::find_window(qpl::string_to_wstring(name));
		}
		bool qpl::winsys::find_window(std::wstring name) {
			HWND handle = FindWindow(NULL, name.c_str());

			if (!IsWindow(handle)) {
				return false;
			}
			return true;
		}
		void qpl::winsys::close_window(std::string name) {
			qpl::winsys::close_window(qpl::string_to_wstring(name));
		}
		void qpl::winsys::close_window(std::wstring name) {
			HWND handle = FindWindow(NULL, name.c_str());

			if (!IsWindow(handle)) {
				return;
			}

			CloseWindow(handle);
		}
		bool qpl::winsys::set_window_position(std::string name, qpl::winsys::rect rectangle, bool on_top, bool show) {
			return qpl::winsys::set_window_position(qpl::string_to_wstring(name), rectangle, on_top, show);
		}
		bool qpl::winsys::set_window_position(std::wstring name, qpl::winsys::rect rectangle, bool on_top, bool show) {

			HWND handle = FindWindow(NULL, name.c_str());
			if (!IsWindow(handle)) {
				qpl::println("qpl::winsys::set_window_position: couldn't find \"", name, "\"");
				return false;
			}

			auto result = SetWindowPos(handle, on_top ? HWND_TOPMOST : 0, rectangle.top_left.x, rectangle.top_left.y, rectangle.width(), rectangle.height(), 0);
			if (!result) {
				qpl::println("SetWindowPos failed");
			}

			if (show) {
				ShowWindow(handle, SW_RESTORE);
			}

			return true;
		}
		qpl::winsys::rect qpl::winsys::get_window_rect(std::wstring name) {

			HWND handle = FindWindow(NULL, name.c_str());
			if (!IsWindow(handle)) {
				qpl::println("qpl::winsys::get_window_rect: couldn't find \"", name, "\"");
				return {};
			}

			RECT rect;
			GetWindowRect(handle, &rect);

			return qpl::winsys::rect(rect.left, rect.top, rect.right, rect.bottom);
		}
		qpl::winsys::rect qpl::winsys::get_window_rect(std::string name) {
			return qpl::winsys::get_window_rect(qpl::string_to_wstring(name));
		}

		process_list& qpl::winsys::get_process_list() {
			qpl::winsys::impl::p_list.clear();
			EnumWindows(qpl::winsys::impl::process_list_window_callback, NULL);
			return qpl::winsys::impl::p_list;
		}
		void qpl::winsys::clear_watchlist() {
			qpl::winsys::impl::w_list.clear();
		}
		bool qpl::winsys::add_to_watchlist(std::wstring window_name) {
			bool found = false;
			for (auto i : qpl::winsys::impl::p_list) {
				if (i.window_name == window_name) {
					qpl::winsys::impl::w_list.push_back(i);
					found = true;
				}
			}
			return found;
		}
		watch_list& qpl::winsys::get_watchlist() {
			EnumWindows(qpl::winsys::impl::watch_list_window_callback, NULL);
			return qpl::winsys::impl::w_list;
		}

		qpl::vec2i qpl::winsys::get_mouse_position() {
			POINT p;
			GetCursorPos(&p);
			return qpl::vec2i{ p.x, p.y };
		}
		bool qpl::winsys::mouse_left_clicked() {
			static bool before = false;
			auto down = (GetKeyState(VK_LBUTTON) & 0x80) != 0;
			if (down && !before) {
				before = down;
				return true;
			}
			else {
				before = down;
				return false;
			}
		}
		std::unordered_map<int, bool> qpl::winsys::impl::key_map;

		bool qpl::winsys::key_pressed(int key) {
			auto holding_before = qpl::winsys::impl::key_map.find(key) != qpl::winsys::impl::key_map.cend();
			if (holding_before) {
				holding_before = qpl::winsys::impl::key_map[key];
			}

			auto keyState = GetKeyState(key);
			bool toggled = keyState & 1;
			bool down = keyState & 0x8000;

			qpl::winsys::impl::key_map[key] = down;
			return down && !holding_before;
		}
		bool qpl::winsys::key_released(int key) {
			auto holding_before = qpl::winsys::impl::key_map.find(key) != qpl::winsys::impl::key_map.cend();
			if (holding_before) {
				holding_before = qpl::winsys::impl::key_map[key];
			}

			auto keyState = GetKeyState(key);
			bool toggled = keyState & 1;
			bool down = keyState & 0x8000;

			qpl::winsys::impl::key_map[key] = down;
			return !down && holding_before;
		}
		bool qpl::winsys::key_holding(int key) {
			auto keyState = GetKeyState(key);
			bool down = keyState & 0x8000;
			qpl::winsys::impl::key_map[key] = down;
			return down;
		}
		void qpl::winsys::click_left_mouse(qpl::vec2 pos) {
			INPUT Inputs[3] = { 0 };
		
			Inputs[0].type = INPUT_MOUSE;
			Inputs[0].mi.dx = static_cast<LONG>(pos.x);
			Inputs[0].mi.dy = static_cast<LONG>(pos.y);
			Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		
			Inputs[1].type = INPUT_MOUSE;
			Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		
			Inputs[2].type = INPUT_MOUSE;
			Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
		
			SendInput(3, Inputs, sizeof(INPUT));
		}
		void qpl::winsys::click_left_mouse() {
			INPUT Input = { 0 };
			// left down 
			Input.type = INPUT_MOUSE;
			Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &Input, sizeof(INPUT));

			// left up
			ZeroMemory(&Input, sizeof(INPUT));
			Input.type = INPUT_MOUSE;
			Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &Input, sizeof(INPUT));
		}
		void qpl::winsys::set_cursor_hand() {
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
		void qpl::winsys::set_cursor_normal() {
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
		void qpl::winsys::hide_console() {
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		}

		void qpl::winsys::show_console() {
			ShowWindow(GetConsoleWindow(), SW_SHOW);
		}
		void qpl::winsys::set_program_launch_on_startup(std::wstring program_path) {
			auto name = qpl::string_split(program_path, L'/').back();
			name = qpl::string_split(name, L'.').front();

			program_path = qpl::string_replace_all(program_path, L"/", L"\\");

			HKEY hkey = NULL;
			LONG create_status = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey); 
			LONG status = RegSetValueEx(hkey, name.c_str(), 0, REG_SZ, (BYTE*)program_path.c_str(), static_cast<DWORD>((program_path.size() + 1) * sizeof(wchar_t)));
		}
		void qpl::winsys::set_program_launch_on_startup(std::string program_path) {
			qpl::winsys::set_program_launch_on_startup(qpl::string_to_wstring(program_path));
		}


		qpl::pixels qpl::winsys::get_screen_pixels(qpl::winsys::rect rectangle) {
			HWND hDesktopWnd = GetDesktopWindow();
			HDC hDesktopDC = GetDC(hDesktopWnd);
			HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
			HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, rectangle.width(), rectangle.height());
			SelectObject(hCaptureDC, hCaptureBitmap);

			BitBlt(hCaptureDC, 0, 0, rectangle.width(), rectangle.height(), hDesktopDC, rectangle.top_left.x, rectangle.top_left.y, SRCCOPY | CAPTUREBLT);


			BITMAPINFOHEADER bi;

			bi.biSize = sizeof(BITMAPINFOHEADER);
			bi.biWidth = rectangle.width();
			bi.biHeight = rectangle.height();
			bi.biPlanes = 1;
			bi.biBitCount = 24;
			bi.biCompression = BI_RGB;
			bi.biSizeImage = 0;
			bi.biXPelsPerMeter = 0;
			bi.biYPelsPerMeter = 0;
			bi.biClrUsed = 0;
			bi.biClrImportant = 0;

			qpl::pixels pixels;
			pixels.set_dimension(rectangle.width(), rectangle.height());

			GetDIBits(hCaptureDC, hCaptureBitmap, 0, rectangle.height(), pixels.data.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);


			DeleteObject(hDesktopDC);
			DeleteObject(hCaptureDC);

			return pixels;
		}
		qpl::pixels qpl::winsys::get_screen_pixels() {
			return qpl::winsys::get_screen_pixels(qpl::winsys::rect{ {0, 0}, qpl::get_screen_dimension() });
		}
		std::string qpl::winsys::get_screen_pixels_bmp_string() {
			auto pixels = qpl::winsys::get_screen_pixels();
			return pixels.generate_bmp_string();
		}

		monitor_capture& qpl::winsys::get_capture_monitor(qpl::size index) {
			return qpl::winsys::impl::monitor_captures[index];
		}
		void qpl::winsys::scan_monitor(qpl::u32 index) {
			qpl::winsys::impl::loop_monitor_index = 0u;
			qpl::winsys::impl::looping_monitors = true;
			qpl::winsys::impl::update_monitor_index = index;
			HDC hdc = GetDC(NULL);
			EnumDisplayMonitors(hdc, NULL, qpl::winsys::impl::capture_monitor_scan_single_callback, 0);
			ReleaseDC(NULL, hdc);
			qpl::winsys::impl::looping_monitors = false;
		}
		void qpl::winsys::init_monitor_captures() {
			qpl::winsys::impl::loop_monitor_index = 0u;
			qpl::winsys::impl::looping_monitors = true;
			HDC hdc = GetDC(NULL);
			EnumDisplayMonitors(hdc, NULL, qpl::winsys::impl::capture_monitor_init_callback, 0);
			ReleaseDC(NULL, hdc);
			qpl::winsys::impl::looping_monitors = false;
		}
		void qpl::winsys::scan_monitor_captures() {
			qpl::winsys::impl::loop_monitor_index = 0u;
			qpl::winsys::impl::looping_monitors = true;
			HDC hdc = GetDC(NULL);
			EnumDisplayMonitors(hdc, NULL, qpl::winsys::impl::capture_monitor_scan_callback, 0);
			ReleaseDC(NULL, hdc);
			qpl::winsys::impl::looping_monitors = false;


		}
		void qpl::winsys::screen_shot_monitors() {
			qpl::winsys::impl::loop_monitor_index = 0u;
			qpl::winsys::impl::looping_monitors = true;
			HDC hdc = GetDC(NULL);
			EnumDisplayMonitors(hdc, NULL, qpl::winsys::impl::capture_monitor_scan_callback, 0);
			ReleaseDC(NULL, hdc);

			for (auto& i : qpl::winsys::impl::monitor_captures) {
				i.pixels.generate_bmp(qpl::get_current_time_string_ms() + ".bmp");
			}
			qpl::winsys::impl::looping_monitors = false;
		}
		qpl::size qpl::winsys::monitor_capture_size() {
			return qpl::winsys::impl::monitor_captures.size();
		}
	}
	qpl::winsys::point qpl::get_screen_dimension() {
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		return { desktop.right, desktop.bottom };
	}
	std::wstring qpl::winsys::get_user_name() {
		wchar_t username[UNLEN + 1];
		DWORD username_len = UNLEN + 1;
		GetUserName(username, &username_len);
		return std::wstring{ username };
	}
	void qpl::screen_shot(const std::string& file_name) {
		auto screen = qpl::winsys::get_screen_pixels();
		screen.generate_bmp(file_name);
	}
	void qpl::screen_shot(const std::string& file_name, qpl::winsys::rect rectangle) {
		auto screen = qpl::winsys::get_screen_pixels(rectangle);
		screen.generate_bmp(file_name);
	}
	void qpl::clear_console() {
		system("cls");
	}

	void qpl::copy_to_clipboard(const std::string& string) {
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, string.size() + 1);
		memcpy(GlobalLock(hMem), string.data(), string.size() + 1);
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}
	void qpl::copy_to_clipboard(const std::wstring& string) {
		auto size = (string.size() + 1) * sizeof(wchar_t);

		HGLOBAL hClipboardData;
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, size);
		WCHAR* pchData;
		pchData = (WCHAR*)GlobalLock(hClipboardData);
		wcscpy_s(pchData, string.size() + 1, string.data());
		GlobalUnlock(hClipboardData);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hClipboardData);
		CloseClipboard();
	}
	void qpl::winsys::enable_utf16() {
		_setmode(_fileno(stdout), _O_U16TEXT);
		qpl::detail::utf_enabled = true;
	}
	void qpl::winsys::enable_utf() {
		fflush(stdout);
#if defined _MSC_VER
#   pragma region WIN_UNICODE_SUPPORT_MAIN
#endif
#if defined _WIN32
		// change code page to UTF-8 UNICODE
		if (!IsValidCodePage(CP_UTF8)) {
			throw qpl::exception("qpl::winsys::enable_utf(): error: ", GetLastError());
		}
		if (!SetConsoleCP(CP_UTF8)) {
			throw qpl::exception("qpl::winsys::enable_utf(): error: ", GetLastError());
		}
		if (!SetConsoleOutputCP(CP_UTF8)) {
			throw qpl::exception("qpl::winsys::enable_utf(): error: ", GetLastError());
		}

		// change console font - post Windows Vista only
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_FONT_INFOEX cfie;
		const auto sz = sizeof(CONSOLE_FONT_INFOEX);
		ZeroMemory(&cfie, sz);
		cfie.cbSize = sz;
		cfie.dwFontSize.Y = 14;
		wcscpy_s(cfie.FaceName,
			L"Lucida Console");
		SetCurrentConsoleFontEx(hStdOut,
			false,
			&cfie);

		// change file stream translation mode
		_setmode(_fileno(stdout), _O_U16TEXT);
		_setmode(_fileno(stderr), _O_U16TEXT);
		_setmode(_fileno(stdin), _O_U16TEXT);
#endif
#if defined _MSC_VER
#   pragma endregion
#endif
		std::ios_base::sync_with_stdio(false);
		qpl::detail::utf_enabled = true;
	}
	void qpl::winsys::disable_utf() {
		qpl::detail::utf_enabled = false;
	}
	std::wstring qpl::winsys::read_utf8_file(const std::string& file) {
		return qpl::winsys::read_utf8_file(qpl::utf8_to_wstring(file));
	}
	std::wstring qpl::winsys::read_utf8_file(const std::wstring& file) {
		std::wstring buffer;
		FILE* f;
		auto error = _wfopen_s(&f, file.c_str(), L"rtS, ccs=UTF-8");
		if (error) {
			throw qpl::exception("qpl::winsys::read_utf8_file(): can't open / find file \"", file, "\"");
		}

		if (f == NULL) {
			throw qpl::exception("qpl::winsys::read_utf8_file(): can't open / find file \"", file, "\"");
		}
		struct _stat fileinfo;
		_wstat(file.c_str(), &fileinfo);
		size_t filesize = fileinfo.st_size;

		if (filesize > 0) {
			buffer.resize(filesize);
			size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
			buffer.resize(wchars_read);
			buffer.shrink_to_fit();
		}

		fclose(f);

		return buffer;
	}

	std::string qpl::winsys::read_file(const std::string& path) {
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			throw qpl::exception("qpl::winsys::read_file(): error ", hFile);
		}

		DWORD fileSize = GetFileSize(hFile, NULL);
		if (fileSize == INVALID_FILE_SIZE) {
			throw qpl::exception("qpl::winsys::read_file(): error: ", hFile, " size: ", fileSize);
			return "";
		}

		char* buffer = new char[fileSize + 1];
		DWORD bytesRead;
		BOOL result = ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);
		CloseHandle(hFile);

		if (!result) {
			// Handle error
			throw qpl::exception("qpl::winsys::read_file(): ReadFile failed.");
			delete[] buffer;
			return "";
		}

		buffer[bytesRead] = '\0';  // Null-terminate the buffer
		std::string fileContent(buffer);
		delete[] buffer;

		return fileContent;
	}

	void qpl::winsys::execute_batch(const std::string& path, const std::string& command) {
		qpl::filesys::create_file(path, command);
		std::system(path.c_str());
		qpl::filesys::remove(path);
	}
	void qpl::winsys::execute_batch_command(const std::string& command) {
		std::system(command.c_str());
	}
	void qpl::winsys::run_command(const std::string& path, const std::string& command, bool print) {
		if (print) {
			qpl::println(qpl::gray, "<in ", qpl::aqua, path, qpl::gray, " executing ", qpl::light_green, command, qpl::gray, ">");
		}
		auto same_dir = qpl::filesys::get_current_location().string().front() == path.front();
		auto set_directory = qpl::to_string(same_dir ? "cd " : "cd /D ", path);
		auto cmd = qpl::to_string("@echo off && ", set_directory, " && echo on && ", command);

		qpl::winsys::execute_batch_command(cmd);
	}

	std::ostream& qpl::operator<<(std::ostream& os, color color) {
		return os;
	}
	std::ostream& qpl::operator<<(std::ostream& os, foreground foreground) {
		return os;
	}
	std::ostream& qpl::operator<<(std::ostream& os, background background) {
		return os;
	}

	qpl::cc qpl::cc::def() {
		qpl::cc result;
		result.foreground = qpl::foreground::white;
		result.background = qpl::background::black;
		return result;
	}
	qpl::cc qpl::cc::random() {
		qpl::cc result;
		result.foreground = qpl::get_random_foreground();
		result.background = qpl::get_random_background();
		return result;
	}
	qpl::cc qpl::cc::random_foreground() {
		qpl::cc result;
		result.foreground = qpl::get_random_foreground();
		return result;
	}
	qpl::cc qpl::cc::random_background() {
		qpl::cc result;
		result.background = qpl::get_random_background();
		return result;
	}

	std::ostream& qpl::operator<<(std::ostream& os, const qpl::cc& cc) {
		return os;
	}
	qpl::cc qpl::get_random_console_color() {
		return qpl::cc::random();
	}
	qpl::background qpl::get_random_background() {
		return static_cast<qpl::background>(qpl::random(0, (int)qpl::background::size - 1));
	}
	qpl::foreground qpl::get_random_foreground() {
		return static_cast<qpl::foreground>(qpl::random(0, (int)qpl::foreground::size - 1));
	}
	void qpl::set_console_color(qpl::color foreground, qpl::color background) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<int>(foreground) + static_cast<int>(background) * 16);
	}
	void qpl::set_console_color(qpl::foreground foreground, qpl::background background) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<int>(foreground) + static_cast<int>(background) * 16);
	}
	void qpl::set_console_color(qpl::background background, qpl::foreground foreground) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<int>(foreground) + static_cast<int>(background) * 16);
	}
	void qpl::set_console_color(qpl::color foreground) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<int>(foreground));
	}
	void qpl::set_console_color(qpl::foreground foreground) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<int>(foreground));
	}
	void qpl::set_console_color(qpl::background background) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<int>(background) * 16);
	}
	void qpl::set_console_color(qpl::cc color) {
		qpl::set_console_color(color.foreground, color.background);
	}
	void qpl::set_console_color_default() {
		qpl::set_console_color(qpl::color::white, qpl::color::black);
	}

	void* qpl::shared_memory::get() {
		if (this->is_array()) {
			return reinterpret_cast<char*>(this->ptr) + qpl::bytes_in_type<qpl::size>();
		}
		else {
			return this->ptr;
		}
	}
	qpl::size qpl::shared_memory::array_byte_size() const {
		if (!this->is_array()) {
			return 0u;
		}
		return *reinterpret_cast<qpl::size*>(this->ptr);
	}
	bool qpl::shared_memory::is_array() const {
		return this->array;
	}
	bool qpl::shared_memory::create(const std::string& name, qpl::size size) {
		this->array = false;
		this->hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			qpl::u32_cast(size),                // maximum object size (low-order DWORD)
			qpl::string_to_wstring(name).c_str());                 // name of mapping object

		if (this->hMapFile == nullptr) {
			qpl::println("qpl::shared_memory::create: could not create file mapping object (", GetLastError(), ")");
			return false;
		}
		this->ptr = MapViewOfFile(this->hMapFile,   
			FILE_MAP_ALL_ACCESS, 
			0,
			0,
			size);

		if (this->ptr == nullptr) {
			qpl::println("qpl::shared_memory::create: could not map view of file (", GetLastError(), ")");

			CloseHandle(this->hMapFile);

			return false;
		}
		return true;
	}
	bool qpl::shared_memory::create_array(const std::string& name, qpl::size size) {
		auto result = this->create(name, size + qpl::bytes_in_type<qpl::size>());
		if (!result) return false;
		this->array = true;
		reinterpret_cast<qpl::size*>(this->ptr)[0] = size;
		return true;
	}

	bool qpl::shared_memory::open(const std::string& name, qpl::size size) {
		this->array = false;
		this->hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			qpl::string_to_wstring(name).c_str());               // name of mapping object

		if (this->hMapFile == nullptr) {
			qpl::println("qpl::shared_memory::create: could not open file mapping object (", GetLastError(), ")");
			return false;
		}

		this->ptr = MapViewOfFile(hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			size);

		if (this->ptr == nullptr) {
			qpl::println("qpl::shared_memory::create: could not map view of file (", GetLastError(), ")");

			CloseHandle(this->hMapFile);

			return false;
		}
		return true;
	}
	bool qpl::shared_memory::open_array(const std::string& name) {
		this->array = true;
		this->hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			qpl::string_to_wstring(name).c_str());               // name of mapping object

		if (this->hMapFile == nullptr) {
			qpl::println("qpl::shared_memory::create: could not open file mapping object (", GetLastError(), ")");
			return false;
		}

		this->ptr = MapViewOfFile(hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			qpl::bytes_in_type<qpl::size>());

		if (this->ptr == nullptr) {
			qpl::println("qpl::shared_memory::create: could not map view of file (", GetLastError(), ")");

			CloseHandle(this->hMapFile);

			return false;
		}
		auto size = this->array_byte_size();
		UnmapViewOfFile(this->ptr);
		this->ptr = MapViewOfFile(hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			size);

		return true;
	}

	void qpl::shared_memory::destroy() {
		UnmapViewOfFile(this->ptr);
		CloseHandle(this->hMapFile);
	}
	namespace detail {
		std::unordered_map<std::string, qpl::shared_memory> qpl::detail::shared_memories;
	}

	qpl::size qpl::get_shared_memory_array_byte_size(std::string name) {
		return qpl::detail::shared_memories[name].array_byte_size();
	}
	void qpl::create_shared_memory_array(std::string name, qpl::size size) {
		qpl::detail::shared_memories[name].create_array(name, size);
	}
	void qpl::open_shared_memory_array(std::string name) {
		qpl::detail::shared_memories[name].open_array(name);
	}
	void qpl::create_shared_memory(std::string name, qpl::size size) {
		qpl::detail::shared_memories[name].create(name, size);
	}
	void qpl::open_shared_memory(std::string name, qpl::size size) {
		qpl::detail::shared_memories[name].open(name, size);
	}
	bool qpl::find_shared_memory(std::string name) {
		qpl::detail::shared_memories[name].hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			qpl::string_to_wstring(name).c_str());               // name of mapping object

		return (qpl::detail::shared_memories[name].hMapFile != nullptr);
	}

}
#endif