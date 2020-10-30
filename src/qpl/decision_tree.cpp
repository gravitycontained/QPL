#include <qpl/decision_tree.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/random.hpp>

namespace qpl {
	qpl::f64 qpl::gini_entry::yes_impurity() const {
		auto sum = this->correct_yes + this->wrong_yes;
		return 1.0 - qpl::square(qpl::f64_cast(this->correct_yes) / sum) - qpl::square(qpl::f64_cast(this->wrong_yes) / sum);
	}
	qpl::f64 qpl::gini_entry::yes_accuracy() const {
		auto sum = this->correct_yes + this->wrong_yes;
		return qpl::f64_cast(this->correct_yes) / sum;
	}
	qpl::f64 qpl::gini_entry::no_impurity() const {
		auto sum = this->correct_no + this->wrong_no;
		return 1.0 - qpl::square(qpl::f64_cast(this->correct_no) / sum) - qpl::square(qpl::f64_cast(this->wrong_no) / sum);
	}
	qpl::f64 qpl::gini_entry::no_accuracy() const {
		auto sum = this->correct_no + this->wrong_no;
		return qpl::f64_cast(this->correct_no) / sum;
	}
	qpl::size qpl::gini_entry::total() const {
		auto yes_sum = this->correct_yes + this->wrong_yes;
		auto no_sum = this->correct_no + this->wrong_no;
		return yes_sum + no_sum;
	}
	void qpl::gini_entry::feed(bool guess, bool real) {
		if (guess && real)++this->correct_yes;
		else if (guess && !real)++this->wrong_yes;
		else if (!guess && real)++this->wrong_no;
		else ++this->correct_no;
	}
	void qpl::gini_entry::reset() {
		this->correct_yes = this->wrong_yes = this->correct_no = this->wrong_no = 0u;
	}
	qpl::f64 qpl::gini_entry::impurity() const {
		auto yes_sum = this->correct_yes + this->wrong_yes;
		auto no_sum = this->correct_no + this->wrong_no;
		return (qpl::f64_cast(yes_sum) / (yes_sum + no_sum)) * this->yes_impurity() + (qpl::f64_cast(no_sum) / (yes_sum + no_sum)) * this->no_impurity();
	}
	qpl::f64 qpl::gini_entry::accuracy() const {
		auto yes_sum = this->correct_yes + this->wrong_yes;
		auto no_sum = this->correct_no + this->wrong_no;
		return (qpl::f64_cast(this->correct_yes) / yes_sum + qpl::f64_cast(this->correct_no) / no_sum) / 2;
		//return qpl::f64_cast(this->correct_no + this->correct_yes) / this->total();
	}
	bool qpl::gini_entry::positive() const {
		return this->accuracy() > 0.5;
	}
	std::string qpl::gini_entry::info_string() const {
		auto p11 = qpl::f64_cast(this->correct_yes) / this->total();
		auto p10 = qpl::f64_cast(this->wrong_yes) / this->total();
		auto p01 = qpl::f64_cast(this->wrong_no) / this->total();
		auto p00 = qpl::f64_cast(this->correct_no) / this->total();
		return qpl::to_string("[11: ", qpl::percentage_string(p11), " - 10: ", qpl::percentage_string(p10), " - 01: ", qpl::percentage_string(p01), " - 00: ", qpl::percentage_string(p00), " - 1-imp: ", this->yes_impurity(), " - 0-imp: ", this->no_impurity(), " - imp: ", this->impurity(), " - acc: ", qpl::percentage_string(this->accuracy()), " - total: ", this->total(), " - pos: ", this->positive(), "]");
	}

	std::string qpl::node_type::string(bool parantheses) const {

		if (this->type == decision_type::node_final_yes) {
			return "Y";
		}
		else if (this->type == decision_type::node_final_no) {
			return "N";
		}
		else {
			if (parantheses) {
				return qpl::to_string('[', this->index, ']');
			}
			else {
				return qpl::to_string(this->index);
			}
		}
	}

