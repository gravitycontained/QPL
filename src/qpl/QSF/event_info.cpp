#include <qpl/QSF/event_info.hpp>

#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)

namespace qsf {
	bool qsf::event_info::key_pressed(sf::Keyboard::Key key) const {
		return this->m_keys_pressed.find(key) != this->m_keys_pressed.cend();
	}
	bool qsf::event_info::keys_pressed(const std::vector<sf::Keyboard::Key>& keys) const {
		for (auto& i : keys) {
			if (this->m_keys_pressed.find(i) == this->m_keys_pressed.cend()) {
				return false;
			}
		}
		return true;
	}
	bool qsf::event_info::key_released(sf::Keyboard::Key key) const {
		return this->m_keys_released.find(key) != this->m_keys_released.cend();
	}
	bool qsf::event_info::keys_released(const std::vector<sf::Keyboard::Key>& keys) const {
		for (auto& i : keys) {
			if (this->m_keys_released.find(i) == this->m_keys_released.cend()) {
				return false;
			}
		}
		return true;
	}
	bool qsf::event_info::key_holding(sf::Keyboard::Key key) const {
		return this->m_keys_holding.find(key) != this->m_keys_holding.cend();
	}
	bool qsf::event_info::keys_holding(const std::vector<sf::Keyboard::Key>& keys) const {
		for (auto& i : keys) {
			if (this->m_keys_holding.find(i) == this->m_keys_holding.cend()) {
				return false;
			}
		}
		return true;
	}

	bool qsf::event_info::mouse_button_clicked(sf::Mouse::Button button) const {
		if (button == sf::Mouse::Button::Left) {
			return this->m_left_mouse_clicked;
		}
		else if (button == sf::Mouse::Button::Middle) {
			return this->m_middle_mouse_clicked;
		}
		else if (button == sf::Mouse::Button::Right) {
			return this->m_right_mouse_clicked;
		}
		return false;
	}
	bool qsf::event_info::mouse_button_released(sf::Mouse::Button button) const {
		if (button == sf::Mouse::Button::Left) {
			return this->m_left_mouse_released;
		}
		else if (button == sf::Mouse::Button::Middle) {
			return this->m_middle_mouse_released;
		}
		else if (button == sf::Mouse::Button::Right) {
			return this->m_right_mouse_released;
		}
		return false;
	}
	bool qsf::event_info::mouse_button_holding(sf::Mouse::Button button) const {
		if (button == sf::Mouse::Button::Left) {
			return this->m_holding_left_mouse;
		}
		else if (button == sf::Mouse::Button::Middle) {
			return this->m_holding_middle_mouse;
		}
		else if (button == sf::Mouse::Button::Right) {
			return this->m_holding_right_mouse;
		}
		return false;
	}
	bool qsf::event_info::mouse_button_clicked() const {
		return this->m_mouse_clicked;
	}
	bool qsf::event_info::mouse_button_released() const {
		return this->m_mouse_released;
	}
	bool qsf::event_info::mouse_button_holding() const {
		return this->m_mouse_holding;
	}

	bool qsf::event_info::mouse_moved() const {
		return this->m_mouse_moved;
	}
	bool qsf::event_info::left_mouse_clicked() const {
		return this->m_left_mouse_clicked;
	}
	bool qsf::event_info::left_mouse_released() const {
		return this->m_left_mouse_released;
	}
	bool qsf::event_info::right_mouse_clicked() const {
		return this->m_right_mouse_clicked;
	}
	bool qsf::event_info::right_mouse_released() const {
		return this->m_right_mouse_released;
	}
	bool qsf::event_info::middle_mouse_clicked() const {
		return this->m_middle_mouse_clicked;
	}
	bool qsf::event_info::middle_mouse_released() const {
		return this->m_middle_mouse_released;
	}
	bool qsf::event_info::scrolled_up() const {
		return this->m_scrolled_up;
	}
	bool qsf::event_info::scrolled_down() const {
		return this->m_scrolled_down;
	}
	bool qsf::event_info::key_pressed() const {
		return this->m_key_pressed;
	}
	bool qsf::event_info::key_released() const {
		return this->m_key_released;
	}
	bool qsf::event_info::key_holding() const {
		return this->m_key_holding;
	}

	bool qsf::event_info::resized() const {
		return this->m_resized;
	}
	bool qsf::event_info::window_closed() const {
		return this->m_window_closed;
	}

	bool qsf::event_info::holding_left_mouse() const {
		return this->m_holding_left_mouse;
	}
	bool qsf::event_info::holding_right_mouse() const {
		return this->m_holding_right_mouse;
	}
	bool qsf::event_info::holding_middle_mouse() const {
		return this->m_holding_middle_mouse;
	}
	bool qsf::event_info::holding_key() const {
		return this->m_holding_key;
	}

	qsf::vector2i qsf::event_info::resized_size() const {
		return this->m_resized_size;
	}

	qsf::vector2i qsf::event_info::mouse_position() const {
		return this->m_mouse_position;
	}
	qsf::vector2i qsf::event_info::mouse_position_desktop() const {
		return this->m_mouse_position_desktop;
	}

}

#endif