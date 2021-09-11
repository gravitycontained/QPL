#include <qpl/QSF/color.hpp>

#ifndef QPL_NO_SFML

#include <qpl/string.hpp>
#include <qpl/random.hpp>

namespace qsf {

	frgb& qsf::frgb::operator=(const rgb& other) {
		if (other == qsf::rgb::unset) {
			*this = qsf::frgb::unset;
		}
		else {
			this->r = other.r / 255.0f;
			this->g = other.g / 255.0f;
			this->b = other.b / 255.0f;
			this->a = other.a / 255.0f;
		}
		return *this;
	}
	frgb& qsf::frgb::operator=(const frgb& other) {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->a = other.a;
		return *this;
	}
	std::string qsf::frgb::string() const {
		return qpl::to_string('(', this->r, ", ", this->g, ", ", this->b, ", ", this->a, ')');
	}


	qsf::frgb& qsf::frgb::operator+=(const qsf::frgb& other) {
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		this->a += other.a;
		return *this;
	}
	qsf::frgb qsf::frgb::operator+(const qsf::frgb& other) const {
		auto copy = *this;
		copy += other;
		return copy;
	}
	qsf::frgb& qsf::frgb::operator-=(const qsf::frgb& other) {
		this->r -= other.r;
		this->g -= other.g;
		this->b -= other.b;
		this->a -= other.a;
		return *this;
	}
	qsf::frgb qsf::frgb::operator-(const qsf::frgb& other) const {
		auto copy = *this;
		copy -= other;
		return copy;
	}
	qsf::frgb& qsf::frgb::operator/=(const qsf::frgb& other) {
		this->r /= other.r;
		this->g /= other.g;
		this->b /= other.b;
		this->a /= other.a;
		return *this;
	}
	qsf::frgb qsf::frgb::operator/(const qsf::frgb& other) const {
		auto copy = *this;
		copy /= other;
		return copy;
	}
	qsf::frgb& qsf::frgb::operator*=(const qsf::frgb& other) {
		this->r *= other.r;
		this->g *= other.g;
		this->b *= other.b;
		this->a *= other.a;
		return *this;
	}
	qsf::frgb qsf::frgb::operator*(const qsf::frgb& other) const {
		auto copy = *this;
		copy *= other;
		return copy;
	}
	bool qsf::frgb::operator==(const qsf::frgb& other) const {
		return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
	}
	bool qsf::frgb::operator!=(const qsf::frgb& other) const {
		return this->r != other.r || this->g != other.g || this->b != other.b || this->a != other.a;
	}



	const qsf::frgb qsf::frgb::unset = qsf::frgb(-1.0f, -1.0f, -1.0f, -1.0f);

	qsf::rgb& qsf::rgb::operator=(const rgb& other) {
		memcpy(this, &other, sizeof(rgb));
		return *this;
	}
	qsf::rgb& qsf::rgb::operator=(const frgb& other) {
		if (other == qsf::frgb::unset) {
			*this = qsf::rgb::unset;
		}
		else {
			this->r = static_cast<qpl::u8>(qpl::clamp(qpl::f32{}, other.r, qpl::f32{ 1 }) * 255);
			this->g = static_cast<qpl::u8>(qpl::clamp(qpl::f32{}, other.g, qpl::f32{ 1 }) * 255);
			this->b = static_cast<qpl::u8>(qpl::clamp(qpl::f32{}, other.b, qpl::f32{ 1 }) * 255);
			this->a = static_cast<qpl::u8>(qpl::clamp(qpl::f32{}, other.a, qpl::f32{ 1 }) * 255);
		}
		return *this;
	}
	qsf::rgb& qsf::rgb::operator=(sf::Color color) {
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
		this->a = color.a;
		return *this;
	}
	qsf::rgb& qsf::rgb::operator=(qpl::u32 uint) {

		if (uint <= (qpl::u32_max >> 8)) {
			uint <<= 8;
			uint |= 0xFFu;
		}

		qpl::u32 n = ((uint & 0xFF000000u) >> 24) | ((uint & 0x000000FFu) << 24) | ((uint & 0x00FF0000u) >> 8) | ((uint & 0x0000FF00u) << 8);
		memcpy(this, &n, sizeof(qpl::u32));
		return *this;
	}
	bool qsf::rgb::operator==(const rgb& other) const {
		return this->uint() == other.uint();
	}

	qsf::rgb::operator sf::Color() const {
		sf::Color color;
		color.r = this->r;
		color.g = this->g;
		color.b = this->b;
		color.a = this->a;
		return color;
	}

	std::string qsf::rgb::string() const {
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
	std::string qsf::rgb::hex_string() const {
		std::ostringstream stream;
		stream << "0x";
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->r, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->g, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->b, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);  //0 -> "00"

		if (this->a != qpl::u8_max) {
			stream << qpl::prepended_to_string_to_fit(qpl::base_string(this->a, qpl::u8{ 16 }, "", qpl::base_format::base36u), '0', 2);
		}
		return stream.str();
	}
	bool qsf::rgb::is_unset() const {
		return (*this == qsf::rgb::unset);
	}
	qpl::u32 qsf::rgb::uint() const {
		qpl::u32 n;
		memcpy(&n, this, sizeof(qpl::u32));
		return n;
	}


