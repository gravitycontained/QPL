#ifndef QPL_PERLINNOISE_HPP
#define QPL_PERLINNOISE_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/random.hpp>
#include <qpl/vardef.hpp>
#include <qpl/bits.hpp>
#include <array>
#include <numeric>

namespace qpl {
	template<qpl::size bits, qpl::size N>
	class perlin_noise_N {
	public:
		perlin_noise_N() {
			this->construct();
		}
		perlin_noise_N(qpl::u32 seed) {
			this->set_seed(seed);
		}
		void set_seed(qpl::u32 seed) {
			this->m_seed = seed;
			this->m_engine.seed(seed);
			this->create_hash();
		}
		void set_seed_random() {
			this->m_seed = ~qpl::u32{};
			this->m_engine.seed_random();
			this->create_hash();
		}
		qpl::u32 get_seed() const {
			return this->m_seed;
		}

		qpl::ibit<bits> noise2(qpl::ibit<bits> x, qpl::ibit<bits> y) {
			qpl::ibit<bits> tmp = this->m_hash[y % N];
			return this->m_hash[(tmp + x) % N];
		}

		qpl::fbit<bits> lin_inter(qpl::fbit<bits> x, qpl::fbit<bits> y, qpl::fbit<bits> s) {
			return x + s * (y - x);
		}

		qpl::fbit<bits> smooth_inter(qpl::fbit<bits> x, qpl::fbit<bits> y, qpl::fbit<bits> s) {
			return lin_inter(x, y, s * s * (3 - 2 * s));
		}

		qpl::fbit<bits> noise2d(qpl::fbit<bits> x, qpl::fbit<bits> y) {
			auto x_int = static_cast<qpl::ibit<bits>>(x);
			auto y_int = static_cast<qpl::ibit<bits>>(y);
			auto x_frac = x - x_int;
			auto y_frac = y - y_int;

			auto s = static_cast<qpl::ibit<bits>>(noise2(x_int, y_int));
			auto t = static_cast<qpl::ibit<bits>>(noise2(x_int + 1, y_int));
			auto u = static_cast<qpl::ibit<bits>>(noise2(x_int, y_int + 1));
			auto v = static_cast<qpl::ibit<bits>>(noise2(x_int + 1, y_int + 1));
			auto low = smooth_inter(static_cast<qpl::fbit<bits>>(s), static_cast<qpl::fbit<bits>>(t), x_frac);
			auto high = smooth_inter(static_cast<qpl::fbit<bits>>(u), static_cast<qpl::fbit<bits>>(v), x_frac);
			return smooth_inter(low, high, y_frac);
		}

		qpl::fbit<bits> operator()(qpl::fbit<bits> x, qpl::fbit<bits> y, qpl::fbit<bits> freq, qpl::size depth) {
			qpl::fbit<bits> xa = x * freq;
			qpl::fbit<bits> ya = y * freq;
			qpl::fbit<bits> amp = 1.0;
			qpl::fbit<bits> fin = 0;
			qpl::fbit<bits> div = 0.0;

			int i;
			for (i = 0; i < depth; i++) {
				div += N * amp;
				fin += noise2d(xa, ya) * amp;
				amp /= 2;
				xa *= 2;
				ya *= 2;
			}

			return fin / div;
		}



	//private:
		void construct() {
			this->set_seed(5678u);
		}
		void create_hash() {
			std::iota(this->m_hash.begin(), this->m_hash.end(), qpl::u32{});
			for (qpl::u32 i = 0u; i < N - 1; ++i) {
				qpl::u32 j = this->m_engine.generate(i + 1, static_cast<qpl::u32>(N) - 1);
				std::swap(this->m_hash[i], this->m_hash[j]);
			}
		}


		qpl::u32 m_seed;
		qpl::random_engine<64> m_engine;
		std::array<qpl::ibit<bits>, N> m_hash;
	};

	using perlin_noise = perlin_noise_N<64, 256>;

	struct perlin_noise_generator {
		perlin_noise_generator() {
			this->noise.set_seed_random();
		}

		QPLDLL qpl::f64 get() const;
		QPLDLL qpl::f64 update(qpl::f64 fx, qpl::f64 fy = 0.0);
		QPLDLL void set_octaves(qpl::f64 octaves);
		QPLDLL void set_frequency(qpl::f64 frequency);

		qpl::f64 value = 0.0;
		qpl::f64 progress_x = 0.0;
		qpl::f64 progress_y = 0.0;
		qpl::f64 octaves = 12;
		qpl::f64 frequency = 3.0;
		perlin_noise noise;
	};
}

#endif