#ifndef QPLSF_EVENTINFO_HPP
#define QPLSF_EVENTINFO_HPP
#pragma once
#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)

#include <qpl/qpldeclspec.hpp>
#include <SFML/Graphics.hpp>
#include <qpl/QSF/vector.hpp>
#include <set>

namespace qsf {

	struct event_info {
		QPLDLL bool key_single_pressed(sf::Keyboard::Key key) const;
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
		QPLDLL bool key_holding() const;

		QPLDLL bool resized() const;
		QPLDLL bool window_closed() const;

		QPLDLL bool holding_left_mouse() const;
		QPLDLL bool holding_right_mouse() const;
		QPLDLL bool holding_middle_mouse() const;
		QPLDLL bool holding_key() const;

		QPLDLL qsf::vector2i resized_size() const;
		QPLDLL qsf::vector2i mouse_position() const;
		QPLDLL qsf::vector2i mouse_position_desktop() const;

		QPLDLL bool text_entered(char c) const;
		QPLDLL bool text_entered(wchar_t c) const;
		QPLDLL bool text_entered(std::string c) const;
		QPLDLL bool text_entered(std::wstring c) const;

		QPLDLL std::wstring text_entered() const;
		QPLDLL std::string text_entered_str() const;
		QPLDLL std::wstring all_text_entered() const;
		QPLDLL std::string all_text_entered_str() const;

		bool m_mouse_clicked;
		bool m_mouse_released;
		bool m_mouse_holding;
		bool m_left_mouse_clicked;
		bool m_left_mouse_released;
		bool m_right_mouse_clicked;
		bool m_right_mouse_released;
		bool m_middle_mouse_clicked;
		bool m_middle_mouse_released;
		bool m_scrolled_up;
		bool m_scrolled_down;
		bool m_key_pressed;
		bool m_key_single_pressed;
		bool m_key_released;
		bool m_key_holding;
		bool m_mouse_moved;
		bool m_window_closed;
		bool m_resized;

		bool m_holding_left_mouse;
		bool m_holding_right_mouse;
		bool m_holding_middle_mouse;
		bool m_holding_key;

		std::wstring m_text_entered;
		std::wostringstream m_text_entered_stream;

		qsf::vector2i m_resized_size;
		qsf::vector2i m_mouse_position;
		qsf::vector2i m_mouse_position_desktop;
		std::set<sf::Keyboard::Key> m_keys_pressed;
		std::set<sf::Keyboard::Key> m_keys_released;
		std::set<sf::Keyboard::Key> m_keys_single_pressed;
		std::set<sf::Keyboard::Key> m_keys_holding;

	};
}

#endif
#endif