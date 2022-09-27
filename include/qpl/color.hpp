#ifndef QPL_COLOR_HPP
#define QPL_COLOR_HPP
#pragma once

#if defined QPL_INTERN_SFML_USE
#include <SFML/Graphics.hpp>
#endif

#include <qpl/qpldeclspec.hpp>
#include <qpl/defines.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/vector.hpp>
#include <qpl/vardef.hpp>

#include <string>
#include <initializer_list>
#include <array>
#include <tuple>

namespace qpl {

	namespace detail {

		template<typename T>
		struct r_impl {
			union {
				struct {
					T r;
				};
				std::array<T, 1> data;
			};

			constexpr r_impl() : data() {
				if constexpr (qpl::is_floating_point<T>()) {
					this->data.fill(T{ 1 });
				}
				else {
					this->data.fill(qpl::type_max<T>());
				}
			}
		};
		template<typename T>
		struct rg_impl {
			union {
				struct {
					T r;
					T g;
				};
				std::array<T, 2> data;
			};

			constexpr rg_impl() : data() {
				if constexpr (qpl::is_floating_point<T>()) {
					this->data.fill(T{ 1 });
				}
				else {
					this->data.fill(qpl::type_max<T>());
				}
			}
		};
		template<typename T>
		struct rgb_impl {
			union {
				struct {
					T r;
					T g;
					T b;
				};
				std::array<T, 3> data;
			};

			constexpr rgb_impl() : data() {
				if constexpr (qpl::is_floating_point<T>()) {
					this->data.fill(T{ 1 });
				}
				else {
					this->data.fill(qpl::type_max<T>());
				}
			}
		};
		template<typename T>
		struct rgba_impl {
			union {
				struct {
					T r;
					T g;
					T b;
					T a;
				};
				std::array<T, 4> data;
			};

			constexpr rgba_impl() : data() {
				if constexpr (qpl::is_floating_point<T>()) {
					this->data.fill(T{ 1 });
				}
				else {
					this->data.fill(qpl::type_max<T>());
				}
			}
		};
	

		template<typename T, qpl::size N>
		using conditional_rgb =
			qpl::conditional<
			qpl::if_true<N == 1>, qpl::detail::r_impl<T>,
			qpl::if_true<N == 2>, qpl::detail::rg_impl<T>,
			qpl::if_true<N == 3>, qpl::detail::rgb_impl<T>,
			qpl::if_true<N == 4>, qpl::detail::rgba_impl<T>>;
	}

	template<typename T, qpl::size N> requires (N >= 1 && N <= 4 && qpl::is_arithmetic<T>() && !(qpl::is_integer<T>() && qpl::is_signed<T>()))
	struct rgbN : qpl::detail::conditional_rgb<T, N> {
		using impl_type = qpl::detail::conditional_rgb<T, N>;

		using delta_type = qpl::conditional<qpl::if_true<is_floating_point<T>()>, T, qpl::f64>;
		using visible_type = qpl::conditional<qpl::if_true<is_same<T, qpl::u8>()>, qpl::u32, qpl::f64>;

		constexpr static bool has_green() {
			return N >= 2;
		}
		constexpr static bool has_blue() {
			return N >= 3;
		}
		constexpr static bool has_alpha() {
			return N >= 4;
		}
		constexpr static bool is_floating_point() {
			return qpl::is_floating_point<T>();
		}
		constexpr static bool is_integer() {
			return qpl::is_integer<T>();
		}
		constexpr static auto min_channel() {
			return T{ 0 };
		}
		constexpr static auto max_channel() {
			if constexpr (is_floating_point()) {
				return T{ 1 };
			}
			else {
				return qpl::type_max<T>();
			}
		}

	
		constexpr rgbN() : impl_type() {

		}
		template<typename U>
		constexpr rgbN(const std::initializer_list<U>& list) : impl_type() {
			*this = list;
		}

