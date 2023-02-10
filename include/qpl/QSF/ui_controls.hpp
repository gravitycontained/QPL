#ifndef QSF_UI_CONTROLS_HPP
#define QSF_UI_CONTROLS_HPP
#pragma once

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vector.hpp>
#include <qpl/color.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/drawables.hpp>

namespace qsf {

	struct button;
	namespace detail {
		QPLDLL extern qsf::button button;
	}

	struct vbutton {

		vbutton() {
			this->background.set_color(qpl::rgba::black());
		}
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f center);
		QPLDLL void set_background_color(qpl::rgba color);
		QPLDLL void set_hover_background_color(qpl::rgba color);
		QPLDLL void set_text_color(qpl::rgba color);
		QPLDLL void set_hover_text_color(qpl::rgba color);
		QPLDLL void set_text_font(std::string font);
		QPLDLL void set_text_character_size(qpl::u32 character_size);
		QPLDLL void set_text_style(qpl::u32 character_style);
		QPLDLL void set_text(std::string text);
		QPLDLL void centerize_text();
		QPLDLL bool is_hovering() const;
		QPLDLL bool is_clicked() const;
		QPLDLL void update(const event_info& event_info);
		QPLDLL void set_text_alpha(qpl::u8 alpha);
		QPLDLL void set_background_alpha(qpl::u8 alpha);

		template<typename T>
		void update(const event_info& event_info) {

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

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::vbutton& operator=(const qsf::vbutton& button);

		qpl::rgba text_color;
		qpl::rgba hover_text_color = qpl::rgba::unset();
		qpl::rgba hover_background_color = qpl::rgba::unset();
		qpl::rgba background_color;
		qsf::vtext text;
		qsf::vrectangle background;
		bool hovering = false;
		bool clicked = false;
		bool outline_on_hover = true;
		bool invert_on_hover = true;
		qpl::u8 text_alpha = 255;
		qpl::u8 background_alpha = 255;
	};

	struct button {

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::button& operator=(const qsf::vbutton& button);


		button() {
			this->background.set_color(qpl::rgba::black());
		}

		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f center);
		QPLDLL void set_background_color(qpl::rgba color);
		QPLDLL void set_hover_background_color(qpl::rgba color);
		QPLDLL void set_text_color(qpl::rgba color);
		QPLDLL void set_text_font(std::string font);
		QPLDLL void set_text_character_size(qpl::u32 character_size);
		QPLDLL void set_text_style(qpl::u32 character_style);
		QPLDLL void set_text_string(std::string text);
		QPLDLL void set_text_string(std::wstring text);

		QPLDLL qpl::vector2f get_dimension() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qpl::vector2f get_center() const;
		QPLDLL qpl::rgba get_background_color() const;
		QPLDLL qpl::rgba get_hover_background_color() const;
		QPLDLL qpl::rgba get_text_color() const;
		QPLDLL std::string get_text_font() const;
		QPLDLL qpl::u32 get_text_character_size() const;
		QPLDLL qpl::u32 get_text_style() const;
		QPLDLL std::string get_text_string() const;
		QPLDLL std::wstring get_text_wstring() const;
		QPLDLL void centerize_text();
		QPLDLL bool is_hovering() const;
		QPLDLL bool is_clicked() const;
		QPLDLL void update(const event_info& event_info);
		QPLDLL void update(const event_info& event_info, bool& hovering);

		template<typename T>
		void update(const event_info& event_info) {

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

		bool outline_on_hover = true;
		bool invert_on_hover = true;
		qpl::rgba text_color = qpl::rgba::white();
		qpl::rgba hover_background_color = qpl::rgba::white();
		qpl::rgba background_color = qpl::rgba::black();
		qsf::text text;
		qsf::rectangle background;
		bool hovering = false;
		bool clicked = false;
	};

	struct smooth_button {
		qsf::text text;
		qsf::vsmooth_rectangle smooth_layout;
		mutable qsf::smooth_rectangle rectangle;
		qpl::vector2f hitbox_increase;
		qpl::animation hover_animation;
		qpl::rgba background_color = qpl::rgba::black();
		qpl::rgba background_outline_color = qpl::rgba::black();
		qpl::rgba text_color = qpl::rgba::white();
		mutable bool layout_changed = false;
		bool simple_hitbox = false;
		bool hovering = false;
		bool clicked = false;
		bool use_basic_hover_animation = true;
		bool hover_before = false;

		smooth_button() {
			this->set_background_color(qpl::rgba::black());
			this->hover_animation.set_duration(0.1);
			this->enable_simple_hitbox();
		}

		QPLDLL void enable_simple_hitbox();
		QPLDLL void disable_simple_hitbox();
		QPLDLL void set_hitbox_increase(qpl::vector2f delta);
		QPLDLL void set_hitbox_increase(qpl::f32 delta);
		QPLDLL void set_color(qpl::rgba color);
		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_hitbox(qpl::hitbox hitbox);
		QPLDLL void set_center(qpl::vector2f position);
		QPLDLL void set_slope(qpl::f64 slope);
		QPLDLL void set_background_color(qpl::rgba color);
		QPLDLL void set_background_outline_thickness(qpl::f32 thickness);
		QPLDLL void set_background_outline_color(qpl::rgba color);
		QPLDLL void set_background_slope_dimension(qpl::vector2f dimension);
		QPLDLL void set_background_slope_point_count(qpl::size point_count);
		QPLDLL void set_text_font(const sf::Font& font);
		QPLDLL void set_text_font(const std::string& font_name);
		QPLDLL void set_text_style(qpl::u32 style);
		QPLDLL void set_text_character_size(qpl::u32 character_size);
		QPLDLL void set_text_color(qpl::rgba color);
		QPLDLL void set_text_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_text_outline_color(qpl::rgba color);
		QPLDLL void set_text_rotation(qpl::f32 angle);
		QPLDLL void set_text_letter_spacing(qpl::f32 spacing);
		QPLDLL void set_text_position(qpl::vector2f position);
		QPLDLL void set_text_center(qpl::vector2f position);
		QPLDLL void set_text_string(const std::string& string);
		QPLDLL void set_text_string(const std::wstring& string);
		QPLDLL void centerize_text();

