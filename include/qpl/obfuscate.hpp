#pragma once

#include <qpl/random.hpp>
#include <qpl/algorithm.hpp>

namespace qpl {

	template<typename T, qpl::size N = 2, qpl::size S = 0x2f> requires (qpl::is_integer<T>())
		struct simple_obfuscated_memory {
		struct block {
			T a;
			T b;
			T c;
			T d;
			T e;
			T f;
			T g;
			T h;
		};

		std::array<block, N> data;
		qpl::u32 ctr = 0u;

		simple_obfuscated_memory() {
			shuffle();
		}
		simple_obfuscated_memory(T value) {
			shuffle();
			*this = value;
		}

		void shuffle() {
			if (!qpl::detail::rng.seeded) {
				qpl::detail::rng.set_seed_random();
			}
			ctr = 0;
			for (auto& i : data) {
				i.a = qpl::random<T>();
				i.b = qpl::random<T>();
				i.c = qpl::random<T>();
				i.d = qpl::random<T>();
				i.e = qpl::random<T>();
				i.f = qpl::random<T>();
				i.g = qpl::random<T>();
				i.h = qpl::random<T>();
			}
		}

		simple_obfuscated_memory& operator+=(T _xecc63aaa) {
			return (*this = operator T() + _xecc63aaa);
		}
		simple_obfuscated_memory& operator-=(T _xecc63aaa) {
			return (*this = operator T() - _xecc63aaa);
		}
		simple_obfuscated_memory& operator/=(T _xecc63aaa) {
			return (*this = operator T() / _xecc63aaa);
		}
		simple_obfuscated_memory& operator*=(T _xecc63aaa) {
			return (*this = operator T() * _xecc63aaa);
		}
		simple_obfuscated_memory& operator^=(T _xecc63aaa) {
			return (*this = operator T() ^ _xecc63aaa);
		}
		simple_obfuscated_memory& operator&=(T _xecc63aaa) {
			return (*this = operator T() & _xecc63aaa);
		}
		simple_obfuscated_memory& operator|=(T _xecc63aaa) {
			return (*this = operator T() | _xecc63aaa);
		}
		simple_obfuscated_memory& operator=(T value) {
			++ctr;
			if (ctr >= S) {
				shuffle();
			}

			data[0].b = value ^ (data[0].g ^ data[0].d);

			for (qpl::u32 i = 0u; i < N; ++i) {
				T n = 0;
				if (i == 0) n = data[0].h;
				if (i == 1) n = data[0].a ^ data[1].h;
				if (i == 2) n = data[1].f;
				n ^= qpl::rotate_left(data[0].e, (data[N - 1 - i].c & 0x7) + 1);
				data[0].b ^= (data[i].g ^ data[i].d ^ n);
			}
			return *this;
		}
		operator T() const {
			T result = data[0].b ^ (data[0].g ^ data[0].d);

			for (qpl::u32 i = 0u; i < N; ++i) {
				T n = 0;
				if (i == 0) n = data[0].h;
				if (i == 1) n = data[0].a ^ data[1].h;
				if (i == 2) n = data[1].f;
				n ^= qpl::rotate_left(data[0].e, (data[N - 1 - i].c & 0x7) + 1);
				result ^= (data[i].g ^ data[i].d ^ n);
			}
			return result;
		}
		T get() const {
			return this->operator T();
		}
		template<typename T, qpl::size N, qpl::size _x3341f11> requires (qpl::is_integer<T>())
		friend std::ostream& operator<<(std::ostream& os, const simple_obfuscated_memory<T, N, _x3341f11>& n);
	};

	template<typename T, qpl::size N, qpl::size _x3341f11> requires (qpl::is_integer<T>())
	std::ostream& operator<<(std::ostream& os, const simple_obfuscated_memory<T, N, _x3341f11>& n) {
		return (os << n.operator T());
	}

}