		template<typename Tuple> requires(qpl::is_tuple<Tuple>())
		constexpr rgbN(const Tuple& tuple) : impl_type() {
			*this = tuple;
		}
		template<typename... Args> requires(sizeof...(Args) > 1 && sizeof...(Args) <= N)
		constexpr rgbN(Args&&... list) : impl_type() {
			*this = std::make_tuple(list...);
		}

		constexpr rgbN(const qpl::rgbN<T, N>& other) : impl_type() {
			*this = other;
		}
		template<typename U, qpl::size M>
		constexpr rgbN(const qpl::rgbN<U, M>& other) : impl_type() {
			*this = other;
		}

		template<typename U, qpl::size M>
		constexpr rgbN(const qpl::vectorN<U, M>& other) : impl_type() {
			*this = other;
		}
		constexpr rgbN(rgbN<T, N> (*func)()) : impl_type() {
			*this = func();
		}

		constexpr qpl::rgbN<T, N>& operator=(const qpl::rgbN<T, N>& other) {
			this->data = other.data;
			return *this;
		}
		template<typename U, qpl::size M>
		constexpr qpl::rgbN<T, N>& operator=(const qpl::rgbN<U, M>& other) {
			constexpr qpl::f64 delta = qpl::f64_cast(max_channel()) / qpl::rgbN<U, M>::max_channel();

			this->clear_black();

			if constexpr (is_floating_point()) {
				for (qpl::size i = 0u; i < std::min(N, M); ++i) {
					this->data[i] = static_cast<T>(other[i] * delta);
				}
			}
			else {
				for (qpl::size i = 0u; i < std::min(N, M); ++i) {
					this->data[i] = static_cast<T>(qpl::clamp<qpl::f64>(0, other[i] * delta, max_channel()));
				}
			}
			return *this;
		}
		template<typename U, qpl::size M>
		constexpr qpl::rgbN<T, N>& operator=(const qpl::vectorN<U, M>& other) {
			this->clear_black();

			if constexpr (is_floating_point()) {
				for (qpl::size i = 0u; i < std::min(N, M); ++i) {
					this->data[i] = static_cast<T>(other[i]);
				}
			}
			else {
				for (qpl::size i = 0u; i < std::min(N, M); ++i) {
					this->data[i] = static_cast<T>(qpl::clamp<qpl::f64>(0, other[i], max_channel()));
				}
			}
			return *this;
		}
		template<typename U>
		constexpr qpl::rgbN<T, N>& operator=(const std::initializer_list<U>& list) {
			this->clear_black();
			for (qpl::size i = 0u; i < qpl::min(list.size(), this->data.size()); ++i) {
				this->data[i] = static_cast<T>(*(list.begin() + i));
			}
			return *this;
		}

		template<typename Tuple> requires(qpl::is_tuple<Tuple>())
		constexpr qpl::rgbN<T, N>& operator=(const Tuple& tuple) {
			this->clear_black();

			constexpr auto size = std::min(N, std::tuple_size_v<Tuple>);
			qpl::constexpr_iterate<size>([&](auto i) {
				this->data[i] = static_cast<T>(std::get<i>(tuple));
			});
			return *this;
		}

