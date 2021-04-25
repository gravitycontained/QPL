#include <qpl/memory.hpp>
#include <qpl/string.hpp>

namespace qpl {

	void qpl::print_character_bool_table(qpl::string_view characters) {
		std::array<bool, 256> table;
		table.fill(false);
		for (auto& i : characters) {
			table[i] = true;
		}

		qpl::println('{');
		for (qpl::u32 i = 0u; i < table.size(); ++i) {
			if (i % 16 == 0) {
				qpl::print('\t');
			}
			qpl::print(table[i], ", ");
			if (i % 16 == 15) {
				qpl::println();
			}
		}
		qpl::println('}');
	}
}