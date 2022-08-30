#include <qpl/color.hpp>


#include <qpl/string.hpp>
#include <qpl/random.hpp>

namespace qpl {

	void qpl::frgba::brighten(qpl::f32 delta) {
		auto acopy = this->a;		
		delta = qpl::clamp_0_1(delta);
		*this = qpl::frgba::white * delta + *this * (1 - delta);
		this->a = acopy;
	}
	qpl::frgba qpl::frgba::brightened(qpl::f32 delta) const {
		auto copy = *this;
		copy.brighten(delta);
		return copy;
	}
	void qpl::frgba::darken(qpl::f32 delta) {
		auto acopy = this->a;
		delta = qpl::clamp_0_1(delta);
		*this = qpl::frgba::black * delta + *this * (1 - delta);
		this->a = acopy;
	}
	qpl::frgba qpl::frgba::darkened(qpl::f32 delta) const {
		auto copy = *this;
		copy.darken(delta);
		return copy;
	}
	void qpl::frgba::light(qpl::f32 delta) {
		if (delta < 0) {
			this->darken(-delta);
		}
		else {
			this->brighten(delta);
		}
	}
	qpl::frgba qpl::frgba::lighted(qpl::f32 delta) const {
		auto copy = *this;
		copy.light(delta);
		return copy;
	}
	void qpl::frgba::intensify(qpl::f32 delta) {
		auto acopy = this->a;
		delta = qpl::clamp_0_1(delta);
		auto min = qpl::min(this->r, this->g, this->b);
		auto max = qpl::max(this->r, this->g, this->b);

		auto diff = max - min;

		auto saturated = (*this - qpl::frgba(min, min, min, 0.0f)) / diff;
		*this = saturated * delta + *this * (1 - delta);
		this->a = acopy;
	}
	qpl::frgba qpl::frgba::intensified(qpl::f32 delta) const {
		auto copy = *this;
		copy.intensify(delta);
		return copy;
	}
	void qpl::frgba::grayify(qpl::f32 delta) {
		auto acopy = this->a;		
		delta = qpl::clamp_0_1(delta);
		auto sum = this->r + this->g + this->b;
		auto grayified = qpl::frgba(sum, sum, sum) / 3.0f;
		*this = grayified * delta + *this * (1 - delta);
		this->a = acopy;
	}
	qpl::frgba qpl::frgba::grayified(qpl::f32 delta) const {
		auto copy = *this;
		copy.grayify(delta);
		return copy;
	}
	void qpl::frgba::saturate(qpl::f32 delta) {
		if (delta < 0) {
			this->grayify(-delta);
		}
		else {
			this->intensify(delta);
		}
	}
	qpl::frgba qpl::frgba::saturated(qpl::f32 delta) const {
		auto copy = *this;
		copy.saturate(delta);
		return copy;
	}
	void qpl::frgba::interpolate(qpl::frgba other, qpl::f32 delta) {
		*this = *this * (1.0 - delta) + (other * delta);
	}
	qpl::frgba qpl::frgba::interpolated(qpl::frgba other, qpl::f32 delta) const {
		auto copy = *this;
		copy.interpolate(other, delta);
		return copy;
	}

	constexpr qpl::frgba qpl::frgba::interpolation(const std::vector<qpl::frgba>& colors, qpl::f64 strength) {
		strength = qpl::clamp(0.0, strength, 1.0);
		if (strength == 1.0) {
			return colors.back();
		}

		auto index = static_cast<qpl::u32>(strength * (colors.size() - 1));
		auto left_over = qpl::f32_cast(((colors.size() - 1) * strength) - index);

		return colors[index].interpolated(colors[index + 1], left_over);
	}
	constexpr qpl::frgba qpl::frgba::interpolation(const std::vector<qpl::rgba>& colors, qpl::f64 strength) {
		return qpl::frgba::interpolation(qpl::type_cast<qpl::frgba>(colors), strength);
	}

	std::string qpl::frgba::string(bool rgb) const {
		if (rgb) {
			return qpl::rgba(*this).string();
		}
		else {
			return qpl::to_string('(', this->r, ", ", this->g, ", ", this->b, ", ", this->a, ')');
		}
	}