		QPLDLL qpl::vector2f get_hitbox_increase() const;
		QPLDLL qpl::vector2f get_dimension() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qpl::vector2f get_center() const;
		QPLDLL qpl::hitbox get_hitbox() const;
		QPLDLL qpl::f64 get_slope() const;
		QPLDLL qpl::rgba get_background_color() const;
		QPLDLL qpl::f32 get_background_outline_thickness() const;
		QPLDLL qpl::rgba get_background_outline_color() const;
		QPLDLL qpl::vector2f get_slope_dimension() const;
		QPLDLL qpl::size get_slope_point_count() const;
		QPLDLL std::string get_text_font() const;
		QPLDLL qpl::u32 get_text_style() const;
		QPLDLL qpl::u32 get_text_character_size() const;
		QPLDLL qpl::rgba get_text_color() const;
		QPLDLL qpl::f32 get_text_outline_thickness() const;
		QPLDLL qpl::rgba get_text_outline_color() const;
		QPLDLL qpl::f32 get_text_letter_spacing() const;
		QPLDLL qpl::vector2f get_text_position() const;
		QPLDLL qpl::vector2f get_text_center() const;
		QPLDLL std::string get_text_string() const;
		QPLDLL std::wstring get_text_wstring() const;
		QPLDLL bool is_hovering() const;
		QPLDLL bool is_clicked() const;

		QPLDLL void update(const qsf::event_info& event);

		QPLDLL void create_check() const;
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
	};


	struct text_field {

		struct line {
			qsf::text text;
			std::vector<qpl::hitbox> character_hitboxes;
			std::vector<qpl::hitbox> character_mouse_hitboxes;
			qpl::hitbox line_hitbox;
			qpl::hitbox line_mouse_hitbox;
			bool mouse_hitboxes_changed = false;
			bool layout_changed = false;

			QPLDLL void apply(qsf::vtext layout);
			QPLDLL void calculate_hitboxes();
			QPLDLL void calculate_mouse_hitboxes(qpl::f32 max_line_width, qpl::f32 extended_line_width);
			QPLDLL void move(qpl::vector2f delta);
			QPLDLL void move(qpl::f32 x, qpl::f32 y);
			QPLDLL std::wstring wstring() const;
			QPLDLL void draw(qsf::draw_object& draw) const;
		};

		text_field();

		QPLDLL void set_font(std::string font);
		QPLDLL void set_text_character_size(qpl::u32 character_size);
		QPLDLL void set_text_style(qpl::u32 style);
		QPLDLL void set_text_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_text_outline_color(qpl::rgba outline_color);
		QPLDLL void set_text_color(qpl::rgba color);

		QPLDLL qpl::hitbox get_hitbox() const;
		QPLDLL qpl::hitbox get_background_hitbox() const;

		QPLDLL void set_background_color(qpl::rgba color);
		QPLDLL void set_background_increase(qpl::vector2f increase);
		QPLDLL void set_background_increase(qpl::f32 increase);
		QPLDLL void set_background_outline_thickness(qpl::f32 thickness);
		QPLDLL void set_background_outline_color(qpl::rgba color);
		QPLDLL qpl::rgba get_background_color() const;
		QPLDLL qpl::rgba get_background_outline_color() const;
		QPLDLL qpl::f32 get_background_outline_thickness() const;
		QPLDLL qpl::vector2f get_background_increase() const;

		QPLDLL void set_minimum_dimension(qpl::vector2f dimension);
		QPLDLL void set_minimum_width(qpl::f32 width);
		QPLDLL void set_minimum_height(qpl::f32 height);
		QPLDLL void set_maximum_dimension(qpl::vector2f dimension);
		QPLDLL void set_maximum_width(qpl::f32 width);
		QPLDLL void set_maximum_height(qpl::f32 height);
		QPLDLL void set_fixed_dimension(qpl::vector2f dimension);
		QPLDLL void set_fixed_width(qpl::f32 width);
		QPLDLL void set_fixed_height(qpl::f32 height);

		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_hitbox(qpl::hitbox hitbox);
		QPLDLL void move(qpl::vec2f delta);

		QPLDLL void set_string(std::wstring string);
		QPLDLL void set_string(std::string string);

		QPLDLL qpl::f32 get_line_height() const;
		QPLDLL bool is_hovering() const;

		QPLDLL bool just_edited_text() const;
		QPLDLL bool just_copied_text() const;
		QPLDLL bool just_pasted_text() const;
		QPLDLL bool just_entered_text() const;
		QPLDLL bool just_finished_text() const;
		QPLDLL bool has_focus() const;

		QPLDLL std::wstring wstring() const;
		QPLDLL std::string string() const;

		QPLDLL std::string get_font() const;
		QPLDLL bool is_font_set() const;

