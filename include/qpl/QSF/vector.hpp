#ifndef QPLSF_VECTOR_HPP
#define QPLSF_VECTOR_HPP
#pragma once

#ifndef QPL_NO_SFML

#include <SFML/Graphics.hpp>
#include <qpl/vardef.hpp>
#include <qpl/string.hpp>
#include <initializer_list>
#include <iostream>
#include <cmath>
#include <array>

namespace qsf {
	template<typename T>
	struct vector2 {
		T x;
		T y;

		constexpr vector2() {
			this->x = this->y = T{};
		}
		template<typename U>
		constexpr vector2(const std::initializer_list<U>& list) : x(), y() {
			*this = list;
		}
		template<typename U>
		constexpr vector2(const vector2<U>& other) : x(), y() {
			*this = other;
		}
		template<typename U>
		constexpr vector2(const sf::Vector2<U>& other) : x(), y() {
			*this = other;
		}
		template<typename U, typename V>
		constexpr vector2(U u, V v) : x(), y() {
			this->x = static_cast<T>(u);
			this->y = static_cast<T>(v);
		}
		template<typename U>
		constexpr operator sf::Vector2<U>() const {
			return sf::Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y));
		}

		constexpr vector2<T> xx() const {
			return vector2<T>(this->x, this->x);
		}
		constexpr vector2<T> yy() const {
			return vector2<T>(this->y, this->y);
		}
		constexpr vector2<T> just_x() const {
			return vector2<T>(this->x, T{});
		}
		constexpr vector2<T> just_y() const {
			return vector2<T>(T{}, this->y);
		}

		constexpr T normal() const {
			return static_cast<T>(std::sqrt(this->x * this->x + this->y * this->y));
		}
		constexpr bool empty() const {
			return this->x == T{ 0 } && this->y == T{ 0 };
		}

		constexpr vector2 operator-() const {
			auto copy = *this;
			copy.x *= -1;
			copy.y *= -1;
			return copy;
		}
		constexpr vector2 operator+() const {
			auto copy = *this;
			copy.x = +copy.x;
			copy.y = +copy.y;
			return copy;
		}

		template<typename U>
		constexpr vector2<U> t() const {
			return qsf::vector2<U>(*this);
		}

		template<typename U, typename V>
		constexpr void move(U x, V y) {
			this->x = static_cast<T>(this->x + x);
			this->y = static_cast<T>(this->y + y);
		}
		template<typename U>
		constexpr void move(qsf::vector2<U> position) {
			this->x = static_cast<T>(this->x + position.x);
			this->y = static_cast<T>(this->y + position.y);
		}

		template<typename U, typename V>
		constexpr qsf::vector2<T> moved(U x, V y) const {
			auto copy = *this;
			copy.move(x, y);
			return copy;
		}
		template<typename U>
		constexpr qsf::vector2<T> moved(qsf::vector2<U> position) const {
			auto copy = *this;
			copy.move(position);
			return copy;
		}


		constexpr bool operator<(const vector2<T>& pos) const {
			if (this->y < pos.y) return true;
			else if (this->y > pos.y) return false;

			return this->x < pos.x;
		}
		template<typename U>
		constexpr bool operator==(const vector2<U>& other) const {
			return this->x == static_cast<T>(other.x) && this->y == static_cast<T>(other.y);
		}
		template<typename U>
		constexpr bool operator!=(const vector2<U>& other) const {
			return this->x != static_cast<T>(other.x) || this->y != static_cast<T>(other.y);
		}
		template<typename U>
		constexpr vector2<T>& operator=(const std::initializer_list<U>& list) {
			if (list.size() == 1) {
				this->x = static_cast<T>(*list.begin());
				this->y = T{ 0 };
			}
			else if (list.size() == 2) {
				this->x = static_cast<T>(*list.begin());
				this->y = static_cast<T>(*(list.begin() + 1));
			}
			return *this;
		}
		template<typename U>
		constexpr vector2<T>& operator=(const vector2<U>& other) {
			this->x = static_cast<T>(other.x);
			this->y = static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		constexpr vector2<T>& operator=(const sf::Vector2<U>& vec) {
			this->x = static_cast<T>(vec.x);
			this->y = static_cast<T>(vec.y);
			return *this;
		}
		constexpr bool operator==(const vector2<T>& other) const {
			return this->x == other.x && this->y == other.y;
		}

		template<typename U>
		constexpr vector2<T>& operator+=(const vector2<U>& other) {
			this->x += static_cast<T>(other.x);
			this->y += static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		constexpr auto operator+(const vector2<U>& other) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy += other;
			return copy;
		}
		template<typename U>
		constexpr vector2<T>& operator+=(U u) {
			this->x += static_cast<T>(u);
			this->y += static_cast<T>(u);
			return *this;
		}
		template<typename U>
		constexpr auto operator+(U u) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy += u;
			return copy;
		}

		template<typename U>
		constexpr vector2<T>& operator-=(const vector2<U>& other) {
			this->x -= static_cast<T>(other.x);
			this->y -= static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		constexpr auto operator-(const vector2<U>& other) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy -= other;
			return copy;
		}
		template<typename U>
		constexpr vector2<T>& operator-=(U u) {
			this->x -= static_cast<T>(u);
			this->y -= static_cast<T>(u);
			return *this;
		}
		template<typename U>
		constexpr auto operator-(U u) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy -= u;
			return copy;
		}

		template<typename U>
		constexpr vector2<T>& operator/=(const vector2<U>& other) {
			this->x /= static_cast<T>(other.x);
			this->y /= static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		constexpr auto operator/(const vector2<U>& other) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy /= other;
			return copy;
		}
		template<typename U>
		constexpr vector2<T>& operator/=(U u) {
			this->x /= static_cast<T>(u);
			this->y /= static_cast<T>(u);
			return *this;
		}
		template<typename U>
		constexpr auto operator/(U u) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy /= u;
			return copy;
		}

		template<typename U>
		constexpr vector2<T>& operator%=(const vector2<U>& other) {
			this->x %= static_cast<T>(other.x);
			this->y %= static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		constexpr auto operator%(const vector2<U>& other) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy %= other;
			return copy;
		}
		template<typename U>
		constexpr vector2<T>& operator%=(U u) {
			this->x %= static_cast<T>(u);
			this->y %= static_cast<T>(u);
			return *this;
		}
		template<typename U>
		constexpr auto operator%(U u) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy %= u;
			return copy;
		}

		template<typename U>
		constexpr vector2<T>& operator*=(const vector2<U>& other) {
			this->x *= static_cast<T>(other.x);
			this->y *= static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		constexpr auto operator*(const vector2<U>& other) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy *= other;
			return copy;
		}
		template<typename U>
		constexpr vector2<T>& operator*=(U u) {
			this->x *= static_cast<T>(u);
			this->y *= static_cast<T>(u);
			return *this;
		}
		template<typename U>
		constexpr auto operator*(U u) const {
			vector2<qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy *= u;
			return copy;
		}
		constexpr std::string string() const {
			return qpl::to_string('(', this->x, ", ", this->y, ')');
		}
		template<typename T>
		friend std::ostream& operator<<(std::ostream& os, const qsf::vector2<T>& vec);
	};
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const qsf::vector2<T>& vec) {
		return os << vec.string();
	}

	using vector2f = qsf::vector2<qpl::f32>;
	using vector2d = qsf::vector2<qpl::f64>;
	using vector2i = qsf::vector2<qpl::i32>;
	using vector2u = qsf::vector2<qpl::u32>;


	namespace impl {
		template<typename T>

		struct vector_impl_1 {
			union {
				struct {
					T x;
				};
				std::array<T, 1> data;
			};
		};
		template<typename T>
		struct vector_impl_2 {
			union {
				struct {
					T x;
					T y;
				};
				std::array<T, 2> data;
			};
		};
		template<typename T>
		struct vector_impl_3 {
			union {
				struct {
					T x;
					T y;
					T z;
				};
				std::array<T, 3> data;
			};
		};
		template<typename T>
		struct vector_impl_4 {
			union {
				struct {
					T x;
					T y;
					T z;
					T w;
				};
				std::array<T, 4> data;
			};
		};
		template<typename T>
		struct vector_impl_5 {
			union {
				struct {
					T a;
					T b;
					T c;
					T d;
					T e;
				};
				std::array<T, 5> data;
			};
		};
		template<typename T>
		struct vector_impl_6 {
			union {
				struct {
					T a;
					T b;
					T c;
					T d;
					T e;
					T f;
				};
				std::array<T, 6> data;
			};
		};
	}

	template<qpl::size N, typename T>
	struct vectorN : qpl::conditional<
		qpl::if_true<N == 1>, impl::vector_impl_1<T>,
		qpl::if_true<N == 2>, impl::vector_impl_2<T>,
		qpl::if_true<N == 3>, impl::vector_impl_3<T>,
		qpl::if_true<N == 4>, impl::vector_impl_4<T>,
		qpl::if_true<N == 5>, impl::vector_impl_5<T>,
		qpl::if_true<N == 6>, impl::vector_impl_6<T>,
		qpl::default_error> {

		using impl_type = qpl::conditional<
			qpl::if_true<N == 1>, impl::vector_impl_1<T>,
			qpl::if_true<N == 2>, impl::vector_impl_2<T>,
			qpl::if_true<N == 3>, impl::vector_impl_3<T>,
			qpl::if_true<N == 4>, impl::vector_impl_4<T>,
			qpl::if_true<N == 5>, impl::vector_impl_5<T>,
			qpl::if_true<N == 6>, impl::vector_impl_6<T>,
			qpl::default_error>;

		constexpr std::string string() const {
			std::ostringstream stream;
			stream << '(';
			bool first = true;
			for (auto& i : this->data) {
				if (!first) {
					stream << ", ";
				}
				first = false;
				stream << i;
			}
			stream << ')';
			return stream.str();
		}

		constexpr vectorN() {
			this->clear();
		}
		constexpr vectorN(const vectorN& other) {
			*this = other;
		}
		template<typename U, typename... Args>
		constexpr vectorN(U first, Args&&... list) {
			*this = qpl::tuple_to_array<T>(std::make_tuple(first, list...));
		}
		template<qpl::size N, typename U>
		constexpr vectorN& operator=(const vectorN& other) {
			this->data = other.data;
			return *this;
		}

		template<qpl::size N, typename U>
		constexpr vectorN& operator=(const std::array<U, N>& array) {
			if (array.empty()) {
				this->clear();
				return *this;
			}
			for (qpl::u32 i = 0u; i < qpl::min(array.size(), this->data.size()); ++i) {
				this->data[i] = static_cast<T>(array[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr void set(const std::initializer_list<U>& list) {
			if (list.size() == 0) {
				this->clear();
				return;
			}
			for (qpl::u32 i = 0u; i < qpl::min(list.size(), this->data.size()); ++i) {
				this->data[i] = static_cast<T>(*(list.begin() + i));
			}
		}


		constexpr void clear() {
			for (auto& i : this->data) {
				i = T{};
			}
		}

		constexpr bool operator<(const vectorN& other) const {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				if (this->data[i] < static_cast<T>(other.data[i])) {
					return true;
				}
				if (this->data[i] > static_cast<T>(other.data[i])) {
					return false;
				}
			}
			return false;
		}
		template<typename U>
		constexpr bool operator==(const vectorN<N, U>& other) const {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				if (this->data[i] != static_cast<T>(other.data[i])) {
					return false;
				}
			}
			return true;
		}
		template<typename U>
		constexpr bool operator!=(const vectorN<N, U>& other) const {
			return !(*this == other);
		}

		template<typename U>
		constexpr vectorN<N, T>& operator+=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this += add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator+=(const vectorN<N, U>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] += static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator+(const vectorN<N, U>& other) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy += other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator+=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] += static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator+(U u) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy += u;
			return copy;
		}


		template<typename U>
		constexpr vectorN<N, T>& operator-=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this -= add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator-=(const vectorN<N, U>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] -= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator-(const vectorN<N, U>& other) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy -= other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator-=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] -= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator-(U u) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy -= u;
			return copy;
		}


		template<typename U>
		constexpr vectorN<N, T>& operator*=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this *= add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator*=(const vectorN<N, U>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] *= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator*(const vectorN<N, U>& other) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy *= other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator*=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] *= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator*(U u) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy *= u;
			return copy;
		}


		template<typename U>
		constexpr vectorN<N, T>& operator/=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this /= add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator/=(const vectorN<N, U>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] /= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator/(const vectorN<N, U>& other) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy /= other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator/=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] /= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator/(U u) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy /= u;
			return copy;
		}


		template<typename U>
		constexpr vectorN<N, T>& operator%=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this %= add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator%=(const vectorN<N, U>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] %= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator%(const vectorN<N, U>& other) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy %= other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator%=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] %= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator%(U u) const {
			vectorN<N, qpl::superior_arithmetic_type<T, U>> copy = *this;
			copy %= u;
			return copy;
		}


		constexpr vectorN operator-() const {
			vectorN copy;
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				copy[i] = -this->data[i];
			}
			return copy;
		}
		constexpr vectorN operator+() const {
			vectorN copy;
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				copy[i] = +this->impl_type::data[i];
			}
			return copy;
		}


		template<qpl::size N, typename T>
		friend std::ostream& operator<<(std::ostream& os, const qsf::vectorN<N, T>& vec);
	};

	template<qpl::size N, typename T>
	std::ostream& operator<<(std::ostream& os, const qsf::vectorN<N, T>& vec) {
		return os << vec.string();
	}

	using vector3f = qsf::vectorN<3, qpl::f32>;
	using vector3d = qsf::vectorN<3, qpl::f64>;
	using vector3i = qsf::vectorN<3, qpl::i32>;
	using vector3u = qsf::vectorN<3, qpl::u32>;

	using vector4f = qsf::vectorN<4, qpl::f32>;
	using vector4d = qsf::vectorN<4, qpl::f64>;
	using vector4i = qsf::vectorN<4, qpl::i32>;
	using vector4u = qsf::vectorN<4, qpl::u32>;

	using vector5f = qsf::vectorN<5, qpl::f32>;
	using vector5d = qsf::vectorN<5, qpl::f64>;
	using vector5i = qsf::vectorN<5, qpl::i32>;
	using vector5u = qsf::vectorN<5, qpl::u32>;

	using vector6f = qsf::vectorN<6, qpl::f32>;
	using vector6d = qsf::vectorN<6, qpl::f64>;
	using vector6i = qsf::vectorN<6, qpl::i32>;
	using vector6u = qsf::vectorN<6, qpl::u32>;

}


#endif
#endif