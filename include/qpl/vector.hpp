#ifndef QPL_VECTOR_HPP
#define QPL_VECTOR_HPP
#pragma once

#if !defined(QPL_NO_SFML) || defined(QPL_USE_ALL)
#include <SFML/Graphics.hpp>
#endif

#include <qpl/vardef.hpp>
#include <qpl/string.hpp>
#include <initializer_list>
#include <iostream>
#include <cmath>
#include <array>

namespace qpl {
	/*
	
	
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
		template<typename U, typename V> requires (qpl::is_arithmetic<U>() && qpl::is_arithmetic<V>())
		constexpr vector2(U u, V v) : x(), y() {
			this->x = static_cast<T>(u);
			this->y = static_cast<T>(v);
		}
#if !defined(QPL_NO_SFML) || defined(QPL_USE_ALL)
		template<typename U>
		constexpr vector2(const sf::Vector2<U>& other) : x(), y() {
			*this = other;
		}
		template<typename U>
		constexpr operator sf::Vector2<U>() const {
			return sf::Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y));
		}
		template<typename U>
		constexpr vector2<T>& operator=(const sf::Vector2<U>& vec) {
			this->x = static_cast<T>(vec.x);
			this->y = static_cast<T>(vec.y);
			return *this;
		}
#endif

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
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr vector2<T> with_x(U x) const {
			return vector2<T>(x, this->y);
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr vector2<T> with_y(U y) const {
			return vector2<T>(this->x, y);
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
			return qpl::vector2<U>(*this);
		}

		template<typename U, typename V> requires (qpl::is_arithmetic<U>() && qpl::is_arithmetic<V>())
		constexpr void move(U x, V y) {
			this->x = static_cast<T>(this->x + x);
			this->y = static_cast<T>(this->y + y);
		}
		template<typename U>
		constexpr void move(qpl::vector2<U> position) {
			this->x = static_cast<T>(this->x + position.x);
			this->y = static_cast<T>(this->y + position.y);
		}

		template<typename U, typename V> requires (qpl::is_arithmetic<U>() && qpl::is_arithmetic<V>())
		constexpr qpl::vector2<T> moved(U x, V y) const {
			auto copy = *this;
			copy.move(x, y);
			return copy;
		}
		template<typename U>
		constexpr qpl::vector2<T> moved(qpl::vector2<U> position) const {
			auto copy = *this;
			copy.move(position);
			return copy;
		}


		constexpr bool operator<(const vector2<T>& pos) const {
			return this->y < pos.y && this->x < pos.x;
		}
		constexpr bool operator<=(const vector2<T>& pos) const {
			return this->y <= pos.y && this->x <= pos.x;
		}
		constexpr bool operator>(const vector2<T>& pos) const {
			return this->y > pos.y && this->x > pos.x;
		}
		constexpr bool operator>=(const vector2<T>& pos) const {
			return this->y >= pos.y && this->x >= pos.x;
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
		constexpr bool operator==(const vector2<T>& other) const {
			return this->x == other.x && this->y == other.y;
		}

		template<typename U>
		constexpr vector2<T>& operator+=(const vector2<U>& other) {
			this->x += static_cast<T>(other.x);
			this->y += static_cast<T>(other.y);
			return *this;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr vector2<T>& operator+=(const std::initializer_list<U>& u) const {
			return *this += vector2<T>(u);
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
		template<typename U> requires (qpl::is_arithmetic<U>())
			constexpr vector2<T>& operator-=(const std::initializer_list<U>& u) const {
			return *this -= vector2<T>(u);
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
		template<typename U> requires (qpl::is_arithmetic<U>())
			constexpr vector2<T>& operator/=(const std::initializer_list<U>& u) const {
			return *this /= vector2<T>(u);
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
		template<typename U> requires (qpl::is_arithmetic<U>())
			constexpr vector2<T>& operator%=(const std::initializer_list<U>& u) const {
			return *this %= vector2<T>(u);
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
		template<typename U> requires (qpl::is_arithmetic<U>())
			constexpr vector2<T>& operator*=(const std::initializer_list<U>& u) const {
			return *this *= vector2<T>(u);
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
		friend std::ostream& operator<<(std::ostream& os, const qpl::vector2<T>& vec);
	};


	template<typename T>
	std::ostream& operator<<(std::ostream& os, const qpl::vector2<T>& vec) {
		return os << vec.string();
	}


	*/