		QPLDLL void new_line(qpl::size y = qpl::size_max);
		QPLDLL qsf::text_field::line& get_line_at_cursor();
		QPLDLL const qsf::text_field::line& get_line_at_cursor() const;

		QPLDLL std::wstring get_selection_wstring() const;
		QPLDLL std::string get_selection_string() const;
		QPLDLL void add_string_at_cursor(std::wstring string);

		QPLDLL void enable_focus();
		QPLDLL void disable_focus();

		QPLDLL void enable_one_line_limit();
		QPLDLL void set_line_limit(qpl::size limit);
		QPLDLL void disable_one_line_limit();

		QPLDLL void set_string_stack_size(qpl::size size);
		QPLDLL qpl::size get_string_stack_size() const;

		QPLDLL bool is_dragging_selection_considering_cooldown() const;

		QPLDLL void delete_at_cursor();
		QPLDLL void delete_selection_string(bool update_cursor_position = true);
		QPLDLL void delete_selection_range();
		QPLDLL void move_cursor_up();
		QPLDLL void move_cursor_down();
		QPLDLL void move_cursor_left();
		QPLDLL void move_cursor_right();
		QPLDLL void update_cursor();
		QPLDLL void update_selection_rectangles();
		QPLDLL void update_mouse_events(const qsf::event_info& event);

		QPLDLL void update(const qsf::event_info& event);
		QPLDLL void draw(qsf::draw_object& draw) const;

		QPLDLL std::pair<qpl::vector2s, qpl::vector2s> get_sorted_selection_range() const;
		QPLDLL void internal_update() const;
		QPLDLL void internal_update_text() const;
		QPLDLL void internal_update_cursor();
		QPLDLL void set_cursor_dimension() const;
		QPLDLL void set_cursor_position();
		QPLDLL void make_selection_rectangles() const;
		QPLDLL void make_selection_rectangles(qpl::size& count, qpl::size y, qpl::size x1, qpl::size x2 = qpl::size_max) const;
		QPLDLL void find_closest_cursor_position();
		QPLDLL bool just_changed() const;

		qsf::vtext text_layout;
		mutable qpl::hitbox hitbox;
		mutable qpl::vector<line> lines;
		mutable qsf::smooth_rectangle background;
		mutable qsf::rectangle cursor;
		mutable std::vector<qsf::rectangle> selection_rectangles;
		mutable std::wstring whole_string;

		qpl::circular_array<std::pair<std::wstring, qpl::vector2s>> string_stack;

		qpl::vector2s cursor_position = qpl::vec(0u, 0u);
		qpl::vector2s selection_a = qpl::vec(0u, 0u);
		qpl::vector2s selection_b = qpl::vec(0u, 0u);
		qpl::vector2f background_increase = qpl::vec(6.0f, 6.0f);
		qpl::vector2f click_mouse_position = qpl::vec(0, 0);
		qpl::vector2f hovering_increase = qpl::vec(5, 5);

		qpl::small_clock selection_drag_timer;
		qpl::small_clock selection_start_drag_timer;
		qpl::small_clock cursor_interval_timer;
		qpl::halted_clock double_click_cooldown;
		qpl::f64 selection_drag_timer_duration = 0.15;
		qpl::f64 selection_start_drag_timer_duration = 0.15;

		qpl::rgba selection_color = qpl::rgb(0, 117, 210);
		qpl::rgba dragging_color = qpl::rgb(200, 150, 0);

		qpl::size string_stack_position = 0u;
		qpl::f64 cursor_interval_duration = 1.0;
		qpl::f64 cursor_blink_duration = 0.5;
		qpl::f32 cursor_x_offset = 0.3f;
		qpl::f32 cursor_width_percentage = 0.1f;

		qpl::vec2f minimum_dimension = { 0, 0 };
		qpl::vec2f maximum_dimension = { qpl::f64_max, qpl::f64_max };

		qpl::size line_limit = qpl::size_max;

		bool hovering = false;
		bool dragging = false;
		bool dragging_selection = false;
		bool dragging_selection_before = false;
		bool edited_text = false;
		bool copied_text = false;
		bool pasted_text = false;
		bool entered_text = false;
		bool focus = false;
		bool finished_text = false;
		bool control_z = false;
		bool control_y = false;
		bool changed = false;

	private:
		mutable bool text_mouse_hitboxes_changed = false;
		mutable bool text_layout_changed = false;
		mutable bool update_required = false;
		mutable bool whole_string_changed = true;
		mutable bool cursor_position_changed = false;
	};

	template<typename T>
	class slider {
	public:
		slider() {
			this->ptr = &this->dummy;
			this->set_colors(qpl::rgba::grey_shade(50), qpl::rgba::white());
		}
		slider(const slider<T>& other) {
			*this = other;
		}

		template<typename U>
		slider(const slider<U>& other) {
			*this = other;
		}

