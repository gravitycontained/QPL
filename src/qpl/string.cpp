#include <qpl/string.hpp>
#include <qpl/random.hpp>

namespace qpl {
	std::string qpl::str_spaced(const std::string& string, qpl::size length, char prepend) {
		return qpl::str_lspaced(string, length, prepend);
	}
	std::wstring qpl::wstr_spaced(const std::wstring& string, qpl::size length, wchar_t prepend) {
		return qpl::wstr_lspaced(string, length, prepend);
	}
	std::string qpl::str_rspaced(const std::string& string, qpl::size length, char prepend) {
		return qpl::prepended_to_string_to_fit(string, prepend, length);
	}
	std::wstring qpl::wstr_rspaced(const std::wstring& string, qpl::size length, wchar_t prepend) {
		return qpl::prepended_to_wstring_to_fit(string, prepend, length);
	}
	std::string qpl::str_lspaced(const std::string& string, qpl::size length, char prepend) {
		return qpl::appended_to_string_to_fit(string, prepend, length);
	}
	std::wstring qpl::wstr_lspaced(const std::wstring& string, qpl::size length, wchar_t prepend) {
		return qpl::appended_to_wstring_to_fit(string, prepend, length);
	}

	bool qpl::string_equals_ignore_case(const std::string& a, const std::string& b) {
		if (a.size() != b.size()) {
			return false;
		}
		for (qpl::u32 i = 0u; i < a.size(); ++i) {
			if (std::tolower(a[i]) != std::tolower(b[i])) {
				return false;
			}
		}
		return true;
	}


