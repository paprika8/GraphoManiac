#include "functions.h"

namespace Graphs
{
	void DFS_rec(node* current, int& counter) {
		for (auto edge : current->edges) {
			if (counter >= current->gr->nodes.size()) {
				break;
			}
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			if (next->mark == 'a') {
				node* new_current = next;
				counter++;
				new_current->mark = counter + 'a';
				DFS_rec(new_current, counter);
			}
		}
	}

	void DFS(node* curr) {
		int counter = 1;
		curr->mark = 'a' + counter;
		for (auto edge : curr->edges) {
			node* next = edge->get_node1() != curr ? edge->get_node1() : edge->get_node2();
			if (next->mark == 'a') {
				node* new_curr = next;
				counter++;
				new_curr->mark = counter + 'a';
				DFS_rec(new_curr, counter);
			}
		}
	}

	// Breadth First Search - обход в ширину

	void BFS(node* curr) {
		int counter = 1;
		int depth_cnt = 1;
		int sz = curr->gr->nodes.size();
		curr->mark = 'a' + counter;
		counter++;

		std::vector<node*> cur_lvl = { curr };
		std::vector<node*> next_lvl;

		//while (counter < sz) {
		while (!cur_lvl.empty()) {
			for (auto cur_node : cur_lvl) {
				for (auto edge : cur_node->edges) {
					node* next = edge->get_node1() != cur_node ? edge->get_node1() : edge->get_node2();
					if (next->mark == 'a') {

						next->mark = counter + 'a';
						next_lvl.push_back(next);
					}
				}
			}
			counter++;
			cur_lvl = next_lvl;
			next_lvl.clear();
		}
	}

	void comp_rec(node* current, int& counter) {
		for (auto edge : current->edges) {
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			if (next->mark == 'a') {
				node* new_current = next;
				new_current->mark = counter + 'a';
				comp_rec(new_current, counter);
			}
		}
	}

	int comp_cnt(graph* gr) {
		int cnt = 0;
		for (auto cur_node : gr->nodes) {
			if (cur_node->mark == 'a') {
				cnt++;
				cur_node->mark = cnt + 'a';
			}
			else {
				continue;
			}
			for (auto edge : cur_node->edges) {
				node* next = edge->get_node1() != cur_node ? edge->get_node1() : edge->get_node2();
				if (next->mark == 'a') {
					next->mark = cnt + 'a';
					comp_rec(cur_node, cnt);
				}
			}

		}
		return cnt;
	}

	bool has_cycle(graph* gr) {
		bool res = false;
		for (auto cur_node : gr->nodes) {
			for (auto edge : cur_node->edges) {
				node* next = edge->get_node1() != cur_node ? edge->get_node1() : edge->get_node2();
				next->mark = 't';
				has_cycle_rec(cur_node, next, res);
				if (res) {
					return res;
				}
			}
		}
		return res;
	}

	void has_cycle_rec(node* prev, node* current, bool& flag) {
		if (flag) {
			return;
		}
		for (auto edge : current->edges) {
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			if (next->mark != 't') {
				node* new_current = next;
				new_current->mark = 't';
				has_cycle_rec(current, new_current, flag);
			}
			else if (next != prev) {
				flag = true;
				return;
			}
		}
	}

	bool is_tree(graph* gr) {
		if (comp_cnt(gr) == 1)
			return !(has_cycle(gr));
		return false;
	}


	std::vector<int> codding_Prufer(graph gr) {
		if (!is_tree(&gr)) {
			throw std::_INVALID_ARGUMENT;
		}

		auto set_comparator = [](node* first, node* second) { return first->id < second->id; };
		std::set<node*, decltype(set_comparator)> st(set_comparator);

		std::vector<int> ans;
		for (auto node : gr.nodes) { st.insert(node); }
		int offset = 0;
		while (st.size() > 2) {
			node* cur_node = (*st.begin() + offset);
			if (cur_node->edges.size() == 1) {
				edge* edg = (*cur_node->edges.begin()); // Получение ребра 1-го эл-та сета
				node* neigh = edg->get_node1() != cur_node ? edg->get_node1() : edg->get_node2();
				ans.push_back(neigh->id);
				st.erase(st.begin(), ++st.begin());
				offset = 0;
				delete neigh;
			}
			else {
				offset++;
			}
		}

		return ans;
	}

	graph decodding_Prufer(std::vector<int>& prufer_code) {
		int n = prufer_code.size() + 2;
		std::vector<int> code_word;
		for (int i = n - 2; i > 0; --i) {
			if (prufer_code[i] > n) { // Не код Прюфера
				throw std::_INVALID_ARGUMENT;
			}
			code_word.push_back(prufer_code[i]);
		}

		graph res;
		// Ищем числа, которых нет в кодовом слове
		std::unordered_set<int> code(prufer_code.begin(), prufer_code.end());
		std::vector<int> anticode;

		for (int i = n; i > 0; --i) {
			if (code.find(i) == code.end()) {
				anticode.push_back(i);
			}
		}

		while (code_word.size() > 0) {
			int id1 = code_word[code_word.size() - 1];
			node a(id1, 'a', res);
			int id2 = anticode[code_word.size() - 1];
			node b(id2, 'a', res);
			a.create_edge(&b);
			code_word.pop_back();
			anticode.pop_back();
			code.erase(code.begin(), ++code.begin());
			if (code.find(id1)) { // сделать поиск, оптимизировать все это
				anticode.push_back(id1);
			}

		}
		return res;
	}

}