	constexpr qpl::frgba& qpl::frgba::operator+=(const qpl::frgba& other) {
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		this->a += other.a;
		return *this;
	}
	constexpr qpl::frgba qpl::frgba::operator+(const qpl::frgba& other) const {
		auto copy = *this;
		copy += other;
		return copy;
	}
	constexpr qpl::frgba& qpl::frgba::operator-=(const qpl::frgba& other) {
		this->r -= other.r;
		this->g -= other.g;
		this->b -= other.b;
		this->a -= other.a;
		return *this;
	}
	constexpr qpl::frgba qpl::frgba::operator-(const qpl::frgba& other) const {
		auto copy = *this;
		copy -= other;
		return copy;
	}
	constexpr qpl::frgba& qpl::frgba::operator/=(const qpl::frgba& other) {
		this->r /= other.r;
		this->g /= other.g;
		this->b /= other.b;
		this->a /= other.a;
		return *this;
	}
	constexpr qpl::frgba qpl::frgba::operator/(const qpl::frgba& other) const {
		auto copy = *this;
		copy /= other;
		return copy;
	}
	constexpr qpl::frgba& qpl::frgba::operator*=(const qpl::frgba& other) {
		this->r *= other.r;
		this->g *= other.g;
		this->b *= other.b;
		this->a *= other.a;
		return *this;
	}
	constexpr qpl::frgba qpl::frgba::operator*(const qpl::frgba& other) const {
		auto copy = *this;
		copy *= other;
		return copy;
	}
	constexpr bool qpl::frgba::operator==(const qpl::frgba& other) const {
		return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
	}
	constexpr bool qpl::frgba::operator!=(const qpl::frgba& other) const {
		return this->r != other.r || this->g != other.g || this->b != other.b || this->a != other.a;
	}

	std::ostream& qpl::operator<<(std::ostream& os, const qpl::frgba& color) {
		return os << color.string();
	}

	void qpl::frgb::brighten(qpl::f32 delta) {
		delta = qpl::clamp_0_1(delta);
		*this = qpl::frgb::white * delta + *this * (1 - delta);
	}
	qpl::frgb qpl::frgb::brightened(qpl::f32 delta) const {
		auto copy = *this;
		copy.brighten(delta);
		return copy;
	}
	void qpl::frgb::darken(qpl::f32 delta) {
		delta = qpl::clamp_0_1(delta);
		*this = qpl::frgb::black * delta + *this * (1 - delta);
	}
	qpl::frgb qpl::frgb::darkened(qpl::f32 delta) const {
		auto copy = *this;
		copy.darken(delta);
		return copy;
	}
	void qpl::frgb::light(qpl::f32 delta) {
		if (delta < 0) {
			this->darken(-delta);
		}
		else {
			this->brighten(delta);
		}
	}
	qpl::frgb qpl::frgb::lighted(qpl::f32 delta) const {
		auto copy = *this;
		copy.light(delta);
		return copy;
	}
	void qpl::frgb::intensify(qpl::f32 delta) {
		delta = qpl::clamp_0_1(delta);
		auto min = qpl::min(this->r, this->g, this->b);
		auto max = qpl::max(this->r, this->g, this->b);

		auto diff = max - min;

		auto saturated = (*this - qpl::frgb(min, min, min)) / diff;
		*this = saturated * delta + *this * (1 - delta);
	}
	qpl::frgb qpl::frgb::intensified(qpl::f32 delta) const {
		auto copy = *this;
		copy.intensify(delta);
		return copy;
	}
	void qpl::frgb::grayify(qpl::f32 delta) {
		delta = qpl::clamp_0_1(delta);
		auto sum = this->r + this->g + this->b;
		auto grayified = qpl::frgb(sum, sum, sum) / 3.0f;
		*this = grayified * delta + *this * (1 - delta);
	}
	qpl::frgb qpl::frgb::grayified(qpl::f32 delta) const {
		auto copy = *this;
		copy.grayify(delta);
		return copy;
	}
	void qpl::frgb::saturate(qpl::f32 delta) {
		if (delta < 0) {
			this->grayify(-delta);
		}
		else {
			this->intensify(delta);
		}
	}
	qpl::frgb qpl::frgb::saturated(qpl::f32 delta) const {
		auto copy = *this;
		copy.saturate(delta);
		return copy;
	}
	void qpl::frgb::interpolate(qpl::frgb other, qpl::f32 delta) {
		*this = *this * (1.0 - delta) + (other * delta);
	}
	qpl::frgb qpl::frgb::interpolated(qpl::frgb other, qpl::f32 delta) const {
		auto copy = *this;
		copy.interpolate(other, delta);
		return copy;
	}

	constexpr qpl::frgb qpl::frgb::interpolation(const std::vector<qpl::frgb>& colors, qpl::f64 strength) {
		strength = qpl::clamp(0.0, strength, 1.0);
		if (strength == 1.0) {
			return colors.back();
		}

		auto index = static_cast<qpl::u32>(strength * (colors.size() - 1));
		auto left_over = qpl::f32_cast(((colors.size() - 1) * strength) - index);

		return colors[index].interpolated(colors[index + 1], left_over);
	}
	constexpr qpl::frgb qpl::frgb::interpolation(const std::vector<qpl::rgba>& colors, qpl::f64 strength) {
		return qpl::frgb::interpolation(qpl::type_cast<qpl::frgb>(colors), strength);
	}

	std::string qpl::frgb::string(bool rgb) const {
		if (rgb) {
			return qpl::rgba(*this).string();
		}
		else {
			return qpl::to_string('(', this->r, ", ", this->g, ", ", this->b, ')');
		}
	}


