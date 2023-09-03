#include <qpl/QSF/ui_controls.hpp>

#if defined QPL_INTERN_SFML_USE

#include <qpl/QSF/utility.hpp>

namespace qsf {
	namespace detail {
		qsf::button qsf::detail::button;
	}

	qpl::vector2f qsf::vbutton::get_position() const {
		return this->background.position;
	}
	void qsf::vbutton::set_dimension(qpl::vector2f dimension) {
		this->background.dimension = dimension;
	}
	void qsf::vbutton::set_position(qpl::vector2f position) {
		this->background.position = position;
	}
	void qsf::vbutton::set_center(qpl::vector2f center) {
		this->background.set_center(center);
	}
	void qsf::vbutton::set_background_color(qpl::rgba color) {
		this->background.set_color(color);
		this->background_color = color;
	}
	void qsf::vbutton::set_hover_background_color(qpl::rgba color) {
		this->hover_background_color = color;
	}
	void qsf::vbutton::set_text_color(qpl::rgba color) {
		this->text.set_color(color);
		this->text_color = color;
	}
	void qsf::vbutton::set_hover_text_color(qpl::rgba color) {
		this->hover_text_color = color;
	}
	void qsf::vbutton::set_text_font(std::string font) {
		this->text.set_font(font);
	}
	void qsf::vbutton::set_text_character_size(qpl::u32 character_size) {
		this->text.set_character_size(character_size);
	}
	void qsf::vbutton::set_text_style(qpl::u32 character_style) {
		this->text.set_style(character_style);
	}
	void qsf::vbutton::set_text(std::string text) {
		this->text.set_string(text);
	}
	void qsf::vbutton::centerize_text() {
		this->text.set_position(this->background.get_center());
	}
	bool qsf::vbutton::is_hovering() const {
		return this->hovering;
	}
	bool qsf::vbutton::is_clicked() const {
		return this->clicked;
	}
	void qsf::vbutton::update(const event_info& event_info) {
		auto pos = event_info.mouse_position();

		auto new_hovering = this->background.contains(pos);
		if (new_hovering != this->hovering) {
			if (new_hovering) {
				if (this->hover_background_color.is_unset()) {
					this->background.set_color(this->background_color.inverted());
				}
				else {
					this->background.set_color(this->hover_background_color);
				}
				if (this->hover_text_color.is_unset()) {
					this->text.set_color(this->text_color.inverted());
				}
				else {
					this->text.set_color(this->hover_text_color);
				}
			}
			else {
				this->background.set_color(this->background_color);
				this->text.set_color(this->text_color);
			}
		}

		this->hovering = new_hovering;

		this->clicked = this->hovering && event_info.left_mouse_clicked();
	}

	void qsf::vbutton::set_text_alpha(qpl::u8 alpha) {
		this->text_alpha = alpha;
	}
	void qsf::vbutton::set_background_alpha(qpl::u8 alpha) {
		this->background_alpha = alpha;
	}

