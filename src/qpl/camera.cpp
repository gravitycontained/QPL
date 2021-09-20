#include <qpl/camera.hpp>
#if defined(QPL_USE_VULKAN) || defined(QPL_USE_ALL)
#include <cmath>
#include <algorithm>

namespace qpl {
	void qpl::camera::construct() {
		this->m_position = { 0, 0, 0 };
		this->m_view_rotation = { 0, 0 };
		this->m_velocites = { 0, 0, 0 };
		this->m_max_velocity = 2;
		this->m_accelerate = 0.2f;
		this->m_deaccelerate = 5.0f;
		this->m_speed = 1.0;
	}

	
	glm::vec3 qpl::camera::get_view_rotation_coordinates() const {
		glm::vec3 result;
		result.x = std::cos(-this->m_view_rotation.x) * std::cos(-this->m_view_rotation.y);
		result.y = std::sin(-this->m_view_rotation.y);
		result.z = std::sin(-this->m_view_rotation.x) * std::cos(-this->m_view_rotation.y);
		return result;
	}
	glm::vec3 qpl::camera::get_resulting_looking_position() const {
		return this->m_position + this->get_view_rotation_coordinates();
	}
	glm::mat4 qpl::camera::get_resulting_projection() const {
		glm::vec3 rot = { 0, 1, 0 };
		return glm::lookAt(this->m_position, this->get_resulting_looking_position(), rot);
	}

	void qpl::camera::set_position(glm::vec3 pos) {
		this->m_position = pos;
	}
	void qpl::camera::set_position(qpl::f32 x, qpl::f32 y, qpl::f32 z) {
		this->set_position({ x, y, z });
	}
	void qpl::camera::set_position_x(qpl::f32 x) {
		this->m_position.x = x;
	}
	void qpl::camera::set_position_y(qpl::f32 y) {
		this->m_position.y = y;
	}
	void qpl::camera::set_position_z(qpl::f32 z) {
		this->m_position.z = z;
	}
	glm::vec3 qpl::camera::get_position() const {
		return this->m_position;
	}

	qpl::f32 qpl::camera::get_position_x() const {
		return this->m_position.x;
	}
	qpl::f32 qpl::camera::get_position_y() const {
		return this->m_position.y;
	}
	qpl::f32 qpl::camera::get_position_z() const {
		return this->m_position.z;
	}

	void qpl::camera::move(glm::vec3 delta) {
		this->m_position += delta;
	}
	void qpl::camera::move(qpl::f32 x, qpl::f32 y, qpl::f32 z) {
		this->move({ x, y, z });
	}
	void qpl::camera::move_x(qpl::f32 x) {
		this->m_position.x += x;
	}
	void qpl::camera::move_y(qpl::f32 y) {
		this->m_position.y += y;
	}
	void qpl::camera::move_z(qpl::f32 z) {
		this->m_position.z += z;
	}


	void qpl::camera::move_forward(qpl::f32 delta) {
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->move(vec);
	}
	void qpl::camera::move_backwards(qpl::f32 delta) {
		this->look_backwards();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_backwards();
		this->move(vec);
	}
	void qpl::camera::move_up(qpl::f32 delta) {
		this->look_up();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_down();
		this->move(vec);
	}
	void qpl::camera::move_down(qpl::f32 delta) {
		this->look_down();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_up();
		this->move(vec);
	}
	void qpl::camera::move_left(qpl::f32 delta) {
		this->look_left();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_right();
		this->move(vec);
	}
	void qpl::camera::move_right(qpl::f32 delta) {
		this->look_right();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_left();
		this->move(vec);
	}

	void qpl::camera::set_view_rotation(glm::vec2 pos) {
		this->set_view_rotation_x(pos.x);
		this->set_view_rotation_y(pos.y);
	}
	void qpl::camera::set_view_rotation(qpl::f32 x, qpl::f32 y) {
		this->set_view_rotation_x(x);
		this->set_view_rotation_y(y);
	}
	void qpl::camera::set_view_rotation_x(qpl::f32 x) {
		this->m_view_rotation.x = x;
	}
	void qpl::camera::set_view_rotation_y(qpl::f32 y) {
		this->m_view_rotation.y = y;
	}
	glm::vec2 qpl::camera::get_view_rotation() const {
		return this->m_view_rotation;
	}
	qpl::f32 qpl::camera::get_view_rotation_x() const {
		return this->m_view_rotation.x;
	}
	qpl::f32 qpl::camera::get_view_rotation_y() const {
		return this->m_view_rotation.y;
	}

	void qpl::camera::rotate_view(glm::vec2 delta) {
		this->set_view_rotation(this->get_view_rotation() + delta);
	}

