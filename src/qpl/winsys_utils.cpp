#pragma once

#include <qpl/winsys_utils.hpp>

#ifdef _WIN32
namespace qpl {
	bool qpl::colored_string::element::is_default_colors() const {
		return this->foreground == qpl::foreground::white && this->background == qpl::background::black;
	}
	void qpl::colored_string::add(const qpl::colored_string::element& element) {
		if (this->empty()) {
			this->elements.push_back(element);
		}
		else if (this->elements.back().foreground == element.foreground && this->elements.back().background == element.background) {
			this->elements.back().text += element.text;
		}
		else {
			this->elements.push_back(element);
		}
	}

	void qpl::colored_string::clear() {
		this->elements.clear();
	}
	void qpl::colored_string::clear_retain_last_color() {
		auto last = this->elements.back();
		this->elements.resize(1u);
		this->elements.back() = last;
		this->elements.back().text.clear();
	}
	bool qpl::colored_string::empty() const {
		return this->elements.empty();
	}
	void qpl::colored_string::print() const {
		qpl::cc cc;

		qpl::print(qpl::cc{});
		for (auto& i : this->elements) {
			bool set = false;
			if (cc.foreground != i.foreground) {
				cc.foreground = i.foreground;
				set = true;
			}
			if (cc.background != i.background) {
				cc.background = i.background;
				set = true;
			}
			if (set) {
				qpl::print(cc);
			}
			qpl::print(i.text);
		}
		qpl::print(qpl::cc{});
	}

	void qpl::colored_string::println() const {
		this->print();
		qpl::println();
	}
	std::string qpl::colored_string::string() const {
		return qpl::wstring_to_string(this->wstring());
	}
	std::wstring qpl::colored_string::wstring() const {
		std::wstring result = L"";
		for (auto& i : this->elements) {
			result += i.text;
		}
		return result;
	}
	std::vector<colored_string> qpl::colored_string::get_lines() const {
		std::vector<colored_string> result;
		result.push_back({});


		for (qpl::size index = 0u; index < this->elements.size(); ++index) {
			const auto& element = this->elements[index];
			result.back().elements.push_back(element);
			qpl::size before = 0u;
			bool found_any = false;
			for (qpl::size i = 0u; i < element.text.length(); ++i) {
				if (element.text[i] == L'\n') {
					if (i - before) {
						result.back().elements.back().text = element.text.substr(before, i - before);
					}
					else {
						result.back().elements.back().text.clear();
					}
					auto last = result.back().elements.back();
					if (index != this->elements.size() - 1) {
						result.push_back({});
						result.back().elements.push_back(last);
						result.back().elements.back().text = L"";
					}
					before = i + 1;
					found_any = true;
				}
			}
			if (found_any) {
				if (before < element.text.length()) {
					result.back().elements.back().text = element.text.substr(before);
				}
			}
		}
		return result;
	}

