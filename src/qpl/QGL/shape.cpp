#include <qpl/QGL/shapes.hpp>

#if defined QPL_INTERN_GLEW_USE

namespace qgl {
	cuboid::cuboid() {
		this->vertex_array = qgl::get_cube();
		this->position = qpl::vec(0, 0, 0);
		this->dimension = qpl::vec(1, 1, 1);
		this->color = qpl::rgb::white();
	}

	void qgl::cuboid::apply() const {
		if (!this->vertex_array.generated()) {
			this->vertex_array.generate();
		}

		constexpr auto block = qgl::get_cube();
		for (qpl::size i = 0u; i < this->vertex_array.vertices.size(); ++i) {
			this->vertex_array.vertices[i].position = block.vertices[i].position * this->dimension + this->position;
			this->vertex_array.vertices[i].color = this->color;
		} 
		this->vertex_array.update();
		this->changed = false;
	}

	void qgl::cuboid::set_position(qpl::vec3 position) {
		this->position = position;
		this->changed = true;
	}
	void qgl::cuboid::set_dimension(qpl::vec3 dimension) {
		this->dimension = dimension;
		this->changed = true;
	}
	void qgl::cuboid::set_center(qpl::vec3 position) {
		this->set_position(position - this->dimension / 2);
	}

	void qgl::cuboid::move(qpl::vec3 delta) {
		this->position.move(delta);
		this->changed = true;
	}

	void qgl::cuboid::set_color(qpl::rgb color) {
		this->color = color;
		this->changed = true;
	}

	qpl::vec3 qgl::cuboid::get_center() const {
		return this->position + this->dimension / 2;
	}
	void qgl::cuboid::draw(qsf::draw_object_gl& draw) const {
		if (this->changed) {
			if (!this->vertex_array.generated()) {
				draw.window->setActive(true);
				this->vertex_array.generate();
				this->apply();
				this->vertex_array.update();
				draw.window->setActive(false);
			}
			else {
				this->apply();
			}
		}
		draw.draw(this->vertex_array);
	}
}

#endif