	constexpr qpl::frgb& qpl::frgb::operator+=(const qpl::frgb& other) {
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		return *this;
	}
	constexpr qpl::frgb qpl::frgb::operator+(const qpl::frgb& other) const {
		auto copy = *this;
		copy += other;
		return copy;
	}
	constexpr qpl::frgb& qpl::frgb::operator-=(const qpl::frgb& other) {
		this->r -= other.r;
		this->g -= other.g;
		this->b -= other.b;
		return *this;
	}
	constexpr qpl::frgb qpl::frgb::operator-(const qpl::frgb& other) const {
		auto copy = *this;
		copy -= other;
		return copy;
	}
	constexpr qpl::frgb& qpl::frgb::operator/=(const qpl::frgb& other) {
		this->r /= other.r;
		this->g /= other.g;
		this->b /= other.b;
		return *this;
	}
	constexpr qpl::frgb qpl::frgb::operator/(const qpl::frgb& other) const {
		auto copy = *this;
		copy /= other;
		return copy;
	}
	constexpr qpl::frgb& qpl::frgb::operator*=(const qpl::frgb& other) {
		this->r *= other.r;
		this->g *= other.g;
		this->b *= other.b;
		return *this;
	}
	constexpr qpl::frgb qpl::frgb::operator*(const qpl::frgb& other) const {
		auto copy = *this;
		copy *= other;
		return copy;
	}
	constexpr bool qpl::frgb::operator==(const qpl::frgb& other) const {
		return this->r == other.r && this->g == other.g && this->b == other.b;
	}
	constexpr bool qpl::frgb::operator!=(const qpl::frgb& other) const {
		return this->r != other.r || this->g != other.g || this->b != other.b;
	}
	std::ostream& qpl::operator<<(std::ostream& os, const qpl::frgb& color) {
		return os << color.string();
	}

#if defined QPL_INTERN_SFML_USE
	qpl::rgba& qpl::rgba::operator=(sf::Color color) {
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
		this->a = color.a;
		return *this;
	}
	qpl::rgba::operator sf::Color() const {
		sf::Color color;
		color.r = this->r;
		color.g = this->g;
		color.b = this->b;
		color.a = this->a;
		return color;
	}

#endif
	constexpr qpl::rgba& qpl::rgba::operator=(qpl::u32 uint) {
		if (uint <= (qpl::u32_max >> 8)) {
			uint <<= 8;
			uint |= 0xFFu;
		}

		this->r = (uint & 0xFF000000u) >> 24;
		this->g = (uint & 0x00FF0000u) >> 16;
		this->b = (uint & 0x0000FF00u) >> 8;
		this->a = (uint & 0x000000FFu);
		return *this;
	}
	constexpr qpl::rgba& qpl::rgba::operator=(const qpl::rgb& rgb) {
		this->r = rgb.r;
		this->g = rgb.g;
		this->b = rgb.b;
		this->a = 255u;
		return *this;
	}
	constexpr qpl::rgba& qpl::rgba::operator=(const qpl::frgb& rgb) {
		this->r = qpl::u8_cast(qpl::clamp(0.0f, rgb.r * 255, 255.0f));
		this->g = qpl::u8_cast(qpl::clamp(0.0f, rgb.g * 255, 255.0f));
		this->b = qpl::u8_cast(qpl::clamp(0.0f, rgb.b * 255, 255.0f));
		this->a = 255u;
		return *this;
	}
	constexpr qpl::rgba& qpl::rgba::operator=(const qpl::frgba& rgb) {
		this->r = qpl::u8_cast(qpl::clamp(0.0f, rgb.r * 255, 255.0f));
		this->g = qpl::u8_cast(qpl::clamp(0.0f, rgb.g * 255, 255.0f));
		this->b = qpl::u8_cast(qpl::clamp(0.0f, rgb.b * 255, 255.0f));
		this->a = qpl::u8_cast(qpl::clamp(0.0f, rgb.a * 255, 255.0f));
		return *this;
	}
	constexpr bool qpl::rgba::operator==(const qpl::rgba& other) const {
		if (std::is_constant_evaluated()) {
			return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
		}
		else {
			return this->uint() == other.uint();
		}
	}

	std::string qpl::rgba::string() const {
		std::ostringstream stream;
		stream << '(';
		stream << static_cast<qpl::i16>(this->r) << ", ";
		stream << static_cast<qpl::i16>(this->g) << ", ";
		stream << static_cast<qpl::i16>(this->b);
		if (this->a != qpl::u8_max) {
			stream << ", " << static_cast<qpl::i16>(this->a);
		}
		stream << ')';
		return stream.str();
	}
	std::string qpl::rgba::hex_string(bool prefix) const {
		std::ostringstream stream;
		if (prefix) {
			stream << "0x";
		}
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->r, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->g, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->b, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);

