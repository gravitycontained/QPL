#pragma once

#include <qpl/random.hpp>
#include <qpl/algorithm.hpp>
#include <array>

namespace qpl {

	template<typename T, qpl::size N = 2, qpl::size S = 0x2f> requires (qpl::is_integer<T>())
	struct simple_obfuscated_memory {
		std::array<std::array<T, 8>, N> data;
		qpl::u32 ctr = 0u;

		simple_obfuscated_memory() {
			shuffle();
		}
		simple_obfuscated_memory(T value) {
			shuffle();
			*this = value;
		}
		constexpr simple_obfuscated_memory(const std::array<T, 8 * N>& data) : data() {
			this->set_array(data);
		}

		void shuffle() {
			if (!qpl::detail::rng.seeded) {
				qpl::detail::rng.set_seed_random();
			}
			ctr = 0;
			for (auto& i : data) {
				for (auto& j : i) {
					j = qpl::random<T>();
				}
			}
		}
		constexpr void set_array(const std::array<T, 8 * N>& data) {
			if (std::is_constant_evaluated()) {
				for (qpl::u32 i = 0u; i < 8 * N; ++i) {
					this->data[i / 8][i % 8] = data[i];
				}
			}
			else {
				memcpy(this->data.data()->data(), data.data(), 8 * N * sizeof(T));
			}
		}
		constexpr std::array<T, 8 * N> get_array() const {
			std::array<T, 8 * N> result;
			if (std::is_constant_evaluated()) {
				for (qpl::u32 i = 0u; i < 8 * N; ++i) {
					result[i] = this->data[i / 8][i % 8];
				}
			}
			else {
				memcpy(result.data(), this->data.data()->data(), 8 * N * sizeof(T));
			}
			return result;
		}

		simple_obfuscated_memory& operator+=(T value) {
			return (*this = this->get() + value);
		}
		simple_obfuscated_memory& operator-=(T value) {
			return (*this = this->get() - value);
		}
		simple_obfuscated_memory& operator/=(T value) {
			return (*this = this->get() / value);
		}
		simple_obfuscated_memory& operator*=(T value) {
			return (*this = this->get() * value);
		}
		simple_obfuscated_memory& operator^=(T value) {
			return (*this = this->get() ^ value);
		}
		simple_obfuscated_memory& operator&=(T value) {
			return (*this = this->get() & value);
		}
		simple_obfuscated_memory& operator|=(T value) {
			return (*this = this->get() | value);
		}
		void set(T value) {
			++ctr;
			if (ctr >= S) {
				shuffle();
			}

			data[0][1] = value ^ (data[0][6] ^ data[0][3]);

			for (qpl::u32 i = 0u; i < N; ++i) {
				T n = 0;
				if (i == 0) n = data[0][7];
				if (i == 1) n = data[0][0] ^ data[1][7];
				if (i == 2) n = data[1][5];
				n ^= qpl::rotate_left(data[0][4], (data[N - 1 - i][2] & 0x7) + 1);
				data[0][1] ^= (data[i][6] ^ data[i][3] ^ n);
			}
		}
		simple_obfuscated_memory& operator=(T value) {
			this->set(value);
			return *this;
		}
		constexpr T get() const {
			T result = data[0][1] ^ (data[0][6] ^ data[0][3]);

			for (qpl::u32 i = 0u; i < N; ++i) {
				T n = 0;
				if (i == 0) n = data[0][7];
				if (i == 1) n = data[0][0] ^ data[1][7];
				if (i == 2) n = data[1][5];
				n ^= qpl::rotate_left(data[0][4], (data[N - 1 - i][2] & 0x7) + 1);
				result ^= (data[i][6] ^ data[i][3] ^ n);
			}
			return result;
		}
		constexpr operator T() const {
			return this->get();
		}
		template<typename T, qpl::size N, qpl::size S> requires (qpl::is_integer<T>())
		friend std::ostream& operator<<(std::ostream& os, const simple_obfuscated_memory<T, N, S>& n);
	};

	template<typename T, qpl::size N, qpl::size S> requires (qpl::is_integer<T>())
	std::ostream& operator<<(std::ostream& os, const simple_obfuscated_memory<T, N, S>& n) {
		return (os << n.get());
	}

	template<typename T, qpl::size N = 2, qpl::size S = 0x2f> requires (qpl::is_integer<T>())
	auto get_obfuscated(T value) {
		simple_obfuscated_memory<T, N, S> result;
		result = value;
		return result;
	}
}