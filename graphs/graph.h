#include "node_edge.h"
#include <set>

class graph
{
private:
    std::set<node> nodes;

public:
    graph() {};

    ~graph() {
        nodes.clear();
    }

    void insert(node n) {
        nodes.insert(n);
    }

    void erase(node n) {
        nodes.erase(n);
    }
}