		slider<T>& operator=(const slider<T>& other) {
			this->background = other.background;
			this->knob = other.knob;
			this->position = other.position;
			this->knob_visible = other.knob_visible;
			this->background_visible = other.background_visible;
			this->knob_hover_effect = other.knob_hover_effect;
			this->background_hover_effect = other.background_hover_effect;
			this->knob_hover_color = other.knob_hover_color;
			this->knob_hover_outline_thickness = other.knob_hover_outline_thickness;
			this->knob_hover_outline_color = other.knob_hover_outline_color;
			this->hitbox_increase = other.hitbox_increase;
			this->background_hover_color = other.background_hover_color;
			this->background_hover_outline_thickness = other.background_hover_outline_thickness;
			this->background_hover_outline_color = other.background_hover_outline_color;
			this->start = other.start;
			this->end = other.end;
			this->dummy = other.dummy;
			this->value_before = other.value_before;
			this->background_color = other.background_color;
			this->background_outline_thickness = other.background_outline_thickness;
			this->background_outline_color = other.background_outline_color;
			this->knob_color = other.knob_color;
			this->knob_outline_thickness = other.knob_outline_thickness;
			this->knob_outline_color = other.knob_outline_color;
			this->smooth_input_key = other.smooth_input_key;
			this->smooth_input_multiply = other.smooth_input_multiply;
			this->click_knob_pos_x = other.click_knob_pos_x;
			this->click_pos_x = other.click_pos_x;
			this->range_pixel_width = other.range_pixel_width;
			this->hovering_over_background = other.hovering_over_background;
			this->click_on_background_to_skip = other.click_on_background_to_skip;
			this->input_smooth_values = other.input_smooth_values;
			this->value_changed = other.value_changed;
			this->value_modified = other.value_modified;
			this->range_set = other.range_set;
			this->allow_change = other.allow_change;
			this->allow_drag = other.allow_drag;
			this->dragging = other.dragging;
			this->released = other.released;
			this->hovering_over_knob = other.hovering_over_knob;
			this->ptr = &this->dummy;
			this->string_function = other.string_function;
			this->text = other.text;

			return *this;
		}
		template<typename U>
		slider<T>& operator=(const slider<U>& other) {
			this->background = other.background;
			this->knob = other.knob;
			this->position = other.position;
			this->knob_visible = other.knob_visible;
			this->background_visible = other.background_visible;
			this->knob_hover_effect = other.knob_hover_effect;
			this->background_hover_effect = other.background_hover_effect;
			this->knob_hover_color = other.knob_hover_color;
			this->knob_hover_outline_thickness = other.knob_hover_outline_thickness;
			this->knob_hover_outline_color = other.knob_hover_outline_color;
			this->hitbox_increase = other.hitbox_increase;
			this->background_hover_color = other.background_hover_color;
			this->background_hover_outline_thickness = other.background_hover_outline_thickness;
			this->background_hover_outline_color = other.background_hover_outline_color;
			this->start = static_cast<T>(other.start);
			this->end = static_cast<T>(other.end);
			this->dummy = static_cast<T>(other.dummy);
			this->value_before = static_cast<T>(other.value_before);
			this->background_color = other.background_color;
			this->background_outline_thickness = other.background_outline_thickness;
			this->background_outline_color = other.background_outline_color;
			this->knob_color = other.knob_color;
			this->knob_outline_thickness = other.knob_outline_thickness;
			this->knob_outline_color = other.knob_outline_color;
			this->smooth_input_key = other.smooth_input_key;
			this->smooth_input_multiply = other.smooth_input_multiply;
			this->click_knob_pos_x = other.click_knob_pos_x;
			this->click_pos_x = other.click_pos_x;
			this->range_pixel_width = other.range_pixel_width;
			this->hovering_over_background = other.hovering_over_background;
			this->click_on_background_to_skip = other.click_on_background_to_skip;
			this->input_smooth_values = other.input_smooth_values;
			this->value_changed = other.value_changed;
			this->value_modified = other.value_modified;
			this->range_set = other.range_set;
			this->allow_change = other.allow_change;
			this->allow_drag = other.allow_drag;
			this->dragging = other.dragging;
			this->released = other.released;
			this->hovering_over_knob = other.hovering_over_knob;
			this->ptr = &this->dummy;
			this->string_function = other.string_function;
			this->text = other.text;

			return *this;
		}

		qpl::f64 get_progress() const {
			if (this->end == this->start) return 0.0;
			return qpl::clamp_0_1(qpl::f64_cast(*this->ptr - this->start) / qpl::f64_cast(this->end - this->start));
		}
		T get_value() const {
			return *this->ptr;
		}
		void set_value(T value) {
			*this->ptr = value;
		}
		void set_pointer(T* value) {
			this->ptr = value;
			if (this->range_set) {
				*this->ptr = qpl::clamp(this->start, *this->ptr, this->end);
			}
		}
		void set_knob_position() {
			auto progress = qpl::f32_cast(this->get_progress());
			auto pos = this->knob.get_position();
			pos.x = this->position.x + this->range_pixel_width * progress;
			this->knob.set_position(pos);
		}
		std::pair<T, T> get_range() const {
			return std::make_pair(this->start, this->end);
		}

