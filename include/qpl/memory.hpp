#ifndef QPL_MEMORY_HPP
#define QPL_MEMORY_HPP
#pragma once

#include <qpl/algorithm.hpp>
#include <qpl/span.hpp>
#include <qpl/string.hpp>
#include <qpl/qpldeclspec.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/system.hpp>
#include <qpl/vardef.hpp>
#include <array>



namespace qpl {
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline C string_to_container_memory(const std::string& data) {
		C result;
		if (data.empty()) {
			return C;
		}
		result.resize((data.size() - 1) / qpl::bytes_in_type<qpl::container_subtype<C>>() + 1);
		memcpy(result.data(), data.data(), data.size());
		return result;
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline void string_to_container_memory(const std::string& source, C& dest) {
		if (source.empty()) {
			return;
		}
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
		if (data.empty()) {
			return;
		}
		destination.resize((data.size() - 1) / sizeof(qpl::container_subtype<C>) + 1);
		memcpy(destination.data(), data.data(), data.size());
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline void string_to_vector_memory(const qpl::string_view& data, C& destination) {
		if (data.empty()) {
			return;
		}
		destination.resize((data.size() - 1) / sizeof(qpl::container_subtype<C>) + 1);
		memcpy(destination.data(), data.data(), data.size());
	}
	template<typename C, QPLCONCEPT(qpl::is_container<C>())>
	constexpr inline void add_string_to_container_memory(const std::string& data, C& destination) {
		if (data.empty()) {
			return;
		}
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
	template<typename T>
	constexpr inline std::array<qpl::u64, qpl::approximate_multiple_up(qpl::bytes_in_type<T>(), qpl::bytes_in_type<qpl::u64>())> memory_to_u64_array(const T& source) {
		std::array<qpl::u64, qpl::approximate_multiple_up(qpl::bytes_in_type<T>(), qpl::bytes_in_type<qpl::u64>())> result;
		result.fill(qpl::u64{ 0u });
		memcpy(result.data(), &source, qpl::bytes_in_type<T>());
		return result;
	}


	QPLDLL void print_character_bool_table(qpl::string_view characters);

	template<qpl::size exponent_bytes, qpl::size mantissa_bytes>
	struct float_memory {
		constexpr float_memory(std::array<qpl::u32, exponent_bytes> exponent, std::array<qpl::u32, mantissa_bytes> mantissa, bool sign) : exponent(exponent), mantissa(mantissa), sign(sign) {

		}

		std::array<qpl::u32, exponent_bytes> exponent;
		std::array<qpl::u32, mantissa_bytes> mantissa;
		bool sign;
	};



	template<typename T, qpl::size N = 0>
	struct circular_array {

		circular_array() {
			this->index = 0u;
			this->rotation_finished = false;
			if constexpr (this->is_array() && qpl::is_arithmetic<T>()) {
				this->data.fill(0u);
			}
		}

		void clear() {
			if constexpr (is_array() && qpl::is_arithmetic<T>()) {
				this->data.fill(0u);
			}
			else {
				std::fill(this->data.begin(), this->data.end(), 0u);
			}
			this->index = 0u;
			this->rotation_finished = false;
		}


		void add(T value) {
			if (this->rotation_finished) {
				this->data[this->index] = value;
				++this->index;

				if (this->index == this->size()) {
					this->index = 0u;
				}
			}
			else {
				this->data[this->index] = value;
				++this->index;

				if (this->index == this->size()) {
					this->index = 0u;
					this->rotation_finished = true;
				}
			}
		}

		template<typename R = qpl::conditional<qpl::if_true<qpl::is_floating_point<T>()>, T, qpl::f64>>
		R get_average() const {
			R sum = 0;
			if (this->rotation_finished) {
				for (auto& i : this->data) {
					sum += i;
				}
				return sum / this->size();
			}
			else {
				for (qpl::u32 i = 0u; i < this->index; ++i) {
					sum += this->data[i];
				}
				return sum / this->index;
			}
		} 
		T get_min() const {
			T min = qpl::type_max<T>();
			if (this->rotation_finished) {

				for (auto& i : this->data) {
					if (i < min) {
						min = i;
					}
				}
				return min;
			}
			else {
				for (qpl::u32 i = 0u; i < this->index; ++i) {
					if (this->data[i] < min) {
						min = this->data[i];
					}
				}
				return min;
			}
		}

		T get_max() const {
			T max = qpl::type_min<T>();
			if (this->rotation_finished) {

				for (auto& i : this->data) {
					if (i > max) {
						max = i;
					}
				}
				return max;
			}
			else {
				for (qpl::u32 i = 0u; i < this->index; ++i) {
					if (this->data[i] > max) {
						max = this->data[i];
					}
				}
				return max;
			}
		}
		T get_sum() const {
			T sum = 0;
			if (this->rotation_finished) {
				for (auto& i : this->data) {
					sum += i;
				}
				return sum;
			}
			else {
				for (qpl::u32 i = 0u; i < this->index; ++i) {
					sum += this->data[i];
				}
				return sum;
			}
		}
		T get_sum_of_last_n(qpl::u32 last_n) const {
			if (last_n == this->size()) {
				return this->get_sum();
			}
			T sum = 0;
			if (this->rotation_finished) {
				qpl::u32 start = 0u;
				if (last_n > (this->index - 1)) {
					start = this->size() - 1 - (last_n - (this->index - 1));
				}
				else {
					start = this->index - 1 - last_n;
				}
				for (qpl::u32 i = 0u; i < last_n; ++i) {
					sum += this->data[(start + i) % this->size()];
				}
				return sum;
			}
			else {
				for (qpl::u32 i = this->index - 1 - last_n; i < this->index; ++i) {
					sum += this->data[i];
				}
				return sum;
			}
		}

		std::pair<T, T> get_min_max() const {
			std::pair<T, T> min_max = std::make_pair(qpl::type_max<T>(), qpl::type_min<T>());
			if (this->rotation_finished) {

				for (auto& i : this->data) {
					if (i < min_max.first) {
						min_max.first = i;
					}
					if (i > min_max.second) {
						min_max.second = i;
					}
				}
				return min_max;
			}
			else {
				for (qpl::u32 i = 0u; i < this->index; ++i) {
					if (this->data[i] < min_max.first) {
						min_max.first = this->data[i];
					}
					if (this->data[i] > min_max.second) {
						min_max.second = this->data[i];
					}
				}
				return min_max;
			}
		}


		void resize(qpl::u32 size) {
			if constexpr (is_vector()) {
				this->data.resize(size);
				std::fill(this->data.begin(), this->data.end(), 0u);
				this->index = 0u;
				this->rotation_finished = false;
			}
		}


		constexpr static bool is_vector() {
			return N == qpl::size{ 0u };
		}
		constexpr static bool is_array() {
			return N != qpl::size{ 0 };
		}

		T& operator[](qpl::size index) {
			return this->data[index];
		}
		const T& operator[](qpl::size index) const {
			return this->data[index];
		}

		T& get() {
			if (this->rotation_finished) {
				auto i = this->index;
				if (i == 0u) {
					i = this->size() - 1u;
				}
				else {
					--i;
				}
				return this->data[i];
			}
			else {
				auto i = this->index;
				if (i == 0u) {
					return T{};
				}
				else {
					return this->data[i - 1];
				}
			}
		}
		const T& get() const {
			if (this->rotation_finished) {
				auto i = this->index;
				if (i == 0u) {
					i = this->size() - 1u;
				}
				else {
					--i;
				}
				return this->data[i];
			}
			else {
				auto i = this->index;
				if (i == 0u) {
					return T{};
				}
				else {
					return this->data[i - 1];
				}
			}
		}

		T& last() {
			if (this->rotation_finished) {
				return this->data[this->index];
			}
			else {
				auto i = this->index;
				if (i == 0u) {
					return this->data[0u];
				}
				else {
					return this->data[0u];
				}
			}
		}
		const T& last() const {
			if (this->rotation_finished) {
				return this->data[this->index];
			}
			else {
				auto i = this->index;
				if (i == 0u) {
					return T{};
				}
				else {
					return this->data[0u];
				}
			}
		}

		constexpr qpl::size size() const {
			return this->data.size();
		}
		qpl::size used_size() const {
			if (this->rotation_finished) {
				return this->size();
			}
			else {
				return this->index;
			}
		}

		bool rotation_finished;
		qpl::size index;
		qpl::conditional<qpl::if_true<N == 0>, std::vector<T>, std::array<T, N>> data;
	};

	namespace detail {
#ifdef QPL_NO_ARRAY_BOUNDARY_CHECK
		constexpr bool array_boundary_check = false;
#else
		constexpr bool array_boundary_check = true;
#endif
#ifdef QPL_NO_VECTOR_BOUNDARY_CHECK
		constexpr bool vector_boundary_check = false;
#else
		constexpr bool vector_boundary_check = true;
#endif
	}

	template<typename T, qpl::size N, bool BOUNDARY_CHECK = detail::array_boundary_check>
	struct array {
	private:
		constexpr void index_check(qpl::size index, bool at) const {
			if (index >= this->size()) {
				std::ostringstream stream;
				stream << qpl::to_string("qpl::array<", qpl::type_name<T>(), ", ", N, ">", at ? ".at()" : "::operator[]", " : index is ", index);
				qpl::i32 convert = qpl::i32_cast(index);
				if (convert < 0) {
					stream << qpl::to_string(" (= ", convert, ") ");
				}
				qpl::println(stream.str());
				throw std::exception(stream.str().c_str());
			}
		}

	public:
		using array_type = std::array<T, N>;

		array_type memory;

		constexpr array() : memory() {

		}

		constexpr array(const array& other) : memory() {
			*this = other;
		}
		constexpr array(const std::array<T, N>& other) : memory() {
			*this = other;
		}
		constexpr array(const std::initializer_list<T>& list) : memory() {
			*this = list;
		}
		template<typename First, typename... Args>
		constexpr array(First first, Args&&... args) : memory() {
			this->memory = qpl::type_cast<T>(first, args...);
		}

		constexpr array& operator=(const array& other) {
			this->memory = other.memory;
			return *this;
		}
		constexpr array& operator=(const std::array<T, N>& other) {
			this->memory = other;
			return *this;
		}

		constexpr array& operator=(const std::initializer_list<T>& list) {
			for (qpl::u32 i = 0u; i < qpl::min(list.size(), this->size()); ++i) {
				this->memory[i] = *(list.begin() + i);
			}
			return *this;
		}

		constexpr operator std::array<T, N>() {
			return this->memory;
		}

		constexpr void fill(const T& value) {
			this->memory.fill(value);
		}

		constexpr T& operator[](qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, false);
			}
			return this->memory[index];
		}
		constexpr const T& operator[](qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, false);
			}
			return this->memory[index];
		}

