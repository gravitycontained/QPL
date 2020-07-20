#include <qpl/window.hpp>

#ifdef QPL_USE_VULKAN
#include <stdexcept>
#include <iostream>

namespace qpl {

	GLFWwindow* qpl::window::p_window = nullptr;
	bool qpl::window::m_key_pressed = false;
	bool qpl::window::m_key_released = false;
	bool qpl::window::m_mouse_left_clicked = false;
	bool qpl::window::m_mouse_left_released = false;
	bool qpl::window::m_mouse_right_clicked = false;
	bool qpl::window::m_mouse_right_released = false;
	bool qpl::window::m_mouse_left_holding = false;
	bool qpl::window::m_mouse_right_holding = false;
	bool qpl::window::m_mouse_wheel_scroll_up = false;
	bool qpl::window::m_mouse_wheel_scroll_down = false;

	qpl::i32 qpl::window::m_scancode = qpl::i32{};
	std::set<qpl::i32> qpl::window::m_press_codes;
	std::set<qpl::i32> qpl::window::m_release_codes;
	std::set<qpl::i32> qpl::window::m_holding_codes;
	std::set<qpl::u32> qpl::window::m_text_press_codes;
	glm::ivec2 qpl::window::m_dimension = { 1600, 900 };
	bool qpl::window::m_dimension_changed = false;
	bool qpl::window::m_created = false;
	std::string_view qpl::window::m_title = "qpl application";

	void qpl::window::reset_events() {
		qpl::window::m_key_pressed = false;
		qpl::window::m_key_released = false;
		qpl::window::m_mouse_left_clicked = false;
		qpl::window::m_mouse_left_released = false;
		qpl::window::m_mouse_right_clicked = false;
		qpl::window::m_mouse_right_released = false;
		qpl::window::m_scancode = 0;
		qpl::window::m_press_codes.clear();
		qpl::window::m_release_codes.clear();
		qpl::window::m_text_press_codes.clear();
		qpl::window::m_dimension_changed = false;
		qpl::window::m_mouse_wheel_scroll_up = false;
		qpl::window::m_mouse_wheel_scroll_down = false;
	}
	void qpl::window::poll_events() {
		qpl::window::reset_events();
		glfwPollEvents();
	}
	bool qpl::window::key_pressed() {
		return qpl::window::m_key_pressed;
	}
	bool qpl::window::key_released() {
		return qpl::window::m_key_released;
	}
	bool qpl::window::key_pressed(qpl::i32 key) {
		return qpl::window::m_press_codes.find(key) != qpl::window::m_press_codes.cend();
	}
	bool qpl::window::key_pressed_utf(qpl::u32 key) {
		return qpl::window::m_text_press_codes.find(key) != qpl::window::m_text_press_codes.cend();
	}
	bool qpl::window::key_released(qpl::i32 key) {
		return qpl::window::m_release_codes.find(key) != qpl::window::m_release_codes.cend();
	}
	bool qpl::window::key_holding() {
		return !qpl::window::m_holding_codes.empty();
	}
	bool qpl::window::key_holding(qpl::i32 utf8) {
		return qpl::window::m_holding_codes.find(utf8) != qpl::window::m_holding_codes.cend();
	}
	glm::vec2 qpl::window::mouse_position() {
		double xpos, ypos;
		glfwGetCursorPos(qpl::window::p_window, &xpos, &ypos);
		return { xpos, ypos };
	}


	bool qpl::window::scrolled_up() {
		return qpl::window::m_mouse_wheel_scroll_up;
	}
	bool qpl::window::scrolled_down() {
		return qpl::window::m_mouse_wheel_scroll_down;
	}