		void update_text_string() {
			if (!this->text.get_font().empty()) {
				if (this->string_function) {
					this->text.set_string(qpl::to_string(this->text_string, " ", this->string_function(this->get_value())));
				}
				else {
					this->text.set_string(qpl::to_string(this->text_string, " ", this->get_value()));
				}
			}
		}
		void set_range(T start, T end) {
			this->start = start;
			this->end = end;
			this->range_set = true;
			this->update_text_string();
		}
		void set_range(T start, T end, T value) {
			this->start = start;
			this->end = end;
			this->dummy = qpl::clamp(this->start, value, this->end);
			this->range_set = true;
			this->update_text_string();
		}
		template<typename P> requires (std::is_pointer_v<P>)
		void set_range(T start, T end, P ptr) {
			this->start = start;
			this->end = end;
			this->range_set = true;
			this->set_pointer(ptr);
			this->update_text_string();
		}
		qpl::vector2f get_position() const {
			return this->position;
		}
		void set_position(qpl::vector2f position) {
			this->position = position;
			this->background.set_position(position);

			auto y_extra = this->background.get_dimension().y - this->knob.get_dimension().y;
			this->knob.set_position(position + qpl::vector2f(0, y_extra / 2));
			this->set_knob_position();

			if (!this->text.get_font().empty()) {
				this->text.set_position(this->background.get_hitbox().middle_left());
				this->text.centerize_y();
			}
		}
		void set_center(qpl::vector2f position) {
			this->set_position(position - this->get_dimension() / 2);
		}
		qpl::vector2f get_dimension() const {
			return this->background.get_dimension();
		}
		qpl::hitbox get_hitbox() const {
			return this->background.get_hitbox();
		}
		qpl::vector2f get_knob_dimension() const {
			return this->knob.get_dimension();
		}
		qpl::vector2f get_knob_position() const {
			return this->knob.get_position();
		}
		void set_knob_dimension(qpl::vector2f dimension) {
			this->knob.set_dimension(dimension);
			this->range_pixel_width = this->background.get_dimension().x - this->knob.get_dimension().x;
		}
		void set_knob_width(qpl::f32 width) {
			auto dim = this->knob.get_dimension();
			this->knob.set_dimension(qpl::vector2f(width, dim.y));
			this->range_pixel_width = this->background.get_dimension().x - width;
		}
		qpl::f32 get_knob_width() const {
			return this->knob.get_dimension().x;
		}
		qpl::hitbox get_knob_hitbox() const {
			return this->knob.get_hitbox();
		}
		void set_dimensions(qpl::vector2f slider_dimension, qpl::vector2f knob_dimension) {
			this->background.set_dimension(slider_dimension);
			this->knob.set_dimension(knob_dimension);
			this->range_pixel_width = this->background.get_dimension().x - this->knob.get_dimension().x;
		}
		void set_dimension(qpl::vector2f dimension) {
			this->set_dimensions(dimension, sf::Vector2f(dimension.y, dimension.y));
		}
		void set_colors(qpl::rgba background_color, qpl::rgba knob_color) {
			this->background.set_color(background_color);
			this->background_color = background_color;
			this->background_hover_color = background_color;
			this->background_hover_outline_color = background_color;

			this->knob.set_color(knob_color);
			this->knob_hover_color = knob_color;
			this->knob_hover_outline_color = knob_color;
			this->knob_color = knob_color;
		}
		void set_alpha(qpl::u8 alpha) {
			this->background.set_color(this->background.get_color().with_alpha(alpha));
			this->background_color = this->background_color.with_alpha(alpha);
			this->background_hover_color = this->background_hover_color.with_alpha(alpha);
			this->background_hover_outline_color = this->background_hover_outline_color.with_alpha(alpha);

			this->knob.set_color(this->knob.get_color().with_alpha(alpha));
			this->knob_hover_color = this->knob_hover_color.with_alpha(alpha);
			this->knob_hover_outline_color = this->knob_hover_outline_color.with_alpha(alpha);
			this->knob_color = this->knob_color.with_alpha(alpha);
		}
		bool value_has_changed() const {
			return this->value_changed;
		}
		bool value_was_modified() const {
			return this->value_modified;
		}
		bool is_dragging() const {
			return this->dragging;
		}
		bool was_released() const {
			return this->released;
		}

		void set_text_font(std::string font) {
			this->text.set_font(font);
		}
		void set_text_string(std::string string) {
			this->text_string = string;
			this->text.set_string(this->text_string);
		}
		void set_text_character_size(qpl::u32 character_size) {
			this->text.set_character_size(character_size);
		}
		void set_text_color(qpl::rgb color) {
			this->text.set_color(color);
		}
		void set_text_string_function(const std::function<std::string(T)>& function) {
			this->string_function = function;
		}

		void set_knob_color(qpl::rgba color) {
			this->knob_color = color;
			this->knob_hover_color = color;
			this->knob_hover_outline_color = color;
			this->knob.set_color(color);
		}
		qpl::rgba get_knob_color() const {
			return this->knob_color;
		}
		void set_knob_outline_thickness(qpl::f32 thickness) {
			this->knob_outline_thickness = thickness;
			this->knob.set_outline_thickness(thickness);
		}
		qpl::f32 get_knob_outline_thickness() const {
			return this->knob_outline_thickness;
		}
		void set_knob_outline_color(qpl::rgba color) {
			this->knob_outline_color = color;
			this->knob.set_outline_color(color);
		}
		qpl::rgba get_knob_outline_color() const {
			return this->knob_outline_color;
		}

		void set_knob_hover_color(qpl::rgba color) {
			this->knob_hover_color = color;
		}
		qpl::rgba get_knob_hover_color() const {
			return this->knob_hover_color;
		}
		void set_knob_hover_outline_thickness(qpl::f32 thickness) {
			this->knob_hover_outline_thickness = thickness;
			if (this->is_hovering_over_knob()) {
				this->knob.set_outline_thickness(thickness);
			}
		}
		qpl::f32 get_knob_hover_outline_thickness() const {
			return this->knob_hover_outline_thickness;
		}
		void set_knob_hover_outline_color(qpl::rgba color) {
			this->knob_hover_outline_color = color;
			if (this->is_hovering_over_knob()) {
				this->knob.set_outline_color(color);
			}
		}
		qpl::rgba get_knob_hover_outline_color() const {
			return this->knob_hover_outline_color;
		}

