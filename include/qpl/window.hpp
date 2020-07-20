#ifndef QPL_WINDOW_HPP
#define QPL_WINDOW_HPP
#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <string_view>
#include <set>

namespace qpl {
	using key_t = qpl::i32;
	namespace key {
		constexpr key_t UNKNOWN = GLFW_KEY_UNKNOWN;
		constexpr key_t SPACE = GLFW_KEY_SPACE;
		constexpr key_t APOSTROPHE = GLFW_KEY_APOSTROPHE;
		constexpr key_t COMMA = GLFW_KEY_COMMA;
		constexpr key_t MINUS = GLFW_KEY_MINUS;
		constexpr key_t PERIOD = GLFW_KEY_PERIOD;
		constexpr key_t SLASH = GLFW_KEY_SLASH;
		constexpr key_t _0 = GLFW_KEY_0;
		constexpr key_t _1 = GLFW_KEY_1;
		constexpr key_t _2 = GLFW_KEY_2;
		constexpr key_t _3 = GLFW_KEY_3;
		constexpr key_t _4 = GLFW_KEY_4;
		constexpr key_t _5 = GLFW_KEY_5;
		constexpr key_t _6 = GLFW_KEY_6;
		constexpr key_t _7 = GLFW_KEY_7;
		constexpr key_t _8 = GLFW_KEY_8;
		constexpr key_t _9 = GLFW_KEY_9;
		constexpr key_t SEMICOLON = GLFW_KEY_SEMICOLON;
		constexpr key_t EQUAL = GLFW_KEY_EQUAL;
		constexpr key_t A = GLFW_KEY_A;
		constexpr key_t B = GLFW_KEY_B;
		constexpr key_t C = GLFW_KEY_C;
		constexpr key_t D = GLFW_KEY_D;
		constexpr key_t E = GLFW_KEY_E;
		constexpr key_t F = GLFW_KEY_F;
		constexpr key_t G = GLFW_KEY_G;
		constexpr key_t H = GLFW_KEY_H;
		constexpr key_t I = GLFW_KEY_I;
		constexpr key_t J = GLFW_KEY_J;
		constexpr key_t K = GLFW_KEY_K;
		constexpr key_t L = GLFW_KEY_L;
		constexpr key_t M = GLFW_KEY_M;
		constexpr key_t N = GLFW_KEY_N;
		constexpr key_t O = GLFW_KEY_O;
		constexpr key_t P = GLFW_KEY_P;
		constexpr key_t Q = GLFW_KEY_Q;
		constexpr key_t R = GLFW_KEY_R;
		constexpr key_t S = GLFW_KEY_S;
		constexpr key_t T = GLFW_KEY_T;
		constexpr key_t U = GLFW_KEY_U;
		constexpr key_t V = GLFW_KEY_V;
		constexpr key_t W = GLFW_KEY_W;
		constexpr key_t X = GLFW_KEY_X;
		constexpr key_t Y = GLFW_KEY_Y;
		constexpr key_t Z = GLFW_KEY_Z;
		//constexpr key_t a = static_cast<qpl::u32>('a');
		//constexpr key_t b = static_cast<qpl::u32>('b');
		//constexpr key_t c = static_cast<qpl::u32>('c');
		//constexpr key_t d = static_cast<qpl::u32>('d');
		//constexpr key_t e = static_cast<qpl::u32>('e');
		//constexpr key_t f = static_cast<qpl::u32>('f');
		//constexpr key_t g = static_cast<qpl::u32>('g');
		//constexpr key_t h = static_cast<qpl::u32>('h');
		//constexpr key_t i = static_cast<qpl::u32>('i');
		//constexpr key_t j = static_cast<qpl::u32>('j');
		//constexpr key_t k = static_cast<qpl::u32>('k');
		//constexpr key_t l = static_cast<qpl::u32>('l');
		//constexpr key_t m = static_cast<qpl::u32>('m');
		//constexpr key_t n = static_cast<qpl::u32>('n');
		//constexpr key_t o = static_cast<qpl::u32>('o');
		//constexpr key_t p = static_cast<qpl::u32>('p');
		//constexpr key_t q = static_cast<qpl::u32>('q');
		//constexpr key_t r = static_cast<qpl::u32>('r');
		//constexpr key_t s = static_cast<qpl::u32>('s');
		//constexpr key_t t = static_cast<qpl::u32>('t');
		//constexpr key_t u = static_cast<qpl::u32>('u');
		//constexpr key_t v = static_cast<qpl::u32>('v');
		//constexpr key_t w = static_cast<qpl::u32>('w');
		//constexpr key_t x = static_cast<qpl::u32>('x');
		//constexpr key_t y = static_cast<qpl::u32>('y');
		//constexpr key_t z = static_cast<qpl::u32>('z');

