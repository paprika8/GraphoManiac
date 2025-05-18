#include "functions.h"

namespace Graphs
{
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

	// Breadth First Search - обход в ширину
	/*
	void BFS(node* curr) {
		int counter = 1;
		int depth_lvl = 1;
		curr->mark = 'a' + counter;
		for (auto edge : curr->edges) {
			node* next = edge->get_node1() != curr ? edge->get_node1() : edge->get_node2();
			counter++;
			next->mark = counter + 'a';
		}
		for (auto edge : curr->edges) {
			node* next = edge->get_node1() != curr ? edge->get_node1() : edge->get_node2();
			BFS_rec(next, counter);
		}
	}

	void BFS_rec(node* current, int& counter) {
		for (auto edge : current->edges) {
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			counter++;
			next->mark = counter + 'a';
		}
		for (auto edge : current->edges) {
			if (counter >= current->gr->nodes.size()) {
				break;
			}
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			BFS_rec(next, counter);
		}
	}
	*/

	void BFS(node* curr) {
		int counter = 1;
		int depth_lvl = 1;
		int sz = curr->gr->nodes.size();
		curr->mark = 'a' + counter;

		while (counter < sz) {
			for (auto edge : curr->edges) {
				node* next = edge->get_node1() != curr ? edge->get_node1() : edge->get_node2();
				counter++;
				next->mark = counter + 'a';
			}
			int i = 1;
			while (i < depth_lvl) {
				// пу пу пу...
			}
		}
	}

	void BFS_check_one_lvl(node* current, int counter, int lvl_cnt) {
		for (auto edge : current->edges) {
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			counter++;
			next->mark = counter + 'a';
		}
	}
}