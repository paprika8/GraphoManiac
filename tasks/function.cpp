#include "functions.h"

namespace Graphs
{
	void DFS(node* curr) {
		int counter = 1;
		for(auto edge : curr->edges){
			node* next = edge->get_node1() != curr ? edge->get_node1() : edge->get_node2();
			if (next->mark == 'a'){
				curr = next;
				curr->mark = counter + 'a';
				counter++;
				DFS_rec(curr, counter);
			}
		}
	}
	void DFS_rec(node* current, int &counter){
		for(auto edge : current->edges){
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			if (next->mark == 'a'){
				current = next;
				current->mark = counter + 'a';
				counter++;
				DFS_rec(current, counter);
			}
		}
	}
}