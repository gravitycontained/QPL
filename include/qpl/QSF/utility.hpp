#ifndef QPLSF_UTILITY_HPP
#define QPLSF_UTILITY_HPP
#pragma once


#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)

#include <qpl/qpldeclspec.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/drawables.hpp>
#include <qpl/vardef.hpp>
#include <qpl/QSF/vector.hpp>
#include <SFML/Graphics/RenderStates.hpp>


namespace qsf {
	struct base_state;

	struct view_rectangle {
		sf::Mouse::Button drag_mouse_button = sf::Mouse::Left;

		QPLDLL void set_hitbox(qsf::vector2f position, qsf::vector2f dimension);
		QPLDLL void set_hitbox(const qsf::base_state& state);
		QPLDLL void reset();
		QPLDLL void set_position(qsf::vector2f position);
		QPLDLL void update(const qsf::event_info& event);
		QPLDLL qsf::vector2d position_at_mouse() const;
		QPLDLL bool empty() const;
		QPLDLL sf::RenderStates get_render_states() const;

		qsf::vrectangle hitbox;
		qsf::vector2d position = { 0,0 };
		qsf::vector2d dimension;
		qsf::vector2d scale = { 1, 1 };
		qsf::vector2d mouse_position;

		qsf::vector2d click_position;
		qsf::vector2d click_mouse_position;

		bool ignore_input = false;
		qpl::f64 rotation = 0;
		qpl::f64 zoom_delta = 0.4;
		bool hitbox_set = false;
		bool hovering = false;
		bool dragging = false;
		bool integer_step = false;
		bool power_of_two = false;
	};
}

#endif
#endif