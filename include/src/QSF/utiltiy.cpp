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
	void qsf::view_rectangle::update(const qsf::event_info& event) {
		if (!this->hitbox_set) {
			qpl::println("qsf::view_rectangle::update: hitbox of view_rectangle was not set");
			return;
		}

		this->hovering = this->hitbox.contains(event.mouse_position());
		if (this->hovering) {
			auto mouse_relative = event.mouse_position() - this->hitbox.position;

			if (event.left_mouse_clicked()) {
				this->dragging = true;
				this->click_position = this->position;
				this->click_mouse_position = mouse_relative;
			}

			if (this->dragging && event.mouse_moved()) {
				auto delta = (this->click_mouse_position - mouse_relative);
				this->position = this->click_position + delta * this->scale;
			}

			if (event.scrolled_down()) {
				this->scale *= (1 + this->zoom_delta);
				this->dimension *= (1 + this->zoom_delta);
				this->position += (mouse_relative * (1 / (1 + this->zoom_delta) - 1)) * (this->scale);
			}
			if (event.scrolled_up()) {
				this->scale *= 1 / (1 + this->zoom_delta);
				this->dimension *= 1 / (1 + this->zoom_delta);
				this->position += (mouse_relative * this->zoom_delta) * (this->scale);
			}
		}

		if (event.left_mouse_released()) {
			this->dragging = false;
			this->click_position = this->position;
			this->click_mouse_position = event.mouse_position() - this->hitbox.position;
		}
	}

	bool qsf::view_rectangle::empty() const {
		return this->hitbox_set == false;
	}
}