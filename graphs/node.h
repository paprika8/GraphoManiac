#include <set>
#include "edge.h"

class node{
friend class edge;
private:
    std::set<edge*> edges;

public:
    int id;
    char mark; // Для маркировки

    node() : id(0), mark(0) {};

    node(int i, char m) : id(i), mark(m) {};

    ~node() {
        for (auto el : edges) delete el;
    }

    void create_edge(node* other){
        edge ed = edge(this, other, 0);
        edges.insert(ed);
        other.edges.insert(ed);
    }
}
    