		if (this->a != qpl::u8_max) {
			stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->a, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		}
		return stream.str();
	}
	constexpr bool qpl::rgba::is_unset() const {
		return (*this == qpl::rgba::unset);
	}
	constexpr qpl::u32 qpl::rgba::uint() const {
		if (std::is_constant_evaluated()) {
			qpl::u32 n = (qpl::u32_cast(this->r) << 24) | (qpl::u32_cast(this->g) << 16) | (qpl::u32_cast(this->b) << 8) | (qpl::u32_cast(this->a));

			return n;
		}
		else {
			qpl::u32 n;
			memcpy(&n, this, sizeof(qpl::u32));
			return n;
		}
	}

	void qpl::rgba::brighten(qpl::f64 delta) {
		*this = qpl::frgba(*this).brightened(qpl::f32_cast(delta));
	}
	qpl::rgba qpl::rgba::brightened(qpl::f64 delta) const {
		return qpl::frgba(*this).brightened(qpl::f32_cast(delta));
	}

	void qpl::rgba::darken(qpl::f64 delta) {
		*this = qpl::frgba(*this).darkened(qpl::f32_cast(delta));
	}
	qpl::rgba qpl::rgba::darkened(qpl::f64 delta) const {
		return qpl::frgba(*this).darkened(qpl::f32_cast(delta));
	}

	void qpl::rgba::light(qpl::f64 delta) {
		*this = qpl::frgba(*this).lighted(qpl::f32_cast(delta));
	}
	qpl::rgba qpl::rgba::lighted(qpl::f64 delta) const {
		return qpl::frgba(*this).lighted(qpl::f32_cast(delta));
	}

	void qpl::rgba::intensify(qpl::f64 delta) {
		*this = qpl::frgba(*this).intensified(qpl::f32_cast(delta));
	}
	qpl::rgba qpl::rgba::intensified(qpl::f64 delta) const {
		return qpl::frgba(*this).intensified(qpl::f32_cast(delta));
	}

	void qpl::rgba::grayify(qpl::f64 delta) {
		*this = qpl::frgba(*this).grayified(qpl::f32_cast(delta));
	}
	qpl::rgba qpl::rgba::grayified(qpl::f64 delta) const {
		return qpl::frgba(*this).grayified(qpl::f32_cast(delta));
	}

	void qpl::rgba::saturate(qpl::f64 delta) {
		*this = qpl::frgba(*this).saturated(qpl::f32_cast(delta));
	}
	qpl::rgba qpl::rgba::saturated(qpl::f64 delta) const {
		return qpl::frgba(*this).saturated(qpl::f32_cast(delta));
	}


	constexpr qpl::rgba& qpl::rgba::interpolate(qpl::rgba color, qpl::f64 strength) {
		strength = qpl::clamp(qpl::f64{ 0 }, strength, qpl::f64{ 1 });
		this->r = static_cast<qpl::u8>((this->r * (1 - strength) + color.r * strength));
		this->g = static_cast<qpl::u8>((this->g * (1 - strength) + color.g * strength));
		this->b = static_cast<qpl::u8>((this->b * (1 - strength) + color.b * strength));
		this->a = static_cast<qpl::u8>((this->a * (1 - strength) + color.a * strength));
		return *this;
	}
	constexpr qpl::rgba qpl::rgba::interpolated(qpl::rgba color, qpl::f64 strength) const {
		auto copy = *this;
		copy.interpolate(color, strength);
		return copy;
	}
	constexpr qpl::rgba qpl::rgba::interpolation(const std::vector<qpl::rgba>& colors, qpl::f64 strength) {
		strength = qpl::clamp(0.0, strength, 1.0);
		if (strength == 1.0) {
			return colors.back();
		}

		auto index = static_cast<qpl::u32>(strength * (colors.size() - 1));
		auto left_over = ((colors.size() - 1) * strength) - index;

		return colors[index].interpolated(colors[index + 1], left_over);
	}

	constexpr qpl::rgba& qpl::rgba::invert() {
		this->r = 255 - this->r;
		this->g = 255 - this->g;
		this->b = 255 - this->b;
		return *this;
	}
	constexpr qpl::rgba qpl::rgba::inverted() const {
		auto copy = *this;
		copy.invert();
		return copy;
	}
	constexpr qpl::rgba qpl::rgba::with_alpha(qpl::u8 alpha) const {
		auto copy = *this;
		copy.a = alpha;
		return copy;
	}
	constexpr qpl::rgba qpl::rgba::multiplied_alpha(qpl::u8 alpha) const {
		auto copy = *this;
		copy.a = qpl::u8_cast(qpl::f32_cast(this->a) * (qpl::f32_cast(alpha) / 255.f));
		return copy;
	}
	constexpr qpl::rgba qpl::rgba::multiplied_color(qpl::rgba color) const {
		if (color == qpl::rgba::white) {
			return *this;
		}
		if (*this == qpl::rgba::white) {
			return color;
		}
		auto copy = *this;
		copy.r = qpl::u8_cast(qpl::f32_cast(this->r) * (qpl::f32_cast(color.r) / 255.f));
		copy.g = qpl::u8_cast(qpl::f32_cast(this->g) * (qpl::f32_cast(color.g) / 255.f));
		copy.b = qpl::u8_cast(qpl::f32_cast(this->b) * (qpl::f32_cast(color.b) / 255.f));
		copy.a = qpl::u8_cast(qpl::f32_cast(this->a) * (qpl::f32_cast(color.a) / 255.f));
		return copy;
	}

	constexpr qpl::rgba& qpl::rgba::operator*=(qpl::rgba other) {
			this->r *= other.r;
			this->g *= other.g;
			this->b *= other.b;
			return *this;
		}
	constexpr qpl::rgba qpl::rgba::operator*(qpl::rgba other) const {
			auto copy = *this;
			copy *= other;
			return copy;
		}
	constexpr qpl::rgba& qpl::rgba::operator-=(qpl::rgba other) {
			this->r -= other.r;
			this->g -= other.g;
			this->b -= other.b;
			return *this;
		}
	constexpr qpl::rgba qpl::rgba::operator-(qpl::rgba other) const {
			auto copy = *this;
			copy -= other;
			return copy;
		}
	constexpr qpl::rgba& qpl::rgba::operator/=(qpl::rgba other) {
			this->r /= other.r;
			this->g /= other.g;
			this->b /= other.b;
			return *this;
		}
	constexpr qpl::rgba qpl::rgba::operator/(qpl::rgba other) const {
			auto copy = *this;
			copy /= other;
			return copy;
		}
	constexpr qpl::rgba& qpl::rgba::operator+=(qpl::rgba other) {
			this->r += other.r;
			this->g += other.g;
			this->b += other.b;
			return *this;
		}
	constexpr qpl::rgba qpl::rgba::operator+(qpl::rgba other) const {
		auto copy = *this;
		copy += other;
		return copy;
	}
	std::ostream& qpl::operator<<(std::ostream& os, const qpl::rgba& color) {
		return os << color.string();
	}

