#ifndef QSF_UTILITY_HPP
#define QSF_UTILITY_HPP
#pragma once

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>
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
}

#endif
#endif