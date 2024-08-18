#ifndef QSF_EVENTINFO_HPP
#define QSF_EVENTINFO_HPP
#pragma once

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE

#include <qpl/qpldeclspec.hpp>
#include <SFML/Graphics.hpp>
#include <qpl/QSF/view.hpp>
#include <qpl/vector.hpp>
#include <qpl/time.hpp>
#include <set>

namespace qsf {
	struct event_info;

	template<typename C, typename ... Args>
	concept has_update_c = requires (C x, const event_info& event, Args... args) {
		x.update(event, args...);
	};
	template<typename C, typename ... Args>
	constexpr bool has_update() {
		return has_update_c<C, Args...>;
	}


	struct event_info {
		QPLDLL bool key_single_pressed(sf::Keyboard::Key key) const;
		QPLDLL bool key_single_released(sf::Keyboard::Key key) const;
		QPLDLL bool key_pressed(sf::Keyboard::Key key) const;
		QPLDLL bool key_released(sf::Keyboard::Key key) const;
		QPLDLL bool key_holding(sf::Keyboard::Key key) const;
		QPLDLL bool keys_pressed(const std::vector<sf::Keyboard::Key>& keys) const;
		QPLDLL bool keys_released(const std::vector<sf::Keyboard::Key>& keys) const;
		QPLDLL bool keys_holding(const std::vector<sf::Keyboard::Key>& keys) const;

		QPLDLL bool mouse_button_clicked(sf::Mouse::Button button) const;
		QPLDLL bool mouse_button_released(sf::Mouse::Button button) const;
		QPLDLL bool mouse_button_holding(sf::Mouse::Button button) const;

		QPLDLL bool mouse_button_clicked() const;
		QPLDLL bool mouse_button_released() const;
		QPLDLL bool mouse_button_holding() const;

		QPLDLL bool mouse_moved() const;
		QPLDLL bool left_mouse_clicked() const;
		QPLDLL bool left_mouse_released() const;
		QPLDLL bool right_mouse_clicked() const;
		QPLDLL bool right_mouse_released() const;
		QPLDLL bool middle_mouse_clicked() const;
		QPLDLL bool middle_mouse_released() const;
		QPLDLL bool scrolled_up() const;
		QPLDLL bool scrolled_down() const;
		QPLDLL bool key_pressed() const;
		QPLDLL bool key_single_pressed() const;
		QPLDLL bool key_released() const;
		QPLDLL bool key_single_released() const;
		QPLDLL bool key_holding() const;

		QPLDLL bool resized() const;
		QPLDLL bool window_closed() const;

		QPLDLL bool holding_left_mouse() const;
		QPLDLL bool holding_right_mouse() const;
		QPLDLL bool holding_middle_mouse() const;
		QPLDLL bool holding_key() const;

		QPLDLL bool left_mouse_fast_clicked() const;
		QPLDLL bool right_mouse_fast_clicked() const;
		QPLDLL bool middle_mouse_fast_clicked() const;

		QPLDLL bool left_mouse_double_clicked() const;
		QPLDLL bool right_mouse_double_clicked() const;
		QPLDLL bool middle_mouse_double_clicked() const;
		QPLDLL qpl::size left_mouse_fast_click_count() const;
		QPLDLL qpl::size right_mouse_fast_click_count() const;
		QPLDLL qpl::size middle_mouse_fast_click_count() const;

		QPLDLL void reset(const sf::RenderWindow& window);
		QPLDLL void update(const sf::Event& event);
		QPLDLL void set_fast_click_duration(qpl::f64 duration);
		QPLDLL qpl::f64 get_fast_click_duration() const;
		QPLDLL void set_fast_double_click_duration(qpl::f64 duration);
		QPLDLL qpl::f64 get_fast_double_click_duration() const;

		QPLDLL qpl::vector2u screen_dimension() const;
		QPLDLL qpl::time frame_time() const;
		QPLDLL qpl::f64 frame_time_f() const;
		QPLDLL qpl::vector2i resized_size() const;
		QPLDLL qpl::vector2f mouse_position() const;
		QPLDLL qpl::vector2f delta_mouse_position() const;
		QPLDLL qpl::vector2i mouse_position_screen() const;
		QPLDLL qpl::vector2i mouse_position_desktop() const;

		QPLDLL void reset_delta_mouse();

		template<typename T>
		void apply_view(const qsf::view_t<T>& view) const {
			this->m_mouse_position = view.transform_point(this->m_mouse_position);
			this->m_delta_mouse_position = view.transform_point_no_offset(this->m_delta_mouse_position);
		}