#if defined QPL_INTERN_SFML_USE
	qpl::rgb& qpl::rgb::operator=(sf::Color color) {
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
		return *this;
	}
	qpl::rgb::operator sf::Color() const {
		sf::Color color;
		color.r = this->r;
		color.g = this->g;
		color.b = this->b;
		return color;
	}

#endif
	constexpr qpl::rgb& qpl::rgb::operator=(qpl::u32 uint) {
		if (uint <= (qpl::u32_max >> 8)) {
			uint <<= 8;
			uint |= 0xFFu;
		}

		this->r = (uint & 0x00FF0000u) >> 16;
		this->g = (uint & 0x0000FF00u) >> 8;
		this->b = (uint & 0x000000FFu);
		return *this;
	}
	constexpr qpl::rgb& qpl::rgb::operator=(const qpl::rgba& rgb) {
		this->r = rgb.r;
		this->g = rgb.g;
		this->b = rgb.b;
		return *this;
	}
	constexpr qpl::rgb& qpl::rgb::operator=(const qpl::frgb& rgb) {
		this->r = qpl::u8_cast(qpl::clamp(0.0f, rgb.r * 255, 255.0f));
		this->g = qpl::u8_cast(qpl::clamp(0.0f, rgb.g * 255, 255.0f));
		this->b = qpl::u8_cast(qpl::clamp(0.0f, rgb.b * 255, 255.0f));
		return *this;
	}
	constexpr qpl::rgb& qpl::rgb::operator=(const qpl::frgba& rgb) {
		this->r = qpl::u8_cast(qpl::clamp(0.0f, rgb.r * 255, 255.0f));
		this->g = qpl::u8_cast(qpl::clamp(0.0f, rgb.g * 255, 255.0f));
		this->b = qpl::u8_cast(qpl::clamp(0.0f, rgb.b * 255, 255.0f));
		return *this;
	}
	constexpr bool qpl::rgb::operator==(const qpl::rgb& other) const {
		if (std::is_constant_evaluated()) {
			return this->r == other.r && this->g == other.g && this->b == other.b;
		}
		else {
			return this->uint() == other.uint();
		}
	}

	std::string qpl::rgb::string() const {
		std::ostringstream stream;
		stream << '(';
		stream << static_cast<qpl::i16>(this->r) << ", ";
		stream << static_cast<qpl::i16>(this->g) << ", ";
		stream << static_cast<qpl::i16>(this->b);
		stream << ')';
		return stream.str();
	}
	std::string qpl::rgb::hex_string(bool prefix) const {
		std::ostringstream stream;
		if (prefix) {
			stream << "0x";
		}
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->r, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->g, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->b, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		return stream.str();
	}
	constexpr qpl::u32 qpl::rgb::uint() const {
		if (std::is_constant_evaluated()) {
			qpl::u32 n = (qpl::u32_cast(this->r) << 16) | (qpl::u32_cast(this->g) << 8) | (qpl::u32_cast(this->b));

			return n;
		}
		else {
			qpl::u32 n = 0u;
			memcpy(&n, this, sizeof(qpl::u32));
			return n;
		}
	}

	void qpl::rgb::brighten(qpl::f64 delta) {
		*this = qpl::frgba(*this).brightened(qpl::f32_cast(delta));
	}
	qpl::rgb qpl::rgb::brightened(qpl::f64 delta) const {
		return qpl::frgba(*this).brightened(qpl::f32_cast(delta));
	}

	void qpl::rgb::darken(qpl::f64 delta) {
		*this = qpl::frgba(*this).darkened(qpl::f32_cast(delta));
	}
	qpl::rgb qpl::rgb::darkened(qpl::f64 delta) const {
		return qpl::frgba(*this).darkened(qpl::f32_cast(delta));
	}

	void qpl::rgb::light(qpl::f64 delta) {
		*this = qpl::frgba(*this).lighted(qpl::f32_cast(delta));
	}
	qpl::rgb qpl::rgb::lighted(qpl::f64 delta) const {
		return qpl::frgba(*this).lighted(qpl::f32_cast(delta));
	}

	void qpl::rgb::intensify(qpl::f64 delta) {
		*this = qpl::frgba(*this).intensified(qpl::f32_cast(delta));
	}
	qpl::rgb qpl::rgb::intensified(qpl::f64 delta) const {
		return qpl::frgba(*this).intensified(qpl::f32_cast(delta));
	}

	void qpl::rgb::grayify(qpl::f64 delta) {
		*this = qpl::frgba(*this).grayified(qpl::f32_cast(delta));
	}
	qpl::rgb qpl::rgb::grayified(qpl::f64 delta) const {
		return qpl::frgba(*this).grayified(qpl::f32_cast(delta));
	}

	void qpl::rgb::saturate(qpl::f64 delta) {
		*this = qpl::frgba(*this).saturated(qpl::f32_cast(delta));
	}
	qpl::rgb qpl::rgb::saturated(qpl::f64 delta) const {
		return qpl::frgba(*this).saturated(qpl::f32_cast(delta));
	}


	constexpr qpl::rgb& qpl::rgb::interpolate(qpl::rgb color, qpl::f64 strength) {
		strength = qpl::clamp(qpl::f64{ 0 }, strength, qpl::f64{ 1 });
		this->r = static_cast<qpl::u8>((this->r * (1 - strength) + color.r * strength));
		this->g = static_cast<qpl::u8>((this->g * (1 - strength) + color.g * strength));
		this->b = static_cast<qpl::u8>((this->b * (1 - strength) + color.b * strength));
		return *this;
	}
	constexpr qpl::rgb qpl::rgb::interpolated(qpl::rgb color, qpl::f64 strength) const {
		auto copy = *this;
		copy.interpolate(color, strength);
		return copy;
	}
	constexpr qpl::rgb qpl::rgb::interpolation(const std::vector<qpl::rgb>& colors, qpl::f64 strength) {
		strength = qpl::clamp(0.0, strength, 1.0);
		if (strength == 1.0) {
			return colors.back();
		}

		auto index = static_cast<qpl::u32>(strength * (colors.size() - 1));
		auto left_over = ((colors.size() - 1) * strength) - index;

		return colors[index].interpolated(colors[index + 1], left_over);
	}

	constexpr qpl::rgb& qpl::rgb::invert() {
		this->r = 255 - this->r;
		this->g = 255 - this->g;
		this->b = 255 - this->b;
		return *this;
	}
	constexpr qpl::rgb qpl::rgb::inverted() const {
		auto copy = *this;
		copy.invert();
		return copy;
	}
	constexpr qpl::rgb qpl::rgb::multiplied_color(qpl::rgb color) const {
		if (color == qpl::rgb::white) {
			return *this;
		}
		if (*this == qpl::rgb::white) {
			return color;
		}
		auto copy = *this;
		copy.r = qpl::u8_cast(qpl::f32_cast(this->r) * (qpl::f32_cast(color.r) / 255.f));
		copy.g = qpl::u8_cast(qpl::f32_cast(this->g) * (qpl::f32_cast(color.g) / 255.f));
		copy.b = qpl::u8_cast(qpl::f32_cast(this->b) * (qpl::f32_cast(color.b) / 255.f));
		return copy;
	}

	constexpr qpl::rgb& qpl::rgb::operator*=(qpl::rgb other) {
		this->r *= other.r;
		this->g *= other.g;
		this->b *= other.b;
		return *this;
	}
	constexpr qpl::rgb qpl::rgb::operator*(qpl::rgb other) const {
		auto copy = *this;
		copy *= other;
		return copy;
	}
	constexpr qpl::rgb& qpl::rgb::operator-=(qpl::rgb other) {
		this->r -= other.r;
		this->g -= other.g;
		this->b -= other.b;
		return *this;
	}
	constexpr qpl::rgb qpl::rgb::operator-(qpl::rgb other) const {
		auto copy = *this;
		copy -= other;
		return copy;
	}
	constexpr qpl::rgb& qpl::rgb::operator/=(qpl::rgb other) {
		this->r /= other.r;
		this->g /= other.g;
		this->b /= other.b;
		return *this;
	}
	constexpr qpl::rgb qpl::rgb::operator/(qpl::rgb other) const {
		auto copy = *this;
		copy /= other;
		return copy;
	}
	constexpr qpl::rgb& qpl::rgb::operator+=(qpl::rgb other) {
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		return *this;
	}
	constexpr qpl::rgb qpl::rgb::operator+(qpl::rgb other) const {
		auto copy = *this;
		copy += other;
		return copy;
	}
	std::ostream& qpl::operator<<(std::ostream& os, const qpl::rgb& color) {
		return os << color.string();
	}

	qpl::rgb qpl::get_random_color() {
		auto value = qpl::random((1u << 24) - 1);
		return qpl::rgba(value);
	}
	qpl::rgba qpl::get_random_transparency_color() {
		return qpl::rgb(qpl::random(qpl::u32_max));
	}
	qpl::rgb qpl::get_rainbow_color(qpl::f64 f) {
		const static std::vector<qpl::rgb> rainbow = { qpl::rgb::red, qpl::rgb::yellow, qpl::rgb::green, qpl::rgb::cyan, qpl::rgb::blue, qpl::rgb::magenta, qpl::rgb::red };
		return qpl::rgb::interpolation(rainbow, f);
	}
	qpl::rgb qpl::get_random_rainbow_color() {
		return qpl::get_rainbow_color(qpl::random(0.0, 1.0));
	}
}

