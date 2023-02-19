#ifndef QPL_RANDOM_HPP
#define QPL_RANDOM_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/time.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/vector.hpp>

#include <array>
#include <iterator>
#include <type_traits>
#include <random>
#include <iostream>

namespace qpl {
	//2^19937-1

	template<typename Ty, qpl::u64 W, qpl::u64 N, qpl::u64 M, qpl::u64 R, Ty P, qpl::u64 U, Ty D, qpl::u64 S, Ty B, qpl::u64 T, Ty C, qpl::u64 L, Ty F>
	class mersenne_twister {
	public:
		using result_type = Ty;
		static constexpr qpl::u64 state_size = N;
		static constexpr qpl::u64 word_size = W;

		mersenne_twister() {
			this->init();
		}
		Ty get_at(qpl::u32 index) const {
			Ty x = this->state[index] & WMSK;
			x ^= (x >> U) & D;
			x ^= (x << S) & B;
			x ^= (x << T) & C;
			x ^= (x & WMSK) >> L;
			return x;
		}

		Ty get_current() const {
			Ty x = this->state[this->index] & WMSK;
			x ^= (x >> U) & D;
			x ^= (x << S) & B;
			x ^= (x << T) & C;
			x ^= (x & WMSK) >> L;
			return x;
		}

		template<typename R = qpl::fbit<qpl::bits_in_type<Ty>()>>
		R generate_0_1() {
			return std::generate_canonical<R, std::numeric_limits<R>::digits>(*this);
		}

		//returns current and advances by 1
		Ty generate() {
			if (this->index >= N) {
				this->shuffle();
			}
			auto current = this->get_current();

			++this->index;
			++this->state_position;

			return current;
		}
		Ty operator()() {
			auto generate = this->generate();
			return generate;
		}
		void seed(Ty seed) {
			Ty prev = this->state[0] = seed & WMSK;
			for (qpl::i32 i = 1; i < N; ++i) {
				prev = this->state[i] = (i + F * (prev ^ (prev >> (W - 2))))& WMSK;
			}
			this->index = N;
		}
		void seed(const std::seed_seq& seq) {
			constexpr auto K = (W + (qpl::bits_in_type<qpl::u32>() - 1)) / qpl::bits_in_type<qpl::u32>();
			qpl::u32 data[K * N];
			seq.generate(&data[0], &data[K * N]);

			qpl::i32 word_id = 0;
			Ty sum = 0;
			for (int i = 0; i < N; ++i, word_id += K) {
				this->state[i] = data[word_id];
				for (int j = 0; ++j < K;) {
					this->state[i] |= static_cast<Ty>(data[word_id + j]) << (qpl::bits_in_type<qpl::u32>() * j);
				}

				this->state[i] &= WMSK;

				if (i == 0) {
					sum = this->state[i] >> R;
				}
				else {
					sum |= this->state[i];
				}
			}

			if (sum == 0) {
				this->state[0] = WMSK;
			}

			this->state_position = 0u;
			this->index = N;
		}

		void discard(qpl::u64 count) {
			for (qpl::u64 i = 0; i < count; ++i) {
				this->generate();
			}
		}
		static constexpr Ty min() {
			return Ty{ 0 };
		}
		static constexpr Ty max() {
			return WMSK;
		}

		void clear() {
			this->state_position = 0;
			this->index = 0;
			this->using_seed_seq = false;
		}
		void init() {
			this->clear();
		}
		void shuffle() {
			qpl::i32 i;
			for (i = 0; i < FH; ++i) {
				Ty bits = (this->state[i] & HMSK) | (this->state[i + 1] & LMSK);
				this->state[i] = (bits >> 1) ^ this->state[i + M] ^ (bits & 0x1 ? P : 0);
			}
			for (; i < N - 1; ++i) {
				Ty bits = (this->state[i] & HMSK) | (this->state[i + 1] & LMSK);
				this->state[i] = (bits >> 1) ^ this->state[i - FH] ^ (bits & 0x1 ? P : 0);
			}
			Ty bits = (this->state[i] & HMSK) | (this->state[0] & LMSK);
			this->state[i] = (bits >> 1) ^ this->state[M - 1] ^ (bits & 0x1 ? P : 0);

			this->index = 0;
		}

