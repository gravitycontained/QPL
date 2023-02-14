#pragma once
#ifndef QPL_WINSYS_UTILS_HPP
#define QPL_WINSYS_UTILS_HPP
#ifdef _WIN32

#include <qpl/winsys.hpp>
#include <qpl/string.hpp>
#include <qpl/vector.hpp>

namespace qpl {
	struct colored_string {
		struct element {
			std::wstring text = L"";
			qpl::foreground foreground = qpl::foreground::white;
			qpl::background background = qpl::background::black;

			QPLDLL bool is_default_colors() const;
		};
		std::vector<element> elements;
		QPLDLL void add(const element& element);

		template<typename T>
		colored_string& operator<<(T value) {
			if (this->elements.empty()) {
				this->elements.push_back({});
			}
			if constexpr (qpl::is_same<std::decay_t<T>, qpl::foreground>()) {
				bool change = (value != this->elements.back().foreground);
				if (change) {
					if (!this->elements.back().text.empty()) {
						this->elements.push_back({});
					}
					this->elements.back().foreground = value;
				}
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, qpl::background>()) {
				bool change = (value != this->elements.back().foreground);
				if (change) {
					if (!this->elements.back().text.empty()) {
						this->elements.push_back({});
					}
					this->elements.back().background = value;
					}
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, qpl::cc>()) {
				bool change = (value.foreground != this->elements.back().foreground) || (value.background != this->elements.back().background);
				if (change) {
					if (!this->elements.back().text.empty()) {
						this->elements.push_back({});
					}
					this->elements.back().foreground = value.foreground;
					this->elements.back().background = value.background;
				}
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, colored_string>()) {
				for (auto& i : value.elements) {
					this->add(i);
				}
			}
			else {
				this->elements.back().text += qpl::to_wstring(value);
				if (!this->elements.back().is_default_colors()) {
					this->elements.push_back({});
				}
			}
			return *this;
		}

		QPLDLL bool empty() const;
		QPLDLL void print() const;
		QPLDLL void println() const;
		QPLDLL std::string string() const;
		QPLDLL std::wstring wstring() const;
		QPLDLL std::vector<colored_string> get_lines() const;
	};

	template<typename... Args>
	colored_string to_colored_string(Args... args) {
		colored_string result;
		((result << args), ...);
		return result;
	}
	template<typename... Args>
	colored_string to_colored_stringln(Args... args) {
		return qpl::to_colored_string(args..., L'\n');
	}

	QPLDLL void print_box_around(const std::wstring& string, qpl::foreground text_color = qpl::foreground::white, qpl::size left_offset = 0u, qpl::vec2s margin = { 1, 0 }, qpl::vec2s wall_width = { 1, 1 }, qpl::foreground box_color = qpl::foreground::gray);
	QPLDLL void print_box_around(const qpl::colored_string& string, qpl::size left_offset = 0u, qpl::vec2s margin = { 1, 0 }, qpl::vec2s wall_width = { 1, 1 }, qpl::foreground color = qpl::foreground::gray);
}

#endif
#endif