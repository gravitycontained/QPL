#include <qpl/random.hpp>
#include <qpl/memory.hpp>

namespace qpl {
	qpl::detail::rng_t qpl::detail::rng;

	void qpl::set_random_range_i(qpl::i64 max) {
		qpl::detail::rng.idist.set_range(max);
	}
	void qpl::set_random_range_i(qpl::i64 min, qpl::i64 max) {
		qpl::detail::rng.idist.set_range(min, max);
	}
	void qpl::set_random_range_u(qpl::u64 max) {
		qpl::detail::rng.udist.set_range(max);
	}
	void qpl::set_random_range_u(qpl::u64 min, qpl::u64 max) {
		qpl::detail::rng.udist.set_range(min, max);
	}
	void qpl::set_random_range_f(qpl::f64 max) {
		qpl::detail::rng.fdist.set_range(max);
	}
	void qpl::set_random_range_f(qpl::f64 min, qpl::f64 max) {
		qpl::detail::rng.fdist.set_range(min, max);
	}
	void qpl::set_random_seed(qpl::u64 seed) {
		qpl::detail::rng.rng.seed(seed);
	}
	bool qpl::random_b() {
		return qpl::detail::rng.rng.generate() & 0x1ull;
	}
	bool qpl::random_b(qpl::f64 probability) {
		return qpl::random_f(1.0) <= probability;
	}
	qpl::i64 qpl::random_i() {
		return qpl::detail::rng.rng.generate(qpl::detail::rng.idist);
	}
	qpl::u64 qpl::random_u() {
		return qpl::detail::rng.rng.generate(qpl::detail::rng.udist);
	}
	qpl::f64 qpl::random_f() {
		return qpl::convert_memory<qpl::f64>(qpl::detail::rng.rng.generate());
	}
	qpl::u64 qpl::random_current() {
		return qpl::detail::rng.rng.get_current();
	}
	qpl::i64 qpl::random_i(qpl::i64 min, qpl::i64 max) {
		return qpl::detail::rng.rng.generate(min, max);
	}
	qpl::u64 qpl::random_u(qpl::u64 min, qpl::u64 max) {
		return qpl::detail::rng.rng.generate(min, max);
	}
	qpl::f64 qpl::random_f(qpl::f64 min, qpl::f64 max) {
		return qpl::detail::rng.rng.generate(min, max);
	}
	qpl::i64 qpl::random_i(qpl::i64 max) {
		return qpl::detail::rng.rng.generate(max);
	}
	qpl::u64 qpl::random_u(qpl::u64 max) {
		return qpl::detail::rng.rng.generate(max);
	}
	qpl::f64 qpl::random_f(qpl::f64 max) {
		return qpl::detail::rng.rng.generate(max);
	}
	qpl::u64 qpl::random() {
		return qpl::detail::rng.rng.generate();
	}
	qpl::f64 qpl::random_falling(qpl::f64 n) {
		return (1.0 / qpl::random(0.0, 1.0 / n)) - n;
	}
}