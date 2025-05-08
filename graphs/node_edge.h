#include <set>

class edge;
class node;

class edge
{
private:
    node* point1;
    node* point2;
public:
    int value;

    edge(node* p1, node* p2, int val) : value(val), point1(p1), point2(p2) {};

    // Дублирование ребра запрещено
    edge(edge& other) = delete;
    edge operator=(edge& other) = delete;

    ~edge() {
        point1->edges.erase(this);
        point2->edges.erase(this);
    }

};

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
        edge *ed = new edge(this, other, 0);
        edges.insert(ed);
        other->edges.insert(ed);
    }
};