#if defined(QPL_USE_VULKAN) || defined(QPL_USE_ALL)
#include <qpl/random.hpp>
#include <qpl/string.hpp>

namespace qpl::vk {
	const qpl::vk::frgba qpl::vk::frgba::red = qpl::vk::frgba(1.0f, 0.0f, 0.0f);
	const qpl::vk::frgba qpl::vk::frgba::green = qpl::vk::frgba(0.0f, 1.0f, 0.0f);
	const qpl::vk::frgba qpl::vk::frgba::blue = qpl::vk::frgba(0.5f, 0.5f, 1.0f);
	const qpl::vk::frgba qpl::vk::frgba::black = qpl::vk::frgba(0.0f, 0.0f, 0.0f);
	const qpl::vk::frgba qpl::vk::frgba::white = qpl::vk::frgba(1.0f, 1.0f, 1.0f);
	const qpl::vk::frgba qpl::vk::frgba::yellow = qpl::vk::frgba(1.0f, 1.0f, 0.0f);
	const qpl::vk::frgba qpl::vk::frgba::magenta = qpl::vk::frgba(1.0f, 0.0f, 1.0f);
	const qpl::vk::frgba qpl::vk::frgba::cyan = qpl::vk::frgba(0.0f, 1.0f, 1.0f);
	const qpl::vk::frgba qpl::vk::frgba::orange = qpl::vk::frgba(1.0f, 0.5f, 0.0f);
	const qpl::vk::frgba qpl::vk::frgba::purple = qpl::vk::frgba(0.5f, 0.0f, 1.0f);
	const qpl::vk::frgba qpl::vk::frgba::amaranth = qpl::vk::frgba(1.0f, 0.0f, 0.5f);
	const qpl::vk::frgba qpl::vk::frgba::turquoise = qpl::vk::frgba(0.0f, 1.0f, 0.5f);