		constexpr key_t LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
		constexpr key_t BACKSLASH = GLFW_KEY_BACKSLASH;
		constexpr key_t RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
		constexpr key_t GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
		constexpr key_t WORLD_1 = GLFW_KEY_WORLD_1;
		constexpr key_t WORLD_2 = GLFW_KEY_WORLD_2;
		constexpr key_t ESCAPE = GLFW_KEY_ESCAPE;
		constexpr key_t ENTER = GLFW_KEY_ENTER;
		constexpr key_t TAB = GLFW_KEY_TAB;
		constexpr key_t BACKSPACE = GLFW_KEY_BACKSPACE;
		constexpr key_t INSERT = GLFW_KEY_INSERT;
		constexpr key_t DELETE_ = GLFW_KEY_DELETE;
		constexpr key_t RIGHT = GLFW_KEY_RIGHT;
		constexpr key_t LEFT = GLFW_KEY_LEFT;
		constexpr key_t DOWN = GLFW_KEY_DOWN;
		constexpr key_t UP = GLFW_KEY_UP;
		constexpr key_t PAGE_UP = GLFW_KEY_PAGE_UP;
		constexpr key_t PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
		constexpr key_t HOME = GLFW_KEY_HOME;
		constexpr key_t END = GLFW_KEY_END;
		constexpr key_t CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
		constexpr key_t SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
		constexpr key_t NUM_LOCK = GLFW_KEY_NUM_LOCK;
		constexpr key_t PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
		constexpr key_t PAUSE = GLFW_KEY_PAUSE;
		constexpr key_t F1 = GLFW_KEY_F1;
		constexpr key_t F2 = GLFW_KEY_F2;
		constexpr key_t F3 = GLFW_KEY_F3;
		constexpr key_t F4 = GLFW_KEY_F4;
		constexpr key_t F5 = GLFW_KEY_F5;
		constexpr key_t F6 = GLFW_KEY_F6;
		constexpr key_t F7 = GLFW_KEY_F7;
		constexpr key_t F8 = GLFW_KEY_F8;
		constexpr key_t F9 = GLFW_KEY_F9;
		constexpr key_t F10 = GLFW_KEY_F10;
		constexpr key_t F11 = GLFW_KEY_F11;
		constexpr key_t F12 = GLFW_KEY_F12;
		constexpr key_t F13 = GLFW_KEY_F13;
		constexpr key_t F14 = GLFW_KEY_F14;
		constexpr key_t F15 = GLFW_KEY_F15;
		constexpr key_t F16 = GLFW_KEY_F16;
		constexpr key_t F17 = GLFW_KEY_F17;
		constexpr key_t F18 = GLFW_KEY_F18;
		constexpr key_t F19 = GLFW_KEY_F19;
		constexpr key_t F20 = GLFW_KEY_F20;
		constexpr key_t F21 = GLFW_KEY_F21;
		constexpr key_t F22 = GLFW_KEY_F22;
		constexpr key_t F23 = GLFW_KEY_F23;
		constexpr key_t F24 = GLFW_KEY_F24;
		constexpr key_t F25 = GLFW_KEY_F25;
		constexpr key_t KP_0 = GLFW_KEY_KP_0;
		constexpr key_t KP_1 = GLFW_KEY_KP_1;
		constexpr key_t KP_2 = GLFW_KEY_KP_2;
		constexpr key_t KP_3 = GLFW_KEY_KP_3;
		constexpr key_t KP_4 = GLFW_KEY_KP_4;
		constexpr key_t KP_5 = GLFW_KEY_KP_5;
		constexpr key_t KP_6 = GLFW_KEY_KP_6;
		constexpr key_t KP_7 = GLFW_KEY_KP_7;
		constexpr key_t KP_8 = GLFW_KEY_KP_8;
		constexpr key_t KP_9 = GLFW_KEY_KP_9;
		constexpr key_t KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
		constexpr key_t KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
		constexpr key_t KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
		constexpr key_t KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
		constexpr key_t KP_ADD = GLFW_KEY_KP_ADD;
		constexpr key_t KP_ENTER = GLFW_KEY_KP_ENTER;
		constexpr key_t KP_EQUAL = GLFW_KEY_KP_EQUAL;
		constexpr key_t LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
		constexpr key_t LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
		constexpr key_t LEFT_ALT = GLFW_KEY_LEFT_ALT;
		constexpr key_t LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
		constexpr key_t RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
		constexpr key_t RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
		constexpr key_t RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
		constexpr key_t RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
		constexpr key_t MENU = GLFW_KEY_MENU;
		constexpr key_t LAST = GLFW_KEY_LAST;
	}

