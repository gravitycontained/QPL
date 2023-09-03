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
					this->data[i] = static_cast<T>(qpl::clamp<qpl::f64>(0, std::round(other[i] * delta), max_channel()));
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
					this->data[i] = static_cast<T>(qpl::clamp<qpl::f64>(0, std::round(other[i]), max_channel()));
				}
			}
			return *this;
		}
		template<typename U>
		constexpr qpl::rgbN<T, N>& operator=(const std::initializer_list<U>& list) {
			this->clear_black();

			if constexpr (is_floating_point()) {
				for (qpl::size i = 0u; i < qpl::min(list.size(), this->data.size()); ++i) {
					this->data[i] = static_cast<T>(*(list.begin() + i));
				}
			}
			else {
				for (qpl::size i = 0u; i < qpl::min(list.size(), this->data.size()); ++i) {
					this->data[i] = static_cast<T>(std::round(*(list.begin() + i)));
				}
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
				return sf::Color(static_cast<sf::Uint8>(std::round(this->r * delta)), 0, 0);
			}
			else if constexpr (N == 2) {
				return sf::Color(static_cast<sf::Uint8>(std::round(this->r * delta)), static_cast<sf::Uint8>(std::round(this->g * delta)), 0);
			}
			else if constexpr (N == 3) {
				return sf::Color(static_cast<sf::Uint8>(std::round(this->r * delta)), static_cast<sf::Uint8>(std::round(this->g * delta)), static_cast<sf::Uint8>(std::round(this->b * delta)));
			}
			else {
				return sf::Color(static_cast<sf::Uint8>(std::round(this->r * delta)), static_cast<sf::Uint8>(std::round(this->g * delta)), static_cast<sf::Uint8>(std::round(this->b * delta)), static_cast<sf::Uint8>(std::round(this->a * delta)));
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

		constexpr auto& operator[](qpl::size index) {
			return this->data[index];
		}
		constexpr const auto& operator[](qpl::size index) const {
			return this->data[index];
		}
		constexpr auto& at(qpl::size index) {
			return this->data[index];
		}
		constexpr const auto& at(qpl::size index) const {
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
			if constexpr (N >= 4) {
				return *std::min_element(this->data.cbegin(), this->data.cend() - 1);
			}
			else {
				return *std::min_element(this->data.cbegin(), this->data.cend());
			}
		}
		constexpr auto max() const {
			if constexpr (N >= 4) {
				return *std::max_element(this->data.cbegin(), this->data.cend() - 1);
			}
			else {
				return *std::max_element(this->data.cbegin(), this->data.cend());
			}
		}
		constexpr auto sum() const {
			if constexpr (N >= 4) {
				return std::accumulate(this->data.cbegin(), this->data.cend() - 1, T{ 0 });
			}
			else {
				return std::accumulate(this->data.cbegin(), this->data.cend(), T{ 0 });
			}
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

		template<typename U, qpl::size M>
		constexpr bool operator==(const qpl::rgbN<U, M>& other) const {
			return *this == qpl::rgbN<T, N>(other);
		}
		template<typename U, qpl::size M>
		constexpr bool operator!=(const qpl::rgbN<U, M>& other) const {
			return *this != qpl::rgbN<T, N>(other);
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
				color.darken(delta);
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
					auto scale = this->sum();
					auto grayified = qpl::rgbN<T, N>(scale, scale, scale) / 3.0f;
					*this = grayified * delta + *this * (1 - delta);
					this->a = acopy;
				}
				else {
					delta = qpl::clamp_0_1(delta);
					auto scale = this->sum();
					auto grayified = qpl::rgbN<T, N>(scale, scale, scale) / 3.0f;
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

		constexpr void grayify_perceived(delta_type delta) {
			if constexpr (is_floating_point()) {
				if constexpr (has_alpha()) {
					auto acopy = this->a;
					delta = qpl::clamp_0_1(delta);
					auto scale = this->get_perceived_brightness();
					auto grayified = qpl::rgbN<qpl::f64, N>(scale, scale, scale);
					*this = grayified * delta + *this * (1 - delta);
					this->a = acopy;
				}
				else {
					delta = qpl::clamp_0_1(delta);
					auto scale = this->get_perceived_brightness();
					auto grayified = qpl::rgbN<qpl::f64, N>(scale, scale, scale);
					*this = grayified * delta + *this * (1 - delta);
				}
			}
			else {
				qpl::rgbN<qpl::f64, N> color = *this;
				color.grayify_perceived(delta);
				*this = color;
			}

		}
		constexpr qpl::rgbN<T, N> grayified_perceived(delta_type delta) const {
			auto copy = *this;
			copy.grayify_perceived(delta);
			return copy;
		}

		constexpr void saturate_perceived(delta_type delta) {
			if (delta < 0) {
				this->grayify_perceived(-delta);
			}
			else {
				this->intensify(delta);
			}
		}
		constexpr qpl::rgbN<T, N> saturated_perceived(delta_type delta) const {
			auto copy = *this;
			copy.saturate_perceived(delta);
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

		constexpr auto with_alpha(T alpha) const {
			if constexpr (N >= 4) {
				auto copy = *this;
				copy.data[3] = alpha;
				return copy;
			}
			else {
				qpl::rgbN<T, 4> copy = *this;
				return copy.with_alpha(alpha);
			}
		}
		constexpr qpl::rgbN<T, N> multiplied_color(const qpl::rgbN<T, N>& other) const {
			auto copy = *this;
			copy = copy * qpl::rgbN<qpl::f32, N>(other);
			return copy;
		}
		
		constexpr auto multiplied_alpha(T alpha) const {
			if constexpr (N >= 4) {
				auto copy = *this;
				copy.data[3] = static_cast<T>(copy.data[3] * (alpha / qpl::f64_cast(max_channel())));
				return copy;
			}
			else {
				qpl::rgbN<T, 4> copy = *this;
				return copy.multiplied_alpha(alpha);
			}
		}

		constexpr qpl::f64 get_perceived_brightness() const {
			if constexpr (qpl::is_same<T, qpl::f64>() && N == 3) {
				return qpl::vectorN<T, 3>(this->data).dot(qpl::vector3d(0.2126, 0.7152, 0.0722));
			}
			else {
				return qpl::rgbN<qpl::f64, 3>(*this).get_perceived_brightness();
			}
		}
		constexpr qpl::f64 get_hue() const requires (N >= 3) {
			auto min = this->min();
			auto max = this->max();
			auto diff = max - min;
			if (!diff) {
				return 0.0;
			}

			if (this->r == max && this->b == min) {
				return ((this->g - min) / qpl::f64_cast(diff)) / 6.0;
			}
			else if (this->g == max && this->b == min) {
				return (1.0 - ((this->r - min) / qpl::f64_cast(diff)) + 1.0) / 6.0;
			}
			else if (this->r == min && this->g == max) {
				return ((this->b - min) / qpl::f64_cast(diff) + 2.0) / 6.0;
			}
			else if (this->r == min && this->b == max) {
				return (1.0 - ((this->g - min) / qpl::f64_cast(diff)) + 3.0) / 6.0;
			}
			else if (this->g == min && this->b == max) {
				return ((this->r - min) / qpl::f64_cast(diff) + 4.0) / 6.0;
			}
			else if (this->r == max && this->g == min) {
				return (1.0 - ((this->b - min) / qpl::f64_cast(diff)) + 5.0) / 6.0;
			}
			return 0.0;
		}

		constexpr void set_hue(qpl::f64 hue)  requires (N >= 3) {
			auto min = this->min();
			auto max = this->max();
			auto diff = max - min;
			if (!diff) {
				return;
			}

			hue = std::fmod(qpl::clamp_0_1(hue), 1.0);
			auto segment = qpl::u32_cast(hue * 6.0) % 6;
			auto strength = (hue * 6.0) - segment;
			if (segment % 2 == 1u) {
				strength = 1.0 - strength;
			}

			T channel;
			if constexpr (is_floating_point()) {
				channel = static_cast<T>(strength * diff) + min;
			}
			else {
				channel = static_cast<T>(std::round(strength * diff)) + min;
			}
			switch (segment) {
			case 0u:
				this->r = max;
				this->g = channel;
				this->b = min;
				break;
			case 1u:
				this->r = channel;
				this->g = max;
				this->b = min;
				break;
			case 2u:
				this->r = min;
				this->g = max;
				this->b = channel;
				break;
			case 3u:
				this->r = min;
				this->g = channel;
				this->b = max;
				break;
			case 4u:
				this->r = channel;
				this->g = min;
				this->b = max;
				break;
			case 5u:
				this->r = max;
				this->g = min;
				this->b = channel;
				break;
			}
		}
		constexpr auto with_hue(qpl::f64 hue) const requires (N >= 3) {
			auto copy = *this;
			copy.set_hue(hue);
			return copy;
		}

		constexpr void add_hue(qpl::f64 delta_hue) requires (N >= 3) {
			this->set_hue(std::fmod(this->get_hue() + delta_hue, 1.0));
		}
		constexpr auto with_added_hue(qpl::f64 delta_hue) const requires (N >= 3) {
			auto copy = *this;
			copy.add_hue(delta_hue);
			return copy;
		}

		constexpr qpl::f64 get_intensity() const requires (N >= 3) {
			auto min = this->min();
			auto max = this->max();
			auto diff = max - min;

			return qpl::f64_cast(diff) / max_channel();
		}

		constexpr qpl::f64 get_perceived_difference(const qpl::rgbN<T, N>& other) const requires (N >= 3) {
			auto h1 = this->get_hue();
			auto h2 = other.get_hue();

			bool swapped = false;
			if (h1 >= h2) {
				swapped = true;
				std::swap(h1, h2);
			}

			auto delta_h1 = qpl::abs(h1 - h2);
			if (h1 < 0.25 || h2 > 0.75) {

				auto delta_h2 = qpl::abs((h1 + 1.0) - h2);
				if (delta_h2 < delta_h1) {
					h1 += 1.0;
				}
			}
			if (swapped) {
				std::swap(h1, h2);
			}
			constexpr auto weights = qpl::vec(1.5, 1.0, 1.7).normalized();

			auto vec1 = qpl::vec3d(h1, this->get_intensity(), this->get_perceived_brightness()) * weights;
			auto vec2 = qpl::vec3d(h2, other.get_intensity(), other.get_perceived_brightness()) * weights;

			return (vec2 - vec1).length();
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

		template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>() && qpl::has_square_brackets_read<C>())
		[[nodiscard]] constexpr static qpl::rgbN<T, N> interpolation(const C& colors, delta_type strength) {
			strength = qpl::clamp<delta_type>(delta_type{ 0.0 }, strength, delta_type{ 1.0 });
			if (strength == 1.0) {
				return colors.back();
			}

			auto index = qpl::size_cast(strength * (colors.size() - 1));
			auto left_over = static_cast<delta_type>(((colors.size() - 1) * strength) - index);

			return colors[index].interpolated(colors[index + 1], left_over);
		}
		template<typename U, qpl::size M, typename C> requires (qpl::is_container<C>() && qpl::has_size<C>() && qpl::has_square_brackets_read<C>())
		[[nodiscard]] constexpr static qpl::rgbN<T, N> interpolation(const C& colors, delta_type strength) {
			return interpolation(qpl::type_cast<qpl::rgbN<T, N>>(colors), strength);
		}
		constexpr std::string string() const {
			bool first = true;
			std::ostringstream stream;
			stream << '(';
			for (qpl::size i = 0u; i < this->data.size(); ++i) {
				if constexpr (N == 4) {
					if (i == 3 && this->data[i] == max_channel()) {
						break;
					}
				}
				if (!first) {
					stream << ", ";
				}
				stream << static_cast<visible_type>(this->data[i]);
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



	namespace detail {
		template<typename C>
		concept has_set_color_c = requires (C & x) {
			x.set_color(qpl::rgba{});
		};
		template<typename C>
		concept has_set_outline_color_c = requires (C & x) {
			x.set_outline_color(qpl::rgba{});
		};

		struct outline_extension {
			qpl::rgba original_outline_color = qpl::rgba::black();
		};
		template<typename T>
		using color_extension_inheritance = qpl::conditional<qpl::if_true<has_set_outline_color_c<T>>, outline_extension, qpl::empty_type>;
	}
	template<typename T> requires detail::has_set_color_c<T>
	struct multiplied_color_extension : T, detail::color_extension_inheritance<T> {
		qpl::rgba original_color = qpl::rgba::white();
		qpl::rgba multiplied_color = qpl::rgba::white();

		multiplied_color_extension() {

		}
		multiplied_color_extension(const multiplied_color_extension& other) = default;
		multiplied_color_extension& operator=(const multiplied_color_extension& value) = default;

		template<typename U>
		multiplied_color_extension(const U& value) : T(value) {

		}
		template<typename U>
		multiplied_color_extension(U&& value) : T(std::move(value)) {

		}
		//template<typename U>
		//multiplied_color_extension& operator=(const U& value) {
		//	*this = multiplied_color_extension{ value };
		//	return *this;
		//}
		//template<typename U>
		//multiplied_color_extension& operator=(U&& value) {
		//	*this = multiplied_color_extension{ std::move(value) };
		//	return *this;
		//}

		void set_color(qpl::rgba color) {
			this->original_color = color;
			T::set_color(this->original_color.multiplied_color(this->multiplied_color));
		}
		void set_outline_color(qpl::rgba color) requires detail::has_set_outline_color_c<T> {
			this->original_outline_color = color;
			T::set_outline_color(this->original_outline_color.multiplied_color(this->multiplied_color));
		}
		void set_multiplied_color(qpl::rgba color) {
			this->multiplied_color = color;
			T::set_color(this->original_color.multiplied_color(this->multiplied_color));
			if constexpr (detail::has_set_outline_color_c<T>) {
				T::set_outline_color(this->original_outline_color.multiplied_color(this->multiplied_color));
			}
		}
		void set_multiplied_alpha(qpl::u8 alpha) {
			this->set_multiplied_color(this->multiplied_color.with_alpha(alpha));
		}
	};


	enum class color : qpl::u32 {
		black = 0,
		blue = 1,
		green = 2,
		aqua = 3,
		red = 4,
		purple = 5,
		yellow = 6,
		white = 7,
		gray = 8,
		light_blue = 9,
		light_green = 10,
		light_aqua = 11,
		light_red = 12,
		light_purple = 13,
		light_yellow = 14,
		bright_white = 15,
		transparent,
		dark_blue,
		pink,
		dark_gray,
	};
	enum class foreground : qpl::u32 {
		black = 0,
		blue = 1,
		green = 2,
		aqua = 3,
		red = 4,
		purple = 5,
		yellow = 6,
		white = 7,
		gray = 8,
		light_blue = 9,
		light_green = 10,
		light_aqua = 11,
		light_red = 12,
		light_purple = 13,
		light_yellow = 14,
		bright_white = 15,
		transparent,
		dark_blue,
		pink,
		dark_gray,
	};
	enum class background : qpl::u32 {
		black = 0,
		blue = 1,
		green = 2,
		aqua = 3,
		red = 4,
		purple = 5,
		yellow = 6,
		white = 7,
		gray = 8,
		light_blue = 9,
		light_green = 10,
		light_aqua = 11,
		light_red = 12,
		light_purple = 13,
		light_yellow = 14,
		bright_white = 15,
		transparent,
		dark_blue,
		pink,
		dark_gray,
	};


	constexpr qpl::foreground black = qpl::foreground::black;
	constexpr qpl::foreground blue = qpl::foreground::blue;
	constexpr qpl::foreground green = qpl::foreground::green;
	constexpr qpl::foreground aqua = qpl::foreground::aqua;
	constexpr qpl::foreground red = qpl::foreground::red;
	constexpr qpl::foreground purple = qpl::foreground::purple;
	constexpr qpl::foreground yellow = qpl::foreground::yellow;
	constexpr qpl::foreground white = qpl::foreground::white;
	constexpr qpl::foreground gray = qpl::foreground::gray;
	constexpr qpl::foreground light_blue = qpl::foreground::light_blue;
	constexpr qpl::foreground light_green = qpl::foreground::light_green;
	constexpr qpl::foreground light_aqua = qpl::foreground::light_aqua;
	constexpr qpl::foreground light_red = qpl::foreground::light_red;
	constexpr qpl::foreground light_purple = qpl::foreground::light_purple;
	constexpr qpl::foreground light_yellow = qpl::foreground::light_yellow;
	constexpr qpl::foreground bright_white = qpl::foreground::bright_white;
	constexpr qpl::foreground pink = qpl::foreground::pink;
	constexpr qpl::foreground dark_gray = qpl::foreground::dark_gray;


	constexpr auto console_colors = std::array{
		/*black,       */ qpl::rgba(12, 12, 12),
		/*blue,        */ qpl::rgba(0, 55, 218),
		/*green,       */ qpl::rgba(19, 161, 14),
		/*aqua,        */ qpl::rgba(58, 150, 221),
		/*red,         */ qpl::rgba(197, 15, 31),
		/*purple,      */ qpl::rgba(136, 23, 152),
		/*yellow,      */ qpl::rgba(193, 156, 0),
		/*white,       */ qpl::rgba(204, 204, 204),
		/*gray,        */ qpl::rgba(118, 118, 118),
		/*light_blue,  */ qpl::rgba(59, 120, 255),
		/*light_green, */ qpl::rgba(22, 198, 12),
		/*light_aqua,  */ qpl::rgba(97, 214, 214),
		/*light_red,   */ qpl::rgba(231, 72, 86),
		/*light_purple,*/ qpl::rgba(180, 0, 158),
		/*light_yellow,*/ qpl::rgba(249, 241, 165),
		/*bright_white,*/ qpl::rgba(242, 242, 242),
		/*transparent, */ qpl::rgba::transparent(),
		/*dark_blue    */ qpl::rgba(0, 15, 75),
		/*pink         */ qpl::rgba(255, 166, 206),
		/*dark-gray,   */ qpl::rgba(83, 83, 83),
	};

	constexpr qpl::rgba background_to_rgb(qpl::background background) {
		if (background == qpl::background::transparent) {
			return qpl::rgba::transparent();
		}
		return qpl::rgba(console_colors[qpl::size_cast(background)]);
	}
	constexpr qpl::rgba foreground_to_rgb(qpl::foreground foreground) {
		return console_colors[qpl::size_cast(foreground)];
	}
	constexpr qpl::foreground rgb_to_foreground(qpl::rgba color) {
		qpl::size index = qpl::size_max;
		qpl::f64 min = qpl::f64_max;
		for (qpl::size i = 0u; i < console_colors.size(); ++i) {
			auto distance = color.get_perceived_difference(console_colors[i]);
			if (min > distance) {
				min = distance;
				index = i;
			}
		}
		return static_cast<qpl::foreground>(index);
	}

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


namespace std {
	template <typename T, qpl::size N>
	struct hash<qpl::rgbN<T, N>> {
		qpl::u64 operator()(const qpl::rgbN<T, N>& k) const {
			std::hash<std::array<T, N>> hash;
			return hash(k.data);
		}
	};
}
#endif