		std::array<Ty, N * 2> state;
		qpl::u32 state_position;
		qpl::u32 index;
		bool using_seed_seq;
		static constexpr Ty FH = N - M;
		static constexpr Ty WMSK = ~((~Ty{ 0 } << (W - 1)) << 1);
		static constexpr Ty HMSK = (WMSK << R) & WMSK;
		static constexpr Ty LMSK = ~HMSK & WMSK;
	};

	using mt19937_32 = qpl::mersenne_twister<qpl::u32, qpl::bits_in_type<qpl::u32>(), 624, 397, 31, 0x9908b0df, 11, 0xffffffff, 7, 0x9d2c5680, 15, 0xefc60000, 18, 1812433253>;
	using mt19937_64 = qpl::mersenne_twister<qpl::u64, qpl::bits_in_type<qpl::u64>(), 312, 156, 31, 0xb5026f5aa96619e9ULL, 29, 0x5555555555555555ULL, 17, 0x71d67fffeda60000ULL, 37, 0xfff7eee000000000ULL, 43, 6364136223846793005ULL>;

	template<qpl::u32 bits>
	class random_engine;

	template<typename T>
	class distribution {
	public:
		distribution(T min, T max) : m_dist(min, max) {

		}
		distribution(T max = T{ 1 }) : m_dist(T{}, max) {

		}
		using type = typename qpl::conditional<
			qpl::if_true<qpl::is_integer<T>()>, std::uniform_int_distribution<T>,
			qpl::if_true<qpl::is_floating_point<T>()>, std::uniform_real_distribution<T>>;

		void set_range(T min, T max) {
			this->m_dist = type(min, max);
		}
		void set_range(T max) {
			this->m_dist = type(T{}, max);
		}
		void set_min(T min) {
			this->m_dist = type(min, this->max());
		}
		void set_max(T max) {
			this->m_dist = type(this->min(), max);
		}
		T min() const {
			return this->m_dist.min();
		}
		T max() const {
			return this->m_dist.max();
		}

		template<qpl::u32 bits>
		friend class random_engine;

		template<qpl::u32 bits>
		T generate(qpl::random_engine<bits>& engine) const {
			return engine.generate(*this);
		}
	
		type m_dist;
	};


	template<qpl::u32 bits>
	class random_engine {
	public:
		using type = typename qpl::conditional<
			qpl::if_true<bits == 32u>, qpl::mt19937_32,
			qpl::if_true<bits == 64u>, qpl::mt19937_64>;
		
		void seed(qpl::u64 value) {
			this->engine.seed(value);
		}
		void seed(const std::seed_seq& seq) {
			this->engine.seed(seq);
		}
		void seed_time() {
			this->engine.seed(static_cast<qpl::u32>(qpl::time::clock_time()));
		}
		auto get_current() const {
			return this->engine.get_current();
		}
		void clear() {
			this->engine.clear();
		}
		void discard(qpl::u64 count) {
			this->engine.discard(count);
		}
		void seed_random() {
			std::array<qpl::u32, random_engine::type::state_size * random_engine::type::word_size / qpl::bits_in_type<qpl::u32>()> random_data;
			std::random_device source;
			std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
			std::seed_seq seeds(std::begin(random_data), std::end(random_data));
			this->engine.seed(seeds);
		}
		template<typename T>
		T generate(qpl::distribution<T> dist) {
			return dist.m_dist(this->engine);
		}
		template<typename T>
		T generate(std::normal_distribution<T> dist) {
			return dist(this->engine);
		}
		template<typename T>
		T generate(std::binomial_distribution<T> dist) {
			return dist(this->engine);
		}
		template<typename T>
		T generate(std::uniform_real_distribution<T> dist) {
			return dist(this->engine);
		}
		template<typename T>
		T generate(std::uniform_int_distribution<T> dist) {
			return dist(this->engine);
		}
		template<typename T>
		T generate(T min, T max) {
			qpl::distribution<T> dist(min, max);
			return dist.m_dist(this->engine);
		}
		template<typename T>
		T generate(T max) {
			qpl::distribution<T> dist(max);
			return dist.m_dist(this->engine);
		}
		auto generate() {
			return this->engine.generate();
		}
		auto generate_0_1() {
			return this->engine.generate_0_1();
		}
		bool generate_b() {
			return this->generate() & 0x1ull;
		}
		bool generate_b(qpl::f64 probability) {
			return this->generate_0_1() <= probability;
		}