	void qpl::camera::look_backwards() {
		this->rotate_view_y(static_cast<qpl::f32>(qpl::pi));
	}
	void qpl::camera::look_up(qpl::f32 delta) {
		this->rotate_view_y(-delta);
	}
	void qpl::camera::look_down(qpl::f32 delta) {
		this->rotate_view_y(delta);
	}
	void qpl::camera::look_left(qpl::f32 delta) {
		this->rotate_view_x(delta);
	}
	void qpl::camera::look_right(qpl::f32 delta) {
		this->rotate_view_x(-delta);
	}
	void qpl::camera::rotate_view_x(qpl::f32 x) {
		this->set_view_rotation_x(this->m_view_rotation.x + x);
	}
	void qpl::camera::rotate_view_y(qpl::f32 y) {
		this->set_view_rotation_y(this->m_view_rotation.y + y);
	}

	void qpl::camera::set_max_velocity(qpl::f32 max) {
		this->m_max_velocity = max;
	}
	qpl::f32 qpl::camera::get_max_velocity() const {
		return this->m_max_velocity;
	}

	void qpl::camera::set_acceleration(qpl::f32 acceleration) {
		this->m_accelerate = acceleration;
	}
	qpl::f32 qpl::camera::get_acceleration() const {
		return this->m_accelerate;
	}

	void qpl::camera::set_deacceleration(qpl::f32 deacceleration) {
		this->m_deaccelerate = deacceleration;
	}
	qpl::f32 qpl::camera::get_deacceleration() const {
		return this->m_deaccelerate;
	}

	void qpl::camera::accelerate_forward(qpl::f32 delta) {
		auto vec = this->get_view_rotation_coordinates() * delta * this->get_acceleration();
		this->m_velocites += vec * this->m_speed;
	}
	void qpl::camera::accelerate_backwards(qpl::f32 delta) {
		this->look_backwards();
		auto vec = this->get_view_rotation_coordinates() * delta * this->get_acceleration();
		this->look_backwards();
		this->m_velocites += vec * this->m_speed;
	}
	void qpl::camera::accelerate_left(qpl::f32 delta) {
		auto copy = this->m_view_rotation;
		this->look_left();
		this->ignore_y_axis();
		auto vec = this->get_view_rotation_coordinates() * delta * this->get_acceleration();
		this->m_view_rotation = copy;
		this->m_velocites += vec * this->m_speed;
	}
	void qpl::camera::accelerate_right(qpl::f32 delta) {
		auto copy = this->m_view_rotation;
		this->look_right();
		this->ignore_y_axis();
		auto vec = this->get_view_rotation_coordinates() * delta * this->get_acceleration();
		this->m_view_rotation = copy;
		this->m_velocites += vec * this->m_speed;
	}
	void qpl::camera::accelerate_up(qpl::f32 delta) {
		this->m_velocites.y += delta * this->get_acceleration() * this->m_speed;;
	}
	void qpl::camera::accelerate_down(qpl::f32 delta) {
		this->m_velocites.y -= delta * this->get_acceleration() * this->m_speed;;
	}


	void qpl::camera::set_speed(qpl::f32 speed) {
		this->m_speed = speed;
	}
	qpl::f32 qpl::camera::get_speed() const {
		return this->m_speed;
	}

	void qpl::camera::update(qpl::f32 delta_time) {
		this->m_velocites.x = std::max(this->m_velocites.x, -this->m_max_velocity);
		this->m_velocites.x = std::min(this->m_velocites.x, this->m_max_velocity);
		this->m_velocites.y = std::max(this->m_velocites.y, -this->m_max_velocity);
		this->m_velocites.y = std::min(this->m_velocites.y, this->m_max_velocity);
		this->m_velocites.z = std::max(this->m_velocites.z, -this->m_max_velocity);
		this->m_velocites.z = std::min(this->m_velocites.z, this->m_max_velocity);

		this->m_velocites *= (qpl::f32{ 1 } - this->m_deaccelerate * delta_time);
		this->m_position += this->m_velocites;


		this->m_view_rotation.x = static_cast<qpl::f32>(std::fmod(this->m_view_rotation.x, qpl::pi * 2));
		this->m_view_rotation.y = static_cast<qpl::f32>(std::fmod(this->m_view_rotation.y + qpl::pi_32 / 2, qpl::pi * 2)) - qpl::pi_32 / 2;
		this->m_view_rotation.y = std::max(this->m_view_rotation.y, -qpl::pi_32 / 2 + 0.01f);
		this->m_view_rotation.y = std::min(this->m_view_rotation.y, qpl::pi_32 / 2 - 0.01f);
	}
	glm::vec3 qpl::camera::get_velocity() const {
		return this->m_velocites;
	}

	void qpl::camera::ignore_y_axis() {
		this->m_view_rotation.y = 0.0f;
	}
}
#endif