		void set_background_color(qpl::rgba color) {
			this->background_color = color;
			this->background_hover_color = color;
			this->background_hover_outline_color = color;
			this->background.set_color(color);
		}
		qpl::rgba get_background_color() const {
			return this->background_color;
		}
		void set_background_outline_thickness(qpl::f32 thickness) {
			this->background_outline_thickness = thickness;
			this->background.set_outline_thickness(thickness);
		}
		qpl::f32 get_background_outline_thickness() const {
			return this->background_outline_thickness;
		}
		void set_background_outline_color(qpl::rgba color) {
			this->background_outline_color = color;
			this->background_outline_thickness.set_outline_color(color);
		}
		qpl::rgba get_background_outline_color() const {
			return this->background_outline_color;
		}

		void set_background_hover_color(qpl::rgba color) {
			this->background_hover_color = color;
			if (this->is_hovering_over_background()) {
				this->background.set_color(color);
			}
		}
		qpl::rgba get_background_hover_color() const {
			return this->background_hover_color;
		}
		void set_background_hover_outline_thickness(qpl::f32 thickness) {
			this->background_hover_outline_thickness = thickness;
			if (this->is_hovering_over_background()) {
				this->background.set_outline_thickness(thickness);
			}
		}
		qpl::f32 get_background_hover_outline_thickness() const {
			return this->background_hover_outline_thickness;
		}
		void set_background_hover_outline_color(qpl::rgba color) {
			this->background_hover_outline_color = color;
			if (this->is_hovering_over_background()) {
				this->background.set_outline_color(color);
			}
		}
		qpl::rgba get_background_hover_outline_color() const {
			return this->background_hover_outline_color;
		}

		void set_hitbox_increase(qpl::f32 increase) {
			this->hitbox_increase = increase;
		}
		qpl::f32 get_hitbox_increase() const {
			return this->hitbox_increase;
		}

		void enable_smooth_input() {
			this->input_smooth_values = true;
		}
		void enable_smooth_input(sf::Keyboard::Key key) {
			this->input_smooth_values = true;
			this->smooth_input_key = key;
		}
		void disable_smooth_input() {
			this->input_smooth_values = false;
		}
		bool is_smooth_input_enabled() const {
			return this->input_smooth_values;
		}
		void set_smooth_input_multiply(qpl::f64 factor) {
			this->smooth_input_multiply = factor;
		}
		qpl::f64 get_smooth_input_multiply() const {
			return this->smooth_input_multiply;
		}

		void allow_click_on_background_to_skip() {
			this->click_on_background_to_skip = true;
		}
		void disallow_click_on_background_to_skip() {
			this->click_on_background_to_skip = false;
		}
		void is_click_on_background_to_skip_allowed() const {
			return this->click_on_background_to_skip;
		}

		bool is_hovering_over_background() const {
			return this->hovering_over_background;
		}
		bool is_hovering_over_knob() const {
			return this->hovering_over_knob;
		}

		void disallow_dragging() {
			this->allow_drag = false;
			this->dragging = false;
		}
		void allow_dragging() {
			this->allow_drag = true;
		}
		bool dragging_allowed() const {
			return this->allow_drag;
		}

		void set_hover_increase(qpl::f32 value) {
			this->knob_hover_outline_thickness = value;
			this->background_hover_outline_thickness = value;
		}

		void update(const qsf::event_info& event) {
			auto pos = event.mouse_position();
			this->hovering_over_background = false;
			auto y_is = qpl::max(this->knob.get_dimension().y, this->background.get_dimension().y);
			this->hovering_over_background = this->background.contains(pos, qpl::vector2f(this->hitbox_increase, this->hitbox_increase + (y_is - this->background.get_dimension().y) / 2));

			this->value_modified = false;
			this->hovering_over_knob = this->knob.contains(pos, { this->hitbox_increase });

			if (this->background_hover_effect && this->hovering_over_background && !this->dragging) {
				this->background.set_color(this->background_hover_color);
				this->background.set_outline_color(this->background_hover_outline_color);
				this->background.set_outline_thickness(this->background_hover_outline_thickness);
			}
			else {
				this->background.set_color(this->background_color);
				this->background.set_outline_color(this->background_outline_color);
				this->background.set_outline_thickness(this->background_outline_thickness);
			}

			if (this->hovering_over_knob) {
				if (this->knob_hover_effect) {
					this->knob.set_color(this->knob_hover_color);
					this->knob.set_outline_color(this->knob_hover_outline_color);
					this->knob.set_outline_thickness(this->knob_hover_outline_thickness);
				}
				if (!this->dragging && event.left_mouse_clicked() && this->allow_drag) {
					this->dragging = true;
					this->click_pos_x = qpl::clamp(this->position.x, qpl::f32_cast(pos.x), this->position.x + this->range_pixel_width);
					this->click_knob_pos_x = pos.x - this->knob.get_position().x;
				}
			}
			else {
				if (this->click_on_background_to_skip && event.left_mouse_clicked() && this->allow_drag && this->hovering_over_background && (this->end != this->start)) {

					this->click_pos_x = qpl::clamp(this->position.x, qpl::f32_cast(pos.x), this->position.x + this->range_pixel_width);
					this->click_knob_pos_x = this->knob.get_dimension().x / 2;
					auto diff = (pos.x - this->position.x - this->click_knob_pos_x);
					auto progress = qpl::clamp_0_1(diff / this->range_pixel_width);

					if constexpr (qpl::is_integer<T>()) {
						*this->ptr = qpl::clamp(this->start, this->start + static_cast<T>((this->end - this->start) * progress + 0.5), this->end);
					}
					else {
						*this->ptr = qpl::clamp(this->start, this->start + static_cast<T>((this->end - this->start) * progress), this->end);
					}
					if (*this->ptr != this->value_before) {
						this->value_modified = true;
					}
					this->dragging = true;
				}

				if (this->knob_hover_effect && !this->dragging) {
					this->knob.set_color(this->knob_color);
					this->knob.set_outline_color(this->knob_outline_color);
					this->knob.set_outline_thickness(this->knob_outline_thickness);
				}
			}

			if (this->input_smooth_values && event.key_single_pressed(this->smooth_input_key)) {
				this->click_pos_x = qpl::clamp(this->position.x, qpl::f32_cast(pos.x), this->position.x + this->range_pixel_width);
			}

			if (!this->value_modified && this->dragging && this->allow_change && (this->end != this->start)) {
				qpl::f64 diff;
				if (this->input_smooth_values && event.key_holding(this->smooth_input_key)) {
					diff = ((pos.x - this->click_pos_x) * this->smooth_input_multiply) + (this->click_pos_x - this->position.x - this->click_knob_pos_x);
				}
				else {
					diff = (pos.x - this->position.x - this->click_knob_pos_x);
				}

				auto progress = qpl::clamp_0_1(diff / this->range_pixel_width);

				if constexpr (qpl::is_integer<T>()) {
					*this->ptr = qpl::clamp(this->start, this->start + static_cast<T>((this->end - this->start) * progress + 0.5), this->end);
				}
				else {
					*this->ptr = qpl::clamp(this->start, this->start + static_cast<T>((this->end - this->start) * progress), this->end);
				}
				if (*this->ptr != this->value_before) {
					this->value_modified = true;
				}
			}


			this->value_changed = false;
			if (this->value_before != *this->ptr) {
				this->set_knob_position();
				this->value_changed = true;
			}
			this->value_before = *this->ptr;

			this->released = false;
			if (event.left_mouse_released() && this->dragging) {
				this->released = true;
				this->dragging = false;
			}
			if (this->value_changed) {
				this->update_text_string();
			}
		}

