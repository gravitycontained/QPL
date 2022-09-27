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

		using float_type = qpl::fbit<bits>;
		using int_type = qpl::ibit<bits>;

		perlin_noise_N() {
			this->construct();
		}
		perlin_noise_N(qpl::u64 seed) {
			this->set_seed(seed);
		}
		void set_seed(qpl::u64 seed) {
			this->m_seed = seed;
			this->seed_random = false;
			this->create_hash();
		}
		void set_seed_random() {
			this->m_seed = ~qpl::u64{};
			this->seed_random = true;
			this->create_hash();
		}
		qpl::u32 get_seed() const {
			return this->m_seed;
		}

		int_type get_coord_hash(int_type x, int_type y) const {
			int_type yhash = this->m_hash[y % N];
			return this->m_hash[(yhash + x) % N];
		}
		template<qpl::size M>
		int_type get_coord_hash(const std::array<int_type, M>& coords) const {
			int_type h = 0;
			for (qpl::size i = 0u; i < M; ++i) {
				h = this->m_hash[(h + coords[M - i - 1]) % N];
			}
			return h;
		}

		constexpr float_type static linear_interpolation(float_type x, float_type y, float_type s) {
			return x + s * (y - x);
		}

		constexpr float_type static smooth_interpolation(float_type x, float_type y, float_type s) {
			return linear_interpolation(x, y, s * s * (3 - 2 * s));
		}


		template<qpl::size M>
		float_type noiseN(const std::array<float_type, M>& coords) const {
			std::array<int_type, M> floors;
			std::array<float_type, M> deltas;
			
			for (qpl::size i = 0u; i < M; ++i) {
				floors[i] = static_cast<int_type>(coords[i]);
				deltas[i] = coords[i] - floors[i];
			}
			constexpr auto array_size = (1 << (M + 1)) - 1;
			std::array<float_type, array_size> array;
			std::array<int_type, M> location;
			
			qpl::size offset = 0u;
			qpl::size before = 0u;
			for (qpl::size d = 0u; d <= M; ++d) {
			
				auto size = 1 << (M - d);
			
				if (d == 0) {
					for (qpl::size i = 0u; i < size; ++i) {
						for (qpl::size b = 0u; b < M; ++b) {
							location[b] = static_cast<int_type>(floors[b] + qpl::get_bit(i, b));
						}
						array[i] = static_cast<float_type>(this->get_coord_hash(location));
					}
				}
				else {
					for (qpl::size i = 0u; i < size; ++i) {
						array[i + offset] = this->smooth_interpolation(array[(offset - before) + i * 2], array[(offset - before) + i * 2 + 1], deltas[d - 1]);
					}
				}
			
				before = size;
				offset += before;
			}

			//qpl::constexpr_iterate<M + 1>([&](auto d) {
			//	constexpr auto size = 1 << (M - d);
			//	constexpr auto offset = [&]() {
			//		qpl::size result = 0u;
			//		for (qpl::size i = 0u; i < d; ++i) {
			//			result += (1 << (M - i));
			//		}
			//		return result;
			//	}();
			//
			//	if constexpr (d == 0) {
			//		for (qpl::size i = 0u; i < size; ++i) {
			//			for (qpl::size b = 0u; b < M; ++b) {
			//				location[b] = static_cast<int_type>(coords[b] + qpl::get_bit(i, b));
			//			}
			//			array[i + offset] = static_cast<float_type>(this->get_coord_hash(location));
			//		}
//	}
//	else {
//		constexpr auto before = offset - size;
//		for (qpl::size i = 0u; i < size; ++i) {
//			array[i + offset] = this->smooth_interpolation(array[before + i * 2], array[before + i * 2 + 1], deltas[d - 1]);
//		}
//	}
//});
return array.back();
		}
		float_type noise2d(float_type x, float_type y) const {
			auto ix = static_cast<int_type>(std::floor(x));
			auto iy = static_cast<int_type>(std::floor(y));
			auto dx = x - ix;
			auto dy = y - iy;

			auto h00 = static_cast<float_type>(this->get_coord_hash(ix, iy));
			auto h10 = static_cast<float_type>(this->get_coord_hash(ix + 1, iy));
			auto h01 = static_cast<float_type>(this->get_coord_hash(ix, iy + 1));
			auto h11 = static_cast<float_type>(this->get_coord_hash(ix + 1, iy + 1));

			auto a = this->smooth_interpolation(h00, h10, dx);
			auto b = this->smooth_interpolation(h01, h11, dx);
			return this->smooth_interpolation(a, b, dy);
		}
		float_type noise3d(float_type x, float_type y, float_type z) const {
			auto ix = static_cast<int_type>(std::floor(x));
			auto iy = static_cast<int_type>(std::floor(y));
			auto iz = static_cast<int_type>(std::floor(z));
			auto dx = x - ix;
			auto dy = y - iy;
			auto dz = z - iz;

			auto h000 = static_cast<float_type>(this->get_coord_hash(std::array{ ix, iy, iz }));
			auto h100 = static_cast<float_type>(this->get_coord_hash(std::array{ ix + 1, iy, iz }));
			auto h010 = static_cast<float_type>(this->get_coord_hash(std::array{ ix, iy + 1, iz }));
			auto h110 = static_cast<float_type>(this->get_coord_hash(std::array{ ix + 1, iy + 1, iz }));

			auto h001 = static_cast<float_type>(this->get_coord_hash(std::array{ ix, iy, iz + 1 }));
			auto h101 = static_cast<float_type>(this->get_coord_hash(std::array{ ix + 1, iy, iz + 1 }));
			auto h011 = static_cast<float_type>(this->get_coord_hash(std::array{ ix, iy + 1, iz + 1 }));
			auto h111 = static_cast<float_type>(this->get_coord_hash(std::array{ ix + 1, iy + 1, iz + 1 }));

			auto x00 = this->smooth_interpolation(h000, h100, dx);
			auto x10 = this->smooth_interpolation(h010, h110, dx);
			auto x01 = this->smooth_interpolation(h001, h101, dx);
			auto x11 = this->smooth_interpolation(h011, h111, dx);

			auto y0 = this->smooth_interpolation(x00, x10, dy);
			auto y1 = this->smooth_interpolation(x01, x11, dy);

			return this->smooth_interpolation(y0, y1, dz);
		}

		template<typename T, qpl::size M>
		float_type get(const qpl::vectorN<T, M>& vec, float_type freq, qpl::u32 depth) const {
			float_type amp = 1.0;
			float_type fin = 0;
			float_type div = 0.0;
			qpl::vectorN<float_type, M> pos = vec * freq;

			for (qpl::u32 i = 0u; i < depth; ++i) {
				div += N * amp;
				if constexpr (M == 2) {
					fin += this->noise2d(pos.x, pos.y) * amp;
				}
				else if constexpr (M == 3) {
					fin += this->noise3d(pos.x, pos.y, pos.z) * amp;
				}
				else {
					fin += this->noiseN(pos.data) * amp;
				}
				amp /= 2;
				pos *= 2;
			}
			return fin / div;
		}
		template<typename T, qpl::size M>
		float_type getf(const qpl::vectorN<T, M>& vec, float_type freq, float_type depth) const {
			float_type amp = 1.0;
			float_type fin = 0;
			float_type div = 0.0;
			qpl::vectorN<float_type, M> pos = vec * freq;

			for (qpl::u32 i = 0u; i < depth; ++i) {
				div += N * amp;
				if constexpr (M == 2) {
					fin += this->noise2d(pos.x, pos.y) * amp;
				}
				else if constexpr (M == 3) {
					fin += this->noise3d(pos.x, pos.y, pos.z) * amp;
				}
				else {
					fin += this->noiseN(pos.data) * amp;
				}
				amp /= 2;
				pos *= 2;
			}

			auto df = std::fmod(depth, float_type{ 1 });
			if (df){
				amp *= df;
				div += N * amp;
				if constexpr (M == 2) {
					fin += this->noise2d(pos.x, pos.y) * amp;
				}
				else if constexpr (M == 3) {
					fin += this->noise3d(pos.x, pos.y, pos.z) * amp;
				}
				else {
					fin += this->noiseN(pos.data) * amp;
				}
			}

			return fin / div;
		}



		void construct() {
			this->set_seed(5678u);
		}
		void create_hash() {
			qpl::random_engine<64> engine;
			if (this->seed_random) {
				engine.seed_random();
			}
			else {
				engine.seed(this->seed_random);
			}

			std::iota(this->m_hash.begin(), this->m_hash.end(), qpl::u32{});
			for (qpl::u32 i = 0u; i < N - 1; ++i) {
				qpl::u32 j = engine.generate(i + 1, static_cast<qpl::u32>(N) - 1);
				std::swap(this->m_hash[i], this->m_hash[j]);
			}
		}


		std::array<int_type, N> m_hash;
		qpl::u64 m_seed = qpl::u64_max;
		bool seed_random = false;
	};

	using perlin_noise = perlin_noise_N<64, 256>;

	template<typename T, qpl::size N>
	struct perlin_noise_generator {
		perlin_noise_generator() {
			this->noise.set_seed_random();
		}

		T get() const {
			return this->value;
		}
		T update(qpl::vectorN<T, N> delta) {
			this->progress += delta;
			this->value = this->noise.get(this->progress, this->frequency, this->octaves);

			return this->value;
		}
		void set_octaves(qpl::u32 octaves) {
			this->octaves = octaves;
		}
		void set_frequency(T frequency) {
			this->frequency = frequency;
		}

		T value = 0.0;
		qpl::vectorN<T, N> progress;
		qpl::u32 octaves = 4u;
		T frequency = 3.0;
		perlin_noise noise;
	};
}

#endif