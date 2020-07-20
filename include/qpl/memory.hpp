#ifndef QPL_MEMORY_HPP
#define QPL_MEMORY_HPP
#pragma once

#include <qpl/algorithm.hpp>
#include <qpl/qpldeclspec.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/vardef.hpp>
#include <array>

namespace qpl {
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline C string_to_container_memory(const std::string& data) {
		C result;
		result.resize((data.size() - 1) / qpl::bytes_in_type<qpl::container_subtype<C>>() + 1);
		memcpy(result.data(), data.data(), data.size());
		return result;
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline void string_to_container_memory(const std::string& source, C& dest) {
		dest.resize((source.size() - 1) / qpl::bytes_in_type<qpl::container_subtype<C>>() + 1);
		memcpy(dest.data(), source.data(), source.size());
	}


	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline std::string container_memory_to_string(const C& data) {
		std::string result;
		result.resize(data.size() * sizeof(qpl::container_subtype<C>));
		memcpy(result.data(), data.data(), result.size());
		return result;
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline void container_memory_to_string(const C& data, std::string& destination) {
		destination.resize(data.size() * sizeof(qpl::container_subtype<C>));
		memcpy(destination.data(), data.data(), destination.size());
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline void string_to_vector_memory(const std::string& data, C& destination) {
		destination.resize((data.size() - 1) / sizeof(qpl::container_subtype<C>) + 1);
		memcpy(destination.data(), data.data(), data.size());
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline void add_string_to_container_memory(const std::string& data, C& destination) {
		auto size = destination.size();
		destination.resize(size + (data.size() - 1) / sizeof(qpl::container_subtype<C>) + 1);
		memcpy(destination.data() + size, data.data(), data.size());
	}

	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr std::wstring container_memory_to_wstring(const C& data) {
		std::wstring result;
		result.resize(data.size() * sizeof(qpl::container_subtype<C>) / (sizeof(wchar_t) / sizeof(char)));
		memcpy(result.data(), data.data(), result.size() * (sizeof(wchar_t) / sizeof(char)));
		return result;
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr void container_memory_to_wstring(const C& data, std::wstring& destination) {
		destination.resize(data.size() * sizeof(qpl::container_subtype<C>));
		memcpy(destination.data(), data.data(), destination.size() * (sizeof(wchar_t) / sizeof(char)));
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr C wstring_to_container_memory(const std::wstring& data) {
		C result;
		result.resize(((data.size() * (sizeof(wchar_t) / sizeof(char)) - 1) / sizeof(qpl::container_subtype<C>) + 1));
		memcpy(result.data(), data.data(), data.size() * (sizeof(wchar_t) / sizeof(char)));
		return result;
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr void wstring_to_container_memory(const std::wstring& data, C& destination) {
		destination.resize(((data.size() * (sizeof(wchar_t) / sizeof(char)) - 1) / sizeof(qpl::container_subtype<C>) + 1));
		memcpy(destination.data(), data.data(), data.size() * (sizeof(wchar_t) / sizeof(char)));
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr void add_wstring_to_container_memory(const std::wstring& data, C& destination) {
		auto size = destination.size();
		destination.resize(size + ((data.size() * (sizeof(wchar_t) / sizeof(char)) - 1) / sizeof(qpl::container_subtype<C>)) + 1);
		memcpy(destination.data() + size, data.data(), data.size() * (sizeof(wchar_t) / sizeof(char)));
	}

	template<typename T, typename U, QPLCONCEPT(qpl::bytes_in_type<T>() == qpl::bytes_in_type<U>())>
	constexpr inline void copy_memory(const T& source, U& destination) {
		memcpy(&destination, &source, qpl::bytes_in_type<T>());
	}
	template<typename T, typename U, QPLCONCEPT(qpl::bytes_in_type<T>() != qpl::bytes_in_type<U>())>
	constexpr inline void copy_memory(const T& source, U& destination) {
		memcpy(&destination, &source, qpl::min(qpl::bytes_in_type<T>(), qpl::bytes_in_type<U>()));
	}
	template<typename C>
	constexpr inline void clear_array(C& destination) {
		memset(&destination, 0, sizeof(C));
	}
	template<typename C>
	constexpr inline void clear_array_offset(C& destination, qpl::size offset = 0u) {
		memset(reinterpret_cast<std::byte*>(&destination) + offset * qpl::bytes_in_type<qpl::container_subtype<C>>(), 0, sizeof(C) - offset * qpl::bytes_in_type<qpl::container_subtype<C>>());
	}
	template<typename U, typename T, QPLCONCEPT(qpl::bytes_in_type<T>() == qpl::bytes_in_type<U>())>
	constexpr inline U convert_memory(const T& source) {
		U destination;
		memcpy(&destination, &source, qpl::bytes_in_type<T>());
		return destination;
	}

	template<typename T>
	constexpr inline std::array<qpl::u8, qpl::bytes_in_type<T>()> memory_to_bytes(const T& source) {
		std::array<qpl::u8, qpl::bytes_in_type<T>()> result;
		memcpy(result.data(), &source, qpl::bytes_in_type<T>());
		return result;
	}

	QPLDLL void print_character_bool_table(std::string_view characters);



	template<qpl::size exponent_bytes, qpl::size mantissa_bytes>
	struct float_memory {
		constexpr float_memory(std::array<qpl::u32, exponent_bytes> exponent, std::array<qpl::u32, mantissa_bytes> mantissa, bool sign) : exponent(exponent), mantissa(mantissa), sign(sign) {

		}

		std::array<qpl::u32, exponent_bytes> exponent;
		std::array<qpl::u32, mantissa_bytes> mantissa;
		bool sign;
	};
}

#endif