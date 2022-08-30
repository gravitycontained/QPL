#pragma once
#ifndef QSF_VIEW_HPP
#define QSF_VIEW_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/vector.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/drawables.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace qsf {

	struct base_state;

	template<typename T>
	struct view_rectangle_t {
		sf::Mouse::Button drag_mouse_button = sf::Mouse::Left;

		void set_hitbox(qpl::vector2<T> position, qpl::vector2<T> dimension) {
			this->set_hitbox(qpl::hitbox{ position, dimension });
		}
		void set_hitbox(const qsf::base_state& state) {
			this->set_hitbox(qpl::hitbox{ {0,0}, state.dimension });
		}
		void set_hitbox(qpl::hitbox hitbox) {
			this->hitbox = hitbox;
			this->dimension = hitbox.dimension() * this->scale;
			this->hitbox_set = true;
		}
		void reset() {
			this->position = { 0,0 };
			this->scale = { 1,1 };
		}
		void move(qpl::vector2<T> delta) {
			this->positiom.move(delta);
		}
		void move(T dx, T dy) {
			this->position.move(qpl::vec(dx, dy));
		}
		void set_scale(qpl::vector2<T> scale) {
			this->scale = scale;
		}
		void set_scale(T scale) {
			this->scale = qpl::vec(scale, scale);
		}
		void set_position(qpl::vector2<T> position) {
			this->position = position;

			if (this->power_of_two) {
				if (this->scale.x >= 1) {
					this->position = qpl::vector2i(this->position) / qpl::vector2i(this->scale) * qpl::vector2i(this->scale);
				}
				else {
					this->position = qpl::vector2i(this->position / this->scale) * this->scale;
				}
			}
			else if (this->integer_step) {
				this->position = qpl::vector2i(this->position);
			}
		}
		void update(const qsf::event_info& event) {
			if (!this->hitbox_set) {
				qpl::println("update: hitbox of view_rectangle was not set");
				return;
			}

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

					if (this->integer_step) {
						this->position = qpl::vector2i(this->position);
					}
				}

				if (event.scrolled_down()) {
					if (this->power_of_two) {
						this->scale *= 2;
						this->dimension *= 2;
						this->position += (mouse_relative / -2) * (this->scale);
						if (this->scale.x >= 1) {
							this->position = qpl::vector2i(this->position) / qpl::vector2i(this->scale) * qpl::vector2i(this->scale);
						}
						else {
							this->position = qpl::vector2i(this->position / this->scale) * this->scale;
						}
					}
					else {
						this->scale *= (1 + this->zoom_delta);
						this->dimension *= (1 + this->zoom_delta);

						if (this->integer_step) {
							auto s = this->scale.x;
							if (s > 2) {
								s = qpl::u32_cast(s + 0.5);
								this->scale = { s, s };
							}
						}

						this->position += (mouse_relative * (1 / (1 + this->zoom_delta) - 1)) * (this->scale);

						if (this->integer_step) {
							this->position = qpl::vector2i(this->position);
						}
						if (this->dragging) {
							this->click_position = this->position;
							this->click_mouse_position = event.mouse_position() - this->hitbox.position;
						}
					}
				}
				if (event.scrolled_up()) {
					if (this->power_of_two) {
						this->scale /= 2;
						this->dimension /= 2;
						this->position += (mouse_relative) * (this->scale);
						if (this->scale.x >= 1) {
							this->position = qpl::vector2i(this->position) / qpl::vector2i(this->scale) * qpl::vector2i(this->scale);
						}
						else {
							this->position = qpl::vector2i(this->position / this->scale) * this->scale;
						}
					}
					else {
						this->scale *= 1 / (1 + this->zoom_delta);
						this->dimension *= 1 / (1 + this->zoom_delta);


						if (this->integer_step) {
							auto s = this->scale.x;
							if (s > 2) {
								s = qpl::u32_cast(s + 0.5);
								this->scale = { s, s };
							}
						}


						this->position += (mouse_relative * this->zoom_delta) * (this->scale);

						if (this->integer_step) {
							this->position = qpl::vector2i(this->position);
						}
						if (this->dragging) {
							this->click_position = this->position;
							this->click_mouse_position = event.mouse_position() - this->hitbox.position;
						}
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

		bool empty() const {
			return this->hitbox_set == false;
		}
		sf::RenderStates get_render_states() const {
			sf::RenderStates states;
			states.transform.scale(qpl::vector2f(qpl::vector2f(1, 1) / this->scale)).translate(-qpl::vector2f(this->position)).rotate(qpl::f32_cast(this->rotation));
			return states;
		}

		qpl::hitbox hitbox;
		qpl::vector2<T> position = { 0,0 };
		qpl::vector2<T> dimension;
		qpl::vector2<T> scale = { 1, 1 };
		qpl::vector2<T> mouse_position;

		qpl::vector2<T> click_position;
		qpl::vector2<T> click_mouse_position;

		bool ignore_input = false;
		T rotation = 0;
		T zoom_delta = 0.4;
		bool hitbox_set = false;
		bool hovering = false;
		bool dragging = false;
		bool integer_step = false;
		bool power_of_two = false;
		bool allow_dragging = true;
		bool enabled = true;
	};

	using view_rectangle = view_rectangle_t<qpl::f64>;

}

#endif
#endif
