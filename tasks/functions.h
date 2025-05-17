#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include <set>

namespace Graphs
{
    // Depth First Search - обход в глубину
    void DFS(node* first) {
        bool deadlocked = false;
        for (auto edge : first->edges) {
            node* next = edge->get_node1() == first ? edge->get_node2() : edge->get_node1();

            while (next != first) {
                if (next->mark != 'c') {
                    next->mark = 'c';

                }
            }
        }
    }
}