		void draw(qsf::draw_object& object) const {
			if (this->background_visible) object.draw(this->background);
			if (this->knob_visible) object.draw(this->knob);
			if (!this->text.get_font().empty()) object.draw(this->text);
		}

		qsf::rectangle background;
		qsf::rectangle knob;

		qpl::vector2f position;
		bool knob_visible = true;
		bool background_visible = true;
		bool knob_hover_effect = true;
		bool background_hover_effect = true;

		qpl::rgba knob_hover_color = qpl::rgba::unset();
		qpl::f32 knob_hover_outline_thickness = 2.0f;
		qpl::rgba knob_hover_outline_color = qpl::rgba::unset();

		qpl::f32 hitbox_increase = 5.0;

		qpl::rgba background_hover_color = qpl::rgba::unset();
		qpl::f32 background_hover_outline_thickness = 2.0f;
		qpl::rgba background_hover_outline_color = qpl::rgba::unset();

		T start = T{ 0 };
		T end = T{ 1 };
		T dummy = T{ 1 };
		T value_before = qpl::type_max<T>();

		T* ptr = nullptr;

		qpl::rgba background_color = qpl::rgba::unset();
		qpl::f32 background_outline_thickness = 0.0f;
		qpl::rgba background_outline_color = qpl::rgba::unset();

		qpl::rgba knob_color = qpl::rgba::unset();
		qpl::f32 knob_outline_thickness = 0.0f;
		qpl::rgba knob_outline_color = qpl::rgba::unset();

		sf::Keyboard::Key smooth_input_key = sf::Keyboard::LShift;
		qpl::f64 smooth_input_multiply = 0.1;

		qpl::f32 click_knob_pos_x = 0.0f;
		qpl::f32 click_pos_x = 0.0f;
		qpl::f32 range_pixel_width = 0.0f;

		qsf::text text;
		std::string text_string;
		std::function<std::string(T)> string_function;

		bool hovering_over_background = false;
		bool click_on_background_to_skip = true;
		bool input_smooth_values = true;
		bool value_changed = false;
		bool value_modified = false;
		bool range_set = false;
		bool allow_change = true;
		bool allow_drag = true;
		bool dragging = false;
		bool released = false;
		bool hovering_over_knob = false;
	};

	struct check_box {
		qsf::rectangle background;
		qsf::text text;
		bool active_value = false;
		bool value_modified = false;

		check_box() {
			this->background.set_dimension({ 30, 30 });
			this->background.set_color(qpl::rgb(100, 100, 100));
			this->background.set_outline_color(qpl::rgb::white());
			this->background.set_outline_thickness(3.0f);
			this->text.set_color(qpl::rgb::white());
			this->text.set_character_size(30);
		}

		void set_font(const std::string& font) {
			this->text.set_font(font);
			this->text.set_string("x");
		}
		void set_font(const sf::Font& font) {
			this->text.set_font(font);
			this->text.set_string("x");
		}
		void centerize_text() {
			this->text.set_center(this->background.get_center());
		}
		void set_dimension(qpl::vector2f dimension) {
			this->background.set_dimension(dimension);
			this->centerize_text();
		}
		void set_character_size(qpl::u32 size) {
			this->text.set_character_size(size);
			this->centerize_text();
		}
		void set_center(qpl::vector2f position) {
			this->background.set_center(position);
			this->centerize_text();
		}
		void set_position(qpl::vector2f position) {
			this->background.set_position(position);
			this->centerize_text();
		}
		void set_value(bool value) {
			this->active_value = value;
		}
		bool value_was_modified() const {
			return this->value_modified;
		}
		bool is_clicked() const {
			return this->value_was_modified();
		}
		bool get_value() const {
			return this->active_value;
		}

