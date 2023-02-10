#pragma once
#ifndef QSF_VIEW_CONTROL_HPP
#define QSF_VIEW_CONTROL_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/vector.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/drawables.hpp>
#include <qpl/QSF/view.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace qsf {

	struct base_state;
	template<typename T>
	struct view_control_t : qsf::view_t<T> {
		sf::Mouse::Button drag_mouse_button = sf::Mouse::Left;

		view_control_t() {
			this->smooth_zoom_animation.set_duration(this->smooth_zoom_duration);
		}
		void set_hitbox(qpl::vector2<T> position, qpl::vector2<T> dimension) {
			this->set_hitbox(qpl::hitbox{ position, dimension });
		}
		void set_hitbox(const qsf::base_state& state) {
			this->set_hitbox(qpl::hitbox{ {0,0}, state.dimension()});
		}
		void set_hitbox(qpl::hitbox hitbox) {
			this->hitbox = hitbox;
			this->dimension = hitbox.dimension * this->scale;
			this->hitbox_set = true;
			this->changed = true;
		}
		void move(qpl::vector2<T> delta) {
			this->move(delta);
			this->changed = true;
		}
		void move(T dx, T dy) {
			this->move(qpl::vec(dx, dy));
		}
		void set_scale(qpl::vector2<T> scale) {
			this->scale = scale;
			this->changed = true;
		}
		void set_scale(T scale) {
			this->set_scale(qpl::vec(scale, scale));
		}
		void set_position(qpl::vector2<T> position) {
			this->position = position;
			this->changed = true;
		}
		void set_center(qpl::vector2<T> position) {
			this->set_position(position - this->dimension / 2);
		}
		void enable_smooth_zoom() {
			this->use_smooth_zoom = true;
		}
		void disable_smooth_zoom() {
			this->use_smooth_zoom = false;
		}
		void set_smooth_zoom_time(qpl::f64 delta) {
			this->smooth_zoom_duration = delta;
			this->smooth_zoom_animation.set_duration(this->smooth_zoom_duration);
		}
		void zoom_in(qpl::vector2f mouse) {
			auto mouse_relative = mouse - this->hitbox.position;
			if (this->use_smooth_zoom) {

				auto scale_start = this->scale;
				auto position_start = this->position;
				auto dimension_start = this->dimension;

				if (this->smooth_zoom_animation.is_running()) {
					scale_start = this->animation_scale_b;
					position_start = this->animation_position_b;
					dimension_start = this->animation_dimension_b;
				}

				this->animation_scale_a = this->scale;
				this->animation_scale_b = scale_start * (1.0 / (1 + this->zoom_delta));

				this->animation_position_a = this->position;
				this->animation_position_b = position_start + (mouse_relative * this->zoom_delta) * this->animation_scale_b;

				this->animation_dimension_a = this->dimension;
				this->animation_dimension_b = dimension_start * (1.0 / (1 + this->zoom_delta));

				this->smooth_zoom_animation.reset_and_start();
			}
			else {
				this->scale *= 1 / (1 + this->zoom_delta);
				this->dimension *= 1 / (1 + this->zoom_delta);
				this->position += (mouse_relative * this->zoom_delta) * (this->scale);
			}

		}
		void zoom_out(qpl::vector2f mouse) {
			auto mouse_relative = mouse - this->hitbox.position;
			if (this->use_smooth_zoom) {

				auto scale_start = this->scale;
				auto position_start = this->position;
				auto dimension_start = this->dimension;

				if (this->smooth_zoom_animation.is_running()) {
					scale_start = this->animation_scale_b;
					position_start = this->animation_position_b;
					dimension_start = this->animation_dimension_b;
				}

				this->animation_scale_a = this->scale;
				this->animation_scale_b = scale_start * (1.0 + this->zoom_delta);

				this->animation_position_a = this->position;
				this->animation_position_b = position_start + (mouse_relative * (1.0 / (1 + this->zoom_delta) - 1)) * this->animation_scale_b;

				this->animation_dimension_a = this->dimension;
				this->animation_dimension_b = dimension_start * (1.0 + this->zoom_delta);

				this->smooth_zoom_animation.reset_and_start();
			}
			else {
				this->scale *= (1 + this->zoom_delta);
				this->dimension *= (1 + this->zoom_delta);
				this->position += (mouse_relative * (1 / (1 + this->zoom_delta) - 1)) * (this->scale);
			}
		}
		void update(const qsf::event_info& event) {
			if (!this->hitbox_set) {
				qpl::println("update: hitbox of view_control was not set");
				return;
			}

			this->smooth_zoom_animation.update(event);
			if (this->smooth_zoom_animation.is_running() && this->use_smooth_zoom) {
				auto p = this->smooth_zoom_animation.get_curve_progress(this->smooth_zoom_slope);

				this->scale = qpl::linear_interpolation(this->animation_scale_a, this->animation_scale_b, p);
				this->position = qpl::linear_interpolation(this->animation_position_a, this->animation_position_b, p);
				this->dimension = qpl::linear_interpolation(this->animation_dimension_a, this->animation_dimension_b, p);
			}

			this->changed = false;
			this->zoomed = false;
			this->mouse_position = this->position + event.mouse_position() * this->scale;

			this->hovering = this->hitbox.contains(event.mouse_position());

			if (!this->allow_dragging) {
				this->dragging = false;
			}
			if (this->hovering) {
				auto mouse_relative = event.mouse_position() - this->hitbox.position;

				if (event.mouse_button_clicked(this->drag_mouse_button)) {
					this->dragging = true;
					this->click_position = this->position;
					this->click_mouse_position = mouse_relative;
				}

				if (this->allow_dragging && this->dragging && event.mouse_moved()) {
					auto delta = (this->click_mouse_position - mouse_relative);
					this->position = this->click_position + delta * this->scale;
					if (delta != qpl::vec(0.f, 0.f)) {
						this->changed = true;
					}
				}

				if (event.scrolled_down()) {
					this->zoom_out(event.mouse_position());
					this->changed = true;
					this->zoomed = true;
				}
				if (event.scrolled_up()) {
					this->zoom_in(event.mouse_position());
					this->changed = true;
					this->zoomed = true;
				}

				if (this->zoomed) {
					if (this->dragging) {
						this->click_position = this->position;
						this->click_mouse_position = mouse_relative;
					}
				}
			}

			if (this->allow_dragging && event.mouse_button_released(this->drag_mouse_button)) {
				this->dragging = false;
				this->click_position = this->position;
				this->click_mouse_position = event.mouse_position() - this->hitbox.position;
			}
		}
		qpl::vector2<T> position_at_mouse() const {
			return this->mouse_position;
		}
		bool just_changed() const {
			return this->changed;
		}
		bool just_zoomed() const {
			return this->zoomed;
		}

		bool empty() const {
			return this->hitbox_set == false;
		}


		qpl::hitbox hitbox;
		qsf::view_t<T> view;
		qpl::vector2<T> dimension;
		qpl::vector2<T> mouse_position;

		qpl::vector2<T> click_position;
		qpl::vector2<T> click_mouse_position;

		qpl::vector2<T> animation_position_a;
		qpl::vector2<T> animation_position_b;
		qpl::vector2<T> animation_dimension_a;
		qpl::vector2<T> animation_dimension_b;
		qpl::vector2<T> animation_scale_a;
		qpl::vector2<T> animation_scale_b;

		qpl::animation smooth_zoom_animation;
		qpl::f64 smooth_zoom_duration = 0.07;
		qpl::f64 smooth_zoom_slope = 1.3;
		T zoom_delta = 0.25;
		bool hitbox_set = false;
		bool hovering = false;
		bool dragging = false;
		bool allow_dragging = true;
		bool enabled = true;
		bool use_smooth_zoom = true;
		bool ignore_input = false;
		bool changed = false;
		bool zoomed = false;
	};

	using view_control = view_control_t<qpl::f64>;

}

#endif
#endif
