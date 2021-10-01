#ifndef QPL_STRING_HPP
#define QPL_STRING_HPP
#pragma once

#include <qpl/algorithm.hpp>
#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/winsys.hpp>
#include <qpl/type_traits.hpp>
#include <string_view>
#include <string>
#include <sstream>
#include <charconv>
#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <iostream>
#include <bitset>
#include <regex>


#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

namespace qpl {
	template<typename T>
	concept is_cout_printable_c = requires(const T t) {
		std::cout << t;
	};
	template<typename... Args>
	constexpr bool is_cout_printable() {
		return (is_cout_printable_c<Args> && ...);
	}
	template<typename T>
	concept is_cin_readable_c = requires(T & t) {
		std::cin >> t;
	};
	template<typename... Args>
	constexpr bool is_cin_readable() {
		return (is_cin_readable_c<Args> && ...);
	}

	template<typename T>
	concept is_wcout_printable_c = requires(const T t) {
		std::wcout << t;
	};
	template<typename... Args>
	constexpr bool is_wcout_printable() {
		return (is_wcout_printable_c<Args> && ...);
	}
	template<typename T>
	concept is_wcin_readable_c = requires(T & t) {
		std::wcin >> t;
	};
	template<typename... Args>
	constexpr bool is_wcin_readable() {
		return (is_wcin_readable_c<Args> && ...);
	}


	template<typename T>
	concept is_printable_c = is_cout_printable_c<T> || is_wcout_printable_c<T>;

	template<typename T>
	concept is_readable_c = is_cin_readable_c<T> || is_wcin_readable_c<T>;

	template<typename... Args>
	constexpr bool is_printable() {
		return (static_cast<bool>(is_printable_c<Args>) && ...);
	}
	template<typename... Args>
	constexpr bool is_readable() {
		return (static_cast<bool>(is_readable_c<Args>) && ...);
	}




	QPLDLL std::wstring string_to_wstring(const std::string& str);
	QPLDLL std::wstring string_to_unicode_wstring(const std::string& str);
	QPLDLL std::string wstring_to_string(const std::wstring& str);


	template<typename... Args> requires (qpl::is_printable<Args...>())
	std::string to_string(Args&&... args) {

		std::ostringstream stream;

		constexpr auto add_to_stream = [&]<typename T>(T value) {
			if constexpr (qpl::is_wstring_type<T>()) {
				stream << qpl::wstring_to_string(value);
			}
			else {
				stream << value;
			}
		};

		(add_to_stream(args), ...);
		
		return stream.str();
	}
	template<typename T> requires qpl::is_wcout_printable_c<T>
	std::wstring to_wstring(const T& first) {
		std::wostringstream stream;
		if constexpr (qpl::is_standard_string_type<T>()) {
			stream << qpl::string_to_wstring(qpl::to_string(first));
		}
		else {
			stream << first;
		}
		return stream.str();
	}

	namespace detail {
		QPLDLL extern std::wostringstream stream_wstr;

		template<typename T>
		void add_to_wstream(const T& first) {
			if constexpr (qpl::is_standard_string_type<T>()) {
				detail::stream_wstr << qpl::string_to_wstring(qpl::to_string(first));
			}
			else {
				detail::stream_wstr << first;
			}
		}
	}

	template<typename T, typename... Args> requires (qpl::is_wcout_printable<T, Args...>())
	std::wstring to_wstring(T&& first, Args&&... args) {
		detail::stream_wstr.str(L"");

		detail::add_to_wstream(first);
		(detail::add_to_wstream(args), ...);

		return detail::stream_wstr.str();
	}

	QPLDLL std::string to_string(const std::string& first);
	QPLDLL std::wstring to_wstring(const std::wstring& first);

	QPLDLL std::string str_spaced(const std::string& string, qpl::size length = 10u, char prepend = ' ');
	QPLDLL std::wstring wstr_spaced(const std::wstring& string, qpl::size length = 10u, wchar_t prepend = ' ');
	QPLDLL std::string str_rspaced(const std::string& string, qpl::size length = 10u, char prepend = ' ');
	QPLDLL std::wstring wstr_rspaced(const std::wstring& string, qpl::size length = 10u, wchar_t prepend = ' ');
	QPLDLL std::string str_lspaced(const std::string& string, qpl::size length = 10u, char prepend = ' ');
	QPLDLL std::wstring wstr_lspaced(const std::wstring& string, qpl::size length = 10u, wchar_t prepend = ' ');
	template<typename T>
	std::string str_spaced(const T& n, qpl::size length = 10u, char prepend = ' ') {
		return qpl::str_spaced(qpl::to_string(n), length, prepend);
	}	
	template<typename T>
	std::wstring wstr_spaced(const T& n, qpl::size length = 10u, wchar_t prepend = ' ') {
		return qpl::wstr_spaced(qpl::to_wstring(n), length, prepend);
	}
	template<typename T>
	std::string str_rspaced(const T& n, qpl::size length = 10u, char prepend = ' ') {
		return qpl::str_rspaced(qpl::to_string(n), length, prepend);
	}
	template<typename T>
	std::wstring wstr_rspaced(const T& n, qpl::size length = 10u, wchar_t prepend = ' ') {
		return qpl::wstr_rspaced(qpl::to_wstring(n), length, prepend);
	}
	template<typename T>
	std::string str_lspaced(const T& n, qpl::size length = 10u, char prepend = ' ') {
		return qpl::str_lspaced(qpl::to_string(n), length, prepend);
	}


	constexpr bool string_contains(std::string_view string, char c) {
		for (qpl::i32 i = 0; i < string.length(); ++i) {
			if (string[i] == c) {
				return true;
			}
		}
		return false;
	}
	constexpr bool string_contains(const std::string_view& string, const std::string_view& sequence) {
		for (qpl::i32 i = 0; i < qpl::i32_cast(string.length()) - sequence.length(); ++i) {
			if (string.substr(i, sequence.length()) == sequence) {
				return true;
			}
		}
		return false;
	}

	QPLDLL bool string_equals_ignore_case(const std::string& a, const std::string& b);


	template<typename... Args>
	std::string to_string_ios_binary(Args&&... args) {
		std::ostringstream stream(std::ios::binary);
		((stream << args), ...);
		return stream.str();
	}

	template<typename T, typename... Args>
	std::string to_string_dash(const T& first, Args&&... args) {
		std::ostringstream stream;
		stream << first;
		((stream << qpl::to_string(" - ", args)), ...);
		return stream.str();
	}
	template<qpl::size N = 10, typename T, typename... Args>
	std::string to_string_dash_space(const T& first, Args&&... args) {
		std::ostringstream stream;
		stream << qpl::str_spaced(first, N);
		((stream << qpl::to_string(" - ", qpl::str_spaced(args, N))), ...);
		return stream.str();
	}
	template<qpl::size N = 10, typename... Args>
	std::string to_string_space(Args&&... args) {
		std::ostringstream stream;
		((stream << qpl::str_spaced(args, N)), ...);
		return stream.str();
	}
	template<qpl::size N = 10, typename... Args>
	std::string to_stringln_space(Args&&... args) {
		std::ostringstream stream;
		((stream << qpl::str_spaced(args, N)), ...);
		stream << '\n';
		return stream.str();
	}
	template<qpl::size N = 10, typename... Args>
	std::string to_string_rspace(Args&&... args) {
		std::ostringstream stream;
		((stream << qpl::str_rspaced(args, N)), ...);
		return stream.str();
	}
	template<qpl::size N = 10, typename... Args>
	std::string to_stringln_rspace(Args&&... args) {
		std::ostringstream stream;
		((stream << qpl::str_rspaced(args, N)), ...);
		stream << '\n';
		return stream.str();
	}


	template<typename... Args>
	std::string to_stringln(Args&&... args) {
		std::ostringstream stream;
		((stream << args), ...);
		stream << '\n';
		return stream.str();
	}

	template<typename... Args>
	std::wstring to_wstringln(Args&&... args) {
		std::wostringstream stream;
		((stream << qpl::to_wstring(args)), ...);
		stream << L'\n';
		return stream.str();
	}
	template<qpl::size N = 10, typename... Args>
	std::wstring to_wstringln_space(Args&&... args) {
		std::wostringstream stream;
		((stream << qpl::wstr_spaced(args, N)), ...);
		stream << '\n';
		return stream.str();
	}

	template<qpl::size N = 10, typename T, typename... Args>
	std::string to_descriptspace_string(const T& first, Args&&... args) {
		std::ostringstream stream;
		stream << (qpl::is_string_type<decltype(first)>() ? qpl::to_string(first) : qpl::to_string(qpl::str_spaced(first, N)));
		((stream << (qpl::is_string_type<decltype(args)>() ? qpl::to_string(args) : qpl::to_string(qpl::str_spaced(args, N)))), ...);
		return stream.str();
	}
	template<qpl::size N = 10, typename T, typename... Args>
	std::string to_descriptspace_string_ln(const T& first, Args&&... args) {
		std::ostringstream stream;
		stream << qpl::to_descriptspace_string<N>(first, args...);
		stream << '\n';
		return stream.str();
	}
	template<qpl::size N = 10, typename T, typename... Args>
	std::string to_descriptspace_dash_string(const T& first, Args&&... args) {
		std::ostringstream stream;
		stream << (qpl::is_string_type<T>() ? qpl::to_string(first) : qpl::to_string(qpl::str_spaced(first, N)));
		((stream << (qpl::is_string_type<decltype(args)>() ? qpl::to_string(" - ", args) : qpl::to_string(qpl::str_spaced(args, N)))), ...);
		return stream.str();
	}
	template<qpl::size N = 10, typename T, typename... Args>
	std::string to_descriptspace_dash_string_ln(const T& first, Args&&... args) {
		std::ostringstream stream;
		stream << qpl::to_descriptspace_dash_string<N>(first, args...);
		stream << '\n';
		return stream.str();
	}
	template<qpl::size N = 10, typename T, typename... Args>
	std::string to_string_dash_space_ln(const T& first, Args&&... args) {
		std::ostringstream stream;
		stream << qpl::str_spaced(first, N);
		((stream << qpl::to_string(" - ", qpl::str_spaced(args, N))), ...);
		stream << '\n';
		return stream.str();
	}