	bool qpl::window::left_mouse_clicked() {
		return qpl::window::m_mouse_left_clicked;
	}
	bool qpl::window::left_mouse_holding() {
		return qpl::window::m_mouse_left_holding;
	}
	bool qpl::window::left_mouse_released() {
		return qpl::window::m_mouse_left_released;
	}
	bool qpl::window::right_mouse_clicked() {
		return qpl::window::m_mouse_right_clicked;
	}
	bool qpl::window::right_mouse_holding() {
		return qpl::window::m_mouse_right_holding;
	}
	bool qpl::window::right_mouse_released() {
		return qpl::window::m_mouse_right_released;
	}
	bool qpl::window::dimension_changed() {
		return qpl::window::m_dimension_changed;
	}
	void qpl::window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		qpl::window::m_key_pressed = action == GLFW_PRESS;
		qpl::window::m_key_released = action == GLFW_RELEASE;
		qpl::window::m_scancode = scancode;
		if (qpl::window::m_key_pressed) {
			qpl::window::m_press_codes.insert(key);
			qpl::window::m_holding_codes.insert(key);
		}
		if (qpl::window::m_key_released) {
			qpl::window::m_release_codes.insert(key);
			if (qpl::window::m_holding_codes.find(key) != qpl::window::m_holding_codes.cend()) {
				qpl::window::m_holding_codes.erase(key);
			}
		}
	}
	void qpl::window::char_callback(GLFWwindow* window, qpl::u32 code) {
		qpl::window::m_text_press_codes.insert(code);
	}
	void qpl::window::resize_callback(GLFWwindow* window, int width, int height) {
		auto before = qpl::window::m_dimension;
		qpl::window::m_dimension = { width, height };
		if (qpl::window::m_dimension != before) {
			qpl::window::m_dimension_changed = true;
		}
	}
	void qpl::window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
		qpl::window::m_mouse_left_clicked = (button == GLFW_MOUSE_BUTTON_1) && (action == GLFW_PRESS);
		qpl::window::m_mouse_left_released = (button == GLFW_MOUSE_BUTTON_1) && (action == GLFW_RELEASE);
		qpl::window::m_mouse_right_clicked = (button == GLFW_MOUSE_BUTTON_2) && (action == GLFW_PRESS);
		qpl::window::m_mouse_right_released = (button == GLFW_MOUSE_BUTTON_2) && (action == GLFW_RELEASE);

		if (qpl::window::m_mouse_left_clicked) {
			qpl::window::m_mouse_left_holding = true;
		}
		else if (qpl::window::m_mouse_left_released) {
			qpl::window::m_mouse_left_holding = false;
		}

		if (qpl::window::m_mouse_right_clicked) {
			qpl::window::m_mouse_right_holding = true;
		}
		else if (qpl::window::m_mouse_right_released) {
			qpl::window::m_mouse_right_holding = false;
		}
	}

	void qpl::window::scroll_callback(GLFWwindow* window, double x, double y) {
		qpl::window::m_mouse_wheel_scroll_up = y > 0.0;
		qpl::window::m_mouse_wheel_scroll_down = y < 0.0;
	}
	void qpl::window::consider_minimize() {
		qpl::i32 width = 0, height = 0;
		glfwGetFramebufferSize(qpl::window::p_window, &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(qpl::window::p_window, &width, &height);
			glfwWaitEvents();
		}
	}
	void qpl::window::destroy() {
		if (qpl::window::is_created()) {
			qpl::window::reset_events();
			qpl::window::m_holding_codes.clear();
			glfwDestroyWindow(qpl::window::p_window);

			glfwTerminate();
			qpl::window::m_created = false;
		}
	}
	void qpl::window::create() {
		if (qpl::window::is_created()) {
			throw std::runtime_error("window already created!");
			return;
		}

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwMakeContextCurrent(qpl::window::p_window);
		glfwSwapInterval(0);
		qpl::window::p_window = glfwCreateWindow(qpl::window::m_dimension.x, qpl::window::m_dimension.y, qpl::window::m_title.data(), nullptr, nullptr);

		glfwSetKeyCallback(qpl::window::p_window, qpl::window::key_callback);
		glfwSetCharCallback(qpl::window::p_window, qpl::window::char_callback);
		glfwSetWindowSizeCallback(qpl::window::p_window, qpl::window::resize_callback);
		glfwSetMouseButtonCallback(qpl::window::p_window, qpl::window::mouse_callback);
		glfwSetScrollCallback(qpl::window::p_window, qpl::window::scroll_callback);
		qpl::window::m_created = true;

	}
	void qpl::window::set_dimension(glm::ivec2 dim) {
		qpl::window::m_dimension = dim;
	}
	glm::ivec2 qpl::window::get_dimension() {
		return qpl::window::m_dimension;
	}
	bool qpl::window::is_created() {
		return qpl::window::m_created;
	}
	void qpl::window::set_title(std::string_view title) {
		qpl::window::m_title = title;
	}
	std::string_view qpl::window::get_title() {
		return qpl::window::m_title;
	}
	bool qpl::window::is_open() {
		return qpl::window::m_created && !glfwWindowShouldClose(qpl::window::p_window);
	}
	void qpl::window::close() {
		qpl::window::destroy();
	}
}
#endif