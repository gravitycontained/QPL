#ifndef QPL_SPAN_HPP
#define QPL_SPAN_HPP
#pragma once

#include <qpl/vardef.hpp>
#include <qpl/qpldeclspec.hpp>
#include <qpl/iterator.hpp>
#include <qpl/type_traits.hpp>
#include <string>
#include <iostream>

#ifdef QPL_CPP17

namespace qpl {

	template<typename T>
	struct span_static_data {
		T* ptr = nullptr;
	};
	template<typename T>
	struct span_dynamic_data {
		T* ptr = nullptr;
		qpl::size size = 0u;
	};

	template<typename T, qpl::size N = qpl::size_max>
	struct span {
		using data_type = qpl::conditional<qpl::if_true<N == qpl::size_max>, span_dynamic_data<T>, qpl::default_type, span_static_data<T>>;
		data_type span_data;

		constexpr static bool is_dynamic() {
			return N == qpl::size_max;
		}

		constexpr span() noexcept {

		}
		constexpr span(T* begin, qpl::size size)noexcept {
			this->span_data.ptr = begin;
			if constexpr (is_dynamic()) {
				this->span_data.size = size;
			}
		}
		constexpr span(T* begin, T* end) noexcept {
			this->span_data.ptr = begin;
			if constexpr (is_dynamic()) {
				this->span_data.size = std::distance(begin, end);
			}
		}
		template<typename It>
		constexpr span(It begin, It end) noexcept {
			this->span_data.ptr = &(*begin);
			if constexpr (is_dynamic()) {
				this->span_data.size = std::distance(&(*begin), &(*(end - 1))) + 1;
			}
		}

		constexpr T& operator[](qpl::size index) {
			return this->span_data.ptr[index];
		}
		constexpr const T& operator[](qpl::size index) const {
			return this->span_data.ptr[index];
		}
		constexpr T& at(qpl::size index) {
			if (index >= this->size()) {
				throw std::exception(std::string("qpl::span: at(") + std::to_string(index) + std::string("): index out of bounds. Size = ") + std::to_string(this->size()));
			}
			return this->span_data.ptr[index];
		}
		constexpr const T& at(qpl::size index) const {
			if (index >= this->size()) {
				throw std::exception(std::string("qpl::span: at(") + std::to_string(index) + std::string("): index out of bounds. Size = ") + std::to_string(this->size()));
			}
			return this->span_data.ptr[index];
		}


		constexpr T& front() {
			return this->span_data.ptr[0u];
		}
		constexpr const T& front() const {
			return this->span_data.ptr[0u];
		}
		constexpr T& back() {
			return this->span_data.ptr[this->size() - 1];
		}
		constexpr const T& back() const {
			return this->span_data.ptr[this->size() - 1];
		}

		constexpr auto begin() {
			return qpl::iterator<T>(this->span_data.ptr);
		}
		constexpr auto begin() const {
			return qpl::const_iterator<T>(this->span_data.ptr);
		}
		constexpr auto rbegin() {
			return qpl::reverse_iterator<T>(this->span_data.ptr + this->size() - 1);
		}
		constexpr auto rbegin() const {
			return qpl::const_reverse_iterator<T>(this->span_data.ptr + this->size() - 1);
		}
		constexpr auto cbegin() const {
			return qpl::const_iterator<T>(this->span_data.ptr);
		}
		constexpr auto crbegin() const {
			return qpl::const_reverse_iterator<T>(this->span_data.ptr + this->size() - 1);
		}


		constexpr auto end() {
			return qpl::iterator<T>(this->span_data.ptr + this->size());
		}
		constexpr auto end() const {
			return qpl::const_iterator<T>(this->span_data.ptr + this->size());
		}
		constexpr auto rend() {
			return qpl::reverse_iterator<T>(this->span_data.ptr - 1);
		}
		constexpr auto rend() const {
			return qpl::const_reverse_iterator<T>(this->span_data.ptr - 1);
		}
		constexpr auto cend() const {
			return qpl::const_iterator<T>(this->span_data.ptr + this->size());
		}
		constexpr auto crend() const {
			return qpl::const_reverse_iterator<T>(this->span_data.ptr - 1);
		}

		constexpr bool empty() const {
			if constexpr (is_dynamic()) {
				return this->span_data.size == 0u;
			}
			else {
				return N == 0u;
			}
		}
		constexpr qpl::size size() const {
			if constexpr (is_dynamic()) {
				return this->span_data.size;
			}
			else {
				return N;
			}
		}

		constexpr T* data() {
			return this->span_data.ptr;
		}
		constexpr const T* data() const {
			return this->span_data.ptr;
		}
	};