	class window {
	public:

		static GLFWwindow* p_window;
		window() = delete;

		QPLDLL static void set_dimension(glm::ivec2 dim);
		QPLDLL static glm::ivec2 get_dimension();

		QPLDLL static void create();
		QPLDLL static bool is_created();
		QPLDLL static void destroy();

		QPLDLL static void set_title(std::string_view title);
		QPLDLL static std::string_view get_title();

		QPLDLL static void consider_minimize();

		QPLDLL static bool is_open();
		QPLDLL static void close();
		QPLDLL static void poll_events();
		QPLDLL static bool key_pressed();
		QPLDLL static bool key_released();
		QPLDLL static bool key_pressed(qpl::i32 key);
		QPLDLL static bool key_pressed_utf(qpl::u32 key);
		QPLDLL static bool key_released(qpl::i32 key);
		QPLDLL static bool key_holding();
		QPLDLL static bool key_holding(qpl::i32 key);
		QPLDLL static glm::vec2 mouse_position();

		QPLDLL static bool scrolled_up();
		QPLDLL static bool scrolled_down();

		QPLDLL static bool left_mouse_clicked();
		QPLDLL static bool left_mouse_holding();
		QPLDLL static bool left_mouse_released();
		QPLDLL static bool right_mouse_clicked();
		QPLDLL static bool right_mouse_holding();
		QPLDLL static bool right_mouse_released();

		QPLDLL static bool dimension_changed();

		template<typename ...args>
		static bool key_holding(qpl::i32 key, args... more) {
			return qpl::window::key_holding(key) && (qpl::window::key_holding(more) && ...);
		}
	private:
		QPLDLL static void reset_events();
		QPLDLL static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		QPLDLL static void char_callback(GLFWwindow* window, qpl::u32 code);
		QPLDLL static void resize_callback(GLFWwindow* window, int width, int height);
		QPLDLL static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
		QPLDLL static void scroll_callback(GLFWwindow* window, double x, double y);

		static bool m_key_pressed;
		static bool m_key_released;
		static bool m_mouse_left_clicked;
		static bool m_mouse_left_released;
		static bool m_mouse_right_clicked;
		static bool m_mouse_right_released;
		static bool m_mouse_left_holding;
		static bool m_mouse_right_holding;
		static bool m_mouse_wheel_scroll_up;
		static bool m_mouse_wheel_scroll_down;
		static qpl::i32 m_scancode;
		static std::set<qpl::i32> m_press_codes;
		static std::set<qpl::i32> m_release_codes;
		static std::set<qpl::i32> m_holding_codes;
		static std::set<qpl::u32> m_text_press_codes;
		static bool m_created;
		static glm::ivec2 m_dimension;
		static bool m_dimension_changed;
		static std::string_view m_title;
	};

}

#endif