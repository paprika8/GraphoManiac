#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include <set>

namespace Graphs
{
    void DFS(node* first) {
        std::set<int> checked_nodes;
        checked_nodes.insert(first->id);
        bool deadlocked = false;
        for (auto edge : first->edges) {
            node* next = edge->get_node1() == first ? edge->get_node2() : edge->get_node1();
            while (true) {

            }
        }
    }
}