		type engine;
	};

	namespace detail {
		struct rng_t {
			rng_t() {
				this->seeded = false;
				this->set_seed_random();
			}
			void set_seed_random() {
				if (!this->seeded) {
					this->engine.seed_random();
					this->seeded = true;
				}
			}
			qpl::random_engine<64> engine;
			qpl::distribution<qpl::i64> idist;
			qpl::distribution<qpl::u64> udist;
			qpl::distribution<qpl::f64> fdist;
			bool seeded = false;
		};

		QPLDLL extern qpl::detail::rng_t rng;
	}

	QPLDLL void set_random_range_i(qpl::i64 max);
	QPLDLL void set_random_range_i(qpl::i64 min, qpl::i64 max);
	QPLDLL void set_random_range_u(qpl::u64 max);
	QPLDLL void set_random_range_u(qpl::u64 min, qpl::u64 max);
	QPLDLL void set_random_range_f(qpl::f64 max);
	QPLDLL void set_random_range_f(qpl::f64 min, qpl::f64 max);
	QPLDLL void set_random_seed(qpl::u64 seed);
	QPLDLL bool random_b();
	QPLDLL bool random_b(qpl::f64 probability);
	QPLDLL qpl::i64 random_i();
	QPLDLL qpl::u64 random_u();
	QPLDLL qpl::f64 random_f();
	QPLDLL qpl::u64 random_current();
	QPLDLL qpl::i64 random_i(qpl::i64 min, qpl::i64 max);
	QPLDLL qpl::u64 random_u(qpl::u64 min, qpl::u64 max);
	QPLDLL qpl::f64 random_f(qpl::f64 min, qpl::f64 max);
	QPLDLL qpl::i64 random_i(qpl::i64 max);
	QPLDLL qpl::u64 random_u(qpl::u64 max);
	QPLDLL qpl::f64 random_f(qpl::f64 max);
	QPLDLL qpl::u64 random();

