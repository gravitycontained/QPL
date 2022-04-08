#ifndef QSF_EVENTINFO_HPP
#define QSF_EVENTINFO_HPP
#pragma once

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)

#include <qpl/qpldeclspec.hpp>
#include <SFML/Graphics.hpp>
#include <qpl/vector.hpp>
#include <qpl/time.hpp>
#include <set>

namespace qsf {


	struct event_info;

	template<typename C>
	concept has_update_c = requires (C x, const event_info & event) {
		x.update(event);
	};
	template<typename C>
	constexpr bool has_update() {
		return has_update_c<C>;
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

		QPLDLL bool left_mouse_double_clicked() const;
		QPLDLL bool right_mouse_double_clicked() const;
		QPLDLL bool middle_mouse_double_clicked() const;

		QPLDLL void reset(const sf::RenderWindow& window);
		QPLDLL void update(const sf::Event& event);

		QPLDLL qpl::vector2u screen_dimension() const;
		QPLDLL qpl::time frame_time() const;
		QPLDLL qpl::f64 frame_time_f() const;
		QPLDLL qpl::vector2i resized_size() const;
		QPLDLL qpl::vector2i mouse_position() const;
		QPLDLL qpl::vector2i mouse_position_desktop() const;

		QPLDLL bool text_entered(char c) const;
		QPLDLL bool text_entered(wchar_t c) const;
		QPLDLL bool text_entered(std::string c) const;
		QPLDLL bool text_entered(std::wstring c) const;

		QPLDLL std::wstring text_entered() const;
		QPLDLL std::string text_entered_str() const;
		QPLDLL std::wstring all_text_entered() const;
		QPLDLL std::string all_text_entered_str() const;


		template<typename T> requires (qsf::has_update<T>() || (qpl::is_container<T>() && qsf::has_update<qpl::container_deepest_subtype<T>>()))
		void update(T& updatable) const {
			if constexpr (qsf::has_update<T>()) {
				updatable.update(*this);
			}
			else {
				for (auto& i : updatable) {
					this->update(i);
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

		std::wstring m_text_entered;
		std::wostringstream m_text_entered_stream;

		qpl::vector2i m_resized_size;
		qpl::vector2i m_mouse_position;
		qpl::vector2i m_mouse_position_desktop;
		std::set<sf::Keyboard::Key> m_keys_pressed;
		std::set<sf::Keyboard::Key> m_keys_released;
		std::set<sf::Keyboard::Key> m_keys_single_pressed;
		std::set<sf::Keyboard::Key> m_keys_single_released;
		std::set<sf::Keyboard::Key> m_keys_holding;

		qpl::time m_frame_time = 0;
		qpl::vector2u m_screen_dimension = qpl::vec(0u, 0u);

		qpl::halted_clock m_left_mouse_clock;
		qpl::halted_clock m_right_mouse_clock;
		qpl::halted_clock m_middle_mouse_clock;
	};

}

#endif
#endif