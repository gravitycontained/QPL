#ifndef QPL_NUMBER_HPP
#define QPL_NUMBER_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/lut.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/memory.hpp>
#include <qpl/random.hpp>
#include <qpl/bits.hpp>
#include <qpl/system.hpp>
#include <span>
#include <array>
#include <vector>
#include <iostream>

#include <qpl/intrinsics.hpp>

namespace qpl {

	template<qpl::u32 base, bool sign>
	struct dynamic_integer {

		struct digit_proxy {
			digit_proxy(dynamic_integer& value, qpl::size index) {
				this->proxy = &value;
				this->index = index;
			}

			template<typename T>
			digit_proxy& operator=(T digit) {
				this->proxy->set_digit(this->index, digit);
				return *this;
			}

			qpl::u32 get_value() const {
				return this->proxy->get_digit(this->index);
			}

			operator qpl::u32() const {
				return this->get_value();
			}

		private:
			dynamic_integer* proxy;
			qpl::size index;
		};

		struct signed_content_type {
			std::vector<qpl::u32> memory;
			bool sign = false;
		};

		struct unsigned_content_type {
			std::vector<qpl::u32> memory;
		};

		template<typename T>
		constexpr dynamic_integer(const T& value) {
			*this = value;
		}
		constexpr dynamic_integer() {
			this->clear();
		}

		template<typename T>
		dynamic_integer& operator=(const T& value) {
			this->set(value);
			return *this;
		}

		void clear(qpl::u32 value = 0u) {
			this->content.memory.resize(1);
			this->content.memory[0] = qpl::u32_cast(value);

			if constexpr (is_signed()) {
				this->content.sign = false;
			}
		}


		constexpr static bool is_signed() {
			return sign;
		}
		constexpr static bool is_unsigned() {
			return !sign;
		}
		constexpr static qpl::u32 base_max() {
			return qpl::base_max(base);
		}
		constexpr static bool optimal_base() {
			return qpl::base_full_bit_usage(base);
		}
		constexpr static qpl::size base_max_log() {
			return qpl::base_max_log(base);
		}



		void remove_empty_back() {
			qpl::size x = this->memory_size() - 1;
			while (!this->content.memory[x]) {
				if (!x) {
					break;
				}
				--x;
			}
			this->content.memory.resize(x + 1);
		}

		void flip_sign() {
			if constexpr (is_signed()) {
				this->content.sign = !this->content.sign;
			}
		}
		dynamic_integer flipped_sign() const {
			auto copy = *this;
			copy.flip_sign();
			return copy;
		}

		bool is_negative() const {
			if constexpr (is_unsigned()) {
				return false;
			}
			else {
				return this->content.sign;
			}
		}
		bool is_positive() const {
			return !this->is_negative();
		}
		void set_negative() {
			if (this->is_positive()) {
				this->content.sign = !this->content.sign;
			}
		}
		void set_positive() {
			if constexpr (is_signed()) {
				if (this->is_negative()) {
					this->content.sign = !this->content.sign;
				}
			}
		}
		inline bool exceeding(qpl::size index) const {
			return index >= this->memory_size();
		}

		qpl::size memory_size() const {
			return this->content.memory.size();
		}
		qpl::size digits() const {
			return (this->memory_size() - 1) * base_max_log() + qpl::significant_digit(this->content.memory.back(), base);
		}

		void flip_digits() {
			if constexpr (optimal_base()) {
				for (qpl::u32 i = 0u; i < this->memory_size() - 1; ++i) {
					qpl::flip_bits(this->content.memory[i]);
				}
				qpl::flip_bits_until_msb(this->content.memory.back());
			}
			else {
				for (qpl::u32 i = 0u; i < this->memory_size() - 1; ++i) {
					qpl::flip_digits(this->content.memory[i], base);
				}
				qpl::flip_digits_until_msd(this->content.memory.back(), base);
			}
		}
		dynamic_integer flipped_digits() const {
			auto copy = *this;
			copy.flip_digits();
			return copy;
		}

		template<typename T>
		void set_digit(qpl::u32 position, T digit) {
			auto div = position / base_max_log();
			auto mod = position % base_max_log();
			if (this->exceeding(div)) {
				this->content.memory.resize(div + 1);
			}

			if constexpr (base == 2u) {
				qpl::set_bit(this->content.memory[div], mod, qpl::bool_cast(digit));
			}
			else {
				qpl::set_digit(this->content.memory[div], mod, qpl::u32_cast(digit), base);
			}
		}
		template<>
		void set_digit(qpl::u32 position, char digit) {
			auto div = position / base_max_log();
			auto mod = position % base_max_log();
			qpl::set_digit(content.memory[div], mod, qpl::from_base_string<qpl::u32>(digit, base, base <= 36u ? qpl::base_format::base36l : qpl::base_format::base64));
		}

		constexpr void set_first_digit(qpl::u32 digit) {
			if constexpr (base == 2u) {
				this->content.memory.front() = digit ? (this->content.memory.front() | qpl::u32{ 1u }) : (this->content.memory.front() & ~(qpl::u32{ 1u }));
			}
			else {
				auto original_digit = static_cast<qpl::u32>(this->content.memory.front() % base);
				if (original_digit >= digit) {
					this->content.memory.front() -= (original_digit - digit);
				}
				else {
					this->content.memory.front() += (digit - original_digit);
				}
			}
		}
		qpl::u32 get_digit(qpl::u32 position) const {
			auto div = position / base_max_log();
			auto mod = position % base_max_log();
			if (this->exceeding(div)) {
				return 0u;
			}
			if constexpr (base == 2u) {
				return qpl::get_bit(this->content.memory[div], mod);
			}
			else {
				return qpl::get_digit(this->content.memory[div], mod, base);
			}
		}
		char get_digit_char(qpl::u32 position) const {
			if (base <= 36u) {
				return qpl::detail::base_36_lower[this->get_digit(position)];
			}
			else {
				return qpl::detail::base_64[this->get_digit(position)];
			}
		}

		static qpl::u32 get_random_digit() {
			return qpl::random(base - 1);
		}
		static char get_random_digit_char() {
			if (base <= 36u) {
				return qpl::detail::base_36_lower[qpl::random(base - 1)];
			}
			else {
				return qpl::detail::base_64[qpl::random(base - 1)];
			}
		}

		inline digit_proxy operator[](qpl::size index) {
			return digit_proxy(*this, index);
		}
		inline const digit_proxy operator[](qpl::size index) const {
			return digit_proxy(*this, index);
		}
		inline digit_proxy front() {
			return digit_proxy(*this, 0u);
		}
		inline const digit_proxy front() const {
			return digit_proxy(*this, 0u);
		}
		inline digit_proxy back() {
			return digit_proxy(*this, this->digits() - 1);
		}
		inline const digit_proxy back() const {
			return digit_proxy(*this, this->digits() - 1);
		}


		template<typename T>
		void set(T value) {
			if constexpr (qpl::is_signed<T>()) {
				if constexpr (is_signed()) {
					if (value < 0) {
						if constexpr (optimal_base()) {
							auto memory1 = qpl::u32_cast(qpl::u64_cast(-value) >> qpl::bits_in_type<qpl::u32>());
							if (memory1) {
								this->content.memory.resize(2ull);
								this->content.memory[0] = qpl::u32_cast(-value);
								this->content.memory[1] = memory1;
							}
							else {
								this->content.memory.resize(1ull);
								this->content.memory[0] = qpl::u32_cast(-value);
							}
						}
						else {
							auto memory1 = qpl::u64_cast((-value) / base_max());
							auto memory2 = qpl::u32_cast(memory1 / base_max());
							if (memory2) {
								this->content.memory.resize(3ull);
								this->content.memory[0] = qpl::u32_cast((-value) % base_max());
								this->content.memory[1] = qpl::u32_cast(memory1 % base_max());
								this->content.memory[2] = memory2;
							}
							else if (memory1) {
								this->content.memory.resize(2ull);
								this->content.memory[0] = qpl::u32_cast((-value) % base_max());
								this->content.memory[1] = qpl::u32_cast(memory1 % base_max());
							}
							else {
								this->content.memory.resize(1ull);
								this->content.memory[0] = qpl::u32_cast((-value) % base_max());
							}
						}
						this->content.sign = true;
						return;
					}
					else {
						this->content.sign = false;
					}
				}
				else {
					if (value < 0) {
						this->clear();
						return;
					}
				}

				if constexpr (optimal_base()) {
					auto memory1 = qpl::u32_cast(qpl::u64_cast(value) >> qpl::bits_in_type<qpl::u32>());
					if (memory1) {
						this->content.memory.resize(2ull);
						this->content.memory[0] = qpl::u32_cast(value);
						this->content.memory[1] = memory1;
					}
					else {
						this->content.memory.resize(1ull);
						this->content.memory[0] = qpl::u32_cast(qpl::u32_cast(value));
					}
				}
				else {
					auto memory1 = qpl::u64_cast(value / base_max());
					auto memory2 = qpl::u32_cast(memory1 / base_max());
					if (memory2) {
						this->content.memory.resize(3ull);
						this->content.memory[0] = qpl::u32_cast(value % base_max());
						this->content.memory[1] = qpl::u32_cast(memory1 % base_max());
						this->content.memory[2] = memory2;
					}
					else if (memory1) {
						this->content.memory.resize(2ull);
						this->content.memory[0] = qpl::u32_cast(value % base_max());
						this->content.memory[1] = qpl::u32_cast(memory1 % base_max());
					}
					else {
						this->content.memory.resize(1ull);
						this->content.memory[0] = qpl::u32_cast(value % base_max());
					}
				}
			}
			else {
				if constexpr (optimal_base()) {
					auto memory1 = qpl::u32_cast(qpl::u64_cast(value) >> qpl::bits_in_type<qpl::u32>());

					if (memory1) {
						this->content.memory.resize(2ull);
						this->content.memory[0] = qpl::u32_cast(value);
						this->content.memory[1] = memory1;
					}
					else {
						this->content.memory.resize(1ull);
						this->content.memory[0] = qpl::u32_cast(value);
					}
				}
				else {
					auto memory1 = qpl::u64_cast(value / base_max());
					auto memory2 = qpl::u32_cast(memory1 / base_max());
					if (memory2) {
						this->content.memory.resize(3ull);
						this->content.memory[0] = qpl::u32_cast(value % base_max());
						this->content.memory[1] = qpl::u32_cast(memory1 % base_max());
						this->content.memory[2] = memory2;
					}
					else if (memory1) {
						this->content.memory.resize(2ull);
						this->content.memory[0] = qpl::u32_cast(value % base_max());
						this->content.memory[1] = qpl::u32_cast(memory1 % base_max());
					}
					else {
						this->content.memory.resize(1ull);
						this->content.memory[0] = qpl::u32_cast(value % base_max());
					}
				}
			}
		}

		template<qpl::size bits, bool sign>
		void set(integer<bits, sign> integer) {
			if (integer.is_negative()) {
				if constexpr (is_signed()) {
					integer.flip_bits();
					this->set(integer);
					this->increment();
					this->flip_sign();
					return;
				}
				else {
					this->clear();
					return;
				}
			}
			if constexpr (optimal_base()) {
				this->content.memory.resize(integer.last_used_index() + 1);
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->content.memory[i] = integer.memory[i];
				}
				return;
			}

			this->clear();
			dynamic_integer helper = 1;
			auto stop = integer.last_used_index();
			for (qpl::u32 i = 0u; i <= stop; ++i) {
				auto temp = helper;
				temp.mul(integer.memory[i]);
				this->add(temp);
				helper.mul(qpl::u64{ 1 } << qpl::bits_in_type<qpl::u32>());
			}
		}

#ifdef QPL_USE_INTRINSICS
		template<qpl::size bits, bool sign>
		void set(x64_integer<bits, sign> integer) {
			if (integer.is_negative()) {
				if constexpr (is_signed()) {
					integer.flip_bits();
					this->set(integer);
					this->increment();
					this->flip_sign();
					return;
				}
				else {
					this->clear();
					return;
				}
			}
			if constexpr (optimal_base()) {
				auto stop = integer.last_used_index_u32() + 1;
				this->content.memory.resize(stop);
				for (qpl::u32 i = 0u; i < stop; ++i) {
					this->content.memory[i] = integer.u32_at(i);
				}
				return;
			}

			this->clear();
			dynamic_integer mul = qpl::u64{ 1 } << qpl::bits_in_type<qpl::u32>();
			mul.mul(qpl::u64{ 1 } << qpl::bits_in_type<qpl::u32>());
			dynamic_integer helper = 1;
			auto stop = integer.last_used_index();
			for (qpl::u32 i = 0u; i <= stop; ++i) {
				auto temp = helper;
				temp.mul(integer.memory[i]);
				this->add(temp);
				helper.mul(mul);;
			}
		}
#endif

		template<qpl::u32 base2, bool sign2>
		void set(dynamic_integer<base2, sign2> integer) {
			if constexpr (sign2) {
				if (integer.is_negative()) {
					if constexpr (is_unsigned()) {
						this->clear();
						return;
					}
				}
			}
			if constexpr (optimal_base() && integer.optimal_base()) {
				this->content.memory.resize(integer.memory_size());
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->content.memory[i] = integer.content.memory[i];
				}
				if constexpr (is_signed()) {
					this->content.sign = integer.content.sign;
				}
				return;
			}
			if constexpr (base == base2) {
				this->content.memory = integer.content.memory;

				if constexpr (is_signed()) {
					if constexpr (sign2) {
						this->content.sign = integer.content.sign;
					}
					else {
						this->content.sign = false;
					}
				}
				return;
			}

			this->clear();
			dynamic_integer helper = 1;
			if constexpr (integer.optimal_base()) {
				for (qpl::u32 i = 0u; i < integer.memory_size(); ++i) {
					auto temp = helper;
					temp.mul(integer.content.memory[i]);
					this->add(temp);
					helper.mul(qpl::u64{ 1 } << qpl::bits_in_type<qpl::u32>());
				}
			}
			else {
				for (qpl::u32 i = 0u; i < integer.memory_size(); ++i) {
					auto temp = helper;
					temp.mul(integer.content.memory[i]);
					this->add(temp);
					helper.mul(integer.base_max());
				}
			}

			if (integer.is_negative()) {
				if constexpr (is_signed()) {
					this->set_negative();
				}
			}
		}

#ifndef QPL_NO_FLOATS
		template<qpl::size exponent_bits, qpl::size mantissa_bits>
		void set(floating_point<exponent_bits, mantissa_bits> value) {
			if constexpr (this->is_signed()) {
				this->set(value.integer_part_signed());
			}
			else {
				this->set(value.integer_part_unsigned());
			}
		}
