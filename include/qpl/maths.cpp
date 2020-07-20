#include <qpl/maths.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/string.hpp>
#include <qpl/ranges.hpp>

namespace qpl {
	std::vector<std::vector<qpl::u32>> sudoku_sum_possibilities(qpl::size sum, qpl::size numbers) {
		std::vector<std::vector<qpl::u32>> result;

		auto arrangements = qpl::list_all_arrangements(qpl::vector_0_to_n(9u, 1u), numbers);

		for (auto& i : arrangements) {
			if (qpl::container_sum(i) == sum) {
				result.push_back(i);
			}
		}
		
		return result;
	}
}