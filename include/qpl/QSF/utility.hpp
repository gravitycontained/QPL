#ifndef QPLSF_UTILITY_HPP
#define QPLSF_UTILITY_HPP
#pragma once


#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)

#include <qpl/qpldeclspec.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/drawables.hpp>
#include <qpl/vardef.hpp>
#include <qpl/QSF/vector.hpp>


namespace qsf {
	struct base_state;

	struct view_rectangle {
		QPLDLL void set_hitbox(qsf::vector2f position, qsf::vector2f dimension);
		QPLDLL void set_hitbox(const qsf::base_state& state);
		QPLDLL void update(const qsf::event_info& event);
		QPLDLL bool empty() const;

		qsf::vrectangle hitbox;
		qsf::vector2f position = { 0,0 };
		qsf::vector2f dimension;
		qsf::vector2f scale = { 1, 1 };

		qsf::vector2f click_position;
		qsf::vector2f click_mouse_position;

		qpl::f32 rotation = 0;
		qpl::f32 zoom_delta = 0.5;
		bool hitbox_set = false;
		bool hovering = false;
		bool dragging = false;
	};
}

#endif
#endif