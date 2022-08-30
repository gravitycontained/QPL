#ifndef QPL_CAMERA_HPP
#define QPL_CAMERA_HPP
#pragma once


#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/vector.hpp>

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE
#include <qpl/QSF/event_info.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#endif

namespace qpl {
	class camera {
	public:
		camera() {
			this->construct();
		}
		
		QPLDLL qpl::vector3f get_view_rotation_coordinates() const;
		QPLDLL qpl::vector3f get_resulting_looking_position() const;
		QPLDLL qpl::mat4 get_view() const;
		QPLDLL qpl::mat4 get_view_projection() const;

		QPLDLL void set_position(qpl::vector3f pos);
		QPLDLL void set_position(qpl::f32 x, qpl::f32 y, qpl::f32 z);
		QPLDLL void set_position_x(qpl::f32 x);
		QPLDLL void set_position_y(qpl::f32 y);
		QPLDLL void set_position_z(qpl::f32 z);
		QPLDLL qpl::vector3f get_position() const;
		QPLDLL qpl::f32 get_position_x() const;
		QPLDLL qpl::f32 get_position_y() const;
		QPLDLL qpl::f32 get_position_z() const;

		QPLDLL void move(qpl::vector3f delta);
		QPLDLL void move(qpl::f32 x, qpl::f32 y, qpl::f32 z);
		QPLDLL void move_x(qpl::f32 x);
		QPLDLL void move_y(qpl::f32 y);
		QPLDLL void move_z(qpl::f32 z);

		QPLDLL void set_aspect(qpl::vector2f dimension);
		QPLDLL void set_near(qpl::f32 value_near);
		QPLDLL void set_far(qpl::f32 value_far);

		QPLDLL void move_forward(qpl::f32 delta = 1);
		QPLDLL void move_backwards(qpl::f32 delta = 1);
		QPLDLL void move_left(qpl::f32 delta = 1);
		QPLDLL void move_right(qpl::f32 delta = 1);
		QPLDLL void move_up(qpl::f32 delta = 1);
		QPLDLL void move_down(qpl::f32 delta = 1);

		QPLDLL void set_view_rotation(qpl::vector2f pos);
		QPLDLL void set_view_rotation(qpl::f32 x, qpl::f32 y);
		QPLDLL void set_view_rotation_x(qpl::f32 x);
		QPLDLL void set_view_rotation_y(qpl::f32 y);

		QPLDLL qpl::vector2f get_view_rotation() const;
		QPLDLL qpl::f32 get_view_rotation_x() const;
		QPLDLL qpl::f32 get_view_rotation_y() const;

		QPLDLL void rotate_view(qpl::vector2f delta);

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

		QPLDLL void accelerate_forwards(qpl::f32 delta);
		QPLDLL void accelerate_backwards(qpl::f32 delta);
		QPLDLL void accelerate_left(qpl::f32 delta);
		QPLDLL void accelerate_right(qpl::f32 delta);
		QPLDLL void accelerate_up(qpl::f32 delta);
		QPLDLL void accelerate_down(qpl::f32 delta);

		QPLDLL void set_speed(qpl::f32 speed);
		QPLDLL qpl::f32 get_speed() const;

		QPLDLL void update(qpl::f32 delta_time);
		QPLDLL void cap_max_velocity();

#if defined QPL_INTERN_SFML_USE
		QPLDLL void update(const qsf::event_info& event);
		QPLDLL sf::RenderStates get_render_states() const;
#endif

		QPLDLL qpl::vector3f get_velocity() const;

		QPLDLL void construct();

		qpl::mat4 perspective;
		qpl::f32 value_near = 1e-3f;
		qpl::f32 value_far = 1e3;
		qpl::f32 aspect = 16 / 9.0f;
		qpl::mat4 model;
		qpl::vector2f view_rotation;
		qpl::vector3f position;

		qpl::vector3f velocities;
		qpl::f32 max_velocity;
		qpl::f32 accelerate;
		qpl::f32 deaccelerate;
		qpl::f32 speed;
		qpl::f32 mouse_speed = 2.5f;
		bool allow_looking = true;
		bool accelerating = false;
		bool accelerate_in_view_direction = false;
	};
}

#endif