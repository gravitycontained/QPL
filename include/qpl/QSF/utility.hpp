#ifndef QSF_UTILITY_HPP
#define QSF_UTILITY_HPP
#pragma once

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>

#include <qpl/camera.hpp>
#include <qpl/QSF/framework.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics.hpp>
#include <qpl/vector.hpp>


namespace qsf {
	namespace detail {
		QPLDLL extern sf::Clipboard clipboard;
	}
	
	template<typename T> requires(qpl::is_string_type<T>())
	void copy_to_clipboard(T&& string) {
		detail::clipboard.setString(string);
	}
	template<typename T = std::wstring> requires(qpl::is_string_type<T>())
	T copy_from_clipboard() {
		auto str = detail::clipboard.getString();
		if constexpr (qpl::is_standard_string_type<T>()) {
			return str.toAnsiString();
		}
		else {
			return str;
		}
	}

	QPLDLL void run_exception_window(const std::string& exception, std::string font_path, qpl::vector2i dimension = { 1280, 720 });

	struct camera_cursor_lock {
		bool lock = false;
		bool first = true;

		template<typename T> requires (qpl::is_derived<T, qsf::base_state>())
		void cursor_on(qpl::camera& camera, T& state) {
			state.show_cursor();
			camera.allow_looking = false;
			this->lock = false;
		}
		template<typename T> requires (qpl::is_derived<T, qsf::base_state>())
		void cursor_off(qpl::camera& camera, T& state) {
			state.hide_cursor();
			camera.allow_looking = true;
			this->lock = true;

			this->set_cursor_center(state);
		}

		
		template<typename T> requires (qpl::is_derived<T, qsf::base_state>())
		void set_cursor_center(T& state) {
			state.set_cursor_position(state.center());
		}

		template<typename T> requires (qpl::is_derived<T, qsf::base_state>())
		void reset(qpl::camera& camera, T& state) {
			this->cursor_off(camera, state);
		}

		template<typename T> requires (qpl::is_derived<qpl::decay<T>, qsf::base_state>())
		void update(qpl::camera& camera, T& state) {
			if (this->first) {
				this->reset(camera, state);
				this->first = false;
			}

			if (state.has_gained_focus()) {
				this->cursor_off(camera, state);
			}
			if (state.has_lost_focus()) {
				this->cursor_on(camera, state);
			}

			if (this->lock) {
				this->set_cursor_center(state);
			}
		}
	};

}

#endif
#endif