#endif

		void set(std::string_view string, qpl::u32 string_base = base) {
			if (string.empty()) {
				this->clear();
			}

			if (string.front() == '-') {
				if constexpr (is_signed()) {
					this->set_negative();
					this->set(string.substr(1));
					return;
				}
				else {
					this->clear();
					return;
				}
			}
			else {
				this->set_positive();
			}

			if (string.length() >= 2) {
				if (string.substr(0, 2) == "0x") {
					dynamic_integer<16u, sign> x = string.substr(2);
					*this = x;
					return;
				}
				else if (string.substr(0, 2) == "0b") {
					dynamic_integer<2u, sign> x = string.substr(2);
					*this = x;
					return;
				}
			}

			if (string_base != base) {
				this->clear();

				bool negative = false;
				if (string.front() == '-') {
					negative = true;
				}

				std::string_view substr;
				if (string.length() >= 2 + negative) {
					substr = string.substr(negative, 2);
				}

				qpl::i32 off = negative;
				if (substr == "0x" || substr == "0b") {
					off += 2;
				};

				std::vector<qpl::u32> base_memory;
				qpl::i32 ctr = 0u;
				for (qpl::u32 i = 0u;; ++i) {
					qpl::i32 index = (qpl::i32_cast(string.length()) - qpl::i32_cast(i * qpl::base_max_log<qpl::u32>(string_base))) - qpl::i32_cast(qpl::base_max_log<qpl::u32>(string_base));
					qpl::i32 size = qpl::i32_cast(qpl::base_max_log<qpl::u32>(string_base));
					if (index < off) {
						size -= off - index;
					}
					index = qpl::max(off, index);

					auto str = string.substr(index, size);
					base_memory.push_back(qpl::from_base_string(str, string_base, string_base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64));


					ctr += qpl::base_max_log<qpl::u32>(string_base);

					if (index == off) {
						break;
					}
				}

				auto multiply = qpl::base_max(string_base);
				qpl::dynamic_integer<base, sign> helper = 1;
				for (auto& i : base_memory) {
					auto add = helper * i;
					this->add(add);

					if (multiply) {
						helper.mul(multiply);
					}
					else {
						helper <<= base_max_log();
					}
				}

				if (negative) {
					this->flip_sign();
				}
				return;
			}

			this->content.memory.resize((string.length() - 1) / base_max_log() + 1);
			auto div = string.length() / base_max_log();
			auto mod = string.length() % base_max_log();

			for (qpl::u32 i = 0u; i < div; ++i) {
				this->content.memory[i] = qpl::from_base_string(string.substr((div - i - 1) * base_max_log() + mod, base_max_log()), base, base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64);
			}
			if (mod) {
				this->content.memory.back() = qpl::from_base_string(string.substr(0, mod), base, base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64);
			}
		}

		void set(const char* string) {
			this->set(std::string_view{ string });
		}

		template<qpl::u32 base2 = base, bool sign2 = sign>
		dynamic_integer<base2, sign2> as_type() const {
			return dynamic_integer<base2, sign2>{ *this };
		}

		template<typename T>
		void add(T value) {
			if constexpr (qpl::bits_in_type<T>() >= qpl::bits_in_type<qpl::u64>()) {
				this->add(dynamic_integer{ value });

				if constexpr (optimal_base()) {
					qpl::u64 m = qpl::u64_cast(qpl::u32_cast(value)) + qpl::u64_cast(this->memory[0]);
					this->memory[0] = qpl::u32_cast(m);
					m >>= qpl::bits_in_type<qpl::u32>();

					m += qpl::u64_cast(value >> qpl::bits_in_type<qpl::u32>());
					for (qpl::u32 i = 1u;; ++i) {
						m += qpl::u64_cast(this->memory[i]);
						this->memory[i] = qpl::u32_cast(m);
						m >>= qpl::bits_in_type<qpl::u32>();
						if (!m) {
							return;
						}
					}
					return;
				}
				else {

					qpl::u64 m = qpl::u64_cast(value % base_max()) + qpl::u64_cast(this->memory[0]);
					this->memory[0] = qpl::u32_cast(m);
					m /= base_max();

					m += qpl::u64_cast(value / base_max());
					for (qpl::u32 i = 1u;; ++i) {
						m += qpl::u64_cast(this->memory[i]);
						this->memory[i] = qpl::u32_cast(m % base_max());
						m /= base_max();
						if (!m) {
							return;
						}
					}
					return;
				}
			}
			if (qpl::is_signed<T>()) {
				if (value < 0) {
					this->sub(value);
					return;
				}
			}

			qpl::u64 m = qpl::u64_cast(value);
			for (qpl::u32 i = 0u; qpl::bool_cast(m); ++i) {
				bool exceeding = this->exceeding(i);

				if (exceeding) {
					if constexpr (optimal_base()) {
						this->content.memory.push_back(qpl::u32_cast(m));
					}
					else {
						this->content.memory.push_back(qpl::u32_cast(m % base_max()));
					}
				}
				else {
					m += qpl::u64_cast(this->content.memory[i]);

					if constexpr (optimal_base()) {
						this->content.memory[i] = qpl::u32_cast(m);
					}
					else {
						this->content.memory[i] = qpl::u32_cast(m % base_max());
					}
				}

				if constexpr (optimal_base()) {
					m >>= qpl::bits_in_type<qpl::u32>();
				}
				else {
					m /= base_max();
				}
			}
		}

		template<qpl::u32 base2, bool sign2>
		void add(dynamic_integer<base2, sign2> other, qpl::size index = 0u) {
			if constexpr (base != base2) {
				this->add(other.as_type<base, sign2>(), index);
				return;
			}
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (my_neg && other_neg) {
					this->flip_sign();
					this->add(other.flipped_sign(), index);
					this->flip_sign();
					return;
				}
				else if (my_neg) {
					*this = other - this->flipped_sign();
					return;
				}
				else if (other_neg) {
					this->sub(other.flipped_sign());
					return;
				}
			}
			else if constexpr (sign2) {
				if (other.is_negative()) {
					this->sub(other);
					return;
				}
			}
			if (this->memory_size() < index) {
				this->content.memory.resize(index + 1);
			}
			qpl::u64 m = 0u;
			for (qpl::u32 i = 0u;; ++i) {

				bool exceeding = this->exceeding(i + index);
				bool exceeding_other = other.exceeding(i);

				if (exceeding_other && exceeding && !m) {
					break;
				}

				if (!exceeding_other) {
					m += qpl::u64_cast(other.content.memory[i]);
				}

				if (exceeding) {
					if constexpr (optimal_base()) {
						this->content.memory.push_back(qpl::u32_cast(m));
					}
					else {
						this->content.memory.push_back(qpl::u32_cast(m % base_max()));
					}
				}
				else {
					m += qpl::u64_cast(this->content.memory[i + index]);

					if constexpr (optimal_base()) {
						this->content.memory[(i + index)] = qpl::u32_cast(m);
					}
					else {
						this->content.memory[(i + index)] = qpl::u32_cast(m % base_max());
					}
				}

				if constexpr (optimal_base()) {
					m >>= qpl::bits_in_type<qpl::u32>();
				}
				else {
					m /= base_max();
				}
			}
		}


		template<bool check_sign = true>
		void increment() {
			if constexpr (check_sign && is_signed()) {
				if (this->content.sign) {
					this->decrement<false>();
					return;
				}
			}

			if constexpr (optimal_base()) {
				for (qpl::u32 i = 0u;; ++i) {
					if (this->exceeding(i)) {
						this->content.memory.push_back(qpl::u32{ 1u });
						return;
					}
					auto result = ++this->content.memory[i];
					if (result) {
						return;
					}
				}
			}
			else {
				for (qpl::u32 i = 0u;; ++i) {
					if (this->exceeding(i)) {
						this->content.memory.push_back(qpl::u32{ 1u });
						return;
					}
					auto result = ++this->content.memory[i];
					if (result != base_max()) {
						return;
					}
					this->content.memory[i] = qpl::u32{ 0u };
				}
			}
		}
		template<bool check_sign = true>
		void decrement() {
			if constexpr (check_sign && is_signed()) {
				if (this->content.sign) {
					this->increment<false>();
					return;
				}
			}

			bool decremented = false;
			if constexpr (optimal_base()) {
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					if (this->content.memory[i]) {
						--this->content.memory[i];
						decremented = true;
						break;
					}
					else {
						this->content.memory[i] = qpl::u32_max;
					}
				}
			}
			else {
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					if (this->content.memory[i]) {
						--this->content.memory[i];
						decremented = true;
						break;
					}
					else {
						this->content.memory[i] = base_max() - 1u;
					}
				}
			}


			if (!decremented) {
				if constexpr (is_signed()) {
					this->clear(1);
					this->content.sign = true;
				}
				else {
					this->clear();
				}
			}
			this->remove_empty_back();
		}

		template<typename T>
		void sub(T value) {
			if constexpr (qpl::is_signed<T>()) {
				if (value < 0) {
					this->add(-value);
					return;
				}
			}
			if constexpr (is_signed()) {
				if (this->is_negative()) {
					this->add(value);
				}
			}

			if (value < this->content.memory[0]) {
				this->content.memory[0] -= value;
			}
			else {
				dynamic_integer other = value;
				this->sub(other);
			}
		}
		template<qpl::u32 base2, bool sign2>
		void sub(dynamic_integer<base2, sign2> other) {
			if constexpr (base != base2) {
				this->sub(other.as_type<base, sign2>());
				return;
			}
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (my_neg && other_neg) {
					*this = this->flipped_sign() + other.flipped_sign();
					this->flip_sign();
					return;
				}
				else if (my_neg) {
					*this = this->flipped_sign() + other;
					this->flip_sign();
					return;
				}
				else if (other_neg) {
					this->add(other.flipped_sign());
					return;
				}
				if (this->less(other)) {
					*this = other - *this;
					this->flip_sign();
					return;
				}
			}
			else if constexpr (sign2) {
				if (other.is_negative()) {
					this->add(other.flipped_sign());
					return;
				}
			}

			if (this->less(other)) {
				this->clear();
				return;
			}

			qpl::u32 over = 0u;
			for (qpl::u32 i = 0u;; ++i) {

				if (this->exceeding(i)) {
					break;
				}

				auto other_exceeding = other.exceeding(i);


				qpl::u32 other_memory;
				if (other_exceeding) {
					other_memory = 0u;
				}
				else {
					other_memory = other.content.memory[i];
				}

				bool less = this->content.memory[i] < (other_memory + over);
				if constexpr (optimal_base()) {
					this->content.memory[i] -= (other_memory + over);
					over = less ? 1u : 0u;
				}
				else {
					if (less) {
						this->content.memory[i] = base_max() - (other_memory - this->content.memory[i]) - over;
						over = 1u;
					}
					else {
						this->content.memory[i] -= (other_memory + over);
						over = 0u;
					}
				}

				if (!over && (other_exceeding)) {
					break;
				}
			}

			this->remove_empty_back();
		}

		template<typename T>
		void mul(T value) {
			if (this->is_zero()) {
				return;
			}
			if (value == 0) {
				this->clear();
				return;
			}
			if constexpr (qpl::bits_in_type<T>() >= qpl::bits_in_type<qpl::u64>()) {
				this->mul(dynamic_integer{ value });
				return;
			}
			if constexpr (qpl::is_signed<T>()) {
				if (value < 0) {
					if constexpr (is_signed()) {
						this->flip_sign();
					}
					this->mul(-value);
					return;
				}
			}

			qpl::u64 m = 0;
			for (qpl::u32 i = 0;; ++i) {
				bool exceeding = this->exceeding(i);

				if (!exceeding) {
					m += qpl::u64_cast(this->content.memory[i]) * value;

					if constexpr (optimal_base()) {
						this->content.memory[i] = qpl::u32_cast(m);
					}
					else {
						this->content.memory[i] = qpl::u32_cast(m % base_max());
					}
				}
				else {
					if (m) {
						if constexpr (optimal_base()) {
							this->content.memory.push_back(qpl::u32_cast(m));
						}
						else {
							this->content.memory.push_back(qpl::u32_cast(m % base_max()));
						}
					}
				}
				if constexpr (optimal_base()) {
					m >>= qpl::bits_in_type<qpl::u32>();
				}
				else {
					m /= qpl::u64_cast(base_max());
				}

				if (!m && exceeding) {
					return;
				}
			}
		}
		void mul(dynamic_integer other) {
			if (this->is_zero()) {
				return;
			}
			if (other.is_zero()) {
				this->clear();
				return;
			}
			if constexpr (is_signed()) {
				this->content.sign ^= other.content.sign;
			}

			auto copy = *this;
			this->clear();
			for (qpl::u32 i = 0u; i < other.memory_size(); ++i) {
				auto add = copy;
				add.mul(other.content.memory[i]);
				this->add(add, i);
			}
		}

		template<typename T>
		void div(T value) {
			this->div(dynamic_integer(value));
		}
		void div(dynamic_integer other) {
			bool flip = false;
			if constexpr (is_signed()) {
				if (this->is_negative() && other.is_negative()) {
					this->flip_sign();
					other.flip_sign();
				}
				else if (this->is_negative()) {
					this->flip_sign();
					flip = true;
				}
				else if (other.is_negative()) {
					other.flip_sign();
					flip = true;
				}
			}

			qpl::dynamic_integer<base, sign> mod;
			qpl::dynamic_integer<base, sign> div;

			for (qpl::i32 i = this->digits() - 1; i >= 0; --i) {
				mod <<= 1;
				mod.set_first_digit(this->get_digit(i));

				if constexpr (base == 2u) {
					if (mod >= other) {
						mod -= other;
						div.set_digit(i, qpl::u32{ 1 });
					}
				}
				else {
					qpl::u32 digit = 0u;
					while (mod >= other) {
						mod -= other;
						++digit;

					}
					if (digit) {
						div.set_digit(i, digit);
					}
				}
			}

			*this = div;
			if constexpr (is_signed()) {
				if (flip) {
					this->increment();
					this->flip_sign();
				}
			}
		}


		template<typename T>
		void mod(T value) {
			this->mod(dynamic_integer(value));
		}
		void mod(dynamic_integer other) {
			bool reverse = false;
			bool flip = false;
			if constexpr (is_signed()) {
				if (this->is_negative() && other.is_negative()) {
					this->flip_sign();
					other.flip_sign();
					flip = true;
				}
				else if (this->is_negative()) {
					this->flip_sign();
					reverse = true;
				}
				else if (other.is_negative()) {
					other.flip_sign();
					flip = true;
					reverse = true;
				}
			}

			qpl::dynamic_integer<base, sign> mod;

			for (qpl::i32 i = this->digits() - 1; i >= 0; --i) {
				mod <<= 1;
				mod.set_first_digit(this->get_digit(i));

				if constexpr (base == 2u) {
					if (mod >= other) {
						mod -= other;
					}
				}
				else {
					while (mod >= other) {
						mod -= other;
					}
				}
			}

			*this = mod;


			if constexpr (is_signed()) {
				if (reverse) {
					*this = other - mod;
				}
				else {
					*this = mod;
				}
				if (flip) {
					this->flip_sign();
				}
			}
			else {
				*this = mod;
			}
		}

		template<typename T>
		bool equals(const T& value) const {
			if constexpr (qpl::is_signed<T>()) {
				if constexpr (is_signed()) {
					if (value < 0) {
						if (!this->content.sign) {
							return false;
						}
					}
					else {
						if (this->content.sign) {
							return false;
						}
					}
				}
				else {
					if (value < 0) {
						return false;
					}
				}
			}
			else {
				if constexpr (is_signed()) {
					if (this->content.sign) {
						return false;
					}
				}
			}

			if constexpr (optimal_base()) {
				if (this->memory_size() > qpl::bytes_in_type<T>()) {
					return false;
				}
			}


			qpl::u64 m;

			if constexpr (qpl::is_signed<T>()) {
				if (value < 0) {
					m = qpl::u64_cast(-value);
				}
				else {
					m = qpl::u64_cast(value);
				}
			}
			else {
				m = qpl::u64_cast(value);
			}

			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {

				if constexpr (optimal_base()) {
					if (this->content.memory[i] != qpl::u32_cast(m)) {
						return false;
					}
				}
				else {
					if (this->content.memory[i] != qpl::u32_cast(m % base_max())) {
						return false;
					}
				}


				if constexpr (optimal_base()) {
					m >>= qpl::bits_in_type<qpl::u32>();
				}
				else {
					m /= base_max();
				}
			}

			return true;
		}
		bool equals(dynamic_integer other) const {
			if constexpr (is_signed()) {
				return this->content.sign == other.content.sign && this->content.memory == other.content.memory;
			}
			else {
				return this->content.memory == other.content.memory;
			}
		}


		template<typename T>
		bool less(const T& value) const {
			if constexpr (qpl::is_signed<T>()) {
				if constexpr (is_signed()) {
					if (value < 0) {
						if (!this->content.sign) {
							return false;
						}
					}
					else {
						if (this->content.sign) {
							return true;
						}
					}
				}
				else {
					if (value < 0) {
						return false;
					}
				}
			}
			else {
				if constexpr (is_signed()) {
					if (this->content.sign) {
						return true;
					}
				}
			}

			if constexpr (optimal_base()) {
				if (this->memory_size() > qpl::bytes_in_type<T>()) {
					return false;
				}
			}


			qpl::u64 m;

			if constexpr (qpl::is_signed<T>()) {
				if (value < 0) {
					m = qpl::u64_cast(-value);
				}
				else {
					m = qpl::u64_cast(value);
				}
			}
			else {
				m = qpl::u64_cast(value);
			}

			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {

				if constexpr (optimal_base()) {
					if (this->content.memory[i] > qpl::u32_cast(m)) {
						return false;
					}
					else if (this->content.memory[i] < qpl::u32_cast(m)) {
						return true;
					}
				}
				else {
					if (this->content.memory[i] > qpl::u32_cast(m % base_max())) {
						return false;
					}
					else if (this->content.memory[i] < qpl::u32_cast(m % base_max())) {
						return true;
					}
				}


				if constexpr (optimal_base()) {
					m >>= qpl::bits_in_type<qpl::u32>();
				}
				else {
					m /= base_max();
				}
			}

			return false;
		}
		template<bool check_sign = true>
		bool less(dynamic_integer other) const {
			if constexpr (check_sign && is_signed()) {
				if (this->content.sign != other.content.sign) {
					return this->content.sign;
				}
				else if (this->is_negative() && other.is_negative()) {
					return this->greater<false>(other);
				}
			}
			if (this->memory_size() != other.memory_size()) {
				return this->memory_size() < other.memory_size();
			}
			for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
				if (this->content.memory[i] < other.content.memory[i]) {
					return true;
				}
				else if (this->content.memory[i] > other.content.memory[i]) {
					return false;
				}
			}
			return false;
		}


		template<typename T>
		bool greater(const T& value) const {
			if constexpr (qpl::is_signed<T>()) {
				if constexpr (is_signed()) {
					if (value < 0) {
						if (!this->content.sign) {
							return true;
						}
					}
					else {
						if (this->content.sign) {
							return false;
						}
					}
				}
				else {
					if (value < 0) {
						return true;
					}
				}
			}
			else {
				if constexpr (is_signed()) {
					if (this->content.sign) {
						return false;
					}
				}
			}

			if constexpr (optimal_base()) {
				if (this->memory_size() > qpl::bytes_in_type<T>()) {
					return true;
				}
			}


			qpl::u64 m;

			if constexpr (qpl::is_signed<T>()) {
				if (value < 0) {
					m = qpl::u64_cast(-value);
				}
				else {
					m = qpl::u64_cast(value);
				}
			}
			else {
				m = qpl::u64_cast(value);
			}

			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {

				if constexpr (optimal_base()) {
					if (this->content.memory[i] > qpl::u32_cast(m)) {
						return true;
					}
					else if (this->content.memory[i] < qpl::u32_cast(m)) {
						return false;
					}
				}
				else {
					if (this->content.memory[i] > qpl::u32_cast(m % base_max())) {
						return true;
					}
					else if (this->content.memory[i] < qpl::u32_cast(m % base_max())) {
						return false;
					}
				}


				if constexpr (optimal_base()) {
					m >>= qpl::bits_in_type<qpl::u32>();
				}
				else {
					m /= base_max();
				}
			}

			return false;
		}
		template<bool check_sign = true>
		bool greater(dynamic_integer other) const {
			if constexpr (check_sign && is_signed()) {
				if (this->content.sign != other.content.sign) {
					return other.content.sign;
				}
				else if (this->is_negative() && other.is_negative()) {
					return this->less<false>(other);
				}
			}
			if (this->memory_size() != other.memory_size()) {
				return this->memory_size() > other.memory_size();
			}
			for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
				if (this->content.memory[i] < other.content.memory[i]) {
					return false;
				}
				else if (this->content.memory[i] > other.content.memory[i]) {
					return true;
				}
			}
			return false;
		}


		void left_memory_shift(qpl::i64 index = 1) {
			this->left_shift(index * base_max_log());
		}
		template<bool clear = true>
		void left_shift(qpl::i64 shift) {
			if (shift < 0) {
				this->right_shift(-shift);
				return;
			}

			auto digits = this->digits();
			auto div = shift / base_max_log();
			auto mod = shift % base_max_log();



			if (this->exceeding(((digits + shift) - 1) / base_max_log() + 1)) {
				this->content.memory.resize(((digits + shift) - 1) / base_max_log() + 1);
			}

			if (mod) {
				if constexpr (optimal_base()) {
					for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i > qpl::i32_cast(div); --i) {
						this->content.memory[i] = qpl::u32_cast(((this->content.memory[i - div] << (mod * qpl::log2(base))) | (this->content.memory[i - div - 1] >> (qpl::bits_in_type<qpl::u32>() - (mod * qpl::log2(base))))));
					}
					this->content.memory[div] = qpl::u32_cast(this->content.memory.front() << (mod * qpl::log2(base)));
				}
				else {
					auto mul_pow = qpl::pow<qpl::u64>(base, mod);
					auto div_pow = qpl::pow<qpl::u64>(base, base_max_log() - mod);

					for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i > qpl::i32_cast(div); --i) {
						this->content.memory[i] = qpl::u32_cast((this->content.memory[i - div] * mul_pow + this->content.memory[i - div - 1] / div_pow) % base_max());
					}
					this->content.memory[div] = qpl::u32_cast((this->content.memory.front() * mul_pow) % base_max());
				}

			}
			else if (div) {
				for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= div; --i) {
					this->content.memory[i] = this->content.memory[i - div];
				}
			}
			if constexpr (clear) {
				for (qpl::i32 i = 0u; i < qpl::i32_cast(div); ++i) {
					this->content.memory[i] = qpl::u32{};
				}
			}
		}

		void right_memory_shift(qpl::i64 index = 1) {
			this->right_shift(index * base_max_log());
		}
		template<bool clear = true>
		void right_shift(qpl::i64 shift) {
			if (shift < 0) {
				this->left_shift(-shift);
				return;
			}

			auto digits = this->digits();
			auto div = shift / base_max_log();
			auto mod = shift % base_max_log();

			if (shift >= digits) {
				this->clear();
				return;
			}

			if (mod) {
				if constexpr (optimal_base()) {
					for (qpl::i32 i = 0u; i < qpl::i32_cast(this->memory_size() - div) - 1; ++i) {
						this->content.memory[i] = qpl::u32_cast(((this->content.memory[i + div] >> (mod * qpl::log2(base))) | (this->content.memory[i + div + 1] << (qpl::bits_in_type<qpl::u32>() - (mod * qpl::log2(base))))));
					}
					this->content.memory[this->memory_size() - div - 1] = qpl::u32_cast(this->content.memory.back() >> (mod * qpl::log2(base)));
				}
				else {
					auto mul_pow = qpl::pow<qpl::u64>(base, base_max_log() - mod);
					auto div_pow = qpl::pow<qpl::u64>(base, mod);

					for (qpl::i32 i = 0u; i < qpl::i32_cast(this->memory_size() - div) - 1; ++i) {
						this->content.memory[i] = qpl::u32_cast((this->content.memory[i + div] / div_pow + this->content.memory[i + div + 1] * mul_pow) % base_max());
					}
					this->content.memory[this->memory_size() - div - 1] = qpl::u32_cast((this->content.memory.back() / div_pow) % base_max());
				}

			}
			else if (div) {
				for (qpl::i32 i = 0u; i < qpl::i32_cast(this->memory_size() - div); ++i) {
					this->content.memory[i] = this->content.memory[i + div];
				}
			}
			this->content.memory.resize(((digits - shift) - 1) / base_max_log() + 1);
		}

		dynamic_integer first_n_digits(qpl::size digits) const {
			if (!digits) {
				return 0;
			}
			if (digits >= this->digits()) {
				return *this;
			}

			auto div = digits / base_max_log();
			auto mod = digits % base_max_log();

			auto size = (digits - 1) / base_max_log();
			dynamic_integer result;
			result.content.memory.resize(size + 1);
			for (qpl::u32 i = 0; i < div; ++i) {
				result.content.memory[i] = this->content.memory[i];
			}
			if (mod) {
				if constexpr (optimal_base()) {
					result.content.memory[size] = this->content.memory[size] & (~qpl::u32{ 0u } >> (qpl::bits_in_type<qpl::u32>() - mod * qpl::log2(base)));
				}
				else {
					result.content.memory[size] = this->content.memory[size] % qpl::pow(base, mod);
				}
			}
			result.remove_empty_back();
			return result;
		}

		dynamic_integer last_n_digits(qpl::size digits) const {
			auto my_digits = this->digits();

			if (digits >= my_digits) {
				return *this;
			}
			auto copy = *this;
			copy.right_shift(my_digits - digits);
			return copy;
		}


		std::string memory_string(qpl::u32 result_base = base, qpl::u32 seperation = 0u, std::string_view prefix = "") const {
			std::ostringstream stream;
			if constexpr (is_signed()) {
				if (this->is_negative()) {
					stream << '-';
				}
			}
			stream << prefix;

			if (seperation) {

				std::ostringstream digits_stream;
				for (qpl::i32 i = qpl::i32_cast(this->memory_size()) - 1; i >= 0; --i) {
					digits_stream << qpl::base_string(this->content.memory[i], result_base, "", result_base <= 36u ? qpl::base_format::base36l : qpl::base_format::base64, i != qpl::i32_cast(this->memory_size()) - 1);
				}
				stream << qpl::string_seperation(digits_stream.str(), ' ', seperation, false);
				return stream.str();
			}
			else {
				std::ostringstream digits_stream;
				for (qpl::i32 i = qpl::i32_cast(this->memory_size()) - 1; i >= 0; --i) {
					stream << qpl::base_string(this->content.memory[i], result_base, "", result_base <= 36u ? qpl::base_format::base36l : qpl::base_format::base64, i != qpl::i32_cast(this->memory_size()) - 1);
				}
				return stream.str();
			}
		}

		std::vector<qpl::u32> base_memory(qpl::u32 base) const {
			auto base_max = qpl::base_max(base);

			std::vector<qpl::u32> result;
			std::vector<qpl::u32> helper;
			helper.push_back(qpl::u32{ 1 });

			for (qpl::u32 j = 0u; j < this->memory_size(); ++j) {
				std::vector<qpl::u32> multiplication = helper;

				//multiplying this->memory[j] into multiplication
				qpl::u32 multiply = this->content.memory[j];

				qpl::u64 m = 0u;
				for (qpl::u32 i = 0u;; ++i) {
					if (base_max == 0) {
						m >>= qpl::bits_in_type<qpl::u32>();
					}
					else {
						m /= base_max;
					}

					bool exceeding = (i >= multiplication.size());

					if (!exceeding) {
						m += qpl::u64_cast(multiplication[i]) * qpl::u64_cast(multiply);
					}
					else if (!m) {
						break;
					}

					if (exceeding) {
						multiplication.push_back(qpl::u32{});
					}

					if (base_max == 0) {
						multiplication[i] = qpl::u32_cast(m);
					}
					else {
						multiplication[i] = qpl::u32_cast(m % base_max);
					}
				}

				//adding multiplication to result
				m = 0;
				for (qpl::u32 i = 0u;; ++i) {


					bool exceeding = (i >= result.size());
					bool exceeding_mul = (i >= multiplication.size());

					if (exceeding_mul && exceeding && !m) {
						break;
					}

					if (!exceeding_mul) {
						m += qpl::u64_cast(multiplication[i]);
					}

					if (!exceeding) {
						m += qpl::u64_cast(result[i]);

						if (base_max == 0) {
							result[i] = qpl::u32_cast(m);
						}
						else {
							result[i] = qpl::u32_cast(m % base_max);
						}
					}
					else {
						if (base_max == 0) {
							result.push_back(qpl::u32_cast(m));
						}
						else {
							result.push_back(qpl::u32_cast(m % base_max));
						}
					}

					if (base_max == 0) {
						m >>= qpl::bits_in_type<qpl::u32>();
					}
					else {
						m /= base_max;
					}
				}

				m = 0u;
				for (qpl::u32 i = 0u; j != this->memory_size() - 1; ++i) {
					if (base_max == 0) {
						m >>= qpl::bits_in_type<qpl::u32>();
					}
					else {
						m /= base_max;
					}

					bool exceeding = (i >= helper.size());

					if (!exceeding) {
						if constexpr (optimal_base()) {
							m += qpl::u64_cast(helper[i]) << qpl::bits_in_type<qpl::u32>();
						}
						else {
							m += qpl::u64_cast(helper[i]) * dynamic_integer::base_max();
						}
					}
					else if (!m) {
						break;
					}

					if (exceeding) {
						helper.push_back(qpl::u32{});
					}

					if (base_max == 0) {
						helper[i] = qpl::u32_cast(m);
					}
					else {
						helper[i] = qpl::u32_cast(m % base_max);
					}
				}
			}
			return result;
		}


		constexpr std::string hex_string(qpl::size seperation = 0u, const std::string& prefix = "0x") const {
			if (this->optimal_base()) {
				return this->memory_string(16u, seperation, prefix);
			}
			else {
				return this->base_string(16u, seperation, prefix);
			}
		}
		constexpr std::string octal_string(qpl::size seperation = 0u, const std::string& prefix = "0") const {
			if (this->optimal_base()) {
				return this->memory_string(8u, seperation, prefix);
			}
			else {
				return this->base_string(8u, seperation, prefix);
			}
		}
		constexpr std::string binary_string(qpl::size seperation = 0u) const {
			if (this->optimal_base()) {
				return this->memory_string(2u, seperation);
			}
			else {
				return this->base_string(2u, seperation);
			}
		}
		std::string base_string(qpl::u32 result_base, qpl::size seperation = 0u, const std::string& prefix = "") const {
			if (result_base == base || (this->optimal_base() && qpl::base_full_bit_usage(result_base))) {
				return this->memory_string(result_base, seperation, prefix);
			}


			auto result = this->base_memory(result_base);

			std::ostringstream stream;

			bool empty = true;
			bool full = false;
			for (qpl::i32 i = qpl::i32_cast(result.size() - 1); i >= 0; --i) {
				if (empty && result[i] || i == 0) {
					empty = false;
				}
				if (!empty) {
					stream << qpl::base_string(result[i], result_base, "", result_base <= 64 ? qpl::base_format::base36l : qpl::base_format::base64, full);
					full = true;
				}
			}
			std::ostringstream rstream;


			if (this->is_negative()) {
				rstream << "-";
			}
			rstream << prefix;
			if (seperation == 0) {
				rstream << stream.str();
			}
			else {
				rstream << qpl::string_seperation(stream.str(), ' ', seperation, false);
			}
			return rstream.str();
		}
		std::string decimal_string(qpl::size seperation = 0u) {
			return this->base_string(10u, seperation);
		}

		template<typename T>
		bool operator==(const T& value) const {
			return this->equals(value);
		}
		template<typename T>
		bool operator!=(const T& value) const {
			return !(this->equals(value));
		}
		template<typename T>
		bool operator<(const T& value) const {
			return this->less(value);
		}
		template<typename T>
		bool operator<=(const T& value) const {
			return !(this->greater(value));
		}
		template<typename T>
		bool operator>(const T& value) const {
			return this->greater(value);
		}
		template<typename T>
		bool operator>=(T& value) const {
			return !(this->less(value));
		}


		template<typename T>
		dynamic_integer& operator+=(T value) {
			this->add(value);
			return *this;
		}
		template<typename T>
		dynamic_integer operator+(T value) const {
			auto copy = *this;
			copy.add(value);
			return copy;
		}

		template<typename T>
		dynamic_integer& operator-=(T value) {
			this->sub(value);
			return *this;
		}
		template<typename T>
		dynamic_integer operator-(T value) const {
			auto copy = *this;
			copy.sub(value);
			return copy;
		}

		template<typename T>
		dynamic_integer& operator*=(T value) {
			this->mul(value);
			return *this;
		}
		template<typename T>
		dynamic_integer operator*(T value) const {
			auto copy = *this;
			copy.mul(value);
			return copy;
		}

		template<typename T>
		dynamic_integer& operator/=(T value) {
			this->div(value);
			return *this;
		}
		template<typename T>
		dynamic_integer operator/(T value) const {
			auto copy = *this;
			copy.div(value);
			return copy;
		}

		template<typename T>
		dynamic_integer& operator%=(T value) {
			this->mod(value);
			return *this;
		}
		template<typename T>
		dynamic_integer operator%(T value) const {
			auto copy = *this;
			copy.mod(value);
			return copy;
		}

		dynamic_integer operator-() const {
			return this->flipped_sign();
		}
		dynamic_integer operator+() const {
			auto copy = *this;
			if (copy.is_negative()) {
				copy.flip_sign();
			}
			return copy;
		}
		dynamic_integer operator~() const {
			return this->flipped_digits();
		}

		template<typename T>
		dynamic_integer& operator<<=(T shift) {
			this->left_shift(qpl::i64_cast(shift));
			return *this;
		}

		template<typename T>
		dynamic_integer operator<<(T shift) const {
			auto copy = *this;
			copy.left_shift(qpl::i64_cast(shift));
			return copy;
		}
		template<typename T>
		dynamic_integer& operator>>=(T shift) {
			this->right_shift(qpl::i64_cast(shift));
			return *this;
		}


		template<typename T>
		dynamic_integer operator>>(T shift) const {
			auto copy = *this;
			copy.right_shift(qpl::i64_cast(shift));
			return copy;
		}
		dynamic_integer& operator++() {
			this->increment();
			return *this;
		}
		dynamic_integer operator++(int dummy) const {
			auto copy = *this;
			return ++copy;
		}
		dynamic_integer& operator--() {
			this->decrement();
			return *this;
		}
		dynamic_integer operator--(int dummy) const {
			auto copy = *this;
			return --copy;
		}

		operator qpl::u64() const {
			if (this->is_negative()) {
				return qpl::u64{};
			}
			qpl::u64 result = qpl::u64_cast(this->content.memory[0u]);
			qpl::u64 mul = 1u;
			for (qpl::u32 i = 1u; i < qpl::min<qpl::size>(3u, this->memory_size()); ++i) {
				mul *= qpl::u64_cast(base_max());
				result += qpl::u64_cast(this->content.memory[i]) * mul;
			}
			return result;
		}
		operator qpl::i64() const {
			qpl::i64 result = qpl::i64_cast(this->content.memory[0u]);
			qpl::i64 mul = 1u;
			for (qpl::u32 i = 1u; i < qpl::min<qpl::size>(3u, this->memory_size()); ++i) {
				mul *= qpl::i64_cast(base_max());
				result += qpl::i64_cast(this->content.memory[i]) * mul;
			}
			if (result < 0) {
				return qpl::i64_min;
			}
			if (this->is_negative()) {
				result *= -1;
			}
			return result;
		}

		operator qpl::u32() const {
			if (this->is_negative()) {
				return qpl::u64{};
			}
			qpl::u32 result = qpl::u64_cast(this->content.memory[0u]);
			qpl::u32 mul = 1u;
			for (qpl::u32 i = 1u; i < qpl::min<qpl::size>(2u, this->memory_size()); ++i) {
				mul *= qpl::u32_cast(base_max());
				result += qpl::u32_cast(this->content.memory[i]) * mul;
			}
			return result;
		}
		operator qpl::i32() const {
			qpl::i32 result = qpl::i64_cast(this->content.memory[0u]);
			qpl::i32 mul = 1u;
			for (qpl::u32 i = 1u; i < qpl::min<qpl::size>(2u, this->memory_size()); ++i) {
				mul *= qpl::i32_cast(base_max());
				result += qpl::i32_cast(this->content.memory[i]) * mul;
			}
			if (result < 0) {
				return qpl::i32_min;
			}
			if (this->is_negative()) {
				result *= -1;
			}
			return result;
		}

		operator qpl::u16() const {
			return qpl::u16_cast(this->content.memory[0]);
		}
		operator qpl::i16() const {
			return qpl::i16_cast(this->content.memory[0]);
		}

		operator qpl::u8() const {
			return qpl::u8_cast(this->content.memory[0]);
		}
		operator qpl::i8() const {
			return qpl::i8_cast(this->content.memory[0]);
		}


		bool is_zero() const {
			return this->memory_size() == 1u && this->content.memory[0u] == qpl::u32{ 0 };
		}
		operator bool() const {
			return !this->is_zero();
		}
		bool operator!() const {
			return this->is_zero();
		}



		void self_n_factorial(qpl::u32 n) {
			if (*this == 0 || *this == 1) {
				this->set(1);
				return;
			}
			for (qpl::u64 i = *this - n; i >= n; i -= n) {
				this->mul(i);
			}
		}
		dynamic_integer n_factorial(qpl::u32 n) const {
			if (*this == 0 || *this == 1) {
				this->set(1);
				return;
			}
			dynamic_integer result = *this;
			for (qpl::u64 i = *this - n; i >= n; i -= n) {
				result.mul(i);
			}
			return result;
		}
		template<typename T>
		static dynamic_integer n_factorial(T value, qpl::u32 n) {
			if (value == 0 || value == 1) {
				return 1;
			}
			dynamic_integer result = value;
			for (qpl::u64 i = value - n; i >= n; i -= n) {
				result.mul(i);
			}
			return result;
		}


		void self_factorial() {
			auto copy = *this;
			for (qpl::u64 i = 2u; i < copy; ++i) {
				this->mul(i);
			}
		}
		dynamic_integer factorial() const {
			dynamic_integer result = *this;
			for (qpl::u64 i = 2u; i < *this; ++i) {
				result.mul(i);
			}
			return result;
		}
		template<typename T>
		static dynamic_integer factorial(T value) {
			dynamic_integer result = value;
			for (qpl::u64 i = 2u; i < value; ++i) {
				result *= i;
			}
			return result;
		}


		template<typename T, typename U>
		static dynamic_integer pow(T a, U b) {
			dynamic_integer bb = a;
			dynamic_integer result = 1;
			while (true) {
				if (b & 1)
					result *= bb;
				b >>= 1;
				if (!b)
					break;
				bb *= bb;
			}

			return result;
		}

		template<typename T>
		constexpr void self_pow(T value) {
			dynamic_integer bb = *this;
			*this = 1;
			while (true) {
				if (value & 1)
					this->mul(bb);
				value >>= 1;
				if (!value)
					break;
				bb *= bb;
			}
		}
		template<typename T>
		constexpr dynamic_integer pow(T value) const {
			auto mul = *this;

			for (T i = 1; i < value; ++i) {
				mul.mul(*this);
			}
			return mul;
		}


		static dynamic_integer random(qpl::u32 digits) {
			dynamic_integer result;
			result.randomize(digits);
			return result;
		}
		template<typename T1, typename T2>
		static dynamic_integer random_range(T1 min, T2 max) {
			dynamic_integer diff = max;
			diff -= min;

			auto stop = diff.memory_size() - 1;
			dynamic_integer result;
			result.content.memory.resize(stop + 1);

			bool consider_range = true;
			for (qpl::i32 i = stop; i >= 0; --i) {
				if (consider_range) {
					result.content.content.memory[i] = qpl::random(diff.content.memory[i]);
					consider_range = (result.content.content.memory[i] == diff.content.memory[i]);
				}
				else {
					if constexpr (optimal_base()) {
						result.content.content.memory[i] = qpl::random<qpl::u32>();
					}
					else {
						result.content.memory[i] = qpl::random<qpl::u32>(base_max() - 1);
					}
				}
			}

			result += min;

			return result;
		}
		template<typename T>
		static dynamic_integer random_range(T max) {
			dynamic_integer conv = max;

			auto stop = conv.memory_size() - 1;
			dynamic_integer result;
			result.content.memory.resize(stop + 1);

			bool consider_range = true;
			for (qpl::i32 i = stop; i >= 0; --i) {
				if (consider_range) {
					result.content.memory[i] = qpl::random(conv.content.memory[i]);
					consider_range = (result.content.memory[i] == conv.content.memory[i]);
				}
				else {
					if constexpr (optimal_base()) {
						result.content.memory[i] = qpl::random<qpl::u32>();
					}
					else {
						result.content.memory[i] = qpl::random<qpl::u32>(base_max() - 1);
					}
				}
			}

			return result;
		}
		void randomize(qpl::u32 digits) {
			auto div = digits / base_max_log();
			auto mod = digits % base_max_log();

			auto size = (digits - 1) / base_max_log() + 1;
			this->content.memory.resize(size);
			for (qpl::u32 i = 0u; i < div; ++i) {
				if constexpr (optimal_base()) {
					this->content.memory[i] = qpl::random<qpl::u32>();
				}
				else {
					this->content.memory[i] = qpl::random<qpl::u32>(base_max() - 1);
				}
			}
			if (mod) {
				this->content.memory.back() = qpl::random(qpl::pow<qpl::u32>(base, mod));
			}

			this->remove_empty_back();

			if constexpr (is_signed()) {
				this->content.sign = qpl::random_b();
			}
		}

		friend std::ostream& operator<<(std::ostream& os, dynamic_integer other) {
			return (os << other.memory_string());
		}


		using holding_type = qpl::conditional<qpl::if_true<sign>, signed_content_type, unsigned_content_type>;
		holding_type content;
	};

	template<typename T, qpl::u32 base, bool sign> requires (!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator==(T value, qpl::dynamic_integer<base, sign> dynamic) {
		return dynamic == value;
	}
	template<typename T, qpl::u32 base, bool sign> requires (!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator!=(T value, qpl::dynamic_integer<base, sign> dynamic) {
		return dynamic != value;
	}
	template<typename T, qpl::u32 base, bool sign> requires (!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator<(T value, qpl::dynamic_integer<base, sign> dynamic) {
		return dynamic >= value;
	}
	template<typename T, qpl::u32 base, bool sign> requires (!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator>(T value, qpl::dynamic_integer<base, sign> dynamic) {
		return dynamic <= value;
	}
	template<typename T, qpl::u32 base, bool sign> requires (!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator<=(T value, qpl::dynamic_integer<base, sign> dynamic) {
		return dynamic > value;
	}
	template<typename T, qpl::u32 base, bool sign> requires (!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator>=(T value, qpl::dynamic_integer<base, sign> dynamic) {
		return dynamic < value;
	}

	template<qpl::size bits, bool sign>
	struct integer {
		using holding_type = qpl::conditional<qpl::if_true<bits% qpl::bits_in_type<qpl::u32>() == 0>, std::array<qpl::u32, (bits - 1) / qpl::bits_in_type<qpl::u32>() + 1>, qpl::default_error>;

		struct bit_proxy {
			constexpr bit_proxy(holding_type& memory, qpl::size index) {
				this->memory = &memory;
				this->index = index;
			}

			constexpr bit_proxy& operator=(bool value) {
				qpl::set_bit((*this->memory)[this->index / qpl::bits_in_type<qpl::u32>()], this->index % qpl::bits_in_type<qpl::u32>(), value);
				return *this;
			}

			constexpr bool get_value() const {
				return (*this->memory)[this->index / qpl::bits_in_type<qpl::u32>()] & (qpl::u32{ 1 } << (this->index % qpl::bits_in_type<qpl::u32>()));
			}

			constexpr operator bool() const {
				return this->get_value();
			}

		private:
			holding_type* memory;
			qpl::size index;
		};

		enum class constexpr_construction {
			zero, bit_flip, one, min_negative, max_negative
		};


		constexpr integer(constexpr_construction constructions) : memory() {
			switch (constructions) {
			case constexpr_construction::zero:
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
				break;
			case constexpr_construction::bit_flip:
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32_max;
				}
				break;
			case constexpr_construction::one:
				this->memory[0u] = qpl::u32{ 1 };
				for (qpl::u32 i = 1u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
				break;
			case constexpr_construction::min_negative:
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
				this->memory.back() = (0x8000'0000u);
				break;
			case constexpr_construction::max_negative:
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32_max;
				}
				this->memory.back() = (0x7FFF'FFFFu);
				break;
			}
		}


		constexpr integer() {

		}
		template<typename T>
		constexpr integer(T value) : memory() {
			*this = value;
		}


		constexpr integer(const holding_type& values) : memory(values) {
		}

		template<typename T>
		constexpr integer& operator=(T value) {
			this->set(value);
			return *this;
		}

		constexpr integer& operator=(std::string string) {
			this->set(string);
			return *this;
		}

		constexpr integer& operator=(const char* string) {
			this->set(std::string_view(string));
			return *this;
		}

		template<qpl::size bits2, bool sign2>
		constexpr integer& operator=(qpl::integer<bits2, sign2> other) {
			this->set(other);
			return *this;
		}

		constexpr static qpl::size memory_size() {
			return (bits - 1) / 32u + 1;
		}
		constexpr static qpl::size bit_size() {
			return bits;
		}
		constexpr static qpl::size used_bit_size() {
			return bits - qpl::size{ sign };
		}
		constexpr static bool is_signed() {
			return sign;
		}
		constexpr static bool is_unsigned() {
			return !sign;
		}
		constexpr static qpl::size max_decimal_exponent() {
			return qpl::size_cast(used_bit_size() / (qpl::ln10 / qpl::ln2));
		}
		constexpr static qpl::size max_base_exponent(qpl::u32 base) {
			if (qpl::base_full_bit_usage(base)) {
				return qpl::base_max_log(base) * memory_size();
			}
			else {
				integer mul = qpl::base_max(base);
				integer<bits + 32, false> n = qpl::base_max(base);
				for (qpl::u32 i = 1u; i < memory_size(); ++i) {
					n *= mul;
				}

				qpl::size ctr = memory_size() * qpl::base_max_log(base);
				while (true) {
					n *= base;
					++ctr;
					if (n.memory.back()) {
						return ctr - 1;
					}
				}
			}
		}
		constexpr static qpl::size base_max_log() {
			return qpl::bits_in_type<qpl::u32>();
		}

		constexpr static qpl::f64 decimal_digits() {
			return used_bit_size() / (qpl::ln10 / qpl::ln2);
		}

		constexpr static integer min() {
			if constexpr (is_unsigned()) {
				integer result{ constexpr_construction::zero };
				return result;
			}
			else {
				integer result{ constexpr_construction::min_negative };
				return result;
			}
		}
		constexpr static integer max() {
			if constexpr (is_unsigned()) {
				integer result{ constexpr_construction::bit_flip };
				return result;
			}
			else {
				integer result{ constexpr_construction::max_negative };
				return result;
			}
		}
		constexpr static integer zero() {
			integer result{ constexpr_construction::zero };
			return result;
		}
		constexpr static integer one() {
			integer result{ constexpr_construction::one };
			return result;
		}
		constexpr const static integer max_decimal() {
			return integer::pow(10, integer::max_decimal_exponent());
		}
		constexpr const static integer max_base(qpl::u32 base) {
			if (qpl::base_full_bit_usage(base)) {
				return integer::pow(base, integer::max_base_exponent(base) - 1);
			}
			else {
				return integer::pow(base, integer::max_base_exponent(base));
			}
		}
		constexpr const static integer max_binary() {
			integer result;
			result.memory.back() = (1u << ((used_bit_size() - 1) % base_max_log()));
			return result;
		}

		constexpr integer next_base_multiple(qpl::u32 base) const {
			integer result = 1;
			if (qpl::is_power_of_two(base)) {
				auto shift = qpl::significant_bit(base) - 1;
				while (result <= *this) {
					result <<= shift;
				}
			}
			else {
				while (result <= *this) {
					result *= base;
				}
			}
			return result;
		}

		constexpr void set_sign_bit_true() {
			qpl::set_bit(this->memory.back(), base_max_log() - 1, true);
		}
		constexpr void set_sign_bit_false() {
			qpl::set_bit(this->memory.back(), base_max_log() - 1, false);
		}
		constexpr void flip_sign_bit() {
			if (this->is_positive()) {
				this->set_sign_bit_true();
			}
			else {
				this->set_sign_bit_false();
			}
		}

		constexpr void set_negative() {
			if (this->is_positive()) {
				this->flip_bits();
				this->increment();
			}
		}
		constexpr void set_positive() {
			if (this->is_negative()) {
				this->flip_bits();
				this->increment();
			}
		}
		constexpr void flip_sign() {
			this->flip_bits();
			this->increment();
		}
		constexpr integer flipped_sign() const {
			auto copy = *this;
			copy.flip_sign();
			return copy;
		}

		constexpr void flip_bits() {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				qpl::flip_bits(this->memory[i]);
			}
		}
		constexpr integer flipped_bits() const {
			auto copy = *this;
			copy.flip_bits();
			return copy;
		}

		constexpr void flip_bits_until(qpl::size position) {
			if (position >= bit_size()) {
				this->flip_bits();
				return;
			}
			auto x = position / base_max_log();
			for (qpl::u32 i = 0u; i < x; ++i) {
				qpl::flip_bits(this->memory[i]);
			}
			qpl::flip_bits_until(this->memory[x], position % base_max_log());
		}
		constexpr integer flipped_bits_until(qpl::size position) const {
			auto copy = *this;
			copy.flip_bits_until(position);
			return copy;
		}

		constexpr void reverse_bits() {
			for (qpl::u32 i = 0u; i < memory_size() / 2; ++i) {
				qpl::reverse_bits(this->memory[i]);
				qpl::reverse_bits(this->memory[memory_size() - 1 - i]);

				auto copy = this->memory[i];
				this->memory[i] = this->memory[memory_size() - 1 - i];
				this->memory[memory_size() - 1 - i] = copy;
			}

			if constexpr (memory_size() % 2) {
				qpl::reverse_bits(this->memory[memory_size() / 2]);
			}
		}
		constexpr void reverse_bits(qpl::u32 position) {
			this->reverse_bits();
			this->right_shift(bit_size() - position);
		}
		constexpr void reverse_bits_to_most_significant_bit() {
			auto pos = this->significant_bit();
			this->reverse_bits();
			this->right_shift(bit_size() - pos);
		}

		constexpr integer reversed_bits() const {
			auto copy = *this;
			copy.reverse_bits();
			return copy;
		}
		constexpr integer reversed_bits(qpl::u32 position) const {
			auto copy = *this;
			copy.reverse_bits(position);
			return copy;
		}
		constexpr integer reversed_bits_to_most_significant_bit() const {
			auto copy = *this;
			copy.reverse_bits_to_most_significant_bit();
			return copy;
		}

		constexpr integer first_n_bits(qpl::u64 bits) const {
			if (!bits) {
				return 0;
			}
			if (bits >= used_bit_size()) {
				return *this;
			}

			auto div = (bits - 1) / base_max_log() + 1;
			auto mod = bits % base_max_log();

			auto size = (bits - 1) / base_max_log();

			integer result;
			for (qpl::u32 i = 0; i < div; ++i) {
				result.memory[i] = this->memory[i];
			}
			if (mod) {
				result.memory[size] = this->memory[size] & (~qpl::u32{ 0u } >> (base_max_log() - mod));
			}
			for (qpl::u32 i = div; i < this->memory_size(); ++i) {
				result.memory[i] = qpl::u32{};
			}

			return result;
		}

		constexpr integer last_n_bits(qpl::u64 bits) const {
			auto msb = this->significant_bit();

			if (bits >= msb) {
				return *this;
			}
			auto copy = *this;
			copy.right_shift(msb - bits);
			return copy;
		}

		constexpr bool is_zero() const {
			return *this == 0;
		}
		constexpr bool is_negative() const {
			if constexpr (is_unsigned()) {
				return false;
			}
			return this->memory.back() & (qpl::u32{ 1 } << (base_max_log() - 1));
		}
		constexpr bool is_positive() const {
			return !this->is_negative();
		}
		constexpr qpl::u32 last_memory_unsigned() const {
			if constexpr (is_unsigned()) {
				return this->memory.back();
			}
			return this->memory.back() & (qpl::u32_max >> 1);
		}

		constexpr qpl::u32 last_used_index(qpl::u32 start = memory_size() - 1) const {
			for (qpl::u32 i = start; i > 0u; --i) {
				if (this->memory[i]) {
					return i;
				}
			}
			return 0u;
		}
		constexpr qpl::u32 last_used_inverted_index() const {
			for (qpl::i32 i = this->memory_size() - 1; i > 0; --i) {
				if (this->memory[i] != qpl::u32_max) {
					return qpl::size_cast(i);
				}
			}
			return 0u;
		}

		template<typename T>
		constexpr void set(T value) {
			this->memory[0] = qpl::u32_cast(value);
			if constexpr (qpl::is_integer<T>() && qpl::bits_in_type<T>() == qpl::bits_in_type<qpl::u64>() && memory_size() > 1u) {
				this->memory[1] = qpl::u32_cast(value >> base_max_log());

				if constexpr (qpl::is_signed<T>()) {
					if (value < T{}) {
						for (qpl::u32 i = 2u; i < this->memory_size(); ++i) {
							this->memory[i] = qpl::u32_max;
						}
						return;
					}
				}
				for (qpl::u32 i = 2u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
				return;
			}
			else if constexpr (qpl::is_signed<T>()) {
				if (value < T{}) {
					for (qpl::u32 i = 1u; i < this->memory_size(); ++i) {
						this->memory[i] = qpl::u32_max;
					}
					return;
				}
			}
			for (qpl::u32 i = 1u; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u32{};
			}
		}

		template<qpl::size bits2, bool sign2>
		constexpr void set(qpl::integer<bits2, sign2> other) {
			for (qpl::u32 i = 0u; i < qpl::min(this->memory_size(), other.memory.size()); ++i) {
				this->memory[i] = other.memory[i];
			}
			for (qpl::u32 i = other.memory_size(); i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u32{};
			}
		}


#ifdef QPL_USE_INTRINSICS
		template<qpl::size bits2, bool sign2>
		constexpr void set(qpl::x64_integer<bits2, sign2> other) {
			if constexpr (this->bit_size() == other.bit_size()) {
				if (std::is_constant_evaluated()) {
					for (qpl::u32 i = 0u; i < this->memory_size(); i += 2) {
						this->memory[i] = qpl::u32_cast(other.memory[i >> 1]);
						this->memory[i + 1] = qpl::u32_cast(other.memory[i >> 1] >> qpl::bits_in_type<qpl::u32>());
					}
				}
				else {
					qpl::copy_memory(other.memory, this->memory);
				}
			}
			else {
				if (std::is_constant_evaluated()) {
					auto stop = other.last_used_index_u32() + 1u;
					for (qpl::u32 i = 0u; i < qpl::min(stop, this->memory_size()); ++i) {
						this->memory[i] = other.u32_at(i);
					}
					for (qpl::u32 i = stop; i < this->memory_size(); ++i) {
						this->memory[i] = qpl::u32{};
					}
				}
				else {
					qpl::copy_memory(other.memory, this->memory);
				}
			}
		}
#endif

		template<qpl::u32 base, bool sign2>
		constexpr void set(qpl::dynamic_integer<base, sign2> value) {
			if constexpr (sign2) {
				if (value.is_negative()) {
					if constexpr (is_unsigned()) {
						this->clear();
						return;
					}
				}
			}

			if constexpr (value.optimal_base()) {
				qpl::u32 i = 0u;
				for (; i < qpl::min(value.memory_size(), this->memory_size()); ++i) {
					this->memory[i] = value.content.memory[i];
				}
				for (; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{ 0u };
				}
			}
			else {
				this->set(qpl::u64_cast(value.content.memory[0u]));
				qpl::integer<bits, sign> mul = 1u;
				for (qpl::u32 i = 1u; i < value.memory_size(); ++i) {

					mul *= qpl::u32_cast(qpl::dynamic_integer<base, sign2>::base_max());
					this->add(qpl::u64_cast(value.content.memory[i]) * mul);
				}
			}


			if constexpr (sign2) {
				if (value.is_negative()) {
					this->flip_sign();
				}
			}
		}


		constexpr void set_hex_string(std::string_view string) {
			std::string_view substr;
			this->clear();

			bool negative = false;
			if (string.front() == '-') {
				negative = true;
			}

			if (string.length() >= 2 + negative) {
				substr = string.substr(negative, 2);
			}

			qpl::i32 off = negative;
			if (substr == "0x") {
				off += 2u;
			};

			qpl::u32 i = 0u;
			qpl::i32 ctr = 0u;
			for (; i < this->memory_size(); ++i) {
				qpl::i32 index = (qpl::i32_cast(string.length()) - qpl::i32_cast(i) * qpl::i32_cast(qpl::base_max_log<qpl::u32>(16u))) - qpl::i32_cast(qpl::base_max_log<qpl::u32>(16u));
				qpl::i32 size = qpl::i32_cast(qpl::base_max_log<qpl::u32>(16u));
				if (index < off) {
					size -= off - index;
				}
				index = qpl::max(off, index);

				auto str = string.substr(index, size);
				this->memory[i] = qpl::from_base_string(str, 16u);

				ctr += qpl::base_max_log<qpl::u32>(16u);

				if (index == off) {
					break;
				}
			}
			for (qpl::u32 j = i + 1; j < this->memory_size(); ++j) {
				this->memory[j] = qpl::u32{};
			}

			if constexpr (is_signed()) {
				if (negative) {
					this->flip_sign();
				}
			}
		}
		constexpr void set_binary_string(std::string_view string) {
			std::string_view substr;
			this->clear();


			bool negative = false;
			if (string.front() == '-') {
				negative = true;
			}

			if (string.length() >= 2 + negative) {
				substr = string.substr(negative, 2);
			}

			qpl::i32 off = negative;
			if (substr == "0b") {
				off += 2u;
			};

			qpl::u32 i = 0u;
			qpl::i32 ctr = 0u;
			for (; i < this->memory_size(); ++i) {
				qpl::i32 index = (qpl::i32_cast(string.length()) - qpl::i32_cast(i) * qpl::i32_cast(qpl::base_max_log<qpl::u32>(2u))) - qpl::i32_cast(qpl::base_max_log<qpl::u32>(2u));
				qpl::i32 size = qpl::i32_cast(qpl::base_max_log<qpl::u32>(2u));
				if (index < off) {
					size -= off - index;
				}
				index = qpl::max(off, index);

				auto str = string.substr(index, size);
				this->memory[i] = qpl::from_base_string(str, 2u);

				ctr += qpl::base_max_log<qpl::u32>(2u);

				if (index == off) {
					break;
				}
			}
			for (qpl::u32 j = i + 1; j < this->memory_size(); ++j) {
				this->memory[j] = qpl::u32{};
			}

			if constexpr (is_signed()) {
				if (negative) {
					this->flip_sign();
				}
			}
		}
		constexpr void set_base_string(std::string_view string, qpl::u32 base = 10u) {
			this->clear();

			bool negative = false;
			if (string.front() == '-') {
				negative = true;
			}

			std::string_view substr;
			if (string.length() >= 2 + negative) {
				substr = string.substr(negative, 2);
			}

			qpl::i32 off = negative;
			if (substr == "0x" || substr == "0b") {
				off += 2;
			};

			std::array<qpl::u32, memory_size() * 2> base_memory{};
			qpl::u32 used_memory = 0u;
			qpl::i32 ctr = 0u;
			for (; used_memory < base_memory.size(); ++used_memory) {
				qpl::i32 index = (qpl::i32_cast(string.length()) - qpl::i32_cast(used_memory * qpl::base_max_log<qpl::u32>(base))) - qpl::i32_cast(qpl::base_max_log<qpl::u32>(base));
				qpl::i32 size = qpl::i32_cast(qpl::base_max_log<qpl::u32>(base));
				if (index < off) {
					size -= off - index;
				}
				index = qpl::max(off, index);

				auto str = string.substr(index, size);
				base_memory[used_memory] = qpl::from_base_string(str, base, base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64);


				ctr += qpl::base_max_log<qpl::u32>(base);

				if (index == off) {
					break;
				}
			}


			auto multiply = qpl::base_max(base);
			qpl::integer<bits, sign> helper = 1;
			for (qpl::u32 i = 0u; i < used_memory + 1; ++i) {
				auto add = helper * base_memory[i];
				this->add(add);

				if (multiply) {
					helper.mul(multiply);
				}
				else {
					helper <<= base_max_log();
				}
			}

			if (negative) {
				this->flip_sign();
			}
		}
		constexpr void set_base_string_vector(std::string_view string, qpl::u32 base) {
			this->clear();
			bool negative = false;
			if (string.front() == '-') {
				negative = true;
			}

			std::string_view substr;
			if (string.length() >= 2 + negative) {
				substr = string.substr(negative, 2);
			}

			qpl::i32 off = negative;
			if (substr == "0x" || substr == "0b") {
				off += 2u;
			};

			//std::array<qpl::u32, memory_size() * 2> base_memory{} {
			//	for (qpl::u32 i = 0u; i < base_memory.size(); ++i) {
			//		base_memory[i] = qpl::u32{};
			//	}
			//};
			std::vector<qpl::u32> base_memory;
			qpl::i32 ctr = 0u;
			for (qpl::u32 i = 0u;; ++i) {
				qpl::i32 index = (qpl::i32_cast(string.length()) - qpl::i32_cast(i * qpl::base_max_log<qpl::u32>(base))) - qpl::i32_cast(qpl::base_max_log<qpl::u32>(base));
				qpl::i32 size = qpl::i32_cast(qpl::base_max_log<qpl::u32>(base));
				if (index < off) {
					size -= off - index;
				}
				index = qpl::max(off, index);

				auto str = string.substr(index, size);
				base_memory.push_back(qpl::from_base_string(str, base, base <= 64 ? qpl::base_format::base36l : qpl::base_format::base64));

				ctr += qpl::base_max_log<qpl::u32>(base);

				if (index == off) {
					break;
				}
			}

			auto multiply = qpl::base_max(base);
			qpl::integer<bits, sign> helper = 1;
			this->clear();
			for (qpl::u32 i = 0u; i < base_memory.size(); ++i) {
				this->add(helper * base_memory[i]);

				if (multiply) {
					helper.mul(multiply);
				}
				else {
					helper <<= base_max_log();
				}
			}
		}
		constexpr void set(std::string_view string, qpl::u32 base = qpl::u32_max) {
			qpl::u32 start = 0u;
			while (start < string.length() && qpl::is_character_white_space(string[start])) { ++start; }

			qpl::u32 unary_left = start;
			while (unary_left < string.length() && qpl::is_character_unary_left_operator(string[unary_left])) { ++unary_left; }

			qpl::u32 special = unary_left;
			while (special < string.length() && !qpl::is_character_arithmetic_special(string[special])) { ++special; }

			bool single_variable = (special == string.length());

			while (special && qpl::is_character_white_space(string[special - 1])) { --special; }

			if (!single_variable) {
				*this = qpl::parse_expression<qpl::integer<bits, sign>>(string);
			}
			else {
				if (base != qpl::u32_max) {
					this->set_base_string(string, base);
					return;
				}

				std::string_view substr;

				bool negative = string.front() == '-';

				if (string.length() >= 2 + negative) {
					substr = string.substr(negative, 2);
				}

				if (substr == "0x") {
					this->set_hex_string(string);
				}
				else if (substr == "0b") {
					this->set_binary_string(string);
				}
				else if (qpl::string_contains_exclusively(string.substr(negative), "0-9")) {
					this->set_base_string(string, 10);
				}
				else {
					this->set_base_string(string, 64);
				}
			}
		}

		constexpr void clear() {
			for (qpl::u32 i = 0; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u32{};
			}
		}
		constexpr void clear_flip() {
			for (qpl::u32 i = 0; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u32_max;
			}
		}

		template<typename T>
		constexpr void add(T value) {

			if constexpr (qpl::is_signed<T>()) {
				if (value < T{}) {
					this->sub(~value);
					this->decrement();
					return;
				}
			}

			if constexpr (qpl::is_integer<T>() && qpl::bits_in_type<T>() == qpl::bits_in_type<qpl::u64>()) {
				qpl::u64 m = qpl::u64_cast(qpl::u32_cast(value)) + qpl::u64_cast(this->memory[0]);
				this->memory[0] = qpl::u32_cast(m);
				m >>= base_max_log();

				m += qpl::u64_cast(value >> base_max_log());
				for (qpl::u32 i = 1u; i < this->memory_size(); ++i) {
					m += qpl::u64_cast(this->memory[i]);
					this->memory[i] = qpl::u32_cast(m);
					m >>= base_max_log();
					if (m == 0) {
						return;
					}
				}
			}
			else {
				qpl::u64 m = value;
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					m += qpl::u64_cast(this->memory[i]);
					this->memory[i] = qpl::u32_cast(m);
					m >>= base_max_log();
					if (m == qpl::u64{}) {
						return;
					}
				}
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr void add(qpl::integer<bits2, sign2> other, qpl::u32 at_position = 0u) {
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (my_neg && other_neg) {
					this->flip_sign();
					other.flip_sign();
					this->add(other);
					this->flip_sign();
					return;
				}
				else if (other_neg) {
					other.flip_sign();
					this->sub(other);
					return;
				}
			}
			if (other.last_used_index() < 1) {
				this->add(qpl::u32_cast(other));
				return;
			}

			qpl::u64 m = 0;

			for (qpl::u32 i = 0u; i < qpl::min(this->memory_size(), other.memory.size()) - at_position; ++i) {
				m += qpl::u64_cast(other.memory[i]) + qpl::u64_cast(this->memory[i + at_position]);
				this->memory[i + at_position] = qpl::u32_cast(m);
				m >>= base_max_log();
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool add_check_overflow(qpl::integer<bits2, sign2> other, qpl::u32 at_position = 0u) {
			if constexpr (is_signed()) {
				if (other.is_negative()) {
					if (this->is_negative()) {

						qpl::u64 m = 0;

						for (qpl::u32 i = 0u; i < qpl::min(this->memory_size(), other.memory.size()) - at_position; ++i) {
							m += qpl::u64_cast(other.memory[i]) + qpl::u64_cast(this->memory[i + at_position]);
							this->memory[i + at_position] = qpl::u32_cast(m);
							m >>= base_max_log();
						}
						return qpl::bool_cast(m);
					}
					else {
						this->sub(-other);
						return false;
					}
				}
				else {
					qpl::u64 m = 0;

					for (qpl::u32 i = 0u; i < qpl::min(this->memory_size(), other.memory.size()) - at_position; ++i) {
						m += qpl::u64_cast(other.memory[i]) + qpl::u64_cast(this->memory[i + at_position]);
						this->memory[i + at_position] = qpl::u32_cast(m);
						m >>= base_max_log();
					}
					return qpl::bool_cast(m);
				}
			}
			else {
				qpl::u64 m = 0;

				for (qpl::u32 i = 0u; i < qpl::min(this->memory_size(), other.memory.size()) - at_position; ++i) {
					m += qpl::u64_cast(other.memory[i]) + qpl::u64_cast(this->memory[i + at_position]);
					this->memory[i + at_position] = qpl::u32_cast(m);
					m >>= base_max_log();
				}
				return qpl::bool_cast(m);
			}

		}


		template<typename T>
		constexpr void mul(T value) {
			if constexpr (qpl::is_signed<T>()) {
				if (value < T{}) {
					this->mul(-value);
					this->flip_sign();
					return;
				}
			}

			if constexpr (qpl::is_integer<T>() && qpl::bits_in_type<T>() == qpl::bits_in_type<qpl::u64>()) {
				this->mul(qpl::integer<bits, sign>{ value });
			}
			else {
				if constexpr (is_signed()) {
					if (this->is_negative()) {
						qpl::u64 m = 1u;
						auto stop = this->last_used_inverted_index();
						for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
							if (!m && i > stop) {
								return;
							}
							m += qpl::u64_cast(~this->memory[i]) * value;
							this->memory[i] = ~qpl::u32_cast(m);
							m >>= base_max_log();
						}
						this->decrement();
						return;
					}
				}
				qpl::u64 m = 0u;
				auto stop = this->last_used_index();
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					if (!m && i > stop) {
						return;
					}

					m += qpl::u64_cast(this->memory[i]) * value;
					this->memory[i] = qpl::u32_cast(m);
					m >>= base_max_log();
				}
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr void mul(qpl::integer<bits2, sign2> other) {

			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (other_neg && my_neg) {
					this->flip_sign();
					other.flip_sign();
					this->mul(other);
					return;
				}
				else if (other_neg) {
					other.flip_sign();
					this->mul(other);
					this->flip_sign();
					return;
				}
				else if (my_neg) {
					this->flip_sign();
					this->mul(other);
					this->flip_sign();
					return;
				}
			}
			if (other.last_used_index() < 1) {
				this->mul(qpl::u32_cast(other));
				return;
			}
			qpl::integer<bits, sign> result;
			result.clear();

			auto other_non_zero = other.last_used_index();
			auto my_non_zero = this->last_used_index();


			qpl::integer<bits, sign> add;

			if (other_non_zero < my_non_zero) {
				qpl::integer<bits, sign> mul = *this;
				for (qpl::u32 i = 0u; i <= other_non_zero; ++i) {
					add = mul;
					add.mul(other.memory[i]);
					result.add(add, i);

				}
			}
			else {
				qpl::integer<bits, sign> mul = other;
				for (qpl::u32 i = 0u; i <= my_non_zero; ++i) {
					add = mul;
					add.mul(this->memory[i]);
					result.add(add, i);
				}
			}
			*this = result;
		}
		template<qpl::size bits2, bool sign2>
		constexpr void mul_left_side(qpl::integer<bits2, sign2> other) {

			qpl::integer<bits * 2, sign> result;
			result.clear();

			auto other_non_zero = other.last_used_index();
			auto my_non_zero = this->last_used_index();


			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (other_neg && my_neg) {
					this->flip_sign();
					other.flip_sign();
					this->mul_left_side(other);
					return;
				}
				else if (other_neg) {
					other.flip_sign();
					this->mul_left_side(other);
					this->flip_sign();
					return;
				}
				else if (my_neg) {
					this->flip_sign();
					this->mul_left_side(other);
					this->flip_sign();
					return;
				}
			}


			qpl::integer<bits * 2, sign> add;


			if (other_non_zero < my_non_zero) {
				qpl::integer<bits * 2, sign> mul = *this;
				for (qpl::u32 i = 0u; i <= other_non_zero; ++i) {
					add = mul;
					add.mul(other.memory[i]);
					result.add(add, i);
				}
			}
			else {
				qpl::integer<bits * 2, sign> mul = other;
				for (qpl::u32 i = 0u; i <= my_non_zero; ++i) {
					add = mul;
					add.mul(this->memory[i]);
					result.add(add, i);
				}
			}


			for (qpl::u32 i = 0u; i < memory_size(); ++i) {
				this->memory[i] = result.memory[i + memory_size()];
			}
		}


		template<typename T>
		constexpr void sub(T value) {
			if constexpr (qpl::is_signed<T>()) {
				if (value < T{}) {
					this->add(~value);
					this->increment();
					return;
				}
			}
			if constexpr (is_signed()) {
				if (this->is_positive() && value < this->memory[0]) {
					this->memory[0] -= value;
				}
				else {
					auto other = qpl::integer<bits, sign>(value);
					if (*this < other) {
						*this = other - *this;
						this->flip_sign();
					}
					else {
						qpl::u32 over = 0u;
						qpl::size other_stop = other.last_used_index() + 1;

						for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
							bool less = this->memory[i] < other.memory[i];
							this->memory[i] = (this->memory[i] - other.memory[i] - over);
							over = less ? 1u : 0u;

							if (i >= other_stop && !less) {
								break;
							}
						}
					}
				}
			}
			else {
				if (value < this->memory[0]) {
					this->memory[0] -= value;
				}
				else {
					auto other = qpl::integer<bits, sign>(value);
					if (*this < other) {
						*this = other - *this;
						this->flip_sign();
					}
					else {
						qpl::u32 over = 0u;
						qpl::size other_stop = other.last_used_index() + 1;

						for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
							bool less = this->memory[i] < other.memory[i];
							this->memory[i] = (this->memory[i] - other.memory[i] - over);
							over = less ? 1u : 0u;

							if (i >= other_stop && !less) {
								break;
							}
						}
					}
				}
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr void sub(qpl::integer<bits2, sign2> other) {
			if (other.is_unsigned() && other.last_used_index() < 1) {
				this->sub(qpl::u32_cast(other));
				return;
			}

			if constexpr (other.is_signed()) {
				if (other.is_negative()) {
					other.flip_sign();
					this->add(other);
					return;
				}
			}
			if (*this < other) {
				*this = (other - *this);
				this->flip_sign();
				return;
			}

			qpl::u32 over = 0u;
			qpl::size other_stop = other.last_used_index() + 1;

			qpl::u32 i = 0u;
			for (; i < qpl::min(other.memory_size(), this->memory_size()); ++i) {
				bool less = this->memory[i] < (other.memory[i] + over);

				this->memory[i] = (this->memory[i] - other.memory[i] - over);
				over = less ? 1u : 0u;

				if (i >= other_stop && !less) {
					break;
				}
			}

			for (; over && i < this->memory_size(); ++i) {
				auto less = !this->memory[i];

				--this->memory[i];
				over = less ? 1u : 0u;
			}
		}


		constexpr integer quick_dm_shifted_bits(qpl::u32 begin, qpl::size size) const {
			integer result;

			auto index_begin = begin / base_max_log();
			qpl::size index_end = size / base_max_log();
			auto mask = size % base_max_log();
			auto bit = begin % base_max_log();


			qpl::u32 i = 0u;

			if (mask) {
				if (bit) {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = (this->memory[i + index_begin] >> bit) | (this->memory[i + index_begin + 1] << (this->base_max_log() - bit));
					}
					if (i + index_begin + 1 < this->memory_size()) {
						result.memory[i] = ((this->memory[i + index_begin] >> bit) | (this->memory[i + index_begin + 1] << (this->base_max_log() - bit))) & (qpl::u32_max >> (this->base_max_log() - mask));
					}
					else {
						result.memory[i] = (this->memory[i + index_begin] >> bit) & (qpl::u32_max >> (this->base_max_log() - mask));
					}
				}
				else {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = this->memory[i + index_begin];
					}
					result.memory[i] = this->memory[i + index_begin] & (qpl::u32_max >> (this->base_max_log() - mask));
				}
			}
			else {
				if (bit) {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = (this->memory[i + index_begin] >> bit) | (this->memory[i + index_begin + 1] << (this->base_max_log() - bit));
					}
				}
				else {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = this->memory[i + index_begin];
					}
				}
			}

			return result;
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool quick_dm_less(qpl::integer<bits2, sign2> other, qpl::size index) const {

			auto my_non = this->last_used_index(index);
			auto other_non = other.last_used_index(index);


			if (my_non != other_non) {
				return my_non < other_non;
			}

			for (qpl::i32 i = my_non; i > 0; --i) {
				if (this->memory[i] < other.memory[i]) {
					return true;
				}
				else if (this->memory[i] > other.memory[i]) {
					return false;
				}
			}
			return this->memory[0u] < other.memory[0u];
		}
		void quick_dm_sub(integer other, qpl::size index) {

			qpl::u32 over = 0u;
			qpl::size other_stop = other.last_used_index(index) + 1;

			qpl::u32 i = 0u;
			for (; i < qpl::min(other.memory_size(), this->memory_size()); ++i) {
				bool less = this->memory[i] < (other.memory[i] + over);

				this->memory[i] = (this->memory[i] - other.memory[i] - over);
				over = less ? 1u : 0u;

				if (i >= other_stop && !less) {
					break;
				}
			}


			//for (; over && i < this->memory_size(); ++i) {
			//	auto less = !this->memory[i];
			//
			//	--this->memory[i];
			//	over = less ? 1u : 0u;
			//}
		}

		template<typename T>
		constexpr void div(T value) {
			qpl::u64 m = 0;
			for (qpl::i32 i = qpl::i32_cast(this->memory_size()) - 1; i >= 0; --i) {
				m |= qpl::u64_cast(this->memory[i]);

				auto div = m / qpl::u64_cast(value);
				m %= qpl::u64_cast(value);
				m <<= base_max_log();

				this->memory[i] = qpl::u32_cast(div);
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr void div(qpl::integer<bits2, sign2> other) {
			this->set(this->dived(other));
		}


		constexpr integer dived2(integer other) const {
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (other_neg && my_neg) {
					integer result = *this;
					result.flip_sign();
					other.flip_sign();
					return result.dived(other);
				}
				else if (other_neg) {
					integer result = *this;
					other.flip_sign();
					result.div(other);
					result.flip_bits();
					return result;
				}
				else if (my_neg) {
					integer result = *this;
					result.flip_sign();
					result.div(other);
					result.flip_bits();
					return result;
				}
			}


			integer div;
			div.clear();

			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (other.is_zero()) {
				return div;
			}
			if (my_position < reduce) {
				return div;
			}

			auto shift = my_position - reduce + 1;


			integer mod = *this;
			mod >>= shift;


			integer shift_bits;

			qpl::u32 msb = reduce - 1;
			qpl::u32 look_index = qpl::min(this->memory_size() - 1, (reduce + 1) / this->base_max_log());

			for (qpl::i32 i = shift - 1; i >= 0; --i) {

				qpl::i32 skip = qpl::min<qpl::i32>(i, (qpl::signed_cast(reduce) - qpl::signed_cast(msb)));

				auto skippable = (skip > 0);
				if (skippable) {

					mod.left_shift(skip);
					shift_bits = this->quick_dm_shifted_bits(i - (skip - 1), skip);

					auto skip_memory = (skip - 1) / this->base_max_log();
					for (qpl::u32 b = 0u; b < skip_memory; ++b) {
						mod.memory[b] = shift_bits.memory[b];
					}
					mod.memory[skip_memory] |= shift_bits.memory[skip_memory];

					i -= (skip - 1);
					msb += skip;

				}
				else {
					mod <<= 1;
					mod.set_first_bit(this->get_bit(i));
				}


				auto nle = !(mod.quick_dm_less(other, look_index));

				if (nle) {
					mod.quick_dm_sub(other, look_index);
					msb = mod.significant_bit(look_index) - 1;

					div.set_bit(i, qpl::u32{ 1 });
				}
			}

			return div;
		}
		constexpr integer dived(integer other) const {
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (other_neg && my_neg) {
					integer result = *this;
					result.flip_sign();
					other.flip_sign();
					return result.dived(other);
				}
				else if (other_neg) {
					integer result = *this;
					other.flip_sign();
					result.div(other);
					result.flip_bits();
					return result;
				}
				else if (my_neg) {
					integer result = *this;
					result.flip_sign();
					result.div(other);
					result.flip_bits();
					return result;
				}
			}


			integer div;
			div.clear();

			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (other.is_zero()) {
				return div;
			}
			if (my_position < reduce) {
				return div;
			}

			auto shift = my_position - reduce + 1;

			integer mod = *this;
			mod >>= shift;
			for (qpl::i32 i = shift - 1; i >= 0; --i) {
				mod <<= 1;

				mod.set_first_bit(this->get_bit(i));

				auto nle = mod >= other;

				if (nle) {
					mod -= other;

					div.set_bit(i, qpl::u32{ 1 });
				}
			}

			return div;
		}

		template<qpl::size bits2, bool sign2>
		constexpr void div_right_side(qpl::integer<bits2, sign2> other) {
			qpl::integer<bits * 2, sign> copy;

			for (qpl::u32 i = 0u; i < memory_size(); ++i) {
				copy.memory[i] = qpl::u32{};
			}
			for (qpl::u32 i = 0u; i < memory_size(); ++i) {
				copy.memory[i + memory_size()] = this->memory[i];
			}
			copy.div(other);
			for (qpl::u32 i = 0u; i < memory_size(); ++i) {
				this->memory[i] = copy.memory[i];
			}
		}



		constexpr void mod(integer other) {
			this->set(this->moded(other));
		}
		constexpr integer moded2(integer other) const {
			if constexpr (is_signed()) {
				auto other_negative = other.is_negative();
				auto my_negative = this->is_negative();

				if (my_negative && other_negative) {
					integer result = *this;
					result.flip_sign();
					other.flip_sign();
					result.mod(other);
					result.flip_sign();
					return result;
				}
				if (my_negative) {
					integer result = *this;
					result.flip_sign();
					result.mod(other);
					return other - result;
				}
				if (other_negative) {
					integer result = *this;
					other.flip_sign();
					result.mod(other);
					result = other - result;
					result.flip_sign();
					return result;
				}
			}


			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (my_position < reduce) {
				return *this;
			}

			auto shift = my_position - reduce + 1;

			integer mod;
			mod = *this;
			mod >>= shift;

			integer shift_bits;

			qpl::u32 msb = reduce - 1;
			qpl::u32 look_index = qpl::min(this->memory_size() - 1, (reduce + 1) / this->base_max_log());

			for (qpl::i32 i = shift - 1; i >= 0; --i) {

				qpl::i32 skip = qpl::min<qpl::i32>(i, (qpl::signed_cast(reduce) - qpl::signed_cast(msb)));

				auto skippable = (skip > 0);

				if (skippable) {

					mod.left_shift(skip);
					shift_bits = this->quick_dm_shifted_bits(i - (skip - 1), skip);

					auto skip_memory = (skip - 1) / this->base_max_log();
					for (qpl::u32 b = 0u; b < skip_memory; ++b) {
						mod.memory[b] = shift_bits.memory[b];
					}
					mod.memory[skip_memory] |= shift_bits.memory[skip_memory];

					i -= (skip - 1);
					msb += skip;

				}
				else {
					mod <<= 1;
					mod.set_first_bit(this->get_bit(i));
				}


				auto nle = !(mod.quick_dm_less(other, look_index));

				if (nle) {
					mod.quick_dm_sub(other, look_index);
					msb = mod.significant_bit(look_index) - 1;
				}
			}
			return mod;
		}
		constexpr integer moded(integer other) const {
			if constexpr (is_signed()) {
				auto other_negative = other.is_negative();
				auto my_negative = this->is_negative();

				if (my_negative && other_negative) {
					integer result = *this;
					result.flip_sign();
					other.flip_sign();
					result.mod(other);
					result.flip_sign();
					return result;
				}
				if (my_negative) {
					integer result = *this;
					result.flip_sign();
					result.mod(other);
					return other - result;
				}
				if (other_negative) {
					integer result = *this;
					other.flip_sign();
					result.mod(other);
					result = other - result;
					result.flip_sign();
					return result;
				}
			}


			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (my_position < reduce) {
				return *this;
			}

			auto shift = my_position - reduce + 1;

			integer mod;
			mod = *this;
			mod >>= shift;

			for (qpl::i32 i = shift - 1; i >= 0; --i) {
				mod <<= 1;
				mod.set_first_bit(this->get_bit(i));

				if (mod >= other) {
					mod -= other;
				}
			}
			return mod;
		}

		template<typename T>
		constexpr std::pair<qpl::integer<bits, sign>, qpl::integer<bits, sign>> div_mod(T value) const {
			return this->div_mod(qpl::integer<bits, sign>(value));
		}

		template<qpl::size bits2, bool sign2>
		constexpr std::pair<qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>>, qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>>> div_mod(qpl::integer<bits2, sign2> other) const {
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> div;
			div.clear();
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> mod;

			if constexpr (is_signed()) {
				auto other_negative = other.is_negative();
				auto my_negative = this->is_negative();

				if (my_negative && other_negative) {
					auto neg = -*this;
					auto result = neg.div_mod(-other);
					result.second.flip_sign();
					return result;
				}
				if (my_negative) {
					qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> negated = *this;
					negated.flip_sign();

					auto reduce = other.significant_bit();
					auto my_position = negated.significant_bit();

					if (reduce) --reduce;
					if (my_position) --my_position;

					if (my_position < reduce) {
						div.clear_flip();

						mod = *this;
						mod.mod(other);
						return std::make_pair(div, mod);
					}

					auto shift = bit_size() - reduce;
					mod = negated;
					mod >>= shift;

					for (qpl::i32 i = shift - 1; i >= 0; --i) {
						mod <<= 1;
						mod.set_first_bit(negated.get_bit(i));

						if (mod >= other) {
							mod -= other;
							div.set_bit(i, qpl::u32{ 1 });
						}
					}
					div.flip_bits();
					mod = other - mod;
					return std::make_pair(div, mod);
				}
				if (other_negative) {
					qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> negated = -other;

					auto reduce = negated.significant_bit();
					auto my_position = this->significant_bit();


					if (reduce) --reduce;
					if (my_position) --my_position;

					if (my_position < reduce) {
						div.clear_flip();

						mod = *this;
						mod.mod(other);
						return std::make_pair(div, mod);
					}

					auto shift = bit_size() - reduce;
					mod = *this;
					mod >>= shift;

					for (qpl::i32 i = shift - 1; i >= 0; --i) {
						mod <<= 1;
						mod.set_first_bit(this->get_bit(i));

						if (mod >= negated) {
							mod -= negated;
							div.set_bit(i, qpl::u32{ 1 });
						}
					}
					div.flip_bits();
					mod = negated - mod;
					mod.flip_sign();
					return std::make_pair(div, mod);
				}
			}

			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (my_position < reduce) {
				mod = *this;
				return std::make_pair(div, mod);
			}

			auto shift = bit_size() - reduce;
			mod = *this;

			mod >>= shift;

			for (qpl::i32 i = shift - 1; i >= 0; --i) {
				mod <<= 1;
				mod.set_first_bit(this->get_bit(i));

				if (mod >= other) {
					mod -= other;
					div.set_bit(i, qpl::u32{ 1 });
				}
			}
			return std::make_pair(div, mod);
		}



		template<typename T>
		constexpr bool equals(T value) const {
			if constexpr (qpl::bits_in_type<T>() <= base_max_log()) {
				if (this->memory[0] != qpl::u32_cast(value)) {
					return false;
				}
			}
			else if constexpr (qpl::bits_in_type<T>() == qpl::bits_in_type<qpl::u64>()) {
				if constexpr (memory_size() == 1u) {
					if constexpr (is_signed()) {
						if (this->is_negative()) {
							return (this->memory[0] == qpl::u32_cast(value) && qpl::u32_max == qpl::u32_cast(value >> base_max_log()));
						}
					}
					return (this->memory[0] == qpl::u32_cast(value) && qpl::u32{} == qpl::u32_cast(value >> base_max_log()));
				}
				else if constexpr (memory_size() == 2u) {
					return this->memory[0] == qpl::u32_cast(value) && this->memory[1] == qpl::u32_cast(value >> base_max_log());
				}
				else {
					if (this->memory[0] != qpl::u32_cast(value) || this->memory[1] != qpl::u32_cast(value >> base_max_log())) {
						return false;
					}
				}
			}
			qpl::size pos = 0u;
			if constexpr (is_signed()) {
				bool invert = this->is_negative();
				if (invert) {
					pos = this->last_used_inverted_index() + 1u;
				}
				else {
					pos = this->last_used_index() + 1u;
				}
			}
			else {
				pos = this->last_used_index() + 1u;
			}

			return (pos <= ((qpl::bits_in_type<T>() - 1) / base_max_log()) + 1);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool equals(qpl::integer<bits2, sign2> other) const {
			if (this->memory[0] != other.memory[0]) {
				return false;
			}

			auto my_non = this->last_used_index();
			auto other_non = other.last_used_index();

			if (my_non != other_non) {
				return false;
			}

			for (qpl::u32 i = 1u; i < my_non + 1; ++i) {
				if (this->memory[i] != other.memory[i]) {
					return false;
				}
			}
			return true;
		}

#ifdef QPL_USE_INTRINSICS
		template<qpl::size bits2, bool sign2>
		constexpr bool equals(qpl::x64_integer<bits2, sign2> other) const {
			for (qpl::u32 i = 0u; i < other.memory_size() && ((i << 1) < this->memory_size()); ++i) {
				if ((i << 1) == this->memory_size() - 1) {
					if (this->memory[i << 1] != qpl::u32_cast(other.memory[i])) {
						return false;
					}
				}
				else {
					if (this->memory[i << 1] != qpl::u32_cast(other.memory[i])) {
						return false;
					}
					if (this->memory[(i << 1) | 0x1u] != qpl::u32_cast(other.memory[i] >> base_max_log())) {
						return false;
					}
				}
			}
			return true;
		}
#endif

		template<typename T>
		constexpr bool less(T value) const {
			qpl::size pos = 0;
			bool negative = false;
			if constexpr (is_signed()) {
				negative = this->is_negative();
				if constexpr (qpl::is_signed<T>()) {
					if (value < T{}) {
						if (!negative) {
							return false;
						}
					}
					else if (negative) {
						return true;
					}
				}
				else {
					if (negative) {
						return true;
					}
				}
			}

			if constexpr (qpl::bits_in_type<T>() <= base_max_log()) {
				if (this->memory[0] >= qpl::u32_cast(value)) {
					return false;
				}
			}
			else if constexpr (qpl::bits_in_type<T>() == qpl::bits_in_type<qpl::u64>()) {
				if constexpr (memory_size() == 1u) {
					return (qpl::u32{} == qpl::u32_cast(value >> base_max_log()) && this->memory[0] < qpl::u32_cast(value));
				}
				else if constexpr (memory_size() == 2u) {
					return this->memory[1] <= qpl::u32_cast(value >> base_max_log()) && this->memory[0] < qpl::u32_cast(value);
				}
				else {
					if (this->memory[1] > qpl::u32_cast(value >> base_max_log())) {
						return false;
					}
					else if (this->memory[1] == qpl::u32_cast(value >> base_max_log()) && this->memory[0] >= qpl::u32_cast(value)) {
						return false;
					}
				}
			}


			if (negative) {
				pos = this->last_used_inverted_index() + 1;
			}
			else {
				pos = this->last_used_index() + 1;
			}
			return (pos <= ((qpl::bits_in_type<T>() - 1) / base_max_log()) + 1);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool less(qpl::integer<bits2, sign2> other) const {
			if constexpr (is_signed()) {
				auto neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (neg && !other_neg) {
					return true;
				}
				if (!neg && other_neg) {
					return false;
				}
			}

			auto my_non = this->last_used_index();
			auto other_non = other.last_used_index();


			if (my_non != other_non) {
				return my_non < other_non;
			}

			for (qpl::i32 i = my_non; i > 0; --i) {
				if (this->memory[i] < other.memory[i]) {
					return true;
				}
				else if (this->memory[i] > other.memory[i]) {
					return false;
				}
			}
			return this->memory[0] < other.memory[0];
		}

		template<typename T>
		constexpr bool greater(T value) const {
			bool invert = false;
			if constexpr (is_signed()) {
				invert = this->is_negative();
				if constexpr (qpl::is_signed<T>()) {
					if (value < T{}) {
						if (!invert) {
							return true;
						}
					}
					else if (invert) {
						return false;
					}
				}
				else {
					if (invert) {
						return false;
					}
				}
			}

			if constexpr (qpl::bits_in_type<T>() <= base_max_log()) {
				if (this->memory[0] <= qpl::u32_cast(value)) {
					return false;
				}
			}
			else if constexpr (qpl::bits_in_type<T>() == qpl::bits_in_type<qpl::u64>()) {
				if constexpr (memory_size() == 1u) {
					return (qpl::u32{} == qpl::u32_cast(value >> base_max_log()) && this->memory[0] > qpl::u32_cast(value));
				}
				else if constexpr (memory_size() == 2u) {
					return this->memory[1] >= qpl::u32_cast(value >> base_max_log()) && this->memory[0] > qpl::u32_cast(value);
				}
				else {
					if (this->memory[1] < qpl::u32_cast(value >> base_max_log())) {
						return false;
					}
					else if (this->memory[1] == qpl::u32_cast(value >> base_max_log()) && this->memory[0] <= qpl::u32_cast(value)) {
						return false;
					}
				}
			}

			qpl::size pos = 0;
			if (invert) {
				pos = this->last_used_inverted_index() + 1;
			}
			else {
				pos = this->last_used_index() + 1;
			}
			return (pos <= ((qpl::bits_in_type<T>() - 1) / base_max_log()) + 1);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool greater(qpl::integer<bits2, sign2> other) const {
			if constexpr (is_signed()) {
				auto neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (neg && !other_neg) {
					return false;
				}
				if (!neg && other_neg) {
					return true;
				}
			}

			auto my_non = this->last_used_index();
			auto other_non = other.last_used_index();

			if (my_non != other_non) {
				return my_non > other_non;
			}

			for (qpl::i32 i = my_non; i > 0; --i) {
				if (this->memory[i] < other.memory[i]) {
					return false;
				}
				else if (this->memory[i] > other.memory[i]) {
					return true;
				}
			}
			return this->memory[0] > other.memory[0];
		}


		template<typename T>
		constexpr void bitwise_or(T value) {
			if constexpr (qpl::bits_in_type<T>() <= base_max_log()) {
				this->memory[0u] |= qpl::u32_cast(value);

				for (qpl::u32 i = 1u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
			}
			else if constexpr (qpl::bits_in_type<T>() == qpl::bits_in_type<qpl::u64>()) {
				this->memory[0u] |= qpl::u32_cast(value);
				this->memory[1u] |= qpl::u32_cast(value >> base_max_log());

				for (qpl::u32 i = 2u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
			}
			else {
				this->bitwise_or(qpl::integer<bits, sign>(value));
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr void bitwise_or(qpl::integer<bits2, sign2> other) {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				this->memory[i] |= other.memory[i];
			}
		}

		template<typename T>
		constexpr void bitwise_and(T value) {
			if constexpr (qpl::bits_in_type<T>() <= base_max_log()) {
				this->memory[0u] &= qpl::u32_cast(value);

				for (qpl::u32 i = 1u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
			}
			else if constexpr (qpl::bits_in_type<T>() == qpl::bits_in_type<qpl::u64>()) {
				this->memory[0u] &= qpl::u32_cast(value);
				this->memory[1u] &= qpl::u32_cast(value >> base_max_log());

				for (qpl::u32 i = 2u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
			}
			else {
				this->bitwise_and(qpl::integer<bits, sign>(value));
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr void bitwise_and(qpl::integer<bits2, sign2> other) {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				this->memory[i] &= other.memory[i];
			}
		}

		template<typename T>
		constexpr void bitwise_xor(T value) {
			if constexpr (qpl::bits_in_type<T>() <= 32) {
				this->memory[0] ^= qpl::u32_cast(value);

				for (qpl::u32 i = 1u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
			}
			else if constexpr (qpl::bits_in_type<T>() == 64) {
				this->memory[0] ^= qpl::u32_cast(value);
				this->memory[1] ^= qpl::u32_cast(value >> 32);

				for (qpl::u32 i = 2u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
			}
			else {
				this->bitwise_xor(qpl::integer<bits, sign>(value));
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr void bitwise_xor(qpl::integer<bits2, sign2> other) {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				this->memory[i] ^= other.memory[i];
			}
		}

		template<bool clear = true>
		constexpr void left_shift(qpl::i64 shift) {
			if (shift == 0) {
				return;
			}
			if (shift < 0) {
				this->right_shift(-shift);
				return;
			}
			auto div = shift / base_max_log();
			auto mod = shift % base_max_log();

			if (div >= this->memory_size()) {
				this->clear();
				return;
			}

			if (mod) {
				auto start = qpl::min(qpl::size_cast(this->last_used_index() + div + 1), this->memory_size() - 1);

				for (qpl::i32 i = qpl::i32_cast(start); i > qpl::i32_cast(div); --i) {
					this->memory[i] = qpl::u32_cast(((this->memory[i - div] << mod) | (this->memory[i - div - 1] >> (base_max_log() - mod))));
				}
				this->memory[div] = qpl::u32_cast(this->memory.front() << mod);
			}
			else {
				if (div) {
					auto start = qpl::min(qpl::size_cast(this->last_used_index() + div + 1), this->memory_size() - 1);

					for (qpl::i32 i = qpl::i32_cast(start); i >= div; --i) {
						this->memory[i] = this->memory[i - div];
					}
				}
			}
			if constexpr (clear) {
				for (qpl::i32 i = 0u; i < qpl::i32_cast(div); ++i) {
					this->memory[i] = qpl::u32{};
				}
			}

		}

		template<bool clear = true>
		constexpr void right_shift(qpl::i64 shift) {
			if (shift == 0) {
				return;
			}
			if (shift < 0) {
				this->left_shift(-shift);
				return;
			}

			auto div = shift / base_max_log();
			auto mod = shift % base_max_log();

			if (div >= this->memory_size()) {
				this->clear();
				return;
			}

			if constexpr (is_signed()) {
				bool has_sign = this->is_negative();

				if (mod) {

					auto stop = qpl::min<qpl::size>(this->last_used_index(), this->memory_size() - 1);
					if (div > stop) {
						if constexpr (clear) {
							this->clear();
						}
						return;
					}

					for (qpl::i32 i = 0u; i < qpl::i32_cast(stop - div); ++i) {
						this->memory[i] = qpl::u32_cast(((this->memory[i + div] >> mod) | (this->memory[i + div + 1] << (base_max_log() - mod))));
					}
					this->memory[stop - div] = qpl::u32_cast(this->memory[stop] >> mod);


					if constexpr (clear) {
						if (has_sign) {
							for (qpl::i32 i = qpl::i32_cast(stop - div) + 1; i < this->memory_size(); ++i) {
								this->memory[i] = qpl::u32_max;
							}
						}
						else {
							for (qpl::i32 i = qpl::i32_cast(stop - div) + 1; i < this->memory_size(); ++i) {
								this->memory[i] = qpl::u32{};
							}
						}
					}
				}
				else {
					if (div) {
						for (qpl::u32 i = 0u; i < qpl::u32_cast(this->memory_size()) - div; ++i) {
							this->memory[i] = this->memory[i + div];
						}
						if constexpr (clear) {
							if (has_sign) {
								for (qpl::u32 i = this->memory_size() - div; i < this->memory_size(); ++i) {
									this->memory[i] = qpl::u32_max;
								}
							}
							else {
								for (qpl::u32 i = this->memory_size() - div; i < this->memory_size(); ++i) {
									this->memory[i] = qpl::u32{};
								}
							}
						}
					}
				}

				if (has_sign) {
					this->memory[this->memory_size() - div - 1] |= (qpl::u32_max << (base_max_log() - mod - 1));
				}
			}
			else {
				if (mod) {
					auto stop = qpl::min<qpl::size>(this->last_used_index(), this->memory_size() - 1);
					if (div > stop) {
						if constexpr (clear) {
							this->clear();
						}
						return;
					}

					for (qpl::i32 i = 0u; i < qpl::i32_cast(stop - div); ++i) {
						this->memory[i] = qpl::u32_cast(((this->memory[i + div] >> mod) | (this->memory[i + div + 1] << (base_max_log() - mod))));
					}
					this->memory[stop - div] = qpl::u32_cast(this->memory[stop] >> mod);


					if constexpr (clear) {
						for (qpl::i32 i = qpl::i32_cast(stop) - qpl::i32_cast(div) + 1; i < this->memory_size(); ++i) {
							this->memory[i] = qpl::u32{};
						}
					}
				}
				else {
					if (div) {
						for (qpl::i32 i = 0u; i < qpl::i32_cast(this->memory_size()) - qpl::i32_cast(div); ++i) {
							this->memory[i] = this->memory[i + div];
						}
						if constexpr (clear) {
							for (qpl::i32 i = qpl::i32_cast(this->memory_size()) - qpl::i32_cast(div); i < this->memory_size(); ++i) {
								this->memory[i] = qpl::u32{};
							}
						}
					}
				}
			}
		}

		constexpr integer factorial() const {
			integer result = *this;
			for (qpl::u64 i = 2u; i < *this; ++i) {
				result.mul(i);
			}
			return result;
		}
		template<typename T>
		constexpr static integer factorial(T value) {
			integer result = value;
			for (qpl::u64 i = 2u; i < value; ++i) {
				result.mul(i);
			}
			return result;
		}
		template<typename T, typename U>
		constexpr static integer pow(T a, U b) {
			integer base = a;
			integer result = 1;
			while (true) {
				if (b & 1)
					result *= base;
				b >>= 1;
				if (!b)
					break;
				base *= base;
			}

			return result;
		}

		template<typename T>
		constexpr void self_pow(T value) {
			integer base = *this;
			*this = 1;
			while (true) {
				if (value & 1)
					this->mul(base);
				value >>= 1;
				if (!value)
					break;
				base *= base;
			}
		}
		template<typename T>
		constexpr integer pow(T value) const {
			auto mul = *this;

			for (T i = 1; i < value; ++i) {
				mul.mul(*this);
			}
			return mul;
		}

		constexpr integer sqrt() const {
			if (this->empty()) {
				return *this;
			}
			auto dividend = *this;
			auto result = *this;
			integer last;
			integer loop;
			loop.clear();

			do {
				last = result;
				result = (result + dividend / result) / 2;

				if (result == loop) {
					break;
				}
				loop = last;

			} while (result - last);

			return result;
		}

		static integer random() {
			integer result;
			result.randomize();
			return result;
		}
		template<typename T1, typename T2>
		static integer random(T1 min, T2 max) {
			integer diff = max;
			diff -= min;

			integer result;

			auto stop = diff.last_used_index();
			bool consider_range = true;
			for (qpl::i32 i = stop; i >= 0; --i) {
				if (consider_range) {
					result.memory[i] = qpl::random(diff.memory[i]);
					consider_range = (result.memory[i] == diff.memory[i]);
				}
				else {
					result.memory[i] = qpl::random<qpl::u32>();
				}
			}

			result += min;

			return result;
		}
		template<typename T>
		static integer random(T max) {
			integer result;
			integer conv = max;

			auto stop = conv.last_used_index();
			bool consider_range = true;
			for (qpl::i32 i = stop; i >= 0; --i) {
				if (consider_range) {
					result.memory[i] = qpl::random(conv.memory[i]);
					consider_range = (result.memory[i] == conv.memory[i]);
				}
				else {
					result.memory[i] = qpl::random<qpl::u32>();
				}
			}
			return result;
		}
		static integer random_bits(qpl::u64 random_bits) {
			integer result;
			result.randomize_bits(random_bits);
			return result;
		}
		static integer random_bits_logarithmic() {
			integer result;
			result.randomize_bits_logarithmic();
			return result;
		}

		void randomize() {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::random<qpl::u32>();
			}
		}
		void randomize_bits(qpl::u64 random_bits) {
			qpl::u32 div = random_bits / base_max_log();
			qpl::u32 mod = random_bits % base_max_log();

			if (div >= this->memory_size()) {
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::random<qpl::u32>();
				}
			}
			else {
				for (qpl::u32 i = 0u; i < div; ++i) {
					this->memory[i] = qpl::random<qpl::u32>();
				}
				if constexpr (this->is_signed()) {
					if (qpl::random_b()) {
						if (mod) {
							this->memory[div] = ~(qpl::random<qpl::u32>() >> (this->base_max_log() - mod));
						}
						for (qpl::u32 i = div + qpl::bool_cast(mod); i < this->memory_size(); ++i) {
							this->memory[i] = ~qpl::u32{};
						}
						return;
					}
				}
				if (mod) {
					this->memory[div] = qpl::random<qpl::u32>() >> (this->base_max_log() - mod);
				}
				for (qpl::u32 i = div + qpl::bool_cast(mod); i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u32{};
				}
			}
		}
		void randomize_bits_logarithmic() {
			this->randomize_bits(qpl::random(bit_size()));
		}

		constexpr void increment(qpl::u32 start = 0u) {
			for (qpl::u32 i = start; i < this->memory_size(); ++i) {
				++this->memory[i];
				if (this->memory[i]) {
					return;
				}
			}
		}
		constexpr void decrement(qpl::u32 start = 0u) {
			for (qpl::u32 i = start; i < this->memory_size(); ++i) {
				if (this->memory[i]) {
					--this->memory[i];
					return;
				}
				this->memory[i] = qpl::u32_max;
			}
		}

		constexpr void clear_significant_bit() {
			auto pos = this->last_used_index();
			qpl::set_bit(this->memory[pos], qpl::significant_bit(this->memory[pos]) - 1, false);
		}
		constexpr qpl::size significant_bit(qpl::size index = memory_size() - 1) const {
			auto pos = this->last_used_index(index);
			return qpl::significant_bit(this->memory[pos]) + pos * base_max_log();
		}
		constexpr qpl::size significant_bit_ignore_last_bit() const {
			auto pos = this->last_used_index();
			if (pos == this->memory_size() - 1) {
				return qpl::significant_bit_ignore_last_bit(this->memory[pos]) + pos * base_max_log();
			}
			else {
				return qpl::significant_bit(this->memory[pos]) + pos * base_max_log();
			}
		}
		constexpr qpl::size significant_bit_inverse() const {
			auto pos = this->last_used_inverted_index();
			return qpl::significant_bit(~this->memory[pos]) + pos * base_max_log();
		}

		constexpr qpl::size digits(qpl::u32 base = 10u) const {
			qpl::size i = 1;
			integer check = base;
			auto compare = *this / base;
			if (!compare) {
				return i;
			}
			auto index = this->last_used_index();
			if (index) {
				if (qpl::base_full_bit_usage(base)) {
					check <<= index * base_max_log();
				}
				else {
					auto mul = qpl::base_max(base);
					for (qpl::u32 c = 0u; c < index; ++c) {
						check.mul(mul);
					}
				}
				i += index * qpl::base_max_log(base);
			}
			while (check <= compare) {
				++i;
				check.mul(base);
			}
			return i + 1;
		}

		constexpr void set_bit(qpl::size index, bool value) {
			qpl::set_bit(this->memory[index / base_max_log()], index % base_max_log(), value);
		}
		constexpr void set_first_bit(bool value) {
			this->memory[0u] = value ? (this->memory[0u] | qpl::u32{ 1u }) : (this->memory[0u] & ~(qpl::u32{ 1u }));
		}
		constexpr bool get_bit(qpl::size index) const {
			return this->memory[index / base_max_log()] & (qpl::u32{ 1 } << (index % base_max_log()));
		}
		constexpr bool get_first_bit() const {
			return this->memory[0u] & qpl::u32{ 1u };
		}

		constexpr operator qpl::u8() const {
			return qpl::u8_cast(this->memory[0]);
		}
		constexpr operator qpl::i8() const {
			return qpl::i8_cast(this->memory[0]);
		}

		constexpr operator qpl::u16() const {
			return qpl::u16_cast(this->memory[0]);
		}
		constexpr operator qpl::i16() const {
			return qpl::i16_cast(this->memory[0]);
		}

		constexpr operator qpl::u32() const {
			return this->memory[0];
		}
		constexpr operator qpl::i32() const {
			return qpl::i32_cast(this->memory[0]);
		}

		constexpr operator qpl::u64() const {
			if constexpr (memory_size() == 1) {
				return qpl::u64_cast(this->memory[0]);
			}
			return qpl::u64_cast(this->memory[0]) | (qpl::u64_cast(this->memory[1]) << base_max_log());
		}
		constexpr operator qpl::i64() const {
			if constexpr (memory_size() == 1) {
				return qpl::i64_cast(qpl::i32_cast(this->memory[0]));
			}
			return qpl::i64_cast(qpl::u64_cast(this->memory[0]) | (qpl::u64_cast(this->memory[1]) << base_max_log()));
		}

		constexpr operator qpl::f64() const {
			return qpl::f64_cast(this->operator qpl::u64());
		}
		constexpr operator qpl::f32() const {
			return qpl::f32_cast(this->operator qpl::u64());
		}


		constexpr operator bool() const {
			return *this != 0;
		}
		constexpr bool operator!() const {
			return *this == 0;
		}

		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer& operator+=(T value) {
			this->add(value);
			return *this;
		}
		template<qpl::size bits2, bool sign2>
		constexpr integer& operator+=(qpl::integer<bits2, sign2> other) {
			this->add(other);
			return *this;
		}

		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer operator+(T value) const {
			auto copy = *this;
			copy.add(value);
			return copy;
		}

		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr integer operator+(qpl::integer<bits2, sign2> other) const {
			auto copy = *this;
			copy.add(other);
			return copy;
		}

		constexpr integer operator-() const {
			auto copy = *this;
			copy.flip_sign();
			return copy;
		}
		constexpr integer operator+() const {
			if (this->is_positive()) {
				return *this;
			}
			auto copy = *this;
			copy.flip_sign();
			return copy;
		}

		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer& operator*=(T value) {
			this->mul(value);
			return *this;
		}
		template<qpl::size bits2, bool sign2>
		constexpr integer& operator*=(qpl::integer<bits2, sign2> other) {
			this->mul(other);
			return *this;
		}
		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer operator*(T value) const {
			auto copy = *this;
			copy.mul(value);
			return copy;
		}

		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> operator*(qpl::integer<bits2, sign2> other) const {
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> copy = *this;
			copy.mul(other);
			return copy;
		}

		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer& operator-=(T value) {
			this->sub(value);
			return *this;
		}
		template<qpl::size bits2, bool sign2>
		constexpr integer& operator-=(qpl::integer<bits2, sign2> other) {
			this->sub(other);
			return *this;
		}

		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer operator-(T value) const {
			auto copy = *this;
			copy.sub(value);
			return copy;
		}

		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> operator-(qpl::integer<bits2, sign2> other) const {
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> copy = *this;
			copy.sub(other);
			return copy;
		}


		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer& operator/=(T value) {
			this->div(value);
			return *this;
		}
		template<qpl::size bits2, bool sign2>
		constexpr integer& operator/=(qpl::integer<bits2, sign2> other) {
			this->div(other);
			return *this;
		}
		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer operator/(T value) const {
			auto copy = *this;
			copy.div(value);
			return copy;
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> operator/(qpl::integer<bits2, sign2> other) const {
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> copy = *this;
			copy.div(other);
			return copy;
		}

		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer& operator%=(T value) {
			this->mod(value);
			return *this;
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr integer& operator%=(qpl::integer<bits2, sign2> other) {
			this->mod(other);
			return *this;
		}
		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer operator%(T value) const {
			auto copy = *this;
			copy.mod(value);
			return copy;
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> operator%(qpl::integer<bits2, sign2> other) const {
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> copy = *this;
			return copy.moded(other);
		}

		template<typename T>
		constexpr integer& operator<<=(T value) {
			this->left_shift(qpl::i64_cast(value));
			return *this;
		}

		template<typename T>
		constexpr integer operator<<(T value) const {
			auto copy = *this;
			copy.left_shift(qpl::i64_cast(value));
			return copy;
		}

		template<typename T>
		constexpr integer& operator>>=(T value) {
			this->right_shift(qpl::i64_cast(value));
			return *this;
		}

		template<typename T>
		constexpr integer operator>>(T value) const {
			auto copy = *this;
			copy.right_shift(qpl::i64_cast(value));
			return copy;
		}


		template<typename T>
		constexpr bool operator==(T value) const {
			return this->equals(value);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool operator==(qpl::integer<bits2, sign2> value) const {
			return this->equals(value);
		}

		template<typename T>
		constexpr bool operator!=(T value) const {
			return !this->equals(value);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool operator!=(qpl::integer<bits2, sign2> value) const {
			return !this->equals(value);
		}

		template<typename T>
		constexpr bool operator<(T value) const {
			return this->less(value);
		}

		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr bool operator<(qpl::integer<bits2, sign2> value) const {
			return this->less(value);
		}
		template<typename T>
		constexpr bool operator<=(T value) const {
			return !this->greater(value);
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr bool operator<=(qpl::integer<bits2, sign2> value) const {
			return !this->greater(value);
		}

		template<typename T>
		constexpr bool operator>(T value) const {
			return this->greater(value);
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr bool operator>(qpl::integer<bits2, sign2> value) const {
			return this->greater(value);
		}

		template<typename T>
		constexpr bool operator>=(T value) const {
			return !this->less(value);
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr bool operator>=(qpl::integer<bits2, sign2> value) const {
			return !this->less(value);
		}


		constexpr integer operator~() const {
			return this->flipped_bits();
		}
		constexpr integer& operator++() {
			this->increment();
			return *this;
		}
		constexpr integer operator++(int dummy) const {
			auto copy = *this;
			return ++copy;
		}
		constexpr integer& operator--() {
			this->decrement();
			return *this;
		}
		constexpr integer operator--(int dummy) const {
			auto copy = *this;
			return --copy;
		}

		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer& operator^=(T value) {
			this->bitwise_xor(value);
			return *this;
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr integer& operator^=(qpl::integer<bits2, sign2> other) {
			this->bitwise_xor(other);
			return *this;
		}
		template<typename T> requires (!qpl::is_floating_point<T>())
		constexpr integer operator^(T value) const {
			auto copy = *this;
			copy.bitwise_xor(value);
			return copy;
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> operator^(qpl::integer<bits2, sign2> other) const {
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> copy = *this;
			copy.bitwise_xor(other);
			return copy;
		}

		template<typename T>
		constexpr integer& operator&=(T value) {
			this->bitwise_and(value);
			return *this;
		}
		template<qpl::size bits2, bool sign2>
		constexpr integer& operator&=(qpl::integer<bits2, sign2> other) {
			this->bitwise_and(other);
			return *this;
		}
		template<typename T>
		constexpr integer operator&(T value) const {
			auto copy = *this;
			copy.bitwise_and(value);
			return copy;
		}
		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> operator&(qpl::integer<bits2, sign2> other) const {
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> copy = *this;
			copy.bitwise_and(other);
			return copy;
		}

		template<typename T>
		constexpr integer& operator|=(T value) {
			this->bitwise_or(value);
			return *this;
		}
		template<qpl::size bits2, bool sign2>
		constexpr integer& operator|=(qpl::integer<bits2, sign2> other) {
			this->bitwise_or(other);
			return *this;
		}
		template<typename T>
		constexpr integer operator|(T value) const {
			auto copy = *this;
			copy.bitwise_or(value);
			return copy;
		}

		template<qpl::size bits2, bool sign2> requires(bits2 != bits && sign2 != sign)
		constexpr qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> operator|(qpl::integer<bits2, sign2> other) const {
			qpl::superior_integer<qpl::integer<bits, sign>, qpl::integer<bits2, sign2>> copy = *this;
			copy.bitwise_or(other);
			return copy;
		}

		template<bool check_sign = is_signed()>
		constexpr std::string memory_base_string(qpl::u32 base = 2, qpl::size seperation = 0u, const std::string& prefix = "") const {
			std::ostringstream stream;

			std::string prefix_stuff;

			if constexpr (check_sign) {
				if (this->is_negative()) {
					prefix_stuff += '-';
				}
			}
			prefix_stuff += prefix;

			bool empty = true;
			bool full = false;

			if constexpr (!check_sign) {
				for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
					if (empty && this->memory[i] || i == 0) {
						empty = false;
					}
					if (!empty) {
						stream << qpl::base_string(this->memory[i], base, "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, full);
						full = true;
					}
				}
			}
			else {
				if (this->is_negative()) {
					qpl::size empty_index = 0u;
					while (true) {
						if (this->memory[empty_index] || empty_index == this->memory_size() - 1) {
							break;
						}
						++empty_index;
					}

					for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
						auto memory = i <= empty_index ? ~this->memory[i] + 1 : ~this->memory[i];
						if (empty && memory || i == 0) {
							empty = false;
						}
						if (!empty) {
							stream << qpl::base_string(memory, base, "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, full);
							full = true;
						}
					}
				}
				else {
					for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
						if (empty && this->memory[i] || i == 0) {
							empty = false;
						}
						if (!empty) {
							stream << qpl::base_string(this->memory[i], base, "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, full);
							full = true;
						}
					}
				}
			}
			if (seperation == 0) {
				return prefix_stuff + stream.str();
			}
			else {
				return prefix_stuff + qpl::string_seperation(stream.str(), ' ', seperation, false);
			}
		}
		template<bool check_sign = is_signed()>
		constexpr std::string memory_base_string_full(qpl::u32 base = 2, qpl::size seperation = 0u, const std::string& prefix = "") const {
			std::ostringstream stream;

			std::string prefix_stuff;
			if constexpr (check_sign) {
				if (this->is_negative()) {
					prefix_stuff += '-';
				}
			}

			prefix_stuff += prefix;

			if constexpr (!check_sign) {
				for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
					stream << qpl::base_string(this->memory[i], base, "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, true);
				}
			}
			else {
				if (this->is_negative()) {
					qpl::size empty_index = 0u;
					while (true) {
						if (this->memory[empty_index] || empty_index == this->memory_size() - 1) {
							break;
						}
						++empty_index;
					}

					for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
						auto memory = i <= empty_index ? ~this->memory[i] + 1 : ~this->memory[i];
						stream << qpl::base_string(memory, base, "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, true);
					}
				}
				else {
					for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
						stream << qpl::base_string(this->memory[i], base, "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, true);
					}
				}
			}


			if (seperation == 0) {
				return prefix_stuff + stream.str();
			}
			else {
				return prefix_stuff + qpl::string_seperation(stream.str(), ' ', seperation, false);
			}
		}


		constexpr std::string hex_string(qpl::size seperation = 0u, const std::string& prefix = "0x") const {
			return this->memory_base_string(16u, seperation, prefix);
		}
		constexpr std::string octal_string(qpl::size seperation = 0u, const std::string& prefix = "0") const {
			return this->memory_base_string(8u, seperation, prefix);
		}
		constexpr std::string binary_string(qpl::size seperation = 0u) const {
			return this->memory_base_string(2u, seperation);
		}
		constexpr std::string base_64(qpl::size seperation = 0u) const {
			return this->base_string(64u, seperation);
		}
		constexpr std::string base_64_quotes(qpl::size seperation = 0u) const {
			return "\"" + this->base_string(64u, seperation) + "\"";
		}

		template<bool check_sign = is_signed()>
		constexpr std::string hex_string_full(qpl::size seperation = 0u, const std::string& prefix = "0x") const {
			return this->memory_base_string_full<check_sign>(16u, seperation, prefix);
		}
		constexpr std::string octal_string_full(qpl::size seperation = 0u, const std::string& prefix = "0") const {
			return this->memory_base_string_full(8u, seperation, prefix);
		}

		template<bool check_sign = is_signed()>
		constexpr std::string binary_string_full(qpl::size seperation = 0u) const {
			return this->memory_base_string_full<check_sign>(2u, seperation);
		}
		constexpr std::string unsigned_binary_string_full(qpl::size seperation = 0u) const {
			return this->memory_base_string_full<false>(2u, seperation);
		}

		constexpr std::string base_string(qpl::u32 base, qpl::size seperation = 0u, const std::string& prefix = "") const {
			if (qpl::base_full_bit_usage(base)) {
				return this->memory_base_string(base, seperation, prefix);
			}

			auto base_max = qpl::base_max(base);

			std::vector<qpl::u32> result;
			std::vector<qpl::u32> helper;

			helper.push_back(1u);


			auto negative = this->is_negative();
			if (negative) {
				result.push_back(qpl::u32{ 1 });
			}
			qpl::u32 stop;
			if constexpr (is_signed()) {
				if (negative) {
					stop = this->last_used_inverted_index() + 1;
				}
				else {
					stop = this->last_used_index() + 1;
				}
			}
			else {
				stop = this->last_used_index() + 1;
			}

			for (qpl::u32 j = 0u; j < stop; ++j) {
				std::vector<qpl::u32> multiplication = helper;

				//multiplying this->memory[j] into multiplication
				qpl::u32 multiply;
				if constexpr (is_signed()) {
					multiply = (negative ? ~this->memory[j] : this->memory[j]);
				}
				else {
					multiply = this->memory[j];
				}

				qpl::u64 m = 0u;
				for (qpl::u32 i = 0u;; ++i) {
					if (base_max == 0) {
						m >>= base_max_log();
					}
					else {
						m /= base_max;
					}

					bool exceeding = (i >= multiplication.size());

					if (!exceeding) {
						m += qpl::u64_cast(multiplication[i]) * qpl::u64_cast(multiply);
					}
					else if (!m) {
						break;
					}

					if (exceeding) {
						multiplication.push_back(qpl::u32{});
					}

					if (base_max == 0) {
						multiplication[i] = qpl::u32_cast(m);
					}
					else {
						multiplication[i] = qpl::u32_cast(m % base_max);
					}
				}

				//adding multiplication to result
				m = 0;
				for (qpl::u32 i = 0u;; ++i) {


					bool exceeding = (i >= result.size());
					bool exceeding_mul = (i >= multiplication.size());

					if (exceeding_mul && exceeding && !m) {
						break;
					}

					if (!exceeding_mul) {
						m += qpl::u64_cast(multiplication[i]);
					}

					if (!exceeding) {
						m += qpl::u64_cast(result[i]);

						if (base_max == 0) {
							result[i] = qpl::u32_cast(m);
						}
						else {
							result[i] = qpl::u32_cast(m % base_max);
						}
					}
					else {
						if (base_max == 0) {
							result.push_back(qpl::u32_cast(m));
						}
						else {
							result.push_back(qpl::u32_cast(m % base_max));
						}
					}

					if (base_max == 0) {
						m >>= base_max_log();
					}
					else {
						m /= base_max;
					}
				}

				//multiplying 1<<32 into helper
				m = 0u;
				for (qpl::u32 i = 0u; j != this->memory_size() - 1; ++i) {
					if (base_max == 0) {
						m >>= base_max_log();
					}
					else {
						m /= base_max;
					}

					bool exceeding = (i >= helper.size());

					if (!exceeding) {
						m += qpl::u64_cast(helper[i]) << base_max_log();
					}
					else if (!m) {
						break;
					}

					if (exceeding) {
						helper.push_back(qpl::u32{});
					}

					if (base_max == 0) {
						helper[i] = qpl::u32_cast(m);
					}
					else {
						helper[i] = qpl::u32_cast(m % base_max);
					}
				}
			}

			std::ostringstream stream;

			bool empty = true;
			bool full = false;
			for (qpl::i32 i = qpl::i32_cast(result.size() - 1); i >= 0; --i) {
				if (empty && result[i] || i == 0) {
					empty = false;
				}
				if (!empty) {
					stream << qpl::base_string(result[i], base, "", base <= 64 ? qpl::base_format::base36l : qpl::base_format::base64, full);
					full = true;
				}
			}
			std::ostringstream rstream;


			if (negative) {
				rstream << "-";
			}
			rstream << prefix;
			if (seperation == 0) {
				rstream << stream.str();
			}
			else {
				rstream << qpl::string_seperation(stream.str(), ' ', seperation, false);
			}
			return rstream.str();



		}


		constexpr std::string decimal_string(qpl::size seperation = 0u) const {
			return this->base_string(10u, seperation);
		}
		constexpr std::string string(qpl::size seperation = 0u) const {
			return this->decimal_string(seperation);
		}


		constexpr bit_proxy operator[](qpl::size index) {
			return bit_proxy(this->memory, index);
		}
		constexpr const bit_proxy operator[](qpl::size index) const {
			return bit_proxy(this->memory, index);
		}
		constexpr bit_proxy front() {
			return bit_proxy(this->memory, 0u);
		}
		constexpr const bit_proxy front() const {
			return bit_proxy(this->memory, 0u);
		}
		constexpr bit_proxy back() {
			return bit_proxy(this->memory, bit_size() - 1);
		}
		constexpr const bit_proxy back() const {
			return bit_proxy(this->memory, bit_size() - 1);
		}



		friend std::ostream& operator<<(std::ostream& os, const integer& integer) {
			return (os << integer.string());
		}

		holding_type memory;
	};

	constexpr qpl::i128 i128_min = qpl::i128{ std::array<qpl::u32, 4>{ 0u, 0u, 0u, 2147483648u } };
	constexpr qpl::i128 i128_max = qpl::i128{ std::array<qpl::u32, 4>{ 4294967295u, 4294967295u, 4294967295u, 2147483647u } };
	constexpr qpl::u128 u128_min = qpl::u128{ std::array<qpl::u32, 4>{ 0u, 0u, 0u, 0u } };
	constexpr qpl::u128 u128_max = qpl::u128{ std::array<qpl::u32, 4>{ 4294967295u, 4294967295u, 4294967295u, 4294967295u } };

	constexpr qpl::i256 i256_min = qpl::i256{ std::array<qpl::u32, 8>{ 0u, 0u, 0u, 0u, 0u, 0u, 0u, 2147483648u } };
	constexpr qpl::i256 i256_max = qpl::i256{ std::array<qpl::u32, 8>{ 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 2147483647u } };
	constexpr qpl::u256 u256_min = qpl::u256{ std::array<qpl::u32, 8>{ 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u } };
	constexpr qpl::u256 u256_max = qpl::u256{ std::array<qpl::u32, 8>{ 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u } };

	constexpr qpl::i512 i512_min = qpl::i512{ std::array<qpl::u32, 16>{ 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 2147483648u } };
	constexpr qpl::i512 i512_max = qpl::i512{ std::array<qpl::u32, 16>{ 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 2147483647u } };
	constexpr qpl::u512 u512_min = qpl::u512{ std::array<qpl::u32, 16>{ 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u } };
	constexpr qpl::u512 u512_max = qpl::u512{ std::array<qpl::u32, 16>{ 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u } };

	constexpr qpl::i1024 i1024_min = qpl::i1024{ std::array<qpl::u32, 32>{ 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 2147483648u } };
	constexpr qpl::i1024 i1024_max = qpl::i1024{ std::array<qpl::u32, 32>{ 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 2147483647u } };
	constexpr qpl::u1024 u1024_min = qpl::u1024{ std::array<qpl::u32, 32>{ 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u } };
	constexpr qpl::u1024 u1024_max = qpl::u1024{ std::array<qpl::u32, 32>{ 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u, 4294967295u } };



	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator==(T value, qpl::integer<bits, sign> other) {
		return other.operator ==(value);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator!=(T value, qpl::integer<bits, sign> other) {
		return other.operator != (value);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator<(T value, qpl::integer<bits, sign> other) {
		return other.operator >= (value);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator<=(T value, qpl::integer<bits, sign> other) {
		return other.operator > (value);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator>(T value, qpl::integer<bits, sign> other) {
		return other.operator  <= (value);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_dynamic_integer<T>() && !qpl::is_qpl_integer<T>())
	constexpr bool operator>=(T value, qpl::integer<bits, sign> other) {
		return other.operator  < (value);
	}



	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>> operator*(T value, qpl::integer<bits, sign> other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>>, qpl::integer<bits, sign>>()) {
			return other * value;
		}
		else {
			return value * qpl::type_cast<T>(other);
		}
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator*=(T& value, const qpl::integer<bits, sign>& other) {
		return value = (value * other);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>> operator+(T value, qpl::integer<bits, sign> other) {
		
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>>, qpl::integer<bits, sign>>()) {
			return other + value;
		}
		else {
			return value + qpl::type_cast<T>(other);
		}
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator+=(T& value, const qpl::integer<bits, sign>& other) {
		return value = (value + other);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>> operator-(T value, const qpl::integer<bits, sign>& other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>>, qpl::integer<bits, sign>>()) {
			return qpl::integer<bits, sign>(value) - other;
		}
		else {
			return value - static_cast<T>(other);
		}
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator-=(T& value, qpl::integer<bits, sign> other) {
		return value = (qpl::integer<bits, sign>(value) - other);
	}

	template<typename T, qpl::size bits, bool sign>  requires(!qpl::is_qpl_arithmetic<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>> operator/(T value, const qpl::integer<bits, sign>& other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>>, qpl::integer<bits, sign>>()) {
			return qpl::integer<bits, sign>(value) / other;
		}
		else {
			return value / static_cast<T>(other);
		}
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator/=(T& value, qpl::integer<bits, sign> other) {
		return value = (qpl::integer<bits, sign>(value) / other);
	}

	template<typename T, qpl::size bits, bool sign>  requires(!qpl::is_qpl_arithmetic<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>> operator%(T value, const qpl::integer<bits, sign>& other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::integer<bits, sign>>, qpl::integer<bits, sign>>()) {
			return qpl::integer<bits, sign>(value) % other;
		}
		else {
			return value % static_cast<T>(other);
		}
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator%=(T& value, qpl::integer<bits, sign> other) {
		return value = (qpl::integer<bits, sign>(value) % other);
	}


	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr qpl::integer<bits, sign> operator^(T value, const qpl::integer<bits, sign>& other) {
		return other ^ value;
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator^=(T& value, const qpl::integer<bits, sign>& other) {
		return value = (value ^ other);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr qpl::integer<bits, sign> operator&(T value, qpl::integer<bits, sign> other) {
		return other & value;
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator&=(T& value, qpl::integer<bits, sign> other) {
		return value = (value & other);
	}

	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr qpl::integer<bits, sign> operator|(T value, qpl::integer<bits, sign> other) {
		return other | value;
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator|=(T& value, qpl::integer<bits, sign> other) {
		return value = (value | other);
	}


	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>() && qpl::is_arithmetic<T>())
	constexpr qpl::integer<bits, sign> operator<<(T value, qpl::integer<bits, sign> other) {
		qpl::integer<bits, sign> result = value;
		result <<= qpl::size_cast(other);
		return result;
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator<<=(T& value, const qpl::integer<bits, sign>& other) {
		return value <<= qpl::size_cast(other);
	}


	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>() && qpl::is_arithmetic<T>())
	constexpr qpl::integer<bits, sign> operator>>(T value, qpl::integer<bits, sign> other) {
		qpl::integer<bits, sign> result = value;
		result >>= qpl::size_cast(other);
		return result;
	}
	template<qpl::size bits, bool sign, typename T> requires(!qpl::is_qpl_arithmetic<T>())
	constexpr T& operator>>=(T& value, qpl::integer<bits, sign> other) {
		return value >>= qpl::size_cast(other);
	}

#if defined(QPL_USE_INTRINSICS) || defined(QPL_USE_ALL)
	template<qpl::size bits, bool sign>
	struct x64_integer {
		using holding_type =
			qpl::conditional<
			qpl::if_true<(bits >= qpl::bits_in_type<qpl::u64>() && bits % qpl::bits_in_type<qpl::u64>() == 0u)>, std::array<qpl::u64, bits / qpl::bits_in_type<qpl::u64>()>,
			qpl::default_error>;

		struct bit_proxy {
			constexpr bit_proxy(holding_type& memory, qpl::size index) {
				this->memory = &memory;
				this->index = index;
			}

			constexpr bit_proxy& operator=(bool value) {
				qpl::u32 div, mod;
				div = _udiv64(qpl::u64_cast(this->index), qpl::u32_cast(qpl::bits_in_type<qpl::u64>()), &mod);
				qpl::set_bit((*this->memory)[div], mod, value);
				return *this;
			}

			constexpr bool get_value() const {
				qpl::u32 div, mod;
				div = _udiv64(qpl::u64_cast(this->index), qpl::u32_cast(qpl::bits_in_type<qpl::u64>()), &mod);
				return qpl::get_bit((*this->memory)[div], mod);
			}

			constexpr operator bool() const {
				return this->get_value();
			}

		private:
			holding_type* memory;
			qpl::size index;
		};


		constexpr static qpl::size bit_size() {
			return bits;
		}
		constexpr static qpl::size used_bit_size() {
			return bits - sign;
		}
		constexpr static qpl::size memory_size() {
			return bits / 64u;
		}

		constexpr static bool is_signed() {
			return sign;
		}
		constexpr static bool is_unsigned() {
			return !sign;
		}

		enum class constexpr_construction {
			zero, bit_flip, one, min_negative, max_negative
		};

		constexpr x64_integer() {

		}

		constexpr x64_integer(constexpr_construction constructions) : memory() {
			switch (constructions) {
			case constexpr_construction::zero:
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u64{};
				}
				break;
			case constexpr_construction::bit_flip:
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u64_max;
				}
				break;
			case constexpr_construction::one:
				this->memory[0u] = qpl::u64{ 1 };
				for (qpl::u32 i = 1u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u64{};
				}
				break;
			case constexpr_construction::min_negative:
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u64{};
				}
				this->memory.back() = (0x8000'0000'0000'0000ull);
				break;
			case constexpr_construction::max_negative:
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u64_max;
				}
				this->memory.back() = (0x7FFF'FFFF'FFFF'FFFFull);
				break;
			}
		}

		template<typename T>
		constexpr x64_integer(T value) : memory() {
			*this = value;
		}

		template<typename T>
		constexpr x64_integer& operator=(T value) {
			this->set(value);
			return *this;
		}

		constexpr void clear() {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u64{};
			}
		}
		constexpr void clear_flip() {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u64_max;
			}
		}

		constexpr qpl::u32 last_used_index(qpl::u32 start = memory_size() - 1) const {
			for (qpl::u32 i = start; i > 0u; --i) {
				if (this->memory[i]) {
					return i;
				}
			}
			return 0u;
		}
		constexpr qpl::size last_used_index_u32() const {
			for (qpl::u32 i = this->memory_size() - 1; i > 0u; --i) {
				if (this->memory[i] >> qpl::bits_in_type<qpl::u32>()) {
					return qpl::size_cast((i << 1) | 0x1u);
				}
				else if (this->memory[i]) {
					return qpl::size_cast((i << 1));
				}
			}
			return qpl::size_cast(qpl::bool_cast(this->memory[0u] >> qpl::bits_in_type<qpl::u32>()));
		}
		constexpr qpl::u32 u32_at(qpl::size index) const {
			auto left = (index % 2u) * qpl::u32_cast(this->memory[index >> 1] >> qpl::bits_in_type<qpl::u32>());
			auto right = ((index + 1u) % 2u) * qpl::u32_cast(this->memory[index >> 1]);
			return left | right;
		}

		constexpr qpl::u32 last_used_inverted_index() const {
			for (qpl::i32 i = this->memory_size() - 1; i > 0; --i) {
				if (this->memory[i] != qpl::u64_max) {
					return qpl::size_cast(i);
				}
			}
			return 0u;
		}
		constexpr static qpl::size base_max_log() {
			return qpl::bits_in_type<qpl::u64>();
		}
		constexpr qpl::size significant_bit(qpl::u32 start = memory_size() - 1) const {
			auto index = this->last_used_index(start);
			return index * this->base_max_log() + qpl::significant_bit(this->memory[index]);
		}


		constexpr bool is_zero() const {
			return *this == 0;
		}
		constexpr bool is_negative() const {
			if constexpr (is_unsigned()) {
				return false;
			}
			return this->memory.back() & (qpl::u64{ 1 } << (base_max_log() - 1));
		}
		constexpr bool is_positive() const {
			return !this->is_negative();
		}
		constexpr void set_sign_bit_true() {
			qpl::set_bit(this->memory.back(), base_max_log() - 1, true);
		}
		constexpr void set_sign_bit_false() {
			qpl::set_bit(this->memory.back(), base_max_log() - 1, false);
		}
		constexpr void flip_sign_bit() {
			if (this->is_positive()) {
				this->set_sign_bit_true();
			}
			else {
				this->set_sign_bit_false();
			}
		}

		constexpr void set_negative() {
			if (this->is_positive()) {
				this->flip_bits();
				this->increment();
			}
		}
		constexpr void set_positive() {
			if (this->is_negative()) {
				this->flip_bits();
				this->increment();
			}
		}
		constexpr void flip_sign() {
			this->flip_bits();
			this->increment();
		}
		constexpr x64_integer flipped_sign() const {
			auto copy = *this;
			copy.flip_sign();
			return copy;
		}

		constexpr void flip_bits() {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				qpl::flip_bits(this->memory[i]);
			}
		}
		constexpr x64_integer flipped_bits() const {
			auto copy = *this;
			copy.flip_bits();
			return copy;
		}

		constexpr void flip_bits_until(qpl::size position) {
			if (position >= bit_size()) {
				this->flip_bits();
				return;
			}

			qpl::u32 div, mod;
			div = _udiv64(qpl::u64_cast(position), qpl::u32_cast(this->base_max_log()), &mod);

			for (qpl::u32 i = 0u; i < div; ++i) {
				qpl::flip_bits(this->memory[i]);
			}
			qpl::flip_bits_until(this->memory[div], mod);
		}
		constexpr x64_integer flipped_bits_until(qpl::size position) const {
			auto copy = *this;
			copy.flip_bits_until(position);
			return copy;
		}

		constexpr static x64_integer min() {
			if constexpr (is_unsigned()) {
				x64_integer result{ constexpr_construction::zero };
				return result;
			}
			else {
				x64_integer result{ constexpr_construction::min_negative };
				return result;
			}
		}
		constexpr static x64_integer max() {
			if constexpr (is_unsigned()) {
				x64_integer result{ constexpr_construction::bit_flip };
				return result;
			}
			else {
				x64_integer result{ constexpr_construction::max_negative };
				return result;
			}
		}
		constexpr static x64_integer zero() {
			x64_integer result{ constexpr_construction::zero };
			return result;
		}
		constexpr static x64_integer one() {
			x64_integer result{ constexpr_construction::one };
			return result;
		}

		constexpr operator qpl::i8() const {
			return qpl::i8_cast(this->memory[0]);
		}
		constexpr operator qpl::u8() const {
			return qpl::u8_cast(this->memory[0]);
		}

		constexpr operator qpl::i16() const {
			return qpl::i16_cast(this->memory[0]);
		}
		constexpr operator qpl::u16() const {
			return qpl::u16_cast(this->memory[0]);
		}

		constexpr operator qpl::i32() const {
			return qpl::i32_cast(this->memory[0]);
		}
		constexpr operator qpl::u32() const {
			return qpl::u32_cast(this->memory[0]);
		}

		constexpr operator qpl::i64() const {
			return qpl::i64_cast(this->memory[0]);
		}
		constexpr operator qpl::u64() const {
			return qpl::u64_cast(this->memory[0]);
		}

		template<typename T>
		constexpr void set(T value) {
			this->memory[0u] = qpl::u64_cast(value);

			if constexpr (qpl::is_signed<T>()) {
				if (value < T{}) {
					for (qpl::u32 i = 1; i < this->memory_size(); ++i) {
						this->memory[i] = qpl::u64_max;
					}
					return;
				}
			}
			for (qpl::u32 i = 1; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u64{};
			}
		}
		template<qpl::size bits2, bool sign2>
		constexpr void set(x64_integer<bits2, sign2> other) {
			if constexpr (this->bit_size() == other.bit_size()) {
				if (std::is_constant_evaluated) {
					for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
						this->memory[i] = other.memory[i];
					}
				}
				else {
					qpl::copy_memory(other.memory, this->memory);
				}
			}
			else {
				for (qpl::u32 i = 0u; i < qpl::min(this->memory_size(), other.memory_size()); ++i) {
					this->memory[i] = other.memory[i];
				}
				for (qpl::u32 i = other.memory_size(); i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u64{};
				}
			}

		}
		template<qpl::size bits2, bool sign2>
		constexpr void set(integer<bits2, sign2> value) {
			auto index = value.last_used_index() + 1;
			auto stop = (index >> 1);


			if (stop >= this->memory_size()) {
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u64_cast(value.memory[(i << 1)]) | (qpl::u64_cast(value.memory[(i << 1) | 0x1u]) << qpl::bits_in_type<qpl::u32>());
				}
				return;
			}
			for (qpl::u32 i = 0u; i < stop; ++i) {
				this->memory[i] = qpl::u64_cast(value.memory[(i << 1)]) | (qpl::u64_cast(value.memory[(i << 1) | 0x1u]) << qpl::bits_in_type<qpl::u32>());
			}
			if (index % 2u) {
				this->memory[stop] = qpl::u64_cast(value.memory[(stop << 1)]);
			}
			for (qpl::u32 i = stop + (index % 2u); i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u64{};
			}
		}
		constexpr void set(std::string_view string, qpl::u32 base = qpl::u32_max) {
			qpl::integer<bits, sign> copy;
			copy.set(string, base);
			this->set(copy);

		}
		constexpr void set(const char* string, qpl::u32 base = qpl::u32_max) {
			qpl::integer<bits, sign> copy;
			copy.set(string, base);
			this->set(copy);

		}

		constexpr void increment(qpl::u32 start = 0u) {
			for (qpl::u32 i = start; i < this->memory_size(); ++i) {
				++this->memory[i];
				if (this->memory[i]) {
					return;
				}
			}
		}
		constexpr void decrement(qpl::u32 start = 0u) {
			for (qpl::u32 i = start; i < this->memory_size(); ++i) {
				if (this->memory[i]) {
					--this->memory[i];
					return;
				}
				this->memory[i] = qpl::u64_max;
			}
		}

		template<typename T>
		constexpr bool equals(T value) const {
			if (this->memory[0u] != qpl::u64_cast(value)) {
				return false;
			}
			if constexpr (this->is_signed()) {
				if (this->is_negative()) {
					auto pos = this->last_used_inverted_index();
					return pos == 0u;
				}

			}
			auto pos = this->last_used_index();
			return pos == 0u;
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool equals(qpl::x64_integer<bits2, sign2> other) const {
			if (this->memory[0u] != other.memory[0u]) {
				return false;
			}

			auto my_non = this->last_used_index();
			auto other_non = other.last_used_index();

			if (my_non != other_non) {
				return false;
			}

			for (qpl::u32 i = 1u; i < my_non + 1; ++i) {
				if (this->memory[i] != other.memory[i]) {
					return false;
				}
			}
			return true;
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool equals(qpl::integer<bits2, sign2> other) const {
			if constexpr (other.memory_size() == 1) {
				if (this->memory[0u] != qpl::u64_cast(other.memory[0u])) {
					return false;
				}
			}
			else {
				if (this->memory[0u] != (qpl::u64_cast(other.memory[0u]) | (qpl::u64_cast(other.memory[1u]) << qpl::bits_in_type<qpl::u32>()))) {
					return false;
				}
			}


			auto my_pos = this->last_used_index();
			auto other_pos = other.last_used_index() >> 1;

			if (my_pos != other_pos) {
				return false;
			}


			for (qpl::u32 i = 1u; i < my_pos + 1; ++i) {
				if ((i << 1) == other.memory_size() - 1) {
					if (this->memory[i] != qpl::u64_cast(other.memory[i << 1])) {
						return false;
					}
				}
				else {
					if (this->memory[i] != (qpl::u64_cast(other.memory[(i << 1)]) | (qpl::u64_cast(other.memory[(i << 1) | 0x1u]) << qpl::bits_in_type<qpl::u32>()))) {
						return false;
					}
				}
			}
			return true;
		}

		template<typename T>
		constexpr bool less(T value) const {
			bool negative = false;
			if constexpr (is_signed()) {
				negative = this->is_negative();
				if constexpr (qpl::is_signed<T>()) {
					if (value < T{}) {
						if (!negative) {
							return false;
						}
					}
					else if (negative) {
						return true;
					}
				}
				else {
					if (negative) {
						return true;
					}
				}
			}

			if (this->memory[0u] >= qpl::u64_cast(value)) {
				return false;
			}
			if constexpr (this->is_signed()) {
				if (negative) {
					auto pos = this->last_used_inverted_index();
					return pos == 0u;
				}

			}
			auto pos = this->last_used_index();
			return pos == 0u;
		}
		template<qpl::size bits2, bool sign2, bool check_sign = true>
		constexpr bool less(qpl::x64_integer<bits2, sign2> other) const {
			if constexpr (check_sign && is_signed()) {
				auto neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (neg && !other_neg) {
					return true;
				}
				if (!neg && other_neg) {
					return false;
				}
			}

			auto my_non = this->last_used_index();
			auto other_non = other.last_used_index();


			if (my_non != other_non) {
				return my_non < other_non;
			}

			for (qpl::i32 i = my_non; i > 0; --i) {
				if (this->memory[i] < other.memory[i]) {
					return true;
				}
				else if (this->memory[i] > other.memory[i]) {
					return false;
				}
			}
			return this->memory[0u] < other.memory[0u];
		}

		template<typename T>
		constexpr bool greater(T value) const {
			bool negative = false;
			if constexpr (is_signed()) {
				negative = this->is_negative();
				if constexpr (qpl::is_signed<T>()) {
					if (value < T{}) {
						if (!negative) {
							return true;
						}
					}
					else if (negative) {
						return false;
					}
				}
				else {
					if (negative) {
						return false;
					}
				}
			}

			if (this->memory[0u] <= qpl::u64_cast(value)) {
				return false;
			}
			if constexpr (this->is_signed()) {
				if (negative) {
					auto pos = this->last_used_inverted_index();
					return pos == 0u;
				}

			}
			auto pos = this->last_used_index();
			return pos == 0u;
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool greater(qpl::x64_integer<bits2, sign2> other) const {
			if constexpr (is_signed()) {
				auto neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (neg && !other_neg) {
					return false;
				}
				if (!neg && other_neg) {
					return true;
				}
			}

			auto my_non = this->last_used_index();
			auto other_non = other.last_used_index();

			if (my_non != other_non) {
				return my_non > other_non;
			}

			for (qpl::i32 i = my_non; i > 0; --i) {
				if (this->memory[i] < other.memory[i]) {
					return false;
				}
				else if (this->memory[i] > other.memory[i]) {
					return true;
				}
			}
			return this->memory[0u] > other.memory[0u];
		}

		template<typename T>
		void add(T value) {
			auto c = _addcarry_u64(0, this->memory[0u], qpl::u64_cast(value), &this->memory[0u]);
			if (c) {
				this->increment(1u);
			}
		}
		void add(x64_integer other) {
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (my_neg && other_neg) {
					this->flip_sign();
					other.flip_sign();
					this->add(other);
					this->flip_sign();
					return;
				}
				else if (other_neg) {
					other.flip_sign();
					this->sub(other);
					return;
				}
			}

			char c = 0;
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				c = _addcarry_u64(c, this->memory[i], other.memory[i], &this->memory[i]);
			}
		}
		x64_integer added(x64_integer other) const {
			x64_integer result;

			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (my_neg && other_neg) {
					result = *this;
					result.flip_sign();
					other.flip_sign();
					result.add(other);
					result.flip_sign();
					return result;
				}
				else if (other_neg) {
					result = *this;
					other.flip_sign();
					result.sub(other);
					return result;
				}
			}

			char c = 0;
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				c = _addcarry_u64(c, this->memory[i], other.memory[i], &result.memory[i]);
			}
			return result;
		}
		void add_shift(x64_integer other, qpl::size index) {
			char c = 0;
			for (qpl::u32 i = 0u; i < this->memory_size() - index; ++i) {
				c = _addcarry_u64(c, this->memory[i + index], other.memory[i], &this->memory[i + index]);
			}
		}

		template<typename T>
		void sub(T value) {
			auto c = _subborrow_u64(0, this->memory[0u], qpl::u64_cast(value), &this->memory[0u]);
			if (c) {
				this->decrement(1u);
			}
		}
		void sub(x64_integer other) {
			if constexpr (other.is_signed()) {
				if (other.is_negative()) {
					other.flip_sign();
					this->add(other);
					return;
				}
			}
			if (*this < other) {
				other.sub(*this);
				*this = other;
				this->flip_sign();
				return;
			}

			if constexpr (this->memory_size() >= 96u) {
				qpl::u64 over = 0ull;
				qpl::size other_stop = other.last_used_index() + 1;

				qpl::u32 i = 0u;
				for (; i < qpl::min(other.memory_size(), this->memory_size()); ++i) {
					bool less = this->memory[i] < (other.memory[i] + over);

					this->memory[i] = (this->memory[i] - other.memory[i] - over);
					over = less ? 1ull : 0ull;

					if (i >= other_stop && !less) {
						break;
					}
				}
			}
			else {

				char c = 0;
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					c = _subborrow_u64(c, this->memory[i], other.memory[i], &this->memory[i]);
				}
			}
		}

		template<typename T>
		void mul(T value) {
			qpl::u64 add_low = 0;
			qpl::u64 mul_low, mul_high;
			auto stop = this->last_used_index();
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				mul_low = _umul128(this->memory[i], qpl::u64_cast(value), &mul_high);
				mul_high += _addcarry_u64(0, add_low, mul_low, &add_low);
				this->memory[i] = add_low;
				add_low = mul_high;
			}
		}
		void mul(x64_integer other) {
			*this = this->mulled(other);
		}

		template<typename T>
		x64_integer mulled(T value) const {
			x64_integer result;
			qpl::u64 add_low = 0;
			qpl::u64 mul_low, mul_high;
			auto stop = this->last_used_index();
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				mul_low = _umul128(this->memory[i], qpl::u64_cast(value), &mul_high);
				mul_high += _addcarry_u64(0, add_low, mul_low, &add_low);
				result.memory[i] = add_low;
				add_low = mul_high;
			}
			return result;
		}
		x64_integer mulled(x64_integer other) const {
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (other_neg && my_neg) {
					x64_integer result = *this;
					result.flip_sign();
					other.flip_sign();
					result.mul(other);
					return result;
				}
				else if (other_neg) {
					x64_integer result = *this;
					other.flip_sign();
					result.mul(other);
					result.flip_sign();
					return result;
				}
				else if (my_neg) {
					x64_integer result = *this;
					result.flip_sign();
					result.mul(other);
					result.flip_sign();
					return result;
				}
			}
			qpl::x64_integer<bits, sign> result;
			result.clear();

			auto other_index = other.last_used_index();
			auto my_index = this->last_used_index();

			qpl::x64_integer<bits, sign> add;


			if (other_index < my_index) {
				qpl::x64_integer<bits, sign> mul = *this;
				for (qpl::u32 i = 0u; i <= other_index; ++i) {
					add = mul;
					add.mul(other.memory[i]);
					result.add_shift(add, i);
				}
			}
			else {
				qpl::x64_integer<bits, sign> mul = other;
				for (qpl::u32 i = 0u; i <= my_index; ++i) {
					add = mul;
					add.mul(this->memory[i]);
					result.add_shift(add, i);
				}
			}

			return result;
		}

		constexpr void bitwise_xor(x64_integer other) {
			auto index = other.last_used_index() + 1;
			for (qpl::u32 i = 0u; i < qpl::min(index, this->memory_size()); ++i) {
				this->memory[i] ^= other.memory[i];
			}
		}
		constexpr void bitwise_or(x64_integer other) {
			auto index = other.last_used_index() + 1;
			for (qpl::u32 i = 0u; i < qpl::min(index, this->memory_size()); ++i) {
				this->memory[i] |= other.memory[i];
			}
		}
		constexpr void bitwise_and(x64_integer other) {
			auto index = other.last_used_index() + 1;
			qpl::u32 i = 0u;
			for (; i < qpl::min(index, this->memory_size()); ++i) {
				this->memory[i] &= other.memory[i];
			}
			for (; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::u64{};
			}
		}

		constexpr bool get_bit(qpl::size index) const {
			qpl::u32 div, mod;
			div = _udiv64(qpl::u64_cast(index), qpl::u32_cast(this->base_max_log()), &mod);

			return qpl::get_bit(this->memory[div], mod);
		}
		constexpr void set_bit(qpl::size index, bool value) {
			qpl::u32 div, mod;
			div = _udiv64(qpl::u64_cast(index), qpl::u32_cast(this->base_max_log()), &mod);

			return qpl::set_bit(this->memory[div], mod, value);
		}

		constexpr x64_integer get_bits(qpl::u32 begin, qpl::u32 end) const {
			if (!end) {
				return this->zero();
			}
			x64_integer result;


			auto index_begin = begin / base_max_log();
			qpl::size index_end = (end - begin) / base_max_log();
			auto s = (begin % base_max_log());
			auto mask = (end - begin) % base_max_log();


			qpl::u32 i = 0u;

			if (mask) {
				if (s) {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = (this->memory[i + index_begin] >> s) | (this->memory[i + index_begin + 1] << (this->base_max_log() - s));
					}
					result.memory[i] = ((this->memory[i + index_begin] >> s) | (this->memory[i + index_begin + 1] << (this->base_max_log() - s))) & (qpl::u64_max >> (this->base_max_log() - mask));
				}
				else {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = this->memory[i + index_begin];
					}
					result.memory[i] = this->memory[i + index_begin] & (qpl::u64_max >> (this->base_max_log() - mask));
				}
				++i;
			}
			else {
				if (s) {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = (this->memory[i + index_begin] >> s) | (this->memory[i + index_begin + 1] << (this->base_max_log() - s));
					}
				}
				else {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = this->memory[i + index_begin];
					}
				}
			}

			for (; i < this->memory_size(); ++i) {
				result.memory[i] = qpl::u64{};
			}

			return result;
		}


		constexpr void set_bit(qpl::size index, qpl::u32 bit, bool value) {
			return qpl::set_bit(this->memory[index], bit, value);
		}
		constexpr void set_first_bit(bool value) {
			this->memory[0u] = value ? (this->memory[0u] | qpl::u64{ 1u }) : (this->memory[0u] & ~(qpl::u64{ 1u }));
		}

		template<bool clear = true>
		constexpr void left_shift(qpl::i64 shift) {
			if (shift == 0) {
				return;
			}
			if (shift < 0) {
				this->right_shift(-shift);
				return;
			}
			qpl::u32 div, mod;
			div = _udiv64(qpl::u64_cast(shift), qpl::u32_cast(this->base_max_log()), &mod);

			if (div >= this->memory_size()) {
				this->clear();
				return;
			}

			if (mod) {
				auto start = qpl::min(this->last_used_index() + div + 1, qpl::u32_cast(this->memory_size()) - 1);

				for (qpl::i32 i = qpl::i32_cast(start); i > qpl::i32_cast(div); --i) {
					this->memory[i] = qpl::u64_cast(((this->memory[i - div] << mod) | (this->memory[i - div - 1] >> (base_max_log() - mod))));
				}
				this->memory[div] = qpl::u64_cast(this->memory.front() << mod);
			}
			else {
				if (div) {
					auto start = qpl::min(this->last_used_index() + div + 1, qpl::u32_cast(this->memory_size()) - 1);

					for (qpl::i32 i = qpl::i32_cast(start); i >= div; --i) {
						this->memory[i] = this->memory[i - div];
					}
				}
			}
			if constexpr (clear) {
				for (qpl::i32 i = 0u; i < qpl::i32_cast(div); ++i) {
					this->memory[i] = qpl::u64{};
				}
			}

		}

		template<bool clear = true>
		constexpr void right_shift(qpl::i64 shift) {
			if (shift == 0) {
				return;
			}
			if (shift < 0) {
				this->left_shift(-shift);
				return;
			}

			qpl::u32 div, mod;
			div = _udiv64(qpl::u64_cast(shift), qpl::u32_cast(this->base_max_log()), &mod);

			if (div >= this->memory_size()) {
				this->clear();
				return;
			}

			if constexpr (is_signed()) {
				bool has_sign = this->is_negative();

				if (mod) {
					auto stop = qpl::min<qpl::size>(this->last_used_index(), this->memory_size() - 1);
					if (div > stop) {
						if constexpr (clear) {
							this->clear();
						}
						return;
					}

					for (qpl::i32 i = 0u; i < qpl::i32_cast(stop - div); ++i) {
						this->memory[i] = qpl::u64_cast(((this->memory[i + div] >> mod) | (this->memory[i + div + 1] << (base_max_log() - mod))));
					}
					this->memory[stop - div] = qpl::u64_cast(this->memory[stop] >> mod);


					if constexpr (clear) {
						if (has_sign) {
							for (qpl::i32 i = qpl::i32_cast(stop - div) + 1; i < this->memory_size(); ++i) {
								this->memory[i] = qpl::u64_max;
							}
						}
						else {
							for (qpl::i32 i = qpl::i32_cast(stop - div) + 1; i < this->memory_size(); ++i) {
								this->memory[i] = qpl::u64{};
							}
						}
					}
				}
				else {
					if (div) {
						for (qpl::u32 i = 0u; i < qpl::u32_cast(this->memory_size()) - div; ++i) {
							this->memory[i] = this->memory[i + div];
						}
						if constexpr (clear) {
							if (has_sign) {
								for (qpl::u32 i = this->memory_size() - div; i < this->memory_size(); ++i) {
									this->memory[i] = qpl::u64_max;
								}
							}
							else {
								for (qpl::u32 i = this->memory_size() - div; i < this->memory_size(); ++i) {
									this->memory[i] = qpl::u64{};
								}
							}
						}
					}
				}

				if (has_sign) {
					this->memory[this->memory_size() - div - 1] |= (qpl::u64_max << (base_max_log() - mod - 1));
				}
			}
			else {
				if (mod) {
					auto stop = qpl::min<qpl::size>(this->last_used_index(), this->memory_size() - 1);
					if (div > stop) {
						if constexpr (clear) {
							this->clear();
						}
						return;
					}

					for (qpl::i32 i = 0u; i < qpl::i32_cast(stop - div); ++i) {
						this->memory[i] = qpl::u64_cast(((this->memory[i + div] >> mod) | (this->memory[i + div + 1] << (base_max_log() - mod))));
					}
					this->memory[stop - div] = qpl::u64_cast(this->memory[stop] >> mod);


					if constexpr (clear) {
						for (qpl::i32 i = qpl::i32_cast(stop) - qpl::i32_cast(div) + 1; i < this->memory_size(); ++i) {
							this->memory[i] = qpl::u64{};
						}
					}
				}
				else {
					if (div) {
						for (qpl::i32 i = 0u; i < qpl::i32_cast(this->memory_size()) - qpl::i32_cast(div); ++i) {
							this->memory[i] = this->memory[i + div];
						}
						if constexpr (clear) {
							for (qpl::i32 i = qpl::i32_cast(this->memory_size()) - qpl::i32_cast(div); i < this->memory_size(); ++i) {
								this->memory[i] = qpl::u64{};
							}
						}
					}
				}
			}
		}

		constexpr x64_integer quick_dm_shifted_bits(qpl::u32 begin, qpl::size size) const {
			x64_integer result;

			auto index_begin = begin / base_max_log();
			qpl::size index_end = size / base_max_log();
			auto mask = size % base_max_log();
			auto bit = begin % base_max_log();


			qpl::u32 i = 0u;

			if (mask) {
				if (bit) {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = (this->memory[i + index_begin] >> bit) | (this->memory[i + index_begin + 1] << (this->base_max_log() - bit));
					}
					if (i + index_begin + 1 < this->memory_size()) {
						result.memory[i] = ((this->memory[i + index_begin] >> bit) | (this->memory[i + index_begin + 1] << (this->base_max_log() - bit))) & (qpl::u64_max >> (this->base_max_log() - mask));
					}
					else {
						result.memory[i] = (this->memory[i + index_begin] >> bit) & (qpl::u64_max >> (this->base_max_log() - mask));
					}
				}
				else {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = this->memory[i + index_begin];
					}
					result.memory[i] = this->memory[i + index_begin] & (qpl::u64_max >> (this->base_max_log() - mask));
				}
				++i;
			}
			else {
				if (bit) {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = (this->memory[i + index_begin] >> bit) | (this->memory[i + index_begin + 1] << (this->base_max_log() - bit));
					}
				}
				else {
					for (; i < qpl::min(this->memory_size() - index_begin, index_end); ++i) {
						result.memory[i] = this->memory[i + index_begin];
					}
				}
			}

			return result;
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool quick_dm_less(qpl::x64_integer<bits2, sign2> other, qpl::size index) const {

			auto my_non = this->last_used_index(index);
			auto other_non = other.last_used_index(index);


			if (my_non != other_non) {
				return my_non < other_non;
			}

			for (qpl::i32 i = my_non; i > 0; --i) {
				if (this->memory[i] < other.memory[i]) {
					return true;
				}
				else if (this->memory[i] > other.memory[i]) {
					return false;
				}
			}
			return this->memory[0u] < other.memory[0u];
		}
		void quick_dm_sub(x64_integer other, qpl::size index) {

			if constexpr (this->memory_size() >= 96u) {
				qpl::u64 over = 0ull;
				qpl::size other_stop = other.last_used_index(index) + 1;

				qpl::u32 i = 0u;
				for (; i < qpl::min(other.memory_size(), this->memory_size()); ++i) {
					bool less = this->memory[i] < (other.memory[i] + over);

					this->memory[i] = (this->memory[i] - other.memory[i] - over);
					over = less ? 1ull : 0ull;

					if (i >= other_stop && !less) {
						break;
					}
				}
			}
			else {
				auto stop = other.last_used_index(index);

				char c = 0;
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					c = _subborrow_u64(c, this->memory[i], other.memory[i], &this->memory[i]);

					if (i >= stop && !c) {
						break;
					}
				}
			}
		}

		void div(x64_integer other) {
			this->set(this->dived(other));
		}
		x64_integer dived2(x64_integer other) const {
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (other_neg && my_neg) {
					x64_integer result = *this;
					result.flip_sign();
					other.flip_sign();
					return result.dived(other);
				}
				else if (other_neg) {
					x64_integer result = *this;
					other.flip_sign();
					result.div(other);
					result.flip_bits();
					return result;
				}
				else if (my_neg) {
					x64_integer result = *this;
					result.flip_sign();
					result.div(other);
					result.flip_bits();
					return result;
				}
			}


			x64_integer div;
			div.clear();

			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (other.is_zero()) {
				return div;
			}
			if (my_position < reduce) {
				return div;
			}

			auto shift = my_position - reduce + 1;


			x64_integer mod = *this;
			mod >>= shift;


			x64_integer shift_bits;

			qpl::u32 msb = reduce - 1;
			qpl::u32 look_index = qpl::min(this->memory_size() - 1, (reduce + 1) / this->base_max_log());

			for (qpl::i32 i = shift - 1; i >= 0; --i) {

				qpl::i32 skip = qpl::min<qpl::i32>(i, (qpl::signed_cast(reduce) - qpl::signed_cast(msb)));

				auto skippable = (skip > 0);
				if (skippable) {
					mod.left_shift(skip);
					shift_bits = this->quick_dm_shifted_bits(i - (skip - 1), skip);

					auto skip_memory = (skip - 1) / this->base_max_log();
					for (qpl::u32 b = 0u; b < skip_memory; ++b) {
						mod.memory[b] = shift_bits.memory[b];
					}
					mod.memory[skip_memory] |= shift_bits.memory[skip_memory];

					i -= (skip - 1);
					msb += skip;

				}
				else {
					mod <<= 1;
					mod.set_first_bit(this->get_bit(i));
				}


				auto nle = !(mod.quick_dm_less(other, look_index));

				if (nle) {
					mod.quick_dm_sub(other, look_index);
					msb = mod.significant_bit(look_index) - 1;

					div.set_bit(i, qpl::u32{ 1 });
				}
			}

			return div;
		}
		x64_integer dived(x64_integer other) const {
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				if (other_neg && my_neg) {
					x64_integer result = *this;
					result.flip_sign();
					other.flip_sign();
					return result.dived(other);
				}
				else if (other_neg) {
					x64_integer result = *this;
					other.flip_sign();
					result.div(other);
					result.flip_bits();
					return result;
				}
				else if (my_neg) {
					x64_integer result = *this;
					result.flip_sign();
					result.div(other);
					result.flip_bits();
					return result;
				}
			}


			x64_integer div;
			div.clear();

			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (other.is_zero()) {
				return div;
			}
			if (my_position < reduce) {
				return div;
			}

			auto shift = my_position - reduce + 1;

			x64_integer mod = *this;
			mod >>= shift;

			for (qpl::i32 i = shift - 1; i >= 0; --i) {

				mod <<= 1;

				mod.set_first_bit(this->get_bit(i));

				auto nle = mod >= other;

				if (nle) {
					mod -= other;

					div.set_bit(i, qpl::u32{ 1 });
				}
			}

			return div;
		}

		void mod(x64_integer other) {
			this->set(this->moded(other));
		}
		x64_integer moded2(x64_integer other) const {
			if constexpr (is_signed()) {
				auto other_negative = other.is_negative();
				auto my_negative = this->is_negative();

				if (my_negative && other_negative) {
					x64_integer result = *this;
					result.flip_sign();
					other.flip_sign();
					result.mod(other);
					result.flip_sign();
					return result;
				}
				if (my_negative) {
					x64_integer result = *this;
					result.flip_sign();
					result.mod(other);
					return other - result;
				}
				if (other_negative) {
					x64_integer result = *this;
					other.flip_sign();
					result.mod(other);
					result = other - result;
					result.flip_sign();
					return result;
				}
			}


			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (my_position < reduce) {
				return *this;
			}

			auto shift = my_position - reduce + 1;

			x64_integer mod;
			mod = *this;
			mod >>= shift;

			x64_integer shift_bits;

			qpl::u32 msb = reduce - 1;
			qpl::u32 look_index = qpl::min(this->memory_size() - 1, (reduce + 1) / this->base_max_log());

			for (qpl::i32 i = shift - 1; i >= 0; --i) {

				qpl::i32 skip = qpl::min<qpl::i32>(i, (qpl::signed_cast(reduce) - qpl::signed_cast(msb)));

				auto skippable = (skip > 0);
				if (skippable) {

					mod.left_shift(skip);
					shift_bits = this->quick_dm_shifted_bits(i - (skip - 1), skip);

					auto skip_memory = (skip - 1) / this->base_max_log();
					for (qpl::u32 b = 0u; b < skip_memory; ++b) {
						mod.memory[b] = shift_bits.memory[b];
					}
					mod.memory[skip_memory] |= shift_bits.memory[skip_memory];

					i -= (skip - 1);
					msb += skip;

				}
				else {
					mod <<= 1;
					mod.set_first_bit(this->get_bit(i));
				}


				auto nle = !(mod.quick_dm_less(other, look_index));

				if (nle) {
					//mod.quick_dm_sub(other, look_index);
					mod -= other;
					msb = mod.significant_bit(look_index) - 1;
				}
			}
			return mod;
		}
		x64_integer moded(x64_integer other) const {
			if constexpr (is_signed()) {
				auto other_negative = other.is_negative();
				auto my_negative = this->is_negative();

				if (my_negative && other_negative) {
					x64_integer result = *this;
					result.flip_sign();
					other.flip_sign();
					result.mod(other);
					result.flip_sign();
					return result;
				}
				if (my_negative) {
					x64_integer result = *this;
					result.flip_sign();
					result.mod(other);
					return other - result;
				}
				if (other_negative) {
					x64_integer result = *this;
					other.flip_sign();
					result.mod(other);
					result = other - result;
					result.flip_sign();
					return result;
				}
			}


			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (my_position < reduce) {
				return *this;
			}

			auto shift = my_position - reduce + 1;

			x64_integer mod;
			mod = *this;
			mod >>= shift;

			for (qpl::i32 i = shift - 1; i >= 0; --i) {
				mod <<= 1;
				mod.set_first_bit(this->get_bit(i));

				if (mod >= other) {
					mod -= other;
				}
			}
			return mod;
		}


		std::pair<x64_integer, x64_integer> divmod(x64_integer other) const {
			if constexpr (is_signed()) {
				auto my_neg = this->is_negative();
				auto other_neg = other.is_negative();

				//if (other_neg && my_neg) {
				//	x64_integer result = *this;
				//	result.flip_sign();
				//	other.flip_sign();
				//	return result.dived(other);
				//}
				//else if (other_neg) {
				//	x64_integer result = *this;
				//	other.flip_sign();
				//	result.div(other);
				//	result.flip_bits();
				//	return result;
				//}
				//else if (my_neg) {
				//	x64_integer result = *this;
				//	result.flip_sign();
				//	result.div(other);
				//	result.flip_bits();
				//	return result;
				//}
			}


			x64_integer div;
			div.clear();

			auto reduce = other.significant_bit();
			auto my_position = this->significant_bit();

			if (reduce) --reduce;
			if (my_position) --my_position;

			if (other.is_zero()) {
				return std::make_pair(div, *this);
			}
			if (my_position < reduce) {
				return std::make_pair(div, *this);
			}

			auto shift = my_position - reduce + 1;


			x64_integer mod = *this;
			mod >>= shift;


			x64_integer shift_bits;

			qpl::u32 msb = reduce - 1;
			qpl::u32 look_index = qpl::min(this->memory_size() - 1, (reduce + 1) / this->base_max_log());

			for (qpl::i32 i = shift - 1; i >= 0; --i) {

				qpl::i32 skip = qpl::min<qpl::i32>(i, (qpl::signed_cast(reduce) - qpl::signed_cast(msb)));

				auto skippable = (skip > 0);
				if (skippable) {
					mod.left_shift(skip);
					shift_bits = this->quick_dm_shifted_bits(i - (skip - 1), skip);

					auto skip_memory = (skip - 1) / this->base_max_log();
					for (qpl::u32 b = 0u; b < skip_memory; ++b) {
						mod.memory[b] = shift_bits.memory[b];
					}
					mod.memory[skip_memory] |= shift_bits.memory[skip_memory];

					i -= (skip - 1);
					msb += skip;

				}
				else {
					mod <<= 1;
					mod.set_first_bit(this->get_bit(i));
				}


				auto nle = !(mod.quick_dm_less(other, look_index));

				if (nle) {
					mod.quick_dm_sub(other, look_index);
					msb = mod.significant_bit(look_index) - 1;

					div.set_bit(i, qpl::u32{ 1 });
				}
			}

			return std::make_pair(div, mod);
		}

		void randomize() {
			for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
				this->memory[i] = qpl::random();
			}
		}
		void randomize_bits(qpl::u64 random_bits) {
			qpl::u32 div, mod;
			div = _udiv64(random_bits, qpl::u32_cast(this->base_max_log()), &mod);


			if (div >= this->memory_size()) {
				for (qpl::u32 i = 0u; i < this->memory_size(); ++i) {
					this->memory[i] = qpl::random();
				}
			}
			else {
				for (qpl::u32 i = 0u; i < div; ++i) {
					this->memory[i] = qpl::random();
				}
				if constexpr (this->is_signed()) {
					if (qpl::random_b()) {
						if (mod) {
							this->memory[div] = ~(qpl::random() >> (this->base_max_log() - mod));
						}
						for (qpl::u32 i = div + qpl::bool_cast(mod); i < this->memory_size(); ++i) {
							this->memory[i] = ~qpl::u64{};
						}
						return;
					}
				}
				if (mod) {
					this->memory[div] = qpl::random() >> (this->base_max_log() - mod);
				}
				for (qpl::u32 i = div + qpl::bool_cast(mod); i < this->memory_size(); ++i) {
					this->memory[i] = qpl::u64{};
				}
			}
		}
		void randomize_bits_logarithmic() {
			this->randomize_bits(qpl::random(bit_size()));
		}


		static x64_integer random() {
			x64_integer result;
			result.randomize();
			return result;
		}
		static x64_integer random_bits(qpl::u64 random_bits) {
			x64_integer result;
			result.randomize_bits(random_bits);
			return result;
		}
		static x64_integer random_bits_logarithmic() {
			x64_integer result;
			result.randomize_bits(qpl::random(bit_size()));
			return result;
		}

		template<bool check_sign = sign>
		constexpr std::string memory_base_string(qpl::u32 base, qpl::size seperation = 0, std::string_view prefix = "") const {
			std::ostringstream prefix_stuff;
			if constexpr (check_sign) {
				if (this->is_negative()) {
					prefix_stuff << '-';
				}
			}
			prefix_stuff << prefix;

			std::ostringstream stream;
			bool empty = true;
			bool full = false;

			if constexpr (check_sign) {
				if (this->is_negative()) {
					qpl::size empty_index = 0u;
					while (true) {
						if (this->memory[empty_index] || empty_index >= this->memory_size() - 1) {
							break;
						}
						++empty_index;
					}

					for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
						auto memory = i <= empty_index ? ~this->memory[i] + 1 : ~this->memory[i];
						if (empty && memory || i == 0) {
							empty = false;
						}
						if (!empty) {
							stream << qpl::base_string(memory, qpl::u64_cast(base), "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, full);
							full = true;
						}
					}
				}
				else {
					for (qpl::i32 i = this->memory_size() - 1; i >= 0; --i) {
						if (empty && this->memory[i] || i == 0) {
							empty = false;
						}
						if (!empty) {
							stream << qpl::base_string(this->memory[i], qpl::u64_cast(base), "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, full);
							full = true;
						}
					}
				}
			}
			else {
				for (qpl::i32 i = this->memory_size() - 1; i >= 0; --i) {
					if (empty && this->memory[i] || i == 0) {
						empty = false;
					}
					if (!empty) {
						stream << qpl::base_string(this->memory[i], qpl::u64_cast(base), "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, full);
						full = true;
					}
				}
			}


			if (seperation) {
				return prefix_stuff.str() + qpl::string_seperation(stream.str(), ' ', seperation, false);
			}
			else {
				prefix_stuff << stream.str();
				return prefix_stuff.str();
			}
		}
		template<bool check_sign = sign>
		constexpr std::string memory_base_string_full(qpl::u32 base, qpl::size seperation = 0, std::string_view prefix = "") const {
			std::ostringstream prefix_stuff;
			if constexpr (check_sign) {
				if (this->is_negative()) {
					prefix_stuff << '-';
				}
			}
			prefix_stuff << prefix;

			std::ostringstream stream;

			if constexpr (check_sign) {
				if (this->is_negative()) {
					qpl::size empty_index = 0u;
					while (true) {
						if (this->memory[empty_index] || empty_index >= this->memory_size() - 1) {
							break;
						}
						++empty_index;
					}

					for (qpl::i32 i = qpl::i32_cast(this->memory_size() - 1); i >= 0; --i) {
						auto memory = i <= empty_index ? ~this->memory[i] + 1 : ~this->memory[i];
						stream << qpl::base_string(memory, qpl::u64_cast(base), "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, true);
					}
				}
				else {
					for (qpl::i32 i = this->memory_size() - 1; i >= 0; --i) {
						stream << qpl::base_string(this->memory[i], qpl::u64_cast(base), "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, true);
					}
				}
			}
			else {
				for (qpl::i32 i = this->memory_size() - 1; i >= 0; --i) {
					stream << qpl::base_string(this->memory[i], qpl::u64_cast(base), "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64, true);
				}
			}


			if (seperation) {
				return prefix_stuff.str() + qpl::string_seperation(stream.str(), ' ', seperation, false);
			}
			else {
				prefix_stuff << stream.str();
				return prefix_stuff.str();
			}
		}

		constexpr std::string base_string(qpl::u32 base, qpl::size seperation = 0u, const std::string& prefix = "") const {
			qpl::integer<bits, sign> copy = *this;
			return copy.base_string(base, seperation, prefix);
		}


		template<bool check_sign = sign>
		constexpr std::string hex_string(qpl::size seperation = 0, std::string_view prefix = "0x") const {
			return this->memory_base_string<check_sign>(16u, seperation, prefix);
		}
		template<bool check_sign = sign>
		constexpr std::string hex_string_full(qpl::size seperation = 0, std::string_view prefix = "0x") const {
			return this->memory_base_string_full<check_sign>(16u, seperation, prefix);
		}
		template<bool check_sign = sign>
		constexpr std::string binary_string(qpl::size seperation = 0) const {
			return this->memory_base_string<check_sign>(2u, seperation);
		}
		template<bool check_sign = sign>
		constexpr std::string binary_string_full(qpl::size seperation = 0) const {
			return this->memory_base_string_full<check_sign>(2u, seperation);
		}

		constexpr std::string decimal_string(qpl::size seperation = 0u) const {
			return this->base_string(10u, seperation);
		}
		constexpr std::string string(qpl::size seperation = 0u) const {
			return this->decimal_string(seperation);
		}



		template<typename T>
		x64_integer& operator+=(T value) {
			this->add(value);
			return *this;
		}
		template<typename T>
		x64_integer operator+(T value) const {
			return this->added(value);
		}

		template<typename T>
		x64_integer& operator-=(T value) {
			this->sub(value);
			return *this;
		}
		template<typename T>
		x64_integer operator-(T value) const {
			auto copy = *this;
			copy.sub(value);
			return copy;
		}


		template<typename T>
		x64_integer& operator*=(T value) {
			this->mul(value);
			return *this;
		}
		template<typename T>
		x64_integer operator*(T value) const {
			return this->mulled(value);
		}


		template<typename T>
		x64_integer& operator/=(T value) {
			this->div(value);
			return *this;
		}
		template<typename T>
		x64_integer operator/(T value) const {
			return this->dived(value);
		}
		template<typename T>
		x64_integer& operator%=(T value) {
			this->mod(value);
			return *this;
		}
		template<typename T>
		x64_integer operator%(T value) const {
			return this->moded(value);
		}


		constexpr x64_integer operator~() const {
			return this->flipped_bits();
		}
		constexpr x64_integer& operator++() {
			this->increment();
			return *this;
		}
		constexpr x64_integer operator++(int dummy) const {
			auto copy = *this;
			return ++copy;
		}
		constexpr x64_integer& operator--() {
			this->decrement();
			return *this;
		}
		constexpr x64_integer operator--(int dummy) const {
			auto copy = *this;
			return --copy;
		}

		template<typename T>
		constexpr bool operator==(T value) const {
			return this->equals(value);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool operator==(qpl::x64_integer<bits2, sign2> value) const {
			return this->equals(value);
		}
		template<typename T>
		constexpr bool operator!=(T value) const {
			return !this->equals(value);
		}

		template<qpl::size bits2, bool sign2>
		constexpr bool operator!=(qpl::x64_integer<bits2, sign2> value) const {
			return !this->equals(value);
		}
		template<typename T>
		constexpr bool operator<(T value) const {
			return this->less(value);
		}

		template<qpl::size bits2, bool sign2>
		constexpr bool operator<(qpl::x64_integer<bits2, sign2> value) const {
			return this->less(value);
		}
		template<typename T>
		constexpr bool operator<=(T value) const {
			return !this->greater(value);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool operator<=(qpl::x64_integer<bits2, sign2> value) const {
			return !this->greater(value);
		}

		template<typename T>
		constexpr bool operator>(T value) const {
			return this->greater(value);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool operator>(qpl::x64_integer<bits2, sign2> value) const {
			return this->greater(value);
		}

		template<typename T>
		constexpr bool operator>=(T value) const {
			return !this->less(value);
		}
		template<qpl::size bits2, bool sign2>
		constexpr bool operator>=(qpl::x64_integer<bits2, sign2> value) const {
			return !this->less(value);
		}

		template<typename T>
		x64_integer& operator&=(T value) {
			this->bitwise_and(value);
			return *this;
		}
		template<typename T>
		x64_integer operator&(T value) const {
			auto copy = *this;
			copy.bitwise_and(value);
			return copy;
		}

		template<typename T>
		x64_integer& operator|=(T value) {
			this->bitwise_or(value);
			return *this;
		}
		template<typename T>
		x64_integer operator|(T value) const {
			auto copy = *this;
			copy.bitwise_or(value);
			return copy;
		}

		template<typename T>
		x64_integer& operator^=(T value) {
			this->bitwise_xor(value);
			return *this;
		}
		template<typename T>
		x64_integer operator^(T value) const {
			auto copy = *this;
			copy.bitwise_xor(value);
			return copy;
		}

		template<typename T>
		constexpr x64_integer& operator<<=(T value) {
			this->left_shift(qpl::i64_cast(value));
			return *this;
		}
		template<typename T>
		constexpr x64_integer operator<<(T value) const {
			auto copy = *this;
			copy.left_shift(qpl::i64_cast(value));
			return copy;
		}
		template<typename T>
		constexpr x64_integer& operator>>=(T value) {
			this->right_shift(qpl::i64_cast(value));
			return *this;
		}
		template<typename T>
		constexpr x64_integer operator>>(T value) const {
			auto copy = *this;
			copy.right_shift(qpl::i64_cast(value));
			return copy;
		}


		constexpr bit_proxy operator[](qpl::size index) {
			return bit_proxy(this->memory, index);
		}
		constexpr const bit_proxy operator[](qpl::size index) const {
			return bit_proxy(this->memory, index);
		}
		constexpr bit_proxy front() {
			return bit_proxy(this->memory, 0u);
		}
		constexpr const bit_proxy front() const {
			return bit_proxy(this->memory, 0u);
		}
		constexpr bit_proxy back() {
			return bit_proxy(this->memory, bit_size() - 1);
		}
		constexpr const bit_proxy back() const {
			return bit_proxy(this->memory, bit_size() - 1);
		}

		friend std::ostream& operator<<(std::ostream& os, x64_integer<bits, sign> integer) {
			return (os << integer.string());
		}

		holding_type memory;
	};
#endif

#ifndef QPL_NO_FLOATS
	template<qpl::size exponent_bits, qpl::size mantissa_bits>
	struct floating_point {
		constexpr floating_point() {

		}

		template<typename T>
		constexpr floating_point(const T& value) {
			*this = value;
		}

		template<qpl::size exponent_bytes, qpl::size mantissa_bytes>
		constexpr floating_point(qpl::float_memory<exponent_bytes, mantissa_bytes> memory) {
			this->set(memory);
		}

		template<typename T>
		constexpr floating_point& operator=(const T& value) {
			this->set(value);
			return *this;
		}

		template<typename T>
		constexpr bool operator==(T other) const {
			return this->equals(static_cast<floating_point>(other));
		}
		template<typename T>
		constexpr bool operator!=(T other) const {
			return !this->equals(static_cast<floating_point>(other));
		}
		template<typename T>
		constexpr bool operator<(T other) const {
			return this->less(static_cast<floating_point>(other));
		}
		template<typename T>
		constexpr bool operator<=(T other) const {
			return !this->greater(static_cast<floating_point>(other));
		}
		template<typename T>
		constexpr bool operator>(T other) const {
			return this->greater(static_cast<floating_point>(other));
		}
		template<typename T>
		constexpr bool operator>=(floating_point other) const {
			return !this->less(static_cast<floating_point>(other));
		}

		constexpr void clear() {
			this->exponent.clear();
			this->mantissa.clear();
			this->sign = false;
		}
		constexpr void clear_1() {
			this->exponent.clear();
			this->mantissa.clear();
			this->mantissa.set_bit(mantissa_bit_size() - 1, true);
			this->sign = false;
		}

		constexpr void set(std::string_view string, qpl::u32 base = 10u) {
			qpl::i32 off = 0u;
			if (string.front() == '-') {
				this->sign = true;
				++off;
			}
			if (string.length() > (off + 2)) {
				if (string.substr(off, 2) == "0x") {
					off += 2;
					base = 16u;
				}
				else if (string.substr(off, 2) == "0b") {
					off += 2;
					base = 2u;
				}
			}

			qpl::size e_position = string.length();
			bool scientific_notation = false;
			for (qpl::u32 i = off; i < qpl::i32_cast(string.length() - 1); ++i) {
				auto substr = string.substr(i, 2);
				if (substr == "e+" || substr == "e-") {
					scientific_notation = true;
					e_position = qpl::size_cast(i);
					break;
				}
			}



			qpl::u32 comma_position = string.length();
			for (qpl::u32 i = off; i < string.length(); ++i) {
				if (string[i] == '.') {
					comma_position = i;
					break;
				}
			}
			comma_position -= off;
			qpl::i32 msb = 0;

			this->exponent.clear();
			if (comma_position >= 1) {
				auto str = string.substr(off, comma_position);
				if (str.length() >= this->mantissa.max_base_exponent(base)) {

					qpl::integer<mantissa_bit_size() * 10, 0> c;
					c.set(str, base);

					auto m = c.significant_bit();
					qpl::i64 diff = qpl::i64_cast(m) - mantissa_bit_size();
					if (diff >= 1) {
						c >>= diff;
						this->exponent += diff;
					}
					this->mantissa = c;

				}
				else {
					this->mantissa.set(str, base);
				}
				msb = qpl::i32_cast(this->mantissa.significant_bit());
				if (msb >= 0) {
					this->mantissa <<= (this->mantissa.bit_size() - msb);
					--msb;
				}
			}
			else {
				this->mantissa.clear();
			}
			this->exponent += (msb);


			if (comma_position + off != e_position) {
				qpl::integer<mantissa_bits, false> floating_part;
				qpl::integer<mantissa_bits, false> target;

				auto size = e_position - (comma_position + off);
				size = qpl::min(floating_part.max_base_exponent(base) - 1, size);

				floating_part.set(string.substr(comma_position + 1 + off, size), base);

				target = floating_part.next_base_multiple(base);

				for (qpl::u32 i = comma_position + 1 + off; i < string.length(); ++i) {
					if (string[i] == '0') {
						target *= base;
					}
					else {
						break;
					}
				}

				for (qpl::i32 i = qpl::i32_cast(this->mantissa.bit_size() - (msb + 1) - 1); i >= 0; --i) {
					floating_part <<= 1;
					if (floating_part >= target) {
						floating_part -= target;
						if (i < this->mantissa.bit_size()) {
							this->mantissa.set_bit(i, true);
						}
					}
				}
				if (msb < 0) {
					auto float_msb = (this->mantissa.bit_size()) - (this->mantissa.significant_bit());
					this->mantissa <<= float_msb;
					this->exponent -= float_msb;
				}

			}
		}

		constexpr void set(qpl::f64 value) {
			this->set(std::string_view{ qpl::to_string(value) });
			//auto content = qpl::double_content(value);
			//
			//this->sign = (qpl::u64)content.sign;
			//this->exponent.memory.front() = (qpl::u32)content.exponent;
			//if (this->exponent.get_bit(qpl::double_content::exponent_size() - 1)) {
			//	this->exponent.set_bit(qpl::double_content::exponent_size() - 1, false);
			//}
			//else {
			//	this->exponent.memory.front() = ~(qpl::u32)content.exponent & (qpl::u32_max >> (qpl::bits_in_type<qpl::u32>() - (qpl::double_content::exponent_size() - 1)));
			//	this->exponent.flip_bits();
			//}
			//++this->exponent;
			//this->mantissa = (qpl::u64)content.mantissa;
			//this->mantissa <<= (mantissa_bits - (qpl::double_content::mantissa_size())) - 1;
			//this->mantissa.back() = true;
		}

		template<typename T> requires(qpl::is_stl_integer<T>())
		constexpr void set(T value) {
			if constexpr (qpl::is_signed<T>()) {
				if (value < T{}) {
					this->set(-value);
					this->sign = true;
					return;
				}
			}
			auto value_msb = qpl::significant_bit(value);

			if (this->mantissa_bit_size() < value_msb) {
				this->mantissa = value >> (value_msb - this->mantissa_bit_size() - 1);
			}
			else {
				this->mantissa = value;
			}

			auto msb = qpl::i64_cast(this->mantissa.significant_bit());
			this->exponent = msb - 1;
			this->mantissa <<= mantissa_bit_size() - msb;


			if (this->mantissa_bit_size() < value_msb) {
				this->exponent += (value_msb - this->mantissa_bit_size());
			}
		}
		template <qpl::size bits, bool sign>
		constexpr void set(integer<bits, sign> value) {
			if constexpr (sign) {
				if (value.is_negative()) {
					value.flip_sign();
					this->flip_sign();
					this->set(value);
					return;
				}
			}

			auto value_msb = value.significant_bit();

			this->mantissa = value.last_n_bits(this->mantissa_bit_size());
			auto msb = qpl::i64_cast(this->mantissa.significant_bit());
			this->exponent = msb - 1;
			this->mantissa <<= mantissa_bit_size() - msb;


			if (this->mantissa_bit_size() < value_msb) {
				this->exponent += (value_msb - this->mantissa_bit_size());
			}
		}
		template <qpl::u32 base, bool sign>
		constexpr void set(dynamic_integer<base, sign> value) {
			if (base != 2u) {
				this->set(value.as_type<2u>());
				return;
			}

			auto value_digits = value.digits();

			this->mantissa = value.last_n_digits(this->mantissa_bit_size());
			auto msb = this->mantissa.significant_bit();
			this->exponent = msb - 1;
			this->mantissa <<= mantissa_bit_size() - msb;


			if (this->mantissa_bit_size() < value_digits) {
				this->exponent += (value_digits - this->mantissa_bit_size());
			}

			if constexpr (sign) {
				this->sign = value.content.sign;
			}
			else {
				this->sign = false;
			}
		}
		template<qpl::size exponent_bits2, qpl::size mantissa_bits2>
		constexpr void set(floating_point<exponent_bits2, mantissa_bits2> value) {
			this->exponent = value.exponent;
			this->sign = value.sign;

			//0 1 2 3
			//0 1 2
			qpl::u32 i = 0u;
			for (; i < qpl::min(this->mantissa.memory_size(), value.mantissa.memory_size()); ++i) {
				this->mantissa.memory[this->mantissa.memory_size() - 1 - i] = value.mantissa.memory[value.mantissa.memory_size() - 1 - i];
			}

			for (; i < this->mantissa.memory_size(); ++i) {
				this->mantissa.memory[i - value.mantissa.memory_size()] = qpl::u32{ 0u };
			}
		}
		template<qpl::size exponent_bytes, qpl::size mantissa_bytes>
		constexpr void set(qpl::float_memory<exponent_bytes, mantissa_bytes> memory) {
			qpl::u32 i = 0u;
			for (; i < qpl::min(this->exponent.memory_size(), exponent_bytes); ++i) {
				this->exponent.memory[this->exponent.memory_size() - 1 - i] = memory.exponent[exponent_bytes - 1 - i];
			}

			for (; i < this->exponent.memory_size(); ++i) {
				if (qpl::get_bit(memory.exponent[exponent_bytes - 1], qpl::bits_in_type<qpl::u32>() - 1)) {
					this->exponent.memory[i - exponent_bytes] = qpl::u32{ ~0u };
				}
				else {
					this->exponent.memory[i - exponent_bytes] = qpl::u32{ 0u };
				}
			}

			i = 0u;
			for (; i < qpl::min(this->mantissa.memory_size(), mantissa_bytes); ++i) {
				this->mantissa.memory[this->mantissa.memory_size() - 1 - i] = memory.mantissa[mantissa_bytes - 1 - i];
			}

			for (; i < this->mantissa.memory_size(); ++i) {
				this->mantissa.memory[i - mantissa_bytes] = qpl::u32{ 0u };
			}

			this->sign = memory.sign;
		}


		std::string get_float_memory_string() const {
			std::ostringstream stream;
			stream << "qpl::float_memory{ std::array" << qpl::container_to_hex_string(this->exponent.memory) << ", std::array" << qpl::container_to_hex_string(this->mantissa.memory) << ", " << qpl::bool_string(this->sign) << "}";
			return stream.str();
		}
		constexpr double to_double() const {
			qpl::double_content content;
			content.sign = this->sign;
			content.exponent = this->exponent + (1 << (qpl::double_content::exponent_size() - 1)) - 1;
			content.mantissa = this->mantissa.last_n_bits(qpl::double_content::mantissa_size() + 1);
			return content.to_double();
		}

		constexpr void normalize_mantissa() {
			auto msb_diff = mantissa_bit_size() - this->mantissa.significant_bit();
			this->exponent -= msb_diff;
			this->mantissa <<= msb_diff;
		}

		template<typename T>
		constexpr void add(T value) {
			this->add(floating_point{ value });
		}
		constexpr void add(floating_point other) {
			if (this->sign != other.sign) {
				if (this->is_negative()) {
					this->flip_sign();
					*this = other - *this;
					return;
				}
				else {
					other.flip_sign();
					this->sub(other);
					return;
				}
			}

			auto exp_diff = qpl::i64_cast(this->exponent - other.exponent);


			if (exp_diff <= 0) {
				this->exponent += -exp_diff;
				this->mantissa >>= -exp_diff;

				auto overflow = this->mantissa.add_check_overflow(other.mantissa);

				if (overflow) {
					++this->exponent;
					this->mantissa >>= 1;
					this->mantissa.set_bit(mantissa_bit_size() - 1, true);
				}
			}
			else {
				other.mantissa >>= exp_diff;

				auto overflow = this->mantissa.add_check_overflow(other.mantissa);

				if (overflow) {
					++this->exponent;
					this->mantissa >>= 1;
					this->mantissa.set_bit(mantissa_bit_size() - 1, true);
				}
			}
		}

		template<typename T>
		constexpr void sub(T value) {
			this->sub(floating_point{ value });
		}
		constexpr void sub(floating_point other) {
			if (this->sign != other.sign) {
				if (this->is_negative()) {
					this->flip_sign();
					this->add(other);
					this->flip_sign();
					return;
				}
				else {
					other.flip_sign();
					this->add(other);
					return;
				}
			}


			auto exp_diff = qpl::i64_cast(this->exponent - other.exponent);


			if (exp_diff <= 0) {
				this->exponent += -exp_diff;
				this->mantissa >>= -exp_diff;

				if (this->mantissa < other.mantissa) {
					this->mantissa = other.mantissa - this->mantissa;
					this->flip_sign();
				}
				else {
					this->mantissa -= other.mantissa;
				}

			}
			else {
				other.mantissa >>= exp_diff;

				this->mantissa -= other.mantissa;
			}
			this->normalize_mantissa();
		}


		template<typename T>
		constexpr void mul(T value) {
			this->mul(floating_point{ value });
		}
		constexpr void mul(floating_point other) {
			
			this->exponent += other.exponent + 1;

			this->mantissa.mul_left_side(other.mantissa);

			this->normalize_mantissa();

			this->sign ^= other.sign;
		}


		template<typename T>
		constexpr void div(T value) {
			this->div(floating_point{ value });
		}
		constexpr void div(floating_point other) {
			this->exponent -= other.exponent + 1;

			qpl::integer<mantissa_bit_size() * 2, 0> temp;

			for (qpl::u32 i = 0u; i < this->mantissa.memory_size(); ++i) {
				temp[i] = qpl::u32{};
			}
			for (qpl::u32 i = 0u; i < this->mantissa.memory_size(); ++i) {
				temp.memory[i + this->mantissa.memory_size()] = this->mantissa.memory[i];
			}

			temp.div(other.mantissa);

			auto msb_diff = mantissa_bit_size() * 2 - temp.significant_bit();
			temp <<= msb_diff;

			for (qpl::u32 i = 0u; i < this->mantissa.memory_size(); ++i) {
				this->mantissa.memory[i] = temp.memory[i + this->mantissa.memory_size()];
			}


			this->exponent -= (msb_diff - mantissa_bit_size());

			this->sign ^= other.sign;
		}


		template<typename T>
		constexpr void mod(T value) {
			this->mod(floating_point{ value });
		}
		constexpr void mod(floating_point other) {
			auto exp_diff = qpl::i64_cast(this->exponent - other.exponent);

			if (exp_diff < 0) {
				return;
			}
			else {
				other.mantissa >>= exp_diff;
			}

			this->mantissa.mod(other.mantissa);

			this->normalize_mantissa();

			this->sign ^= other.sign;
		}

		constexpr std::pair<qpl::integer<mantissa_bits, 1>, floating_point> div_mod(floating_point other) const {
			auto exp_diff = qpl::i64_cast(this->exponent - other.exponent);

			floating_point m = *this;
			if (exp_diff < 0) {
				return std::make_pair(0, *this);
			}
			else {
				other.mantissa >>= exp_diff;
			}

			auto [div, mod] = m.mantissa.div_mod(other.mantissa);
			m.mantissa = mod;
			m.normalize_mantissa();

			bool negative = this->sign ^ other.sign;
			if (negative) {
				div.set_negative();
			}

			return std::make_pair(div, m);
		}

		constexpr operator bool() const {
			return qpl::bool_cast(this->mantissa);
		}

		constexpr bool equals(floating_point other) const {
			return this->sign == other.sign && this->exponent == other.exponent && this->mantissa == other.mantissa;
		}
		constexpr bool less(floating_point other) const {
			if (this->sign != other.sign) {
				return this->sign;
			}
			if (this->exponent != other.exponent) {
				return this->exponent < other.exponent;
			}
			return this->mantissa < other.mantissa;
		}
		constexpr bool greater(floating_point other) const {
			if (this->sign != other.sign) {
				return other.sign;
			}
			if (this->exponent != other.exponent) {
				return this->exponent > other.exponent;
			}
			return this->mantissa > other.mantissa;
		}

		constexpr void left_shift(qpl::i64 shift) {
			this->exponent += shift;
		}
		constexpr void right_shift(qpl::i64 shift) {
			this->exponent -= shift;
		}


		constexpr void calculate_pi() {

			this->clear();

			auto before = *this;


			qpl::ub n = 1;
			qpl::ub d = 1;
			for (qpl::ub i = 0u;; ++i) {


				if (i) {
					n *= (i * 2);
				}
				d *= (i * 2 + 1);

				floating_point add = n;
				add /= d;
				add >>= i;

				this->add(add);

				if (before == *this) {
					break;
				}
				before = *this;

				if (qpl::get_time_signal(1.0)) {
					qpl::println(this->get_float_memory_string());
				}
			}
			this->left_shift(1);
		}

		constexpr static floating_point pi() {
			constexpr floating_point pi = qpl::lut::pi;
			return pi;
		}
		constexpr static floating_point e() {
			constexpr floating_point e = qpl::lut::e;
			return e;
		}
		constexpr static floating_point ln2() {
			constexpr floating_point ln2 = qpl::lut::ln2;
			return ln2;
		}
		constexpr static floating_point sqrt2() {
			constexpr floating_point sqrt2 = qpl::lut::sqrt2;
			return sqrt2;
		}

		constexpr void arithmetic_mean(floating_point value) {
			floating_point copy;

			//auto before = value;
			//auto less = value < *this;
			while (true) {
				copy = *this;
				this->add(value);
				--this->exponent;

				if (*this == value) {
					break;
				}

				//if ((value < *this) != less) {
				//	break;
				//}
				//before = value;

				value.mul(copy);
				value.sqrt();
			}
		}
		constexpr floating_point arithmetic_meaned() const {
			return floating_point::arithmetic_mean(*this);
		}

		constexpr void ln_precision(qpl::u32 bits = mantissa_bit_size()) {
			if (this->exponent != -1ll) {
				auto e = qpl::i64_cast(this->exponent);
				this->exponent.clear_flip();
				this->ln_precision(bits);
				auto ln2 = floating_point::ln2();
				ln2.mul(e + 1);
				this->add(ln2);
				return;
			}

			floating_point m;
			m.clear_1();

			floating_point div = *this;
			div <<= bits;
			floating_point s;
			s.clear_1();
			s <<= 2;
			s.div(div);


			m.arithmetic_mean(s);

			m <<= 1;


			*this = floating_point::pi();
			this->div(m);

			auto ln = floating_point::ln2();
			ln.mul(bits);
			this->sub(ln);
		}
		constexpr void ln() {
			this->ln_precision();
		}
		constexpr floating_point static ln(floating_point value, qpl::u32 bits = mantissa_bit_size()) {
			value.ln_precision(bits);
			return value;
		}
		constexpr floating_point lned(qpl::u32 bits = mantissa_bit_size()) const {
			return floating_point::ln(*this, bits);
		}

		constexpr void exp_precision(qpl::u32 bits = mantissa_bit_size() >> 1) {
			if (!this->has_floating_part()) {
				auto integer = this->integer_part();

				auto negative = integer.is_negative();
				integer.set_positive();

				if (integer.significant_bit() >= 32) {
					this->clear();
					return;
				}

				this->clear_1();
				bool set_init = true;
				for (qpl::u32 i = 0u; i < 32; ++i) {
					if (integer.get_bit(i)) {
						if (set_init) {
							if (negative) {
								this->set(qpl::lut::e_exponentials[i + 31]);
							}
							else {
								this->set(qpl::lut::e_exponentials[i]);
							}
							set_init = false;
						}
						else {
							if (negative) {
								this->mul(qpl::lut::e_exponentials[i + 31]);
							}
							else {
								this->mul(qpl::lut::e_exponentials[i]);
							}
						}
					}
				}
				return;
			}


			if (this->is_negative()) {
				this->flip_sign();
				this->exp_precision(bits);
				this->invert();
				return;
			}

			auto integer = this->integer_part();
			if (integer) {
				*this = this->floating_part();
				this->exp_precision(bits);
				floating_point mul = qpl::i64_cast(integer);
				mul.exp();
				this->mul(mul);
				return;
			}


			auto copy = *this;
			this->add(bits);
			floating_point x = bits;
			for (qpl::u32 i = bits - 1; i > 1; --i) {
				x.mul(i);
				this->mul(copy);
				this->add(x);
			}
			this->mul(copy);
			this->add(x);

			this->div(x);
		}
		constexpr void exp() {
			this->exp_precision();
		}
		constexpr floating_point static exp(floating_point value, qpl::u32 bits = mantissa_bit_size() >> 1) {
			value.exp_precision(bits);
			return value;
		}
		constexpr floating_point exped(qpl::u32 bits = mantissa_bit_size() >> 1) const {
			return floating_point::exp(*this, bits);
		}


		constexpr void pow_precision(floating_point value, qpl::u32 bits = mantissa_bit_size()) {
			if (value.is_negative()) {
				value.flip_sign();
				this->pow(value, bits);
				this->invert();
				return;
			}
			if (!value.has_floating_part()) {
				auto n = value.integer_part_unsigned();
				if (n == 0u) {
					this->clear_1();
				}
				else if (n == 1u) {
					return;
				}

				auto p = *this;
				auto msb = n.significant_bit();
				bool init_set = true;
				this->clear_1();
				for (qpl::u32 i = 0u; i < msb; ++i) {
					if (n.get_bit(i)) {
						if (init_set) {
							this->set(p);
							init_set = false;
						}
						else {
							this->mul(p);
						}
					}
					p.mul(p);
				}
			}
			else {
				if (this->is_negative()) {
					this->clear();
					return;
				}

				auto integer = value.integer_part_unsigned();
				value = value.floating_part();



				if (integer) {
					auto v = *this;

					qpl::begin_benchmark_end_previous("pow", "ln");

					this->ln_precision(bits);
					qpl::begin_benchmark_end_previous("pow", "mul");
					this->mul(value);
					qpl::begin_benchmark_end_previous("pow", "exp");
					this->exp_precision(bits >> 1);
					qpl::end_benchmark();

					v.pow(integer);
					this->mul(v);
				}
				else {
					qpl::begin_benchmark_end_previous("pow", "ln");
					this->ln_precision(bits);
					qpl::begin_benchmark_end_previous("pow", "mul");
					this->mul(value);
					qpl::begin_benchmark_end_previous("pow", "exp");
					this->exp_precision(bits >> 1);
					qpl::end_benchmark();
				}
			}
		}
		constexpr void pow(floating_point value) {
			this->pow_precision(value);
		}

		constexpr static floating_point pow(floating_point a, floating_point b, qpl::u32 bits = mantissa_bit_size()) {
			a.pow_precision(b, bits);
			return a;
		}
		constexpr floating_point powed(floating_point other, qpl::u32 bits = mantissa_bit_size()) const {
			return floating_point::pow(*this, other, bits);
		}

		constexpr void sin() {
			if (this->is_negative()) {
				if (this->less(-(floating_point::pi() << 1))) {
					this->mod(floating_point::pi() << 1);
					this->sin();
					return;
				}
				else {
					this->add(floating_point::pi() << 1);
				}
			}
			if (this->greater(floating_point::pi() << 1)) {
				this->mod(floating_point::pi() << 1);
				this->sin();
				return;
			}

			auto square = *this;
			square.mul(square);

			auto n = *this;
			qpl::u32 i = 1u;
			floating_point d = i;
			floating_point div;

			floating_point before;
			while (true) {
				n.mul(square);
				div = n;

				i += 2u;
				d *= ((i - 1) * i);
				div.div(d);

				this->sub(div);

				if (before == *this) {
					return;
				}
				before = *this;

				n.mul(square);
				div = n;

				i += 2u;
				d *= ((i - 1) * i);
				div.div(d);

				this->add(div);
				if (before == *this) {
					return;
				}
				before = *this;
			}
		}
		constexpr static floating_point sin(floating_point value) {
			value.sin();
			return value;
		}
		constexpr floating_point sined() const {
			return floating_point::sin(*this);
		}


		constexpr void cos() {
			if (this->is_negative()) {
				if (this->less(-(floating_point::pi() << 1))) {
					this->mod(floating_point::pi() << 1);
					this->cos();
					return;
				}
				else {
					this->add(floating_point::pi() << 1);
				}
			}
			if (this->greater(floating_point::pi() << 1)) {
				this->mod(floating_point::pi() << 1);
				this->cos();
				return;
			}

			auto square = *this;
			square.mul(square);

			auto n = square;
			qpl::u32 i = 0u;
			floating_point d = 1;
			floating_point div;

			floating_point before;

			this->clear_1();
			while (true) {
				div = n;

				i += 2u;
				d *= ((i - 1) * i);
				div.div(d);

				this->sub(div);

				if (before == *this) {
					return;
				}
				before = *this;

				n.mul(square);
				div = n;

				i += 2u;
				d *= ((i - 1) * i);
				div.div(d);

				this->add(div);
				if (before == *this) {
					return;
				}
				before = *this;
				n.mul(square);
			}
		}
		constexpr static floating_point cos(floating_point value) {
			value.cos();
			return value;
		}
		constexpr floating_point cosed() const {
			return floating_point::cos(*this);
		}


		constexpr void sqrt() {
			if (this->is_zero() || this->is_negative()) {
				this->clear();
				return;
			}
			auto expo = this->exponent >> 1;
			this->exponent -= expo << 1;

			auto dividend = *this;
			floating_point last;
			floating_point loop_last;

			//qpl::println("sqrt( ", *this, " ) = ");

			while (true) {

				auto div = dividend;
				div.div(*this);
				this->add(div);

				*this >>= 1;

				if (*this == last || *this == loop_last) {
					break;
				}
				loop_last = last;
				last = *this;

			}

			this->exponent = expo;

			/*
						auto expo = this->exponent;
			this->exponent.clear();

			auto dividend = *this;
			floating_point last;

			qpl::println("sqrt( ", *this, " )");

			while (true) {
				auto div = dividend;
				div.div(*this);
				this->add(div);
				*this >>= 1;

				if (*this == last) {
					break;
				}
				last = *this;

			}

			this->exponent = expo >> 1;
			if (expo % 2 == 1) {
				this->mul(*this);
			}
			*/
		}
		constexpr static floating_point sqrt(floating_point value) {
			value.sqrt();
			return value;
		}
		constexpr floating_point sqrted() const {
			return floating_point::sqrt(*this);
		}


		constexpr void invert() {
			auto copy = *this;
			this->exponent.clear();
			this->mantissa.clear();
			this->mantissa.set_bit(mantissa_bit_size() - 1, true);
			this->div(copy);
		}
		constexpr static floating_point invert(floating_point value) {
			value.invert();
			return value;
		}
		constexpr floating_point inverted() const {
			return floating_point::invert(*this);
		}
		static floating_point random() {
			floating_point result;
			result.sign = qpl::random_b();
			result.exponent.randomize();
			result.mantissa.randomize();
			return result;
		}
		static floating_point random(floating_point min, floating_point max) {
			auto diff = max;
			diff.sub(min);

			auto result = floating_point::random_0_1();
			result.mul(diff);
			result.add(min);
			return result;
		}
		static floating_point random_0_1() {
			floating_point result;
			result.mantissa.randomize();
			result.mantissa.set_bit(mantissa_bit_size() - 1, true);

			auto d = qpl::random(0.0, 1.0);
			result.exponent = qpl::i32_cast(std::log(d) / std::log(2)) - 1;
			return result;
		}
		static floating_point random_0_1_negative() {
			auto r = floating_point::random_0_1();
			r.sign = qpl::random_b();
			return r;
		}

		constexpr static integer<mantissa_bits, 0> max_representable_integer() {
			integer<mantissa_bit_size(), 0> result;
			result.flip_bits();
			return result;
		}
		constexpr static floating_point epsilon() {
			floating_point result;
			result.mantissa = 1;
			return result;
		}
		constexpr static floating_point one_minus_epsilon() {
			floating_point result;
			result.exponent = -1;
			result.mantissa = result.mantissa.max();
			return result;
		}
		constexpr static floating_point one_plus_epsilon() {
			floating_point result;
			result.mantissa = 1;
			result.mantissa.set_bit(mantissa_bit_size() - 1, true);
			return result;
		}
		constexpr static floating_point max() {
			floating_point result;
			result.exponent = result.exponent.max();
			result.mantissa = result.mantissa.max();
			return result;
		}
		constexpr static floating_point min() {
			floating_point result;
			result.exponent = result.exponent.min();
			result.mantissa = result.mantissa.min();
			return result;
		}

		std::string binary_double_string() const {
			std::ostringstream stream;
			auto exponent_str = this->exponent.binary_string_full<false>();
			stream << this->sign << exponent_str.front() << qpl::string_last_n_characters(exponent_str, 10) << qpl::string_first_n_characters(this->mantissa.binary_string_full(), 52);
			return stream.str();
		}


		constexpr static qpl::size precision_digits(qpl::u32 base = 10u) {
			return qpl::integer<mantissa_bits, false>::max_base_exponent(base);
		}
		constexpr bool is_zero() const {
			return this->mantissa.is_zero();
		}
		constexpr bool is_negative() const {
			return this->sign;
		}
		constexpr bool is_positive() const {
			return !this->sign;
		}
		constexpr void flip_sign() {
			this->sign = !this->sign;
		}
		constexpr void set_negative() {
			this->sign = true;
		}
		constexpr void set_positive() {
			this->sign = false;
		}



		constexpr floating_point operator-() const {
			auto copy = *this;
			copy.flip_sign();
			return copy;
		}
		constexpr floating_point operator+() const {
			if (this->is_positive()) {
				return *this;
			}
			auto copy = *this;
			copy.set_positive();
			return copy;
		}

		constexpr static qpl::size exponent_bit_size() {
			return exponent_bits;
		}
		constexpr static qpl::size mantissa_bit_size() {
			return mantissa_bits;
		}

		std::string base_string(qpl::u32 base = 10u, qpl::u32 precision = qpl::u32_max) const {
			if (precision == qpl::u32_max) {
				precision = precision_digits(base);
			}

			
			qpl::ub left = this->mantissa;

			auto shift = ((qpl::i64_cast(this->exponent) + 1) - qpl::i64_cast(mantissa_bit_size()));
			left <<= shift;


			auto bits = qpl::i64_cast(mantissa_bit_size() + 1) - (qpl::i64_cast(this->exponent) + 2);
			qpl::ub right;
			if (bits > 0) {
				right = this->mantissa.first_n_bits(bits);
			}

			std::string floating_str = "";
			qpl::i32 zeroes = 0;
			if (right) {
				auto base_quo = std::log(base) / std::log(2);
				auto base_exp = qpl::u64_cast(bits / base_quo + 1);

				qpl::ub floating_part = 0;
				qpl::ub base_frac = qpl::ub::pow(base, base_exp);

				for (qpl::i32 b = bits; b >= 0; --b) {
					if (right.get_digit(b)) {
						floating_part += base_frac;
					}
					base_frac >>= 1;
				}

				floating_str = floating_part.base_string(base);

				zeroes = qpl::max(0, qpl::i32_cast(base_exp) - qpl::i32_cast(floating_str.length()));
				if (precision < base_exp && precision < floating_str.length()) {

					bool round = floating_str[precision] >= qpl::base_char((base / 2) + (base % 2), base);


					floating_str = floating_str.substr(0, zeroes + precision);

					if (round) {
						qpl::i32 i = floating_str.length() - 1;
						for (; i >= 0; --i) {
							if (floating_str[i] == qpl::base_char(base - 1, base)) {
								floating_str[i] = qpl::base_char(0u, base);
							}
							else {
								floating_str[i] = qpl::base_char(qpl::from_base_char<qpl::u32>(floating_str[i], base) + 1, base);
								break;
							}
						}
						if (i == -1) {
							++left;
						}
					}
				}

				qpl::i32 trailing_zero = qpl::i32_cast(floating_str.length()) - 1;
				while (trailing_zero >= 0) {
					if (floating_str[trailing_zero] != qpl::base_char(0u, base)) {
						break;
					}
					else {
						--trailing_zero;
					}
				}
				floating_str = floating_str.substr(0, trailing_zero + 1);
			}



			std::ostringstream result;

			if (this->sign) {
				result << '-';
			}
			result << left.base_string(base);

			if (floating_str.length()) {
				result << '.';
			}

			if (floating_str.length()) {
				if (zeroes > 0) {
					result << qpl::to_string_repeat(qpl::base_char(0u, base), zeroes);
				}
				result << floating_str;
			}
			return result.str();
		}

		std::string quick_scientific_notation(qpl::u32 base = 16u, std::string_view prefix = "", qpl::u32 precision = qpl::u32_max) const {
			auto m = this->mantissa;
			m <<= 1;

			auto content_str = m.base_string(base);

			if (precision < content_str.length()) {

				bool round = content_str[precision + 1] >= qpl::base_char((base / 2) + (base % 2), base);


				content_str = content_str.substr(0, precision + 1);

				if (round) {
					qpl::i32 i = content_str.length() - 1;
					for (; i >= 0; --i) {
						if (content_str[i] == qpl::base_char(base - 1, base)) {
							content_str[i] = qpl::base_char(0u, base);
						}
						else {
							content_str[i] = qpl::base_char(qpl::from_base_char<qpl::u32>(content_str[i], base) + 1, base);
							break;
						}
					}
					if (i == -1) {
						content_str = qpl::base_char(1u, base) + content_str;
					}
				}
			}

			qpl::i32 trailing_zero = qpl::i32_cast(content_str.length()) - 1;
			while (trailing_zero >= 0) {
				if (content_str[trailing_zero] != qpl::base_char(0u, base)) {
					break;
				}
				else {
					--trailing_zero;
				}
			}
			content_str = content_str.substr(0, trailing_zero + 1);

			std::ostringstream result;
			if (this->sign) {
				result << '-';
			}
			result << prefix;
			result << qpl::base_char(1u, base);
			if (!content_str.empty()) {
				result << '.' << content_str;
			}


			if (exponent.is_negative()) {
				result << " >> " << this->exponent.flipped_sign().decimal_string();
			}
			else {
				result << " << " << this->exponent.decimal_string();
			}
			return result.str();
		}

		std::string scientific_notation(qpl::u32 base = 10u, std::string_view prefix = "", qpl::u32 precision = qpl::u32_max) const {
			if (precision == qpl::u32_max) {
				precision = precision_digits(base);
			}

			if (qpl::base_full_bit_usage(base)) {
				return this->quick_scientific_notation(base, prefix, precision);
			}

			qpl::ub left = this->mantissa;
			auto shift = ((qpl::i64_cast(this->exponent) + 1) - qpl::i64_cast(mantissa_bit_size()));
			left <<= shift;


			auto bits = qpl::i64_cast(mantissa_bit_size() + 1) - (qpl::i64_cast(this->exponent) + 2);
			qpl::ub right;
			if (bits > 0) {
				right = this->mantissa.first_n_bits(bits);
			}

			std::string floating_str = "";
			qpl::i32 zeroes = 0;
			if (right) {
				auto base_quo = std::log(base) / std::log(2);
				auto base_exp = qpl::u64_cast(bits / base_quo + 1);

				qpl::ub floating_part = 0;
				qpl::ub base_frac = qpl::ub::pow(base, base_exp);

				for (qpl::i32 b = bits; b >= 0; --b) {
					if (right.get_digit(b)) {
						floating_part += base_frac;
					}
					base_frac >>= 1;
				}

				floating_str = floating_part.base_string(base);

				zeroes = qpl::max(0, qpl::i32_cast(base_exp) - qpl::i32_cast(floating_str.length()));
			}


			std::ostringstream content;


			auto left_str = left.base_string(base);

			qpl::i32 exponent = left_str.length() - 1;
			if (left_str == "0") {
				exponent = -zeroes - 1;
				content << floating_str;
			}
			else {
				content << left_str;
				if (left_str.length() > 1) {
					if (zeroes > 0) {
						content << qpl::to_string_repeat(qpl::base_char(0u, base), zeroes);
					}
					content << floating_str;
				}
				else if (floating_str.length()) {
					if (zeroes > 0) {
						content << qpl::to_string_repeat(qpl::base_char(0u, base), zeroes);
					}
					content << floating_str;
				}
			}
			auto content_str = content.str();

			if (precision < content_str.length()) {

				bool round = content_str[precision + 1] >= qpl::base_char((base / 2) + (base % 2), base);


				content_str = content_str.substr(0, precision + 1);

				if (round) {
					qpl::i32 i = content_str.length() - 1;
					for (; i >= 0; --i) {
						if (content_str[i] == qpl::base_char(base - 1, base)) {
							content_str[i] = qpl::base_char(0u, base);
						}
						else {
							content_str[i] = qpl::base_char(qpl::from_base_char<qpl::u32>(content_str[i], base) + 1, base);
							break;
						}
					}
					if (i == -1) {
						content_str = qpl::base_char(1u, base) + content_str;
						--zeroes;
					}
				}
			}

			qpl::i32 trailing_zero = qpl::i32_cast(content_str.length()) - 1;
			while (trailing_zero >= 0) {
				if (content_str[trailing_zero] != qpl::base_char(0u, base)) {
					break;
				}
				else {
					--trailing_zero;
				}
			}
			content_str = content_str.substr(0, trailing_zero + 1);

			std::ostringstream result;
			if (this->sign) {
				result << '-';
			}

			if (content_str.length()) {
				result << content_str.substr(0, 1);
				if (content_str.length() > 1) {
					result << '.';
					result << content_str.substr(1);
				}
			}
			else {
				result << qpl::base_char(0u, base);
			}
			if (exponent) {
				result << 'e';
				if (exponent > 0) {
					result << '+' << qpl::base_string(qpl::u32_cast(exponent), base, "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64);
				}
				else {
					result << '-' << qpl::base_string(qpl::u32_cast(-exponent), base, "", base <= 36 ? qpl::base_format::base36l : qpl::base_format::base64);
				}
			}

			return result.str();
		}
		std::string hex_scientific_notation(std::string_view prefix = "0x", qpl::u32 precision = qpl::u32_max) {
			return this->scientific_notation(16u, prefix, precision);
		}

		qpl::ub dynamic_integer_part_unsigned() const {
			qpl::ub result = this->mantissa;
			auto shift = ((qpl::i64_cast(this->exponent) + 1) - qpl::i64_cast(mantissa_bit_size()));
			result <<= shift;
			return result;
		}
		qpl::ib dynamic_integer_part() const {
			qpl::ib result = this->mantissa;
			auto shift = ((qpl::i64_cast(this->exponent) + 1) - qpl::i64_cast(mantissa_bit_size()));
			result <<= shift;
			if (this->is_negative()) {
				result.flip_sign();
			}
			return result;
		}
		template<qpl::size bits = mantissa_bits>
		constexpr qpl::integer<bits, 0> integer_part_unsigned() const {
			qpl::integer<bits, 0> result = this->mantissa;
			auto shift = ((qpl::i64_cast(this->exponent) + 1) - qpl::i64_cast(mantissa_bit_size()));
			result <<= shift;
			return result;
		}

		template<qpl::size bits = mantissa_bits>
		constexpr qpl::integer<bits, 1> integer_part() const {
			qpl::integer<bits, 1> result;
			result = this->integer_part_unsigned();
			if (this->is_negative()) {
				result.flip_sign();
			}
			return result;
		}
		constexpr floating_point floating_part() const {
			if (this->exponent.is_negative()) {
				return *this;
			}

			auto copy = *this;
			copy.mantissa <<= qpl::i64_cast(copy.exponent) + 1;
			copy.exponent.clear_flip();
			copy.normalize_mantissa();
			return copy;
		}


		constexpr bool has_integer_part() const {
			return this->exponent.is_positive();
		}
		constexpr bool has_floating_part() const {
			if (this->exponent.is_negative()) {
				return true;
			}
			if (this->exponent > this->mantissa_bit_size()) {
				return false;
			}
			auto copy = this->mantissa;
			copy <<= qpl::i64_cast(this->exponent) + 1;
			return copy != 0u;
		}


		std::string string() const {
			return this->base_string();
		}
		std::string string_precision(qpl::u32 precision) const {
			return this->base_string(10u, precision);
		}
		std::string hex_string(std::string_view prefix = "0x") const {
			return qpl::to_string(prefix, this->base_string(16u));
		}
		std::string binary_string(qpl::u32 precision = qpl::u32_max) const {
			return this->base_string(2u, precision);
		}

		template<typename T>
		constexpr floating_point& operator+=(T value) {
			this->add(value);
			return *this;
		}
		template<typename T>
		constexpr floating_point operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}

		template<typename T>
		constexpr floating_point& operator-=(T value) {
			this->sub(value);
			return *this;
		}
		template<typename T>
		constexpr floating_point operator-(T value) const {
			auto copy = *this;
			copy -= value;
			return copy;
		}

		template<typename T>
		constexpr floating_point& operator*=(T value) {
			this->mul(value);
			return *this;
		}
		template<typename T>
		constexpr floating_point operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}


		template<typename T>
		constexpr floating_point& operator/=(T value) {
			this->div(value);
			return *this;
		}
		template<typename T>
		constexpr floating_point operator/(T value) const {
			auto copy = *this;
			copy.div(value);
			return copy;
		}


		template<typename T>
		constexpr floating_point& operator%=(T value) {
			this->mod(value);
			return *this;
		}
		template<typename T>
		constexpr floating_point operator%(T value) const {
			auto copy = *this;
			copy %= value;
			return copy;
		}


		template<typename T>
		constexpr floating_point& operator<<=(T value) {
			this->left_shift(qpl::i64_cast(value));
			return *this;
		}
		template<typename T>
		constexpr floating_point operator<<(T value) const {
			auto copy = *this;
			return copy <<= value;
		}
		template<typename T>
		constexpr floating_point& operator>>=(T value) {
			this->right_shift(qpl::i64_cast(value));
			return *this;
		}
		template<typename T>
		constexpr floating_point operator>>(T value) const {
			auto copy = *this;
			return copy >>= value;
		}


		template<typename T>
		constexpr floating_point& operator^=(T value) {
			this->pow(value);
			return *this;
		}
		template<typename T>
		constexpr floating_point operator^(T value) const {
			auto copy = *this;
			copy.pow(value);
			return copy;
		}


		friend std::ostream& operator<<(std::ostream& os, const floating_point& value) {
			return (os << value.string());
		}

		qpl::integer<exponent_bits, 1> exponent;
		qpl::integer<mantissa_bits, 0> mantissa;
		bool sign = false;
	};



	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>> operator*(T value, qpl::floating_point<exponent_bits, mantissa_bits> other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>>, qpl::floating_point<exponent_bits, mantissa_bits>>()) {
			return qpl::floating_point<exponent_bits, mantissa_bits>(value).operator *(other);
		}
		else {
			return value * qpl::type_cast<T>(other);
		}
	}
	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
		constexpr T& operator*=(T& value, const qpl::floating_point<exponent_bits, mantissa_bits>& other) {
		return value = (value * other);
	}

	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>> operator/(T value, qpl::floating_point<exponent_bits, mantissa_bits> other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>>, qpl::floating_point<exponent_bits, mantissa_bits>>()) {
			return qpl::floating_point<exponent_bits, mantissa_bits>(value).operator /(other);
		}
		else {
			return value / qpl::type_cast<T>(other);
		}
	}
	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
		constexpr T& operator/=(T& value, const qpl::floating_point<exponent_bits, mantissa_bits>& other) {
		return value = (value / other);
	}

	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>> operator+(T value, qpl::floating_point<exponent_bits, mantissa_bits> other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>>, qpl::floating_point<exponent_bits, mantissa_bits>>()) {
			return qpl::floating_point<exponent_bits, mantissa_bits>(value).operator +(other);
		}
		else {
			return value + qpl::type_cast<T>(other);
		}
	}
	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
	constexpr T& operator+=(T& value, const qpl::floating_point<exponent_bits, mantissa_bits>& other) {
		return value = (value + other);
	}

	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>> operator-(T value, qpl::floating_point<exponent_bits, mantissa_bits> other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>>, qpl::floating_point<exponent_bits, mantissa_bits>>()) {
			return qpl::floating_point<exponent_bits, mantissa_bits>(value).operator -(other);
		}
		else {
			return value - qpl::type_cast<T>(other);
		}
	}
	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
		constexpr T& operator-=(T& value, const qpl::floating_point<exponent_bits, mantissa_bits>& other) {
		return value = (value - other);
	}

	template<typename T>
	concept has_pow_operator = requires (const T t) {
		t ^ 2;
	};

	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
	constexpr qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>> operator^(T value, qpl::floating_point<exponent_bits, mantissa_bits> other) {
		if constexpr (qpl::is_same<qpl::superior_arithmetic_type<T, qpl::floating_point<exponent_bits, mantissa_bits>>, qpl::floating_point<exponent_bits, mantissa_bits>>()) {
			return qpl::floating_point<exponent_bits, mantissa_bits>(value).operator ^(other);
		}
		else {
			if constexpr (has_pow_operator<T>()) {
				return value ^ qpl::type_cast<T>(other);
			}
			else {
				return std::pow(value, qpl::type_cast<T>(other));
			}
		}
	}
	template<qpl::size exponent_bits, qpl::size mantissa_bits, typename T> requires(!qpl::is_qpl_floating_point<T>())
		constexpr T& operator^=(T& value, const qpl::floating_point<exponent_bits, mantissa_bits>& other) {
		return value = (value ^ other);
	}
#endif

	template<typename T>
	constexpr inline qpl::u128 u128_cast(T&& data) {
		return static_cast<qpl::u128>(data);
	}
	template<typename T>
	constexpr inline qpl::i128 i128_cast(T&& data) {
		return static_cast<qpl::i128>(data);
	}

	template<typename T>
	constexpr inline qpl::u256 u256_cast(T&& data) {
		return static_cast<qpl::u256>(data);
	}
	template<typename T>
	constexpr inline qpl::i256 i256_cast(T&& data) {
		return static_cast<qpl::i256>(data);
	}

	template<typename T>
	constexpr inline qpl::u512 u512_cast(T&& data) {
		return static_cast<qpl::u512>(data);
	}
	template<typename T>
	constexpr inline qpl::i512 i512_cast(T&& data) {
		return static_cast<qpl::i512>(data);
	}


	template<typename T>
	inline qpl::u u_cast(T&& data) {
		return static_cast<qpl::u>(data);
	}
	template<typename T>
	inline qpl::i i_cast(T&& data) {
		return static_cast<qpl::i>(data);
	}
	template<typename T>
	inline qpl::uh uh_cast(T&& data) {
		return static_cast<qpl::uh>(data);
	}
	template<typename T>
	inline qpl::ih ih_cast(T&& data) {
		return static_cast<qpl::ih>(data);
	}
	template<typename T>
	inline qpl::ub ub_cast(T&& data) {
		return static_cast<qpl::ub>(data);
	}
	template<typename T>
	inline qpl::ib ib_cast(T&& data) {
		return static_cast<qpl::ib>(data);
	}
	template<qpl::u32 base, typename T>
	inline qpl::ubase<base> ubase_cast(T&& data) {
		return static_cast<qpl::ubase<base>>(data);
	}
	template<qpl::u32 base, typename T>
	inline qpl::ibase<base> ibase_cast(T&& data) {
		return static_cast<qpl::ibase<base>>(data);
	}
}



#endif