	QPLDLL std::string bool_string(bool b);

	#define qpl_vstring(name) std::string(#name)
	#define qpl_vistring(name) qpl::two_strings_fixed_insert(std::string(#name), name, 80, ' ')
	#define qpl_vidstring(name) qpl::two_strings_fixed_insert(std::string(#name), name, 80, ". ")

	#define qpl_vsprintln(name) (qpl::vprintln(std::string(#name),  name, 80, " "))
	#define qpl_vdprintln(name) (qpl::vprintln(std::string(#name),  name, 80, ".  "))
	#define qpl_vdpprintln(name) (qpl::vsprintln(std::string(#name),  name, 80, ".  "))
	#define qpl_vprintln(name) qpl::println(std::string(#name), ": ", name)
	#define qpl_cprintln(str) qpl::print(std::string(#str), " -> "); qpl::println(str);


	template<typename T, typename... Args>
	std::string to_string_line(T&& first, Args&&... args) {
		std::ostringstream stream;
		stream << first;
		((stream << '\n' << args), ...);
		return stream.str();
	}	
	template<typename T, typename... Args>
	std::string to_string_s(T&& first, Args&&... args) {
		std::ostringstream stream;
		stream << first;
		((stream << ' ' << args), ...);
		return stream.str();
	}
	template<typename T, typename... Args>
	std::string to_string_seq(T&& first, Args&&... args) {
		std::ostringstream stream;
		stream << first;
		((stream << ", " << args), ...);
		return stream.str();
	}
	template<typename T, typename... Args>
	std::string to_string_par(T&& first, Args&&... args) {
		std::ostringstream stream;
		stream << '(' << first;
		((stream << ", " << args), ...);
		stream << ')';
		return stream.str();
	}
	template<typename T, typename... Args>
	std::string to_string_square(T&& first, Args&&... args) {
		std::ostringstream stream;
		stream << '[' << first;
		((stream << ", " << args), ...);
		stream << ']';
		return stream.str();
	}
	template<typename T>
	std::string to_string_repeat(T&& value, qpl::size repeat) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < repeat; ++i) {
			stream << value;
		}
		return stream.str();
	}
	template<typename T>
	std::string to_stringln_repeat(T&& value, qpl::size repeat) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < repeat; ++i) {
			stream << value;
		}
		stream << '\n';
		return stream.str();
	}
	template<typename... Args>
	std::string to_string_precision(qpl::size precision, Args&&... args) {
		std::ostringstream stream;
		((stream << std::fixed << std::setprecision(precision) << qpl::f64_cast(args)), ...);
		return stream.str();
	}
	template<typename... Args>
	std::string to_string_full_precision(Args&&... args) {
		std::ostringstream stream;
		((stream << std::fixed << std::setprecision(qpl::f64_digits) << qpl::f64_cast(args)), ...);
		return stream.str();
	}



	template<typename... Args>
	constexpr bool is_any_string(Args&&... args) {
		return (qpl::is_string_type<Args>() || ...);
	}
	template<typename... Args>
	constexpr bool is_any_standard_string(Args&&... args) {
		return (qpl::is_standard_string_type<Args>() || ...);
	}
	template<typename... Args>
	constexpr bool is_any_wstring(Args&&... args) {
		return (qpl::is_wstring_type<Args>() || ...);
	}

	template<typename... Args>
	auto to_auto_string(Args... args) {
		if constexpr (qpl::is_any_wstring(args...)) {
			return qpl::to_wstring(args...);
		}
		else {
			return qpl::to_string(args...);
		}
	}

	template<typename... Args>
	auto to_auto_stringln(Args&&... args) {
		return qpl::to_auto_string(args..., '\n');
	}


	template<typename T>
	auto auto_str_spaced(const T& n, qpl::size length = 10u) {
		if constexpr (qpl::is_wstring_type<T>()) {
			return qpl::wstr_spaced(n, length, L' ');
		}
		else {
			return qpl::str_spaced(n, length, ' ');
		}
	}

	template<typename T>
	auto auto_str_lspaced(const T& n, qpl::size length = 10u) {
		if constexpr (qpl::is_wstring_type<T>()) {
			return qpl::wstr_lspaced(n, length, L' ');
		}
		else {
			return qpl::str_lspaced(n, length, ' ');
		}
	}

	template<typename T>
	auto auto_str_rspaced(const T& n, qpl::size length = 10u) {
		if constexpr (qpl::is_wstring_type<T>()) {
			return qpl::wstr_rspaced(n, length, L' ');
		}
		else {
			return qpl::str_rspaced(n, length, ' ');
		}
	}

	template<typename T, typename... Args>
	std::wstring to_wstring_line(T&& first, Args&&... args) {
		std::wostringstream stream;
		stream << first;
		((stream << '\n' << args), ...);
		return stream.str();
	}
	template<typename T, typename... Args>
	std::wstring to_wstring_s(T&& first, Args&&... args) {
		std::wostringstream stream;
		stream << first;
		((stream << ' ' << args), ...);
		return stream.str();
	}
	template<typename T, typename... Args>
	std::wstring to_wstring_seq(T&& first, Args&&... args) {
		std::wostringstream stream;
		stream << first;
		((stream << ", " << args), ...);
		return stream.str();
	}
	template<typename T, typename... Args>
	std::wstring to_wstring_par(T&& first, Args&&... args) {
		std::wostringstream stream;
		stream << '(' << first;
		((stream << ", " << args), ...);
		stream << ')';
		return stream.str();
	}
	template<typename T, typename... Args>
	std::wstring to_wstring_square(T&& first, Args&&... args) {
		std::wostringstream stream;
		stream << '[' << first;
		((stream << ", " << args), ...);
		stream << ']';
		return stream.str();
	}
	template<typename T>
	std::wstring to_wstring_repeat(T&& value, qpl::size repeat) {
		std::wostringstream stream;
		for (auto i = qpl::size{}; i < repeat; ++i) {
			stream << value;
		}
		return stream.str();
	}
	template<typename T>
	std::wstring to_wstringln_repeat(T&& value, qpl::size repeat) {
		std::wostringstream stream;
		for (auto i = qpl::size{}; i < repeat; ++i) {
			stream << value;
		}
		stream << L'\n';
		return stream.str();
	}
	template<typename... Args>
	std::string to_wstring_precision(qpl::size precision, Args&&... args) {
		std::wostringstream stream;
		((stream << std::fixed << std::setprecision(precision) << qpl::is_integer<decltype(args)>() ? static_cast<qpl::f64>(args) : args), ...);
		return stream.str();
	}

	template<class S1, class S2>
	auto string_cast(const S2& string) {
		if constexpr (std::is_same_v<S1, S2>) {
			return string;
		}
		else if constexpr (std::is_same_v<S1, std::wstring>) {
			if constexpr (std::is_same_v<S2, std::string>) {
				return qpl::string_to_wstring(string);
			}
			else {
				return qpl::to_wstring(string);
			}
		}
		else if constexpr (std::is_same_v<S1, std::string>) {
			if constexpr (std::is_same_v<S2, std::wstring>) {
				return qpl::wstring_to_string(string);
			}
			else {
				return qpl::to_string(string);
			}
		}
		else {
			static_assert("invalid string_cast argument");
		}
	}

	QPLDLL inline std::string string_to_fit(const std::string& string, char append, qpl::size length);
	QPLDLL std::string appended_to_string_to_fit(const std::string& string, char append, qpl::size length);
	template<typename T>
	std::string appended_to_string_to_fit(const T& value, char append, qpl::size length) {
		return qpl::appended_to_string_to_fit(qpl::to_string(value), append, length);
	}

	QPLDLL std::string appended_to_string_to_fit(const std::string& string, const std::string_view& prepend, qpl::size length);
	template<typename T>
	std::string appended_to_string_to_fit(const T& value, const std::string_view& prepend, qpl::size length) {
		return qpl::appended_to_string_to_fit(qpl::to_string(value), prepend, length);
	}

	QPLDLL std::string prepended_to_string_to_fit(const std::string& string, char prepend, qpl::size length);
	template<typename T>
	std::string prepended_to_string_to_fit(const T& value, char prepend, qpl::size length) {
		return qpl::prepended_to_string_to_fit(qpl::to_string(value), prepend, length);
	}

	QPLDLL std::string prepended_to_string_to_fit(const std::string& string, const std::string_view& prepend, qpl::size length);
	template<typename T>
	std::string prepended_to_string_to_fit(const T& value, const std::string_view& prepend, qpl::size length) {
		return qpl::prepended_to_string_to_fit(qpl::to_string(value), prepend, length);
	}

	QPLDLL std::string two_strings_fixed_insert(const std::string& a, const std::string& b, const std::string_view& insert, qpl::size length, qpl::u32 rotation = 0u);
	template<typename T, typename U>
	std::string two_strings_fixed_insert(const T& a, const U& b, const std::string_view& insert, qpl::size length, qpl::u32 rotation = 0u) {
		return qpl::two_strings_fixed_insert(qpl::to_string(a), qpl::to_string(b), insert, length);
	}

	template<typename ...Args>
	std::string console_space(qpl::size n, const Args&... args) {
		return qpl::console_space(n, qpl::to_string(args...));
	}
	template<>
	QPLDLL std::string console_space(qpl::size n, const std::string& string);


	QPLDLL inline std::wstring wstring_to_fit(const std::wstring& string, wchar_t append, qpl::size length);
	QPLDLL std::wstring appended_to_wstring_to_fit(const std::wstring& string, wchar_t append, qpl::size length);
	QPLDLL std::wstring prepended_to_wstring_to_fit(const std::wstring& string, wchar_t prepend, qpl::size length);

	struct print_effect {
		
	};
	QPLDLL std::ostream& operator<<(std::ostream& os, print_effect effect);
	struct t_cclearln : print_effect {
		const t_cclearln& operator()() const {
			return *this;
		}
	};

	struct t_cclear : print_effect {

		const t_cclear& operator()() const {
			return *this;
		}
	};

	struct cspace : qpl::print_effect {
		cspace(qpl::u32 value) : value(value) {

		}
		qpl::u32 value;
	};
	struct cspaceln : qpl::print_effect {
		cspaceln(qpl::u32 value) : value(value) {

		}
		qpl::u32 value;
	};
	struct cspace_permament : qpl::print_effect {
		cspace_permament(qpl::u32 value) : value(value) {

		}
		qpl::u32 value;
	};
	struct ccolor : qpl::print_effect {
		ccolor(qpl::cc value) : value(value) {

		}
		ccolor(qpl::color foreground) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
		}
		ccolor(qpl::foreground foreground) {
			this->value.foreground = foreground;
		}
		ccolor(qpl::background background) {
			this->value.background = background;
		}
		ccolor(qpl::color foreground, qpl::color background) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
			this->value.background = static_cast<qpl::background>(background);
		}
		ccolor(qpl::color foreground, qpl::background background) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
			this->value.background = background;
		}
		ccolor(qpl::foreground foreground, qpl::color background) {
			this->value.foreground = foreground;
			this->value.background = static_cast<qpl::background>(background);
		}
		ccolor(qpl::foreground foreground, qpl::background background) {
			this->value.foreground = foreground;
			this->value.background = background;
		}
		ccolor(qpl::background background, qpl::foreground foreground) {
			this->value.foreground = foreground;
			this->value.background = background;
		}
		qpl::cc value;
	};
	struct ccolorln : qpl::print_effect {
		ccolorln(qpl::cc value) : value(value) {

		}
		ccolorln(qpl::color foreground) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
		}
		ccolorln(qpl::foreground foreground) {
			this->value.foreground = foreground;
		}
		ccolorln(qpl::background background) {
			this->value.background = background;
		}
		ccolorln(qpl::color foreground, qpl::color background) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
			this->value.background = static_cast<qpl::background>(background);
		}
		ccolorln(qpl::color foreground, qpl::background background) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
			this->value.background = background;
		}
		ccolorln(qpl::foreground foreground, qpl::color background) {
			this->value.foreground = foreground;
			this->value.background = static_cast<qpl::background>(background);
		}
		ccolorln(qpl::foreground foreground, qpl::background background) {
			this->value.foreground = foreground;
			this->value.background = background;
		}
		ccolorln(qpl::background background, qpl::foreground foreground) {
			this->value.foreground = foreground;
			this->value.background = background;
		}
		qpl::cc value;
	};
	struct ccolor_permament : qpl::print_effect {
		ccolor_permament(qpl::cc value) : value(value) {

		}
		ccolor_permament(qpl::color foreground) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
		}
		ccolor_permament(qpl::foreground foreground) {
			this->value.foreground = foreground;
		}
		ccolor_permament(qpl::background background) {
			this->value.background = background;
		}
		ccolor_permament(qpl::color foreground, qpl::color background) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
			this->value.background = static_cast<qpl::background>(background);
		}
		ccolor_permament(qpl::color foreground, qpl::background background) {
			this->value.foreground = static_cast<qpl::foreground>(foreground);
			this->value.background = background;
		}
		ccolor_permament(qpl::foreground foreground, qpl::color background) {
			this->value.foreground = foreground;
			this->value.background = static_cast<qpl::background>(background);
		}
		ccolor_permament(qpl::foreground foreground, qpl::background background) {
			this->value.foreground = foreground;
			this->value.background = background;
		}
		ccolor_permament(qpl::background background, qpl::foreground foreground) {
			this->value.foreground = foreground;
			this->value.background = background;
		}
		qpl::cc value;
	};

	QPLDLL extern t_cclearln cclearln;
	QPLDLL extern t_cclear cclear;
	namespace detail {
		QPLDLL extern qpl::cc current_console_color;
		QPLDLL extern qpl::u32 next_print_space;
		QPLDLL extern bool next_println_space;
		QPLDLL extern qpl::u32 println_space;
		QPLDLL extern qpl::u32 println_default_space;
		QPLDLL extern bool next_print_color;
		QPLDLL extern bool next_println_color;
		QPLDLL extern qpl::cc println_color;
		QPLDLL extern qpl::cc println_default_color;
	}

	template<typename C> requires qpl::is_container_c<C>
	inline std::string container_to_string(const C& data, std::string_view delimiter = ", ", std::string_view brackets = "{}") {
		std::ostringstream str;
		str << brackets.front();
		bool start = true;
		for (const auto& e : data) {
			if constexpr (qpl::is_container<qpl::container_subtype<C>>() && !qpl::is_string_type<C>()) {
				if (!start) {
					str << delimiter;
				}
				str << container_to_string(e);
				start = false;
			}
			else {
				if (!start) {
					str << delimiter;
				}
				str << e;
				start = false;
			}
		}
		str << brackets.back();
		return str.str();
	}


	template<typename T> requires (qpl::is_printable<T>())
	inline void single_print(T&& value) {
		if constexpr (!qpl::is_derived<std::decay_t<T>, qpl::print_effect>() && !qpl::is_same<std::decay_t<T>, qpl::color>() && !qpl::is_same<std::decay_t<T>, qpl::background>() && !qpl::is_same<std::decay_t<T>, qpl::foreground>() && !qpl::is_same<std::decay_t<T>, qpl::cc>()) {
			bool backslashn = false;
			bool newln = false;
			if constexpr (qpl::is_same<std::decay_t<T>, char>()) {
				if (value == '\n') {
					backslashn = true;
				}
			}
			else {
				if constexpr (qpl::is_standard_string_type<T>()) {
					if (qpl::string_contains(value, '\n')) {
						newln = true;
					}
				}
			}

			if (backslashn) {
				if constexpr (qpl::is_wstring_type<T>()) {
					std::wcout << value;
				}
				else {
					std::cout << value;
				}
			}
			else {
				if (qpl::detail::next_print_space) {
					if constexpr (qpl::is_same_decayed<T, std::wstring>() || qpl::is_same_decayed<T, wchar_t>()) {
						std::wcout << qpl::wstr_spaced(value, qpl::detail::next_print_space);
					}
					else {
						std::cout << qpl::str_spaced(value, qpl::detail::next_print_space);
					}
					qpl::detail::next_print_space = qpl::detail::println_space;
				}
				else {
					if constexpr (qpl::is_wstring_type<T>()) {
						std::wcout << value;
					}
					else {
						std::cout << value;
					}
				}
				if (qpl::detail::next_print_color) {
					qpl::set_console_color(qpl::detail::println_color);
				}
			}
			if (newln || backslashn) {
				qpl::detail::println_color = qpl::detail::println_default_color;
				qpl::detail::current_console_color = qpl::detail::println_color;

				qpl::detail::next_println_color = false;
				qpl::detail::println_space = qpl::detail::println_default_space;
				qpl::detail::next_println_space = false;
			}
		}

		if constexpr (qpl::is_derived<std::decay_t<T>, qpl::print_effect>()) {
			if constexpr (qpl::is_same<std::decay_t<T>, cspace>()) {
				qpl::detail::next_print_space = value.value;
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, cspaceln>()) {
				qpl::detail::println_space = value.value;
				qpl::detail::next_println_space = true;
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, cspace_permament>()) {
				qpl::detail::println_default_space = value.value;
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, ccolor>()) {
				qpl::detail::next_print_color = true;
				qpl::set_console_color(value.value);
				qpl::detail::current_console_color = value.value;
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, ccolorln>()) {
				qpl::detail::println_color = value.value;
				qpl::detail::next_println_color = true;
				qpl::set_console_color(value.value);
				qpl::detail::current_console_color = value.value;
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, ccolor_permament>()) {
				qpl::detail::println_default_color = value.value;
				qpl::detail::println_color = value.value;
				qpl::set_console_color(value.value);
				qpl::detail::current_console_color = value.value;
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, qpl::t_cclearln>()) {
				std::cout << qpl::to_string("\r", qpl::to_string_repeat(' ', 40));
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, qpl::t_cclear>()) {
				qpl::clear_console();
			}
			return;
		}
		else if constexpr (qpl::is_same<std::decay_t<T>, qpl::color>()) {
			qpl::detail::next_print_color = true;
			qpl::set_console_color(value);
			qpl::detail::current_console_color.foreground = value;
			return;
		}
		else if constexpr (qpl::is_same<std::decay_t<T>, qpl::foreground>()) {
			qpl::detail::next_print_color = true;
			qpl::detail::current_console_color.foreground = value;
			qpl::set_console_color(qpl::detail::current_console_color);
			return;
		}
		else if constexpr (qpl::is_same<std::decay_t<T>, qpl::background>()) {
			qpl::detail::next_print_color = true;
			qpl::detail::current_console_color.background = value;
			qpl::set_console_color(qpl::detail::current_console_color);
			return;
		}
		else if constexpr (qpl::is_same<std::decay_t<T>, qpl::cc>()) {
			qpl::detail::next_print_color = true;
			qpl::set_console_color(value);
			qpl::detail::current_console_color = value;
			return;
		}
	}

	template<typename... Args> requires (qpl::is_printable<Args...>())
	inline void print(Args&&... args) {
		((qpl::single_print(args)), ...);
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_ln(T&& first, Args&&... args) {
		qpl::print(first);
		(qpl::print('\n', args), ...);
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_s(T&& first, Args&&... args) {
		qpl::print(first);
		(qpl::print(' ', args), ...);
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_dash(T&& first, Args&&... args) {
		qpl::print(first);
		(qpl::print(" - ", args), ...);
	}
	template<qpl::size N = 10, typename... Args> requires (qpl::is_printable<Args...>())
	inline void print_space(Args&&... args) {
		qpl::print(qpl::to_string_space<N>(args...));
	}
	template<qpl::size N = 10, typename... Args> requires (qpl::is_printable<Args...>())
	inline void print_rspace(Args&&... args) {
		qpl::print(qpl::to_string_rspace<N>(args...));
	}
	template<qpl::size N = 10, typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_dash_space(T&& first, Args&&... args) {
		qpl::print(qpl::to_string_dash_space<N>(first, args...));
	}
	template<qpl::size N = 10, typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_descriptspace(T&& first, Args&&... args) {
		qpl::print(qpl::to_descriptspace_string<N>(first, args...));
	}
	template<qpl::size N = 10, typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_descriptspace_dash(T&& first, Args&&... args) {
		qpl::print(qpl::to_descriptspace_dash_string<N>(first, args...));
	}
	template<qpl::size N = 10, typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_format(T&& first, Args&&... args) {
		qpl::print_descriptspace_dash<N>(first, args...);
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_seq(T&& first, Args&&... args) {
		qpl::print(first);
		(qpl::print(", ", args), ...);
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_par(T&& first, Args&&... args) {
		qpl::print('(', first);
		(qpl::print(", ", args), ...);
		qpl::print(')');
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void print_sq(T&& first, Args&&... args) {
		qpl::print('[', first);
		(qpl::print(", ", args), ...);
		qpl::print(']');
	}
	template<typename T> requires (qpl::is_printable<T>())
	inline void print_repeat(T&& value, qpl::size repeat) {
		qpl::print(qpl::to_string_repeat(value, repeat));
	}
	template<typename... Args> requires (qpl::is_printable<Args...>())
	inline void print_precision(qpl::size precision, Args&&... args) {
		qpl::print(qpl::to_string_precision(precision, args...));
	}
	template<typename... Args> requires (qpl::is_printable<Args...>())
	inline void print_full_precision(Args&&... args) {
		qpl::print(qpl::to_string_full_precision(args...));
	}
	template<typename T, typename U> requires (qpl::is_printable<T, U>())
	inline void vprint(const T& a, const U& b, qpl::size length = 60, const std::string_view& insert = ".  ", qpl::u32 rotation = 0u) {
		qpl::print(qpl::two_strings_fixed_insert(qpl::to_string(a, ": "), qpl::to_string(' ', b), length, insert, rotation));
	}
	template<typename T> requires (qpl::is_printable<T>())
	inline void vprint(const T& a, qpl::size length = 60, const std::string_view& insert = ".  ", qpl::u32 rotation = 0u) {
		qpl::vprint("value", a, length, insert, rotation);
	}
	template<typename T, typename U> requires (qpl::is_printable<T, U>())
	inline void vsprint(const T& a, const U& b, qpl::size length = 60, const std::string_view& insert = ".  ", qpl::u32 rotation = 0u) {
		qpl::print(qpl::two_strings_fixed_insert(qpl::to_string(a, ": "), qpl::to_string(" \"", b, "\""), insert, length, rotation));
	}
	template<typename T> requires (qpl::is_printable<T>())
	inline void vsprint(const T& a, qpl::size length = 60, const std::string_view& insert = ".  ", qpl::u32 rotation = 0u) {
		qpl::vsprint("value", a, length, insert, rotation);
	}

	template<typename... Args> requires (qpl::is_printable<Args...>())
	inline void println(Args&&... args) {
		qpl::print(args...);
		qpl::print('\n');
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_ln(T&& first, Args&&... args) {
		qpl::print_ln(first, args...);
		qpl::print('\n');
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_s(T&& first, Args&&... args) {
		qpl::print_s(first, args...);
		qpl::print('\n');
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_dash(T&& first, Args&&... args) {
		qpl::print_dash(first, args...);
		qpl::print('\n');
	}
	template<qpl::size N = 10, typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_dash_space(T&& first, Args&&... args) {
		qpl::print_dash_space<N>(first, args...);
		qpl::print('\n');
	}
	template<qpl::size N = 10, typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_descriptspace(T&& first, Args&&... args) {
		qpl::print_descriptspace<N>(first, args...);
		qpl::print('\n');
	}
	template<qpl::size N = 10, typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_descriptspace_dash(T&& first, Args&&... args) {
		qpl::print_descriptspace_dash<N>(first, args...);
		qpl::print('\n');
	}
	template<qpl::size N = 10, typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_format(T&& first, Args&&... args) {
		qpl::print_format<N>(first, args...);
		qpl::print('\n');
	}
	template<qpl::size N = 10, typename... Args> requires (qpl::is_printable<Args...>())
	inline void println_space(Args&&... args) {
		qpl::print_space<N>(args...);
		qpl::print('\n');
	}
	template<qpl::size N = 10, typename... Args> requires (qpl::is_printable<Args...>())
	inline void println_rspace(Args&&... args) {
		qpl::print_rspace<N>(args...);
		qpl::print('\n');
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_seq(T&& first, Args&&... args) {
		qpl::print_seq(first, args...);
		qpl::print('\n');
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_par(T&& first, Args&&... args) {
		qpl::print_par(first, args...);
		qpl::print('\n');
	}
	template<typename T, typename... Args> requires (qpl::is_printable<T, Args...>())
	inline void println_sq(T&& first, Args&&... args) {
		qpl::print_sq(first, args...);
		qpl::print('\n');
	}
	template<typename T> requires (qpl::is_printable<T>())
	inline void println_repeat(T&& value, qpl::size repeat) {
		qpl::print_repeat(value, repeat);
		qpl::print('\n');
	}
	template<typename... Args> requires (qpl::is_printable<Args...>())
	inline void println_precision(qpl::size precision, Args&&... args) {
		qpl::println(qpl::to_string_precision(precision, args...));
	}
	template<typename... Args> requires (qpl::is_printable<Args...>())
	inline void println_full_precision(Args&&... args) {
		qpl::println(qpl::to_string_full_precision(args...));
	}
	template<typename T, typename U> requires (qpl::is_printable<T, U>())
	inline void vprintln(const T& a, const U& b, qpl::size length = 60, const std::string_view& insert = ".  ", qpl::u32 rotation = 0u) {
		qpl::println(qpl::two_strings_fixed_insert(qpl::to_string(a, ": "), qpl::to_string(' ', b), insert, length, rotation));
	}
	template<typename T> requires (qpl::is_printable<T>())
	inline void vprintln(const T& a, qpl::size length = 60, const std::string_view& insert = ".  ", qpl::u32 rotation = 0u) {
		qpl::vprintln("value", a, length, insert, rotation);
	}
	template<typename T, typename U> requires (qpl::is_printable<T>())
	inline void vsprintln(const T& a, const U& b, qpl::size length = 60, const std::string_view& insert = ".  ", qpl::u32 rotation = 0u) {
		qpl::println(qpl::two_strings_fixed_insert(qpl::to_string(a, ": "), qpl::to_string(" \"", b, "\""), insert, length, rotation));
	}
	template<typename T> requires (qpl::is_printable<T>())
	inline void vsprintln(const T& a, qpl::size length = 60, const std::string_view& insert = ".  ", qpl::u32 rotation = 0u) {
		qpl::vsprintln("value", a, length, insert, rotation);
	}


	template<typename T, typename U>
	inline void print_diff_green_red(T&& first, U&& second) {
		auto str1 = qpl::to_string(first);
		auto str2 = qpl::to_string(second);

		for (qpl::u32 i = 0u; i < str1.length(); ++i) {
			if (i < str2.length() && str1[i] == str2[i]) {
				qpl::print(qpl::cc(qpl::background::light_green, qpl::foreground::black), str1[i]);
			}
			else {
				qpl::print(qpl::cc(qpl::background::light_red, qpl::foreground::black), str1[i]);
			}
		}
	}

	template<typename T, typename U>
	inline void println_diff_green_red(T&& first, U&& second) {
		auto str1 = qpl::to_string(first);
		auto str2 = qpl::to_string(second);

		for (qpl::u32 i = 0u; i < str1.length(); ++i) {
			if (i < str2.length() && str1[i] == str2[i]) {
				qpl::print(qpl::cc(qpl::background::light_green, qpl::foreground::black), str1[i]);
			}
			else {
				qpl::print(qpl::cc(qpl::background::light_red, qpl::foreground::black), str1[i]);
			}
		}
		qpl::println();
		for (qpl::u32 i = 0u; i < str2.length(); ++i) {
			if (i < str1.length() && str1[i] == str2[i]) {
				qpl::print(qpl::cc(qpl::background::light_green, qpl::foreground::black), str2[i]);
			}
			else {
				qpl::print(qpl::cc(qpl::background::light_red, qpl::foreground::black), str2[i]);
			}
		}
		qpl::println();
	}

	template<typename T, typename U>
	inline void print_diff(T&& first, U&& second) {
		auto str1 = qpl::to_string(first);
		auto str2 = qpl::to_string(second);

		for (qpl::u32 i = 0u; i < str1.length(); ++i) {
			if (i < str2.length() && str1[i] == str2[i]) {
				qpl::print(str1[i]);
			}
			else {
				qpl::print(qpl::cc(qpl::background::light_red, qpl::foreground::black), str1[i]);
			}
		}
	}

	template<typename T, typename U>
	inline void println_diff(T&& first, U&& second) {
		auto str1 = qpl::to_string(first);
		auto str2 = qpl::to_string(second);

		for (qpl::u32 i = 0u; i < str1.length(); ++i) {
			if (i < str2.length() && str1[i] == str2[i]) {
				qpl::print(str1[i]);
			}
			else {
				qpl::print(qpl::cc(qpl::background::light_red, qpl::foreground::black), str1[i]);
			}
		}
		qpl::println();
		for (qpl::u32 i = 0u; i < str2.length(); ++i) {
			if (i < str1.length() && str1[i] == str2[i]) {
				qpl::print(str2[i]);
			}
			else {
				qpl::print(qpl::cc(qpl::background::light_red, qpl::foreground::black), str2[i]);
			}
		}
		qpl::println();
	}

	template<typename C>  requires qpl::is_container_c<C>
	inline void print_container(const C& data, std::string_view delimiter = ", ") {
		qpl::print(qpl::container_to_string(data));
	}
	template<typename T, typename U>
	std::string pair_to_string(const std::pair<T, U>& pair) {
		return qpl::to_string('{', pair.first, ", ", pair.second, '}');
	}
	template<typename C>  requires qpl::is_container_c<C>
	inline void println_container(const C& data) {
		qpl::println(qpl::container_to_string(data));
	}

	enum class base_format {
		base64, base36u, base36l
	};

	namespace detail{
		constexpr std::array<qpl::u8, 64> base_64 = { 
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
			'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
			'U', 'V', 'W', 'X', 'Y', 'Z',
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
			'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
			'u', 'v', 'w', 'x', 'y', 'z',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'+', '/'
		}; 
		constexpr std::array<qpl::u8, qpl::type_configurations<qpl::u8>()> base_64_inv = {
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0x3e}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0x3f}, qpl::u8{0x34}, qpl::u8{0x35},
			qpl::u8{0x36}, qpl::u8{0x37}, qpl::u8{0x38}, qpl::u8{0x39}, qpl::u8{0x3a}, qpl::u8{0x3b}, qpl::u8{0x3c}, qpl::u8{0x3d}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0x0},  qpl::u8{0x1},  qpl::u8{0x2},  qpl::u8{0x3},  qpl::u8{0x4},
			qpl::u8{0x5},  qpl::u8{0x6},  qpl::u8{0x7},  qpl::u8{0x8},  qpl::u8{0x9},  qpl::u8{0xa},  qpl::u8{0xb},  qpl::u8{0xc},  qpl::u8{0xd},  qpl::u8{0xe},
			qpl::u8{0xf},  qpl::u8{0x10}, qpl::u8{0x11}, qpl::u8{0x12}, qpl::u8{0x13}, qpl::u8{0x14}, qpl::u8{0x15}, qpl::u8{0x16}, qpl::u8{0x17}, qpl::u8{0x18},
			qpl::u8{0x19}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0x1a}, qpl::u8{0x1b}, qpl::u8{0x1c},
			qpl::u8{0x1d}, qpl::u8{0x1e}, qpl::u8{0x1f}, qpl::u8{0x20}, qpl::u8{0x21}, qpl::u8{0x22}, qpl::u8{0x23}, qpl::u8{0x24}, qpl::u8{0x25}, qpl::u8{0x26},
			qpl::u8{0x27}, qpl::u8{0x28}, qpl::u8{0x29}, qpl::u8{0x2a}, qpl::u8{0x2b}, qpl::u8{0x2c}, qpl::u8{0x2d}, qpl::u8{0x2e}, qpl::u8{0x2f}, qpl::u8{0x30},
			qpl::u8{0x31}, qpl::u8{0x32}, qpl::u8{0x33}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}
		};
		constexpr std::array<qpl::u8, 36> base_36_upper = {
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
			'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
			'U', 'V', 'W', 'X', 'Y', 'Z'
		};

		constexpr std::array<qpl::u8, 36> base_36_lower = {
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
			'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
			'u', 'v', 'w', 'x', 'y', 'z'
		};
		constexpr std::array<qpl::u8, qpl::type_configurations<qpl::u8>()> base_36_inv = {
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0x0},  qpl::u8{0x1},
			qpl::u8{0x2},  qpl::u8{0x3},  qpl::u8{0x4},  qpl::u8{0x5},  qpl::u8{0x6},  qpl::u8{0x7},  qpl::u8{0x8},  qpl::u8{0x9},  qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xa},  qpl::u8{0xb},  qpl::u8{0xc},  qpl::u8{0xd},  qpl::u8{0xe},
			qpl::u8{0xf},  qpl::u8{0x10}, qpl::u8{0x11}, qpl::u8{0x12}, qpl::u8{0x13}, qpl::u8{0x14}, qpl::u8{0x15}, qpl::u8{0x16}, qpl::u8{0x17}, qpl::u8{0x18},
			qpl::u8{0x19}, qpl::u8{0x1a}, qpl::u8{0x1b}, qpl::u8{0x1c}, qpl::u8{0x1d}, qpl::u8{0x1e}, qpl::u8{0x1f}, qpl::u8{0x20}, qpl::u8{0x21}, qpl::u8{0x22},
			qpl::u8{0x23}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xa},  qpl::u8{0xb},  qpl::u8{0xc},
			qpl::u8{0xd},  qpl::u8{0xe},  qpl::u8{0xf},  qpl::u8{0x10}, qpl::u8{0x11}, qpl::u8{0x12}, qpl::u8{0x13}, qpl::u8{0x14}, qpl::u8{0x15}, qpl::u8{0x16},
			qpl::u8{0x17}, qpl::u8{0x18}, qpl::u8{0x19}, qpl::u8{0x1a}, qpl::u8{0x1b}, qpl::u8{0x1c}, qpl::u8{0x1d}, qpl::u8{0x1e}, qpl::u8{0x1f}, qpl::u8{0x20},
			qpl::u8{0x21}, qpl::u8{0x22}, qpl::u8{0x23}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff},
			qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}, qpl::u8{0xff}
		};

		constexpr std::array<bool, qpl::type_configurations<qpl::u8>()> operator_table = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};

		constexpr std::array<bool, qpl::type_configurations<qpl::u8>()> unary_left_operator_table = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};

		constexpr std::array<bool, qpl::type_configurations<qpl::u8>()> unary_right_operator_table = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};

		constexpr std::array<bool, qpl::type_configurations<qpl::u8>()> white_space_table = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};

		constexpr std::array<bool, qpl::type_configurations<qpl::u8>()> parantheses_table = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};

		constexpr std::array<bool, qpl::type_configurations<qpl::u8>()> arithmetic_special_table = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};
	}





	template<typename T> requires (qpl::is_integer<T>())
	std::string base_string(T value, T base, const std::string& prefix = "", base_format base_format = base_format::base36l, bool prepend_zeroes = false) {
		if (value == T{}) {
			if (prepend_zeroes) {
				switch (base_format) {
				case base_format::base36l:
					return prefix + qpl::to_string_repeat(static_cast<qpl::char_type>(qpl::detail::base_36_lower[0]), qpl::base_max_log(base));
				case base_format::base36u:
					return prefix + qpl::to_string_repeat(static_cast<qpl::char_type>(qpl::detail::base_36_upper[0]), qpl::base_max_log(base));
				case base_format::base64:
					return prefix + qpl::to_string_repeat(static_cast<qpl::char_type>(qpl::detail::base_64[0]), qpl::base_max_log(base));
				}
			}
			else {
				switch (base_format) {
				case base_format::base36l:
					return prefix + qpl::to_string(static_cast<qpl::char_type>(qpl::detail::base_36_lower[0]));
				case base_format::base36u:
					return prefix + qpl::to_string(static_cast<qpl::char_type>(qpl::detail::base_36_upper[0]));
				case base_format::base64:
					return prefix + qpl::to_string(static_cast<qpl::char_type>(qpl::detail::base_64[0]));
				}
			}
		}

		std::ostringstream stream;
		std::ostringstream digits;

		if constexpr (qpl::is_signed<T>()) {
			if (value < T{}) {
				stream << "-";
				value *= T{ -1 };
			}
		}
		stream << prefix;


		auto logapprox = qpl::log_approximation(base, value);
		T mod;

		switch (base_format) {
		case base_format::base36l:
			while (logapprox) {
				mod = value % logapprox;
				digits << static_cast<qpl::char_type>(qpl::detail::base_36_lower[value / logapprox]);
				value = mod;
				logapprox /= base;
			} break;
		case base_format::base36u:
			while (logapprox) {
				mod = value % logapprox;
				digits << static_cast<qpl::char_type>(qpl::detail::base_36_upper[value / logapprox]);
				value = mod;
				logapprox /= base;
			} break;
		case base_format::base64:
			while (logapprox) {
				mod = value % logapprox;
				digits << static_cast<qpl::char_type>(qpl::detail::base_64[value / logapprox]);
				value = mod;
				logapprox /= base;
			} break;
		}

		auto str = digits.str();
		if (prepend_zeroes) {
			qpl::i64 left = qpl::i64_cast(qpl::base_max_log(base)) - qpl::i64_cast(str.length());

			if (left > qpl::i64{}) {
				switch (base_format) {
				case base_format::base36l:
					stream << qpl::to_string_repeat(static_cast<qpl::char_type>(qpl::detail::base_36_lower[0]), left);
					break;
				case base_format::base36u:
					stream << qpl::to_string_repeat(static_cast<qpl::char_type>(qpl::detail::base_36_upper[0]), left);
					break;
				case base_format::base64:
					stream << qpl::to_string_repeat(static_cast<qpl::char_type>(qpl::detail::base_64[0]), left);
					break;
				}
			}
		}

		stream << digits.str();
		return stream.str();
	}


	template<typename T> requires (qpl::is_integer<T>())
	char base_char(T value, T base, bool upper36 = false) {
		if (base <= 36) {
			if (upper36) {
				return qpl::detail::base_36_upper[value];
			}
			else {
				return qpl::detail::base_36_lower[value];
			}
		}
		else {
			return qpl::detail::base_64[value];
		}
	}

	template<typename T> requires (qpl::is_integer<T>())
	T from_base_char(char c, T base) {
		if (base <= 36) {
			return qpl::detail::base_36_inv[c];
		}
		else {
			return qpl::detail::base_64_inv[c];
		}
	}

	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr T from_string(const std::string_view& string) {
		if constexpr (qpl::is_same_decayed<T, std::string>()) {
			return string;
		}
		else if constexpr (qpl::is_same_decayed<T, qpl::f64>()) {
			return std::strtod(string.data(), nullptr);
		}
		else if constexpr (qpl::is_same_decayed<T, qpl::f32>()) {
			return std::strtof(string.data(), nullptr);
		}
		else if constexpr (qpl::is_qpl_integer<T>()) {
			return T{ string };
		}
		else {
			return static_cast<T>(std::atoll(string.data()));
		}
	}
	template<typename T> requires (qpl::is_same_decayed<T, std::string>())
	constexpr T from_string(const std::string& string) {
		return string;
	}

	template<typename T> requires (qpl::is_integer<T>())
	constexpr T from_base_string(const std::string_view& string, T base, base_format base_format = base_format::base36l) {
		bool negative = false;
		if (string.front() == '-') {
			negative = true;
		}

		qpl::i32 stop = negative;
		if (string.length() >= 2 && string.substr(negative, 2) == "0x") {
			stop += 2;
		}
		T result = T{};
		T mul = T{ 1 };
		for (qpl::i32 i = string.length() - 1; i >= stop; --i) {
			switch (base_format) {
			case base_format::base36l:
			case base_format::base36u:
				result += static_cast<T>(qpl::detail::base_36_inv[string[i]]) * mul;
				break;
			case base_format::base64:
				result += static_cast<T>(qpl::detail::base_64_inv[string[i]]) * mul;
				break;
			}
			mul *= base;
		}
		if constexpr (qpl::is_signed<T>()) {
			if (negative) {
				result = -result;
			}
		}
		return result;
	}


	template<typename T> requires (qpl::is_integer<T>())
	std::string hex_string(T value, const std::string& prefix = "0x", base_format base_format = base_format::base36l, bool prepend_zeroes = false) {
		return qpl::base_string(value, T{ 16 }, prefix, base_format, prepend_zeroes);
	}
	template<typename T> requires (qpl::is_integer<T>())
	std::string hex_string_full(T value, const std::string& prefix = "0x", base_format base_format = base_format::base36l) {
		return qpl::base_string(value, T{ 16 }, prefix, base_format, true);
	}
	template<typename T> requires (qpl::is_integer<T>())
	std::string binary_string(T value, base_format base_format = base_format::base36l, bool prepend_zeroes = false) {
		return qpl::base_string(value, T{ 2 }, "", base_format, prepend_zeroes);
	}
	template<typename T> requires (qpl::is_integer<T>())
	std::string binary_string_full(T value, base_format base_format = base_format::base36l) {
		return qpl::base_string(value, T{ 2 }, "", base_format, true);
	}

	template<typename C> requires (qpl::is_container<C>())
	inline std::string container_to_hex_string(const C& data, std::string_view delimiter = ", ", std::string_view brackets = "{}") {
		std::ostringstream str;
		str << brackets.front();
		bool start = true;
		for (const auto& e : data) {
			if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
				if (!start) {
					str << delimiter;
				}
				str << container_to_string(e);
				start = false;
			}
			else {
				if (!start) {
					str << delimiter;
				}
				str << qpl::hex_string_full(e);
				if constexpr (qpl::is_unsigned<decltype(e)>()) {
					str << 'u';
				}
				start = false;
			}
		}
		str << brackets.back();
		return str.str();
	}

	template<typename C> requires (qpl::is_container<C>())
	inline void println_container_hex(const C& data) {
		qpl::println(qpl::container_to_hex_string(data));
	}

	enum class strform_type {
		fixed, dynamic, fill, newline
	};
	enum class strform_orientation {
		left, middle, right
	};
	struct strform_content {
		std::string str;
		strform_orientation orientation = strform_orientation::left;
		strform_type type = strform_type::dynamic;
	};

	template<typename T>
	strform_content strform_fill(T&& value, strform_orientation orientation = strform_orientation::left) {
		strform_content result;
		result.str = qpl::to_string(value);
		result.orientation = orientation;
		result.type = strform_type::fill;
		return result;
	}
	template<typename T>
	strform_content strform_dynamic(T&& value, strform_orientation orientation = strform_orientation::left) {
		strform_content result;
		result.str = qpl::to_string(value);
		result.orientation = orientation;
		result.type = strform_type::dynamic;
		return result;
	}
	template<typename T>
	strform_content strform_fixed(T&& value, strform_orientation orientation = strform_orientation::left) {
		strform_content result;
		result.str = qpl::to_string(value);
		result.orientation = orientation;
		result.type = strform_type::fixed;
		return result;
	}
	template<typename T>
	strform_content strform_left(T&& value, strform_type type = strform_type::dynamic) {
		strform_content result;
		result.str = qpl::to_string(value);
		result.orientation = strform_orientation::left;
		result.type = type;
		return result;
	}
	template<typename T>
	strform_content strform_middle(T&& value, strform_type type = strform_type::dynamic) {
		strform_content result;
		result.str = qpl::to_string(value);
		result.orientation = strform_orientation::middle;
		result.type = type;
		return result;
	}

	template<typename T>
	strform_content strform_right(T&& value, strform_type type = strform_type::dynamic) {
		strform_content result;
		result.str = qpl::to_string(value);
		result.orientation = strform_orientation::right;
		result.type = type;
		return result;
	}
	QPLDLL strform_content strform_endl();

	class strform_builder {
	public:
		template<typename T>
		void add_entry(T&& info) {
			strform_content entry;
			entry.str = qpl::to_string(info);
			entry.type = strform_type::dynamic;
			entry.orientation = strform_orientation::left;
			this->m_lines.emplace_back(entry);
		}
		QPLDLL void complete_line();
		QPLDLL void add_entry(strform_content&& content);
		QPLDLL void add_fill_whitespace();
		QPLDLL std::string string() const;
		QPLDLL qpl::size entries_per_line() const;
		QPLDLL qpl::size length_of_entry(qpl::size n) const;
	private:

		std::vector<strform_content> m_lines;
	};



	struct wstrform_content {
		std::wstring str;
		strform_orientation orientation = strform_orientation::left;
		strform_type type = strform_type::dynamic;
	};

	template<typename T>
	wstrform_content wstrform_fill(T&& value, strform_orientation orientation = strform_orientation::left) {
		wstrform_content result;
		result.str = qpl::to_wstring(value);
		result.orientation = orientation;
		result.type = strform_type::fill;
		return result;
	}
	template<typename T>
	wstrform_content wstrform_dynamic(T&& value, strform_orientation orientation = strform_orientation::left) {
		wstrform_content result;
		result.str = qpl::to_wstring(value);
		result.orientation = orientation;
		result.type = strform_type::dynamic;
		return result;
	}
	template<typename T>
	wstrform_content wstrform_fixed(T&& value, strform_orientation orientation = strform_orientation::left) {
		wstrform_content result;
		result.str = qpl::to_wstring(value);
		result.orientation = orientation;
		result.type = strform_type::fixed;
		return result;
	}
	template<typename T>
	wstrform_content wstrform_left(T&& value, strform_type type = strform_type::dynamic) {
		wstrform_content result;
		result.str = qpl::to_wstring(value);
		result.orientation = strform_orientation::left;
		result.type = type;
		return result;
	}
	template<typename T>
	wstrform_content wstrform_middle(T&& value, strform_type type = strform_type::dynamic) {
		wstrform_content result;
		result.str = qpl::to_wstring(value);
		result.orientation = strform_orientation::middle;
		result.type = type;
		return result;
	}

	template<typename T>
	wstrform_content wstrform_right(T&& value, strform_type type = strform_type::dynamic) {
		wstrform_content result;
		result.str = qpl::to_wstring(value);
		result.orientation = strform_orientation::right;
		result.type = type;
		return result;
	}
	QPLDLL wstrform_content wstrform_endl();

	class wstrform_builder {
	public:
		template<typename T>
		void add_entry(T&& info) {
			wstrform_content entry;
			entry.str = qpl::to_wstring(info);
			entry.type = strform_type::dynamic;
			entry.orientation = strform_orientation::left;
			this->m_lines.emplace_back(entry);
		}
		QPLDLL void complete_line();
		QPLDLL void add_entry(wstrform_content&& content);
		QPLDLL void add_fill_whitespace();
		QPLDLL std::wstring string() const;
		QPLDLL qpl::size entries_per_line() const;
		QPLDLL qpl::size length_of_entry(qpl::size n) const;
	private:

		std::vector<wstrform_content> m_lines;
	};

	QPLDLL qpl::char_type random_visible_char();
	QPLDLL std::string random_string(qpl::size length);
	QPLDLL std::string random_number_string(qpl::size length);
	QPLDLL std::string random_lowercase_uppercase_number_string(qpl::size length);
	QPLDLL std::string random_lowercase_number_string(qpl::size length);
	QPLDLL std::string random_uppercase_number_string(qpl::size length);
	QPLDLL std::string random_lowercase_uppercase_string(qpl::size length);
	QPLDLL std::string random_lowercase_string(qpl::size length);
	QPLDLL std::string random_uppercase_string(qpl::size length);
	QPLDLL std::string random_string_full_range(qpl::size length);
	QPLDLL std::wstring random_wstring_full_range(qpl::size length);

	QPLDLL std::string to_lower(const std::string& string);
	QPLDLL std::string to_upper(const std::string& string);

	QPLDLL std::vector<std::string> split_string(const std::string& string, char by_what);
	QPLDLL std::vector<std::string> split_string(const std::string& string, const std::string& expression);
	QPLDLL std::vector<std::string> split_string(const std::string& string);
	QPLDLL std::vector<std::wstring> split_string(const std::wstring& string, char by_what);
	QPLDLL std::vector<std::wstring> split_string(const std::wstring& string, const std::wstring& expression);
	QPLDLL std::vector<std::wstring> split_string(const std::wstring& string);


	template<typename T>
	T string_cast(const std::string_view& string) {
		T value;
		std::from_chars(string.data(), string.data() + string.size(), value);

		return value;
	}
	template<typename T>
	T string_cast(const std::string& string) {
		if constexpr (std::is_same_v<T, qpl::f64>) {
			return std::stod(string);
		}
		else if constexpr (std::is_same_v<T, qpl::f32>) {
			return std::stof(string);
		}
		else if constexpr (std::is_same_v<T, qpl::i8>) {
			return static_cast<qpl::i8>(std::stoi(string));
		}
		else if constexpr (std::is_same_v<T, qpl::u8>) {
			return static_cast<qpl::u8>(std::stoul(string));
		}
		else if constexpr (std::is_same_v<T, qpl::i16>) {
			return static_cast<qpl::i16>(std::stoi(string));
		}
		else if constexpr (std::is_same_v<T, qpl::u16>) {
			return static_cast<qpl::u16>(std::stoul(string));
		}
		else if constexpr (std::is_same_v<T, qpl::i32>) {
			return std::stoi(string);
		}
		else if constexpr (std::is_same_v<T, qpl::u32>) {
			return static_cast<qpl::u32>(std::stoul(string));
		}
		else if constexpr (std::is_same_v<T, qpl::i64>) {
			return std::stoll(string);
		}
		else if constexpr (std::is_same_v<T, qpl::u64>) {
			return std::stoull(string);
		}
	}

	template<typename T>
	std::vector<T> split_numbers(const std::string& string) {
		std::vector<T> result;

		if constexpr (qpl::is_floating_point<T>()) {
			static std::regex reg{ "[0-9.e+]+" };
			auto s = std::sregex_iterator(string.cbegin(), string.cend(), reg);
			while (s != std::sregex_iterator()) {
				result.push_back(qpl::string_cast<T>(s->str()));
				++s;
			}
		}
		else if constexpr (qpl::is_integer<T>()) {
			static std::regex reg{ "[0-9]+" };
			auto s = std::sregex_iterator(string.cbegin(), string.cend(), reg);
			while (s != std::sregex_iterator()) {
				result.push_back(qpl::string_cast<T>(s->str()));
				++s;
			}
		}
		else {
			static_assert("split_numbers<T>: T is not arithmetic");
		}
		return result;
	}


	QPLDLL std::string string_first_n_characters(const std::string& string, qpl::size n);
	constexpr std::string_view string_first_n_characters(const std::string_view& string, qpl::size n) {
		return string.substr(0, n);
	}
	constexpr std::string_view string_first_n_characters(const char* string, qpl::size n) {
		return qpl::string_first_n_characters(std::string_view(string), n);
	}
	QPLDLL std::string string_last_n_characters(const std::string& string, qpl::size n);
	constexpr std::string_view string_last_n_characters(const std::string_view& string, qpl::size n) {
		return string.substr(string.length() - n);
	}
	constexpr std::string_view string_last_n_characters(const char* string, qpl::size n) {
		return qpl::string_last_n_characters(std::string_view(string), n);
	}

	QPLDLL std::string string_seperation(const std::string& string, const std::string& seperation, qpl::size n, bool left = true);
	QPLDLL std::string string_seperation(const std::string& string, char seperation, qpl::size n, bool left = true);

	//sequence : 0-9, a-z, A-Z, 0-9a-z
	constexpr bool string_contains_exclusively(std::string_view string, std::string_view sequence) {
		if (sequence.length() >= 3 && sequence[1] == '-') {
			for (auto& c : string) {
				for (qpl::u32 i = 0; i < sequence.length() / 3; ++i) {
					bool found = false;
					if (c >= sequence[i * 3 + 0] && c <= sequence[i * 3 + 2]) {
						found = true;
					}
					if (!found) {
						return false;
					}
				}
			}
		}
		else {
			for (auto& c : string) {
				bool found = false;

				for (auto &i : sequence) {
					if (c == i) {
						found = true;
					}
				}
				if (!found) {
					return false;
				}
			}
		}
		return true;
	}
	constexpr bool is_scientific_notation(const std::string_view& string) {
		for (qpl::u32 i = 0u; i < string.length() - 1; ++i) {
			auto substr = string.substr(i, 2);
			if (substr == "e+" || substr == "e-") {
				return true;
			}
		}
		return false;
	}



	enum class operator_type {
		plus,
		minus,
		mul,
		div,
		mod,
		pow,
		less,
		less_equal,
		greater,
		greater_equal,
		equal,
		not_equal,
		left_shift,
		right_shift,
		bitwise_or,
		bitwise_and,
		bitwise_xor,
		logic_or,
		logic_and,
		none
	};

	constexpr bool is_character_operator(char c) {
		return qpl::detail::operator_table[c];
	}
	constexpr bool is_character_unary_left_operator(char c) {
		return qpl::detail::unary_left_operator_table[c];
	}
	constexpr bool is_character_unary_right_operator(char c) {
		return qpl::detail::unary_right_operator_table[c];
	}
	constexpr bool is_character_white_space(char c) {
		return qpl::detail::white_space_table[c];
	}
	constexpr bool is_character_parantheses(char c) {
		return qpl::detail::parantheses_table[c];
	}
	constexpr bool is_character_arithmetic_special(char c) {
		return qpl::detail::arithmetic_special_table[c];
	}
	QPLDLL bool is_string_floating_point(std::string string);
	QPLDLL bool is_string_number(std::string string);


	constexpr operator_type get_operator_type(const std::string_view& expression) {
		
		if (expression == "+") return qpl::operator_type::plus;
		else if (expression == "-") return qpl::operator_type::minus;
		else if (expression == "*") return qpl::operator_type::mul;
		else if (expression == "/") return qpl::operator_type::div;
		else if (expression == "%") return qpl::operator_type::mod;
		else if (expression == "**") return qpl::operator_type::pow;
		else if (expression == "<") return qpl::operator_type::less;
		else if (expression == "<=") return qpl::operator_type::less_equal;
		else if (expression == ">") return qpl::operator_type::greater;
		else if (expression == ">=") return qpl::operator_type::greater_equal;
		else if (expression == "==") return qpl::operator_type::equal;
		else if (expression == "!=") return qpl::operator_type::not_equal;
		else if (expression == "<<") return qpl::operator_type::left_shift;
		else if (expression == ">>") return qpl::operator_type::right_shift;
		else if (expression == "|") return qpl::operator_type::bitwise_or;
		else if (expression == "&") return qpl::operator_type::bitwise_and;
		else if (expression == "^") return qpl::operator_type::bitwise_xor;
		else if (expression == "||") return qpl::operator_type::logic_or;
		else if (expression == "&&") return qpl::operator_type::logic_and;
		else return qpl::operator_type::none;
	}

	template<typename T>
	constexpr T parse_single_expression(const std::string_view& expression) {
		//(3 + (5 * 3))

		qpl::u32 a1 = 0u;
		while (qpl::is_character_white_space(expression[a1])) { ++a1; }


		auto a_unary_left = a1;

		while (qpl::is_character_unary_left_operator(expression[a1])) { ++a1; }

		auto a2 = a1;
		while (!qpl::is_character_arithmetic_special(expression[a2])){ ++a2; }



		auto op1_begin = a2;
		auto a_unary_right = op1_begin;


		while (qpl::is_character_white_space(expression[a2 - 1])) { --a2; };

		if (a2 != op1_begin) {
			a_unary_right = a2;
		}
		else {
			while (is_character_unary_right_operator(expression[a_unary_right])) { ++a_unary_right; }
		}


		auto op1 = a_unary_right;

		while (qpl::is_character_white_space(expression[op1])) { ++op1; }

		if (op1 == a_unary_right) {
			op1 = a_unary_right = op1_begin;
		}

		auto op2 = op1;

		while (qpl::is_character_operator(expression[op2])) { ++op2; }

		if (op2 == op1) {
			op1 = op1_begin;
			op2 = a_unary_right;
			a_unary_right = a2;
		}

		auto b_unary_left = op2;
		while (qpl::is_character_white_space(expression[b_unary_left])) { ++b_unary_left; }

		auto b1 = b_unary_left;
		while (qpl::is_character_unary_left_operator(expression[b1])) { ++b1; }

		auto b2 = b1;
		while (!qpl::is_character_arithmetic_special(expression[b2]) && b2 < expression.length()) { ++b2; }


		auto b_unary_right = b2;
		while (qpl::is_character_unary_right_operator(expression[b_unary_right])) { ++b_unary_right; }

		T a = expression.substr(a1, a2 - a1);
		T b = expression.substr(b1, b2 - b1);
		T c;

		auto a_unary_left_str = expression.substr(a_unary_left, a1 - a_unary_left);
		auto a_unary_right_str = expression.substr(a2, a_unary_right - a2);

		for (auto& i : a_unary_right_str) {
			if (i == '!') {
				a = qpl::factorial(a);
			}
		}
		for (auto& i : a_unary_left_str) {
			if (i == '-') {
				a = -a;
			}
			else if (i == '+') {
				a = +a;
			}
			else if (i == '~') {
				a = ~a;
			}
			else if (i == '!') {
				a = !a;
			}
		}

		auto b_unary_left_str = expression.substr(b_unary_left, b1 - b_unary_left);
		auto b_unary_right_str = expression.substr(b2, b_unary_right - b2);

		for (auto& i : b_unary_right_str) {
			if (i == '!') {
				b = qpl::factorial(b);
			}
		}
		for (auto& i : b_unary_left_str) {
			if (i == '-') {
				b = -b;
			}
			else if (i == '+') {
				b = +b;
			}
			else if (i == '~') {
				b = ~b;
			}
			else if (i == '!') {
				b = !b;
			}
		}
		auto op_str = expression.substr(op1, op2 - op1);

		auto op = get_operator_type(op_str);
		switch (op) {
		case qpl::operator_type::plus: c = a + b; break;
			case qpl::operator_type::minus:  c = a - b; break;
			case qpl::operator_type::mul:  c = a * b; break;
			case qpl::operator_type::div:  c = a / b; break;
			case qpl::operator_type::mod:  c = a % b; break;
			case qpl::operator_type::pow:  c = qpl::pow<T>(a, b); break;
			case qpl::operator_type::less:  c = a < b; break;
			case qpl::operator_type::less_equal:  c = a <= b; break;
			case qpl::operator_type::greater:  c = a > b; break;
			case qpl::operator_type::greater_equal:  c = a >= b; break;
			case qpl::operator_type::equal:  c = a == b; break;
			case qpl::operator_type::not_equal:  c = a != b; break;
			case qpl::operator_type::left_shift:  c = a << b; break;
			case qpl::operator_type::right_shift:  c = a >> b; break;
			case qpl::operator_type::bitwise_or:  c = a | b; break;
			case qpl::operator_type::bitwise_and:  c = a & b; break;
			case qpl::operator_type::bitwise_xor:  c = a ^ b; break;
			case qpl::operator_type::logic_or:  c = a || b; break;
			case qpl::operator_type::logic_and:  c = a && b; break;
			case qpl::operator_type::none: break;
		}


		return c;
	}
	template<typename T>
	constexpr T parse_expression(const std::string_view& expression) {
		qpl::u32 start = 0u;
		while (start < expression.length() && qpl::is_character_white_space(expression[start])) { ++start; }

		qpl::u32 special = start;
		while (special < expression.length() && !qpl::is_character_arithmetic_special(expression[special])) { ++special; }

		bool single_variable = (special == expression.length());

		while (special >= 1 && qpl::is_character_white_space(expression[special - 1])) { --special; }

		if (single_variable) {
			return T{ expression.substr(start, special - start) };
		}
		else {
			return qpl::parse_single_expression<T>(expression.substr(start));
		}

		//(3 + (5 * 3))
		return 0;
	}
	template<typename T>
	T parse_formular(const std::string& formular, const std::vector<T>& values) {
		//( +( * ))


	}


	template<typename T> requires (qpl::is_arithmetic<T>())
	std::string big_number_string(T number, qpl::u32 precision = 2) {
		if (number == T{}) {
			return qpl::to_string(number);
		}
		if constexpr (qpl::is_signed<T>()) {
			if (number < 0) {
				return "-" + qpl::big_number_string(-number, precision);
			}
		}

		qpl::i64 log;
		if constexpr (qpl::is_qpl_integer<T>()) {
			log = (number.digits() - 1) / 3.0;
		}
		else {
			log = static_cast<qpl::i64>(std::log(number) / std::log(1000.0));
		}
		std::string name;
		bool negative = false;


		if (log < 0) {
			log *= -1;
			negative = true;
		}

		const static std::array<std::string, 10> p_ones = { "", "un", "dou", "tre", "quat", "quin", "sex", "sept", "oct", "non" };
		const static std::array<std::string, 10> p_tens = { "dec", "vig", "trig", "quadrag", "quinquag", "sexag", "septuag", "octog", "nonag" };

		if (log >= 1 && log <= 100) {
			switch (log) {
			case 1: name = "k"; break;
			case 2: name = "mln"; break;
			case 3: name = "bln"; break;
			case 4: name = "trn"; break;
			case 5: name = "quad"; break;
			case 6: name = "quint"; break;
			case 7: name = "sext"; break;
			case 8: name = "sept"; break;
			case 9: name = "oct"; break;
			case 10: name = "non"; break;
			default: name = qpl::to_string(p_ones[(log - 1) % 10], p_tens[((log - 1) / 10) - 1]);
			}
		}
		if constexpr (qpl::is_integer<T>()) {
			if (number < 1000) {
				return qpl::to_string(number);
			}
		}
		if (log == 0) {
			return qpl::to_string(qpl::to_string_precision(precision, number), name);
		}
		if (negative) {
			return qpl::to_string(qpl::to_string_precision(precision, number * std::pow(1000, log)), " 1/", name);
		}
		else {
			return qpl::to_string(qpl::to_string_precision(precision, number / std::pow(1000, log)), name);
		}
	}


	template<typename T> requires (qpl::is_arithmetic<T>())
	std::string big_number_string_short(T number, qpl::u32 precision = 1) {
		if (number == T{}) {
			return qpl::to_string(number);
		}
		if constexpr (qpl::is_signed<T>()) {
			if (number < 0) {
				return "-" + qpl::big_number_string(-number, precision);
			}
		}
		qpl::i64 log;
		if constexpr (qpl::is_qpl_integer<T>()) {
			log = (number.digits() - 1) / 3.0;
		}
		else {
			log = static_cast<qpl::i64>(std::log(number) / std::log(1000.0));
		}
		std::string name;
		bool negative = false;


		if (log < 0) {
			log *= -1;
			negative = true;
		}

		const static std::array<std::string, 10> p_ones = { "", "Un", "Dou", "Tre", "Qa", "Qi", "Sx", "Sp", "Oc", "No" };
		const static std::array<std::string, 10> p_tens = { "Dec", "Vig", "Tri", "Qa", "Qi", "Sx", "Sp", "Oc", "No" };

		if (log >= 1 && log <= 100) {
			switch (log) {
			case 1: name = "K"; break;
			case 2: name = "M"; break;
			case 3: name = "B"; break;
			case 4: name = "T"; break;
			case 5: name = "Qa"; break;
			case 6: name = "Qi"; break;
			case 7: name = "Sx"; break;
			case 8: name = "Sp"; break;
			case 9: name = "Oc"; break;
			case 10: name = "No"; break;
			default: name = qpl::to_string(p_ones[(log - 1) % 10], p_tens[((log - 1) / 10) - 1]);
			}
		}
		if constexpr (qpl::is_integer<T>()) {
			if (number < 1000) {
				return qpl::to_string(number);
			}
		}
		if (log == 0) {
			return qpl::to_string(qpl::to_string_precision(precision, number), name);
		}
		if (negative) {
			qpl::f64 pow;
			if constexpr (qpl::is_qpl_integer<T>()) {
				T x = 1000;
				pow = x.pow(log);
			}
			else {
				pow = std::pow(1000, log);
			}
			return qpl::to_string(qpl::to_string_precision(precision, number * pow), " 1/", name);
		}
		else {
			qpl::f64 pow;
			if constexpr (qpl::is_qpl_integer<T>()) {
				T x = 1000;
				pow = x.pow(log);
			}
			else {
				pow = std::pow(1000, log);
			}
			
			return qpl::to_string(qpl::to_string_precision(precision, number / pow), name);
		}
	}


	template<typename T> requires (qpl::is_arithmetic<T>())
	std::string percentage_plus_string(T number) {
		return qpl::to_string(number >= 0 ? "+" : "", qpl::big_number_string(number * 100), '%');
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	std::string percentage_string(T number) {
		return qpl::to_string(qpl::big_number_string(number * 100), '%');
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	std::string percentage_string_precision(T number, qpl::u32 precision) {
		return qpl::to_string(qpl::big_number_string(number * 100, precision), '%');
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	std::string plus_percentage_string_precision(T number, qpl::u32 precision) {
		return qpl::to_string(number >= 0 ? "+" : "", qpl::big_number_string(number * 100, precision), '%');
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	std::string to_radians_string(T number) {
		return qpl::to_string((number / qpl::pi) * 180);
	}

}

#endif