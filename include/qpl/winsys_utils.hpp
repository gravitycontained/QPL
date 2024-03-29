﻿#pragma once
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
			qpl::background background = qpl::background::transparent;
			colored_text_get_position_type get_position;

			QPLDLL bool is_default_colors() const;
		};
		std::vector<element> elements;
		QPLDLL void add(const element& element);


		template<typename T>
		void create_from_styled_string(const qpl::styled_string<T>& string) {
			this->clear();
			for (auto& i : string.elements) {

				qpl::colored_string::element element;
				element.text = qpl::to_basic_string<wchar_t>(i.text);
				element.foreground = qpl::rgb_to_foreground(i.color);
				element.get_position = i.get_position;
				this->add(element);
			}
		}
		void reset_get_position_search() {
			for (auto& i : this->elements) {
				if (!i.get_position.name.empty()) {
					i.get_position.done = false;
				}
			}
		}

		template<typename T>
		auto as_styled_string() const {
			qpl::styled_string<T> result;

			for (auto& i : this->elements) {
				result.add(i.foreground);
				result.add(i.background);
				result.add(i.text);
				result.add(i.get_position);
			}
			return result;
		}

		template<typename T>
		colored_string& operator<<(T value) {
			if (this->elements.empty()) {
				this->elements.push_back({});
			}
			if constexpr (qpl::is_same<std::decay_t<T>, qpl::foreground>()) {
				bool change = (value != this->elements.back().foreground || !this->elements.back().get_position.name.empty());
				if (change) {
					if (!this->elements.back().text.empty()) {
						this->elements.push_back({});
					}
					this->elements.back().foreground = value;
				}
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, qpl::colored_text_get_position_type>()) {
				this->elements.push_back({});
				this->elements.back().get_position = value;
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, qpl::background>()) {
				bool change = (value != this->elements.back().foreground || !this->elements.back().get_position.name.empty());
				if (change) {
					if (!this->elements.back().text.empty()) {
						this->elements.push_back({});
					}
					this->elements.back().background = value;
				}
			}
			else if constexpr (qpl::is_same<std::decay_t<T>, qpl::cc>()) {
				bool change = (value.foreground != this->elements.back().foreground) || (value.background != this->elements.back().background || !this->elements.back().get_position.name.empty());
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
			else if constexpr (qpl::is_container<std::decay_t<T>>() && !qpl::is_string_type<T>()) {
				bool first = true;
				for (auto& i : value) {
					if (!first) {
						this->elements.back().text += L", ";
					}
					first = false;
					this->elements.back().text += qpl::to_wstring(i);
				}
				if (!this->elements.back().is_default_colors()) {
					this->elements.push_back({});
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

		QPLDLL void clear();
		QPLDLL void clear_retain_last_color();
		QPLDLL bool empty() const;
		QPLDLL void print() const;
		QPLDLL void println() const;
		QPLDLL qpl::size count_new_line_size() const;
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

	QPLDLL void print_box_around(const std::wstring& string, qpl::foreground text_color = qpl::foreground::white, qpl::size left_offset = 0u, qpl::vec2s margin = { 1, 0 }, qpl::vec2s wall_width = { 1, 1 }, qpl::foreground box_color = qpl::foreground::gray, std::wstring border_blocks = L"▄█▀");
	QPLDLL void print_box_around(const qpl::colored_string& string, qpl::size left_offset = 0u, qpl::vec2s margin = { 1, 0 }, qpl::vec2s wall_width = { 1, 1 }, qpl::foreground color = qpl::foreground::gray, std::wstring border_blocks = L"▄█▀");
}

#endif
#endif