	std::wstring qpl::string_to_wstring(const std::string& s) {
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
	std::wstring qpl::string_to_unicode_wstring(const std::string& str) {
		wchar_t* wc = new wchar_t[str.size()];

#pragma warning( push )
#pragma warning( disable : 4996)
		mbstowcs(wc, str.data(), str.size());
#pragma warning( pop ) 
		std::wstring result = wc;
		delete[] wc;
		return result;
	}
	

	std::string qpl::wstring_to_string(const std::wstring& s) {
		int len;
		int slength = (int)s.length() + 1;
		len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
		char* buf = new char[len];
		WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
		std::string r(buf);
		delete[] buf;
		return r;
	}


	QPLDLL std::wostringstream detail::stream_wstr;

	std::string qpl::to_string(const std::string& first) {
		return first;
	}
	std::wstring qpl::to_wstring(const std::wstring& first) {
		return first;
	}
	std::string qpl::bool_string(bool b) {
		return b ? std::string("true") : std::string("false");
	}

	std::string qpl::string_to_fit(const std::string& string, char append, qpl::size length) {
		return std::string(string.length() >= length ? 0ull : length - string.length(), append);
	}
	std::string qpl::appended_to_string_to_fit(const std::string& string, char append, qpl::size length) {
		if (string.length() >= length) {
			return string;
		}
		return string + std::string(length - string.length(), append);
	}
	std::string qpl::appended_to_string_to_fit(const std::string& string, const std::string_view& prepend, qpl::size length) {
		if (string.length() >= length) {
			return string;
		}

		auto multiple = ((string.length() - 1) / prepend.length() + 1) * prepend.length();
		auto left = multiple - string.length();
		auto full = length < multiple ? 0u : ((length - multiple) / prepend.length());
		auto right = length < multiple ? 0u : (length % prepend.length());

		std::ostringstream stream;
		stream << string;
		

		if (left) {
			stream << prepend.substr(prepend.length() - left, length - string.length());
		}
		for (qpl::u32 i = 0u; i < full; ++i) {
			stream << prepend;
		}
		if (right) {
			stream << prepend.substr(0, right);
		}

		return stream.str();
	}


	std::string qpl::prepended_to_string_to_fit(const std::string& string, char prepend, qpl::size length) {
		if (string.length() >= length) {
			return string;
		}
		return std::string(length - string.length(), prepend) + string;
	}
	std::string qpl::prepended_to_string_to_fit(const std::string& string, const std::string_view& prepend, qpl::size length) {
		if (string.length() >= length) {
			return string;
		}

		auto mod = (length - string.length()) % prepend.length();
		auto div = (length - string.length()) / prepend.length();

		std::ostringstream stream;
		for (qpl::u32 i = 0u; i < div; ++i) {
			stream << prepend;
		}
		if (mod) {
			stream << prepend.substr(0, mod);
		}

		stream << string;
		return stream.str();
	}

	std::string qpl::two_strings_fixed_insert(const std::string& a, const std::string& b, const std::string_view& insert, qpl::size length, qpl::u32 rotation) {
		std::ostringstream stream;
		auto size = ((a.length() - 1) / insert.length() + 1) * insert.length() + rotation % insert.length();
		size = qpl::min(size, a.length() + (length - (a.length() + b.length())));

		stream << qpl::appended_to_string_to_fit(a, insert, size);
		stream << qpl::prepended_to_string_to_fit(b, insert, length - size);
		auto str_size = stream.str().length();
		//stream << " sizes = " << size << " ( added " << (qpl::appended_to_string_to_fit(a, insert, size).length() - a.length()) << " characters)" << " - " << length - size << " ( added " << (qpl::prepended_to_string_to_fit(b, insert, length - size).length() - b.length()) << " characters) " << str_size;
		return stream.str();
	}

	template<>
	std::string qpl::console_space(qpl::size n, const std::string& string) {
		return qpl::prepended_to_string_to_fit(string, ' ', n);
	}
	std::wstring qpl::wstring_to_fit(const std::wstring& string, wchar_t append, qpl::size length) {
		return std::wstring(string.length() >= length ? 0ull : length - string.length(), append);
	}
	std::wstring qpl::appended_to_wstring_to_fit(const std::wstring& string, wchar_t append, qpl::size length) {
		if (string.length() >= length) {
			return string;
		}
		return string + std::wstring(length - string.length(), append);
	}
	std::wstring qpl::prepended_to_wstring_to_fit(const std::wstring& string, wchar_t prepend, qpl::size length) {
		if (string.length() >= length) {
			return string;
		}
		return std::wstring(length - string.length(), prepend) + string;
	}
	std::ostream& qpl::operator<<(std::ostream& os, print_effect effect) {
		return os;
	}

	t_cclearln qpl::cclearln;
	t_cclear qpl::cclear;
	namespace detail {
		qpl::cc qpl::detail::current_console_color = qpl::cc::def();
		qpl::u32 qpl::detail::next_print_space = 0u;
		bool qpl::detail::next_println_space = false;
		qpl::u32 qpl::detail::println_space = 0u;
		qpl::u32 qpl::detail::println_default_space = 0u;
		bool qpl::detail::next_print_color = false;
		bool qpl::detail::next_println_color = false;
		qpl::cc qpl::detail::println_color = qpl::cc::def();
		qpl::cc qpl::detail::println_default_color = qpl::cc::def();
	}

	std::string qpl::hex_string(const std::string& string) {
		std::ostringstream stream;
		for (auto& i : string) {
			stream << qpl::prepended_to_string_to_fit(qpl::hex_string(qpl::u8_cast(i), ""), "0", 2);
		}
		return stream.str();
	}
	std::string qpl::binary_string(const std::string& string) {
		std::ostringstream stream;
		for (auto& i : string) {
			stream << qpl::prepended_to_string_to_fit(qpl::binary_string(i), "0", 8);
		}
		return stream.str();
	}
	strform_content qpl::strform_endl() {
		strform_content result;
		result.type = strform_type::newline;
		return result;
	}
	void qpl::strform_builder::add_entry(strform_content&& content) {
		this->m_lines.emplace_back(content);
	}
	void qpl::strform_builder::add_fill_whitespace() {
		this->m_lines.emplace_back(qpl::strform_fill(" "));
	}
	std::string qpl::strform_builder::string() const {
		std::ostringstream stream;
		qpl::size n = qpl::size{};
		auto line_width = this->entries_per_line();
		for (qpl::size i = qpl::size{}; i < this->m_lines.size(); ++i) {
			if (this->m_lines[i].type == qpl::strform_type::newline) {
				stream << '\n';
			}
			else if (this->m_lines[i].type == qpl::strform_type::dynamic) {
					auto entry_length = this->length_of_entry(n);
					auto word_length = this->m_lines[i].str.length();

					if (entry_length == word_length) {
						stream << this->m_lines[i].str;
					}
					else {
						auto diff = qpl::signed_cast(entry_length - word_length);
						if (this->m_lines[i].orientation == qpl::strform_orientation::middle) {
							diff /= 2;
						}
						if (this->m_lines[i].orientation != qpl::strform_orientation::left) {
							if (i && this->m_lines[i - qpl::size{ 1 }].type == qpl::strform_type::fill) {
								auto div = diff / this->m_lines[i - qpl::size{ 1 }].str.length();
								auto mod = diff % this->m_lines[i - qpl::size{ 1 }].str.length();
								auto str = this->m_lines[i - qpl::size{ 1 }].str;
								for (qpl::u32 j = 0u; j < div; ++j) {
									stream << str;
								}
								if (mod) {
									stream << std::string(str.cbegin(), str.cbegin() + mod);
								}
							}
							else {
								stream << std::string(diff, ' ');
							}
						}
						stream << this->m_lines[i].str;
						if (this->m_lines[i].orientation == qpl::strform_orientation::middle) {
							diff = (entry_length - word_length) - diff;
						}
						if (this->m_lines[i].orientation != qpl::strform_orientation::right) {
							if (i != this->m_lines.size() - qpl::size{ 1 } && this->m_lines[i + qpl::size{ 1 }].type == qpl::strform_type::fill) {
								auto div = diff / this->m_lines[i + qpl::size{ 1 }].str.length();
								auto mod = diff % this->m_lines[i + qpl::size{ 1 }].str.length();
								auto str = this->m_lines[i + qpl::size{ 1 }].str;
								if (mod) {
									stream << std::string(str.cbegin() + mod, str.cend());
								}
								for (qpl::u32 j = 0u; j < div; ++j) {
									stream << str;
								}
							}
							else {
								stream << std::string(diff, ' ');
							}
						}
					}
				}
			else if (this->m_lines[i].type == qpl::strform_type::fixed) {
				stream << this->m_lines[i].str;
			}

			++n;			
			if (this->m_lines[i].type == qpl::strform_type::newline) {
				n = qpl::size{};
			}
		}
		return stream.str();
	}
	qpl::size qpl::strform_builder::entries_per_line() const {
		qpl::size result = qpl::size{};
		qpl::size seen = qpl::size{};
		qpl::size ctr = qpl::size{};
		for (const auto& e : this->m_lines) {
			if (e.type == qpl::strform_type::newline) {
				if (result < (ctr - seen)) {
					result = (ctr - seen);
				}
				seen = ctr + 1;
			}
			++ctr;
		}
		return result + 1;
	}
	qpl::size qpl::strform_builder::length_of_entry(qpl::size n) const {
		qpl::size result = qpl::size{};
		auto line_width = this->entries_per_line();
		for (qpl::size i = n; i < this->m_lines.size(); i += line_width) {
			if (result < this->m_lines[i].str.length()) {
				result = this->m_lines[i].str.length();
			}
		}
		return result;
	}
	void qpl::strform_builder::complete_line() {
		strform_content content;
		content.type = strform_type::newline;
		this->m_lines.emplace_back(content);
	}


	wstrform_content wstrform_endl() {
		wstrform_content result;
		result.type = strform_type::newline;
		return result;
	}
	void qpl::wstrform_builder::add_entry(wstrform_content&& content) {
		this->m_lines.emplace_back(content);
	}
	void qpl::wstrform_builder::add_fill_whitespace() {
		this->m_lines.emplace_back(qpl::wstrform_fill(L" "));
	}
	std::wstring qpl::wstrform_builder::string() const {
		std::wostringstream stream;
		qpl::size n = qpl::size{};
		auto line_width = this->entries_per_line();
		for (qpl::size i = qpl::size{}; i < this->m_lines.size(); ++i) {

			if (this->m_lines[i].type == qpl::strform_type::newline) {
				stream << '\n';
			}
			else if (this->m_lines[i].type == qpl::strform_type::dynamic) {
				auto entry_length = this->length_of_entry(n);
				auto word_length = this->m_lines[i].str.length();

				if (entry_length == word_length) {
					stream << this->m_lines[i].str;
				}
				else {
					auto diff = qpl::signed_cast(entry_length - word_length);
					if (this->m_lines[i].orientation == qpl::strform_orientation::middle) {
						diff /= 2;
					}
					if (this->m_lines[i].orientation != qpl::strform_orientation::left) {
						if (i && this->m_lines[i - qpl::size{ 1 }].type == qpl::strform_type::fill) {
							auto div = diff / this->m_lines[i - qpl::size{ 1 }].str.length();
							auto mod = diff % this->m_lines[i - qpl::size{ 1 }].str.length();
							auto str = this->m_lines[i - qpl::size{ 1 }].str;
							for (qpl::u32 j = 0u; j < div; ++j) {
								stream << str;
							}
							if (mod) {
								stream << std::wstring(str.cbegin(), str.cbegin() + mod);
							}
						}
						else {
							stream << std::wstring(diff, ' ');
						}
					}
					stream << this->m_lines[i].str;
					if (this->m_lines[i].orientation == qpl::strform_orientation::middle) {
						diff = (entry_length - word_length) - diff;
					}
					if (this->m_lines[i].orientation != qpl::strform_orientation::right) {
						if (i != this->m_lines.size() - qpl::size{ 1 } && this->m_lines[i + qpl::size{ 1 }].type == qpl::strform_type::fill) {
							auto div = diff / this->m_lines[i + qpl::size{ 1 }].str.length();
							auto mod = diff % this->m_lines[i + qpl::size{ 1 }].str.length();
							auto str = this->m_lines[i + qpl::size{ 1 }].str;
							if (mod) {
								stream << std::wstring(str.cbegin() + mod, str.cend());
							}
							for (qpl::u32 j = 0u; j < div; ++j) {
								stream << str;
							}
						}
						else {
							stream << std::wstring(diff, ' ');
						}
					}
				}
			}
			else if (this->m_lines[i].type == qpl::strform_type::fixed) {
				stream << this->m_lines[i].str;
			}

			++n;
			if (this->m_lines[i].type == qpl::strform_type::newline) {
				n = qpl::size{};
			}
		}
		return stream.str();
	}
	qpl::size qpl::wstrform_builder::entries_per_line() const {
		qpl::size result = qpl::size{};
		qpl::size seen = qpl::size{};
		qpl::size ctr = qpl::size{};
		for (const auto& e : this->m_lines) {
			if (e.type == qpl::strform_type::newline) {
				if (result < (ctr - seen)) {
					result = (ctr - seen);
				}
				seen = ctr + 1;
			}
			++ctr;
		}
		return result + 1;
	}
	qpl::size qpl::wstrform_builder::length_of_entry(qpl::size n) const {
		qpl::size result = qpl::size{};
		auto line_width = this->entries_per_line();
		for (qpl::size i = n; i < this->m_lines.size(); i += line_width) {
			if (result < this->m_lines[i].str.length()) {
				result = this->m_lines[i].str.length();
			}
		}
		return result;
	}
	void qpl::wstrform_builder::complete_line() {
		wstrform_content content;
		content.type = strform_type::newline;
		this->m_lines.emplace_back(content);
	}


	qpl::char_type qpl::random_visible_char() {
		return static_cast<qpl::char_type>(qpl::random_i(32, 126));
	}
	std::string qpl::random_string_with_repetions(qpl::size length, qpl::size repetion_size) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {

			auto rep = qpl::random(qpl::size{ 1 }, repetion_size);
			rep = qpl::min(rep, length - i);

			stream << std::string(rep, qpl::random_visible_char());
		}
		return stream.str();
	}
	std::string qpl::random_string(qpl::size length) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::random_visible_char();
		}
		return stream.str();
	}
	std::string qpl::random_number_string(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i('0', '9');
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << static_cast<qpl::char_type>(qpl::random_i());
		}
		return stream.str();
	}
	std::string qpl::random_lowercase_uppercase_number_string(qpl::size length) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {
			qpl::set_random_range_i('a', 'z');
			auto c = qpl::random_i();
			qpl::set_random_range_i('0', '9');
			auto d = qpl::random_i();
			stream << static_cast<qpl::char_type>(qpl::random_b() ? d : (qpl::random_b() ? (c - ('a' - 'A')) : c));
		}
		return stream.str();
	}
	std::string qpl::random_lowercase_number_string(qpl::size length) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {
			qpl::set_random_range_i('a', 'z');
			auto c = qpl::random_i();
			qpl::set_random_range_i('0', '9');
			auto d = qpl::random_i();
			stream << static_cast<qpl::char_type>(qpl::random_b() ? d : c);
		}
		return stream.str();
	}
	std::string qpl::random_uppercase_number_string(qpl::size length) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {
			qpl::set_random_range_i('A', 'Z');
			auto c = qpl::random_i();
			qpl::set_random_range_i('0', '9');
			auto d = qpl::random_i();
			stream << static_cast<qpl::char_type>(qpl::random_b() ? d : c);
		}
		return stream.str();
	}
	std::string qpl::random_lowercase_uppercase_string(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i('a', 'z');
		for (auto i = qpl::size{}; i < length; ++i) {
			auto r = qpl::random_i();
			stream << static_cast<qpl::char_type>(qpl::random_b() ? (r - ('a' - 'A')) : r);
		}
		return stream.str();
	}
	std::string qpl::random_lowercase_string(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i('a', 'z');
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << static_cast<qpl::char_type>(qpl::random_i());
		}
		return stream.str();
	}
	std::string qpl::random_uppercase_string(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i('A', 'Z');
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << static_cast<qpl::char_type>(qpl::random_i());
		}
		return stream.str();
	}
	std::string qpl::to_lower(const std::string& string) {
		auto result = string;
		for (auto& i : result) {
			i = std::tolower(i);
		}
		return result;
	}
	std::string qpl::to_upper(const std::string& string) {
		auto result = string;
		for (auto& i : result) {
			i = std::toupper(i);
		}
		return result;
	}
	std::vector<std::string> qpl::split_string(const std::string& string, char by_what) {
		std::vector<std::string> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == by_what) {
				if (i - before) {
					result.push_back(string.substr(before, i - before));
				}
				++i;
				while (i < string.length() && string[i] == by_what) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.push_back(string.substr(before));
		}
		return result;
	}
	std::vector<std::string> qpl::split_string_whitespace(const std::string& string) {
		std::vector<std::string> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (qpl::is_character_white_space(string[i])) {
				if (i - before) {
					result.push_back(string.substr(before, i - before));
				}
				++i;
				while (i < string.length() && qpl::is_character_white_space(string[i])) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.push_back(string.substr(before));
		}
		return result;
	}
	std::vector<std::string> qpl::split_string(const std::string& string, const std::string& expression) {
		std::vector<std::string> result;
		std::smatch smatch;
		std::regex reg = std::regex{ "[^" + expression + "]+" };
		auto s = std::sregex_iterator(string.cbegin(), string.cend(), reg);
		while (s != std::sregex_iterator()) {
			result.push_back(s->str());
			++s;
		}
		return result;
	}
	std::vector<std::string> qpl::split_string(const std::string& string) {
		std::vector<std::string> result;

		qpl::size before = 0u;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (std::isspace(string[i])) {
				if (i - before) {
					result.push_back(string.substr(before, i - before));
				}
				++i;
				while (i < string.length() && std::isspace(string[i])) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.push_back(string.substr(before));
		}
		return result;
	}
	std::vector<std::wstring> qpl::split_string(const std::wstring& string, char by_what) {
		std::vector<std::wstring> result;
		std::wsmatch smatch;
		std::wregex reg{ L"[^" + qpl::to_wstring(by_what) + L"]+" };
		auto s = std::wsregex_iterator(string.cbegin(), string.cend(), reg);
		while (s != std::wsregex_iterator()) {
			result.push_back(s->str());
			++s;
		}
		return result;
	}
	std::vector<std::wstring> qpl::split_string(const std::wstring& string, const std::wstring& expression) {
		std::vector<std::wstring> result;
		std::wsmatch smatch;
		std::wregex reg = std::wregex{ L"[^" + expression + L"]+" };
		auto s = std::wsregex_iterator(string.cbegin(), string.cend(), reg);
		while (s != std::wsregex_iterator()) {
			result.push_back(s->str());
			++s;
		}
		return result;
	}
	std::vector<std::wstring> qpl::split_string(const std::wstring& string) {
		std::vector<std::wstring> result;
		std::wsmatch smatch;
		std::wregex reg{ L"[^\\s]+" };
		auto s = std::wsregex_iterator(string.cbegin(), string.cend(), reg);
		while (s != std::wsregex_iterator()) {
			result.push_back(s->str());
			++s;
		}
		return result;
	}

	std::vector<std::string> qpl::split_string_every(const std::string& string, qpl::size n) {
		if (string.empty()) {
			return {};
		}
		auto size = (string.size() - 1) / n + 1;
		std::vector<std::string> result;
		result.reserve(size);
		qpl::size pos = 0;
		for (qpl::u32 i = 0u;; ++i) {

			if (string.length() <= pos) {
				return result;
			}
			auto size = qpl::min(string.length() - pos, n);

			std::string s;
			s.resize(size);
			memcpy(s.data(), string.data() + pos, size);
			result.push_back(s);

			pos += n;
		}
		return result;
	}
	std::string qpl::combine_strings(const std::vector<std::string>& strings) {
		std::ostringstream stream;
		for (auto& i : strings) {
			stream.write(i.data(), i.length());
		}
		return stream.str();
	}

	std::string qpl::string_first_n_characters(const std::string& string, qpl::size n) {
		return string.substr(0, n);
	}
	std::string qpl::string_last_n_characters(const std::string& string, qpl::size n) {
		return string.substr(string.length() - n);
	}

	std::string qpl::string_seperation(const std::string& string, const std::string& seperation, qpl::size n, bool left) {
		std::ostringstream stream;
		if (left) {
			for (qpl::size i = 0u; i < string.length(); i += n) {
				if (i) {
					stream << seperation;
				}
				stream << string.substr(i, qpl::min(string.size() - i, n));
			}
		}
		else {
			auto mod = string.length() % n;
			if (mod) {
				stream << string.substr(0, mod);
				if (mod != string.length()) {
					stream << seperation;
				}
			}
			for (qpl::size i = mod; i < string.length(); i += n) {
				if (i != mod) {
					stream << seperation;
				}
				stream << string.substr(i, n);
			}
		}
		return stream.str();
	}
	std::string qpl::string_seperation(const std::string& string, char seperation, qpl::size n, bool left) {
		std::ostringstream stream;
		if (left) {
			for (qpl::size i = 0u; i < string.length(); i += n) {
				if (i) {
					stream << seperation;
				}
				stream << string.substr(i, qpl::min(string.length() - i, n));
			}
		}
		else {
			auto mod = string.length() % n;
			if (mod) {
				stream << string.substr(0, mod);
				if (mod != string.length()) {
					stream << seperation;
				}
			}
			for (qpl::size i = mod; i < string.length(); i += n) {
				if (i != mod) {
					stream << seperation;
				}
				stream << string.substr(i, n);
			}
		}
		return stream.str();
	}

	bool qpl::is_string_floating_point(std::string string) {
		std::istringstream iss(string);
		qpl::f64 f;
		iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
		// Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}
	bool qpl::is_string_number(std::string string) {
		std::istringstream iss(string);
		qpl::i64 i;
		iss >> std::noskipws >> i; // noskipws considers leading whitespace invalid
		// Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}
	std::string qpl::big_number_string(std::string decimal_string) {

		//const static std::array<std::string, 10> p_ones = { "", "un", "dou", "tre", "quat", "quin", "sex", "sept", "oct", "non" };
		//const static std::array<std::string, 10> p_tens = { "dec", "vig", "trig", "quadrag", "quinquag", "sexag", "septuag", "octog", "nonag" };

		auto leftover = decimal_string.length() % 3u;
		std::ostringstream stream;
		auto n = (decimal_string.length() - 1) / 3u;
		stream << "(" << n << ")" << decimal_string.substr(0u, leftover) << " ";
		for (auto i = leftover; i < decimal_string.length(); i += 3u) {
			--n;
			stream << "(" << n << ")" << decimal_string.substr(i, 3) << " ";
		}
		return stream.str();
	}
	std::string exponent_number_name_short(qpl::u32 exponent3) {
		constexpr auto name_small = std::array{ "K", "M", "B", "Tr", "Qa", "Qi", "Sx", "Sp", "Oc", "No" };
		constexpr auto name_one = std::array{ "", "Un", "Do", "Tr", "Qa", "Qi", "Sx", "Sp", "Oc", "No" };
		constexpr auto name_ten = std::array{ "", "De", "Vi", "Tr", "Qa", "Qi", "Sx", "Sp", "Oc", "No" };
		constexpr auto name_hundred = std::array{ "", "Ce", "Duc", "Trg", "Qag", "Qig", "Sxg", "Spg", "Ocg", "Nog" };
		if (exponent3 <= 10) {
			if (!exponent3) {
				return "";
			}
			return name_small[exponent3 - 1];
		}
		else {
			--exponent3;
			auto one = exponent3 % 10u;
			auto ten = (exponent3 / 10u) % 10u;
			auto hundred = (exponent3 / 100u) % 10u;
			return qpl::to_string(name_one[one], name_ten[ten], name_hundred[hundred]);
		}
	}
	std::string exponent_number_name(qpl::u32 exponent3) {
		return "";
	}

	std::string qpl::random_string_full_range(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i(qpl::type_min<char>(), qpl::type_max<char>());
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::char_cast(qpl::random_i());
		}
		return stream.str();
	}
	std::string qpl::random_string_full_range_with_repetions(qpl::size length, qpl::size repetition_size) {
		std::ostringstream stream;
		qpl::set_random_range_i(qpl::type_min<char>(), qpl::type_max<char>());
		for (auto i = qpl::size{}; i < length; ) {
			auto rep = qpl::random(qpl::size{ 1 }, repetition_size);
			rep = qpl::min(rep, length - i);
			
			stream << std::string(rep, qpl::char_cast(qpl::random_i()));
			i += rep;
		}
		return stream.str();
	}
	std::wstring qpl::random_wstring_full_range(qpl::size length) {
		std::wostringstream stream;
		qpl::set_random_range_i(qpl::type_min<wchar_t>(), qpl::type_max<wchar_t>());
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::char_cast(qpl::random_i());
		}
		return stream.str();
	}


	void qpl::collection_string::set_string(const std::string& string) {
		this->string = string;
	}
	bool qpl::collection_string::read_info() {
		if (this->string.length() < qpl::bytes_in_type<qpl::u64>() + qpl::bytes_in_type<qpl::u32>()) {
			return false;
		}
		qpl::size position = 0;
		qpl::u64 check;
		memcpy(&check, this->string.data() + position, qpl::bytes_in_type<qpl::u64>());

		if (check != this->check_value) {
			return false;
		}

		position += qpl::bytes_in_type<qpl::u64>();
		qpl::u32 size_size;
		memcpy(&size_size, this->string.data() + position, qpl::bytes_in_type<qpl::u32>());
		position += qpl::bytes_in_type<qpl::u32>();

		if (string.length() < (position + size_size * qpl::bytes_in_type<qpl::size>() * 2)) {
			return false;
		}
		this->sizes.resize(size_size);
		for (qpl::u32 i = 0u; i < size_size; ++i) {
			qpl::size size;
			memcpy(&size, this->string.data() + position, qpl::bytes_in_type<qpl::size>());
			position += qpl::bytes_in_type<qpl::size>();
			this->sizes[i].first = size;
			memcpy(&size, this->string.data() + position, qpl::bytes_in_type<qpl::size>());
			position += qpl::bytes_in_type<qpl::size>();
			this->sizes[i].second = size;
		}
		return true;
	}
	std::string qpl::collection_string::get_string() const {
		return this->string;
	}
	std::string_view qpl::collection_string::get_string_sv() const {
		return this->string;
	}
	std::string qpl::collection_string::get_string(qpl::u32 index) const {
		if (index >= this->sizes.size()) {
			throw std::exception(qpl::to_string("qpl::string_collection::get_string(", index, "): size is only ", this->sizes.size()).c_str());
		}
		return std::string(this->string.begin() + this->sizes[index].first, this->string.begin() + this->sizes[index].second);
	}
	std::string_view qpl::collection_string::get_string_sv(qpl::u32 index) const {
		if (index >= this->sizes.size()) {
			throw std::exception(qpl::to_string("qpl::string_collection::get_string_sv(", index, "): size is only ", this->sizes.size()).c_str());
		}
		return std::string_view(this->string.begin() + this->sizes[index].first, this->string.begin() + this->sizes[index].second);
	}
	void qpl::collection_string::add_string(const std::string& string) {
		auto back = this->string.length();
		this->string.append(string);
		this->sizes.push_back({ qpl::size_cast(back), qpl::size_cast(back + string.length()) });
	}
	void qpl::collection_string::finalize() {
		auto header_size = qpl::bytes_in_type<qpl::u64>() + qpl::bytes_in_type<qpl::u32>() + (this->sizes.size() * 2 * qpl::bytes_in_type<qpl::size>());
		std::ostringstream stream;

		qpl::u32 size_size = qpl::u32_cast(this->sizes.size());
		stream.write(reinterpret_cast<const char*>(&this->check_value), qpl::bytes_in_type<qpl::u64>());
		stream.write(reinterpret_cast<const char*>(&size_size), qpl::bytes_in_type<qpl::u32>());
		for (auto& i : this->sizes) {
			i.first += header_size;
			i.second += header_size;
			stream.write(reinterpret_cast<const char*>(&i.first), qpl::bytes_in_type<qpl::size>());
			stream.write(reinterpret_cast<const char*>(&i.second), qpl::bytes_in_type<qpl::size>());
		}

		auto copy = this->string;
		this->string.clear();
		this->string.append(stream.str());
		this->string.append(copy);
	}
	qpl::size qpl::collection_string::size() const {
		return this->sizes.size();
	}
	void qpl::collection_string::clear() {
		this->sizes.clear();
		this->string.clear();
	}
}