		constexpr qpl::rgbN<T, N>& operator=(rgbN<T, N>(*func)()) {
			*this = func();
			return *this;
		}

#if defined QPL_INTERN_SFML_USE
		constexpr rgbN(sf::Color color) {
			*this = color;
		}
		constexpr qpl::rgbN<T, N>& operator=(sf::Color color) {
			constexpr qpl::f64 delta = qpl::f64_cast(max_channel()) / 255;

			this->r = static_cast<T>(color.r * delta);

			if constexpr (has_green()) {
				this->g = static_cast<T>(color.g * delta);
			}
			if constexpr (has_blue()) {
				this->b = static_cast<T>(color.b * delta);
			}
			if constexpr (has_alpha()) {
				this->a = static_cast<T>(color.a * delta);
			}
			return *this;
		}
		constexpr operator sf::Color() const {

			constexpr qpl::f64 delta = 255 / qpl::f64_cast(max_channel());

			if constexpr (N == 1) {
				return sf::Color(static_cast<sf::Uint8>(this->r * delta), 0, 0);
			}
			else if constexpr (N == 2) {
				return  sf::Color(static_cast<sf::Uint8>(this->r * delta), static_cast<sf::Uint8>(this->g * delta), 0);
			}
			else if constexpr (N == 3) {
				return sf::Color(static_cast<sf::Uint8>(this->r * delta), static_cast<sf::Uint8>(this->g * delta), static_cast<sf::Uint8>(this->b * delta));
			}
			else {
				return sf::Color(static_cast<sf::Uint8>(this->r * delta), static_cast<sf::Uint8>(this->g * delta), static_cast<sf::Uint8>(this->b * delta), static_cast<sf::Uint8>(this->a * delta));
			}
		}
#endif
		constexpr void clear_black() {
			this->data.fill(min_channel());
			if constexpr (has_alpha()) {
				this->data[3] = max_channel();
			}
		}
		constexpr void clear_white() {
			this->data.fill(max_channel());
		}

		constexpr qpl::size size() const {
			return this->data.size();
		}

		constexpr auto operator[](qpl::size index) {
			return this->data[index];
		}
		constexpr const auto operator[](qpl::size index) const {
			return this->data[index];
		}
		constexpr auto at(qpl::size index) {
			return this->data[index];
		}
		constexpr const auto at(qpl::size index) const {
			return this->data[index];
		}
		constexpr auto begin() {
			return this->data.begin();
		}
		constexpr const auto begin() const {
			return this->data.begin();
		}
		constexpr const auto cbegin() const {
			return this->data.cbegin();
		}
		constexpr auto end() {
			return this->data.end();
		}
		constexpr const auto end() const {
			return this->data.end();
		}
		constexpr const auto cend() const {
			return this->data.cend();
		}
		constexpr auto rbegin() {
			return this->data.rbegin();
		}
		constexpr const auto rbegin() const {
			return this->data.rbegin();
		}
		constexpr const auto rcbegin() const {
			return this->data.crbegin();
		}
		constexpr auto rend() {
			return this->data.rend();
		}
		constexpr const auto rend() const {
			return this->data.rend();
		}
		constexpr const auto rcend() const {
			return this->data.crend();
		}

