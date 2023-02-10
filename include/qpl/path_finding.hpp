#pragma once
#ifndef QPL_PATH_FINDING_HPP
#define QPL_PATH_FINDING_HPP

#include <qpl/vector.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/type_traits.hpp>
#include <queue>

namespace qpl {
	struct astar_node {
		qpl::vec2s position;
		std::shared_ptr<astar_node> parent;
		qpl::size g = 0;
		qpl::size h = 0;
		qpl::size f = 0;


		astar_node() {

		}
		astar_node(qpl::vec2s position, const std::shared_ptr<astar_node>& parent) {
			this->position = position;
			this->parent = parent;
		}
		bool operator==(const astar_node& other) const {
			return this->position == other.position;
		}

		bool operator<(const astar_node& other) const {
			return this->position.y < other.position.y || (this->position.y == other.position.y && this->position.x < other.position.x);
		}
	};


	struct bfs_node {
		qpl::vec2s position;
		std::shared_ptr<bfs_node> parent;

		bfs_node() {

		}
		bfs_node(qpl::vec2s position, const std::shared_ptr<bfs_node>& parent) {
			this->position = position;
			this->parent = parent;
		}
		bool operator==(const bfs_node& other) const {
			return this->position == other.position;
		}
		bool operator<(const bfs_node& other) const {
			return this->position.y < other.position.y || (this->position.y == other.position.y && this->position.x < other.position.x);
		}
	};

	template<bool allow_diagonal, typename T, typename F> requires (std::is_integral_v<T> && qpl::is_callable<F>())
	std::vector<qpl::vec2s> astar_path_finding(const std::vector<std::vector<T>>& maze, qpl::vec2s start, qpl::vec2s end, F valid_check) {
		if (maze.empty()) {
			return{};
		}

		auto width = qpl::signed_cast(maze[0].size());
		auto height = qpl::signed_cast(maze.size());

		std::vector<std::shared_ptr<qpl::astar_node>> queue;
		std::shared_ptr<qpl::astar_node> starting_node = std::make_shared<qpl::astar_node>(start, nullptr);
		queue.push_back(starting_node);

		std::unordered_set<qpl::vec2s> visited;

		constexpr auto direction = [&]() {
			if constexpr (allow_diagonal) {
				return std::array{
					qpl::vec2is{1, 0}, qpl::vec2is(0, 1), qpl::vec2is(-1, 0), qpl::vec2is(0, -1),
					qpl::vec2is(1, 1), qpl::vec2is(1, -1), qpl::vec2is(-1, 1), qpl::vec2is(-1, -1) };
			}
			else {
				return std::array{ qpl::vec2is{1, 0}, qpl::vec2is(0, 1), qpl::vec2is(-1, 0), qpl::vec2is(0, -1) };
			}
		}();

		while (!queue.empty()) {
			std::shared_ptr<qpl::astar_node> current_node = *queue.begin();
			qpl::size current_index = 0u;
			qpl::size ctr = 0u;
			for (auto& element : queue) {
				if (element->f < current_node->f) {
					current_node = element;
					current_index = ctr;
				}
				++ctr;
			}

			visited.insert(current_node->position);
			queue.erase(queue.begin() + current_index);

			if (current_node->position == end) {
				auto traverse = current_node;
				std::vector<qpl::vec2s> path;
				while (traverse) {
					path.push_back(traverse->position);
					traverse = traverse->parent;
				}
				std::ranges::reverse(path);
				return path;
			}

			std::vector<std::shared_ptr<qpl::astar_node>> children;
			for (auto& dir : direction) {
				auto check = qpl::vec2is(current_node->position) + dir;


				if (check.x >= 0 && check.x < width && check.y >= 0 && check.y < height) {
					bool valid = false;
					if constexpr (qpl::parameter_size(valid_check) == 2u) {
						valid = valid_check(maze[check.y][check.x], maze[current_node->position.y][current_node->position.x]);
					}
					else {
						valid = valid_check(maze[check.y][check.x]);
					}
					if (valid) {
						bool found = visited.find(check) != visited.cend();

						bool queue_found = false;
						for (auto& i : queue) {
							if (i->position == check) {
								queue_found = true;
								break;
							}
						}

						if (!found && !queue_found) {
							auto next = std::make_shared<qpl::astar_node>(check, current_node);
							children.push_back(next);
						}
					}
				}
			}

			for (auto& child : children) {
				auto x = qpl::signed_cast(child->position.x) - qpl::signed_cast(end.x);
				auto y = qpl::signed_cast(child->position.y) - qpl::signed_cast(end.y);

				child->g = current_node->g + 1u;
				child->h = x * x + y * y;
				child->f = child->g + child->h;

				queue.push_back(child);
			}
		}
		return {};
	}


	template<bool allow_diagonal, typename T, typename F> requires (std::is_integral_v<T> && qpl::is_callable<F>())
	std::vector<qpl::vec2s> bfs_path_finding(const std::vector<std::vector<T>>& maze, qpl::vec2s start, qpl::vec2s end, F valid_check) {
		if (maze.empty()) {
			return{};
		}
		auto width = qpl::signed_cast(maze[0].size());
		auto height = qpl::signed_cast(maze.size());

		std::queue<std::shared_ptr<qpl::bfs_node>> queue;
		auto starting_node = std::make_shared<qpl::bfs_node>(start, nullptr);
		queue.push(starting_node);

		std::unordered_set<qpl::vec2s> visited;
		visited.insert(start);

		constexpr auto direction = [&]() {
			if constexpr (allow_diagonal) {
				return std::array{
					qpl::vec2is{1, 0}, qpl::vec2is(0, 1), qpl::vec2is(-1, 0), qpl::vec2is(0, -1),
					qpl::vec2is(1, 1), qpl::vec2is(1, -1), qpl::vec2is(-1, 1), qpl::vec2is(-1, -1) };
			}
			else {
				return std::array{ qpl::vec2is{1, 0}, qpl::vec2is(0, 1), qpl::vec2is(-1, 0), qpl::vec2is(0, -1) };
			}
		}();

		while (!queue.empty()) {
			std::shared_ptr<qpl::bfs_node> current_node = queue.front();
			queue.pop();

			if (current_node->position == end) {
				auto traverse = current_node;
				std::vector<qpl::vec2s> path;
				while (traverse) {
					path.push_back(traverse->position);
					traverse = traverse->parent;
				}
				std::ranges::reverse(path);
				return path;
			}

			for (auto& dir : direction) {
				auto check = qpl::vec2is(current_node->position) + dir;

				if (check.x >= 0 && check.x < width && check.y >= 0 && check.y < height) {
					bool valid = false;
					if constexpr (qpl::parameter_size(valid_check) == 2u) {
						auto search_value = maze[check.y][check.x];
						auto previously_value = maze[current_node->position.y][current_node->position.x];
						valid = valid_check(search_value, previously_value);
					}
					else {
						auto search_value = maze[check.y][check.x];
						valid = valid_check(search_value);
					}
					if (valid) {
						bool found = visited.find(check) != visited.cend();
						if (!found) {
							auto next = std::make_shared<qpl::bfs_node>(check, current_node);
							queue.push(next);
							visited.insert(check);
						}
					}
				}
			}
		}
		return {};
	}

}

#endif