	namespace impl {
		template<typename T>

		struct vector_impl_1 {
			union {
				struct {
					T x;
				};
				std::array<T, 1> data;
			};

			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 1> with_x(U x) const {
				return std::array<T, 1>(static_cast<T>(x));
			}
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
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 2> with_x(U x) const {
				return std::array<T, 2>{ static_cast<T>(x), this->y };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 2> with_y(U y) const {
				return std::array<T, 2>{ this->x, static_cast<T>(y) };
			}

			constexpr std::array<T, 2> just_x() const {
				return std::array<T, 2>{ this->x, T{ 0 } };
			}
			constexpr std::array<T, 2> just_y() const{
				return std::array<T, 2>{ T{ 0 },  this->y };
			}
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
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 3> with_x(U x) const {
				return std::array<T, 3>{ static_cast<T>(x), this->y, this->z };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 3> with_y(U y) const {
				return std::array<T, 3>{ this->x, static_cast<T>(y), this->z  };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 3> with_z(U z) const {
				return std::array<T, 3>{ this->x, this->y, static_cast<T>(z)};
			}

			constexpr std::array<T, 3> just_x() const {
				return std::array<T, 3>{ this->x, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 3> just_y() const {
				return std::array<T, 3>{ T{ 0 }, this->y, T{ 0 } };
			}
			constexpr std::array<T, 3> just_z() const {
				return std::array<T, 3>{ T{ 0 }, T{ 0 }, this->z };
			}
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

			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 4> with_x(U x) const {
				return std::array<T, 4>{ static_cast<T>(x), this->y, this->z, this->w };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 4> with_y(U y) const {
				return std::array<T, 4>{ this->x, static_cast<T>(y), this->z, this->w };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 4> with_z(U z) const {
				return std::array<T, 4>{ this->x, this->y, static_cast<T>(z), this->w };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 4> with_w(U w) const {
				return std::array<T, 4>{ this->x, this->y, this->z, static_cast<T>(w) };
			}

			constexpr std::array<T, 4> just_x() const {
				return std::array<T, 4>{ this->x, T{ 0 }, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 4> just_y() const {
				return std::array<T, 4>{ T{ 0 }, this->y, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 4> just_z() const {
				return std::array<T, 4>{ T{ 0 }, T{ 0 }, this->z, T{ 0 } };
			}
			constexpr std::array<T, 4> just_w() const {
				return std::array<T, 4>{ T{ 0 }, T{ 0 }, T{ 0 }, this->w };
			}
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

			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 5> with_a(U a) const {
				return std::array<T, 5>{ static_cast<T>(a), this->b, this->c, this->d, this->e };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 5> with_b(U b) const {
				return std::array<T, 5>{ this->a, static_cast<T>(b), this->c, this->d, this->e };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 5> with_c(U c) const {
				return std::array<T, 5>{ this->a, this->b, static_cast<T>(c), this->d, this->e };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 5> with_d(U d) const {
				return std::array<T, 5>{ this->a, this->b, this->c, static_cast<T>(d), this->e };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 5> with_e(U e) const {
				return std::array<T, 5>{ this->a, this->b, this->c, this->d, static_cast<T>(e) };
			}

			constexpr std::array<T, 5> just_a() const {
				return std::array<T, 5>{ this->a, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 5> just_b() const {
				return std::array<T, 5>{ T{ 0 }, this->b, T{ 0 }, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 5> just_c() const {
				return std::array<T, 5>{ T{ 0 }, T{ 0 }, this->c, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 5> just_d() const {
				return std::array<T, 5>{ T{ 0 }, T{ 0 }, T{ 0 }, this->d, T{ 0 }};
			}
			constexpr std::array<T, 5> just_e() const {
				return std::array<T, 5>{ T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 }, this->e };
			}
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

			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 6> with_a(U a) const {
				return std::array<T, 6>{ static_cast<T>(a), this->b, this->c, this->d, this->e, this->f };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 6> with_b(U b) const {
				return std::array<T, 6>{ this->a, static_cast<T>(b), this->c, this->d, this->e, this->f };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 6> with_c(U c) const {
				return std::array<T, 6>{ this->a, this->b, static_cast<T>(c), this->d, this->e, this->f };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 6> with_d(U d) const {
				return std::array<T, 6>{ this->a, this->b, this->c, static_cast<T>(d), this->e, this->f };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 6> with_e(U e) const {
				return std::array<T, 6>{ this->a, this->b, this->c, this->d, static_cast<T>(e), this->f };
			}
			template<typename U> requires (qpl::is_arithmetic<U>())
				constexpr std::array<T, 6> with_f(U f) const {
				return std::array<T, 6>{ this->a, this->b, this->c, this->d, this->e, static_cast<T>(f) };
			}

			constexpr std::array<T, 6> just_a() const {
				return std::array<T, 6>{ this->a, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 6> just_b() const {
				return std::array<T, 6>{ T{ 0 }, this->b, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 6> just_c() const {
				return std::array<T, 6>{ T{ 0 }, T{ 0 }, this->c, T{ 0 }, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 6> just_d() const {
				return std::array<T, 6>{ T{ 0 }, T{ 0 }, T{ 0 }, this->d, T{ 0 }, T{ 0 } };
			}
			constexpr std::array<T, 6> just_e() const {
				return std::array<T, 6>{ T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 }, this->e, T{ 0 } };
			}
			constexpr std::array<T, 6> just_f() const {
				return std::array<T, 6>{ T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 }, this->f };
			}
		};
		template<typename T, qpl::size N>
		struct vector_impl_N {
			std::array<T, N> data;
		};
	}

	template<typename T, qpl::size N>
	struct vectorN : qpl::conditional<
		qpl::if_true<N == 0>, qpl::error_type,
		qpl::if_true<N == 1>, impl::vector_impl_1<T>,
		qpl::if_true<N == 2>, impl::vector_impl_2<T>,
		qpl::if_true<N == 3>, impl::vector_impl_3<T>,
		qpl::if_true<N == 4>, impl::vector_impl_4<T>,
		qpl::if_true<N == 5>, impl::vector_impl_5<T>,
		qpl::if_true<N == 6>, impl::vector_impl_6<T>,
		qpl::default_type,    impl::vector_impl_N<T, N>> {

		using impl_type = qpl::conditional<
			qpl::if_true<N == 0>, qpl::error_type,
			qpl::if_true<N == 1>, impl::vector_impl_1<T>,
			qpl::if_true<N == 2>, impl::vector_impl_2<T>,
			qpl::if_true<N == 3>, impl::vector_impl_3<T>,
			qpl::if_true<N == 4>, impl::vector_impl_4<T>,
			qpl::if_true<N == 5>, impl::vector_impl_5<T>,
			qpl::if_true<N == 6>, impl::vector_impl_6<T>,
			qpl::default_type,    impl::vector_impl_N<T, N>>;

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
		template<typename U>
		constexpr vectorN(const vectorN<U, N>& other) {
			*this = other;
		}
		template<typename U, typename... Args> 
		constexpr vectorN(U first, Args&&... list) {
			*this = qpl::tuple_to_array<T>(std::make_tuple(first, list...));
		}

#if !defined(QPL_NO_SFML) || defined(QPL_USE_ALL)
		template<typename U>
		constexpr vectorN(const sf::Vector2<U>& other) /*: x(), y()*/ {
			*this = other;
		}
		template<typename U>
		constexpr operator sf::Vector2<U>() const {
			if constexpr (N == 1) {
				return sf::Vector2<U>(static_cast<U>(this->x), U{ 0 });
			}
			else {
				return sf::Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y));
			}
		}
		template<typename U>
		constexpr vectorN& operator=(const sf::Vector2<U>& vec) {
			this->x = static_cast<T>(vec.x);
			this->y = static_cast<T>(vec.y);
			if constexpr (N == 1) {
				this->x = static_cast<T>(vec.x);
			}
			else {
				this->x = static_cast<T>(vec.x);
				this->y = static_cast<T>(vec.y);
			}
			return *this;
		}
#endif


		template<qpl::size N, typename U>
		constexpr vectorN& operator=(const vectorN<U, N>& other) {
			for (qpl::u32 i = 0u; i < qpl::min(this->data.size(), other.data.size()); ++i) {
				this->data[i] = other.data[i];
			}
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
		template<typename U>
		void move(const vectorN<U, N>& delta) {
			*this += delta;
		}
		template<typename U, typename... Args>
		void move(U first, Args&&... list) {
			*this += vectorN(first, list...);
		}
		template<typename U>
		auto moved(const vectorN<U, N>& delta) const {
			return *this + delta;
		}
		template<typename U, typename... Args>
		auto moved(U first, Args&&... list) {
			return *this + vectorN(first, list...);
		}

		constexpr static vectorN values(T value) {
			vectorN result;
			for (auto& i : result.data) {
				i = value;
			}
			return result;
		}
		constexpr static vectorN zero() {
			return vectorN::values(0);
		}
		constexpr static vectorN one() {
			return vectorN::values(1);
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
		constexpr bool operator<=(const vectorN& other) const {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				if (this->data[i] < static_cast<T>(other.data[i])) {
					return true;
				}
				if (this->data[i] > static_cast<T>(other.data[i])) {
					return false;
				}
			}
			return true;
		}
		constexpr bool operator>(const vectorN& other) const {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				if (this->data[i] > static_cast<T>(other.data[i])) {
					return true;
				}
				if (this->data[i] < static_cast<T>(other.data[i])) {
					return false;
				}
			}
			return false;
		}
		constexpr bool operator>=(const vectorN& other) const {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				if (this->data[i] > static_cast<T>(other.data[i])) {
					return true;
				}
				if (this->data[i] < static_cast<T>(other.data[i])) {
					return false;
				}
			}
			return true;
		}
		template<typename U>
		constexpr bool operator==(const vectorN<U, N>& other) const {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				if (this->data[i] != static_cast<T>(other.data[i])) {
					return false;
				}
			}
			return true;
		}
		template<typename U>
		constexpr bool operator!=(const vectorN<U, N>& other) const {
			return !(*this == other);
		}

		template<typename U>
		constexpr vectorN<T, N>& operator+=(const std::initializer_list<U>& list) {
			vectorN<T, N> add;
			add.set(list);
			return (*this += add);
		}
		template<typename U>
		constexpr vectorN<T, N>& operator+=(const vectorN<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] += static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr vectorN<T, N>& operator+=(const std::array<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] += static_cast<T>(other[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator+(const vectorN<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy += other;
			return copy;
		}
		template<typename U>
		constexpr auto operator+(const std::array<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy += other;
			return copy;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr vectorN<T, N>& operator+=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] += static_cast<T>(u);
			}
			return *this;
		} 
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr auto operator+(U u) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy += u;
			return copy;
		}


		template<typename U>
		constexpr vectorN<T, N>& operator-=(const std::initializer_list<U>& list) {
			vectorN<T, N> add;
			add.set(list);
			return (*this -= add);
		}
		template<typename U>
		constexpr vectorN<T, N>& operator-=(const vectorN<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] -= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr vectorN<T, N>& operator-=(const std::array<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] -= static_cast<T>(other[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator-(const vectorN<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy -= other;
			return copy;
		}
		template<typename U>
		constexpr auto operator-(const std::array<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy -= other;
			return copy;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr vectorN<T, N>& operator-=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] -= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator-(U u) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy -= u;
			return copy;
		}


		template<typename U>
		constexpr vectorN<T, N>& operator*=(const std::initializer_list<U>& list) {
			vectorN<T, N> add;
			add.set(list);
			return (*this *= add);
		}
		template<typename U>
		constexpr vectorN<T, N>& operator*=(const vectorN<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] *= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr vectorN<T, N>& operator*=(const std::array<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] *= static_cast<T>(other[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator*(const vectorN<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy *= other;
			return copy;
		}
		template<typename U>
		constexpr auto operator*(const std::array<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy *= other;
			return copy;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr vectorN<T, N>& operator*=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] *= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr auto operator*(U u) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy *= u;
			return copy;
		}


		template<typename U>
		constexpr vectorN<T, N>& operator/=(const std::initializer_list<U>& list) {
			vectorN<T, N> add;
			add.set(list);
			return (*this /= add);
		}
		template<typename U>
		constexpr vectorN<T, N>& operator/=(const vectorN<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] /= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr vectorN<T, N>& operator/=(const std::array<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] /= static_cast<T>(other[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator/(const vectorN<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy /= other;
			return copy;
		}
		template<typename U>
		constexpr auto operator/(const std::array<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy /= other;
			return copy;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr vectorN<T, N>& operator/=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] /= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr auto operator/(U u) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy /= u;
			return copy;
		}


		template<typename U>
		constexpr vectorN<T, N>& operator%=(const std::initializer_list<U>& list) {
			vectorN<T, N> add;
			add.set(list);
			return (*this %= add);
		}
		template<typename U>
		constexpr vectorN<T, N>& operator%=(const vectorN<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] %= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr vectorN<T, N>& operator%=(const std::array<U, N>& other) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] %= static_cast<T>(other[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator%(const vectorN<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy %= other;
			return copy;
		}
		template<typename U>
		constexpr auto operator%(const std::array<U, N>& other) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy %= other;
			return copy;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr vectorN<T, N>& operator%=(U u) {
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] %= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr auto operator%(U u) const {
			vectorN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy %= u;
			return copy;
		}


		constexpr vectorN operator-() const {
			vectorN copy;
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				copy.data[i] = -this->data[i];
			}
			return copy;
		}
		constexpr vectorN operator+() const {
			vectorN copy;
			for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
				copy.data[i] = +this->impl_type::data[i];
			}
			return copy;
		}


		template<qpl::size N, typename T>
		friend std::ostream& operator<<(std::ostream& os, const qpl::vectorN<T, N>& vec);
	};

	template<qpl::size N, typename T>
	std::ostream& operator<<(std::ostream& os, const qpl::vectorN<T, N>& vec) {
		return os << vec.string();
	}

	template<typename T>
	using vector2 = qpl::vectorN<T, 2>;
	template<typename T>
	using vector3 = qpl::vectorN<T, 3>;
	template<typename T>
	using vector4 = qpl::vectorN<T, 4>;
	template<typename T>
	using vector5 = qpl::vectorN<T, 5>;
	template<typename T>
	using vector6 = qpl::vectorN<T, 6>;

	using vector2f = qpl::vector2<qpl::f32>;
	using vector2d = qpl::vector2<qpl::f64>;
	using vector2i = qpl::vector2<qpl::i32>;
	using vector2u = qpl::vector2<qpl::u32>;

	using vector3f = qpl::vector3<qpl::f32>;
	using vector3d = qpl::vector3<qpl::f64>;
	using vector3i = qpl::vector3<qpl::i32>;
	using vector3u = qpl::vector3<qpl::u32>;

	using vector4f = qpl::vector4<qpl::f32>;
	using vector4d = qpl::vector4<qpl::f64>;
	using vector4i = qpl::vector4<qpl::i32>;
	using vector4u = qpl::vector4<qpl::u32>;

	using vector5f = qpl::vector5<qpl::f32>;
	using vector5d = qpl::vector5<qpl::f64>;
	using vector5i = qpl::vector5<qpl::i32>;
	using vector5u = qpl::vector5<qpl::u32>;

	using vector6f = qpl::vector6<qpl::f32>;
	using vector6d = qpl::vector6<qpl::f64>;
	using vector6i = qpl::vector6<qpl::i32>;
	using vector6u = qpl::vector6<qpl::u32>;


	template <typename T, typename ...Args> requires (qpl::is_arithmetic<T>())
	constexpr auto vec(T first, Args... rest) {
		return qpl::vectorN<T, sizeof...(Args) + 1>(first, rest...);
	}

	template<typename T>
	struct straight_line_t {
		using maths_float_type = qpl::conditional<qpl::if_true<qpl::is_same<T, qpl::f32>()>, qpl::f32, qpl::default_type, qpl::f64>;

		constexpr straight_line_t() : a(), b() {

		}
		constexpr straight_line_t(qpl::vector2<T> a, qpl::vector2<T> b) : a(a), b(b) {

		}
		std::string string() const {
			return qpl::to_string("[", this->a.string(), ", ", this->b.string(), "]");
		}
		maths_float_type length() const {
			auto diff = this->b - this->a;
			return static_cast<maths_float_type>(std::sqrt(diff.x * diff.x + diff.y * diff.y));
		}
		qpl::vector2<maths_float_type> normal() const {
			return qpl::vector2<maths_float_type>{ this->a.y - this->b.y, this->b.x - this->a.x } / this->length();
		}
		maths_float_type angle() const {
			auto atan = std::atan2(this->a.y - this->b.y, this->b.x - this->a.x);
			if (atan < 0) atan = 2 * qpl::pi + atan;
			return static_cast<maths_float_type>(atan);
		}

		qpl::vector2<T> a;
		qpl::vector2<T> b;
	};
	using straight_line = straight_line_t<qpl::f32>;

	template<typename T>
	struct hitbox_t {
		hitbox_t() {
			this->position = this->dimension = qpl::vector2<T>(0, 0);
		}
		hitbox_t(qpl::vector2<T> position, qpl::vector2<T> dimension) {
			this->position = position;
			this->dimension = dimension;
		}

		void set_dimension(qpl::vector2<T> dimension) {
			this->dimension = dimension;
		}
		void set_position(qpl::vector2<T> position) {
			this->position = position;
		}
		void set_center(qpl::vector2<T> position) {
			this->position = position - this->dimension / 2;
		}


		qpl::vector2<T> get_center() const {
			return this->position + this->dimension / 2;
		}
		qpl::vector2<T> get_dimension() const {
			return this->dimension;
		}
		qpl::vector2<T> get_position() const {
			return this->position;
		}

		std::string string() const {
			return qpl::to_string('[', this->position.string(), ", ", this->dimension.string(), ']');
		}

		void increase(T delta) {
			this->position -= qpl::vector2<T>(delta, delta);
			this->dimension += qpl::vector2<T>(delta, delta) * 2;
		}
		qpl::hitbox_t<T> increased(qpl::f32 delta) const {
			auto copy = *this;
			copy.increase(delta);
			return copy;
		}
		bool contains(qpl::vector2f position) const {
			return (position.x > this->position.x && position.x < (this->position.x + this->dimension.x) &&
				position.y > this->position.y && position.y < (this->position.y + this->dimension.y));
		}

		template<typename U>
		bool collides(qpl::straight_line_t<U> line) const {

			auto x1 = this->get_position().x;
			auto x2 = this->get_position().x + this->get_dimension().x;
			auto y1 = this->get_position().y;
			auto y2 = this->get_position().y + this->get_dimension().y;

			return this->collides(x1, x2, y1, y2, line);
		}

		template<typename U>
		bool collides(qpl::straight_line_t<U> line, T increase) const {

			auto x1 = this->get_position().x - increase;
			auto x2 = this->get_position().x + this->get_dimension().x + increase;
			auto y1 = this->get_position().y - increase;
			auto y2 = this->get_position().y + this->get_dimension().y + increase;

			return this->collides(x1, x2, y1, y2, line);
		}

		template<typename U>
		void move(qpl::vector2<U> delta) {
			this->position += qpl::vector2<T>(delta);
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		void move(U x, U y) {
			this->position += qpl::vector2<T>(x, y);
		}



		qpl::vector2<T> dimension;
		qpl::vector2<T> position;

		private:

			template<typename U>
			bool collides(T x1, T x2, T y1, T y2, qpl::straight_line_t<U> line) const {

				auto x_a_inside = (line.a.x > x1) && (line.a.x < x2);
				auto y_a_inside = (line.a.y > y1) && (line.a.y < y2);
				if (x_a_inside && y_a_inside) return true;

				auto x_b_inside = (line.b.x > x1) && (line.b.x < x2);
				auto y_b_inside = (line.b.y > y1) && (line.b.y < y2);
				if (x_b_inside && y_b_inside) return true;

				auto x_cross = ((line.a.x < x1) && (line.b.x > x2)) || ((line.b.x < x1) && (line.a.x > x2));
				if (x_cross && (y_a_inside && y_b_inside)) return true;

				auto y_cross = ((line.a.y < y1) && (line.b.y > y2)) || ((line.b.y < y1) && (line.a.y > y2));
				if (y_cross && (x_a_inside && x_b_inside)) return true;

				if (line.a.x < x1 && line.b.x < x1) return false;
				if (line.a.x > x2 && line.b.x > x2) return false;
				if (line.a.y < y1 && line.b.y < y1) return false;
				if (line.a.y > y2 && line.b.y > y2) return false;

				auto c1 = qpl::vec(x1, y1);
				auto c2 = qpl::vec(x2, y1);
				auto c3 = qpl::vec(x1, y2);
				auto c4 = qpl::vec(x2, y2);

				qpl::straight_line line1(line.a, {});
				qpl::straight_line line2(line.a, {});
				bool found = false;

				if (line.a.x < c1.x && line.a.y < c1.y) { //a_outside_c1
					line1.b = c3;
					line2.b = c2;
					found = true;
				}
				else if (line.a.x > c2.x && line.a.y < c2.y) { //a_outside_c2
					line1.b = c1;
					line2.b = c4;
					found = true;
				}
				else if (line.a.x < c3.x && line.a.y > c3.y) { //a_outside_c3
					line1.b = c4;
					line2.b = c1;
					found = true;
				}
				else if (line.a.x > c4.x && line.a.y > c4.y) { //a_outside_c3
					line1.b = c2;
					line2.b = c3;
					found = true;
				}
				else if (line.a.x < c1.x) { //a_left
					line1.b = c3;
					line2.b = c1;
					found = true;
				}
				else if (line.a.y < c1.y) { //a_top
					line1.b = c1;
					line2.b = c2;
					found = true;
				}
				else if (line.a.x > c2.x) { //a_right
					line1.b = c2;
					line2.b = c4;
					found = true;
				}
				else if (line.a.y > c2.y) { //a_bottom
					line1.b = c4;
					line2.b = c3;
					found = true;
				}

				if (!found) {
					return false;
				}
				auto line_angle = line.angle();

				auto angle1 = line1.angle();
				auto angle2 = line2.angle();

				if (angle2 < angle1) {
					if (line_angle < qpl::pi) {
						angle1 -= qpl::pi * 2;
					}
					else {
						angle2 += qpl::pi * 2;
					}
				}

				return (line_angle >= angle1 && line_angle <= angle2);
			}


	};

	using hitbox = hitbox_t<qpl::f32>;

}


#endif