	void qpl::print_box_around(const std::wstring& string, qpl::foreground text_color, qpl::size left_offset, qpl::vec2s margin, qpl::vec2s wall_width, qpl::foreground box_color, std::wstring border_blocks) {
		auto lines = qpl::string_split(string, L'\n');
		qpl::size max = 0u;
		for (auto& i : lines) {
			//if (max < i.length()) {
			//	max = i.length();
			//}
			if (max < qpl::unicode_string_length(i)) {
				max = qpl::unicode_string_length(i);
			}
		}
		auto offset = qpl::to_wstring_repeat(L" ", left_offset);
		auto xspace = qpl::to_wstring_repeat(L" ", margin.x);
		auto wall = qpl::to_wstring_repeat(border_blocks[1], wall_width.x);


		for (qpl::size i = 0u; i < wall_width.y - 1; ++i) {
			auto w = max + margin.x * 2;
			qpl::println(offset, box_color, qpl::to_wstring(qpl::to_wstring_repeat(i ? border_blocks[1] : border_blocks[0], w + wall_width.x * 2)));
		}
		qpl::println(offset, box_color, qpl::to_wstring(wall, qpl::to_wstring_repeat(border_blocks[2], max + margin.x * 2), wall));

		for (qpl::size i = 0u; i < margin.y; ++i) {
			qpl::print(offset, box_color, wall);
			qpl::print(qpl::to_string_repeat(" ", max + margin.x * 2));
			qpl::println(box_color, wall);
		}
		for (auto& line : lines) {
			qpl::print(offset, box_color, wall);
			qpl::print(xspace, text_color, line, xspace);
			//qpl::print(qpl::to_string_repeat(" ", max - line.length()));
			qpl::print(qpl::to_string_repeat(" ", max - qpl::unicode_string_length(line)));
			qpl::println(box_color, wall);
		}
		for (qpl::size i = 0u; i < margin.y; ++i) {
			qpl::print(offset, box_color, wall);
			qpl::print(qpl::to_string_repeat(" ", max + margin.x * 2));
			qpl::println(box_color, wall);
		}
		qpl::println(offset, box_color, qpl::to_wstring(wall, qpl::to_wstring_repeat(border_blocks[0], max + margin.x * 2), wall));
		for (qpl::size i = 1u; i < wall_width.y; ++i) {
			auto index = wall_width.y - i - 1;
			auto w = max + margin.x * 2;
			qpl::println(offset, box_color, qpl::to_wstring(qpl::to_wstring_repeat(index ? border_blocks[1] : border_blocks[2], w + wall_width.x * 2)));
		}
	}
	void qpl::print_box_around(const qpl::colored_string& string, qpl::size left_offset, qpl::vec2s margin, qpl::vec2s wall_width, qpl::foreground color, std::wstring border_blocks) {
		auto lines = string.get_lines();
		qpl::size max = 0u;
		for (auto& i : lines) {
			if (max < qpl::unicode_string_length(i.wstring())) {
				max = qpl::unicode_string_length(i.wstring());
			}
		}
		auto offset = qpl::to_wstring_repeat(L" ", left_offset);
		auto xspace = qpl::to_wstring_repeat(L" ", margin.x);
		auto wall = qpl::to_wstring_repeat(border_blocks[1], wall_width.x);


		for (qpl::size i = 0u; i < wall_width.y - 1; ++i) {
			auto w = max + margin.x * 2;
			qpl::println(offset, color, qpl::to_wstring(qpl::to_wstring_repeat(i ? border_blocks[1] : border_blocks[0], w + wall_width.x * 2)));
		}
		qpl::println(offset, color, qpl::to_wstring(wall, qpl::to_wstring_repeat(border_blocks[2], max + margin.x * 2), wall));

		for (qpl::size i = 0u; i < margin.y; ++i) {
			qpl::print(offset, color, wall);
			qpl::print(qpl::to_string_repeat(" ", max + margin.x * 2));
			qpl::println(color, wall);
		}
		for (auto& line : lines) {
			qpl::print(offset, color, wall);
			qpl::print(xspace);
			line.print();
			qpl::print(qpl::to_string_repeat(" ", max - qpl::unicode_string_length(line.wstring())));
			qpl::print(xspace);
			qpl::println(color, wall);
		}
		for (qpl::size i = 0u; i < margin.y; ++i) {
			qpl::print(offset, color, wall);
			qpl::print(qpl::to_string_repeat(" ", max + margin.x * 2));
			qpl::println(color, wall);
		}
		qpl::println(offset, color, qpl::to_wstring(wall, qpl::to_wstring_repeat(border_blocks[0], max + margin.x * 2), wall));
		for (qpl::size i = 1u; i < wall_width.y; ++i) {
			auto index = wall_width.y - i - 1;
			auto w = max + margin.x * 2;
			qpl::println(offset, color, qpl::to_wstring(qpl::to_wstring_repeat(index ? border_blocks[1]: border_blocks[2], w + wall_width.x * 2)));
		}
	}
}
#endif