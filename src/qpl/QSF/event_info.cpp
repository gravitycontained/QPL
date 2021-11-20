#include <qpl/QSF/event_info.hpp>

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)

namespace qsf {

	bool qsf::event_info::key_single_pressed(sf::Keyboard::Key key) const {
		return this->m_keys_single_pressed.find(key) != this->m_keys_single_pressed.cend();
	}
	bool qsf::event_info::key_single_released(sf::Keyboard::Key key) const {
		return this->m_keys_single_released.find(key) != this->m_keys_single_released.cend();
	}
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
	bool qsf::event_info::key_single_pressed() const {
		return this->m_key_single_pressed;
	}
	bool qsf::event_info::key_released() const {
		return this->m_key_released;
	}
	bool qsf::event_info::key_single_released() const {
		return this->m_key_single_released;
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
	bool qsf::event_info::left_mouse_double_clicked() const {
		return this->m_left_mouse_double_click;
	}
	bool qsf::event_info::right_mouse_double_clicked() const {
		return this->m_right_mouse_double_click;
	}
	bool qsf::event_info::middle_mouse_double_clicked() const {
		return this->m_middle_mouse_double_click;
	}

	void qsf::event_info::reset(const sf::RenderWindow& window) {
		this->m_mouse_clicked = false;
		this->m_mouse_released = false;
		this->m_left_mouse_clicked = false;
		this->m_left_mouse_released = false;
		this->m_right_mouse_clicked = false;
		this->m_right_mouse_released = false;
		this->m_middle_mouse_clicked = false;
		this->m_middle_mouse_released = false;
		this->m_scrolled_up = false;
		this->m_scrolled_down = false;
		this->m_key_pressed = false;
		this->m_key_single_pressed = false;
		this->m_key_released = false;
		this->m_mouse_moved = false;
		this->m_key_holding = false;
		this->m_window_closed = false;
		this->m_resized = false;
		this->m_left_mouse_double_click = false;
		this->m_right_mouse_double_click = false;
		this->m_middle_mouse_double_click = false;

		this->m_keys_pressed.clear();
		this->m_keys_released.clear();
		this->m_keys_single_pressed.clear();

		this->m_text_entered.clear();

		this->m_mouse_position = sf::Mouse::getPosition(window);
		this->m_mouse_position_desktop = sf::Mouse::getPosition();
	}
	void qsf::event_info::update(const sf::Event& event) {
		if (event.type == sf::Event::TextEntered) {
			this->m_text_entered.push_back(event.text.unicode);
			this->m_text_entered_stream << (wchar_t)event.text.unicode;

		}
		if (event.type == sf::Event::MouseButtonPressed) {
			this->m_mouse_clicked = true;
			this->m_mouse_holding = true;
			if (event.mouseButton.button == sf::Mouse::Left) {
				this->m_left_mouse_clicked = true;
				this->m_holding_left_mouse = true;
				if (this->m_left_mouse_clock.is_running()) {
					if (this->m_left_mouse_clock.elapsed_f() < 0.2) {
						this->m_left_mouse_double_click = true;
					}
					else {
						this->m_left_mouse_clock.reset();
					}
				}
				else {
					this->m_left_mouse_clock.reset();
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right) {
				this->m_right_mouse_clicked = true;
				this->m_holding_right_mouse = true;
				if (this->m_right_mouse_clock.is_running()) {
					if (this->m_right_mouse_clock.elapsed_f() < 0.2) {
						this->m_right_mouse_double_click = true;
					}
					else {
						this->m_right_mouse_clock.reset();
					}
				}
				else {
					this->m_right_mouse_clock.reset();
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Middle) {
				this->m_middle_mouse_clicked = true;
				this->m_holding_middle_mouse = true;
				if (this->m_middle_mouse_clock.is_running()) {
					if (this->m_middle_mouse_clock.elapsed_f() < 0.2) {
						this->m_middle_mouse_double_click = true;
					}
					else {
						this->m_middle_mouse_clock.reset();
					}
				}
				else {
					this->m_middle_mouse_clock.reset();
				}
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			this->m_mouse_released = true;
			this->m_mouse_holding = false;
			if (event.mouseButton.button == sf::Mouse::Left) {
				this->m_left_mouse_released = true;
				this->m_holding_left_mouse = false;
			}
			else if (event.mouseButton.button == sf::Mouse::Right) {
				this->m_right_mouse_released = true;
				this->m_holding_right_mouse = false;
			}
			else if (event.mouseButton.button == sf::Mouse::Middle) {
				this->m_middle_mouse_released = true;
				this->m_holding_middle_mouse = false;
			}
		}
		else if (event.type == sf::Event::KeyPressed) {
			this->m_key_pressed = true;
			this->m_key_holding = true;
			this->m_keys_pressed.insert(event.key.code);
			if (!this->key_holding(event.key.code)) {
				this->m_keys_single_pressed.insert(event.key.code);
				this->m_key_single_pressed = true;
			}
			this->m_keys_holding.insert(event.key.code);

		}
		else if (event.type == sf::Event::KeyReleased) {
			this->m_key_released = true;
			this->m_key_holding = false;
			this->m_keys_released.insert(event.key.code);
			this->m_keys_holding.erase(event.key.code);
		}
		else if (event.type == sf::Event::MouseWheelMoved) {
			if (event.mouseWheel.delta < 0) {
				this->m_scrolled_down = true;
			}
			if (event.mouseWheel.delta > 0) {
				this->m_scrolled_up = true;
			}
		}
		else if (event.type == sf::Event::Closed) {
			this->m_window_closed = true;
		}
		else if (event.type == sf::Event::MouseMoved) {
			this->m_mouse_moved = true;
		}
		else if (event.type == sf::Event::Resized) {
			this->m_resized = true;
			this->m_resized_size = { event.size.width, event.size.height };
		}
	}

	qpl::vector2i qsf::event_info::resized_size() const {
		return this->m_resized_size;
	}
	bool qsf::event_info::text_entered(char c) const {
		auto str = this->text_entered_str();
		for (auto& i : str) {
			if (i == c) {
				return true;
			}
		}
		return false;
	}
	bool qsf::event_info::text_entered(wchar_t c) const {
		for (auto& i : this->m_text_entered) {
			if (i == c) {
				return true;
			}
		}
		return false;
	}
	bool qsf::event_info::text_entered(std::string c) const {
		for (auto& i : c) {
			if (!this->text_entered(i)) {
				return false;
			}
		}
		return true;
	}
	bool qsf::event_info::text_entered(std::wstring c) const {
		for (auto& i : c) {
			if (!this->text_entered(i)) {
				return false;
			}
		}
		return true;
	}


	qpl::vector2i qsf::event_info::mouse_position() const {
		return this->m_mouse_position;
	}
	qpl::vector2i qsf::event_info::mouse_position_desktop() const {
		return this->m_mouse_position_desktop;
	}
	std::wstring qsf::event_info::text_entered() const {
		return this->m_text_entered;
	}
	std::string qsf::event_info::text_entered_str() const {
		return qpl::wstring_to_string(this->m_text_entered);
	}
	std::wstring qsf::event_info::all_text_entered() const {
		return this->m_text_entered_stream.str();
	}
	std::string qsf::event_info::all_text_entered_str() const {
		return qpl::wstring_to_string(this->m_text_entered_stream.str());
	}

}

#endif