		constexpr T& at(qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, true);
			}
			return this->memory.at(index);
		}
		constexpr const T& at(qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, true);
			}
			return this->memory.at(index);
		}

		constexpr auto data() {
			return this->memory.data();
		}
		constexpr auto data() const {
			return this->memory.data();
		}


		constexpr T& front(qpl::size index) {
			return this->memory.front(index);
		}
		constexpr const T& front(qpl::size index) const {
			return this->memory.front(index);
		}
		constexpr T& back(qpl::size index) {
			return this->memory.back(index);
		}
		constexpr const T& back(qpl::size index) const {
			return this->memory.back(index);
		}

		constexpr qpl::size size() const {
			return this->memory.size();
		}

		constexpr auto begin() {
			return this->memory.begin();
		}
		constexpr auto begin() const {
			return this->memory.begin();
		}
		constexpr auto rbegin() {
			return this->memory.rbegin();
		}
		constexpr auto rbegin() const {
			return this->memory.rbegin();
		}
		constexpr auto cbegin() {
			return this->memory.cbegin();
		}
		constexpr auto crbegin() {
			return this->memory.crbegin();
		}		

		constexpr auto end() {
			return this->memory.end();
		}
		constexpr auto end() const {
			return this->memory.end();
		}
		constexpr auto rend() {
			return this->memory.rend();
		}
		constexpr auto rend() const {
			return this->memory.rend();
		}
		constexpr auto cend() {
			return this->memory.cend();
		}
		constexpr auto crend() {
			return this->memory.crend();
		}

		std::string string() const {
			return qpl::container_to_string(this->memory);
		}
		template<typename T, qpl::size N>
		friend std::ostream& operator<<(std::ostream& os, const array<T, N>& array);
	};

	template<typename T, qpl::size N>
	std::ostream& operator<<(std::ostream& os, const qpl::array<T, N>& array) {
		return (os << array.string());
	}

	template<typename T, bool BOUNDARY_CHECK = detail::vector_boundary_check>
	struct vector {

	private:
		constexpr void index_check(qpl::size index, bool at) const {
			if (index >= this->size()) {
				std::ostringstream stream;
				stream << qpl::to_string("qpl::vector<", qpl::type_name<T>(), ">", at ? ".at()" : "::operator[]", " : index is ", index);
				qpl::i32 convert = qpl::i32_cast(index);
				if (convert < 0) {
					stream << qpl::to_string(" (= ", convert, ") ");
				}
				stream << " - size of vector is " << this->size();
				qpl::println(stream.str());
				throw std::exception(stream.str().c_str());
			}
		}
		constexpr void front_check(bool front) const {
			if (this->empty()) {
				std::ostringstream stream;
				stream << qpl::to_string("qpl::vector<", qpl::type_name<T>(), ">", front ? ".front()" : ".back()", " : vector is empty");
				qpl::println(stream.str());
				throw std::exception(stream.str().c_str());
			}
		}
	public:

		using vector_type = std::vector<T>;
		vector_type memory;

		constexpr vector() {

		}

		constexpr vector(const vector& other) : memory() {
			*this = other;
		}
		constexpr vector(const std::vector<T>& other) : memory() {
			*this = other;
		}
		constexpr vector(const std::initializer_list<T>& list) : memory() {
			*this = list;
		}
		template<typename First, typename... Args>
		constexpr vector(First first, Args&&... args) : memory() {
			auto result = qpl::type_cast<T>(first, args...);
			for (qpl::u32 i = 0u; i < qpl::min(this->size(), result.size()); ++i) {
				this->data[i] = result[i];
			}
		}

		constexpr vector& operator=(const vector& other) {
			this->memory = other.memory;
			return *this;
		}
		constexpr vector& operator=(const std::vector<T>& other) {
			this->memory = other;
			return *this;
		}
		constexpr vector& operator=(const std::initializer_list<T>& list) {
			for (qpl::u32 i = 0u; i < qpl::min(list.size(), this->size()); ++i) {
				this->memory[i] = *(list.begin() + i);
			}
			return *this;
		}

		constexpr operator std::vector<T>() {
			return this->memory;
		}

		constexpr void pop_back(const T& value) {
			this->memory.pop_back();
		}
		constexpr void push_back(const T& value) {
			this->memory.push_back(value);
		}
		constexpr void emplace_back(T&& value) {
			this->memory.emplace_back(std::forward(value));
		}

		constexpr void fill(const T& value) {
			this->memory.fill(value);
		}
		constexpr void reserve(qpl::size reserve) {
			this->memory.reserve(reserve);
		}
		constexpr void resize(qpl::size resize) {
			this->memory.resize(resize);
		}
		

		constexpr T& operator[](qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, false);
			}
			return this->memory[index];
		}
		constexpr const T& operator[](qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, false);
			}
			return this->memory[index];
		}

		constexpr T& at(qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, true);
			}
			return this->memory.at(index);
		}
		constexpr const T& at(qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, true);
			}
			return this->memory.at(index);
		}

		constexpr auto data() {
			return this->memory.data();
		}
		constexpr auto data() const {
			return this->memory.data();
		}


		constexpr T& front(qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->front_check(index, true);
			}
			return this->memory.front(index);
		}
		constexpr const T& front(qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->front_check(index, true);
			}
			return this->memory.front(index);
		}
		constexpr T& back(qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->front_check(index, false);
			}
			return this->memory.back(index);
		}
		constexpr const T& back(qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->front_check(index, false);
			}
			return this->memory.back(index);
		}

		constexpr qpl::size size() const {
			return this->memory.size();
		}
		constexpr bool empty() const {
			return this->memory.empty();
		}

		constexpr auto begin() {
			return this->memory.begin();
		}
		constexpr auto begin() const {
			return this->memory.begin();
		}
		constexpr auto rbegin() {
			return this->memory.rbegin();
		}
		constexpr auto rbegin() const {
			return this->memory.rbegin();
		}
		constexpr auto cbegin() {
			return this->memory.cbegin();
		}
		constexpr auto crbegin() {
			return this->memory.crbegin();
		}

		constexpr auto end() {
			return this->memory.end();
		}
		constexpr auto end() const {
			return this->memory.end();
		}
		constexpr auto rend() {
			return this->memory.rend();
		}
		constexpr auto rend() const {
			return this->memory.rend();
		}
		constexpr auto cend() {
			return this->memory.cend();
		}
		constexpr auto crend() {
			return this->memory.crend();
		}

		std::string string() const {
			return qpl::container_to_string(this->memory);
		}
		template<typename T>
		friend std::ostream& operator<<(std::ostream& os, const vector<T>& array);
	};


	template<typename T>
	std::ostream& operator<<(std::ostream& os, const vector<T>& array) {
		return (os << array.string());
	}
}

#endif