	void qpl::decision_tree::randomize_sub_size(bool is_node) {
		if (is_node) {
			this->sub_trees.resize(2);
			this->sub_trees[0].node.type = decision_type::node_final_yes;
			this->sub_trees[1].node.type = decision_type::node_final_no;
			this->node.type = decision_type::node;
		}
		else {
			auto rnd = qpl::random(0, 1);
			switch (rnd) {
			case 0:
				this->sub_trees.resize(1);
				this->sub_trees[0].node.type = decision_type::node_final_no;
				this->node.type = decision_type::node_yes;
				break;
			case 1:
				this->sub_trees.resize(1);
				this->sub_trees[0].node.type = decision_type::node_final_yes;
				this->node.type = decision_type::node_no;
				break;
			}
		}
	}
	void qpl::decision_tree::grow_sub(std::vector<qpl::u32>& indices, qpl::f64 node_chance) {
		if (indices.empty()) {
			return;
		}

		auto node = qpl::random_b(node_chance);
		this->randomize_sub_size(node);

		this->node.index = indices.back();
		indices.pop_back();
	}
	void qpl::decision_tree::grow_at_depth_helper(std::vector<qpl::u32>& indices, qpl::f64 node_chance, qpl::u32 depth, qpl::u32 current) {
		if (indices.empty()) {
			return;
		}

		++current;
		if (current == depth) {
			this->grow_sub(indices, node_chance);
		}
		else {
			for (auto& s : this->sub_trees) {
				s.grow_at_depth_helper(indices, node_chance, depth, current);
			}
		}
		--current;
	}
	void qpl::decision_tree::grow_at_depth(std::vector<qpl::u32>& indices, qpl::f64 node_chance, qpl::u32 depth) {
		qpl::u32 current = 0u;
		for (auto& s : this->sub_trees) {
			s.grow_at_depth_helper(indices, node_chance, depth, current);
			if (indices.empty()) {
				return;
			}
		}
	}
	void qpl::decision_tree::grow(std::vector<qpl::u32>& indices, qpl::f64 node_chance, qpl::u32 max_depth) {
		if (indices.empty()) {
			return;
		}


		this->node.index = indices.back();
		indices.pop_back();

		this->randomize_sub_size(true);

		for (qpl::u32 i = 1u; i < max_depth; ++i) {
			this->grow_at_depth(indices, node_chance, i);
			auto depth = this->depth();
			if (indices.empty() || i > depth) {
				return;
			}
		}
	}
	void qpl::decision_tree::depth_helper(qpl::size depth, qpl::size& max) const {
		++depth;
		max = qpl::max(depth, max);
		for (auto& s : this->sub_trees) {
			s.depth_helper(depth, max);
		}
		--depth;
	}
	qpl::size qpl::decision_tree::depth() const {
		qpl::size max = 0u;
		this->depth_helper(0u, max);
		return max;
	}
	qpl::size qpl::decision_tree::size() const {
		qpl::size sum = this->sub_trees.size() + 1;
		for (auto& i : this->sub_trees) {
			sum += i.size();
		}
		return sum;
	}
	void qpl::decision_tree::get_nodes_at_depth_helper(std::vector<node_type>& result, qpl::u32 depth, qpl::u32 current) {
		if (current == depth - 1) {

			if (this->sub_trees.size() != 2 && this->node.type == decision_type::node_yes) {
				node_type node;
				node.type = decision_type::node_final_yes;
				if (this->node.touched && (this->sub_trees.empty() || (!this->sub_trees[0].node.touched))) {
					node.touched = true;
				}
				result.push_back(node);
			}
			for (auto& n : this->sub_trees) {
				result.push_back(n.node);
			}
			if (this->sub_trees.size() != 2 && this->node.type == decision_type::node_no) {
				node_type node;
				node.type = decision_type::node_final_no;
				if (this->node.touched && (this->sub_trees.empty() || (!this->sub_trees[0].node.touched))) {
					node.touched = true;
				}
				result.push_back(node);
			}
		}
		else {
			++current;
			for (auto& s : this->sub_trees) {
				s.get_nodes_at_depth_helper(result, depth, current);
			}
			--current;
		}
	}
	std::vector<node_type> qpl::decision_tree::get_nodes_at_depth(qpl::u32 depth) {
		std::vector<node_type> result;
		if (!depth) {
			result.push_back(this->node);
			return result;
		}
		qpl::u32 current = 0u;

		this->get_nodes_at_depth_helper(result, depth, current);

		return result;
	}
	void qpl::decision_tree::string_helper(std::ostringstream& stream, qpl::u32 depth, qpl::u32 current) const {
		++current;
		if (current == depth) {
			bool first = true;

			if (this->node.type == decision_type::node_yes) {
				stream << 'Y';
				first = false;
			}

			for (auto& s : this->sub_trees) {
				if (!first) {
					stream << ' ';
				}
				first = false;

				stream << s.node.string();
			}

			if (this->node.type == decision_type::node_no) {
				if (!first) {
					stream << ' ';
				}
				stream << 'N';
			}
			if (!first) {
				stream << " | ";
			}
		}
		else {
			for (auto& s : this->sub_trees) {
				s.string_helper(stream, depth, current);
			}
		}
		--current;
	}
	std::string qpl::decision_tree::string() const {
		std::ostringstream stream;
		stream << "[" << this->node.index << "]\n";

		auto depth = this->depth();
		for (qpl::u32 i = 1; i < depth; ++i) {
			this->string_helper(stream, i, 0u);
			stream << '\n';
		}
		return stream.str();
	}
	void qpl::decision_tree::reset_touch() {
		this->node.touched = false;
		for (auto& i : this->sub_trees) {
			i.reset_touch();
		}
	}
	void qpl::decision_tree::add_leaves() {
		if (this->node.type != qpl::decision_type::node_final_no && this->node.type != qpl::decision_type::node_final_yes) {
			if (this->sub_trees.empty()) {
				this->sub_trees.resize(2);
				this->node.type = qpl::decision_type::node;
				this->sub_trees[0].node.type = qpl::decision_type::node_final_yes;
				this->sub_trees[1].node.type = qpl::decision_type::node_final_no;
			}
		}
		for (auto& i : this->sub_trees) {
			i.add_leaves();
		}
	}

}