	qpl::vk::frgba& qpl::vk::frgba::operator=(const qpl::vk::frgba& other) {
		this->m_rgb = other.m_rgb;
		return *this;
	}
	qpl::vk::frgba& qpl::vk::frgba::operator=(const glm::vec3& other) {
		this->m_rgb = other;
		return *this;
	}
	bool qpl::vk::frgba::operator==(const qpl::vk::frgba& other) const {
		return this->m_rgb == other.m_rgb;
	}
	bool qpl::vk::frgba::operator==(const glm::vec3& other) const {
		return this->m_rgb == other;
	}

	std::string qpl::vk::frgba::string() const {
		return qpl::to_string("(", this->m_rgb.r, ", ", this->m_rgb.g, ", ", this->m_rgb.b, ")");
	}
	qpl::vk::frgba::operator glm::vec3() const {
		return this->get();
	}
	glm::vec3 qpl::vk::frgba::get() const {
		return this->m_rgb;
	}


	void qpl::vk::frgba::brighten(qpl::f32 delta) {
		this->m_rgb = qpl::vk::frgba::white.get() * delta + this->m_rgb * (1 - delta);
	}
	qpl::vk::frgba qpl::vk::frgba::brightened(qpl::f32 delta) const {
		auto copy = *this;
		copy.brighten(delta);
		return copy;
	}

