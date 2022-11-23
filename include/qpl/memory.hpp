#ifndef QPL_MEMORY_HPP
#define QPL_MEMORY_HPP
#pragma once

#include <qpl/algorithm.hpp>
#include <qpl/string.hpp>
#include <qpl/qpldeclspec.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/system.hpp>
#include <qpl/vardef.hpp>
#include <qpl/exception.hpp>
#include <array>
#include <string_view>
#include <sstream>



namespace qpl {
	template<typename C> requires (qpl::is_container<C>())
	constexpr inline C string_to_container_memory(const std::string& data) {
		C result;
		if (data.empty()) {
			return result;
		}
		result.resize((data.size() - 1) / qpl::bytes_in_type<qpl::container_subtype<C>>() + 1);
		memcpy(result.data(), data.data(), data.size());
		return result;
	}

	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr inline std::string container_memory_to_string(const C& data) {
		std::string result;
		result.resize(data.size() * sizeof(qpl::container_subtype<C>));
		memcpy(result.data(), data.data(), result.size());
		return result;
	}
	template<typename C>
	inline std::string memory_to_string(const C& data) {
		std::string result;
		result.resize(sizeof(C));
		memcpy(result.data(), &data, result.size());
		return result;
	}
	template<typename C>
	constexpr inline std::string stack_memory_to_string(const C& data) {
		return memory_to_string(data);
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr inline std::string heap_memory_to_string(const C& data) {
		return container_memory_to_string(data);
	}
	template<typename C>
	inline const char* memory_to_cstring(const C& data) {
		return reinterpret_cast<const char*>(&data);
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr inline void container_memory_to_string(const C& data, std::string& destination) {
		destination.resize(data.size() * sizeof(qpl::container_subtype<C>));
		memcpy(destination.data(), data.data(), destination.size());
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr inline void string_to_container_memory(const std::string_view& data, C& destination) {
		if (data.empty()) {
			return;
		}
		if constexpr (qpl::has_resize<C>()) {
			destination.resize((data.size() - 1) / sizeof(qpl::container_subtype<C>) + 1);
		}
		memcpy(destination.data(), data.data(), data.size());
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
		constexpr inline void string_to_heap_memory(const std::string_view& data, C& destination) {
		string_to_container_memory(data, destination);
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_resize<C>())
	constexpr inline void string_to_container_memory(const std::string& data, C& destination) {
		if (data.empty()) {
			return;
		}
		destination.resize((data.size() - 1) / sizeof(qpl::container_subtype<C>) + 1);
		memcpy(destination.data(), data.data(), data.size());
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_resize<C>())
	constexpr inline void string_to_heap_memory(const std::string& data, C& destination) {
		string_to_container_memory(data, destination);
	}
	template<typename C>
	inline void string_to_memory(const std::string& data, C& destination) {
		memcpy(&destination, data.data(), qpl::min(sizeof(C), data.size()));
	}
	template<typename C>
	inline void string_to_stack_memory(const std::string& data, C& destination) {
		string_to_memory(data, destination);
	}
	template<typename C>
	inline C string_to_memory(const std::string& data) {
		C destination;
		if (data.size() != sizeof(C)) {
			throw qpl::exception("string_to_memory: sizes not equal: string: ", data.size(), " stack object: ", sizeof(C));
			return C{};
		}
		memcpy(&destination, data.data(), data.size());
		return destination;
	}
	template<typename C> requires (qpl::has_data<C>())
	constexpr inline void string_to_stack_memory(const std::string_view& data, C& destination) {
		string_to_memory(data, destination);
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr inline void add_string_to_container_memory(const std::string& data, C& destination) {
		if (data.empty()) {
			return;
		}
		auto size = destination.size();
		destination.resize(size + (data.size() - 1) / sizeof(qpl::container_subtype<C>) + 1);
		memcpy(destination.data() + size, data.data(), data.size());
	}

	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr std::wstring container_memory_to_wstring(const C& data) {
		std::wstring result;
		result.resize(data.size() * sizeof(qpl::container_subtype<C>) / (sizeof(wchar_t) / sizeof(char)));
		memcpy(result.data(), data.data(), result.size() * (sizeof(wchar_t) / sizeof(char)));
		return result;
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr void container_memory_to_wstring(const C& data, std::wstring& destination) {
		destination.resize(data.size() * sizeof(qpl::container_subtype<C>));
		memcpy(destination.data(), data.data(), destination.size() * (sizeof(wchar_t) / sizeof(char)));
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr C wstring_to_container_memory(const std::wstring& data) {
		C result;
		result.resize(((data.size() * (sizeof(wchar_t) / sizeof(char)) - 1) / sizeof(qpl::container_subtype<C>) + 1));
		memcpy(result.data(), data.data(), data.size() * (sizeof(wchar_t) / sizeof(char)));
		return result;
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr void wstring_to_container_memory(const std::wstring& data, C& destination) {
		destination.resize(((data.size() * (sizeof(wchar_t) / sizeof(char)) - 1) / sizeof(qpl::container_subtype<C>) + 1));
		memcpy(destination.data(), data.data(), data.size() * (sizeof(wchar_t) / sizeof(char)));
	}
	template<typename C> requires (qpl::has_data<C>() && qpl::has_size<C>())
	constexpr void add_wstring_to_container_memory(const std::wstring& data, C& destination) {
		auto size = destination.size();
		destination.resize(size + ((data.size() * (sizeof(wchar_t) / sizeof(char)) - 1) / sizeof(qpl::container_subtype<C>)) + 1);
		memcpy(destination.data() + size, data.data(), data.size() * (sizeof(wchar_t) / sizeof(char)));
	}

	template<typename T, typename U> requires(qpl::bytes_in_type<T>() == qpl::bytes_in_type<U>())
	constexpr inline void copy_memory(const T& source, U& destination) {
		memcpy(&destination, &source, qpl::bytes_in_type<T>());
	}
	template<typename T, typename U> requires(qpl::bytes_in_type<T>() != qpl::bytes_in_type<U>())
	constexpr inline void copy_memory(const T& source, U& destination) {
		memcpy(&destination, &source, qpl::min(qpl::bytes_in_type<T>(), qpl::bytes_in_type<U>()));
	}
	template<typename U, typename T> requires(qpl::bytes_in_type<T>() == qpl::bytes_in_type<U>())
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

	template<typename T> requires (!qpl::is_same_decayed<T, std::string>())
		void write_memory_to_stream(std::ostream& os, T value) {
		os.write(qpl::memory_to_cstring(value), sizeof(T));
	}
	QPLDLL void write_string_to_stream(std::ostream& os, const std::string& value);


	QPLDLL void print_character_bool_table(std::string_view characters);

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

		struct iterator {
			circular_array* data;
			qpl::size index = 0u;

			iterator(circular_array& array, qpl::size index) : data(&array), index(index) {

			}
			iterator& operator++() {
				++this->index;
				return *this;
			}
			iterator operator++(int dummy) {
				auto copy = *this;
				++this->index;
				return copy;
			}
			bool operator==(const iterator& other) const {
				return this->index == other.index;
			}
			bool operator!=(const iterator& other) const {
				return this->index != other.index;
			}
			T& operator*() {
				return this->data->get(this->index);
			}
			const T& operator*() const {
				return this->data->get(this->index);
			}
		};

		struct const_iterator {
			const circular_array* data;
			qpl::size index = 0u;

			const_iterator(const circular_array& array, qpl::size index) : data(&array), index(index) {

			}
			const_iterator& operator++() {
				++this->index;
				return *this;
			}
			const_iterator operator++(int dummy) {
				auto copy = *this;
				++this->index;
				return copy;
			}
			bool operator==(const const_iterator& other) const {
				return this->index == other.index;
			}
			bool operator!=(const const_iterator& other) const {
				return this->index != other.index;
			}
			const T& operator*() const {
				return this->data->get(this->index);
			}
		};
		circular_array() {
			this->index = 0u;
			this->rotation_finished = false;
			if constexpr (this->is_array() && qpl::is_arithmetic<T>()) {
				this->data.fill(0u);
			}
		}

		void clear() {
			if constexpr (is_array() && qpl::is_arithmetic<T>()) {
				this->data.fill(T{});
			}
			else {
				std::fill(this->data.begin(), this->data.end(), T{});
			}
			this->index = 0u;
			this->rotation_finished = false;
		}
		void reset() {
			this->index = 0u;
			this->rotation_finished = false;
		}

		void add(T value) {
			if (this->rotation_finished) {
				this->data[this->index] = std::move(value);
				++this->index;

				if (this->index == this->size()) {
					this->index = 0u;
				}
			}
			else {
				this->data[this->index] = std::move(value);
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
				for (qpl::size i = 0u; i < this->index; ++i) {
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
				for (qpl::size i = 0u; i < this->index; ++i) {
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
				for (qpl::size i = 0u; i < this->index; ++i) {
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
				for (qpl::size i = 0u; i < this->index; ++i) {
					sum += this->data[i];
				}
				return sum;
			}
		}
		T get_sum_of_last_n(qpl::size last_n) const {
			if (last_n == this->size()) {
				return this->get_sum();
			}
			T sum = 0;
			if (this->rotation_finished) {
				qpl::size start = 0u;
				if (last_n > (this->index - 1)) {
					start = this->size() - 1 - (last_n - (this->index - 1));
				}
				else {
					start = this->index - 1 - last_n;
				}
				for (qpl::size i = 0u; i < last_n; ++i) {
					sum += this->data[(start + i) % this->size()];
				}
				return sum;
			}
			else {
				for (qpl::size i = this->index - 1 - last_n; i < this->index; ++i) {
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
				for (qpl::size i = 0u; i < this->index; ++i) {
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

		void resize(qpl::size size) {
			if constexpr (is_vector()) {
				this->data.resize(size);
				std::fill(this->data.begin(), this->data.end(), T{});
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
		T& get_actual(qpl::size index) {
			return this->data[index];
		}
		const T& get_actual(qpl::size index) const {
			return this->data[index];
		}

		T& get_previous(qpl::size index) {
			return this->get(this->used_size() - index - 1);
		}
		const T& get_previous(qpl::size index) const {
			return this->get(this->used_size() - index - 1);
		}
		T& get(qpl::size index) {
			if (this->rotation_finished) {
				auto idx = (index + this->index) % this->used_size();
				return this->data[idx];
			}
			else {
				return this->data[index];
			}
		}
		const T& get(qpl::size index) const {
			if (this->rotation_finished) {
				auto idx = (index + this->index) % this->used_size();
				return this->data[idx];
			}
			else {
				return this->data[index];
			}
		}
		T& operator[](qpl::size index) {
			return this->get(index);
		}
		const T& operator[](qpl::size index) const {
			return this->get(index);
		}

		auto begin() {
			return iterator(*this, 0u);
		}
		auto cbegin() {
			return const_iterator(*this, 0u);
		}
		auto end() {
			return iterator(*this, this->used_size());
		}
		auto cend() {
			return const_iterator(*this, this->used_size());
		}

		T& front() {
			if (this->rotation_finished) {
				return this->data[this->index];
			}
			else {
				return this->data[0u];
			}
		}
		const T& front() const {
			if (this->rotation_finished) {
				return this->data[this->index];
			}
			else {
				return this->data[0u];
			}
		}
		T& back() {
			auto index = this->index;
			if (index) {
				--index;
			}
			else {
				index = 0u;
			}
			return this->data[index];
		}
		const T& back() const {
			auto index = this->index;
			if (index) {
				--index;
			}
			else {
				index = 0u;
			}
			return this->data[index];
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
		bool empty() const {
			return this->used_size() == 0u;
		}

		private:
		qpl::conditional<qpl::if_true<N == 0>, std::vector<T>, std::array<T, N>> data;
		qpl::size index = 0u;
		bool rotation_finished = false;
	};

	
	namespace detail {
#ifdef QPL_NO_BOUNDARY_CHECK
		constexpr bool array_boundary_check = false;
		constexpr bool vector_boundary_check = false;
#elif defined  QPL_BOUNDARY_CHECK
	constexpr bool array_boundary_check = true;
	constexpr bool vector_boundary_check = true;
#else
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
#endif

	}

	template<typename T, qpl::size N, bool BOUNDARY_CHECK = detail::array_boundary_check>
	struct array : std::array<T, N> {
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
		constexpr auto& operator=(const std::array<T, N>& other) {
			return std::array<T, N>::operator=(other);
		}

		constexpr T& operator[](qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, false);
			}
			return std::array<T, N>::operator[](index);
		}
		constexpr const T& operator[](qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, false);
			}
			return std::array<T, N>::operator[](index);
		}

		constexpr T& at(qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, true);
			}
			return std::array<T, N>::at(index);
		}
		constexpr const T& at(qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, true);
			}
			return std::array<T, N>::at(index);
		}
	};

	template<typename T, bool BOUNDARY_CHECK = detail::vector_boundary_check>
	struct vector : std::vector<T> {

	private:
		constexpr void index_check(qpl::size index, bool at) const {
			if (index >= this->size()) {
				std::ostringstream stream;
				stream << qpl::to_string("qpl::vector<", qpl::type_name<T>(), ">", at ? ".at()" : "::operator[]", " : index is ", index);
				auto convert = qpl::signed_cast(index);
				if (convert < 0) {
					stream << qpl::to_string(" (= ", convert, ") ");
				}
				stream << " - size of vector is " << this->size();
				throw std::exception(stream.str().c_str());
			}
		}
		constexpr void front_check(bool front) const {
			if (this->empty()) {
				std::ostringstream stream;
				stream << qpl::to_string("qpl::vector<", qpl::type_name<T>(), ">", front ? ".front()" : ".back()", " : vector is empty");
				throw std::exception(stream.str().c_str());
			}
		}
	public:
		constexpr auto& operator=(const std::vector<T>& other) {
			return std::vector<T>::operator=(other);
		}

		constexpr T& operator[](qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, false);
			}
			return std::vector<T>::operator[](index);
		}
		constexpr const T& operator[](qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, false);
			}
			return std::vector<T>::operator[](index);
		}

		constexpr T& at(qpl::size index) {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, true);
			}
			return std::vector<T>::at(index);
		}
		constexpr const T& at(qpl::size index) const {
			if constexpr (BOUNDARY_CHECK) {
				this->index_check(index, true);
			}
			return std::vector<T>::at(index);
		}
	};
	struct save_state;

	namespace impl {
		template<typename T>
		concept has_save = requires(const T a) {
			{ a.save() } -> std::same_as<std::string>;
		};

		template<typename T>
		concept has_load = requires(T a, save_state state) {
			a.load(state);
		};
	}

	template<typename T>
	constexpr bool has_save() {
		if constexpr (qpl::is_container<T>() && qpl::has_size<T>()) {
			return qpl::has_save<qpl::container_subtype<T>>();
		}
		else {
			return qpl::impl::has_save<T>;
		}
	}
	template<typename T>
	constexpr bool has_load() {
		if constexpr (qpl::is_container<T>() && qpl::has_resize<T>()) {
			return qpl::has_load<qpl::container_subtype<T>>();
		}
		else {
			return qpl::impl::has_load<T>;
		}
	}

	struct save_state {
		qpl::collection_string collection_string;
		qpl::size ctr = 0u;

		save_state() {

		}
		template<typename... Ts>
		save_state(const Ts&... saves) {
			this->save(saves...);
		}

		QPLDLL void clear();
		QPLDLL void file_save(std::string path);
		QPLDLL void file_save(std::string path, const std::array<qpl::u64, 4>& key);
		QPLDLL void file_load(std::string path);
		QPLDLL void file_load(std::string path, const std::array<qpl::u64, 4>& key);
		QPLDLL void set_string(const std::string& str);
		QPLDLL void finalize_string();
		QPLDLL std::string get_string() const;
		QPLDLL std::string get_finalized_string();
		QPLDLL std::string get_next_string();
		QPLDLL save_state get_next_save_state();

		template<typename T>
		void save_single(const T& data) {
			if constexpr (qpl::is_standard_string_type<T>()) {
				this->collection_string.add_string(data);
			}
			else if constexpr (qpl::has_save<T>()) {
				if constexpr (qpl::is_container<T>() && qpl::has_size<T>()) {
					auto str = qpl::stack_memory_to_string(data.size());
					this->collection_string.add_string(str);
					for (auto& i : data) {
						this->save_single(i);
					}
				}
				else {
					this->collection_string.add_string(data.save());
				}
			}
			else if constexpr (qpl::has_data<T>() && qpl::has_size<T>()) {
				auto str = qpl::heap_memory_to_string(data);
				this->collection_string.add_string(str);
			}
			else {
				auto str = qpl::stack_memory_to_string(data);
				this->collection_string.add_string(str);
			}
		}
		template<typename... Ts>
		void save(const Ts&... data) {
			(this->save_single(data), ...);
		}
		template<typename T>
		void load_single(T& data) {
			if (this->ctr >= this->collection_string.size()) {
				throw qpl::exception("save_state: trying to load resource #", this->ctr, " but size is only ", this->collection_string.size());
			}
			if constexpr (qpl::has_load<T>()) {
				if constexpr (qpl::is_container<T>() && qpl::has_size<T>()) {
					qpl::size size;
					qpl::string_to_stack_memory(this->collection_string.get_string(this->ctr), size);
					data.resize(size);
					++ctr;
					for (auto& i : data) {
						this->load_single(i);
					}
				}
				else {
					data.load(this->get_next_save_state());
				}
			}
			else if constexpr (qpl::has_data<T>() && qpl::has_size<T>()) {
				if constexpr (qpl::is_std_array_type<T>()) {
					qpl::string_to_stack_memory(this->collection_string.get_string(this->ctr), data);
				}
				else {
					qpl::string_to_heap_memory(this->collection_string.get_string(this->ctr), data);
				}
				++this->ctr;
			}
			else {
				qpl::string_to_stack_memory(this->collection_string.get_string(this->ctr), data);
				++this->ctr;
			}
		}
		template<typename... Ts>
		void load(Ts&... data) {
			(this->load_single(data), ...);
		}
	};


}


#endif