		QPLDLL bool text_entered(char c) const;
		QPLDLL bool text_entered(wchar_t c) const;
		QPLDLL bool text_entered(std::string c) const;
		QPLDLL bool text_entered(std::wstring c) const;

		QPLDLL bool is_text_entered() const;
		QPLDLL std::wstring text_entered() const;
		QPLDLL qpl::u32_string u32_text_entered() const;
		QPLDLL std::string text_entered_str() const;
		QPLDLL std::wstring all_text_entered() const;
		QPLDLL std::string all_text_entered_str() const;

		template<typename T, typename ... Args> requires (qsf::has_update<T, Args...>() || (qpl::is_container<T>() && qsf::has_update<qpl::container_deepest_subtype<T>, Args...>()))
		void update(T& updatable, Args&&... args) const {
			if constexpr (qsf::has_update<T, Args...>()) {
				if constexpr (qsf::has_view<T>()) {
					if (updatable.auto_view.is_default_view()) {
						updatable.update(*this, args...);
					}
					else {
						auto before = this->m_mouse_position;
						auto before_delta = this->m_delta_mouse_position;
						this->apply_view(updatable.auto_view);

						updatable.update(*this, args...);

						this->m_mouse_position = before;
						this->m_delta_mouse_position = before_delta;
					}
				}
				else {
					updatable.update(*this, args...);
				}
			}
			else {
				for (auto& i : updatable) {
					this->update(i, args...);
				}
			}
		}

		bool m_mouse_clicked = false;
		bool m_mouse_released = false;
		bool m_mouse_holding = false;
		bool m_left_mouse_clicked = false;
		bool m_left_mouse_released = false;
		bool m_right_mouse_clicked = false;
		bool m_right_mouse_released = false;
		bool m_middle_mouse_clicked = false;
		bool m_middle_mouse_released = false;
		bool m_scrolled_up = false;
		bool m_scrolled_down = false;
		bool m_key_pressed = false;
		bool m_key_single_pressed = false;
		bool m_key_released = false;
		bool m_key_single_released = false;
		bool m_key_holding = false;
		bool m_mouse_moved = false;
		bool m_window_closed = false;
		bool m_resized = false;

		bool m_holding_left_mouse = false;
		bool m_holding_right_mouse = false;
		bool m_holding_middle_mouse = false;
		bool m_holding_key = false;
		bool m_left_mouse_double_click = false;
		bool m_right_mouse_double_click = false;
		bool m_middle_mouse_double_click = false;
		bool m_left_mouse_fast_click = false;
		bool m_right_mouse_fast_click = false;
		bool m_middle_mouse_fast_click = false;

		std::wstring m_text_entered;
		qpl::u32_string m_u32_text_entered;
		std::wstring m_text_entered_total;

		qpl::vector2i m_resized_size;
		mutable qpl::vector2f m_mouse_position;
		mutable qpl::vector2f m_delta_mouse_position;
		qpl::vector2i m_mouse_position_screen;
		qpl::vector2i m_mouse_position_screen_before;
		qpl::vector2i m_mouse_position_desktop;
		std::set<sf::Keyboard::Key> m_keys_pressed;
		std::set<sf::Keyboard::Key> m_keys_released;
		std::set<sf::Keyboard::Key> m_keys_single_pressed;
		std::set<sf::Keyboard::Key> m_keys_single_released;
		std::set<sf::Keyboard::Key> m_keys_holding;

		qpl::time m_frame_time = 0;
		qpl::vector2u m_screen_dimension = qpl::vec(0u, 0u);

		qpl::small_clock m_left_mouse_click_release_clock;
		qpl::small_clock m_right_mouse_click_release_clock;
		qpl::small_clock m_middle_mouse_click_release_clock;

		qpl::halted_clock m_left_mouse_clock;
		qpl::halted_clock m_right_mouse_clock;
		qpl::halted_clock m_middle_mouse_clock;
		qpl::size m_left_mouse_fast_click_ctr = 1u;
		qpl::size m_right_mouse_fast_click_ctr = 1u;
		qpl::size m_middle_mouse_fast_click_ctr = 1u;

		qpl::f64 m_fast_double_click_duration = 0.2;
		qpl::f64 m_fast_click_duration = 0.15;

		//bool m_joystick_connect;
		//bool m_joystick_disconnect;
		//qpl::vec2 m_joystick_left_direction;
		//qpl::vec2 m_joystick_right_direction;
		//std::unordered_set<sf::Joystick::Axis> m_joystick_axis_pressed;
		//std::unordered_set<sf::Joystick::Axis> m_joystick_axis_released;
		//std::unordered_set<sf::Joystick::Axis> m_joystick_axis_holding;
	};

}

#endif
#endif