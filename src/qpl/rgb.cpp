#include <qpl/rgb.hpp>
#ifdef QPL_USE_VULKAN
#include <qpl/random.hpp>
#include <qpl/string.hpp>

namespace qpl {
	const qpl::frgb qpl::frgb::red = qpl::frgb(1.0f, 0.0f, 0.0f);
	const qpl::frgb qpl::frgb::green = qpl::frgb(0.0f, 1.0f, 0.0f);
	const qpl::frgb qpl::frgb::blue = qpl::frgb(0.5f, 0.5f, 1.0f);
	const qpl::frgb qpl::frgb::black = qpl::frgb(0.0f, 0.0f, 0.0f);
	const qpl::frgb qpl::frgb::white = qpl::frgb(1.0f, 1.0f, 1.0f);
	const qpl::frgb qpl::frgb::yellow = qpl::frgb(1.0f, 1.0f, 0.0f);
	const qpl::frgb qpl::frgb::magenta = qpl::frgb(1.0f, 0.0f, 1.0f);
	const qpl::frgb qpl::frgb::cyan = qpl::frgb(0.0f, 1.0f, 1.0f);
	const qpl::frgb qpl::frgb::orange = qpl::frgb(1.0f, 0.5f, 0.0f);
	const qpl::frgb qpl::frgb::purple = qpl::frgb(0.5f, 0.0f, 1.0f);
	const qpl::frgb qpl::frgb::amaranth = qpl::frgb(1.0f, 0.0f, 0.5f);
	const qpl::frgb qpl::frgb::turquoise = qpl::frgb(0.0f, 1.0f, 0.5f);

	qpl::frgb& qpl::frgb::operator=(const qpl::frgb& other) {
		this->m_rgb = other.m_rgb;
		return *this;
	}
	qpl::frgb& qpl::frgb::operator=(const glm::vec3& other) {
		this->m_rgb = other;
		return *this;
	}
	bool qpl::frgb::operator==(const qpl::frgb& other) const {
		return this->m_rgb == other.m_rgb;
	}
	bool qpl::frgb::operator==(const glm::vec3& other) const {
		return this->m_rgb == other;
	}

	std::string qpl::frgb::string() const {
		return qpl::to_string("(", this->m_rgb.r, ", ", this->m_rgb.g, ", ", this->m_rgb.b, ")");
	}
	qpl::frgb::operator glm::vec3() const {
		return this->get();
	}
	glm::vec3 qpl::frgb::get() const {
		return this->m_rgb;
	}


	void qpl::frgb::brighten(qpl::f32 delta) {
		this->m_rgb = qpl::frgb::white.get() * delta + this->m_rgb * (1 - delta);
	}
	qpl::frgb qpl::frgb::brightened(qpl::f32 delta) const {
		auto copy = *this;
		copy.brighten(delta);
		return copy;
	}
	
	void qpl::frgb::darken(qpl::f32 delta) {
		this->m_rgb = qpl::frgb::black.get() * delta + this->m_rgb * (1 - delta);
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
		delta = qpl::min(delta, 1.0f);
		auto min = qpl::min(this->m_rgb.r, this->m_rgb.g, this->m_rgb.b);
		auto max = qpl::max(this->m_rgb.r, this->m_rgb.g, this->m_rgb.b);

		auto diff = max - min;
		auto saturated = (this->m_rgb - glm::vec3(min)) / diff;
		this->m_rgb = saturated * delta + this->m_rgb * (1 - delta);
	}
	qpl::frgb qpl::frgb::intensified(qpl::f32 delta) const {
		auto copy = *this;
		copy.intensify(delta);
		return copy;
	}

	void qpl::frgb::grayify(qpl::f32 delta) {
		delta = qpl::min(delta, 1.0f);
		auto sum = this->m_rgb.r + this->m_rgb.g + this->m_rgb.b;
		auto grayified = glm::vec3(sum) / 3.0f;
		this->m_rgb = grayified * delta + this->m_rgb * (1 - delta);
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

	void qpl::frgb::black_gray_intense_white(qpl::f32 delta) {
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
	qpl::frgb qpl::frgb::black_gray_intense_whited(qpl::f32 delta) const {
		auto copy = *this;
		copy.black_gray_intense_white(delta);
		return copy;
	}


	void qpl::frgb::black_intense_white(qpl::f32 delta) {
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
	qpl::frgb qpl::frgb::black_intense_whited(qpl::f32 delta) const {
		auto copy = *this;
		copy.black_intense_white(delta);
		return copy;
	}



	void qpl::frgb::interpolate(qpl::frgb other, qpl::f32 delta) {
		this->m_rgb = this->m_rgb * (1 - delta) + other.m_rgb * delta;
	}
	qpl::frgb qpl::frgb::interpolated(qpl::frgb other, qpl::f32 delta) const {
		auto copy = *this;
		copy.interpolate(other, delta);
		return copy;
	}

	qpl::frgb qpl::random_color() {
		return qpl::frgb({ 
			static_cast<qpl::f32>(qpl::random_f(0.0f, 1.0f)), 
			static_cast<qpl::f32>(qpl::random_f(0.0f, 1.0f)),
			static_cast<qpl::f32>(qpl::random_f(0.0f, 1.0f)),
		});
	}
}
#endif