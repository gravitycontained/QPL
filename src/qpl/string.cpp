#include <qpl/string.hpp>
#include <qpl/random.hpp>
#include <locale>
#include <cwctype>

namespace qpl {

	std::string qpl::to_lower(const std::string& string) {
		auto result = string;
		for (auto& i : result) {
			i = std::tolower(i);
		}
		return result;
	}
	std::wstring qpl::to_lower(const std::wstring& string) {
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
	std::wstring qpl::to_upper(const std::wstring& string) {
		auto result = string;
		for (auto& i : result) {
			i = std::toupper(i);
		}
		return result;
	}

	std::wstring qpl::string_to_wstring(std::string_view s) {
		return qpl::string_to_wstring(std::string{ s });
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
	

	std::string qpl::wstring_to_string(const std::wstring& str) {
		int size_needed = WideCharToMultiByte(CP_ACP, 0, str.c_str(), (int)str.size(), 0, 0, 0, 0);
		std::string result(size_needed, 0);
		WideCharToMultiByte(CP_ACP, 0, &str[0], (int)str.size(), &result[0], size_needed, 0, 0);
		return result;
	}

	std::wstring qpl::string_to_wstring(const std::string& str) {
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring result(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], size_needed);
		return result;
	}
	std::string qpl::wstring_to_string(std::wstring_view s) {
		return qpl::wstring_to_string(std::wstring{ s });
	}	
	std::string qpl::wchar_to_utf8(wchar_t wc) {
		std::string result;
		if (wc < 0x80) {
			result.push_back(static_cast<char>(wc));
		}
		else if (wc < 0x800) {
			result.push_back(static_cast<char>(0xC0 | wc >> 6));
			result.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
		}
		else if (wc < 0x10000) {
			result.push_back(static_cast<char>(0xE0 | wc >> 12));
			result.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
		}
		else {
			result.push_back(static_cast<char>(0xF0 | wc >> 18));
			result.push_back(static_cast<char>(0x80 | ((wc >> 12) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
		}
		return result;
	}
	std::string qpl::wstring_to_utf8(const std::wstring& string) {
		std::string result = "";
		for (auto& i : string) {
			result += qpl::wchar_to_utf8(i);
		}
		return result;
	}
	wchar_t qpl::utf8_to_wchar(const std::string& utf8) {
		qpl::size len = utf8.size();
		if (len == 0) {
			throw qpl::exception("Empty UTF-8 string");
		}

		wchar_t wc = 0;
		if ((utf8[0] & 0x80) == 0) { // 0xxxxxxx
			wc = utf8[0];
		}
		else if ((utf8[0] & 0xE0) == 0xC0 && len >= 2) { // 110xxxxx 10xxxxxx
			wc = ((utf8[0] & 0x1F) << 6) | (utf8[1] & 0x3F);
		}
		else if ((utf8[0] & 0xF0) == 0xE0 && len >= 3) { // 1110xxxx 10xxxxxx 10xxxxxx
			wc = ((utf8[0] & 0x0F) << 12) | ((utf8[1] & 0x3F) << 6) | (utf8[2] & 0x3F);
		}
		else if ((utf8[0] & 0xF8) == 0xF0 && len >= 4) { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			wc = ((utf8[0] & 0x07) << 18) | ((utf8[1] & 0x3F) << 12) | ((utf8[2] & 0x3F) << 6) | (utf8[3] & 0x3F);
		}
		else {
			throw qpl::exception("Invalid UTF-8 encoding");
		}
		return wc;
	}

	std::wstring qpl::utf8_to_wstring(const std::string& utf8_string) {
		std::wstring result;

		for (qpl::size i = 0; i < utf8_string.size(); ) {
			char c = utf8_string[i];
			if ((c & 0x80) == 0) { // 0xxxxxxx
				result.push_back(utf8_to_wchar(utf8_string.substr(i, 1)));
				++i;
			}
			else if ((c & 0xE0) == 0xC0) { // 110xxxxx 10xxxxxx
				result.push_back(utf8_to_wchar(utf8_string.substr(i, 2)));
				i += 2;
			}
			else if ((c & 0xF0) == 0xE0) { // 1110xxxx 10xxxxxx 10xxxxxx
				result.push_back(utf8_to_wchar(utf8_string.substr(i, 3)));
				i += 3;
			}
			else if ((c & 0xF8) == 0xF0) { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
				result.push_back(utf8_to_wchar(utf8_string.substr(i, 4)));
				i += 4;
			}
			else {
				throw qpl::exception("Invalid UTF-8 encoding");
			}
		}
		return result;
	}
	colored_text_get_position_type qpl::colored_text_get_position;
	std::function<void(std::string, qpl::vec2, std::wstring)> qpl::colored_text_get_position_callback = [](const std::string& name, qpl::vec2 position, const std::wstring& information) {};

	std::function<void(std::wstring)> qpl::default_output_function_w = [](const std::wstring& string) {
		std::wprintf(L"%s", string.c_str());
		//std::wcout << string;
	};
	std::function<void(std::string)> qpl::default_output_function = [](const std::string& string) {
		//std::cout << string;
		std::fwrite(string.data(), sizeof(string[0]) * string.size(), 1, stderr);
	};
	std::function<void(qpl::colored_text_get_position_type)> qpl::default_output_function_get_position{

	};
	std::function<std::wstring()> qpl::default_input_function = []() {
		std::wstring line;
		std::getline(std::wcin, line);
		return line;
	};
	std::function<void(qpl::cc)> qpl::default_output_color_function = [](qpl::cc color) {
		qpl::set_console_color(color);
	};
	std::function<void(std::wstring)> qpl::custom_output_function_w = qpl::default_output_function_w;
	std::function<void(std::string)> qpl::custom_output_function = qpl::default_output_function;
	std::function<void(qpl::colored_text_get_position_type)> qpl::custom_output_function_get_position = qpl::default_output_function_get_position;
	std::function<std::wstring()> qpl::custom_input_function = qpl::default_input_function;
	std::function<void(qpl::cc)> qpl::custom_output_color_function = qpl::default_output_color_function;

	std::function<void(std::wstring)> qpl::output_function_w = qpl::default_output_function_w;
	std::function<void(std::string)> qpl::output_function = qpl::default_output_function;
	std::function<void(qpl::colored_text_get_position_type)> qpl::output_function_get_position = qpl::default_output_function_get_position;
	std::function<std::wstring()> qpl::input_function = qpl::default_input_function;
	std::function<void(qpl::cc)> qpl::output_color_function = qpl::default_output_color_function;

	bool qpl::use_default_print = true;
	void qpl::use_default_print_functions() {
		qpl::output_function_w = qpl::default_output_function_w;
		qpl::output_function = qpl::default_output_function;
		qpl::output_function_get_position = qpl::default_output_function_get_position;
		qpl::input_function = qpl::default_input_function;
		qpl::output_color_function = qpl::default_output_color_function;
		qpl::use_default_print = true;
	}
	void qpl::use_custom_print_functions() {
		qpl::output_function_w = qpl::custom_output_function_w;
		qpl::output_function = qpl::custom_output_function;
		qpl::output_function_get_position = qpl::custom_output_function_get_position;
		qpl::input_function = qpl::custom_input_function;
		qpl::output_color_function = qpl::custom_output_color_function;
		qpl::use_default_print = false;
	}

	std::wostringstream detail::stream_wstr;

	std::string qpl::to_string(const std::string& first) {
		return first;
	}
	std::wstring qpl::to_wstring(const std::wstring& first) {
		return first;
	}
	std::string qpl::bool_string(bool b) {
		return b ? std::string("true") : std::string("false");
	}
	std::string qpl::memory_size_string(qpl::size bytes, bool binary) {
		if (bytes < 1000) {
			return qpl::to_string(bytes, " bytes");
		}
		else {
			auto converted = qpl::f64_cast(bytes);

			if (binary) {
				constexpr auto names = std::array{ "kibi", "mebi", "gibi", "tebi", "pebi", "exbi", "zebi", "yebi" };

				for (qpl::size i = 0u; i < names.size(); ++i) {
					converted /= 1024.0;
					if (converted < 1000) {
						return qpl::to_string(qpl::to_string_precision(1, converted), " ", names[i], "bytes");
					}
				}
			}
			else {
				constexpr auto names = std::array{ "kilo", "mega", "giga", "tera", "peta", "exa", "zetta", "yotta" };

				for (qpl::size i = 0u; i < names.size(); ++i) {
					converted /= 1000.0;
					if (converted < 1000) {
						return qpl::to_string(qpl::to_string_precision(1, converted), " ", names[i], "bytes");
					}
				}
			}
		}
		return "";
	}

	std::string qpl::string_to_fit(const std::string& string, char append, qpl::size length) {
		return std::string(string.length() >= length ? 0ull : length - string.length(), append);
	}

	namespace detail {
		std::string  qpl::detail::appended_to_string_to_fit(const std::string_view& string, char append, qpl::size length) {
			if (string.length() >= length) {
				return std::string{ string };
			}
			auto result = std::string{ string };
			result += std::string(length - string.length(), append);
			return result;
		}
		std::string  qpl::detail::appended_to_string_to_fit(const std::string_view& string, const std::string_view& prepend, qpl::size length) {
			if (string.length() >= length) {
				return std::string{ string };
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
		std::wstring qpl::detail::appended_to_string_to_fit(const std::wstring_view& string, wchar_t append, qpl::size length) {
			if (string.length() >= length) {
				return std::wstring{ string };
			}
			auto result = std::wstring{ string };
			result += std::wstring(length - string.length(), append);
			return result;
		}
		std::wstring qpl::detail::appended_to_string_to_fit(const std::wstring_view& string, const std::wstring_view& prepend, qpl::size length) {
			if (string.length() >= length) {
				return std::wstring{ string };
			}

			auto multiple = ((string.length() - 1) / prepend.length() + 1) * prepend.length();
			auto left = multiple - string.length();
			auto full = length < multiple ? 0u : ((length - multiple) / prepend.length());
			auto right = length < multiple ? 0u : (length % prepend.length());

			std::wostringstream stream;
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
		std::string  qpl::detail::appended_to_fit(qpl::size prepended_length, const std::string_view& prepend, qpl::size length) {
			if (prepended_length >= length) {
				return "";
			}

			auto multiple = ((prepended_length - 1) / prepend.length() + 1) * prepend.length();
			auto left = multiple - prepended_length;
			auto full = length < multiple ? 0u : ((length - multiple) / prepend.length());
			auto right = length < multiple ? 0u : (length % prepend.length());

			std::ostringstream stream;

			if (left) {
				stream << prepend.substr(prepend.length() - left, length - prepended_length);
			}
			for (qpl::u32 i = 0u; i < full; ++i) {
				stream << prepend;
			}
			if (right) {
				stream << prepend.substr(0, right);
			}

			return stream.str();
		}
		std::wstring qpl::detail::appended_to_fit(qpl::size prepended_length, const std::wstring_view& prepend, qpl::size length) {
			if (prepended_length >= length) {
				return L"";
			}

			auto multiple = ((prepended_length - 1) / prepend.length() + 1) * prepend.length();
			auto left = multiple - prepended_length;
			auto full = length < multiple ? 0u : ((length - multiple) / prepend.length());
			auto right = length < multiple ? 0u : (length % prepend.length());

			std::wostringstream stream;

			if (left) {
				stream << prepend.substr(prepend.length() - left, length - prepended_length);
			}
			for (qpl::u32 i = 0u; i < full; ++i) {
				stream << prepend;
			}
			if (right) {
				stream << prepend.substr(0, right);
			}

			return stream.str();
		}

		std::string  qpl::detail::prepended_to_string_to_fit(const std::string_view& string, char prepend, qpl::size length) {
			if (string.length() >= length) {
				return std::string{ string };
			}
			auto result = std::string(length - string.length(), prepend);
			result += string;
			return result;
		}
		std::string  qpl::detail::prepended_to_string_to_fit(const std::string_view& string, const std::string_view& prepend, qpl::size length) {
			if (string.length() >= length) {
				return std::string{ string };
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
		std::wstring qpl::detail::prepended_to_string_to_fit(const std::wstring_view& string, wchar_t prepend, qpl::size length) {
			if (string.length() >= length) {
				return std::wstring{ string };
			}
			auto result = std::wstring(length - string.length(), prepend);
			result += string;
			return result;
		}
		std::wstring qpl::detail::prepended_to_string_to_fit(const std::wstring_view& string, const std::wstring_view& prepend, qpl::size length) {
			if (string.length() >= length) {
				return std::wstring{ string };
			}

			auto mod = (length - string.length()) % prepend.length();
			auto div = (length - string.length()) / prepend.length();

			std::wostringstream stream;
			for (qpl::u32 i = 0u; i < div; ++i) {
				stream << prepend;
			}
			if (mod) {
				stream << prepend.substr(0, mod);
			}

			stream << string;
			return stream.str();
		}
		std::string  qpl::detail::prepended_to_fit(qpl::size appended_length, const std::string_view& prepend, qpl::size length) {
			if (appended_length >= length) {
				return "";
			}

			auto mod = (length - appended_length) % prepend.length();
			auto div = (length - appended_length) / prepend.length();

			std::ostringstream stream;
			for (qpl::u32 i = 0u; i < div; ++i) {
				stream << prepend;
			}
			if (mod) {
				stream << prepend.substr(0, mod);
			}
			return stream.str();
		}
		std::wstring qpl::detail::prepended_to_fit(qpl::size appended_length, const std::wstring_view& prepend, qpl::size length) {
			if (appended_length >= length) {
				return L"";
			}

			auto mod = (length - appended_length) % prepend.length();
			auto div = (length - appended_length) / prepend.length();

			std::wostringstream stream;
			for (qpl::u32 i = 0u; i < div; ++i) {
				stream << prepend;
			}
			if (mod) {
				stream << prepend.substr(0, mod);
			}
			return stream.str();
		}

		std::wstring qpl::detail::appended_to_unicode_string_to_fit(const std::wstring_view& string, wchar_t append, qpl::size length) {
			auto ulength = qpl::unicode_string_length(string);
			if (ulength >= length) {
				return std::wstring{ string };
			}
			auto result = std::wstring{ string };
			result += std::wstring(length - ulength, append);
			return result;
		}
		std::wstring qpl::detail::appended_to_unicode_string_to_fit(const std::wstring_view& string, const std::wstring_view& prepend, qpl::size length) {
			auto ulength = qpl::unicode_string_length(string);
			if (ulength >= length) {
				return std::wstring{ string };
			}

			auto multiple = ((ulength - 1) / prepend.length() + 1) * prepend.length();
			auto left = multiple - ulength;
			auto full = length < multiple ? 0u : ((length - multiple) / prepend.length());
			auto right = length < multiple ? 0u : (length % prepend.length());

			std::wostringstream stream;
			stream << string;


			if (left) {
				stream << prepend.substr(prepend.length() - left, length - ulength);
			}
			for (qpl::u32 i = 0u; i < full; ++i) {
				stream << prepend;
			}
			if (right) {
				stream << prepend.substr(0, right);
			}

			return stream.str();
		}
		std::wstring qpl::detail::prepended_to_unicode_string_to_fit(const std::wstring_view& string, wchar_t prepend, qpl::size length) {
			auto ulength = qpl::unicode_string_length(string);
			if (ulength >= length) {
				return std::wstring{ string };
			}
			auto result = std::wstring(length - ulength, prepend);
			result += string;
			return result;
		}
		std::wstring qpl::detail::prepended_to_unicode_string_to_fit(const std::wstring_view& string, const std::wstring_view& prepend, qpl::size length) {
			auto ulength = qpl::unicode_string_length(string);
			if (ulength >= length) {
				return std::wstring{ string };
			}

			auto mod = (length - ulength) % prepend.length();
			auto div = (length - ulength) / prepend.length();

			std::wostringstream stream;
			for (qpl::u32 i = 0u; i < div; ++i) {
				stream << prepend;
			}
			if (mod) {
				stream << prepend.substr(0, mod);
			}

			stream << string;
			return stream.str();
		}
	}

	std::string qpl::two_strings_fixed_insert(const std::string& a, const std::string& b, const std::string_view& insert, qpl::size length, qpl::u32 rotation) {
		std::ostringstream stream;
		auto size = ((a.length() - 1) / insert.length() + 1) * insert.length() + rotation % insert.length();
		size = qpl::min(size, a.length() + (length - (a.length() + b.length())));

		stream << qpl::appended_to_string_to_fit(a, insert, size);
		stream << qpl::prepended_to_string_to_fit(b, insert, length - size);
		auto str_size = stream.str().length();
		return stream.str();
	}

	template<>
	std::string qpl::console_space(qpl::size n, const std::string& string) {
		return qpl::prepended_to_string_to_fit(string, ' ', n);
	}
	std::ostream& qpl::operator<<(std::ostream& os, print_effect effect) {
		return os;
	}

	t_cclearln qpl::cclearln;
	t_cclear qpl::cclear;

	bool qpl::console_effect_state::is_default() const {
		return this->current_console_color == qpl::cc::def()
				&& this->println_color == qpl::cc::def()
				&& this->println_default_color == qpl::cc::def()
				&& this->next_print_space == 0u
				&& this->println_space == 0u
				&& this->println_default_space == 0u
				&& this->next_print_color == false
				&& this->next_println_space == false
				&& this->next_println_color == false;
	}

	namespace detail {
		qpl::console_effect_state console_effect_state;
	}
	std::string qpl::base64_string(const std::string_view& input) {
		std::string output;
		for (qpl::size i = 0u; i < input.length(); i += 3u) {
			std::bitset<24> buffer{ 0 };
			for (qpl::size j = 0; j < 3; ++j) {
				buffer <<= 8;
				if (i + j < input.length()) {
					buffer |= qpl::u8_cast(input[i + j]);
				}
			}
			for (qpl::size j = 0; j < 4; ++j) {
				if (i + j * 6 / 8 < input.length()) {
					output += qpl::detail::base_64[(buffer >> (18 - j * 6)).to_ulong() & 0x3F];
				}
			}
		}
		return output;
	}
	std::string qpl::from_base64_string(const std::string_view& input) {
		std::string output;
		for (qpl::size i = 0; i < input.length(); i += 4) {
			std::bitset<24> buffer{ 0 };

			for (qpl::size j = 0; j < 4; ++j) {
				buffer <<= 6;
				if (i + j < input.length()) {
					buffer |= qpl::detail::base_64_inv[qpl::u8_cast(input[i + j])];
				}
			}

			for (qpl::size j = 0u; j < 3u; ++j) {
				if (i + j + 1 < input.length()) {
					output += static_cast<char>((buffer >> (16 - j * 8)).to_ulong() & 0xFF);
				}
			}
		}

		return output;
	}
	std::string qpl::hex_string(const std::string_view& string) {
		std::ostringstream stream;
		for (auto& i : string) {
			stream << qpl::prepended_to_string_to_fit(qpl::hex_string(qpl::u8_cast(i), ""), "0", 2);
		}
		return stream.str();
	}
	std::string qpl::from_hex_string(const std::string_view& string) {
		std::ostringstream stream;
		for (qpl::size i = 0u; i < string.length(); i += 2u) {
			stream << qpl::char_cast(qpl::from_base_string(string.substr(i, 2u), 16));
		}
		return stream.str();
	}
	std::string qpl::hex_to_base64_string(const std::string_view& string) {
		return qpl::base64_string(qpl::from_hex_string(string));
	}
	std::string qpl::base64_to_hex_string(const std::string_view& string) {
		return qpl::hex_string(qpl::from_base64_string(string));
	}
	std::string qpl::binary_string(const std::string& string) {
		std::ostringstream stream;
		for (auto& i : string) {
			stream << qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u8_cast(i)), "0", 8);
		}
		return stream.str();
	}
	std::string qpl::string_xor(std::string a, std::string b) {
		std::string result;
		result.resize(qpl::min(a.size(), b.size()));
		for (qpl::size i = 0u; i < result.length(); ++i) {
			result[i] = a[i] ^ b[i];
		}
		return result;
	}
	std::string qpl::hex_string_xor(std::string a, std::string b) {
		std::string result;
		result.resize(qpl::min(a.size(), b.size()));
		for (qpl::size i = 0u; i < result.length(); ++i) {
			auto value = qpl::from_base_char(a[i], 16u) ^ qpl::from_base_char(b[i], 16u);
			result[i] = qpl::base_char(value, 16u);
		}
		return result;
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


	qpl::char_type qpl::get_random_visible_char() {
		return static_cast<qpl::char_type>(qpl::random_i(32, 126));
	}
	std::string qpl::get_random_string_with_repetions(qpl::size length, qpl::size repetion_size) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length;) {

			auto rep = qpl::random(qpl::size{ 1 }, repetion_size);
			rep = qpl::min(rep, length - i);

			stream << std::string(rep, qpl::get_random_visible_char());
			i += rep;
		}
		return stream.str();
	}
	std::string qpl::get_random_string_with_repetions(qpl::size length, std::vector<qpl::size> repetions) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length;) {

			auto rep = qpl::random_element(repetions);
			rep = qpl::min(rep, length - i);

			stream << std::string(rep, qpl::get_random_visible_char());
			i += rep;
		}
		return stream.str();
	}
	std::string qpl::get_random_string(qpl::size length) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::get_random_visible_char();
		}
		return stream.str();
	}
	std::string qpl::get_random_number_string(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i('0', '9');
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << static_cast<qpl::char_type>(qpl::random_i());
		}
		return stream.str();
	}
	std::string qpl::get_random_lowercase_uppercase_number_string(qpl::size length) {
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
	std::string qpl::get_random_lowercase_number_string(qpl::size length) {
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
	std::string qpl::get_random_uppercase_number_string(qpl::size length) {
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
	std::string qpl::get_random_lowercase_uppercase_string(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i('a', 'z');
		for (auto i = qpl::size{}; i < length; ++i) {
			auto r = qpl::random_i();
			stream << static_cast<qpl::char_type>(qpl::random_b() ? (r - ('a' - 'A')) : r);
		}
		return stream.str();
	}
	std::string qpl::get_random_lowercase_string(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i('a', 'z');
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << static_cast<qpl::char_type>(qpl::random_i());
		}
		return stream.str();
	}
	std::string qpl::get_random_uppercase_string(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i('A', 'Z');
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << static_cast<qpl::char_type>(qpl::random_i());
		}
		return stream.str();
	}

	std::string qpl::get_random_string_full_range(qpl::size length) {
		std::ostringstream stream;
		qpl::set_random_range_i(qpl::type_min<char>(), qpl::type_max<char>());
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::char_cast(qpl::random_i());
		}
		return stream.str();
	}
	std::string qpl::get_random_string_full_range_with_repetitions(qpl::size length, qpl::size repetition_size) {
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
	std::wstring qpl::get_random_wstring_full_range(qpl::size length) {
		std::wostringstream stream;
		qpl::set_random_range_i(qpl::type_min<wchar_t>(), qpl::type_max<wchar_t>());
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::char_cast(qpl::random_i());
		}
		return stream.str();
	}
	std::string qpl::get_random_hex_string(qpl::size length) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::detail::base_36_lower[qpl::random(0u, 15u)];
		}
		return stream.str();
	}
	std::string qpl::get_random_uppercase_hex_string(qpl::size length) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::detail::base_36_upper[qpl::random(0u, 15u)];
		}
		return stream.str();
	}
	std::string qpl::get_random_hex_string_with_repetitions(qpl::size length, qpl::size repetitions) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ) {

			auto rep = qpl::random(qpl::size{ 1 }, repetitions);
			rep = qpl::min(rep, length - i);

			auto hex = qpl::detail::base_36_lower[qpl::random(0u, 15u)];
			stream << qpl::to_string_repeat(hex, rep);
			i += rep;
		}
		return stream.str();
	}
	std::string qpl::get_random_base64_string(qpl::size length) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ++i) {
			stream << qpl::detail::base_64[qpl::random(0u, 63u)];
		}
		return stream.str();
	}
	std::string qpl::get_random_base64_string_with_repetitions(qpl::size length, qpl::size repetitions) {
		std::ostringstream stream;
		for (auto i = qpl::size{}; i < length; ) {

			auto rep = qpl::random(qpl::size{ 1 }, repetitions);
			rep = qpl::min(rep, length - i);

			auto base64 = qpl::detail::base_64[qpl::random(0u, 63u)];
			stream << qpl::to_string_repeat(base64, rep);
			i += rep;
		}
		return stream.str();
	}

	std::string qpl::remove_backslash_r(const std::string& string) {
		std::string result = string;
		if (result.back() == '\r') {
			result.pop_back();
		}
		return result;
	}
	std::wstring qpl::remove_backslash_r(const std::wstring& string) {
		std::wstring result = string;
		if (result.back() == L'\r') {
			result.pop_back();
		}
		return result;
	}

	std::string qpl::string_replace(const std::string& string, const std::string& search, const std::string& replace, bool ignore_case) {
		if (search.empty()) {
			return string;
		}
		std::ostringstream result;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());

			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result << string.substr(0u, i);
				result << replace << string.substr(i + search.length());
				return result.str();
			}
		}
		return string;
	}
	std::string qpl::string_replace_all(const std::string& string, const std::string& search, const std::string& replace, bool ignore_case) {
		if (search.empty()) {
			return string;
		}
		std::ostringstream result;
		qpl::size index_before = 0u;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());
			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result << string.substr(index_before, i - index_before);
				result << replace;

				index_before = i + search.length();
				i += (search.length() - 1);
			}
		}
		result << string.substr(index_before, string.length() - index_before + 1);
		auto s = result.str();
		if (s.empty()) {
			return string;
		}
		return s;
	}

	std::string qpl::string_remove(const std::string& string, const std::string& search, bool ignore_case) {
		if (search.empty()) {
			return string;
		}
		std::ostringstream result;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());
			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result << string.substr(0u, i);
				result << string.substr(i + search.length());
				return result.str();
			}
		}
		return string;
	}
	std::string qpl::string_remove_all(const std::string& string, const std::string& search, bool ignore_case) {
		if (search.empty()) {
			return string;
		}
		std::ostringstream result;
		qpl::size index_before = 0u;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());
			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result << string.substr(index_before, i - index_before);

				index_before = i + search.length();
				i += (search.length() - 1);
			}
		}
		result << string.substr(index_before, string.length() - index_before + 1);
		auto s = result.str();
		if (s.empty()) {
			return string;
		}
		return s;
	}
	std::string qpl::string_remove_whitespace(const std::string& string) {
		std::string result;
		for (auto& i : string) {
			if (!qpl::is_character_whitespace(i)) {
				result.push_back(i);
			}
		}
		return result;
	}
	std::vector<qpl::size> qpl::string_find_all(const std::string_view& string, const std::string_view& search, bool ignore_case) {
		if (search.empty()) {
			return {};
		}
		std::vector<qpl::size> result;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());

			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result.push_back(i);
				i += (search.length() - 1);
			}
		}
		return result;
	}
	std::vector<qpl::size> qpl::string_find_all(const std::wstring_view& string, const std::wstring_view& search, bool ignore_case) {
		if (search.empty()) {
			return {};
		}
		std::vector<qpl::size> result;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());

			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result.push_back(i);
				i += (search.length() - 1);
			}
		}
		return result;
	}

	std::wstring qpl::string_replace(const std::wstring& string, const std::wstring& search, const std::wstring& replace, bool ignore_case) {
		if (search.empty()) {
			return string;
		}
		std::wostringstream result;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());

			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result << string.substr(0u, i);
				result << replace << string.substr(i + search.length());
				return result.str();
			}
		}
		return string;
	}
	std::wstring qpl::string_replace_all(const std::wstring& string, const std::wstring& search, const std::wstring& replace, bool ignore_case) {
		if (search.empty()) {
			return string;
		}
		std::wostringstream result;
		qpl::size index_before = 0u;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());
			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result << string.substr(index_before, i - index_before);
				result << replace;

				index_before = i + search.length();
				i += (search.length() - 1);
			}
		}
		result << string.substr(index_before, string.length() - index_before + 1);
		auto s = result.str();
		if (s.empty()) {
			return string;
		}
		return s;
	}

	std::wstring qpl::string_remove(const std::wstring& string, const std::wstring& search, bool ignore_case) {
		if (search.empty()) {
			return string;
		}
		std::wostringstream result;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());
			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result << string.substr(0u, i);
				result << string.substr(i + search.length());
				return result.str();
			}
		}
		return string;
	}
	std::wstring qpl::string_remove_all(const std::wstring& string, const std::wstring& search, bool ignore_case) {
		if (search.empty()) {
			return string;
		}
		std::wostringstream result;
		qpl::size index_before = 0u;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());
			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result << string.substr(index_before, i - index_before);

				index_before = i + search.length();
				i += (search.length() - 1);
			}
		}
		result << string.substr(index_before, string.length() - index_before + 1);
		auto s = result.str();
		if (s.empty()) {
			return string;
		}
		return s;
	}

	qpl::size qpl::string_find(const std::wstring& string, const std::wstring& search, bool ignore_case) {
		if (search.empty()) {
			return 0u;
		}
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());

			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				return i;
			}
		}
		return string.length();
	}
	std::vector<qpl::size> qpl::string_find_all(const std::wstring& string, const std::wstring& search, bool ignore_case) {
		if (search.empty()) {
			return {};
		}
		std::vector<qpl::size> result;
		for (qpl::size i = 0u; i < string.length() - search.length() + 1; ++i) {
			auto substr = string.substr(i, search.length());

			bool match = (ignore_case ? substr == search : qpl::string_equals_ignore_case(substr, search));
			if (match) {
				result.push_back(i);
				i += (search.length() - 1);
			}
		}
		return result;
	}


	std::string qpl::string_extract(std::string& string, char seperated_by_what) {
		std::string found = "";
		qpl::size begin = qpl::size_max;
		qpl::size end = qpl::size_max;
		for (qpl::size i = 0u; i < string.length(); ++i) {
			if (string[i] == seperated_by_what) {
				if (begin == qpl::size_max) {
					begin = i;
				}
				else {
					end = i + 1;
					break;
				}
			}
		}
		if (begin != qpl::size_max && end != qpl::size_max) {
			found = string.substr(begin + 1, end - begin - 2);
			string = qpl::string_remove(string, string.substr(begin, end - begin));
		}
		return found;
	}
	std::string qpl::string_extract(std::string& string, std::string seperated_by_what) {
		std::string found = "";
		qpl::size begin = qpl::size_max;
		qpl::size end = qpl::size_max;
		for (qpl::size i = 0u; i < string.length() - seperated_by_what.length(); ++i) {

			auto next = string.substr(i, seperated_by_what.length());
			if (next == seperated_by_what) {
				if (begin == qpl::size_max) {
					begin = i;
				}
				else {
					end = i + seperated_by_what.length();
					break;
				}
			}
		}
		if (begin != qpl::size_max && end != qpl::size_max) {
			found = string.substr(begin + seperated_by_what.length(), end - begin - (seperated_by_what.length() * 2));
			string = qpl::string_remove(string, string.substr(begin, end - begin));
		}
		return found;
	}
	std::vector<std::string> qpl::string_split(const std::string_view& string, char by_what) {
		std::vector<std::string> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == by_what) {
				if (i - before) {
					result.emplace_back(std::string{ string.substr(before, i - before) });
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
			result.emplace_back(std::string{ string.substr(before) });
		}
		return result;
	}
	std::vector<std::string> qpl::string_split_whitespace(const std::string_view& string) {
		std::vector<std::string> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (qpl::is_character_white_space(string[i])) {
				if (i - before) {
					result.emplace_back(std::string{ string.substr(before, i - before) });
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
			result.emplace_back(std::string{ string.substr(before) });
		}
		return result;
	}

	std::vector<std::wstring> qpl::string_split_whitespace(const std::wstring_view& string) {
		std::vector<std::wstring> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (std::iswspace(string[i])) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				while (i < string.length() && std::iswspace(string[i])) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.emplace_back(std::wstring{ string.substr(before) });
		}
		return result;
	}

	std::vector<std::wstring> qpl::string_split_consider_quotes(const std::wstring_view& string, wchar_t by_what, wchar_t quotes) {
		std::vector<std::wstring> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == quotes) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				qpl::size begin = i;
				while (i < string.length() && string[i] != quotes) {
					++i;
				}
				result.emplace_back(std::wstring{ string.substr(begin, i - begin) });
				++i;
				before = i;
			}
			else if (string[i] == by_what) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
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
		if (before < string.length()) {
			result.emplace_back(std::wstring{ string.substr(before) });
		}
		return result;
	}
	std::vector<std::wstring> qpl::string_split_allow_empty_consider_quotes(const std::wstring_view& string, wchar_t by_what, wchar_t quotes) {
		std::vector<std::wstring> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == quotes) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				qpl::size begin = i;
				while (i < string.length() && string[i] != quotes) {
					++i;
				}
				result.emplace_back(std::wstring{ string.substr(begin, i - begin) });
				++i;
				before = i;
			}
			else if (string[i] == by_what) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				while (i < string.length() && string[i] == by_what) {
					result.push_back(L"");
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before < string.length()) {
			result.emplace_back(std::wstring{ string.substr(before) });
		}
		return result;
	}
	std::vector<std::wstring> qpl::string_split_whitespace_consider_quotes(const std::wstring_view& string, wchar_t quotes) {
		std::vector<std::wstring> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == quotes) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				qpl::size begin = i;
				while (i < string.length() && string[i] != quotes) {
					++i;
				}
				result.emplace_back(std::wstring{ string.substr(begin, i - begin) });
				++i;
				before = i;
			}
			else if (std::iswspace(string[i])) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				while (i < string.length() && std::iswspace(string[i])) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before < string.length()) {
			result.emplace_back(std::wstring{ string.substr(before) });
		}

		std::vector<std::wstring> filtered;
		for (auto& entry : result) {
			if (entry != L",") {
				filtered.push_back(entry);
			}
		}
		return filtered;
	}
	std::vector<std::wstring> qpl::string_split_whitespace_consider_quotes_and_extra_quotes(const std::wstring_view& string, wchar_t quotes, wchar_t extra_quotes) {
		std::vector<std::wstring> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == quotes) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				qpl::size begin = i;

				if (i < string.length() && string[i] != quotes) {
					bool inside_extra_quotes = false;
					while (i < string.length()) {
						++i;
						if (string[i] == extra_quotes) {
							inside_extra_quotes = !inside_extra_quotes;
						}
						if (!inside_extra_quotes && string[i] == quotes) {
							break;
						}
					}
				}
				result.emplace_back(std::wstring{ string.substr(begin, i - begin) });
				++i;
				before = i;
			}
			else if (std::iswspace(string[i])) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				while (i < string.length() && std::iswspace(string[i])) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before < string.length()) {
			result.emplace_back(std::wstring{ string.substr(before) });
		}


		std::vector<std::wstring> filtered;
		for (auto& entry : result) {
			if (entry != L",") {
				filtered.push_back(entry);
			}
		}
		return filtered;
	}
	std::vector<std::wstring> qpl::string_split_whitespace_consider_parantheses_and_extra_quotes(const std::wstring_view& string, std::vector<std::wstring> paras, std::wstring additional_quotes) {
		std::vector<std::wstring> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {

			bool any_para_match = false;
			for (auto& para : paras) {
				if (string[i] == para[0]) {
					any_para_match = true;
					if (i - before) {
						result.emplace_back(std::wstring{ string.substr(before, i - before) });
					}
					++i;
					qpl::size begin = i;

					if (i < string.length() && string[i] != para[1]) {
						std::vector<char> inside_extra_quotes(additional_quotes.length());
						for (auto& extra : inside_extra_quotes) {
							extra = false;
						}

						while (i < string.length()) {
							++i;
							for (qpl::size j = 0u; j < additional_quotes.length(); ++j) {
								if (string[i] == additional_quotes[j]) {
									bool any_other = false;
									for (qpl::size k = 0u; k < inside_extra_quotes.size(); ++k) {
										if (j == k) {
											continue;
										}
										if (inside_extra_quotes[k]) {
											any_other = true;
											break;
										}
									}
									if (!any_other) {
										inside_extra_quotes[j] = !qpl::bool_cast(inside_extra_quotes[j]);
									}
								}
							}
							bool any_inside = false;
							for (auto& extra : inside_extra_quotes) {
								if (extra) {
									any_inside = true;
								}
							}
							if (!any_inside && string[i] == para[1]) {
								break;
							}
						}
					}
					result.emplace_back(std::wstring{ string.substr(begin, i - begin) });
					++i;
					before = i;
					break;
				}
			}
			if (!any_para_match) {
				std::vector<char> inside_extra_quotes(additional_quotes.length());
				for (auto& extra : inside_extra_quotes) {
					extra = false;
				}

				bool any_extra_quote = false;
				for (qpl::size j = 0u; j < additional_quotes.length(); ++j) {
					if (string[i] == additional_quotes[j]) {
						inside_extra_quotes[j] = true;
						any_extra_quote = true;
						break;
					}
				}
				if (any_extra_quote) {
					while (i < string.length()) {
						++i;
						for (qpl::size j = 0u; j < additional_quotes.length(); ++j) {
							if (string[i] == additional_quotes[j]) {
								bool any_other = false;
								for (qpl::size k = 0u; k < inside_extra_quotes.size(); ++k) {
									if (j == k) {
										continue;
									}
									if (inside_extra_quotes[k]) {
										any_other = true;
										break;
									}
								}
								if (!any_other) {
									inside_extra_quotes[j] = !qpl::bool_cast(inside_extra_quotes[j]);
								}
							}
						}
						bool any_inside = false;
						for (auto& extra : inside_extra_quotes) {
							if (extra) {
								any_inside = true;
							}
						}
						if (!any_inside) {
							break;
						}
					}
				}
				else if (std::iswspace(string[i])) {
					if (i - before) {
						result.emplace_back(std::wstring{ string.substr(before, i - before) });
					}
					++i;
					while (i < string.length() && std::iswspace(string[i])) {
						++i;
					}
					before = i;
				}
				else {
					++i;
				}
			}
		}
		if (before < string.length()) {
			result.emplace_back(std::wstring{ string.substr(before) });
		}

		std::vector<std::wstring> filtered;
		for (auto& entry : result) {
			if (entry != L",") {
				filtered.push_back(entry);
			}
		}
		return filtered;
	}

	std::vector<std::pair<std::wstring, qpl::size>> string_split_whitespace_with_indices(const std::wstring_view& string) {
		std::vector<std::pair<std::wstring, qpl::size>> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (std::iswspace(string[i])) {
				if (i - before) {
					result.push_back(std::make_pair(std::wstring{ string.substr(before, i - before) }, before));
				}
				++i;
				while (i < string.length() && std::iswspace(string[i])) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.push_back(std::make_pair(std::wstring{ string.substr(before) }, before));
		}
		return result;
	}
	std::vector<std::string> qpl::string_split(const std::string& string, const std::string& expression) {
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
	std::vector<std::string> qpl::string_split(const std::string_view& string) {
		std::vector<std::string> result;

		qpl::size before = 0u;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (std::isspace(string[i])) {
				if (i - before) {
					result.emplace_back(std::string{ string.substr(before, i - before) });
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
			result.emplace_back(std::string{ string.substr(before) });
		}
		return result;
	}
	std::vector<std::string> qpl::string_split_allow_empty(const std::string_view& string, char by_what) {
		std::vector<std::string> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == by_what) {
				if (i - before) {
					result.emplace_back(std::string{ string.substr(before, i - before) });
				}
				++i;
				while (i < string.length() && string[i] == by_what) {
					result.push_back("");
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.emplace_back(std::string{ string.substr(before) });
		}
		return result;
	}
	std::vector<std::wstring> qpl::string_split(const std::wstring_view& string, char by_what) {
		std::vector<std::wstring> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == by_what) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
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
			result.emplace_back(std::wstring{ string.substr(before) });
		}
		return result;
	}
	std::vector<std::wstring> qpl::string_split(const std::wstring_view& string, wchar_t by_what) {
		std::vector<std::wstring> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == by_what) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
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
			result.emplace_back(std::wstring{ string.substr(before) });
		}
		return result;
	}
	std::vector<std::wstring> qpl::string_split(const std::wstring& string, const std::wstring& expression) {
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
	std::vector<std::wstring> qpl::string_split(const std::wstring_view& string) {
		if (string.empty()) {
			return {};
		}
		std::vector<std::wstring> result;

		qpl::size before = 0u;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (std::isspace(string[i])) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				while (i < string.length() && std::iswspace(string[i])) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.emplace_back(std::wstring{ string.substr(before) });
		}
		return result;
	}

	std::vector<std::string> qpl::string_split_words(const std::string_view& string) {
		if (string.empty()) {
			return {};
		}
		std::vector<std::string> result;

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (qpl::is_character_special(string[i])) {
				if (i - before) {
					result.emplace_back(std::string{ string.substr(before, i - before) });
				}
				++i;
				while (i < string.length() && qpl::is_character_special(string[i])) {
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.emplace_back(std::string{ string.substr(before) });
		}
		return result;
	}
	std::vector<std::wstring> qpl::string_split_allow_empty(const std::wstring_view& string, wchar_t by_what) {
		if (string.empty()) {
			return {};
		}
		std::vector<std::wstring> result;

		if (string.front() == by_what) {
			result.push_back(L"");
		}

		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (string[i] == by_what) {
				if (i - before) {
					result.emplace_back(std::wstring{ string.substr(before, i - before) });
				}
				++i;
				while (i < string.length() && string[i] == by_what) {
					result.push_back(L"");
					++i;
				}
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.emplace_back(std::wstring{ string.substr(before) });
		}
		if (string.back() == by_what) {
			result.push_back(L"");
		}
		return result;
	}
	std::vector<std::string> qpl::string_split_digit_alpha(const std::string_view& string) {
		if (string.empty()) {
			return {};
		}
		std::vector<std::string> result;

		bool alpha = qpl::is_character_alpha(string.front());
		qpl::size before = 0;
		for (qpl::size i = 0u; i < string.length(); ) {
			if (alpha && !qpl::is_character_alpha(string[i])) {
				if (i - before) {
					result.emplace_back(std::string{ string.substr(before, i - before) });
				}
				while (i < string.length() && (!qpl::is_character_alpha(string[i]) && !qpl::is_character_digit_or_dot(string[i]))) {
					++i;
				}

				alpha = qpl::is_character_alpha(string[i]);
				before = i;
			}
			else if (!alpha && !qpl::is_character_digit_or_dot(string[i])) {
				if (i - before) {
					result.emplace_back(std::string{ string.substr(before, i - before) });
				}
				while (i < string.length() && (!qpl::is_character_alpha(string[i]) && !qpl::is_character_digit_or_dot(string[i]))) {
					++i;
				}

				alpha = qpl::is_character_alpha(string[i]);
				before = i;
			}
			else {
				++i;
			}
		}
		if (before != string.length()) {
			result.emplace_back(std::string{ string.substr(before) });
		}
		return result;
	}

	std::vector<std::string> qpl::string_split_every(const std::string& string, qpl::size n) {
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

	qpl::size qpl::string_levenshtein_distance(const std::string_view& a, const std::string_view& b) {
		std::vector<std::vector<qpl::size>> matrix(a.length() + 1, std::vector<qpl::size>(b.length() + 1, 0));

		qpl::size cost{};
		for (qpl::size i = 0u; i <= a.length(); i++) {
			matrix[i][0] = i;
		}
		for (qpl::size j = 0; j <= b.length(); j++) {
			matrix[0][j] = j;
		}
		for (qpl::size i = 1; i <= a.length(); i++) {
			for (qpl::size j = 1; j <= b.length(); j++) {
				if (a[i - 1] == b[j - 1]) {
					cost = 0;
				}
				else {
					cost = 1;
				}
				matrix[i][j] = std::min(matrix[i - 1][j] + 1, std::min(matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost));

				if ((i > 1) && (j > 1) && (a[i - 1] == b[j - 2]) && (a[i - 2] == b[j - 1])) {
					matrix[i][j] = std::min(matrix[i][j], matrix[i - 2][j - 2] + cost);
				}
			}
		}
		return matrix[a.length()][b.length()];
	}

	qpl::size qpl::string_levenshtein_distance(const std::wstring_view& a, const std::wstring_view& b) {
		std::vector<std::vector<qpl::size>> matrix(a.length() + 1, std::vector<qpl::size>(b.length() + 1, 0));

		qpl::size cost{};
		for (qpl::size i = 0u; i <= a.length(); i++) {
			matrix[i][0] = i;
		}
		for (qpl::size j = 0; j <= b.length(); j++) {
			matrix[0][j] = j;
		}
		for (qpl::size i = 1; i <= a.length(); i++) {
			for (qpl::size j = 1; j <= b.length(); j++) {
				if (a[i - 1] == b[j - 1]) {
					cost = 0;
				}
				else {
					cost = 1;
				}
				matrix[i][j] = std::min(matrix[i - 1][j] + 1, std::min(matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost));

				if ((i > 1) && (j > 1) && (a[i - 1] == b[j - 2]) && (a[i - 2] == b[j - 1])) {
					matrix[i][j] = std::min(matrix[i][j], matrix[i - 2][j - 2] + cost);
				}
			}
		}
		return matrix[a.length()][b.length()];
	}
	void qpl::string_trim_whitespace(std::wstring& string) {
		qpl::size index = 0u;
		while (index < string.length() && qpl::is_character_whitespace(qpl::u8_cast(string[index]))) {
			++index;
		}
		if (index < string.length()) {
			string = string.substr(index);
		}
		else {
			string.clear();
		}
	}
	void qpl::string_trim_whitespace(std::string& string) {
		qpl::size index = 0u;
		while (index < string.length() && qpl::is_character_whitespace(qpl::u8_cast(string[index]))) {
			++index;
		}
		if (index < string.length()) {
			string = string.substr(index);
		}
		else {
			string.clear();
		}
	}
	bool qpl::string_starts_with_ignore_case(const std::string_view& a, const std::string_view& b) {
		if (b.length() > a.length()) {
			return false;
		}
		for (qpl::size i = 0u; i < b.length(); ++i) {
			if (std::tolower(a[i]) != std::tolower(b[i])) {
				return false;
			}
		}
		return true;
	}
	bool qpl::string_starts_with(const std::string_view& a, const std::string_view& b) {
		if (b.length() > a.length()) {
			return false;
		}
		for (qpl::size i = 0u; i < b.length(); ++i) {
			if (a[i] != b[i]) {
				return false;
			}
		}
		return true;
	}
	bool qpl::string_starts_with_ignore_case(const std::wstring_view& a, const std::wstring_view& b) {
		if (b.length() > a.length()) {
			return false;
		}
		for (qpl::size i = 0u; i < b.length(); ++i) {
			if (std::tolower(a[i]) != std::tolower(b[i])) {
				return false;
			}
		}
		return true;
	}
	bool qpl::string_starts_with(const std::wstring_view& a, const std::wstring_view& b) {
		if (b.length() > a.length()) {
			return false;
		}
		for (qpl::size i = 0u; i < b.length(); ++i) {
			if (a[i] != b[i]) {
				return false;
			}
		}
		return true;
	}
	bool qpl::string_ends_with_ignore_case(const std::string_view& a, const std::string_view& b) {
		if (b.length() > a.length()) {
			return false;
		}
		auto start = a.length() - b.length();
		for (qpl::size i = start; i < a.length(); ++i) {
			if (std::tolower(a[i]) != std::tolower(b[i - start])) {
				return false;
			}
		}
		return true;
	}
	bool qpl::string_ends_with(const std::string_view& a, const std::string_view& b) {
		if (b.length() > a.length()) {
			return false;
		}
		auto start = a.length() - b.length();
		for (qpl::size i = start; i < a.length(); ++i) {
			if (std::tolower(a[i]) != std::tolower(b[i - start])) {
				return false;
			}
		}
		return true;
	}
	bool qpl::string_ends_with_ignore_case(const std::wstring_view& a, const std::wstring_view& b) {
		if (b.length() > a.length()) {
			return false;
		}
		auto start = a.length() - b.length();
		for (qpl::size i = start; i < a.length(); ++i) {
			if (std::tolower(a[i]) != std::tolower(b[i - start])) {
				return false;
			}
		}
		return true;
	}
	bool qpl::string_ends_with(const std::wstring_view& a, const std::wstring_view& b) {
		if (b.length() > a.length()) {
			return false;
		}
		auto start = a.length() - b.length();
		for (qpl::size i = start; i < a.length(); ++i) {
			if (std::tolower(a[i]) != std::tolower(b[i - start])) {
				return false;
			}
		}
		return true;
	}



	std::vector<qpl::size> qpl::best_string_matches_at_start_or_contains(const std::vector<std::string>& list, const std::string& search) {
		std::vector<qpl::size> check_result;
		for (qpl::size i = 0u; i < list.size(); ++i) {
			if (qpl::string_starts_with(list[i], search)) {
				check_result.push_back(i);
			}
		}
		if (check_result.size() == 1u) {
			return check_result;
		}
		for (qpl::size i = 0u; i < list.size(); ++i) {
			if (qpl::string_contains(list[i], search) && !qpl::find(check_result, i)) {
				check_result.push_back(i);
			}
		}
		if (check_result.size() == 1u) {
			return check_result;
		}

		check_result.clear();
		for (qpl::size i = 0u; i < list.size(); ++i) {
			if (qpl::string_starts_with_ignore_case(list[i], search) && !qpl::find(check_result, i)) {
				check_result.push_back(i);
			}
		}
		if (check_result.size() == 1u) {
			return check_result;
		}
		for (qpl::size i = 0u; i < list.size(); ++i) {
			if (qpl::string_contains_ignore_case(list[i], search) && !qpl::find(check_result, i)) {
				check_result.push_back(i);
			}
		}
		if (check_result.size() == 1u) {
			return check_result;
		}
		return check_result;
	}
	std::vector<qpl::size> qpl::best_string_matches_indices(const std::vector<std::string>& list, const std::string& search) {
		std::vector<qpl::size> result;
		std::vector<std::pair<qpl::size, qpl::size>> score(list.size());
		for (qpl::size i = 0u; i < score.size(); ++i) {
			score[i] = std::make_pair(qpl::string_levenshtein_distance(list[i], search), i);
		}

		qpl::sort(score, [](auto a, auto b) { return a.first < b.first; });

		qpl::size best_score = score[0].first;
		for (qpl::size i = 0u; i < score.size(); ++i) {
			if (score[i].first == best_score) {
				result.push_back(score[i].second);
			}
		}
		return result;
	}
	std::vector<qpl::size> qpl::best_string_matches_check_start_contains_indices(const std::vector<std::string>& list, const std::string& search) {
		std::vector<qpl::size> result;

		auto test = qpl::best_string_matches_at_start_or_contains(list, search);
		if (!test.empty()) {
			return test;
		}

		std::vector<std::pair<qpl::size, qpl::size>> score(list.size());
		for (qpl::size i = 0u; i < score.size(); ++i) {
			score[i] = std::make_pair(qpl::string_levenshtein_distance(list[i], search), i);
		}

		qpl::sort(score, [](auto a, auto b) { return a.first < b.first; });

		qpl::size best_score = score[0].first;
		for (qpl::size i = 0u; i < score.size(); ++i) {
			if (score[i].first == best_score) {
				result.push_back(score[i].second);
			}
		}
		return result;
	}
	std::vector<std::string> qpl::best_string_matches(const std::vector<std::string>& list, const std::string& search) {
		std::vector<std::string> result;
		std::vector<std::pair<qpl::size, std::string>> score(list.size());
		for (qpl::size i = 0u; i < score.size(); ++i) {
			score[i] = std::make_pair(qpl::string_levenshtein_distance(list[i], search), list[i]);
		}

		qpl::sort(score, [](auto a, auto b) { return a.first < b.first; });

		qpl::size best_score = score[0].first;
		for (qpl::size i = 0u; i < score.size(); ++i) {
			if (score[i].first == best_score) {
				result.push_back(score[i].second);
			}
		}
		return result;
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
	bool qpl::is_string_number(std::wstring string) {
		std::wistringstream iss(string);
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



	std::string qpl::get_string_permutation(const std::string& symbols, qpl::size permutation_size, qpl::size position) {
		std::string permutation;
		permutation.resize(permutation_size);

		auto base = symbols.length();

		for (qpl::size i = 0u; i < permutation_size; ++i) {

			auto index = (position / qpl::int_pow(base, i)) % base;
			permutation[permutation_size - i - 1] = symbols[index];
		}
		return permutation;
	}
	std::vector<std::string> qpl::list_string_permutations(const std::string& symbols, qpl::size permutation_size) {

		auto result_size = qpl::size(std::pow(symbols.length(), permutation_size));
		std::vector<std::string> result(result_size);

		for (qpl::size i = 0u; i < result.size(); ++i) {
			result[i] = get_string_permutation(symbols, permutation_size, i);
		}

		return result;
	}
	std::vector<std::string> qpl::list_unique_string_permutations(const std::string& symbols, qpl::size permutation_size) {

		std::vector<std::string> result;

		std::set<qpl::size> seen;
		for (std::size_t i = 0u;; ++i) {

			std::string permutation;
			permutation.resize(permutation_size);

			auto base = symbols.length();

			bool unique = true;
			bool last_permutation = true;
			seen.clear();

			qpl::size index_before = 0u;
			for (qpl::size j = 0u; j < permutation_size; ++j) {

				auto index = (i / qpl::int_pow(base, j)) % base;

				if (seen.find(index) != seen.cend()) {
					unique = false;
					break;
				}
				seen.insert(index);

				if (index < index_before) {
					last_permutation = false;
				}
				index_before = index;
				permutation[permutation_size - j - 1] = symbols[index];
			}
			if (unique) {
				result.push_back(permutation);
				if (last_permutation) {
					break;
				}
			}
		}

		return result;
	}
	void qpl::collection_string::set_string(const std::string& string) {
		this->string = string;
		this->index = 0u;
	}
	bool qpl::collection_string::set_string_and_read(const std::string& string) {
		this->set_string(string);
		return this->read_info();
	}
	bool qpl::collection_string::read_info() {
		if (this->string.length() < qpl::bytes_in_type<qpl::u32>()) {
			return false;
		}
		qpl::size position = 0;
		qpl::u32 size_size;
		this->index = 0u;
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
	std::string qpl::collection_string::get_next_string() const {
		return this->get_string(this->index++);
	}
	std::string_view qpl::collection_string::get_next_string_sv() const {
		return this->get_string_sv(this->index++);
	}
	std::string qpl::collection_string::get_string() const {
		return this->string;
	}
	std::string_view qpl::collection_string::get_string_sv() const {
		return this->string;
	}
	std::string qpl::collection_string::get_string(qpl::size index) const {
		if (index >= this->sizes.size()) {
			throw std::exception(qpl::to_string("qpl::string_collection::get_string(", index, "): size is only ", this->sizes.size()).c_str());
		}
		return std::string(this->string.begin() + this->sizes[index].first, this->string.begin() + this->sizes[index].second);
	}
	std::string_view qpl::collection_string::get_string_sv(qpl::size index) const {
		if (index >= this->sizes.size()) {
			throw std::exception(qpl::to_string("qpl::string_collection::get_string_sv(", index, "): size is only ", this->sizes.size()).c_str());
		}
		return std::string_view(this->string.begin() + this->sizes[index].first, this->string.begin() + this->sizes[index].second);
	}
	std::string qpl::collection_string::get_finalized_string() {
		this->finalize();
		return this->get_string();
	}
	std::string_view qpl::collection_string::get_finalized_string_sv() {
		this->finalize();
		return this->get_string_sv();
	}
	void qpl::collection_string::add_string(const std::string& string) {
		auto back = this->string.length();
		this->string.append(string);
		this->sizes.push_back({ qpl::size_cast(back), qpl::size_cast(back + string.length()) });
	}
	void qpl::collection_string::finalize() {
		auto header_size = qpl::bytes_in_type<qpl::u32>() + (this->sizes.size() * 2 * qpl::bytes_in_type<qpl::size>());
		std::ostringstream stream;

		qpl::u32 size_size = qpl::u32_cast(this->sizes.size());
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

	void set_language_locale(std::string local) {
		std::locale::global(std::locale(local));
	}

}