		void update(const qsf::event_info& event) {
			this->value_modified = false;
			if (this->background.get_hitbox().contains(event.mouse_position()) && event.left_mouse_clicked()) {
				this->active_value = !this->active_value;
				this->value_modified = true;
			}
		}
		void draw(qsf::draw_object& draw) const {
			draw.draw(this->background);
			if (this->active_value) {
				draw.draw(this->text);
			}
		}
	};

	struct color_picker {

		mutable qsf::vertex_array gradient;
		mutable qsf::vertex_array hue_slider;
		mutable qsf::vertex_array gradient_cross;
		mutable qsf::vertex_array transparency_slider;
		mutable qsf::vertex_array transparency_checkerboard;
		mutable qsf::smooth_rectangle background;
		mutable qsf::smooth_rectangle hue_slider_knob;
		mutable qsf::smooth_rectangle transparency_slider_knob;
		mutable qsf::smooth_rectangle display_color_rectangle;
		mutable qsf::circle gradient_control_circle;
		mutable qsf::smooth_button confirm_button;

		qpl::hitbox hitbox;
		qpl::f32 slider_width = 30.0f;
		qpl::f32 slider_margin = 20.0f;
		qpl::f32 background_increase = 40.0f;
		qpl::f32 gradient_control_circle_radius = 8.0f;
		qpl::f32 bottom_margin = 20.0f;
		qpl::f32 bottom_height = 40.0f;
		qpl::f32 hitbox_increase = 5.0f;

		qpl::vec2d gradient_control;
		qpl::f64 hue_control = 0.0;
		qpl::f64 transparency_control = 1.0;

		qpl::rgb hue_color;

		mutable qpl::rgba color_result;
		mutable qsf::text_field input_field;

		mutable bool internal_check = false;
		mutable bool internal_gradient_hitbox_changed = true;
		mutable bool internal_gradient_color_changed = true;
		bool visible = true;
		bool dragging_hue_slider = false;
		bool dragging_transparency_slider = false;
		bool dragging_gradient = false;
		bool color_changed = false;
		bool cross_enabled = true;
		bool focus = false;
		bool transparency_used = true;
		bool dont_update_input_text = false;

		constexpr static qpl::size gradient_segments = 15u;
		constexpr static qpl::size hue_slider_segments = 14u;
		constexpr static qpl::vec2 transparency_checkerboard_dimension = qpl::vec2{ 10, 10 };
		constexpr static qpl::rgb transparency_checkerboard_color_dark = qpl::rgb::grey_shade(100);
		constexpr static qpl::rgb transparency_checkerboard_color_light = qpl::rgb::grey_shade(150);

		color_picker() {
			this->init();
		}
		QPLDLL void init();
		QPLDLL void reset();
		QPLDLL void update_values();

		QPLDLL bool color_value_changed() const;
		QPLDLL bool has_focus() const;
		QPLDLL bool is_font_set() const;

		QPLDLL void set_color_value(qpl::rgba color);
		QPLDLL qpl::rgba get_color_value() const;
		QPLDLL qpl::rgb get_gradient_color_at(qpl::vec2f pos) const;
		QPLDLL qpl::hitbox get_gradient_hitbox() const;
		QPLDLL qpl::hitbox get_hue_slider_hitbox() const;
		QPLDLL qpl::hitbox get_transparency_slider_hitbox() const;
		QPLDLL qpl::f64 get_hue_control_value() const;
		QPLDLL qpl::f64 get_transparency_control_value() const;
		QPLDLL qpl::vec2d get_gradient_control_value() const;

		QPLDLL void disable_transparency();
		QPLDLL void enable_transparency();

		QPLDLL void set_font(std::string font);
		QPLDLL void set_invisible();
		QPLDLL void set_visible();
		QPLDLL bool is_visible() const;
		QPLDLL bool is_dragging_hue_slider() const;
		QPLDLL bool is_dragging_transparency_slider() const;
		QPLDLL bool is_dragging_gradient() const;
		QPLDLL bool is_dragging() const;


		QPLDLL void set_input_field_width(qpl::f32 width);
		QPLDLL void set_dimension(qpl::vec2f dimension);
		QPLDLL void set_position(qpl::vec2f position);
		QPLDLL void set_hitbox(qpl::hitbox hitbox);
		QPLDLL void set_center(qpl::vec2f center);
		QPLDLL void set_hue_wheel(qpl::f64 hue);
		QPLDLL void set_background_color(qpl::rgba color);
		QPLDLL void set_hue_control_color(qpl::rgba color);
		QPLDLL void set_gradient_control_color(qpl::rgba color);

		QPLDLL void create_gradient() const;
		QPLDLL void update_gradient() const;
		QPLDLL void create_hue_slider() const;
		QPLDLL void create_transparency_slider() const;
		QPLDLL void update_bottom_row_positions() const;
		QPLDLL void internal_update() const;
		QPLDLL void update_hue_slider_knob_position() const;
		QPLDLL void update_transparency_slider_knob_position() const;
		QPLDLL void update_gradient_control_position() const;
		QPLDLL void update_internal_colors();

		QPLDLL void update_entered_text();

		QPLDLL void update(const qsf::event_info& event);
		QPLDLL void draw(qsf::draw_object& draw) const;
	};
}

#endif
#endif