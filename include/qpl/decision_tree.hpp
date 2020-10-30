#ifndef QPL_DECISIONTREE_HPP
#define QPL_DECISIONTREE_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <vector>

namespace qpl {

	struct gini_entry {
		qpl::u32 correct_yes = 0u;
		qpl::u32 wrong_yes = 0u;

		qpl::u32 correct_no = 0u;
		qpl::u32 wrong_no = 0u;

		QPLDLL qpl::f64 yes_accuracy() const;
		QPLDLL qpl::f64 yes_impurity() const;
		QPLDLL qpl::f64 no_accuracy() const;
		QPLDLL qpl::f64 no_impurity() const;
		QPLDLL qpl::size total() const;
		QPLDLL void feed(bool guess, bool real);
		QPLDLL void reset();
		QPLDLL qpl::f64 impurity() const;
		QPLDLL qpl::f64 accuracy() const;
		QPLDLL bool positive() const;
		QPLDLL std::string info_string() const;
	};


	enum class decision_type {
		node, node_yes, node_no, node_final_yes, node_final_no
	};
	struct node_type {
		decision_type type = decision_type::node;
		qpl::u32 index = qpl::u32_max;
		bool touched = false;

		QPLDLL std::string string(bool parantheses = true) const;
	};


	struct decision_tree {

		QPLDLL void randomize_sub_size(bool is_node);
		QPLDLL void grow_sub(std::vector<qpl::u32>& indices, qpl::f64 node_chance);
		QPLDLL void grow_at_depth_helper(std::vector<qpl::u32>& indices, qpl::f64 node_chance, qpl::u32 depth, qpl::u32 current);
		QPLDLL void grow_at_depth(std::vector<qpl::u32>& indices, qpl::f64 node_chance, qpl::u32 depth);
		QPLDLL void grow(std::vector<qpl::u32>& indices, qpl::f64 node_chance, qpl::u32 max_depth = qpl::u32_max);
		QPLDLL void depth_helper(qpl::size depth, qpl::size& max) const;
		QPLDLL qpl::size depth() const;
		QPLDLL qpl::size size() const;
		QPLDLL void get_nodes_at_depth_helper(std::vector<node_type>& result, qpl::u32 depth, qpl::u32 current);
		QPLDLL std::vector<node_type> get_nodes_at_depth(qpl::u32 depth);
		QPLDLL void string_helper(std::ostringstream& stream, qpl::u32 depth, qpl::u32 current) const;
		QPLDLL std::string string() const;
		QPLDLL void reset_touch();
		QPLDLL void add_leaves();

		template<typename C>
		bool apply(const C& container) {
			this->node.touched = true;
			if (this->node.type == decision_type::node_final_no) {
				return false;
			}
			else if (this->node.type == decision_type::node_final_yes) {
				return true;
			}

			bool value = container[this->node.index];

			switch (this->node.type) {
			case decision_type::node:
				return this->sub_trees[!value].apply(container);
				break;
			case decision_type::node_no:
				if (!value) {
					return false;
				}
				return this->sub_trees[0].apply(container);
				break;
			case decision_type::node_yes:
				if (value) {
					return true;
				}
				return this->sub_trees[0].apply(container);
				break;
			}
		}


		node_type node;
		std::vector<decision_tree> sub_trees;
	};

}

#endif