	template<typename T>
	struct basic_string_view {
		const T* ptr = nullptr;
		qpl::size str_size = 0u;

		constexpr basic_string_view() noexcept {

		}
		constexpr basic_string_view(const T* begin, qpl::size size)noexcept {
			this->ptr = begin;
			this->str_size = size;
		}
		constexpr basic_string_view(const T* begin, const T* end) noexcept {
			this->ptr = begin;
			this->str_size = std::distance(begin, end);
		}
		constexpr basic_string_view(const T* cstring) noexcept {
			this->ptr = cstring;
			if constexpr (qpl::is_standard_string_type<T>()) {
				this->str_size = std::strlen(cstring);
			}
			else if constexpr (qpl::is_wstring_type<T>()) {
				this->str_size = std::wcslen(cstring);
			}
		}
		constexpr basic_string_view(const std::string& string) {
			if constexpr (qpl::is_standard_string_type<T>()) {
				this->ptr = string.data();
				this->str_size = string.size();
			}
			else {
				static_assert("incompatible string type");
			}
		}
		constexpr basic_string_view(const std::wstring& string) {
			if constexpr (qpl::is_wstring_type<T>()) {
				this->ptr = string.data();
				this->str_size = string.size();
			}
			else {
				static_assert("incompatible string type");
			}
		}

		constexpr basic_string_view substr(qpl::size off, qpl::size size = qpl::size_max) const {
			auto copy = *this;
			copy.ptr += off;
			if (size == qpl::size_max) {
				size = copy.str_size - off;
			}
			size = (copy.str_size - off) < size ? copy.str_size - off : size;
			copy.str_size = size;
			return copy;
		}

		constexpr const T& front() const {
			return this->ptr[0u];
		}
		constexpr const T& back() const {
			return this->ptr[this->size() - 1];
		}

		constexpr std::string make_string() const {
			std::string s(this->ptr, this->str_size);
			return s;
		}
		constexpr operator std::string() const {
			return this->make_string();
		}


		constexpr const T& operator[](qpl::size index) const {
			return this->ptr[index];
		}
		constexpr const T& at(qpl::size index) const {
			if (index >= this->size()) {
				throw std::exception(std::string("qpl::basic_string_view: at(") + std::to_string(index) + std::string("): index out of bounds. Size = ") + std::to_string(this->size()));
			}
			return this->ptr[index];
		}

		constexpr auto begin() const {
			return qpl::const_iterator<T>(this->ptr);
		}
		constexpr auto rbegin() const {
			return qpl::const_reverse_iterator<T>(this->ptr + this->size() - 1);
		}
		constexpr auto cbegin() const {
			return qpl::const_iterator<T>(this->ptr);
		}
		constexpr auto crbegin() const {
			return qpl::const_reverse_iterator<T>(this->ptr + this->size() - 1);
		}

		constexpr auto end() const {
			return qpl::const_iterator<T>(this->ptr + this->size());
		}
		constexpr auto rend() const {
			return qpl::const_reverse_iterator<T>(this->ptr - 1);
		}
		constexpr auto cend() const {
			return qpl::const_iterator<T>(this->ptr + this->size());
		}
		constexpr auto crend() const {
			return qpl::const_reverse_iterator<T>(this->ptr - 1);
		}

		constexpr bool operator==(const T* str) const {
			return *this == basic_string_view(str);
		}
		constexpr bool operator!=(const T* str) const {
			return *this != basic_string_view(str);
		}
		constexpr bool operator==(const basic_string_view& other) const {
			return this->ptr == other.ptr && this->str_size == other.str_size;
		}
		constexpr bool operator!=(const basic_string_view& other) const {
			return !(*this == other);
		}

		constexpr qpl::size size() const {
			return this->str_size;
		}
		constexpr qpl::size length() const {
			return this->str_size - 1;
		}
		constexpr bool empty() const {
			return this->str_size == 0u;
		}

		constexpr const T* data() const {
			return this->ptr;
		}


		template<typename T1, typename T2>
		friend std::ostream& operator<<(std::basic_ostream<T1>& os, const qpl::basic_string_view<T2>& view);
	};

	using string_view = basic_string_view<char>;
	using wstring_view = basic_string_view<wchar_t>;


	template<typename T1, typename T2>
	std::ostream& operator<<(std::basic_ostream<T1>& os, const qpl::basic_string_view<T2>& view) {
		os.write(view.data(), view.size());
		return os;
	}
}

#else

#include <string_view>
#include <span>

namespace qpl {
	template<typename T, qpl::size N = qpl::size_max>
	using span = std::span<T, N>;

	using string_view = std::string_view;
}
#endif

#endif