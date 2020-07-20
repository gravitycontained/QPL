
#include <qpl/winsys.hpp>
#include <qpl/random.hpp>
#include <qpl/string.hpp>

#include <iostream>
#include <sstream>
#include <regex>

#include <TlHelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
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
		process_list& qpl::winsys::get_process_list() {
			qpl::winsys::impl::p_list.clear();
			EnumWindows(qpl::winsys::impl::process_list_window_callback, NULL);
			return qpl::winsys::impl::p_list;
		}
		void clear_watchlist() {
			qpl::winsys::impl::w_list.clear();
		}
		bool add_to_watchlist(std::wstring window_name) {
			bool found = false;
			for (auto i : qpl::winsys::impl::p_list) {
				if (i.window_name == window_name) {
					qpl::winsys::impl::w_list.push_back(i);
					found = true;
				}
			}
			return found;
		}
		watch_list& get_watchlist() {
			EnumWindows(qpl::winsys::impl::watch_list_window_callback, NULL);
			return qpl::winsys::impl::w_list;
		}

		point get_mouse_position() {
			POINT p;
			GetCursorPos(&p);
			return { p.x, p.y };
		}
		bool mouse_left_clicked() {
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


		std::vector<qpl::pixel_rgb> qpl::winsys::get_screen_pixels(qpl::winsys::rect rectangle) {
			auto dim = qpl::get_screen_dimension();
			HWND hDesktopWnd = GetDesktopWindow();
			HDC hDesktopDC = GetDC(hDesktopWnd);
			HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
			HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, dim.x, dim.y);
			SelectObject(hCaptureDC, hCaptureBitmap);

			BitBlt(hCaptureDC, 0, 0, dim.x, dim.y, hDesktopDC, 0, 0, SRCCOPY | CAPTUREBLT);


			// Create a BITMAPINFO specifying the format you want the pixels in.
			// To keep this simple, we'll use 32-bits per pixel (the high byte isn't
			// used).
			BITMAPINFO bmi = { 0 };
			bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
			bmi.bmiHeader.biWidth = dim.x;
			bmi.bmiHeader.biHeight = dim.y;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;

			// Allocate a buffer to receive the pixel data.
			RGBQUAD* pPixels = new RGBQUAD[dim.x * dim.y];

			// Call GetDIBits to copy the bits from the device dependent bitmap
			// into the buffer allocated above, using the pixel format you
			// chose in the BITMAPINFO.
			::GetDIBits(hCaptureDC,
				hCaptureBitmap,
				0,  // starting scanline
				dim.y,  // scanlines to copy
				pPixels,  // buffer for your copy of the pixels
				&bmi,  // format you want the data in
				DIB_RGB_COLORS);  // actual pixels, not palette references

			std::vector<qpl::pixel_rgb> pixels(rectangle.width() * rectangle.height());

			unsigned ctr = 0u;
			for (int y = 0; y < rectangle.height(); ++y) {
				for (int x = 0; x < rectangle.width(); ++x) {
					auto pi = (rectangle.height() - y - 1) * rectangle.width() + x;
					auto wi = (dim.y - (y + rectangle.top_left.y) - 1) * dim.x + (x + rectangle.top_left.x);

					if (pi > pixels.size()) {
						//
					}
					if (wi > dim.x * dim.y) {
						//
					}

					pixels[pi].r = pPixels[wi].rgbRed;
					pixels[pi].g = pPixels[wi].rgbGreen;
					pixels[pi].b = pPixels[wi].rgbBlue;
				}
			}
			// You can now access the raw pixel data in pPixels.  Note that they are
			// stored from the bottom scanline to the top, so pPixels[0] is the lower
			// left pixel, pPixels[1] is the next pixel to the right,
			// pPixels[nScreenWidth] is the first pixel on the second row from the
			// bottom, etc.

			// Don't forget to free the pixel buffer.
			delete[] pPixels;
			return pixels;
		}
		std::vector<qpl::pixel_rgb> qpl::winsys::get_screen_pixels() {
			return qpl::winsys::get_screen_pixels(qpl::winsys::rect{ {0, 0}, qpl::get_screen_dimension() });
		}
	}
	qpl::winsys::point qpl::get_screen_dimension() {
		RECT desktop;
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)

		return { desktop.right, desktop.bottom };
	}
	void qpl::screen_shot(const std::string& file_name) {
		auto dim = qpl::get_screen_dimension();
		auto screen = qpl::winsys::get_screen_pixels();
		qpl::generate_bmp(screen, dim.x, dim.y, file_name);
	}
	void qpl::clear_console() {
		system("cls");
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
	void qpl::copy_to_clipboard(const std::string& string) {
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, string.size() + 1);
		memcpy(GlobalLock(hMem), string.data(), string.size() + 1);
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}
}