	template<typename T> requires (qpl::is_arithmetic<T>())
	T random(T min, T max) {
		qpl::distribution<T> dist(min, max);
		return qpl::detail::rng.engine.generate(dist);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	T random(T max) {
		qpl::distribution<T> dist(T{}, max);
		return qpl::detail::rng.engine.generate(dist);
	}
	template<qpl::size N, typename T>
	qpl::vectorN<T, N> random(qpl::vectorN<T, N> max) {
		qpl::vectorN<T, N> result;
		for (qpl::u32 i = 0u; i < N; ++i) {
			qpl::distribution<T> dist(T{}, max.data[i]);
			result.data[i] = qpl::detail::rng.engine.generate(dist);
		}
		return result;
	}
	template<qpl::size N, typename T>
	qpl::vectorN<T, N> random(qpl::vectorN<T, N> min, qpl::vectorN<T, N> max) {
		qpl::vectorN<T, N> result;
		for (qpl::u32 i = 0u; i < N; ++i) {
			qpl::distribution<T> dist(min.data[i], max.data[i]);
			result.data[i] = qpl::detail::rng.engine.generate(dist);
		}
		return result;
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	T random() {
		qpl::distribution<T> dist(qpl::type_min<T>(), qpl::type_max<T>());
		return qpl::detail::rng.engine.generate(dist);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	T random(std::normal_distribution<T> dist) {
		return qpl::detail::rng.engine.generate(dist);
	}
	template<typename T> requires (qpl::is_integer<T>())
	T random(std::binomial_distribution<T> dist) {
		return qpl::detail::rng.engine.generate(dist);
	}
	template<typename T> requires (qpl::is_integer<T>())
	T random(std::uniform_int_distribution<T> dist) {
		return qpl::detail::rng.engine.generate(dist);
	}
	template<typename T> requires (qpl::is_floating_point<T>())
	T random(std::uniform_real_distribution<T> dist) {
		return qpl::detail::rng.engine.generate(dist);
	}

	QPLDLL qpl::f64 random_falling(qpl::f64 n);

	template<typename C>
	void shuffle(C& data, qpl::u32 offset = 0u) {
		qpl::detail::rng.engine.discard(1);
		std::shuffle(data.begin() + offset, data.end(), qpl::detail::rng.engine.engine);
	}

	template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
	const qpl::container_subtype<C>& random_element(const C& data) {
		auto size = data.size();
		if (size == 0u) {
			throw qpl::exception("qpl::random_element( ", qpl::type_name<C>(), " ) : array is empty.");
		}
		auto index = qpl::random_i(qpl::size{}, size - qpl::size{ 1 });
		return data[index];
	}
	template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
	qpl::container_subtype<C>& random_element(C& data) {
		auto size = data.size();
		if (size == 0u) {
			throw qpl::exception("qpl::random_element( ", qpl::type_name<C>(), " ) : array is empty.");
		}
		auto index = qpl::random_i(qpl::size{}, size - qpl::size{ 1 });
		return data[index];
	}

	

	template<typename T>
	qpl::u32 random_weighted_index(std::vector<T> weights) {
		std::decay_t<T> sum = 0;
		for (auto& i : weights) {
			sum += i;
		}

		auto index = qpl::random(std::decay_t<T>{}, sum - 1);
		std::decay_t<T> target_sum = 0;
		for (qpl::u32 i = 0u; i < weights.size(); ++i) {
			target_sum += weights[i];
			if (index < target_sum) {
				return i;
			}
		}
		return weights.size();
	}

	template<typename T>
	qpl::u32 random_weighted_index(std::span<T> weights) {
		std::decay_t<T> sum = 0;
		for (auto& i : weights) {
			sum += i;
		}
		if (sum == 0) {
			return 0u;
		}
		auto index = qpl::random(T{}, sum - 1);
		std::decay_t<T> target_sum = 0;
		for (qpl::u32 i = 0u; i < weights.size(); ++i) {
			target_sum += weights[i];
			if (index < target_sum) {
				return i;
			}
		}
		return weights.size();
	}

	template<typename T, typename U, typename ...Args>
	T random_element(T&& first, U&& second, Args&&... n) {
		return qpl::random_element(std::vector<T>{ first, second, n... });
	}

	template<typename T>
	std::vector<T> random_vector_from_sum_target(T target, qpl::size size) {
		std::vector<T> result(size);
		for (qpl::u32 i = 0u; i < size; ++i) {
			T value = T{};
			if (i == size - 1) {
				value = target;
			}
			else if (qpl::signed_cast(target - (size - i - 1)) >= T{ 1 }) {
				value = qpl::random<T>(T{ 1 }, target - (size - i - 1));
			}
			result[i] = value;
			target -= value;
		}
		qpl::shuffle(result);
		return result;
	}
	template<typename T>
	std::vector<T> random_vector(qpl::size size, T min = qpl::type_min<T>(), T max = qpl::type_max<T>()) {
		std::vector<T> result(size);
		for (qpl::u32 i = 0u; i < size; ++i) {
			result[i] = qpl::random(min, max);
		}
		return result;
	}


	template<typename T>
	std::vector<T> random_distribution_frequency(T min, T max, qpl::size N, const std::vector<qpl::f64>& probabilities) {
		auto size = (max - min + 1);
		std::vector<T> result(size, T{ 0 });

		qpl::f64 div = qpl::f64_cast(size);
		qpl::f64 sum = 0.0;

		auto probability = probabilities[0u];
		for (T i = min; i <= max; ++i) {
			div = (1 / probabilities[i - min]) * (1 - sum);
			probability = 1.0 / div;

			std::binomial_distribution<qpl::i64> dist(N, probability);
			auto value = qpl::random(dist);
			result[i - min] = value;
			N -= value;

			sum += probabilities[i - min];
		}
		return result;
	}

	template<typename T, typename R> requires(qpl::is_integer<T>())
	std::vector<T> random_unique_range(T min, T max, qpl::size N, R& engine) {
		std::vector<T> result(N);

		std::set<T> used;
		for (qpl::size i = 0u; i < N; ++i) {
			auto stop = max - static_cast<T>(i);
			auto n = engine.generate(min, stop);

			for (auto& u : used) {
				if (u <= n) {
					++n;
				}
				else {
					break;
				}
			}
			result[i] = n;
			used.insert(n);
		}
		return result;
	}

	template<typename T> requires(qpl::is_integer<T>())
	std::vector<T> random_unique_range(T min, T max, qpl::size N) {
		return qpl::random_unique_range(min, max, N, detail::rng.engine);
	}

	template<typename T> requires(qpl::is_integer<T>())
	std::vector<T> random_unique_range_shuffle(T min, T max, qpl::size N) {
		
		std::vector<T> result(max - min + 1);
		for (T i = min; i <= max; ++i) {
			result[i - min] = i;
		}

		qpl::shuffle(result);
		std::vector<T> res(N);
		for (qpl::size i = 0u; i < N; ++i) {
			res[i] = result[i];
		}
		return res;
	}

	template<typename T>
	std::vector<T> random_distribution_frequency(T min, T max, qpl::size N) {
		auto size = (max - min + 1);
		std::vector<T> result(size, T{ 0 });

		for (T i = min; i <= max; ++i) {
			auto probability = 1.0 / (size - (i - min));

			std::binomial_distribution<qpl::i64> dist(N, probability);
			auto value = qpl::random(dist);
			result[i - min] = value;
			N -= value;
		}
		return result;
	}

	template<qpl::size B = 64, qpl::size MN = 256u, qpl::u64 MX = 0xFA581ull, qpl::u64 MY = 0x17CD5ull >
	struct white_noise_engine_N {
		constexpr static qpl::u64 chunk_width = (B == 64u ? 17u : 24u);
		using utype = qpl::ubit<B>;
		using ftype = qpl::fbit<B>;

		qpl::random_engine<B> seed_engine;
		qpl::random_engine<B> chunk_engine;
		std::array<utype, MN> hash;
		qpl::u64 hash_key;
		qpl::u64 seed = 5489ull;
		qpl::u64 loaded_chunk = qpl::u64_max;
		qpl::u64 loaded_seed_chunk = qpl::u64_max;
		bool first = true;

		void set_seed(qpl::u64 seed) {
			this->seed = seed;

			this->seed_engine.seed(this->seed);
			this->seed_engine.engine.shuffle();
			for (qpl::size i = 0u; i < MN; ++i) {
				this->hash[i] = qpl::reverse_bits_copied(this->seed_engine.engine.get_at(i));
			}
			this->hash_key = qpl::rotate_right(this->seed_engine.engine.get_at(MN), qpl::bits_in_type<utype>() / 2);
			this->first = true;
		}

		auto generate(qpl::i64 x, qpl::i64 y) {
			auto ux = qpl::u64_cast(x);
			auto uy = qpl::u64_cast(y);

			auto cx = ux / chunk_width;
			auto cy = uy / chunk_width;

			auto c = cy * chunk_width + cx;
			auto sc = (cy / chunk_width) * chunk_width + (cx / chunk_width);

			if (first || this->loaded_seed_chunk != sc) {
				this->loaded_seed_chunk = sc;
				this->seed_engine.seed(this->seed ^ sc);
				this->seed_engine.engine.shuffle();
			}
			if (first || this->loaded_chunk != c) {
				this->loaded_chunk = c;
				auto seed_engine_index = (cy % chunk_width) * chunk_width + (cx % chunk_width);
				this->chunk_engine.seed(this->seed_engine.engine.get_at(seed_engine_index) ^ this->seed);
				this->chunk_engine.engine.shuffle();
			}
			first = false;
			auto index = (uy % chunk_width) * chunk_width + (ux % chunk_width);
			auto magic = (((ux % MX) * (uy % MY)) ^ (~this->seed) ^ this->hash_key) % MN;
			return this->chunk_engine.engine.get_at(index) ^ this->hash[magic];
		}

		auto generate_0_1(qpl::i64 x, qpl::i64 y) {
			return this->generate(x, y) / static_cast<ftype>(qpl::type_max<utype>());
		}
	};

	using white_noise_engine = white_noise_engine_N<64u>;

}

#endif