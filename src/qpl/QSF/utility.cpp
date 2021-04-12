#include <qpl/QSF/utility.hpp>
#include <qpl/QSF/framework.hpp>
#include <qpl/system.hpp>

namespace qsf {
	void qsf::view_rectangle::set_hitbox(qsf::vector2f position, qsf::vector2f dimension) {
		this->hitbox.set_position(position);
		this->hitbox.set_dimension(dimension);
		this->dimension = (dimension - position) * this->scale;
		this->hitbox_set = true;
	}
	void qsf::view_rectangle::set_hitbox(const qsf::base_state& state) {
		this->hitbox.set_position({ 0,0 });
		this->hitbox.set_dimension(state.dimension());
		this->dimension = state.dimension() * this->scale;
		this->hitbox_set = true;
	}
	void qsf::view_rectangle::reset() {
		this->position = { 0,0 };
		this->scale = { 1,1 };
	}
	void qsf::view_rectangle::set_position(qsf::vector2f position) {
		this->position = position;

		if (this->power_of_two) {
			if (this->scale.x >= 1) {
				this->position = qsf::vector2i(this->position) / qsf::vector2i(this->scale) * qsf::vector2i(this->scale);
			}
			else {
				this->position = qsf::vector2i(this->position / this->scale) * this->scale;
			}
		}
		else if (this->integer_step) {
			this->position = qsf::vector2i(this->position);
		}
	}
	void qsf::view_rectangle::update(const qsf::event_info& event) {
		if (!this->hitbox_set) {
			qpl::println("qsf::view_rectangle::update: hitbox of view_rectangle was not set");
			return;
		}

		this->mouse_position = this->position + event.mouse_position() * this->scale;

		this->hovering = this->hitbox.contains(event.mouse_position());
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
					this->position = qsf::vector2i(this->position);
				}
			}

			if (event.scrolled_down()) {
				if (this->power_of_two) {
					this->scale *= 2;
					this->dimension *= 2;
					this->position += (mouse_relative / -2) * (this->scale);
					if (this->scale.x >= 1) {
						this->position = qsf::vector2i(this->position) / qsf::vector2i(this->scale) * qsf::vector2i(this->scale);
					}
					else {
						this->position = qsf::vector2i(this->position / this->scale) * this->scale;
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
						this->position = qsf::vector2i(this->position);
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
						this->position = qsf::vector2i(this->position) / qsf::vector2i(this->scale) * qsf::vector2i(this->scale);
					}
					else {
						this->position = qsf::vector2i(this->position / this->scale) * this->scale;
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
						this->position = qsf::vector2i(this->position);
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
	qsf::vector2d qsf::view_rectangle::position_at_mouse() const {
		return this->mouse_position;
	}

	bool qsf::view_rectangle::empty() const {
		return this->hitbox_set == false;
	}
	sf::RenderStates qsf::view_rectangle::get_render_states() const {
		sf::RenderStates states;
		states.transform.scale(qsf::vector2f(1, 1) / this->scale).translate(-this->position).rotate(this->rotation);
		return states;
	}
}