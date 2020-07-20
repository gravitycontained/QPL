#ifndef QPL_CAMERA_HPP
#define QPL_CAMERA_HPP
#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>

namespace qpl {
	class camera {
	public:
		camera() {
			this->construct();
		}
		
		QPLDLL glm::vec3 get_view_rotation_coordinates() const;
		QPLDLL glm::vec3 get_resulting_looking_position() const;
		QPLDLL glm::mat4 get_resulting_projection() const;

		QPLDLL void set_position(glm::vec3 pos);
		QPLDLL void set_position(qpl::f32 x, qpl::f32 y, qpl::f32 z);
		QPLDLL void set_position_x(qpl::f32 x);
		QPLDLL void set_position_y(qpl::f32 y);
		QPLDLL void set_position_z(qpl::f32 z);
		QPLDLL glm::vec3 get_position() const;
		QPLDLL qpl::f32 get_position_x() const;
		QPLDLL qpl::f32 get_position_y() const;
		QPLDLL qpl::f32 get_position_z() const;

		QPLDLL void move(glm::vec3 delta);
		QPLDLL void move(qpl::f32 x, qpl::f32 y, qpl::f32 z);
		QPLDLL void move_x(qpl::f32 x);
		QPLDLL void move_y(qpl::f32 y);
		QPLDLL void move_z(qpl::f32 z);

		QPLDLL void move_forward(qpl::f32 delta = 1);
		QPLDLL void move_backwards(qpl::f32 delta = 1);
		QPLDLL void move_left(qpl::f32 delta = 1);
		QPLDLL void move_right(qpl::f32 delta = 1);
		QPLDLL void move_up(qpl::f32 delta = 1);
		QPLDLL void move_down(qpl::f32 delta = 1);

		QPLDLL void set_view_rotation(glm::vec2 pos);
		QPLDLL void set_view_rotation(qpl::f32 x, qpl::f32 y);
		QPLDLL void set_view_rotation_x(qpl::f32 x);
		QPLDLL void set_view_rotation_y(qpl::f32 y);

		QPLDLL glm::vec2 get_view_rotation() const;
		QPLDLL qpl::f32 get_view_rotation_x() const;
		QPLDLL qpl::f32 get_view_rotation_y() const;

		QPLDLL void rotate_view(glm::vec2 delta);

		QPLDLL void look_backwards();
		QPLDLL void look_up(qpl::f32 delta = qpl::pi_32 / 2);
		QPLDLL void look_down(qpl::f32 delta = qpl::pi_32 / 2);
		QPLDLL void look_left(qpl::f32 delta = qpl::pi_32 / 2);
		QPLDLL void look_right(qpl::f32 delta = qpl::pi_32 / 2);

		QPLDLL void rotate_view_x(qpl::f32 x);
		QPLDLL void rotate_view_y(qpl::f32 x);


		QPLDLL void set_max_velocity(qpl::f32 max);
		QPLDLL qpl::f32 get_max_velocity() const;

		QPLDLL void set_acceleration(qpl::f32 acceleration);
		QPLDLL qpl::f32 get_acceleration() const;

		QPLDLL void set_deacceleration(qpl::f32 deacceleration);
		QPLDLL qpl::f32 get_deacceleration() const;

		QPLDLL void accelerate_forward(qpl::f32 delta = 1);
		QPLDLL void accelerate_backwards(qpl::f32 delta = 1);
		QPLDLL void accelerate_left(qpl::f32 delta = 1);
		QPLDLL void accelerate_right(qpl::f32 delta = 1);
		QPLDLL void accelerate_up(qpl::f32 delta = 1);
		QPLDLL void accelerate_down(qpl::f32 delta = 1);

		QPLDLL void set_speed(qpl::f32 speed);
		QPLDLL qpl::f32 get_speed() const;

		QPLDLL void update(qpl::f32 delta_time);
		QPLDLL glm::vec3 get_velocity() const;
	private:
		QPLDLL void construct();
		QPLDLL void ignore_y_axis();

		glm::vec2 m_view_rotation;
		glm::vec3 m_position;

		glm::vec3 m_velocites;
		qpl::f32 m_max_velocity;
		qpl::f32 m_accelerate;
		qpl::f32 m_deaccelerate;
		qpl::f32 m_speed;
	};
}

#endif