	void qsf::vbutton::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::button = *this;
		qsf::detail::button.draw(window, states);
	}
	qsf::vbutton& qsf::vbutton::operator=(const qsf::vbutton& button) {
		this->outline_on_hover = button.outline_on_hover;
		this->invert_on_hover = button.invert_on_hover;
		this->text_color = button.text_color;
		this->hover_background_color = button.hover_background_color;
		this->background_color = button.background_color;
		this->text = button.text;
		this->background = button.background;
		this->hovering = button.hovering;
		this->clicked = button.clicked;
		this->background_alpha = button.background_alpha;
		this->text_alpha = button.text_alpha;
		return *this;
	}

	void qsf::button::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->background.draw(window, states);
		this->text.draw(window, states);
	}
	qsf::button& qsf::button::operator=(const qsf::vbutton& button) {
		this->background = button.background;
		this->background.set_color(button.background.get_color());

		this->text = button.text;
		this->text.set_color(button.text.color);

		this->text.set_center(this->background.center());
		return *this;
	}

	void qsf::button::set_dimension(qpl::vector2f dimension) {
		this->background.set_dimension(dimension);
	}
	void qsf::button::set_position(qpl::vector2f position) {
		this->background.set_position(position);
	}
	void qsf::button::set_center(qpl::vector2f center) {
		this->background.set_center(center);
	}
	void qsf::button::set_background_color(qpl::rgba color) {
		this->background.set_color(color);
		this->background_color = color;
	}
	void qsf::button::set_hover_background_color(qpl::rgba color) {
		this->hover_background_color = color;
	}
	void qsf::button::set_text_color(qpl::rgba color) {
		this->text.set_color(color);
		this->text_color = color;
	}
	void qsf::button::set_text_font(std::string font) {
		this->text.set_font(font);
	}
	void qsf::button::set_text_character_size(qpl::u32 character_size) {
		this->text.set_character_size(character_size);
	}
	void qsf::button::set_text_style(qpl::u32 character_style) {
		this->text.set_style(character_style);
	}
	void qsf::button::set_text_string(std::string text) {
		this->text.set_string(text);
	}
	void qsf::button::set_text_string(std::wstring text) {
		this->text.set_string(text);
	}

	qpl::vector2f qsf::button::get_dimension() const {
		return this->background.get_dimension();
	}
	qpl::vector2f qsf::button::get_position() const {
		return this->background.get_position();
	}
	qpl::vector2f qsf::button::get_center() const {
		return this->background.get_center();
	}
	qpl::rgba qsf::button::get_background_color() const {
		return this->background_color;
	}
	qpl::rgba qsf::button::get_hover_background_color() const {
		return this->hover_background_color;
	}
	qpl::rgba qsf::button::get_text_color() const {
		return this->text_color;
	}
	std::string qsf::button::get_text_font() const {
		return this->text.get_font();
	}
	qpl::u32 qsf::button::get_text_character_size() const {
		return this->text.get_character_size();
	}
	qpl::u32 qsf::button::get_text_style() const {
		return this->text.get_style();
	}
	std::string qsf::button::get_text_string() const {
		return this->text.get_string();
	}
	std::wstring qsf::button::get_text_wstring() const {
		return this->text.get_wstring();
	}
	void qsf::button::centerize_text() {
		this->text.set_center(this->background.center());
	}
	bool qsf::button::is_hovering() const {
		return this->hovering;
	}
	bool qsf::button::is_clicked() const {
		return this->clicked;
	}
	void qsf::button::update(const event_info& event_info) {
		auto pos = event_info.mouse_position();

		auto new_hovering = this->background.contains(pos);
		if (new_hovering != this->hovering) {
			if (this->invert_on_hover) {
				if (new_hovering) {
					this->background.set_color(this->background_color.inverted());
					this->text.set_color(this->text_color.inverted());
				}
				else {
					this->background.set_color(this->background_color);
					this->text.set_color(this->text_color);
				}
			}
			else {
				if (new_hovering) {
					this->background.set_color(this->hover_background_color);
				}
				else {
					this->background.set_color(this->background_color);
				}
			}
		}

		this->hovering = new_hovering;

		this->clicked = this->hovering && event_info.left_mouse_clicked();
	}
	void qsf::button::update(const event_info& event_info, bool& hovering) {
		this->update(event_info);
		hovering = hovering || this->hovering;
	}

	void qsf::smooth_button::enable_simple_hitbox() {
		this->simple_hitbox = true;
	}
	void qsf::smooth_button::disable_simple_hitbox() {
		this->simple_hitbox = false;
	}
	void qsf::smooth_button::set_hitbox_increase(qpl::vector2f delta) {
		this->hitbox_increase = delta;
	}
	void qsf::smooth_button::set_hitbox_increase(qpl::f32 delta) {
		this->hitbox_increase = qpl::vec(delta, delta);
	}
	void qsf::smooth_button::set_color(qpl::rgba color) {
		this->text.set_color(color);
		this->smooth_layout.set_color(color);
		this->rectangle.polygon.set_color(color);
	}
	void qsf::smooth_button::set_dimension(qpl::vector2f dimension) {
		this->smooth_layout.set_dimension(dimension);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_position(qpl::vector2f position) {
		this->smooth_layout.set_position(position);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_hitbox(qpl::hitbox hitbox) {
		this->smooth_layout.set_hitbox(hitbox);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_center(qpl::vector2f center) {
		this->smooth_layout.set_center(center);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_slope(qpl::f64 slope) {
		this->smooth_layout.set_slope(slope);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_color(qpl::rgba color) {
		this->background_color = color;
		this->smooth_layout.set_color(color);
		this->rectangle.set_color(color);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_outline_thickness(qpl::f32 thickness) {
		this->smooth_layout.set_outline_thickness(thickness);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_outline_color(qpl::rgba color) {
		this->background_outline_color = color;
		this->smooth_layout.set_outline_color(color);
		this->rectangle.set_outline_color(color);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_slope_dimension(qpl::vector2f dimension) {
		this->smooth_layout.set_slope_dimension(dimension);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_slope_point_count(qpl::size point_count) {
		this->smooth_layout.set_slope_point_count(point_count);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_text_font(const sf::Font& font) {
		this->text.set_font(font);
	}
	void qsf::smooth_button::set_text_font(const std::string& font_name) {
		this->text.set_font(font_name);
	}
	void qsf::smooth_button::set_text_style(qpl::u32 style) {
		this->text.set_style(style);
	}
	void qsf::smooth_button::set_text_character_size(qpl::u32 character_size) {
		this->text.set_character_size(character_size);
	}
	void qsf::smooth_button::set_text_color(qpl::rgba color) {
		this->text_color = color;
		this->text.set_color(color);
	}
	void qsf::smooth_button::set_text_outline_thickness(qpl::f32 outline_thickness) {
		this->text.set_outline_thickness(outline_thickness);
	}
	void qsf::smooth_button::set_text_outline_color(qpl::rgba color) {
		this->text.set_outline_color(color);
	}
	void qsf::smooth_button::set_text_rotation(qpl::f32 angle) {
		this->text.set_rotation(angle);
	}
	void qsf::smooth_button::set_text_letter_spacing(qpl::f32 spacing) {
		this->text.set_letter_spacing(spacing);
	}
	void qsf::smooth_button::set_text_position(qpl::vector2f position) {
		this->text.set_position(position);
	}
	void qsf::smooth_button::set_text_center(qpl::vector2f position) {
		this->text.set_center(position);
	}
	void qsf::smooth_button::set_text_string(const std::string& string) {
		this->text.set_string(string);
	}
	void qsf::smooth_button::set_text_string(const std::wstring& string) {
		this->text.set_string(string);
	}
	void qsf::smooth_button::centerize_text() {
		this->text.set_center(this->smooth_layout.get_center());
	}

	qpl::vector2f qsf::smooth_button::get_hitbox_increase() const {
		return this->hitbox_increase;
	}
	qpl::vector2f qsf::smooth_button::get_dimension() const {
		return this->smooth_layout.get_dimension();
	}
	qpl::vector2f qsf::smooth_button::get_position() const {
		return this->smooth_layout.get_position();
	}
	qpl::vector2f qsf::smooth_button::get_center() const {
		return this->smooth_layout.get_center();
	}
	qpl::hitbox qsf::smooth_button::get_hitbox() const {
		return this->smooth_layout.get_hitbox();
	}
	qpl::f64 qsf::smooth_button::get_slope() const {
		return this->smooth_layout.get_slope();
	}
	qpl::rgba qsf::smooth_button::get_background_color() const {
		return this->smooth_layout.get_color();
	}
	qpl::f32 qsf::smooth_button::get_background_outline_thickness() const {
		return this->smooth_layout.get_outline_thickness();
	}
	qpl::rgba qsf::smooth_button::get_background_outline_color() const {
		return this->smooth_layout.get_outline_color();
	}
	qpl::vector2f qsf::smooth_button::get_slope_dimension() const {
		return this->smooth_layout.get_slope_dimension();
	}
	qpl::size qsf::smooth_button::get_slope_point_count() const {
		return this->smooth_layout.get_slope_point_count();
	}
	std::string qsf::smooth_button::get_text_font() const {
		return this->text.get_font();
	}
	qpl::u32 qsf::smooth_button::get_text_style() const {
		return this->text.get_style();
	}
	qpl::u32 qsf::smooth_button::get_text_character_size() const {
		return this->text.get_character_size();
	}
	qpl::rgba qsf::smooth_button::get_text_color() const {
		return this->text.get_color();
	}
	qpl::f32 qsf::smooth_button::get_text_outline_thickness() const {
		return this->text.get_outline_thickness();
	}
	qpl::rgba qsf::smooth_button::get_text_outline_color() const {
		return this->text.get_outline_color();
	}
	qpl::f32 qsf::smooth_button::get_text_letter_spacing() const {
		return this->text.get_letter_spacing();
	}
	qpl::vector2f qsf::smooth_button::get_text_position() const {
		return this->text.get_visible_hitbox().position;
	}
	qpl::vector2f qsf::smooth_button::get_text_center() const {
		return this->text.get_visible_hitbox().get_center();
	}
	std::string qsf::smooth_button::get_text_string() const {
		return this->text.get_string();
	}
	std::wstring qsf::smooth_button::get_text_wstring() const {
		return this->text.get_wstring();
	}
	bool qsf::smooth_button::is_hovering() const {
		return this->hovering;
	}
	bool qsf::smooth_button::is_clicked() const {
		return this->clicked;
	}
	void qsf::smooth_button::update(const qsf::event_info& event) {
		this->create_check();
		if (this->simple_hitbox) {
			this->hovering = this->rectangle.get_hitbox().increased(5).contains(event.mouse_position());
		}
		else {
			this->hovering = this->rectangle.contains(event.mouse_position());
		}
		this->clicked = this->hovering && event.left_mouse_clicked();

		if (this->clicked) {
			this->clicked_once = true;
			this->click_timer.reset();
		}

		qpl::rgba text_color_effect = this->text_color;
		if (this->use_basic_hover_animation) {
			if (this->hovering && !this->hover_before) {
				this->hover_animation.go_forwards();
			}
			else if (!this->hovering && this->hover_before) {
				this->hover_animation.go_backwards();
			}
			this->hover_before = this->hovering;

			this->hover_animation.update(event.frame_time_f());
			if (this->hover_animation.is_running()) {
				auto progress = this->hover_animation.get_curve_progress();

				auto color = this->background_color;
				color.interpolate(this->background_color_hover, progress);
				this->smooth_layout.set_color(color);

				color = this->background_outline_color;
				color.interpolate(color.inverted(), progress);
				this->smooth_layout.set_outline_color(color);

				color = this->text_color;
				color.interpolate(this->text_color_hover, progress);

				text_color_effect = color;
				this->text.set_color(color);
				this->layout_changed = true;
			}
		}


		if (this->clicked_once && this->click_timer.elapsed_f() < 1.5) {
			auto progress = qpl::curve_slope(this->click_timer.elapsed_f() / 1.5, 1.0);
			this->text.set_color(text_color_effect.interpolated(this->text_color_click, 1.0 - progress));
		}

	}
	void qsf::smooth_button::create_check() const {
		if (this->layout_changed) {
			this->rectangle = this->smooth_layout;
			this->layout_changed = false;
		}
	}
	void qsf::smooth_button::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->create_check();
		this->rectangle.draw(window, states);
		this->text.draw(window, states);
	}


	void qsf::text_field::line::apply(qsf::vtext layout) {
		this->text.set_font(layout.font_name);
		this->text.set_character_size(layout.character_size);
		this->text.set_color(layout.color);
		this->text.set_outline_color(layout.outline_color);
		this->text.set_outline_thickness(layout.outline_thickness);
		this->text.set_style(layout.style);
	}
	void qsf::text_field::line::calculate_hitboxes() {
		this->character_hitboxes = this->text.get_all_characters_hitbox_whitespace_included();
		if (this->character_hitboxes.empty()) {
			this->character_hitboxes.push_back({});

			this->character_hitboxes.back().position = this->text.get_starting_line_position() + qpl::vec(this->text.get_character_size() / 15.0f, 0);
			this->character_hitboxes.back().dimension = this->text.get_character_dimension(L'x');
		}
		this->line_hitbox.position = this->text.get_position();
		this->line_hitbox.position.x = this->text.get_starting_line_position().x;
		this->line_hitbox.dimension.y = this->text.get_line_spacing();
		this->line_hitbox.dimension.x = ((this->character_hitboxes.back().dimension + this->character_hitboxes.back().position) - this->line_hitbox.position).x;
	}
	void qsf::text_field::line::calculate_mouse_hitboxes(qpl::f32 max_line_x, qpl::f32 extended_line_width) {
		this->character_mouse_hitboxes.resize(this->character_hitboxes.size() + 1u);

		qpl::f32 y = this->text.get_starting_line_position().y - this->text.get_delta_underline();
		qpl::f32 height = this->text.get_line_spacing();

		qpl::f32 first_x = this->line_hitbox.dimension.x + this->line_hitbox.position.x;
		qpl::f32 last_x = first_x;
		for (qpl::size i = 0u; i < this->character_hitboxes.size() - 1; ++i) {
			qpl::f32 x = this->character_hitboxes[i].get_center().x;
			qpl::f32 width = this->character_hitboxes[i + 1].get_center().x - x;

			if (i == 0u) {
				first_x = x;
			}
			last_x = x + width;

			qpl::hitbox hitbox;
			hitbox.position = qpl::vec(x, y);
			hitbox.dimension = qpl::vec(width, height);
			this->character_mouse_hitboxes[i + 1] = hitbox;
		}

		auto& front = this->character_mouse_hitboxes.front();
		auto& back = this->character_mouse_hitboxes.back();


		qpl::hitbox hitbox;
		hitbox.position = qpl::vec(this->line_hitbox.position.x, y);
		hitbox.dimension = qpl::vec(first_x - this->line_hitbox.position.x, height);
		hitbox.extend_left(extended_line_width);
		front = hitbox;
		hitbox.position = qpl::vec(last_x, y);
		hitbox.dimension = qpl::vec(max_line_x - last_x, height);
		hitbox.extend_right(extended_line_width);
		back = hitbox;

		this->line_mouse_hitbox.position = front.position;
		this->line_mouse_hitbox.dimension = (back.position + back.dimension) - front.position;
	}
	void qsf::text_field::line::move(qpl::vector2f delta) {
		this->text.move(delta);
		this->line_hitbox.move(delta);
		this->line_mouse_hitbox.move(delta);
		for (auto& i : this->character_hitboxes) {
			i.move(delta);
		}
		for (auto& i : this->character_mouse_hitboxes) {
			i.move(delta);
		}
	}
	void qsf::text_field::line::move(qpl::f32 x, qpl::f32 y) {
		this->move(qpl::vec(x, y));
	}
	std::wstring qsf::text_field::line::wstring() const {
		return this->text.get_wstring();
	}
	void qsf::text_field::line::draw(qsf::draw_object& draw) const {

		//qsf::rectangle rect;
		//for (qpl::size i = 0u; i < this->character_mouse_hitboxes.size(); ++i) {
		//	qpl::rgba color = qpl::rgba::yellow;
		//	if (i % 2 == 0u) {
		//		color = qpl::rgba::cyan;
		//	}
		//	rect.set_hitbox(this->character_mouse_hitboxes[i]);
		//	rect.set_color(color.with_alpha(50));
		//	draw.draw(rect);
		//}

		draw.draw(this->text);
	}


	qsf::text_field::text_field() {
		this->set_dimension(qpl::vec(300, 50));
		this->set_background_color(qpl::rgba::grey_shade(30));
		this->set_background_outline_color(qpl::rgba::grey_shade(220));
		this->set_background_outline_thickness(1.0f);
		this->reset();
	}
	void qsf::text_field::reset() {
		this->lines.resize(1u);
		this->set_string_stack_size(10u);
		for (auto& i : this->lines) {
			i.layout_changed = true;
		}
	}

	void qsf::text_field::set_font(std::string font) {
		this->text_layout.set_font(font);
		this->text_layout_changed = true;
		for (auto& line : this->lines) {
			line.layout_changed = true;
		}
		if (this->lines.size() == 1u) {
			if (this->lines.front().wstring().empty()) {
				this->set_string(L"");
			}
		}
	}

	void qsf::text_field::set_text_character_size(qpl::u32 character_size) {
		this->text_layout.set_character_size(character_size);
		this->text_layout_changed = true;
		for (auto& line : this->lines) {
			line.layout_changed = true;
		}
	}
	void qsf::text_field::set_text_style(qpl::u32 style) {
		this->text_layout.set_style(style);
		this->text_layout_changed = true;
		for (auto& line : this->lines) {
			line.layout_changed = true;
		}
	}
	void qsf::text_field::set_text_outline_thickness(qpl::f32 outline_thickness) {
		this->text_layout.set_outline_thickness(outline_thickness);
		this->text_layout_changed = true;
		for (auto& line : this->lines) {
			line.layout_changed = true;
		}
	}
	void qsf::text_field::set_text_outline_color(qpl::rgba outline_color) {
		this->text_layout.set_outline_color(outline_color);
		this->text_layout_changed = true;
		for (auto& line : this->lines) {
			line.layout_changed = true;
		}
	}
	void qsf::text_field::set_text_color(qpl::rgba color) {
		this->text_layout.set_color(color);
		this->text_layout_changed = true;
		for (auto& line : this->lines) {
			line.layout_changed = true;
		}
	}
	qpl::hitbox qsf::text_field::get_hitbox() const {
		this->internal_update();
		qpl::hitbox result;
		result.position = this->lines.front().line_hitbox.position;

		qpl::f32 x = 0.0f;
		for (qpl::size i = 0u; i < this->lines.size(); ++i) {
			x = qpl::max(x, this->lines[i].line_hitbox.dimension.x);
		}

		auto y = (this->lines.back().line_hitbox.position.y + this->lines.back().text.get_line_height()) - result.position.y;
		result.dimension = qpl::vec(x, y);
		return result;
	}
	qpl::hitbox qsf::text_field::get_background_hitbox() const {
		return this->background.get_hitbox();
	}
	void qsf::text_field::set_background_increase(qpl::vector2f increase) {
		this->background_increase = increase;
	}
	void qsf::text_field::set_background_increase(qpl::f32 increase) {
		this->background_increase = qpl::vec(increase, increase);
	}

	void qsf::text_field::set_background_outline_thickness(qpl::f32 thickness) {
		this->background.set_outline_thickness(thickness);
	}
	void qsf::text_field::set_background_outline_color(qpl::rgba color) {
		this->background.set_outline_color(color);
	}
	qpl::rgba qsf::text_field::get_background_color() const {
		return this->background.get_color();
	}
	qpl::rgba qsf::text_field::get_background_outline_color() const {
		return this->background.get_outline_color();
	}
	qpl::f32 qsf::text_field::get_background_outline_thickness() const {
		return this->background.get_outline_thickness();
	}
	void qsf::text_field::set_background_color(qpl::rgba color) {
		this->background.set_color(color);
	}
	qpl::vector2f qsf::text_field::get_background_increase() const {
		return this->background_increase;
	}

	void qsf::text_field::set_minimum_dimension(qpl::vector2f dimension) {
		this->minimum_dimension = dimension;
		this->update_required = true;
	}
	void qsf::text_field::set_minimum_width(qpl::f32 width) {
		this->minimum_dimension.x = width;
		this->update_required = true;
	}
	void qsf::text_field::set_minimum_height(qpl::f32 height) {
		this->minimum_dimension.y = height;
		this->update_required = true;
	}
	void qsf::text_field::set_maximum_dimension(qpl::vector2f dimension) {
		this->maximum_dimension = dimension;
		this->update_required = true;
	}

	void qsf::text_field::set_maximum_width(qpl::f32 width) {
		this->minimum_dimension.x = width;
		this->update_required = true;
	}
	void qsf::text_field::set_maximum_height(qpl::f32 height) {
		this->minimum_dimension.y = height;
		this->update_required = true;
	}
	void qsf::text_field::set_fixed_dimension(qpl::vector2f dimension) {
		this->minimum_dimension = this->maximum_dimension = dimension;
		this->update_required = true;
	}

	void qsf::text_field::set_fixed_width(qpl::f32 width) {
		this->minimum_dimension.x = this->maximum_dimension.x = width;
		this->update_required = true;
	}
	void qsf::text_field::set_fixed_height(qpl::f32 height) {
		this->minimum_dimension.y = this->maximum_dimension.y = height;
		this->update_required = true;
	}
	void qsf::text_field::set_position(qpl::vector2f position) {
		auto delta = position - this->hitbox.position;
		this->move(delta);
	}
	void qsf::text_field::set_dimension(qpl::vector2f dimension) {
		this->hitbox.set_dimension(dimension);
		this->update_required = true;
	}
	void qsf::text_field::set_hitbox(qpl::hitbox hitbox) {
		this->set_dimension(hitbox.dimension);
		this->set_position(hitbox.position);
	}
	void qsf::text_field::move(qpl::vec2f delta) {
		this->hitbox.move(delta);

		for (auto& i : this->lines) {
			i.move(delta);
		}
		for (auto& i : this->selection_rectangles) {
			i.move(delta);
		}
		this->update_required = true;
		this->internal_update();
	}

	void qsf::text_field::set_string(std::wstring string) {
		this->internal_update_text();

		auto split = qpl::string_split_allow_empty(string, L'\n');

		qpl::size size = split.size();
		if (size == 0u) {
			this->lines.resize(1u);
			this->lines.front().apply(this->text_layout);
			this->lines.front().text.set_string(L"");
			this->lines.front().calculate_hitboxes();
			this->lines.front().mouse_hitboxes_changed = true;
			this->text_mouse_hitboxes_changed = true;
			return;
		}
		auto size_before = this->lines.size();
		this->lines.resize(split.size());
		for (qpl::size i = size_before; i < size; ++i) {
			this->lines[i].apply(this->text_layout);
		}

		auto pos = this->hitbox.position;
		for (qpl::size i = 0u; i < size; ++i) {
			auto diff = pos - this->lines[i].text.get_position();
			this->lines[i].move(diff);
			this->lines[i].text.set_string(split[i]);
			this->lines[i].calculate_hitboxes();
			this->lines[i].mouse_hitboxes_changed = true;
			this->text_mouse_hitboxes_changed = true;
			pos.y += this->get_line_height();
		}
		this->whole_string_changed = true;
		this->update_required = true;
	}
	void qsf::text_field::set_string(std::string string) {
		this->set_string(qpl::string_to_wstring(string));
	}

	qsf::text_field::line& qsf::text_field::get_line_at_cursor() {
		return this->lines[this->cursor_position.y];
	}
	const qsf::text_field::line& qsf::text_field::get_line_at_cursor() const {
		return this->lines[this->cursor_position.y];
	}

	std::wstring qsf::text_field::get_selection_wstring() const {
		auto [begin, end] = this->get_sorted_selection_range();
		if (begin == end) {
			return L"";
		}

		if (begin.y == end.y) {
			auto str = this->lines[begin.y].wstring();
			return str.substr(begin.x, end.x - begin.x);
		}
		else {
			std::wstring result;
			for (qpl::size y = begin.y; y <= end.y; ++y) {
				if (y == begin.y) {
					result += this->lines[y].wstring().substr(begin.x);
				}
				else if (y == end.y) {
					result += L"\n";
					result += this->lines[y].wstring().substr(0u, end.x);
				}
				else {
					result += L"\n";
					result += this->lines[y].wstring();
				}
			}
			return result;
		}
	}
	std::string qsf::text_field::get_selection_string() const {
		return qpl::wstring_to_string(this->get_selection_wstring());
	}
	void qsf::text_field::add_string_at_cursor(std::wstring string) {
		auto split = qpl::string_split_allow_empty(string, L'\n');

		for (qpl::size i = 0u; i < split.size(); ++i) {
			if (i) {
				this->new_line(this->cursor_position.y);
			}

			if (split[i].size() && split[i].back() == L'\r') {
				split[i].pop_back();
			}

			auto& line = this->get_line_at_cursor();
			auto str = line.text.get_wstring();
			std::wstring new_string;
			new_string += str.substr(0u, this->cursor_position.x);
			new_string += split[i];
			if (this->cursor_position.x < str.length()) {
				new_string += str.substr(this->cursor_position.x);
			}
			line.text.set_string(new_string);
			line.calculate_hitboxes();
			line.mouse_hitboxes_changed = true;

			this->cursor_position.x += split[i].length();
		}

		this->whole_string_changed = true;
		this->text_mouse_hitboxes_changed = true;
		this->update_required = true;
		this->internal_update();
		this->cursor_position_changed = true;
		this->cursor_interval_timer.reset();
	}

	void qsf::text_field::enable_focus() {
		this->focus = true;
	}
	void qsf::text_field::disable_focus() {
		this->focus = false;
	}
	void qsf::text_field::enable_one_line_limit() {
		this->line_limit = 1u;
	}
	void qsf::text_field::set_line_limit(qpl::size limit) {
		this->line_limit = limit;
	}
	void qsf::text_field::disable_one_line_limit() {
		this->line_limit = qpl::size_max;
	}

	void qsf::text_field::set_string_stack_size(qpl::size size) {
		this->string_stack.resize(size);
		this->string_stack.front().first = this->wstring();
		this->string_stack.front().second = this->cursor_position;
	}
	qpl::size qsf::text_field::get_string_stack_size() const {
		return this->string_stack.size();
	}
	qpl::f32 qsf::text_field::get_line_height() const {
		return this->lines.front().text.get_line_height();
	}

	bool qsf::text_field::is_hovering() const {
		return this->hovering;
	}

	bool qsf::text_field::just_edited_text() const {
		return this->edited_text;
	}
	bool qsf::text_field::just_copied_text() const {
		return this->copied_text;
	}
	bool qsf::text_field::just_pasted_text() const {
		return this->pasted_text;
	}
	bool qsf::text_field::just_entered_text() const {
		return this->entered_text;
	}
	bool qsf::text_field::just_finished_text() const {
		return this->finished_text;
	}
	bool qsf::text_field::has_focus() const {
		return this->focus;
	}

	std::wstring qsf::text_field::wstring() const {
		if (this->whole_string_changed) {
			this->whole_string = L"";
			for (qpl::size i = 0u; i < this->lines.size(); ++i) {
				if (i) {
					this->whole_string += L"\n";
				}
				this->whole_string += this->lines[i].wstring();
			}
			this->whole_string_changed = false;
		}
		return this->whole_string;
	}
	std::string qsf::text_field::string() const {
		return qpl::wstring_to_string(this->wstring());
	}

	std::string qsf::text_field::get_font() const {
		return this->text_layout.font_name;
	}
	bool qsf::text_field::is_font_set() const {
		return !this->text_layout.font_name.empty();
	}
	void qsf::text_field::new_line(qpl::size y) {
		if (y == qpl::size_max) {
			y = this->lines.size() - 1;
		}

		auto pos = this->hitbox.position;
		pos.y += this->get_line_height() * (y + 1);

		auto current_str = this->lines[y].wstring();
		this->lines[y].text.set_string(current_str.substr(0u, this->cursor_position.x));
		this->lines[y].calculate_hitboxes();

		qsf::text_field::line line;
		line.apply(this->text_layout);
		line.text.set_string(current_str.substr(this->cursor_position.x));
		line.text.set_position(pos);
		line.calculate_hitboxes();
		line.mouse_hitboxes_changed = true;
		this->text_mouse_hitboxes_changed = true;
		this->whole_string_changed = true;

		this->lines.push_back({});

		for (qpl::isize i = qpl::signed_cast(this->lines.size()) - 1; i > qpl::signed_cast(y + 1); --i) {
			this->lines[i] = this->lines[i - 1];

			this->lines[i].move(0.0, this->get_line_height());
		}
		this->lines[y + 1] = line;

		this->cursor_position.x = 0u;
		++this->cursor_position.y;

		this->update_required = true;
		this->internal_update();

	}

	bool qsf::text_field::is_dragging_selection_considering_cooldown() const {
		return this->dragging_selection && this->selection_start_drag_timer.elapsed_f() > this->selection_start_drag_timer_duration;
	}
	void qsf::text_field::delete_at_cursor() {

		auto& line = this->get_line_at_cursor();
		auto str = line.wstring();

		if (this->cursor_position.x == 0u) {
			if (this->cursor_position.y) {
				auto& line_before = this->lines[this->cursor_position.y - 1];

				auto new_cursor_x = line_before.wstring().length();

				auto new_string = line_before.wstring();
				new_string += str;


				line_before.text.set_string(new_string);
				line_before.calculate_hitboxes();
				line.mouse_hitboxes_changed = true;
				this->text_mouse_hitboxes_changed = true;
				this->whole_string_changed = true;

				for (qpl::size i = this->cursor_position.y; i < this->lines.size() - 1; ++i) {
					this->lines[i] = this->lines[i + 1];
					this->lines[i].move(0.0, -this->get_line_height());
				}
				this->lines.pop_back();

				--this->cursor_position.y;
				this->cursor_position.x = new_cursor_x;
			}
		}
		else {
			std::wstring new_string;
			new_string += str.substr(0u, this->cursor_position.x - 1);
			if (this->cursor_position.x != str.length()) {
				new_string += str.substr(this->cursor_position.x);
			}
			line.text.set_string(new_string);
			line.calculate_hitboxes();
			line.mouse_hitboxes_changed = true;
			this->text_mouse_hitboxes_changed = true;
			this->whole_string_changed = true;

			--this->cursor_position.x;

		}

		this->cursor_interval_timer.reset();
		this->update_required = true;
		this->internal_update();

	}
	void qsf::text_field::delete_selection_string(bool update_cursor_position) {
		auto [begin, end] = this->get_sorted_selection_range();

		if (begin == end) {
			return;
		}

		if (begin.y == end.y) {
			auto& line = this->lines[begin.y];
			auto str = line.wstring();
			line.text.set_string(str.substr(0u, begin.x) + str.substr(end.x));
			line.calculate_hitboxes();
			line.calculate_mouse_hitboxes(this->hitbox.dimension.x + this->hitbox.position.x, this->background_increase.x);
		}
		else {
			auto& first_line = this->lines[begin.y];
			auto& last_line = this->lines[end.y];

			auto first_str = first_line.wstring().substr(0u, begin.x);
			auto last_str = last_line.wstring().substr(end.x);


			first_line.text.set_string(first_str + last_str);
			first_line.calculate_hitboxes();
			first_line.calculate_mouse_hitboxes(this->hitbox.dimension.x + this->hitbox.position.x, this->background_increase.x);

			auto remove = end.y - begin.y;
			for (qpl::size i = 1u;; ++i) {
				auto index1 = i + begin.y;
				auto index2 = index1 + remove;
				if (index2 >= this->lines.size()) {
					break;
				}
				this->lines[index1] = this->lines[index2];
				this->lines[index1].move(0.0, -this->get_line_height() * remove);
			}
			this->lines.resize(this->lines.size() - remove);
		}
		this->update_required = true;
		this->text_mouse_hitboxes_changed = true;
		this->whole_string_changed = true;
		this->text_layout_changed = true;

		if (update_cursor_position) {
			bool larger = (this->cursor_position.y > end.y || (this->cursor_position.y == end.y && this->cursor_position.x > end.x));
			bool smaller = (this->cursor_position.y < begin.y || (this->cursor_position.y == begin.y && this->cursor_position.x < begin.x));

			if (larger && !smaller) {
				auto diff = qpl::vector2<qpl::isize>(this->cursor_position) - qpl::vector2<qpl::isize>(begin);
				if (this->cursor_position.y == end.y) {
					this->cursor_position.x = begin.x + (this->cursor_position.x - end.x);
				}
				this->cursor_position.y -= (end.y - begin.y);
			}
			else if (!smaller && !larger) {
				this->cursor_position = begin;
			}
			this->cursor_position_changed = true;
		}
		this->internal_update();
		this->delete_selection_range();
		this->make_selection_rectangles();
	}

	void qsf::text_field::delete_selection_range() {
		this->selection_a = this->selection_b = this->cursor_position;
	}
	void qsf::text_field::move_cursor_up() {
		if (!this->cursor_position.y) {
			return;
		}
		--this->cursor_position.y;
		this->find_closest_cursor_position();
		this->cursor_interval_timer.reset();
	}
	void qsf::text_field::move_cursor_down() {
		if (this->cursor_position.y == this->lines.size() - 1) {
			return;
		}

		++this->cursor_position.y;
		this->find_closest_cursor_position();
		this->cursor_interval_timer.reset();
	}
	void qsf::text_field::move_cursor_left() {
		if (this->cursor_position.x == 0u) {
			if (this->cursor_position.y) {
				--this->cursor_position.y;
				this->cursor_position.x = this->get_line_at_cursor().wstring().length();
			}
		}
		else {
			--this->cursor_position.x;
		}

		this->cursor_interval_timer.reset();
		this->cursor_position_changed = true;
	}
	void qsf::text_field::move_cursor_right() {
		const auto& line = this->get_line_at_cursor();
		if (this->cursor_position.x == line.wstring().length()) {
			if (this->cursor_position.y < this->lines.size() - 1) {
				++this->cursor_position.y;
				this->cursor_position.x = 0u;
			}
		}
		else {
			++this->cursor_position.x;
		}

		this->cursor_interval_timer.reset();
		this->cursor_position_changed = true;
	}

	void qsf::text_field::update_cursor() {
		auto elapsed = this->cursor_interval_timer.elapsed_f();
		auto delta = std::fmod(elapsed, this->cursor_interval_duration);

		qpl::rgba color = qpl::rgba::white();
		if (this->is_dragging_selection_considering_cooldown()) {
			color = qpl::rgb::yellow();
		}
		if (delta > this->cursor_blink_duration) {
			color.a = 0;
		}
		this->cursor.set_color(color);
	}
	void qsf::text_field::update_selection_rectangles() {
		if (this->is_dragging_selection_considering_cooldown() && !this->dragging_selection_before) {
			qpl::rgba color = this->dragging_color;
			color.a = this->selection_color.a;
			for (auto& i : this->selection_rectangles) {
				i.set_color(color);
			}
		}
		else if (!this->dragging_selection && this->dragging_selection_before) {
			qpl::rgba color = this->selection_color;
			for (auto& i : this->selection_rectangles) {
				i.set_color(color);
			}
		}
	}
	void qsf::text_field::update_mouse_events(const qsf::event_info& event) {
		auto max = qpl::vec2::max_values(this->hovering_increase, this->background_increase);
		this->hovering = this->hitbox.increased(max).contains(event.mouse_position());

		bool selection_hovering = false;
		for (auto& i : this->selection_rectangles) {
			if (i.contains(event.mouse_position())) {
				selection_hovering = true;
				break;
			}
		}
		if (event.left_mouse_clicked() && !selection_hovering) {
			if (this->hovering && !this->focus) {
				this->cursor_interval_timer.reset();
			}
			else if (!this->hovering && this->focus) {
				this->delete_selection_range();
				this->make_selection_rectangles();
				selection_hovering = false;
			}
			this->focus = this->hovering;
		}

		bool delayed_click = false;
		if (selection_hovering && this->focus && event.left_mouse_fast_clicked()) {
			bool reset = false;

			if (this->double_click_cooldown.is_running() && this->double_click_cooldown.elapsed_f() > 0.3) {
				reset = true;
			}
			if (reset) {
				this->delete_selection_range();
				this->make_selection_rectangles();
				selection_hovering = false;
				delayed_click = true;
			}
		}

		if (!this->focus) {
			this->dragging = false;
		}

		bool stop = false;

		if (this->hovering && (event.left_mouse_clicked() || delayed_click)) {
			if (selection_hovering) {
				if (this->selection_start_drag_timer.elapsed_f() > this->selection_start_drag_timer_duration) {
					this->selection_drag_timer.reset();
					this->dragging_selection = true;
				}
			}
			else {
				for (qpl::size y = 0u; !stop && y < this->lines.size(); ++y) {
					const auto& line = this->lines[y];
					if (line.line_mouse_hitbox.contains(event.mouse_position())) {
						for (qpl::size x = 0u; x < line.character_mouse_hitboxes.size(); ++x) {
							if (line.character_mouse_hitboxes[x].contains(event.mouse_position())) {
								this->cursor_position = qpl::vec(x, y);

								if (this->lines[y].wstring().empty()) {
									this->cursor_position.x = 0u;
								}

								this->delete_selection_range();
								this->selection_rectangles.clear();
								this->click_mouse_position = event.mouse_position();
								this->dragging = true;
								this->cursor_interval_timer.reset();
								this->cursor_position_changed = true;
								stop = true;
								break;
							}
						}
					}
				}
			}
		}

		if (event.left_mouse_clicked()) {
			this->selection_start_drag_timer.reset();
		}

		if ((this->dragging || this->dragging_selection) && event.mouse_moved()) {
			bool found = false;

			auto before = this->selection_b;

			for (qpl::size y = 0u; !found && y < this->lines.size(); ++y) {
				const auto& line = this->lines[y];
				if (line.line_mouse_hitbox.contains(event.mouse_position())) {
					for (qpl::size x = 0u; x < line.character_mouse_hitboxes.size(); ++x) {
						if (line.character_mouse_hitboxes[x].contains(event.mouse_position())) {

							this->cursor_position = qpl::vec(x, y);

							if (line.wstring().empty()) {
								this->cursor_position.x = 0u;
							}

							if (!this->dragging_selection) {
								this->selection_b = this->cursor_position;
							}
							this->cursor_interval_timer.reset();
							this->cursor_position_changed = true;
							found = true;
							break;
						}
					}
				}
			}

			if (!found) {
				if (event.mouse_position().x > this->hitbox.get_bottom_right().x || event.mouse_position().y > this->hitbox.get_bottom_right().y) {
					if (!this->dragging_selection) {
						this->selection_b.y = this->lines.size() - 1;
						this->selection_b.x = this->lines.back().wstring().length();
					}
					this->cursor_position = this->selection_b;
					this->cursor_interval_timer.reset();
					this->cursor_position_changed = true;
				}
				else if (event.mouse_position().x < this->hitbox.get_top_left().x || event.mouse_position().y < this->hitbox.get_top_left().y) {
					if (!this->dragging_selection) {
						this->selection_b = qpl::vec(0u, 0u);
					}
					this->cursor_position = this->selection_b;
					this->cursor_interval_timer.reset();
					this->cursor_position_changed = true;
				}
			}

			if (this->selection_b != before) {
				this->make_selection_rectangles();
			}
		}

		if (event.left_mouse_released()) {
			this->dragging = false;

			if (this->is_dragging_selection_considering_cooldown() && !selection_hovering) {
				auto str = this->get_selection_wstring();

				this->delete_selection_string();
				this->delete_selection_range();
				this->make_selection_rectangles();
				this->edited_text = true;
				this->update_required = true;
				this->text_mouse_hitboxes_changed = true;
				this->whole_string_changed = true;
				this->text_layout_changed = true;
				this->internal_update();
				this->add_string_at_cursor(str);
			}
			this->dragging_selection = false;
		}

		if (!this->focus) {
			return;
		}

		auto fast_click = event.left_mouse_fast_click_count();

		if (fast_click == 2u) {
			this->double_click_cooldown.reset();
			auto& line = this->get_line_at_cursor();

			if (!line.wstring().empty()) {

				wchar_t c = line.wstring().back();
				if (this->cursor_position.x < line.wstring().length()) {
					c = line.wstring()[this->cursor_position.x];
				}

				bool is_special = !(std::iswalpha(c) || std::iswalnum(c) || (c == L'_'));
				bool is_space = std::iswspace(c);

				qpl::isize begin = this->cursor_position.x;
				qpl::isize end = this->cursor_position.x;
				while (begin > 0) {
					auto c = line.wstring()[begin - 1];
					bool allowed;
					if (is_space) {
						allowed = std::iswspace(c);
					}
					else if (is_special) {
						allowed = !(std::iswalpha(c) || std::iswalnum(c) || (c == L'_') || std::iswspace(c));
					}
					else {
						allowed = std::iswalpha(c) || std::iswalnum(c) || (c == L'_');
					}

					if (!allowed) {
						break;
					}
					--begin;
				}
				while (end < qpl::isize_cast(line.wstring().length())) {
					auto c = line.wstring()[end];

					bool allowed;
					if (is_space) {
						allowed = std::iswspace(c);
					}
					else if (is_special) {
						allowed = !(std::iswalpha(c) || std::iswalnum(c) || (c == L'_') || std::iswspace(c));
					}
					else {
						allowed = std::iswalpha(c) || std::iswalnum(c) || (c == L'_');
					}
					if (!allowed) {
						break;
					}
					++end;
				}

				this->selection_a.y = this->selection_b.y = this->cursor_position.y;
				this->selection_a.x = qpl::size_cast(begin);
				this->selection_b.x = qpl::size_cast(end);
				this->cursor_position.x = qpl::size_cast(end);

				this->cursor_position_changed = true;
				this->make_selection_rectangles();
			}
		}
		else if (fast_click == 3u) {
			this->selection_a.y = this->selection_b.y = this->cursor_position.y;
			this->selection_a.x = 0u;
			this->selection_b.x = this->get_line_at_cursor().wstring().length();
			this->cursor_position.x = this->selection_b.x;
			if (this->selection_b.y != this->lines.size() - 1) {
				++this->selection_b.y;
				this->selection_b.x = 0u;
			}

			this->cursor_position_changed = true;
			this->make_selection_rectangles();
		}
	}
	void qsf::text_field::update(const qsf::event_info& event) {
		auto cursor_before = this->cursor_position;
		auto string_before = this->wstring();

		this->update_cursor();
		this->internal_update();

		bool special_input = false;
		this->edited_text = false;
		this->copied_text = false;
		this->pasted_text = false;
		this->entered_text = false;
		this->finished_text = false;
		this->control_z = false;
		this->control_y = false;
		this->changed = false;
		this->pressed_enter_key = false;

		this->update_mouse_events(event);

		if (!this->focus) {
			return;
		}

		if (event.key_pressed(sf::Keyboard::Backspace)) {
			if (this->selection_a != this->selection_b) {
				this->delete_selection_string();
			}
			else {
				this->delete_at_cursor();
			}
			special_input = true;
			this->edited_text = true;
			this->changed = true;
		}
		if (event.key_pressed(sf::Keyboard::Enter)) {
			this->pressed_enter_key = true;

			if (!this->ignore_pressing_enter_key) {
				if (this->lines.size() >= this->line_limit) {
					this->finished_text = true;
					this->focus = false;
					this->delete_selection_range();
					this->make_selection_rectangles();
					return;
				}
				this->delete_selection_string();
				this->new_line(this->cursor_position.y);
				special_input = true;
				this->changed = true;
			}
			else {
				return;
			}
		}
		if (event.key_pressed(sf::Keyboard::Up)) {
			auto before = this->cursor_position;
			this->move_cursor_up();
			special_input = true;
			if (event.key_holding(sf::Keyboard::LShift)) {
				if (this->selection_a == this->selection_b) {
					this->selection_a = before;
				}
				this->selection_b = this->cursor_position;
				this->make_selection_rectangles();
			}
			else {
				this->delete_selection_range();
				this->make_selection_rectangles();
			}
			this->changed = true;
		}
		if (event.key_pressed(sf::Keyboard::Down)) {
			auto before = this->cursor_position;
			this->move_cursor_down();
			special_input = true;
			if (event.key_holding(sf::Keyboard::LShift)) {
				if (this->selection_a == this->selection_b) {
					this->selection_a = before;
				}
				this->selection_b = this->cursor_position;
				this->make_selection_rectangles();
			}
			else {
				this->delete_selection_range();
				this->make_selection_rectangles();
			}
			this->changed = true;
		}
		if (event.key_pressed(sf::Keyboard::Left)) {
			auto before = this->cursor_position;
			this->move_cursor_left();
			special_input = true;
			if (event.key_holding(sf::Keyboard::LShift)) {
				if (this->selection_a == this->selection_b) {
					this->selection_a = before;
				}
				this->selection_b = this->cursor_position;
				this->make_selection_rectangles();
			}
			else {
				this->delete_selection_range();
				this->make_selection_rectangles();
			}
			this->changed = true;
		}
		if (event.key_pressed(sf::Keyboard::Right)) {
			auto before = this->cursor_position;
			this->move_cursor_right();
			special_input = true;
			if (event.key_holding(sf::Keyboard::LShift)) {
				if (this->selection_a == this->selection_b) {
					this->selection_a = before;
				}
				this->selection_b = this->cursor_position;
				this->make_selection_rectangles();
			}
			else {
				this->delete_selection_range();
				this->make_selection_rectangles();
			}
			this->changed = true;
		}
		if (event.key_holding(sf::Keyboard::LControl)) {
			if (event.key_pressed(sf::Keyboard::A)) {
				this->selection_a = qpl::vec(0u, 0u);
				this->selection_b = qpl::vec(this->lines.back().wstring().length(), this->lines.size() - 1);

				this->cursor_position = this->selection_b;
				this->cursor_interval_timer.reset();
				this->cursor_position_changed = true;
				this->make_selection_rectangles();
				special_input = true;
				this->changed = true;
			}
			else if (event.key_pressed(sf::Keyboard::X)) {
				auto str = this->get_selection_wstring();
				qsf::copy_to_clipboard(str);
				this->delete_selection_string();
				special_input = true;
				this->edited_text = true;
				this->copied_text = true;
				this->changed = true;
			}
			else if (event.key_pressed(sf::Keyboard::C)) {
				auto str = this->get_selection_wstring();
				qsf::copy_to_clipboard(str);
				special_input = true;
				this->copied_text = true;
				this->changed = true;
			}
			else if (event.key_pressed(sf::Keyboard::V)) {
				this->delete_selection_string();
				auto str = qsf::copy_from_clipboard();
				this->add_string_at_cursor(str);
				special_input = true;

				this->update_required = true;
				this->internal_update();
				this->edited_text = true;
				this->pasted_text = true;
				this->changed = true;
			}
			else if (event.key_pressed(sf::Keyboard::Z)) {
				if (this->string_stack.size()) {
					auto last = 0u;
					if (this->string_stack_position != last) {
						--this->string_stack_position;
						this->set_string(this->string_stack[this->string_stack_position].first);
						this->cursor_position = this->string_stack[this->string_stack_position].second;

						this->cursor_position_changed = true;
						this->whole_string_changed = true;
						this->text_mouse_hitboxes_changed = true;
						this->cursor_interval_timer.reset();
						this->update_required = true;
						this->internal_update();
						this->edited_text = true;
						this->control_z = true;
						this->changed = true;
					}
				}
				special_input = true;
			}
			else if (event.key_pressed(sf::Keyboard::Y)) {
				if (this->string_stack.size()) {
					auto last = this->string_stack.used_size() - 1;
					if (this->string_stack.used_size() && this->string_stack_position != last) {
						++this->string_stack_position;
						this->set_string(this->string_stack[this->string_stack_position].first);
						this->cursor_position = this->string_stack[this->string_stack_position].second;
						this->cursor_position_changed = true;
						this->whole_string_changed = true;
						this->text_mouse_hitboxes_changed = true;
						this->cursor_interval_timer.reset();
						this->update_required = true;
						this->internal_update();
						this->edited_text = true;
						this->control_z = true;
						this->changed = true;
					}
				}
				special_input = true;
			}
			else {
				special_input = true;
			}
		}
		if (event.is_text_entered() && !special_input) {
			this->delete_selection_string();
			this->add_string_at_cursor(event.text_entered());

			this->update_required = true;
			this->internal_update();
			this->edited_text = true;
			this->entered_text = true;
			this->changed = true;
		}

		if (this->string_stack.size() && this->edited_text && (!this->control_z && !this->control_y)) {
			auto current_str = this->wstring();
			if (string_before != current_str) {
				if (this->string_stack.used_size() == 0u) {
					this->string_stack.add(std::make_pair(string_before, cursor_before));
					this->string_stack_position = 0u;
				}
				if (this->string_stack_position != this->string_stack.used_size() - 1) {
					auto copy = this->string_stack;
					this->string_stack.reset();
					for (qpl::size i = 0u; i <= this->string_stack_position; ++i) {
						this->string_stack.add(copy[i]);
					}
				}
				this->string_stack.back().second = cursor_before;

				this->string_stack.add(std::make_pair(current_str, cursor_before));
				this->string_stack_position = this->string_stack.used_size() - 1;
				this->changed = true;
			}
		}

		this->internal_update_cursor();
		this->update_selection_rectangles();

		this->dragging_selection_before = this->is_dragging_selection_considering_cooldown();
	}
	void qsf::text_field::draw(qsf::draw_object& draw) const {
		draw.draw(this->background);
		draw.draw(this->selection_rectangles);
		draw.draw(this->lines);

		if (this->focus) {
			draw.draw(this->cursor);
		}
	}

	std::pair<qpl::vector2s, qpl::vector2s> qsf::text_field::get_sorted_selection_range() const {
		auto begin = this->selection_a;
		auto end = this->selection_b;

		if (begin.y > end.y) {
			std::swap(begin, end);
		}
		else if (begin.y == end.y && begin.x > end.x) {
			std::swap(begin, end);
		}
		return std::make_pair(begin, end);
	}
	void qsf::text_field::internal_update() const {
		this->internal_update_text();

		if (this->update_required) {
			this->update_required = false;

			auto hitbox = this->get_hitbox();

			if (hitbox.dimension.x != this->hitbox.dimension.x) {
				for (auto& line : this->lines) {
					line.mouse_hitboxes_changed = true;
				}
				this->text_mouse_hitboxes_changed = true;
			}

			hitbox.dimension.clamp(this->minimum_dimension, this->maximum_dimension);

			this->hitbox.set_dimension(hitbox.dimension);

			if (this->background_increase != qpl::vec(0, 0)) {
				hitbox.dimension.y += this->background_increase.y * 0.9f;
			}

			auto background_hitbox = hitbox.increased(this->background_increase);
			this->background.set_hitbox(background_hitbox);
			this->cursor_position_changed = true;
		}


		if (this->text_mouse_hitboxes_changed) {
			for (auto& i : this->lines) {
				if (i.mouse_hitboxes_changed) {
					i.calculate_mouse_hitboxes(this->hitbox.dimension.x + this->hitbox.position.x, this->background_increase.x);
					i.mouse_hitboxes_changed = false;
				}
			}
			this->text_mouse_hitboxes_changed = false;
		}
	}
	void qsf::text_field::internal_update_text() const {

		if (this->text_layout_changed) {
			for (auto& i : this->lines) {
				if (i.layout_changed) {
					i.apply(this->text_layout);
					i.layout_changed = false;
				}
			}
			this->set_cursor_dimension();

			this->text_layout_changed = false;
		}
	}

	void qsf::text_field::internal_update_cursor() {
		if (this->cursor_position_changed) {
			this->set_cursor_position();
			this->cursor_position_changed = false;
		}
	}
	void qsf::text_field::set_cursor_dimension() const {
		qpl::vector2f cursor_dim;
		cursor_dim.x = this->cursor_width_percentage * this->text_layout.character_size;
		cursor_dim.y = this->lines.front().text.get_line_height() + this->lines.front().text.get_delta_underline();
		this->cursor.set_dimension(cursor_dim);
	}
	void qsf::text_field::set_cursor_position() {
		this->internal_update();

		if (this->cursor_position.y >= this->lines.size()) {
			this->cursor_position.y = this->lines.size() - 1;
			if (this->get_line_at_cursor().wstring().empty()) {
				this->cursor_position.x = 0u;
			}
		}

		const auto& line = this->get_line_at_cursor();
		auto hitbox_index = this->cursor_position.x;

		bool right_side = false;
		if (line.character_hitboxes.empty()) {
			return;
		}
		if (hitbox_index >= line.character_hitboxes.size()) {
			hitbox_index = line.character_hitboxes.size() - 1;
			if (line.wstring().empty()) {

			}
			right_side = true;
		}
		const auto& hitbox = line.character_hitboxes[hitbox_index];

		qpl::f32 x_offset;
		if (right_side) {
			x_offset = hitbox.dimension.x + this->cursor_x_offset;
		}
		else {
			x_offset = -(this->cursor_x_offset + this->cursor.get_dimension().x);
		}

		auto position = hitbox.position;
		position.y = line.text.get_starting_line_position().y - line.text.get_delta_underline();
		position.x += x_offset;

		this->cursor.set_position(position);
	}
	void qsf::text_field::make_selection_rectangles() const {
		if (this->selection_a == this->selection_b) {
			this->selection_rectangles.clear();
			return;
		}
		auto [begin, end] = this->get_sorted_selection_range();

		this->selection_rectangles.clear();
		auto size = end.y - begin.y + 1;
		this->selection_rectangles.reserve(size);
		qpl::size count = 0u;


		if (begin.y == end.y) {
			this->make_selection_rectangles(count, begin.y, begin.x, end.x);
		}
		else {
			for (qpl::size i = 0u; i < size; ++i) {
				if (i == 0u) {
					this->make_selection_rectangles(count, i + begin.y, begin.x);
				}
				else if (i == size - 1) {
					this->make_selection_rectangles(count, i + begin.y, 0u, end.x);
				}
				else {
					this->make_selection_rectangles(count, i + begin.y, 0u);
				}
			}
		}
	}
	void qsf::text_field::make_selection_rectangles(qpl::size& count, qpl::size y, qpl::size x1, qpl::size x2) const {


		const auto& line = this->lines[y];

		if (x2 == qpl::size_max) {
			x2 = line.character_hitboxes.size();
		}
		if (x1 >= line.character_hitboxes.size()) {

			auto a = line.character_hitboxes.back();
			auto length = line.line_mouse_hitbox.dimension.x + line.line_mouse_hitbox.position.x - (a.dimension.x + a.position.x) - this->get_background_outline_thickness();
			qpl::hitbox result;
			result.position = a.position + a.dimension.just_x();
			result.dimension.x = length;

			result.position.y = line.text.get_starting_line_position().y - line.text.get_delta_underline();
			result.dimension.y = line.text.get_line_height() + line.text.get_delta_underline();

			this->selection_rectangles.push_back({});
			this->selection_rectangles.back().set_hitbox(result);
			this->selection_rectangles.back().set_color(this->selection_color);
			++count;

			return;
		}
		auto a = line.character_hitboxes[x1];

		qpl::hitbox b;

		auto right_side = x2 == line.character_hitboxes.size();
		qpl::f32 offset_x;
		if (right_side) {
			if (x2 == 0u) {
				b = line.character_hitboxes[x2];
			}
			else {
				b = line.character_hitboxes[x2 - 1];
			}
			offset_x = b.dimension.x + this->cursor_x_offset;
		}
		else {
			b = line.character_hitboxes[x2];
			offset_x = -(this->cursor_x_offset + this->cursor.get_dimension().x);
		}

		qpl::vector2f position = a.position;
		qpl::vector2f dimension = b.position - a.position + offset_x;

		position.y = line.text.get_starting_line_position().y - line.text.get_delta_underline();
		dimension.y = line.text.get_line_height() + line.text.get_delta_underline();

		this->selection_rectangles.push_back({});
		this->selection_rectangles.back().set_hitbox({ position, dimension });
		this->selection_rectangles.back().set_color(this->selection_color);
		++count;
	}
	void qsf::text_field::find_closest_cursor_position() {
		const auto& hitboxes = this->lines[this->cursor_position.y].character_hitboxes;

		bool found = false;
		qpl::size new_position = 0u;
		qpl::f32 diff_before = qpl::f32_max;
		for (qpl::size i = 0u; i < hitboxes.size(); ++i) {

			auto x = hitboxes[i].position.x - (this->cursor_x_offset + this->cursor.get_dimension().x);
			auto diff = qpl::abs(this->cursor.get_position().x - x);

			if (diff > diff_before) {
				found = true;
				break;
			}
			diff_before = diff;
			new_position = i;
		}
		if (!found) {
			new_position = hitboxes.size();
		}
		this->cursor_position.x = new_position;
		this->cursor_position_changed = true;
	}
	bool qsf::text_field::just_changed() const {
		return this->changed || this->pressed_enter_key;
	}
	void qsf::text_field::update_all_changes() {
		this->text_mouse_hitboxes_changed = true;
		this->text_layout_changed = true;
		this->update_required = true;
		this->whole_string_changed = true;
		this->cursor_position_changed = true;
		this->edited_text = true;
		this->entered_text = true;
		this->changed = true;
		this->hitbox.dimension = qpl::vec(qpl::f32_max, qpl::f32_max);
		this->internal_update();
	}


	void qsf::color_picker::init() {
		this->set_hitbox({ qpl::vec(0, 0), qpl::vec(460, 400) });

		this->gradient.resize(this->gradient_segments * this->gradient_segments * 4);
		this->gradient.set_primitive_type(qsf::primitive_type::quads);

		this->hue_slider.resize(this->hue_slider_segments * 4);
		this->hue_slider.set_primitive_type(qsf::primitive_type::quads);

		this->background.set_color(qpl::rgb(50, 50, 50));
		this->background.set_slope_dimension(40);

		auto slope = 4.0f;
		this->hue_slider_knob.set_dimension(qpl::vec(this->slider_width + slope * 2, 24.0f));
		this->hue_slider_knob.set_slope_dimension(slope);
		this->hue_slider_knob.set_color(qpl::rgb::white().with_alpha(220));

		this->transparency_slider_knob = this->hue_slider_knob;

		this->gradient_control = { 1.0, 0.0 };

		this->gradient_control_circle.set_radius(this->gradient_control_circle_radius);
		this->gradient_control_circle.set_color(qpl::rgba::transparent());
		this->gradient_control_circle.set_outline_color(qpl::rgb::white());
		this->gradient_control_circle.set_outline_thickness(4.0f);

		this->gradient_cross.resize(4u * 4u);
		this->gradient_cross.set_primitive_type(qsf::primitive_type::quads);

		this->display_color_rectangle.set_dimension({ 100, 40 });
		this->display_color_rectangle.set_outline_thickness(4.0f);
		this->display_color_rectangle.set_outline_color(qpl::rgb::white());

		this->input_field.set_background_outline_thickness(0.0f);
		this->input_field.enable_one_line_limit();
		this->set_input_field_width(200);

		this->transparency_slider.resize(4u);
		this->transparency_slider.set_primitive_type(qsf::primitive_type::quads);
		this->transparency_slider[2].color = qpl::rgba::transparent();
		this->transparency_slider[3].color = qpl::rgba::transparent();

		this->transparency_checkerboard.set_primitive_type(qsf::primitive_type::quads);

		this->confirm_button.set_dimension({ 100, 40 });
		this->confirm_button.set_background_color(qpl::rgb::grey_shade(30));
		this->confirm_button.set_text_color(qpl::rgb::white());

		this->reset();
	}
	void qsf::color_picker::reset() {
		this->gradient_control = { 1, 0 };
		this->hue_control = 0.0;
		this->transparency_control = 1.0;
	}
	void qsf::color_picker::update_values() {
		this->hue_color = qpl::get_rainbow_color(this->hue_control);
		this->color_result = this->get_gradient_color_at(this->gradient_control);
		this->display_color_rectangle.set_color(this->color_result);

		if (this->is_font_set()) {
			this->input_field.set_string(this->color_result.string());
		}
		this->color_changed = true;
		this->internal_gradient_color_changed = true;
	}

	bool qsf::color_picker::color_value_changed() const {
		return this->color_changed;
	}
	bool qsf::color_picker::has_focus() const {
		return this->focus;
	}
	bool qsf::color_picker::is_font_set() const {
		return this->input_field.is_font_set();
	}

	void qsf::color_picker::set_color_value(qpl::rgba color) {
		qpl::f64 x = 0.0;
		if (color.max()) {
			x = 1.0 - (color.min() / qpl::f64_cast(color.max()));
		}
		qpl::f64 y = 1.0 - color.max() / qpl::f64_cast(color.max_channel());
		this->gradient_control = { x, y };
		this->hue_control = color.get_hue();

		this->update_gradient_control_position();
		this->update_hue_slider_knob_position();

		if (this->transparency_used) {
			this->transparency_control = color.a / 255.0;
			this->update_transparency_slider_knob_position();
		}
		this->color_changed = true;
		this->dont_update_input_text = true;

		this->update_internal_colors();
	}
	qpl::rgba qsf::color_picker::get_color_value() const {
		if (this->internal_gradient_hitbox_changed || this->internal_gradient_color_changed) {
			this->internal_update();
		}
		return this->color_result;
	}
	qpl::rgb qsf::color_picker::get_gradient_color_at(qpl::vec2f pos) const {
		auto color = this->hue_color;
		qpl::frgb result = color;
		result.interpolate(qpl::rgb::white(), 1.0f - pos.x);
		result.interpolate(qpl::rgb::black(), pos.y);
		return result;
	}
	qpl::hitbox qsf::color_picker::get_gradient_hitbox() const {
		auto slider_space = (this->slider_margin + this->slider_width);
		if (this->transparency_used) {
			slider_space *= 2;
		}
		auto hitbox = this->hitbox;
		hitbox.set_width(this->hitbox.get_width() - slider_space);
		hitbox.set_height(this->hitbox.get_height() - (this->bottom_margin + this->bottom_height));
		return hitbox;
	}
	qpl::hitbox qsf::color_picker::get_hue_slider_hitbox() const {
		auto slider_space = this->slider_width;
		if (this->transparency_used) {
			slider_space += (this->slider_margin + this->slider_width);
		}
		auto hitbox = this->hitbox;
		hitbox.set_width(this->slider_width);
		hitbox.set_height(this->hitbox.get_height() - (this->bottom_margin + this->bottom_height));
		hitbox.set_position(this->hitbox.position + qpl::vec(this->hitbox.get_width() - slider_space, 0));
		return hitbox;
	}
	qpl::hitbox qsf::color_picker::get_transparency_slider_hitbox() const {
		auto hitbox = this->hitbox;
		hitbox.set_width(this->slider_width);
		hitbox.set_height(this->hitbox.get_height() - (this->bottom_margin + this->bottom_height));
		hitbox.set_position(this->hitbox.position + qpl::vec(this->hitbox.get_width() - this->slider_width, 0));
		return hitbox;
	}
	qpl::f64 qsf::color_picker::get_hue_control_value() const {
		auto height = this->get_hue_slider_hitbox().get_height() - this->hue_slider_knob.get_dimension().y;

		auto y = this->hue_slider_knob.get_position().y - this->hitbox.position.y;
		return qpl::clamp_0_1(y / height);
	}
	qpl::f64 qsf::color_picker::get_transparency_control_value() const {
		auto height = this->get_transparency_slider_hitbox().get_height() - this->transparency_slider_knob.get_dimension().y;

		auto y = this->transparency_slider_knob.get_position().y - this->hitbox.position.y;
		return 1.0 - qpl::clamp_0_1(y / height);
	}
	qpl::vec2d qsf::color_picker::get_gradient_control_value() const {
		auto value = this->gradient_control_circle.get_center() - this->hitbox.position;
		return (value / this->get_gradient_hitbox().get_dimension()).clamped(0.0, 1.0);
	}

	void qsf::color_picker::disable_transparency() {
		this->transparency_used = false;
	}
	void qsf::color_picker::enable_transparency() {
		this->transparency_used = true;
	}

	void qsf::color_picker::set_font(std::string font) {
		this->input_field.set_font(font);
		this->confirm_button.set_text_font(font);

		this->confirm_button.set_text_string("ok");
		this->update_values();
	}
	void qsf::color_picker::set_invisible() {
		this->visible = false;
	}
	void qsf::color_picker::set_visible() {
		this->visible = true;
	}
	bool qsf::color_picker::is_visible() const {
		return this->visible;
	}
	bool qsf::color_picker::is_dragging_hue_slider() const {
		return this->dragging_hue_slider;
	}
	bool qsf::color_picker::is_dragging_gradient() const {
		return this->dragging_gradient;
	}
	bool qsf::color_picker::is_dragging() const {
		return this->is_dragging_gradient() || this->is_dragging_hue_slider();
	}

	void qsf::color_picker::set_input_field_width(qpl::f32 width) {
		this->input_field.set_fixed_width(width);
	}
	void qsf::color_picker::set_dimension(qpl::vec2f dimension) {
		this->hitbox.dimension = dimension;
		this->internal_gradient_hitbox_changed = true;
	}
	void qsf::color_picker::set_position(qpl::vec2f position) {
		this->hitbox.position = position;
		this->internal_gradient_hitbox_changed = true;
	}
	void qsf::color_picker::set_hitbox(qpl::hitbox hitbox) {
		this->hitbox = hitbox;
		this->internal_gradient_hitbox_changed = true;
	}
	void qsf::color_picker::set_center(qpl::vec2f center) {
		this->hitbox.set_center(center);
		this->internal_gradient_hitbox_changed = true;
	}
	void qsf::color_picker::set_hue_wheel(qpl::f64 hue) {
		this->hue_control = hue;
		this->internal_gradient_color_changed = true;
	}
	void qsf::color_picker::set_background_color(qpl::rgba color) {
		this->background.set_color(color);
	}
	void qsf::color_picker::set_hue_control_color(qpl::rgba color) {
		this->hue_slider_knob.set_color(color);
	}
	void qsf::color_picker::set_gradient_control_color(qpl::rgba color) {
		this->gradient_control_circle.set_color(color);
	}

	void qsf::color_picker::create_gradient() const {
		auto gradient_dimension = this->get_gradient_hitbox().dimension / this->gradient_segments;

		auto gradient_dim = qpl::vec(this->gradient_segments, this->gradient_segments);
		for (qpl::vec2s pos : gradient_dim.list_possibilities_range()) {
			auto index = pos.y * this->gradient_segments + pos.x;
			for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {
				auto position = (pos + qpl::vec_square4[i]) * gradient_dimension + this->hitbox.position;
				this->gradient[index * 4 + i].position = position;
			}
		}
	}
	void qsf::color_picker::update_gradient() const {
		auto gradient_dim = qpl::vec(this->gradient_segments, this->gradient_segments);
		for (qpl::vec2s pos : gradient_dim.list_possibilities_range()) {
			auto index = pos.y * this->gradient_segments + pos.x;
			for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {
				auto gradient_progress = (pos + qpl::vec_square4[i]) / qpl::vec2f(this->gradient_segments, this->gradient_segments);
				qpl::frgb color_result = this->hue_color;
				color_result.interpolate(qpl::rgb::white(), 1.0f - gradient_progress.x);
				color_result.interpolate(qpl::rgb::black(), gradient_progress.y);
				this->gradient[index * 4 + i].color = color_result;
			}
		}

		this->internal_gradient_color_changed = false;
	}
	void qsf::color_picker::create_hue_slider() const {
		auto right_side = this->get_gradient_hitbox().get_top_right();
		auto slider_height = this->get_hue_slider_hitbox().get_height() / this->hue_slider_segments;
		for (qpl::size s = 0u; s < this->hue_slider_segments; ++s) {
			for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {

				auto position = (qpl::vec(0, s) + qpl::vec_square4[i]) * qpl::vec(this->slider_width, slider_height) + right_side + qpl::vec(this->slider_margin, 0);
				this->hue_slider[s * 4 + i].position = position;

				auto progress = qpl::f64_cast(s + qpl::vec_square4[i].y) / this->hue_slider_segments;
				this->hue_slider[s * 4 + i].color = qpl::get_rainbow_color(progress);
			}
		}
	}
	void qsf::color_picker::create_transparency_slider() const {
		if (this->transparency_used) {
			auto transparency_hitbox = this->get_transparency_slider_hitbox();
			for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {
				auto position = transparency_hitbox.position + transparency_hitbox.dimension * qpl::vec_square4[i];
				this->transparency_slider[i].position = position;
			}
			auto checkerboard_size = qpl::vec2s(transparency_hitbox.dimension / this->transparency_checkerboard_dimension);
			this->transparency_checkerboard.resize(checkerboard_size.x * checkerboard_size.y * 4u);

			for (qpl::vec2s pos : checkerboard_size.list_possibilities_range()) {
				auto index = pos.y * checkerboard_size.x + pos.x;
				for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {
					auto position = (pos + qpl::vec_square4[i]) * this->transparency_checkerboard_dimension + transparency_hitbox.position;
					position.clamp(transparency_hitbox.get_bottom_right());
					this->transparency_checkerboard[index * 4 + i].position = position;

					qpl::rgb color;
					if ((pos.x + pos.y) % 2) {
						color = this->transparency_checkerboard_color_dark;
					}
					else {
						color = this->transparency_checkerboard_color_light;
					}
					this->transparency_checkerboard[index * 4 + i].color = color;
				}
			}
		}

	}
	void qsf::color_picker::update_bottom_row_positions() const {
		auto background_move = qpl::vec(0.f, -this->background_increase / 2);

		auto h = this->display_color_rectangle.get_hitbox();
		h.set_left_center(this->hitbox.get_left_bottom().moved(background_move + qpl::vec(this->display_color_rectangle.get_outline_thickness(), 0.0)));
		this->display_color_rectangle.set_hitbox(h);

		if (this->is_font_set()) {
			h = this->input_field.get_background_hitbox();
			h.set_left_center(this->display_color_rectangle.get_hitbox().get_right_center().moved(this->input_field.get_background_increase() + qpl::vec(this->slider_margin, 0)));
			this->input_field.set_hitbox(h);
		}

		h = this->confirm_button.get_hitbox();
		h.set_bottom_right(this->hitbox.get_bottom_right().moved(qpl::vec(5, 0)));
		this->confirm_button.set_hitbox(h);
		this->confirm_button.centerize_text();

	}
	void qsf::color_picker::internal_update() const {
		if (this->internal_gradient_hitbox_changed) {
			this->internal_gradient_hitbox_changed = false;

			this->update_gradient_control_position();
			this->update_hue_slider_knob_position();
			this->update_transparency_slider_knob_position();

			this->background.set_hitbox(this->hitbox.increased(this->background_increase));
			this->update_bottom_row_positions();

			this->create_gradient();
			this->create_hue_slider();
			this->create_transparency_slider();
		}
		if (this->internal_gradient_color_changed) {
			this->update_gradient();
		}
	}
	void qsf::color_picker::update_hue_slider_knob_position() const {
		auto hitbox = this->get_hue_slider_hitbox();

		auto height = hitbox.get_height() - this->hue_slider_knob.get_dimension().y;
		auto y = height * this->hue_control;
		this->hue_slider_knob.set_position(hitbox.position + qpl::vec(-this->hue_slider_knob.get_slope_dimension().x, y));
	}
	void qsf::color_picker::update_transparency_slider_knob_position() const {
		auto hitbox = this->get_transparency_slider_hitbox();

		auto height = hitbox.get_height() - this->transparency_slider_knob.get_dimension().y;
		auto y = height * (1.0 - this->transparency_control);
		this->transparency_slider_knob.set_position(hitbox.position + qpl::vec(-this->transparency_slider_knob.get_slope_dimension().x, y));
	}
	void qsf::color_picker::update_gradient_control_position() const {
		auto gradient_hitbox = this->get_gradient_hitbox();

		auto value = gradient_hitbox.dimension * this->gradient_control;
		auto pos = gradient_hitbox.position + value;
		this->gradient_control_circle.set_center(pos);

		if (!this->cross_enabled) {
			return;
		}
		qpl::size cross_ctr = 0u;

		auto cross_width = 2.0f;
		auto circle_radius = this->gradient_control_circle_radius + this->gradient_control_circle.get_outline_thickness();

		for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {
			auto& vertex = this->gradient_cross[cross_ctr * 4 + i];
			auto cross_value = qpl::vec_square4[(i + cross_ctr + 3) % qpl::vec_square4.size()];

			auto start = qpl::vec(0, value.y - (cross_width / 2));
			auto target_x = value.x - circle_radius;
			auto position = (start + qpl::vec(target_x, cross_width) * cross_value).clamped(gradient_hitbox.dimension);
			vertex.position = gradient_hitbox.position + position;
		}
		++cross_ctr;

		for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {
			auto& vertex = this->gradient_cross[cross_ctr * 4 + i];
			auto cross_value = qpl::vec_square4[(i + cross_ctr + 3) % qpl::vec_square4.size()];

			auto start = qpl::vec(value.x - (cross_width / 2), 0);
			auto target_y = value.y - circle_radius;
			auto position = (start + qpl::vec(cross_width, target_y) * cross_value).clamped(gradient_hitbox.dimension);
			vertex.position = gradient_hitbox.position + position;
		}
		++cross_ctr;

		for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {
			auto& vertex = this->gradient_cross[cross_ctr * 4 + i];
			auto cross_value = qpl::vec_square4[(i + cross_ctr + 3) % qpl::vec_square4.size()];

			auto start = qpl::vec(value.x + circle_radius, value.y - (cross_width / 2));
			auto target_x = gradient_hitbox.dimension.x - value.x - circle_radius;
			auto position = (start + qpl::vec(target_x, cross_width) * cross_value).clamped(gradient_hitbox.dimension);
			vertex.position = gradient_hitbox.position + position;
		}
		++cross_ctr;

		for (qpl::size i = 0u; i < qpl::vec_square4.size(); ++i) {
			auto& vertex = this->gradient_cross[cross_ctr * 4 + i];
			auto cross_value = qpl::vec_square4[(i + cross_ctr + 3) % qpl::vec_square4.size()];

			auto start = qpl::vec(value.x - (cross_width / 2), value.y + circle_radius);
			auto target_y = gradient_hitbox.dimension.y - value.y - circle_radius;
			auto position = (start + qpl::vec(cross_width, target_y) * cross_value).clamped(gradient_hitbox.dimension);
			vertex.position = gradient_hitbox.position + position;
		}
		++cross_ctr;

		for (qpl::size i = 0u; i < this->gradient_cross.size(); ++i) {
			auto vertex = i % 4;
			if (vertex == 0 || vertex == 1) {
				this->gradient_cross[i].color = qpl::rgba::transparent();
			}
		}
	}
	void qsf::color_picker::update_internal_colors() {
		auto gradient_color = this->get_gradient_color_at(this->gradient_control);
		this->color_result = gradient_color;

		if (this->transparency_used) {
			this->color_result.a = qpl::u8_cast(std::round(this->transparency_control * 255));
		}
		this->display_color_rectangle.set_color(this->color_result);

		if (this->transparency_used) {
			this->transparency_slider[0].color = gradient_color;
			this->transparency_slider[1].color = gradient_color;
		}

		if (!this->dont_update_input_text && this->is_font_set()) {
			this->input_field.set_string(this->color_result.string());
		}
	}

	void qsf::color_picker::update_entered_text() {
		auto str = this->input_field.string();
		auto numbers = qpl::string_split_numbers<qpl::size>(str);
		if (!numbers.empty() && (numbers.size() == 3u || numbers.size() == 4u)) {
			qpl::rgba result;
			for (qpl::size i = 0u; i < numbers.size(); ++i) {
				result[i] = qpl::u8_cast(qpl::clamp<qpl::size>(0u, numbers[i], 255u));
			}
			this->set_color_value(result);
			this->dont_update_input_text = true;
		}

	}

	void qsf::color_picker::update(const qsf::event_info& event) {
		if (!this->visible) {
			return;
		}

		this->focus = false;
		this->dont_update_input_text = false;

		if (this->is_font_set()) {
			event.update(this->confirm_button);
			event.update(this->input_field);
			if (this->input_field.has_focus()) {
				this->focus = true;
			}
			if (this->input_field.just_changed()) {
				this->update_entered_text();
			}
		}

		this->color_changed = false;
		bool clicked_hue_slider = false;
		bool clicked_transparency_slider = false;
		bool clicked_gradient = false;
		if (this->get_gradient_hitbox().increased(this->hitbox_increase).contains(event.mouse_position())) {
			if (event.left_mouse_clicked()) {
				clicked_gradient = true;
				this->dragging_gradient = true;
			}
		}
		if (this->get_hue_slider_hitbox().increased(this->hitbox_increase).contains(event.mouse_position())) {
			if (event.left_mouse_clicked()) {
				clicked_hue_slider = true;
				this->dragging_hue_slider = true;
			}
		}
		if (this->transparency_used && this->get_transparency_slider_hitbox().increased(this->hitbox_increase).contains(event.mouse_position())) {
			if (event.left_mouse_clicked()) {
				clicked_transparency_slider = true;
				this->dragging_transparency_slider = true;
			}
		}
		if (this->dragging_gradient && (clicked_gradient || event.mouse_moved())) {
			this->gradient_control_circle.set_center(event.mouse_position());

			this->gradient_control = this->get_gradient_control_value();
			this->update_gradient_control_position();
			this->color_changed = true;
			this->focus = true;
		}
		if (this->dragging_hue_slider && (clicked_hue_slider || event.mouse_moved())) {
			auto x = this->hue_slider_knob.get_position().x;
			this->hue_slider_knob.set_position(qpl::vec(x, event.mouse_position().y - this->hue_slider_knob.get_dimension().y / 2));
			this->internal_gradient_color_changed = true;

			this->hue_control = this->get_hue_control_value();
			this->hue_color = qpl::get_rainbow_color(this->hue_control);
			this->update_hue_slider_knob_position();
			this->color_changed = true;
			this->focus = true;
		}
		if (this->dragging_transparency_slider && (clicked_transparency_slider || event.mouse_moved())) {
			auto x = this->transparency_slider_knob.get_position().x;
			this->transparency_slider_knob.set_position(qpl::vec(x, event.mouse_position().y - this->transparency_slider_knob.get_dimension().y / 2));
			this->internal_gradient_color_changed = true;

			this->transparency_control = this->get_transparency_control_value();
			this->update_transparency_slider_knob_position();
			this->color_changed = true;
			this->focus = true;
		}

		if (this->color_changed) {
			this->update_internal_colors();
		}

		if (event.left_mouse_released()) {
			this->dragging_hue_slider = false;
			this->dragging_transparency_slider = false;
			this->dragging_gradient = false;
		}
	}
	void qsf::color_picker::draw(qsf::draw_object& draw) const {
		if (!this->visible) {
			return;
		}
		if (this->internal_gradient_hitbox_changed || this->internal_gradient_color_changed) {
			this->internal_update();
		}
		draw.draw(this->background);
		draw.draw(this->gradient);
		draw.draw(this->hue_slider);
		draw.draw(this->hue_slider_knob);
		if (this->transparency_used) {
			draw.draw(this->transparency_checkerboard);
			draw.draw(this->transparency_slider);
			draw.draw(this->transparency_slider_knob);
		}
		draw.draw(this->gradient_control_circle);
		draw.draw(this->display_color_rectangle);
		if (this->cross_enabled) {
			draw.draw(this->gradient_cross);
		}
		if (this->is_font_set()) {
			draw.draw(this->input_field);
			draw.draw(this->confirm_button);
		}
	}

	qsf::scroll_bar::scroll_bar() {
		this->background.set_color(this->background_color);
		this->knob.set_color(this->knob_color);
		this->background.set_slope_dimension(5);
		this->knob.set_slope_dimension(5);
		this->hover_animation.set_duration(0.2);
		this->background_hover_animation.set_duration(0.2);
	}
	void qsf::scroll_bar::set_dimension(qpl::vec2f dimension) {
		this->hitbox.dimension = dimension;
		this->update_positions();
	}
	void qsf::scroll_bar::set_position(qpl::vec2f position) {
		this->hitbox.position = position;
		this->update_positions();
	}
	void qsf::scroll_bar::set_hitbox(qpl::hitbox hitbox) {
		this->hitbox = hitbox;
		this->update_positions();
	}
	void qsf::scroll_bar::set_visible_knob_progress(qpl::f64 progress) {
		this->visible_knob_progress = progress;
		auto height = this->hitbox.get_height() - this->knob.get_dimension().y;
		this->knob.set_position(this->hitbox.position + qpl::vec2f(0.0f, height * this->visible_knob_progress));
	}
	void qsf::scroll_bar::update_knob() {
		//auto progress = this->stepped_progress();
		auto progress = this->stepped_progress_unless_dragging();
		this->knob.set_dimension(qpl::vec2f(this->hitbox.get_width(), this->hitbox.get_height() * this->knob_range));
		this->set_visible_knob_progress(progress);
	}
	void qsf::scroll_bar::update_positions() {
		this->background.set_dimension(this->hitbox.dimension);
		this->background.set_position(this->hitbox.position);
		this->update_knob();
	}
	qpl::f32 qsf::scroll_bar::stepped_progress() const {
		qpl::f32 progress = this->knob_progress;
		if (this->integer_step != qpl::size_max) {
			progress = std::round(this->knob_progress * this->integer_step) / (this->integer_step);
		}
		return progress;
	}
	qpl::f32 qsf::scroll_bar::stepped_progress_unless_dragging() const {
		if (this->dragging) {
			return this->knob_progress;
		}
		else {
			return this->stepped_progress();
		}
	}
	qpl::f32 qsf::scroll_bar::get_progress() const {
		return this->knob_progress;
	}
	qpl::size qsf::scroll_bar::get_progress_step() const {
		//return qpl::size_cast(std::ceil(this->knob_progress * this->integer_step));
		return qpl::size_cast(std::round(this->knob_progress * (this->integer_step)));
	}
	void qsf::scroll_bar::set_progress_integer_step(qpl::size step, qpl::size knob_steps) {
		this->knob_range = qpl::max(0.05, qpl::f64_cast(knob_steps) / step);
		this->integer_step = step - knob_steps;
	}
	void qsf::scroll_bar::set_progress(qpl::f32 progress) {
		this->knob_progress = qpl::clamp_0_1(progress);
		this->update_knob();
	}
	void qsf::scroll_bar::set_knob_height(qpl::f32 height) {
		this->knob_range = height;
		this->update_knob();
	}
	void qsf::scroll_bar::set_knob_range(qpl::f32 range) {
		this->knob_range = range;
	}
	void qsf::scroll_bar::update_hover(const qsf::event_info& event) {
		auto hitbox = this->knob.get_hitbox().increased(5);
		this->hovering = hitbox.contains(event.mouse_position());
		if (this->hovering && event.left_mouse_clicked() && this->allow_dragging) {
			this->dragging_position = event.mouse_position();
			this->dragging = true;
		}

		if (this->hovering || this->dragging) {
			this->hover_animation.go_forwards();
		}
		else {
			this->hover_animation.go_backwards();
		}
		this->hover_animation.update(event);
		if (this->hover_animation.is_running()) {
			auto p = this->hover_animation.get_progress();
			auto curve = qpl::smooth_slope(p);
			this->knob.set_color(this->knob_color.interpolated(this->knob_color_hover, curve));
		}
	}
	void qsf::scroll_bar::update_background_hover(const qsf::event_info& event) {
		this->hovering_background = !this->dragging && !this->hovering && this->hitbox.increased(5).contains(event.mouse_position());
		if (this->hovering_background && event.left_mouse_clicked()) {
			if (event.mouse_position().y < this->knob.get_position().y) {
				this->clicked_on_background_above = true;
			}
			else {
				this->clicked_on_background_below = true;
			}
		}

		if (this->hovering_background) {
			this->background_hover_animation.go_forwards();
		}
		else {
			this->background_hover_animation.go_backwards();
		}
		this->background_hover_animation.update(event);
		if (this->background_hover_animation.is_running()) {
			auto p = this->background_hover_animation.get_progress();
			auto curve = qpl::smooth_slope(p);
			this->background.set_color(this->background_color.interpolated(this->background_color_hover, curve));
		}
	}
	void qsf::scroll_bar::update(const qsf::event_info& event) {
		this->value_changed = false;
		this->released_dragging = false;
		this->clicked_on_background_above = false;
		this->clicked_on_background_below = false;
		if (this->knob_range >= 1.0f) {
			return;
		}

		this->update_hover(event);
		this->update_background_hover(event);

		if (this->dragging) {
			auto delta = event.mouse_position().y - this->dragging_position.y;

			auto height = this->hitbox.dimension.y - this->knob.get_dimension().y;
			auto percentage = delta / height;

			auto before = this->stepped_progress();
			this->knob_progress = qpl::clamp_0_1(this->knob_progress + percentage);

			this->update_knob();
			if (before != this->stepped_progress()) {
				this->value_changed = true;
			}

			this->dragging_position = event.mouse_position();
		}
		if (event.left_mouse_released()) {
			if (this->dragging) {
				this->released_dragging = true;
			}
			this->dragging = false;
		}
	}
	void qsf::scroll_bar::draw(qsf::draw_object& draw) const {
		if (this->knob_range >= 1.0f) {
			return;
		}
		draw.draw(this->background);
		draw.draw(this->knob);
	}

	void qsf::console::press_enter_to_continue_mode() {
		this->enter_to_continue = true;
	}
	void qsf::console::reset_visible_range() {
		this->visible_y_min = 0;
		this->visible_y_max = 0;
	}
	void qsf::console::clear() {
		this->colored_text.clear();
		this->string.clear();
		this->input_string.clear_copy_style_self();
		this->string_and_input.clear();
		this->input_string_split.clear();
		this->string_split.clear();
		this->string_and_input_split.clear();
		this->selection_rectangle.clear();
		this->visible_y_min = 0;
		this->visible_y_max = 0;
		this->cursor_position = { 0, 0 };
		this->view_row = 0u;
		this->view.position.y = 0;
		this->visible_rows = 0u;
		this->process_text();
	}
	void qsf::console::init() {
		this->scroll_transition_animation.set_duration(0.4);
		this->view.position.x = -10.0f;
		this->update_cursor_dimension();
		this->update_cursor_position();
	}
	void qsf::console::apply_font() {
		this->init();
		this->calculate_default_character_size();
	}
	void qsf::console::set_border_texture(const sf::Texture& texture) {
		//this->shadow_border.set_texture(texture);
		//this->shadow_border.set_color(this->shadow_border_color);
		//this->shadow_border.set_scale(0.2f);
		//this->shadow_border.clear();
		//this->shadow_border.add_top();

		this->scroll_bar_shadow_border.set_texture(texture);
		//this->scroll_bar_shadow_border.set_color(this->shadow_border_color);
		this->scroll_bar_shadow_border.set_color(qpl::rgb::grey_shade(10));
		this->scroll_bar_shadow_border.set_scale(0.125f);
		this->scroll_bar_shadow_border.clear();
		this->scroll_bar_shadow_border.add_right();

		this->border_texture_set = true;
	}
	void qsf::console::set_font(std::string font) {
		this->colored_text.set_font(qsf::get_font(font));
		this->apply_font();
	}
	void qsf::console::set_unicode_font(std::string font) {
		this->colored_text.set_unicode_font(qsf::get_font(font));
		this->apply_font();
	}
	void qsf::console::calculate_default_character_size() {
		this->character_size.y = this->colored_text.get_line_spacing_pixels();
		this->character_size.x = this->colored_text.get_white_space_width();
	}
	void qsf::console::track_before_input_values() {
		this->before_input_vertices_size = this->colored_text.vertices.size();
		this->before_input_outline_vertices_size = this->colored_text.outline_vertices.size();
		this->before_input_unicode_vertices_size = this->colored_text.unicode_vertices.size();
		this->before_input_unicode_outline_vertices_size = this->colored_text.unicode_outline_vertices.size();
		this->before_input_text_position = this->colored_text.text_position;
		this->before_input_text_rows = this->colored_text.rows;
	}
	void qsf::console::start_accepting_input() {
		if (this->accept_input) {
			return;
		}
		this->accept_input = true;
		this->track_before_input_values();
		this->cursor_position = { 0, 0 };
		this->update_cursor_position(true); 
		this->move_to_input();
	}
	void qsf::console::stop_accepting_input() {
		this->accept_input = false;
		this->string << this->input_string;
		this->input_string.clear_copy_style_self();
	}
	void qsf::console::clamp_view_y(bool transition) {
		if (transition) {
			this->scroll_transition_start = this->view.position.y;
		}
		auto max_rows = qpl::isize_cast((this->dimension.y * this->view.scale.y) / this->colored_text.get_line_spacing_pixels());
		this->view_row = qpl::max(0ll, this->view_row);

		if (transition) {
			this->scroll_transition_end = this->view_row * this->colored_text.get_line_spacing_pixels();
			this->scroll_transition_animation.reset_and_start();
		}
	}
	void qsf::console::set_dimension(qpl::vec2f dimension) {
		this->dimension = dimension;
		auto width = 25.0f;
		auto margin = 5.0f;
		this->scroll_bar.set_position({ this->dimension.x - width, margin });
		this->scroll_bar.set_dimension(qpl::vec2f(width, this->dimension.y - margin * 2));
		this->update_visible_rows_count();
		this->clamp_view_y();
		this->scroll_bar.update_knob();
		this->make_selection_rectangles();

		if (this->border_texture_set) {
			//this->shadow_border.set_dimension(dimension);
			//this->shadow_border.clear();
			//this->shadow_border.add_top();

			this->scroll_bar_shadow_border.set_dimension(qpl::vec(this->scroll_bar.hitbox.position.x, dimension.y));
			this->scroll_bar_shadow_border.clear();
			this->scroll_bar_shadow_border.add_right();
		}
		this->update_cursor_position(false);
	}
	void qsf::console::set_input_color(qpl::foreground color) {
		this->input_string << color;
	}
	void qsf::console::prepare_scroll() {
		this->scroll_bar_transition_start = this->scroll_bar.visible_knob_progress;
		this->scroll_bar.set_progress(this->view_row / qpl::f64_cast(this->colored_text.rows - this->visible_rows));
		this->scroll_bar_transition_end = this->scroll_bar.get_progress();

		this->clamp_view_y();
	}
	void qsf::console::update_visible_rows_count() {
		this->visible_rows = std::floor(this->dimension.y / this->colored_text.get_line_spacing_pixels());
		this->scroll_bar.set_progress_integer_step(this->colored_text.rows + 1, this->visible_rows);
	}
	void qsf::console::end_animation() {
		this->scroll_transition_start = this->scroll_transition_end;
		this->view.position.y = this->scroll_transition_end;
		this->scroll_bar.set_visible_knob_progress(this->scroll_bar_transition_end);
		this->scroll_transition_animation.reset();
	}
	void qsf::console::update_string_and_input_split() {
		this->string_and_input = this->string;
		this->string_and_input << this->input_string;
		this->string_and_input_split = qpl::string_split_allow_empty(qpl::to_basic_string<wchar_t>(this->string_and_input.string()), L'\n');
	}
	void qsf::console::update_string_split() {
		this->string_split = qpl::string_split_allow_empty(qpl::to_basic_string<wchar_t>(this->string.string()), L'\n');
	}
	void qsf::console::update_input_string_split() {
		this->input_string_split = qpl::string_split_allow_empty(qpl::to_basic_string<wchar_t>(this->input_string.string()), L'\n');
		this->update_string_and_input_split();
	}
	void qsf::console::update_cursor_position(bool reset_timer) {
		auto cursor_pos_with_tabs = this->cursor_position;
		if (this->cursor_position.y < this->input_string_split.size()) {
			auto count = qpl::count(this->input_string_split[this->cursor_position.y].substr(0u, this->cursor_position.x), L'\t');
			cursor_pos_with_tabs.x += count * 3u;
		}
		auto off = cursor_pos_with_tabs * this->character_size;

		this->cursor.set_position(this->before_input_text_position + off - qpl::vec(0.0f, this->colored_text.character_size));
		if (reset_timer) {
			this->cursor_blink_timer.reset();
		}
	}
	void qsf::console::update_input_text_graphics() {
		this->colored_text.text_position = this->before_input_text_position;

		if (this->before_input_vertices_size != qpl::size_max) {
			this->colored_text.vertices.resize(this->before_input_vertices_size);
		}
		if (this->before_input_outline_vertices_size != qpl::size_max) {
			this->colored_text.outline_vertices.resize(this->before_input_outline_vertices_size);
		}
		if (this->before_input_unicode_vertices_size != qpl::size_max) {
			this->colored_text.unicode_vertices.resize(this->before_input_unicode_vertices_size);
		}
		if (this->before_input_unicode_outline_vertices_size != qpl::size_max) {
			this->colored_text.unicode_outline_vertices.resize(this->before_input_unicode_outline_vertices_size);
		}

		this->colored_text.rows = this->before_input_text_rows;
		this->colored_text.add(this->input_string);
		//this->reset_visible_range();
		this->update_cursor_position(false);
	}
	void qsf::console::add_text_input(const qpl::u32_string& string, bool end_of_line) {
		auto size = this->string_split.size();
		auto pos = this->cursor_position;

		if (end_of_line) {
			pos.x = this->string_and_input_split[pos.y].length() - 1;
		}
		else {
			pos.x = qpl::max(0ll, qpl::signed_cast(pos.x));
		}

		auto count = qpl::count(qpl::to_basic_string<wchar_t>(string), L'\n');
		if (count) {
			auto split = qpl::string_split_allow_empty(qpl::to_basic_string<wchar_t>(string), L'\n');
			this->cursor_position.y += count;
			if (split.size()) {
				this->cursor_position.x = split.back().length();
			}
			else {
				this->cursor_position.x = 0u;
			}
		}
		else {
			this->cursor_position.x += string.length();
		}

		this->input_string.add_text_at(pos, string);

		this->update_input_text_graphics();
		this->update_input_string_split();
		this->update_cursor_position(true);
	}
	void qsf::console::pop_character_at_cursor() {
		if (this->input_string.size() && this->input_string.elements.back().text.length()) {

			auto size = this->string_split.size();
			auto pos = this->cursor_position;
			pos.x = qpl::max(0ll, qpl::signed_cast(pos.x));

			if (this->cursor_position.x) {
				--this->cursor_position.x;
				this->update_cursor_position(true);
			}
			else if (this->cursor_position.y) {
				--this->cursor_position.y;
				this->cursor_position.x = this->get_input_text_width(this->cursor_position.y);
				this->update_cursor_position(true);
			}
			this->input_string.remove_character_at(pos);
			this->update_input_string_split();
			this->update_input_text_graphics();
		}
	}
	void qsf::console::update_cursor_dimension() {
		this->cursor.set_dimension(qpl::vec(3.0f, this->colored_text.get_line_spacing_pixels()));
	}
	void qsf::console::update_text_range() {
		auto y = this->view.position.y / this->character_size.y;
		auto min_y = y;
		auto max_y = y + this->visible_rows;

		if (min_y < this->visible_y_min || max_y > this->visible_y_max) {
			this->visible_y_min = min_y - visible_buffer;
			this->visible_y_max = max_y + visible_buffer;

			auto a = this->visible_y_min * this->character_size.y;
			auto b = this->visible_y_max * this->character_size.y;

			this->colored_text.create(this->string_and_input, a, b);
		}
	}
	void qsf::console::process_character_size() {

		this->reset_visible_range();

		if (this->accept_input) {
			this->colored_text.add(this->input_string);
		}

		auto text_pos_before = this->before_input_text_position / this->character_size;

		this->scroll_bar.set_progress(this->view_row / qpl::f64_cast(this->colored_text.rows - this->visible_rows));
		this->scroll_bar.set_visible_knob_progress(this->view_row / qpl::f64_cast(this->scroll_bar.integer_step));
		this->view.position.y = this->view_row * this->colored_text.get_line_spacing_pixels();
		this->calculate_default_character_size();

		auto pos = text_pos_before * this->character_size;
		pos.x = 0.f;
		this->before_input_text_position = pos;

		this->update_input_string_split();
		this->update_input_text_graphics();
		this->update_cursor_dimension();
		this->update_cursor_position(false);
		this->make_selection_rectangles();


		this->update_visible_rows_count();
		this->clamp_view_y(false);
		this->prepare_scroll();
		this->end_animation();
	}
	qpl::size qsf::console::get_text_height() const {
		return this->string_and_input_split.size();
	}
	qpl::size qsf::console::get_text_width(qpl::size y) const {
		if (y >= this->string_and_input_split.size()) {
			return 0u;
		}
		return this->string_and_input_split[y].length();
	}
	qpl::size qsf::console::get_input_text_width(qpl::size y) const {
		if (y >= this->input_string_split.size()) {
			return 0u;
		}
		return this->input_string_split[y].length();
	}
	qpl::size qsf::console::get_input_text_height() const {
		return this->input_string_split.size();
	}
	qpl::vec2f qsf::console::position_to_text_position(qpl::vec2f position) const {
		return position / this->character_size;
	}
	void qsf::console::clear_selection_rectangles_if_visible() {
		if (this->selection_rectangle.size()) {
			this->selection_rectangle.clear();
		}
	}
	auto qsf::console::get_selection_rectangle_bounds() const {
		auto fmin = this->selection_rectangle_start;
		auto fmax = this->selection_rectangle_end;

		fmin.y = qpl::max(0.f, fmin.y);
		fmin.x = qpl::max(0.f, fmin.x);
		fmax.y = qpl::max(0.f, fmax.y);
		fmax.x = qpl::max(0.f, fmax.x);

		if (qpl::isize_cast(fmax.y) < qpl::isize_cast(fmin.y)) {
			std::swap(fmin, fmax);
		}
		else if (qpl::isize_cast(fmin.y) == qpl::isize_cast(fmax.y) && fmax.x < fmin.x) {
			std::swap(fmin, fmax);
		}

		auto min = qpl::vec2is{ std::round(fmin.x), fmin.y };
		auto max = qpl::vec2is{ std::round(fmax.x), qpl::min(qpl::size_cast(fmax.y), this->get_text_height() ? this->get_text_height() - 1 : 0ull)};
		return std::make_pair(min, max);
	}
	std::wstring qsf::console::get_selection_rectangle_string() const {
		std::wstring result = L"";

		auto [min, max] = this->get_selection_rectangle_bounds();

		for (qpl::size y = min.y; y <= max.y; ++y) {
			if (min.y == max.y) {
				if (min.x <= this->string_and_input_split[y].length()) {
					auto size = max.x - min.x;
					result += this->string_and_input_split[y].substr(min.x, size);
				}
			}
			else if (y == min.y) {
				if (min.x <= this->string_and_input_split[y].length()) {
					result += this->string_and_input_split[y].substr(min.x);
				}
				result += L'\n';
			}
			else if (y == max.y) {
				if (max.x && max.x <= this->string_and_input_split[y].length()) {
					result += this->string_and_input_split[y].substr(0u, max.x);
				}
			}
			else {
				if (this->string_and_input_split[y].size()) {
					result += this->string_and_input_split[y];
				}
				result += L'\n';
			}
		}
		return result;
	}
	void qsf::console::make_selection_rectangles() {
		auto [min, max] = this->get_selection_rectangle_bounds();

		max.y = qpl::min(max.y, qpl::signed_cast(this->get_text_height() - 1));

		auto size = qpl::max(0ll, (max.y - min.y) + 1);
		this->selection_rectangle.resize(size);

		for (qpl::isize i = min.y; i <= max.y; ++i) {
			auto index = i - min.y;
			auto pos = qpl::vec2s(0ull, i);
			qpl::size width = 0u;

			if (min.y == max.y) {
				auto text_width = this->get_text_width(max.y);
				min.x = qpl::min(min.x, qpl::signed_cast(text_width));
				max.x = qpl::min(max.x, qpl::signed_cast(text_width));
				pos.x = min.x;
				width = (max.x - min.x) * this->colored_text.get_white_space_width();
			}
			else if (i == min.y) {
				auto text_width = this->get_text_width(min.y);
				min.x = qpl::min(min.x, qpl::signed_cast(text_width));
				pos.x = min.x;
				width = this->dimension.x - (pos.x * this->character_size.x);
			}
			else if (i == max.y) {
				auto text_width = this->get_text_width(max.y);
				max.x = qpl::min(max.x, qpl::signed_cast(text_width));
				width = max.x * this->colored_text.get_white_space_width();
			}
			else {
				width = this->dimension.x;
			}

			auto offset_y = this->colored_text.get_chracter_top_offset() - this->colored_text.get_character_size() / 10.f;
			this->selection_rectangle[index].set_position(pos * this->character_size + qpl::vec(0.f, offset_y));
			this->selection_rectangle[index].set_dimension(qpl::vec(width, this->colored_text.get_line_spacing_pixels()));
			this->selection_rectangle[index].set_color(this->selection_rectangle_color);
		}
	}
	void qsf::console::move_to_input() {
		auto input_pos = qpl::isize_cast(this->colored_text.rows - this->visible_rows) + 1;
		auto input_pos_end = qpl::isize_cast(this->colored_text.rows);
		if (input_pos_end < this->visible_rows) {
			input_pos_end = 0;
		}
		if (this->view_row < input_pos) {
			this->view_row = input_pos;
			this->prepare_scroll();
		}
		if (this->view_row > input_pos_end) {
			this->view_row = input_pos_end;
			this->prepare_scroll();
		}
	}
	auto qsf::console::get_input_text() const {
		return this->input_string.string();
	}
	std::wstring qsf::console::get_last_input_line() const {
		if (this->input_string_split.empty()) {
			return L"";
		}
		return this->input_string_split[this->input_string_split.size() - 2u];
	}
	void qsf::console::process_text() {
		this->update_visible_rows_count();
		this->scroll_bar.set_progress(this->view_row / qpl::f64_cast(this->scroll_bar.integer_step));
		this->update_cursor_position();

		auto size = this->colored_text.vertices.size() + this->colored_text.outline_vertices.size();
		this->update_string_split();
		this->update_string_and_input_split();
	}
	void qsf::console::add(const qpl::styled_string<qpl::u32_string>& string) {
		this->colored_text.add(string);
		this->string << string;
		this->process_text();
	}
	void qsf::console::create(const qpl::styled_string<qpl::u32_string>& string) {
		this->colored_text.clear();
		this->string.clear();
		this->add(string);
		this->update_cursor_position(true);
	}
	void qsf::console::update_key_input(const qsf::event_info& event) {
		this->line_entered = false;
		this->text_entered = false;
		this->exited_enter_to_continue = false;
		if (!this->accept_input) {
			return;
		}
		bool special_input = false;

		if (!this->enter_to_continue) {
			if (event.key_holding(sf::Keyboard::LControl)) {
				special_input = true;
				if (event.key_pressed(sf::Keyboard::V)) {
					this->clear_selection_rectangles_if_visible();
					this->add_text_input(qpl::to_u32_string(qsf::copy_from_clipboard()));
					this->text_entered = true;
					this->update_visible_rows_count();
					this->clear_selection_rectangles_if_visible();
				}
				if (event.key_pressed(sf::Keyboard::C)) {
					qsf::copy_to_clipboard(this->get_selection_rectangle_string());
					this->clear_selection_rectangles_if_visible();
				}
				if (event.key_pressed(sf::Keyboard::A)) {
					this->selection_rectangle_start = { 0, 0 };
					this->selection_rectangle_end = qpl::vec2f(this->string_and_input_split.back().length(), this->get_text_height());
					this->make_selection_rectangles();
				}
			}
			if (event.key_pressed(sf::Keyboard::Backspace)) {
				this->clear_selection_rectangles_if_visible();

				if (this->allow_going_up_with_cursor || this->cursor_position.x) {
					this->pop_character_at_cursor();
				}
				special_input = true;
			}

			if (event.key_pressed(sf::Keyboard::Right)) {
				this->clear_selection_rectangles_if_visible();
				if (this->cursor_position.x < this->get_input_text_width(this->cursor_position.y)) {
					++this->cursor_position.x;
					this->update_cursor_position(true);
				}
				else if (qpl::signed_cast(this->cursor_position.y) < qpl::signed_cast(this->get_input_text_height()) - 1) {
					++this->cursor_position.y;
					this->cursor_position.x = 0u;
					this->update_cursor_position(true);
				}
			}
			if (event.key_pressed(sf::Keyboard::Left)) {
				this->clear_selection_rectangles_if_visible();
				if (this->cursor_position.x) {
					--this->cursor_position.x;
					this->update_cursor_position(true);
				}
				else if (this->allow_going_up_with_cursor && this->cursor_position.y) {
					--this->cursor_position.y;
					this->cursor_position.x = this->get_input_text_width(this->cursor_position.y);
					this->update_cursor_position(true);
				}
			}
			if (event.key_pressed(sf::Keyboard::Up)) {
				this->clear_selection_rectangles_if_visible();
				if (!this->allow_going_up_with_cursor || this->get_input_text_height() <= 1u) {

					if (this->input_history_index) {
						--this->input_history_index;
					}

					if (this->input_history_index < this->input_history.size()) {
						this->input_string.clear_last_line();
						this->update_input_text_graphics();
						this->update_input_string_split();

						this->cursor_position.x = 0u;
						const auto& history_text = this->input_history[this->input_history_index];

						this->add_text_input(qpl::to_u32_string(history_text));
						this->cursor_position.x = history_text.length();

						this->update_input_text_graphics();
						this->update_input_string_split();
						this->update_cursor_position(true);
					}
				}
				else if (this->allow_going_up_with_cursor && this->cursor_position.y) {
					--this->cursor_position.y;
					this->cursor_position.x = qpl::min(this->cursor_position.x, this->get_input_text_width(this->cursor_position.y));
					this->update_cursor_position(true);
				}
				special_input = true;
			}
			if (event.key_pressed(sf::Keyboard::Down)) {
				this->clear_selection_rectangles_if_visible();
				if (!this->allow_going_up_with_cursor || this->get_input_text_height() <= 1u) {
					if (this->input_history.empty() || (this->input_history_index >= this->input_history.size() - 1)) {
						this->input_string.clear_last_line();
						this->update_input_string_split();
						this->update_input_text_graphics();
						this->cursor_position.x = 0u;
						this->update_cursor_position(true);
					}
					else {
						++this->input_history_index;
						this->input_string.clear_last_line();
						const auto& history_text = this->input_history[this->input_history_index];
						this->add_text_input(qpl::to_u32_string(history_text));
						this->cursor_position.x = history_text.length();
						this->update_input_text_graphics();
						this->update_input_string_split();
						this->update_cursor_position(true);
					}
				}
				else if (qpl::signed_cast(this->cursor_position.y) < qpl::signed_cast(this->get_input_text_height()) - 1) {
					++this->cursor_position.y;
					this->cursor_position.x = qpl::min(this->cursor_position.x, this->get_input_text_width(this->cursor_position.y));
					this->update_cursor_position(true);
				}
				special_input = true;
			}
			if (event.key_pressed(sf::Keyboard::Escape)) {
				special_input = true;
				this->input_string.clear_last_line();
				this->update_input_string_split();
				this->update_input_text_graphics();
				this->cursor_position.x = 0u;
				this->update_cursor_position(true);
			}
		}
		if (event.key_pressed(sf::Keyboard::Enter)) {
			this->add_text_input(qpl::to_u32_string('\n'), !this->allow_going_up_with_cursor);
			special_input = true;

			if (this->enter_to_continue) {
				this->enter_to_continue = false;
				this->exited_enter_to_continue = true;
			}
			this->line_entered = true;
			this->text_entered = true;
			auto last_input = this->get_last_input_line();
			if (!last_input.empty()) {
				this->input_history.push_back(this->get_last_input_line());
				this->input_history_index = this->input_history.size();
			}
		}
		if (!this->enter_to_continue && event.is_text_entered() && !special_input) {
			this->add_text_input(event.u32_text_entered());
			this->text_entered = true;
		}
	}
	void qsf::console::update_selection_rectangle(const qsf::event_info& event) {
		if (event.left_mouse_clicked() && this->allow_text_dragging) {
			this->clicked_mouse_position = event.mouse_position();
			this->text_dragging = true;

			this->selection_rectangle_start = this->position_to_text_position(event.mouse_position() + this->view.position);
		}
		if (this->text_dragging) {
			this->selection_rectangle_end = this->position_to_text_position(event.mouse_position() + this->view.position);

			auto zoom_factor = qpl::max(1ll, 2 + this->zooms / 2);
			if (this->selection_rectangle_end.y < this->view_row) {
				this->view_row -= zoom_factor;
				this->view_row = qpl::max(0ll, this->view_row);
				this->prepare_scroll();
			}
			if (this->selection_rectangle_end.y > qpl::signed_cast(this->view_row + (this->visible_rows - 2))) {
				this->view_row += zoom_factor;
				this->prepare_scroll();
			}
			this->make_selection_rectangles();
		}

		if (event.left_mouse_released()) {
			this->text_dragging = false;
		}
	}
	void qsf::console::update_cursor() {

		auto f = this->cursor_blink_timer.elapsed_f();
		qpl::f64 progress = 1.0;
		if (f > 0.5) {
			auto time = (f - 0.5) / this->cursor_interval_duration;
			progress = 1.0 - qpl::triangle_progression(std::fmod(time, 1.0));
		}
		this->cursor.set_color(this->cursor.get_color().with_alpha(progress * 255));
	}
	void qsf::console::update(const qsf::event_info& event) {
		this->scroll_bar.allow_dragging = !this->text_dragging;

		event.update(this->scroll_bar);
		this->allow_text_dragging = !(this->scroll_bar.hovering || this->scroll_bar.hovering_background);
		this->update_selection_rectangle(event);

		if (this->scroll_bar.value_changed) {
			this->view_row = this->scroll_bar.get_progress_step();
			this->scroll_bar_transition_start = this->scroll_bar_transition_end = this->scroll_bar.visible_knob_progress;
			this->clamp_view_y();
		}
		if (this->scroll_bar.released_dragging) {
			this->scroll_bar_transition_start = this->scroll_bar_transition_end = this->scroll_bar.visible_knob_progress;
			this->prepare_scroll();
		}
		if (this->scroll_bar.clicked_on_background_above) {
			--this->view_row;
			this->prepare_scroll();
		}
		if (this->scroll_bar.clicked_on_background_below) {
			++this->view_row;
			this->prepare_scroll();
		}

		if (this->visible_rows) {
			if (event.key_holding(sf::Keyboard::LControl)) {
				if (event.scrolled_down()) {
					if (this->colored_text.character_size > 2) {
						this->end_animation();

						this->colored_text.character_size--;
						this->process_character_size();
						++this->zooms;
					}

				}
				if (event.scrolled_up()) {
					this->end_animation();

					this->colored_text.character_size++;
					this->process_character_size();
					--this->zooms;
				}
			}
			else {
				if (event.scrolled_down()) {
					this->view_row += qpl::max(1ll, (3 + this->zooms / 2));
					this->view_row = qpl::min(this->view_row, qpl::isize_cast(this->colored_text.rows - 3u));
					this->prepare_scroll();
				}
				if (event.scrolled_up()) {
					this->view_row -= qpl::max(1ll, (3 + this->zooms / 2));
					this->prepare_scroll();
				}
			}
		}

		if (event.key_pressed(sf::Keyboard::End)) {
			this->move_to_input();
		}

		this->update_key_input(event);

		if (this->text_entered) {
			this->move_to_input();
		}
		if (this->line_entered) {
			this->update_visible_rows_count();
		}

		this->scroll_transition_animation.update(event);
		if (this->scroll_transition_animation.is_running()) {
			auto p = this->scroll_transition_animation.get_progress();
			auto curve = qpl::smooth_slope(p);
			this->view.position.y = qpl::linear_interpolation(this->scroll_transition_start, this->scroll_transition_end, curve);

			if (!this->scroll_bar.dragging) {
				this->scroll_bar.set_visible_knob_progress(qpl::linear_interpolation(this->scroll_bar_transition_start, this->scroll_bar_transition_end, curve));
			}
		}
		if (this->scroll_transition_animation.just_finished()) {
			this->scroll_transition_start = this->scroll_transition_end;
			this->scroll_bar_transition_start = this->scroll_bar_transition_end;
		}
		this->update_cursor();
		this->update_text_range();


		if (event.key_holding(sf::Keyboard::LShift) && event.holding_middle_mouse()) {
			this->view.move(-event.delta_mouse_position());
		}
	}
	void qsf::console::draw(qsf::draw_object& draw) const {
		auto copy = this->view;
		copy.position.y += this->colored_text.get_chracter_top_offset();
		if (this->accept_input) {
			draw.draw(this->cursor, copy);
		}
		draw.draw(this->colored_text, copy);
		draw.draw(this->selection_rectangle, copy);

		if (this->border_texture_set && this->scroll_bar.knob_range < 1.0f) {
			//if (this->view_row > 1) {
			//	draw.draw(this->shadow_border);
			//}
			draw.draw(this->scroll_bar_shadow_border);
		}
		draw.draw(this->scroll_bar);
	}

}

#endif