		constexpr auto min() const {
			return *std::min_element(this->data.cbegin(), this->data.cend());
		}
		constexpr auto max() const {
			return *std::max_element(this->data.cbegin(), this->data.cend());
		}
		constexpr auto sum() const {
			return std::accumulate(this->data.cbegin(), this->data.cend(), T{ 0 });
		}

		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr qpl::rgbN<T, N>& operator/=(U value) {
			for (auto& i : this->data) {
				i = static_cast<T>(i / value);
			}
			return *this;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr auto operator/(U value) const {
			rgbN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy /= value;
			return copy;
		}

		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr qpl::rgbN<T, N>& operator*=(U value) {
			for (auto& i : this->data) {
				i = static_cast<T>(i * value);
			}
			return *this;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr auto operator*(U value) const {
			rgbN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy *= value;
			return copy;
		}

		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr qpl::rgbN<T, N>& operator+=(U value) {
			for (auto& i : this->data) {
				i = static_cast<T>(i + value);
			}
			return *this;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr auto operator+(U value) const {
			rgbN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy += value;
			return copy;
		}

		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr qpl::rgbN<T, N>& operator-=(U value) {
			for (auto& i : this->data) {
				i = static_cast<T>(i - value);
			}
			return *this;
		}
		template<typename U> requires (qpl::is_arithmetic<U>())
		constexpr auto operator-(U value) const {
			rgbN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy -= value;
			return copy;
		}

		constexpr qpl::rgbN<T, N>& operator/=(const qpl::rgbN<T, N>& other) {
			for (qpl::size i = 0u; i < N; ++i) {
				this->data[i] /= other[i];
			}
			return *this;
		}
		template<typename U>
		constexpr qpl::rgbN<T, N> operator/(const qpl::rgbN<U, N>& other) const {
			qpl::rgbN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy /= other;
			return copy;
		}

		constexpr qpl::rgbN<T, N>& operator*=(const qpl::rgbN<T, N>& other) {
			for (qpl::size i = 0u; i < N; ++i) {
				this->data[i] *= other[i];
			}
			return *this;
		}
		template<typename U>
		constexpr qpl::rgbN<T, N> operator*(const qpl::rgbN<U, N>& other) const {
			qpl::rgbN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy *= other;
			return copy;
		}

		constexpr qpl::rgbN<T, N>& operator+=(const qpl::rgbN<T, N>& other) {
			for (qpl::size i = 0u; i < N; ++i) {
				this->data[i] += other[i];
			}
			return *this;
		}
		template<typename U>
		constexpr qpl::rgbN<T, N> operator+(const qpl::rgbN<U, N>& other) const {
			qpl::rgbN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy += other;
			return copy;
		}

		constexpr qpl::rgbN<T, N>& operator-=(const qpl::rgbN<T, N>& other) {
			for (qpl::size i = 0u; i < N; ++i) {
				this->data[i] -= other[i];
			}
			return *this;
		}
		template<typename U>
		constexpr qpl::rgbN<T, N> operator-(const qpl::rgbN<U, N>& other) const {
			qpl::rgbN<qpl::superior_arithmetic_type<T, U>, N> copy = *this;
			copy -= other;
			return copy;
		}


		constexpr bool operator==(const qpl::rgbN<T, N>& other) const {
			return this->data == other.data;
		}
		constexpr bool operator!=(const qpl::rgbN<T, N>& other) const {
			return this->data != other.data;
		}


		constexpr void brighten(delta_type delta) {
			if constexpr (is_floating_point()) {
				if constexpr (has_alpha()) {
					auto acopy = this->a;
					delta = qpl::clamp_0_1(delta);
					*this = white() * delta + *this * (1 - delta);
					this->a = acopy;
				}
				else {
					delta = qpl::clamp_0_1(delta);
					*this = white() * delta + *this * (1 - delta);
				}
			}
			else {
				qpl::rgbN<qpl::f64, N> color = *this;
				color.brighten(delta);
				*this = color;
			}
		}
		constexpr qpl::rgbN<T, N> brightened(delta_type delta) const {
			auto copy = *this;
			copy.brighten(delta);
			return copy;
		}
		constexpr void darken(delta_type delta) {
			if constexpr (is_floating_point()) {
				if constexpr (has_alpha()) {
					auto acopy = this->a;
					delta = qpl::clamp_0_1(delta);
					*this = *this * (1 - delta);
					this->a = acopy;
				}
				else {
					delta = qpl::clamp_0_1(delta);
					*this = *this * (1 - delta);
				}
			}
			else {
				qpl::rgbN<qpl::f64, N> color = *this;
				color.brighten(delta);
				*this = color;
			}
		}
		constexpr qpl::rgbN<T, N> darkened(delta_type delta) const {
			auto copy = *this;
			copy.darken(delta);
			return copy;
		}
		constexpr void light(delta_type delta) {
			if (delta < 0) {
				this->darken(-delta);
			}
			else {
				this->brighten(delta);
			}
		}
		constexpr qpl::rgbN<T, N> lighted(delta_type delta) const {
			auto copy = *this;
			copy.light(delta);
			return copy;
		}
		constexpr void intensify(delta_type delta) {
			if constexpr (is_floating_point()) {
				if constexpr (has_alpha()) {
					auto acopy = this->a;
					delta = qpl::clamp_0_1(delta);
					auto min = this->min();
					auto max = this->max();

					auto diff = max - min;

					auto saturated = (*this - qpl::rgbN<T, N>::filled(min)) / diff;
					*this = saturated * delta + *this * (1 - delta);
					this->a = acopy;
				}
				else {
					delta = qpl::clamp_0_1(delta);
					auto min = this->min();
					auto max = this->max();

					auto diff = max - min;

					auto saturated = (*this - qpl::rgbN<T, N>::filled(min)) / diff;
					*this = saturated * delta + *this * (1 - delta);
				}
			}
			else {
				qpl::rgbN<qpl::f64, N> color = *this;
				color.intensify(delta);
				*this = color;
			}
		}
		constexpr qpl::rgbN<T, N> intensified(delta_type delta) const {
			auto copy = *this;
			copy.intensify(delta);
			return copy;
		}
		constexpr void grayify(delta_type delta) {

			if constexpr (is_floating_point()) {
				if constexpr (has_alpha()) {
					auto acopy = this->a;
					delta = qpl::clamp_0_1(delta);
					auto sum = this->sum();
					auto grayified = qpl::rgbN<T, N>(sum, sum, sum) / 3.0f;
					*this = grayified * delta + *this * (1 - delta);
					this->a = acopy;
				}
				else {
					delta = qpl::clamp_0_1(delta);
					auto sum = this->sum();
					auto grayified = qpl::rgbN<T, N>(sum, sum, sum) / 3.0f;
					*this = grayified * delta + *this * (1 - delta);
				}
			}
			else {
				qpl::rgbN<qpl::f64, N> color = *this;
				color.grayify(delta);
				*this = color;
			}

		}
		constexpr qpl::rgbN<T, N> grayified(delta_type delta) const {
			auto copy = *this;
			copy.grayify(delta);
			return copy;
		}
		constexpr void saturate(delta_type delta) {
			if (delta < 0) {
				this->grayify(-delta);
			}
			else {
				this->intensify(delta);
			}
		}
		constexpr qpl::rgbN<T, N> saturated(delta_type delta) const {
			auto copy = *this;
			copy.saturate(delta);
			return copy;
		}
		constexpr void interpolate(const qpl::rgbN<T, N>& other, delta_type delta) {
			*this = *this * (1.0 - delta) + (other * delta);
		}
		constexpr qpl::rgbN<T, N> interpolated(const qpl::rgbN<T, N>& other, delta_type delta) const {
			auto copy = *this;
			copy.interpolate(other, delta);
			return copy;
		}

		constexpr void invert() {
			for (qpl::size i = 0u; i < std::min(qpl::size{ 3 }, this->size()); ++i) {
				this->data[i] = max_channel() - this->data[i];
			}
		}
		constexpr qpl::rgbN<T, N> inverted() const {
			auto copy = *this;
			copy.invert();
			return copy;
		}

		constexpr qpl::rgbN<T, N> with_alpha(T alpha) const requires (N >= 4) {
			auto copy = *this;
			copy.data[3] = alpha;
			return copy;
		}
		constexpr qpl::rgbN<T, N> multiplied_color(const qpl::rgbN<T, N>& other) const {
			auto copy = *this;
			copy = copy * qpl::rgbN<qpl::f32, N>(other);
			//copy = qpl::rgbN<qpl::f32, N>(copy) * qpl::rgbN<qpl::f32, N>(other);
			return copy;
		}
		
		constexpr qpl::rgbN<T, N> multiplied_alpha(T alpha) const requires (N >= 4) {
			auto copy = *this;
			copy.data[3] = static_cast<T>(copy.data[3] * (alpha / qpl::f64_cast(max_channel())));
			return copy;
		}

		constexpr static auto red() {
			return qpl::rgbN<T, N> { max_channel(), 0, 0 };
		}
		constexpr static auto green() {
			return qpl::rgbN<T, N>(0, max_channel(), 0);
		}
		constexpr static auto blue() {
			return qpl::rgbN<T, N>(0, 0, max_channel());
		}
		constexpr static auto white() {
			return qpl::rgbN<T, N>(max_channel(), max_channel(), max_channel());
		}
		constexpr static auto black() {
			return qpl::rgbN<T, N>(0, 0, 0);
		}
		constexpr static auto yellow() {
			return qpl::rgbN<T, N>(max_channel(), max_channel(), 0);
		}
		constexpr static auto cyan() {
			return qpl::rgbN<T, N>(0, max_channel(), max_channel());
		}
		constexpr static auto magenta() {
			return qpl::rgbN<T, N>(max_channel(), 0, max_channel());
		}
		constexpr static auto unset() requires(N >= 4) {
			return rgbN(0, 0, 0, 0);
		}
		constexpr static auto transparent() requires(N >= 4) {
			return rgbN(max_channel(), max_channel(), max_channel(), 0);
		}

		constexpr bool is_unset() const requires(N >= 4) {
			return *this == unset();
		}

		template<typename U> requires(qpl::is_arithmetic<U>())
		[[nodiscard]] constexpr static rgbN<T, N> filled(U value) {
			rgbN<T, N> result;
			result.data.fill(static_cast<T>(value));
			return result;
		}

		template<typename U> requires(qpl::is_arithmetic<U>())
		[[nodiscard]] constexpr static rgbN<T, N> grey_shade(U value) {
			rgbN<T, N> result;
			result.data.fill(static_cast<T>(value));
			if constexpr (N >= 4) {
				result.data[3] = max_channel();
			}
			return result;
		}

		[[nodiscard]] constexpr static qpl::rgbN<T, N> interpolation(const std::vector<qpl::rgbN<T, N>>& colors, delta_type strength) {
			strength = qpl::clamp(0.0, strength, 1.0);
			if (strength == 1.0) {
				return colors.back();
			}

			auto index = qpl::size_cast(strength * (colors.size() - 1));
			auto left_over = static_cast<delta_type>(((colors.size() - 1) * strength) - index);

			return colors[index].interpolated(colors[index + 1], left_over);
		}
		template<typename U, qpl::size M>
		[[nodiscard]] constexpr static qpl::rgbN<T, N> interpolation(const std::vector<qpl::rgbN<U, M>>& colors, delta_type strength) {
			return interpolation(qpl::type_cast<qpl::rgbN<T, N>>(colors), strength);
		}
		constexpr std::string string() const {
			bool first = true;
			std::ostringstream stream;
			stream << '(';
			for (auto& i : this->data) {
				if (!first) {
					stream << ", ";
				}
				stream << static_cast<visible_type>(i);
				first = false;
			}
			stream << ')';
			return stream.str();
		}

		template<typename T, qpl::size N>
		friend std::ostream& operator<<(std::ostream& os, const qpl::rgbN<T, N>& color);
	};

	template<typename T, qpl::size N>
	std::ostream& operator<<(std::ostream& os, const qpl::rgbN<T, N>& color) {
		return (os << color.string());
	}

	template<typename T, typename U, qpl::size N> requires(qpl::is_arithmetic<U>())
	constexpr auto operator+(U value, const rgbN<T, N>& vec) {
		return vec + value;
	}
	template<typename T, typename U, qpl::size N> requires(qpl::is_arithmetic<U>())
	constexpr auto operator-(U value, const rgbN<T, N>& vec) {
		return rgbN<T, N>::filled(value) - vec;
	}
	template<typename T, typename U, qpl::size N> requires(qpl::is_arithmetic<U>())
	constexpr auto operator*(U value, const rgbN<T, N>& vec) {
		return vec * value;
	}
	template<typename T, typename U, qpl::size N> requires(qpl::is_arithmetic<U>())
	constexpr auto operator/(U value, const rgbN<T, N>& vec) {
		return rgbN<T, N>::filled(value) / vec;
	}

	using rgb = rgbN<qpl::u8, 3>;
	using rgba = rgbN<qpl::u8, 4>;
	using frgb = rgbN<qpl::f32, 3>;
	using frgba = rgbN<qpl::f32, 4>;

	QPLDLL qpl::rgb get_random_color();
	QPLDLL qpl::rgba get_random_transparency_color();
	QPLDLL qpl::rgb get_rainbow_color(qpl::f64 f);
	QPLDLL qpl::rgb get_random_rainbow_color();
}


#if defined QPL_INTERN_VULKAN_USE
#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <string>

#include <glm/glm.hpp>

namespace qpl::vk {
	class frgb {
	public:
		constexpr frgb() noexcept : m_rgb(0, 0, 0) {

		}
		constexpr frgb(const frgb& other) noexcept : m_rgb(other.m_rgb) {

		}
		constexpr frgb(const glm::vec3& other) noexcept : m_rgb(other) {

		}
		constexpr frgb(qpl::f32 r, qpl::f32 g, qpl::f32 b) noexcept : m_rgb(r, g, b) {

		}
		QPLDLL frgb& operator=(const qpl::frgb& other);
		QPLDLL frgb& operator=(const glm::vec3& other);
		QPLDLL bool operator==(const frgb& other) const;
		QPLDLL bool operator==(const glm::vec3& other) const;

		QPLDLL operator glm::vec3() const;
		QPLDLL glm::vec3 get() const;

		QPLDLL std::string string() const;


		QPLDLL void brighten(qpl::f32 delta);
		QPLDLL qpl::vk::frgb brightened(qpl::f32 delta) const;

		QPLDLL void darken(qpl::f32 delta);
		QPLDLL qpl::vk::frgb darkened(qpl::f32 delta) const;

		QPLDLL void light(qpl::f32 delta);
		QPLDLL qpl::vk::frgb lighted(qpl::f32 delta) const;

		QPLDLL void intensify(qpl::f32 delta);
		QPLDLL qpl::vk::frgb intensified(qpl::f32 delta) const;

		QPLDLL void grayify(qpl::f32 delta);
		QPLDLL qpl::vk::frgb grayified(qpl::f32 delta) const;

		QPLDLL void saturate(qpl::f32 delta);
		QPLDLL qpl::vk::frgb saturated(qpl::f32 delta) const;


		QPLDLL void black_gray_intense_white(qpl::f32 delta);
		QPLDLL qpl::vk::frgb black_gray_intense_whited(qpl::f32 delta) const;

		QPLDLL void black_intense_white(qpl::f32 delta);
		QPLDLL qpl::vk::frgb black_intense_whited(qpl::f32 delta) const;

		QPLDLL void interpolate(qpl::frgb other, qpl::f32 delta);
		QPLDLL qpl::vk::frgb interpolated(qpl::frgb other, qpl::f32 delta) const;

		const static qpl::frgb red;
		const static qpl::frgb green;
		const static qpl::frgb blue;
		const static qpl::frgb black;
		const static qpl::frgb white;
		const static qpl::frgb yellow;
		const static qpl::frgb magenta;
		const static qpl::frgb cyan;

		const static qpl::frgb orange;
		const static qpl::frgb purple;
		const static qpl::frgb amaranth;
		const static qpl::frgb turquoise;
	private:
		glm::vec3 m_rgb;
	};

	QPLDLL qpl::vk::frgb random_color();


	inline constexpr qpl::vk::frgb qpl::vk::frgb::red = qpl::vk::frgb(1.0f, 0.0f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::green = qpl::vk::frgb(0.0f, 1.0f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::blue = qpl::vk::frgb(0.5f, 0.5f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::black = qpl::vk::frgb(0.0f, 0.0f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::white = qpl::vk::frgb(1.0f, 1.0f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::yellow = qpl::vk::frgb(1.0f, 1.0f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::magenta = qpl::vk::frgb(1.0f, 0.0f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::cyan = qpl::vk::frgb(0.0f, 1.0f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::orange = qpl::vk::frgb(1.0f, 0.5f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::purple = qpl::vk::frgb(0.5f, 0.0f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::amaranth = qpl::vk::frgb(1.0f, 0.0f, 0.5f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::turquoise = qpl::vk::frgb(0.0f, 1.0f, 0.5f);
}
#endif


//namespace std {
	//template <>
	//struct hash<qpl::rgba> {
	//	qpl::u32 operator()(const qpl::rgba& k) const {
	//		return k.uint();
	//	}
	//};
//}
#endif