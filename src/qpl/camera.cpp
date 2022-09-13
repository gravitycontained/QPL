#include <qpl/camera.hpp>

#include <cmath>
#include <algorithm>

namespace qpl {
	void qpl::camera::construct() {
		this->position = { 0, 0, 0 };
		this->view_rotation = { 0, 0 };
		this->velocities = { 0, 0, 0 };
		this->max_velocity = 2;
		this->accelerate = 0.2f;
		this->deaccelerate = 5.0f;
		this->speed = 1.0;
		this->accelerating = false;
		this->model = qpl::mat4::rotate(qpl::mat4(1.0f), 0.0f, { 0, 0, 1 });
		this->perspective = qpl::mat4::perspective(qpl::radians(45.0f), this->aspect, this->value_near, this->value_far);


		this->set_deacceleration(10.0f);
		this->set_acceleration(15.0f);
		this->set_max_velocity(3.0f);
		this->set_position({ 3, 3, 3 });
		this->set_view_rotation({ -2.5, -0.5 });
	}

	
	qpl::vec3 qpl::camera::get_view_rotation_coordinates() const {

		auto pitch = this->view_rotation.x;
		auto yaw = this->view_rotation.y;

		qpl::vec3 result;
		result.x = std::cos(pitch) * std::cos(yaw);
		result.y = std::sin(yaw);
		//result.y = std::sin(pitch);
		result.z = std::sin(pitch) * std::cos(yaw);
		return result;
	}
	qpl::vec3 qpl::camera::get_resulting_looking_position() const {
		return this->position + this->get_view_rotation_coordinates();
	}
	qpl::mat4 qpl::camera::get_view() const {
		qpl::vec3 rot = { 0, 1, 0 };

		auto look = this->get_resulting_looking_position();
		return qpl::mat4::look_at(this->position, look, rot);
	}
	qpl::mat4 qpl::camera::get_view_projection() const {
		return this->perspective * this->get_view() * this->model;
	}

	void qpl::camera::set_position(qpl::vec3 pos) {
		this->position = pos;
	}
	void qpl::camera::set_position(qpl::f32 x, qpl::f32 y, qpl::f32 z) {
		this->set_position({ x, y, z });
	}
	void qpl::camera::set_position_x(qpl::f32 x) {
		this->position.x = x;
	}
	void qpl::camera::set_position_y(qpl::f32 y) {
		this->position.y = y;
	}
	void qpl::camera::set_position_z(qpl::f32 z) {
		this->position.z = z;
	}
	qpl::vec3 qpl::camera::get_position() const {
		return this->position;
	}

	qpl::f32 qpl::camera::get_position_x() const {
		return this->position.x;
	}
	qpl::f32 qpl::camera::get_position_y() const {
		return this->position.y;
	}
	qpl::f32 qpl::camera::get_position_z() const {
		return this->position.z;
	}

	void qpl::camera::move(qpl::vec3 delta) {
		this->position += delta;
	}
	void qpl::camera::move(qpl::f32 x, qpl::f32 y, qpl::f32 z) {
		this->move({ x, y, z });
	}
	void qpl::camera::move_x(qpl::f32 x) {
		this->position.x += x;
	}
	void qpl::camera::move_y(qpl::f32 y) {
		this->position.y += y;
	}
	void qpl::camera::move_z(qpl::f32 z) {
		this->position.z += z;
	}


