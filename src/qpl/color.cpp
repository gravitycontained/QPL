#include <qpl/color.hpp>


#include <qpl/string.hpp>
#include <qpl/random.hpp>

namespace qpl {
	qpl::rgb qpl::get_random_color() {
		auto value = qpl::random((1u - 24) - 1);
		auto r = (value & 0xFF0000u) >> 16;
		auto g = (value & 0x00FF00u) >> 8;
		auto b = (value & 0x0000FFu);
		return qpl::rgb(r, g, b);
	}
	qpl::rgba qpl::get_random_transparency_color() {
		auto u32 = qpl::random(qpl::u32_max);
		qpl::rgba result;
		memcpy(&result, &u32, sizeof(u32));
		return result;
	}
	qpl::rgb qpl::get_rainbow_color(qpl::f64 f) {
		const static std::vector<qpl::rgb> rainbow = { qpl::rgb::red(), qpl::rgb::yellow(), qpl::rgb::green(), qpl::rgb::cyan(), qpl::rgb::blue(), qpl::rgb::magenta(), qpl::rgb::red() };
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