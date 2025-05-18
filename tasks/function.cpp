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

	void comp_cnt(graph* gr) {
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
		//return cnt;
	}

}