	qsf::rgb& qsf::rgb::interpolate(qsf::rgb color, qpl::f64 strength) {
		strength = qpl::clamp(qpl::f64{ 0 }, strength, qpl::f64{ 1 });
		this->r = static_cast<qpl::u8>((this->r * (1 - strength) + color.r * strength));
		this->g = static_cast<qpl::u8>((this->g * (1 - strength) + color.g * strength));
		this->b = static_cast<qpl::u8>((this->b * (1 - strength) + color.b * strength));
		this->a = static_cast<qpl::u8>((this->a * (1 - strength) + color.a * strength));
		return *this;
	}
	qsf::rgb qsf::rgb::interpolated(qsf::rgb color, qpl::f64 strength) const {
		auto copy = *this;
		copy.interpolate(color, strength);
		return copy;
	}
	qsf::rgb qsf::rgb::interpolation(const std::vector<qsf::rgb>& colors, qpl::f64 strength) {
		strength = qpl::clamp(0.0, strength, 1.0);
		if (strength == 1.0) {
			return colors.back();
		}

		auto index = static_cast<qpl::u32>(strength * (colors.size() - 1));
		auto left_over = ((colors.size() - 1) * strength) - index;

		return colors[index].interpolated(colors[index + 1], left_over);
	}
	qsf::rgb qsf::rgb::grey_shade(qpl::u8 strength) {
		return qsf::rgb(strength, strength, strength);
	}

	qsf::rgb& qsf::rgb::invert() {
		this->r = 255 - this->r;
		this->g = 255 - this->g;
		this->b = 255 - this->b;
		return *this;
	}
	qsf::rgb qsf::rgb::inverted() const {
		auto copy = *this;
		copy.invert();
		return copy;
	}
	qsf::rgb qsf::rgb::with_alpha(qpl::u8 alpha) const {
		auto copy = *this;
		copy.a = alpha;
		return copy;
	}
	qsf::rgb qsf::rgb::multiplied_alpha(qpl::u8 alpha) const {
		auto copy = *this;
		copy.a = qpl::u8_cast(qpl::f32_cast(this->a) * (qpl::f32_cast(alpha) / 255.f));
		return copy;
	}

	qsf::rgb& qsf::rgb::operator*=(qsf::rgb other) {
		this->r *= other.r;
		this->g *= other.g;
		this->b *= other.b;
		return *this;
	}
	qsf::rgb qsf::rgb::operator*(qsf::rgb other) const {
		auto copy = *this;
		copy *= other;
		return copy;
	}
	qsf::rgb& qsf::rgb::operator-=(qsf::rgb other) {
		this->r -= other.r;
		this->g -= other.g;
		this->b -= other.b;
		return *this;
	}
	qsf::rgb qsf::rgb::operator-(qsf::rgb other) const {
		auto copy = *this;
		copy -= other;
		return copy;
	}
	qsf::rgb& qsf::rgb::operator/=(qsf::rgb other) {
		this->r /= other.r;
		this->g /= other.g;
		this->b /= other.b;
		return *this;
	}
	qsf::rgb qsf::rgb::operator/(qsf::rgb other) const {
		auto copy = *this;
		copy /= other;
		return copy;
	}
	qsf::rgb& qsf::rgb::operator+=(qsf::rgb other) {
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		return *this;
	}
	qsf::rgb qsf::rgb::operator+(qsf::rgb other) const {
		auto copy = *this;
		copy += other;
		return copy;
	}

	const qsf::rgb qsf::rgb::red = 0xFF'00'00;
	const qsf::rgb qsf::rgb::green = 0x00'FF'00;
	const qsf::rgb qsf::rgb::blue = 0x00'00'FF;
	const qsf::rgb qsf::rgb::yellow = 0xFF'FF'00;
	const qsf::rgb qsf::rgb::orange = 0xFF'88'00;
	const qsf::rgb qsf::rgb::cyan = 0x00'FF'FF;
	const qsf::rgb qsf::rgb::magenta = 0xFF'00'FF;
	const qsf::rgb qsf::rgb::white = 0xFF'FF'FF;
	const qsf::rgb qsf::rgb::grey = 0x88'88'88;
	const qsf::rgb qsf::rgb::black = 0x00'00'00;
	const qsf::rgb qsf::rgb::transparent = 0xFF'FF'FF'00;
	const qsf::rgb qsf::rgb::unset = 0x00'00'00'00;


	qsf::rgb qsf::get_random_color() {
		auto value = (qpl::random((1u << 24) - 1) << 8) | 0xFF;
		return qsf::rgb(value);
	}
	qsf::rgb qsf::get_random_transparency_color() {
		return qsf::rgb(qpl::random(qpl::u32_max));
	}
	qsf::rgb qsf::get_rainbow_color(qpl::f64 f) {
		const static std::vector<qsf::rgb> rainbow = { qsf::rgb::red, qsf::rgb::yellow, qsf::rgb::green, qsf::rgb::cyan, qsf::rgb::blue, qsf::rgb::magenta, qsf::rgb::red };
		return qsf::rgb::interpolation(rainbow, f);
	}
}
#endif