	void qpl::camera::set_aspect(qpl::vec2 dimension) {
		this->aspect = dimension.x / dimension.y;
		this->perspective = qpl::mat4::perspective(qpl::radians(45.0f), this->aspect, this->value_near, this->value_far);
	}
	void qpl::camera::set_near(qpl::f32 value_near) {
		this->value_near = value_near;
		this->perspective = qpl::mat4::perspective(qpl::radians(45.0f), this->aspect, this->value_near, this->value_far);
	}
	void qpl::camera::set_far(qpl::f32 value_far) {
		this->value_far = value_far;
		this->perspective = qpl::mat4::perspective(qpl::radians(45.0f), this->aspect, this->value_near, this->value_far);
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

	void qpl::camera::set_view_rotation(qpl::vec2 pos) {
		this->set_view_rotation_x(pos.x);
		this->set_view_rotation_y(pos.y);
	}
	void qpl::camera::set_view_rotation(qpl::f32 x, qpl::f32 y) {
		this->set_view_rotation_x(x);
		this->set_view_rotation_y(y);
	}
	void qpl::camera::set_view_rotation_x(qpl::f32 x) {
		this->view_rotation.x = x;
	}
	void qpl::camera::set_view_rotation_y(qpl::f32 y) {
		this->view_rotation.y = y;
	}
	qpl::vec2 qpl::camera::get_view_rotation() const {
		return this->view_rotation;
	}
	qpl::f32 qpl::camera::get_view_rotation_x() const {
		return this->view_rotation.x;
	}
	qpl::f32 qpl::camera::get_view_rotation_y() const {
		return this->view_rotation.y;
	}

	void qpl::camera::rotate_view(qpl::vec2 delta) {
		this->set_view_rotation(this->get_view_rotation() + delta);
	}

	void qpl::camera::look_backwards() {
		this->rotate_view_y(static_cast<qpl::f32>(qpl::pi));
	}
	void qpl::camera::look_up(qpl::f32 delta) {
		this->rotate_view_y(delta);
	}
	void qpl::camera::look_down(qpl::f32 delta) {
		this->rotate_view_y(-delta);
	}
	void qpl::camera::look_left(qpl::f32 delta) {
		this->rotate_view_x(-delta);
	}
	void qpl::camera::look_right(qpl::f32 delta) {
		this->rotate_view_x(delta);
	}
	void qpl::camera::rotate_view_x(qpl::f32 x) {
		this->set_view_rotation_x(this->view_rotation.x + x);
	}
	void qpl::camera::rotate_view_y(qpl::f32 y) {
		this->set_view_rotation_y(this->view_rotation.y + y);
	}

	void qpl::camera::set_max_velocity(qpl::f32 max) {
		this->max_velocity = max;
	}
	qpl::f32 qpl::camera::get_max_velocity() const {
		return this->max_velocity;
	}

	void qpl::camera::set_acceleration(qpl::f32 acceleration) {
		this->accelerate = acceleration;
	}
	qpl::f32 qpl::camera::get_acceleration() const {
		return this->accelerate;
	}

	void qpl::camera::set_deacceleration(qpl::f32 deacceleration) {
		this->deaccelerate = deacceleration;
	}
	qpl::f32 qpl::camera::get_deacceleration() const {
		return this->deaccelerate;
	}

	void qpl::camera::accelerate_forwards(qpl::f32 delta) {
		auto vec = this->get_view_rotation_coordinates();
		if (!this->accelerate_in_view_direction) {
			vec.y = 0;
		}
		this->velocities += vec.normalized() * delta * this->accelerate;
		this->accelerating = true;
	}
	void qpl::camera::accelerate_backwards(qpl::f32 delta) {

		this->look_backwards();
		auto vec = this->get_view_rotation_coordinates();
		this->look_backwards();

		if (!this->accelerate_in_view_direction) {
			vec.y = 0;
		}
		this->velocities += vec.normalized() * delta * this->accelerate;
		this->accelerating = true;
	}
	void qpl::camera::accelerate_left(qpl::f32 delta) {
		this->look_left();
		auto vec = this->get_view_rotation_coordinates();
		this->look_right();

		if (!this->accelerate_in_view_direction) {
			vec.y = 0;
		}
		auto accel = vec.normalized() * delta * this->accelerate;
		this->velocities += accel;
		this->accelerating = true;
	}
	void qpl::camera::accelerate_right(qpl::f32 delta) {
		this->look_right();
		auto vec = this->get_view_rotation_coordinates();
		this->look_left();

		if (!this->accelerate_in_view_direction) {
			vec.y = 0;
		}
		auto accel = vec.normalized() * delta * this->accelerate;
		this->velocities += accel;
		this->accelerating = true;
	}
	void qpl::camera::accelerate_up(qpl::f32 delta) {
		this->velocities.y += delta * this->accelerate;
		this->accelerating = true;
	}
	void qpl::camera::accelerate_down(qpl::f32 delta) {
		this->velocities.y -= delta * this->accelerate;
		this->accelerating = true;
	}


	void qpl::camera::set_speed(qpl::f32 speed) {
		this->speed = speed;
	}
	qpl::f32 qpl::camera::get_speed() const {
		return this->speed;
	}

	void qpl::camera::cap_max_velocity() {
		if (this->velocities.length() > this->max_velocity) {
			this->velocities = this->velocities.normalized() * this->max_velocity;
		}
	}
	void qpl::camera::update(qpl::f32 delta_time) {
		this->cap_max_velocity();

		if (!this->accelerating) {
			auto n = delta_time * this->deaccelerate;
			auto deaccel = std::pow(0.5, n);

			this->velocities *= deaccel;
		}

		this->position += this->velocities * delta_time * this->speed;

		this->view_rotation.x = static_cast<qpl::f32>(std::fmod(this->view_rotation.x, qpl::pi * 2));
		this->view_rotation.y = static_cast<qpl::f32>(std::fmod(this->view_rotation.y + qpl::pi_32 / 2, qpl::pi * 2)) - qpl::pi_32 / 2;

		this->view_rotation.y = std::max(this->view_rotation.y, -qpl::pi_32 / 2 + 0.01f);
		this->view_rotation.y = std::min(this->view_rotation.y, qpl::pi_32 / 2 - 0.01f);
	}

#if defined QPL_INTERN_SFML_USE

	void qpl::camera::update(const qsf::event_info& event) {

		if (this->allow_looking) {
			auto delta = event.delta_mouse_position() / event.screen_dimension();
			this->look_right(delta.x * this->mouse_speed);
			this->look_down(delta.y * this->mouse_speed);
		}
		auto delta_time = qpl::f32_cast(event.frame_time_f());

		bool up = event.key_holding(sf::Keyboard::Key::Space);
		bool down = event.key_holding(sf::Keyboard::Key::LShift);
		bool left = event.key_holding(sf::Keyboard::Key::A);
		bool right = event.key_holding(sf::Keyboard::Key::D);
		bool forwards = event.key_holding(sf::Keyboard::Key::W);
		bool backwards = event.key_holding(sf::Keyboard::Key::S);

		this->accelerating = false;
		if (up) this->accelerate_up(delta_time);
		if (down) this->accelerate_down(delta_time);
		if (left) this->accelerate_left(delta_time);
		if (right) this->accelerate_right(delta_time);
		if (forwards) this->accelerate_forwards(delta_time);
		if (backwards) this->accelerate_backwards(delta_time);

		this->accelerating = left || right || up || down || forwards || backwards;

		this->update(delta_time);

		if (event.resized()) {
			this->set_aspect(event.screen_dimension());
		}
	}
	sf::RenderStates qpl::camera::get_render_states() const {
		sf::RenderStates states;
		states.transform;

		auto proj = this->get_view_projection();
		memcpy(&states.transform, &proj, sizeof(states.transform));
		return states;
	}

#endif

	qpl::vec3 qpl::camera::get_velocity() const {
		return this->velocities;
	}
}