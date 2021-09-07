#ifndef QPL_FRACTION_HPP
#define QPL_FRACTION_HPP
#pragma once


#include <qpl/type_traits.hpp>
#include <qpl/string.hpp>
#include <string>

namespace qpl {
	template<typename T, typename F> requires(qpl::is_integer<T>() && qpl::is_signed<T>() && qpl::is_floating_point<F>())
	struct fraction_type {
		T n;
		T d;

		static constexpr T gcd(T a, T b) {
            if (a == 0) {
                return b;
            }
            if (b == 0) {
                return a;
            }
            if constexpr (qpl::is_signed<T>()) {
                if (a < T{} && b >= T{}) {
                    return -gcd(a * -1, b);
                }
                else if (a >= T{} && b < T{}) {
                    return -gcd(a, b * -1);
                }
                else if (a < T{} && b < T{}) {
                    return gcd(a * -1, b * -1);
                }
            }

            qpl::u32 k;
            for (k = 0; ((a | b) & 1) == 0; ++k) {
                a >>= 1;
                b >>= 1;
            }

            while ((a & 1) == 0) {
                a >>= 1;
            }

            do {
                while ((b & 1) == 0) {
                    b >>= 1;
                }

                if (a > b) {
                    std::swap(a, b);
                }
                b = (b - a);
            } while (b != 0);

            return a << k;
        }

        constexpr void reduce() {
            auto a = gcd(this->n, this->d);
            this->n /= a;
            this->d /= a;
            if (this->d < T{}) {
                this->n *= -1;
                this->d *= -1;
            }
        }

		template<typename T1, typename T2>
		constexpr fraction_type(T1 a, T2 b) : n(static_cast<T>(a)), d(static_cast<T>(b)) {
            this->reduce();
		}
        constexpr fraction_type() : n(1), d(1) {

        }
		constexpr operator F() const {
			return static_cast<F>(this->n) / this->d;
		}
        constexpr F get_float() const {
            return this->operator F();
        }
		std::string string() const {
			return qpl::to_string(this->n, " / ", this->d);
		}

        void inverse() {
            std::swap(this->n, this->d);
        }
        fraction_type inversed() const {
            auto copy = *this;
            copy.inverse();
            return copy;
        }

        fraction_type operator+() const {
            auto copy = *this;
            if (copy.n < T{}) {
                copy.n *= -1;
            }
            return copy;
        }
        fraction_type operator-() const {
            auto copy = *this;
            copy.n *= -1;
            return copy;
        }

        fraction_type& operator+=(const fraction_type& other) {
            auto lcm = qpl::abs(this->d * other.d) / gcd(this->d, other.d);
            auto a = *this;
            this->n *= (lcm / a.d);
            this->d = lcm;
            this->n += (other.n * (lcm / other.d));
            this->reduce();
            return *this;
        }
        fraction_type operator+(const fraction_type& other) const {
            auto copy = *this;
            copy += other;
            return copy;
        }
        fraction_type& operator-=(const fraction_type& other) {
            return this->operator+=(-other);
        }
        fraction_type operator-(const fraction_type& other) const {
            auto copy = *this;
            copy -= other;
            return copy;
        }
        fraction_type& operator*=(const fraction_type& other) {
            this->n *= other.n;
            this->d *= other.d;
            this->reduce();
            return *this;
        }
        fraction_type operator*(const fraction_type& other) const {
            auto copy = *this;
            copy *= other;
            return copy;
        }
        fraction_type& operator/=(const fraction_type& other) {
            this->n *= other.d;
            this->d *= other.n;
            this->reduce();
            return *this;
        }
        fraction_type operator/(const fraction_type& other) const {
            auto copy = *this;
            copy /= other;
            return copy;
        }
        
        template<typename U>
        fraction_type& operator+=(U value) {
            this->n += this->d * value;
            this->reduce();
            return *this;
        }
        template<typename U>
        fraction_type operator+(U value) const {
            auto copy = *this;
            copy += value;
            return copy;
        }
        template<typename U>
        fraction_type& operator-=(U value) {
            this->n -= this->d * value;
            this->reduce();
            return *this;
        }
        template<typename U>
        fraction_type operator-(U value) const {
            auto copy = *this;
            copy -= value;
            return copy;
        }
        template<typename U>
        fraction_type& operator*=(U value) {
            this->n *= value;
            this->reduce();
            return *this;
        }
        template<typename U>
        fraction_type operator*(U value) const {
            auto copy = *this;
            copy *= value;
            return copy;
        }
        template<typename U>
        fraction_type& operator/=(U value) {
            this->d *= value;
            this->reduce();
            return *this;
        }
        template<typename U>
        fraction_type operator/(U value) const {
            auto copy = *this;
            copy /= value;
            return copy;
        }
	};

	using fraction = fraction_type<qpl::i64, qpl::f64>;
}

#endif