	void qpl::vk::frgba::darken(qpl::f32 delta) {
		this->m_rgb = qpl::vk::frgba::black.get() * delta + this->m_rgb * (1 - delta);
	}
	qpl::vk::frgba qpl::vk::frgba::darkened(qpl::f32 delta) const {
		auto copy = *this;
		copy.darken(delta);
		return copy;
	}



	void qpl::vk::frgba::light(qpl::f32 delta) {
		if (delta < 0) {
			this->darken(-delta);
		}
		else {
			this->brighten(delta);
		}
	}
	qpl::vk::frgba qpl::vk::frgba::lighted(qpl::f32 delta) const {
		auto copy = *this;
		copy.light(delta);
		return copy;
	}


	void qpl::vk::frgba::intensify(qpl::f32 delta) {
		delta = qpl::min(delta, 1.0f);
		auto min = qpl::min(this->m_rgb.r, this->m_rgb.g, this->m_rgb.b);
		auto max = qpl::max(this->m_rgb.r, this->m_rgb.g, this->m_rgb.b);

		auto diff = max - min;
		auto saturated = (this->m_rgb - glm::vec3(min)) / diff;
		this->m_rgb = saturated * delta + this->m_rgb * (1 - delta);
	}
	qpl::vk::frgba qpl::vk::frgba::intensified(qpl::f32 delta) const {
		auto copy = *this;
		copy.intensify(delta);
		return copy;
	}

	void qpl::vk::frgba::grayify(qpl::f32 delta) {
		delta = qpl::min(delta, 1.0f);
		auto sum = this->m_rgb.r + this->m_rgb.g + this->m_rgb.b;
		auto grayified = glm::vec3(sum) / 3.0f;
		this->m_rgb = grayified * delta + this->m_rgb * (1 - delta);
	}
	qpl::vk::frgba qpl::vk::frgba::grayified(qpl::f32 delta) const {
		auto copy = *this;
		copy.grayify(delta);
		return copy;
	}


	void qpl::vk::frgba::saturate(qpl::f32 delta) {
		if (delta < 0) {
			this->grayify(-delta);
		}
		else {
			this->intensify(delta);
		}
	}
	qpl::vk::frgba qpl::vk::frgba::saturated(qpl::f32 delta) const {
		auto copy = *this;
		copy.saturate(delta);
		return copy;
	}

	void qpl::vk::frgba::black_gray_intense_white(qpl::f32 delta) {
		if (delta < -0.5f) {
			this->grayify(1);
			this->darken((-delta - 0.5f) * 2);
		}
		else if (delta < 0.0f) {
			this->grayify(-delta * 2.0f);
		}
		else if (delta < 0.5f) {
			this->intensify(delta * 2.0f);
		}
		else {
			this->intensify(1);
			this->brighten((delta - 0.5f) * 2.0f);
		}
	}
	qpl::vk::frgba qpl::vk::frgba::black_gray_intense_whited(qpl::f32 delta) const {
		auto copy = *this;
		copy.black_gray_intense_white(delta);
		return copy;
	}


	void qpl::vk::frgba::black_intense_white(qpl::f32 delta) {
		if (delta < 0.0f) {
			this->darken((-delta));
		}
		else if (delta < 0.5f) {
			this->intensify(delta * 2.0f);
		}
		else {
			this->intensify(1);
			this->brighten((delta - 0.5f) * 2.0f);
		}
	}
	qpl::vk::frgba qpl::vk::frgba::black_intense_whited(qpl::f32 delta) const {
		auto copy = *this;
		copy.black_intense_white(delta);
		return copy;
	}



	void qpl::vk::frgba::interpolate(qpl::vk::frgba other, qpl::f32 delta) {
		this->m_rgb = this->m_rgb * (1 - delta) + other.m_rgb * delta;
	}
	qpl::vk::frgba qpl::vk::frgba::interpolated(qpl::vk::frgba other, qpl::f32 delta) const {
		auto copy = *this;
		copy.interpolate(other, delta);
		return copy;
	}

	qpl::vk::frgba qpl::random_color() {
		return qpl::vk::frgba({
			static_cast<qpl::f32>(qpl::random_f(0.0f, 1.0f)),
			static_cast<qpl::f32>(qpl::random_f(0.0f, 1.0f)),
			static_cast<qpl::f32>(qpl::random_f(0.0f, 1.0f)),
			});
	}
}
#endif