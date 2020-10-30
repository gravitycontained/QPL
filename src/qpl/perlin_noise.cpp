#include <qpl/perlin_noise.hpp>

namespace qpl {

	qpl::f64 qpl::perlin_noise_generator::get() const {
		return this->value;
	}
	qpl::f64 qpl::perlin_noise_generator::update(qpl::f64 fx, qpl::f64 fy) {
		this->progress_x += fx;
		this->progress_y += fy;
		this->value = this->noise(this->progress_x, this->progress_y, this->frequency, this->octaves);
		return this->value;
	}
	void qpl::perlin_noise_generator::set_octaves(qpl::f64 octaves) {
		this->octaves = octaves;
	}
	void qpl::perlin_noise_generator::set_frequency(qpl::f64 frequency) {
		this->frequency = frequency;
	}
}