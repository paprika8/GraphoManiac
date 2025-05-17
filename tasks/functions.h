#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include <set>
#include <string>



namespace Graphs
{
	
	// Depth First Search - обход в глубину
	void DFS(node* first) {
		/*
		bool deadlocked = false;
		for (auto edge : first->edges) {
			node* curr = first;
			node* next;
			if (edge->get_node1() == first){
				next = edge->get_node2();
			}
			else{
				next = edge->get_node1();
			}

			while (next != first) {
					if (next->mark != 'c') {
							next->mark = 'c';

					}
			}
		}
		*/
		
		/*
		int counter = first->gr->nodes.size() - 1;
		first->mark = 'm';
		node* curr = first;
		while(counter > 0){
			for(auto edge : curr->edges){
				node* next = edge->get_node1() != curr ? edge->get_node1() : edge->get_node2();
				if (next->mark != 'm'){
					curr = next;
					curr->mark = 'm';
					counter--;
					break;
				}
				else{
					
				}

			}
		}
		*/
		//int sz = first->gr->nodes.size();
		int counter = 1;
		for(auto edge : curr->edges){
			node* next = edge->get_node1() != curr ? edge->get_node1() : edge->get_node2();
			if (next->mark == null){
				curr = next;
				curr->mark = std::to_string(counter);
				counter++;
				DFS_rec(curr, counter)
			}
		}
	}

	void DFS_rec(node* current, int &counter){
		for(auto edge : current->edges){
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			if (next->mark == null){
				current = next;
				current->mark = std::to_string(counter);
				counter++;
				DFS_rec(current, counter);
			}
		}
	}
}
