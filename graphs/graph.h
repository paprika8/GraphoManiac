#include "node_edge.h"

#include <math.h>
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


class graph
{
public:
    std::set<node> nodes;
    std::set<edge*> edges;

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

    void normalize(int distance){
        for(auto a: nodes){
            a.old_x = a.x;
            a.old_y = a.y;
        }
        for(auto a: nodes){
            int dx = 0, dy = 0;
            for(auto b: nodes){
                if(&a != &b){
                    int x = a.old_x - b.old_x;
                    int y = a.old_x - b.old_x;
                    
                    double d = std::sqrt(x*x + y*y);
                    int delta = d - distance;
                    delta /= 2;
                    dx += delta / d * x;
                    dy += delta / d * y;
                }
            }
            a.x += dx;
            a.y += dy;
        }
    }
};


class node
{
    friend class edge;
private:
    std::set<edge*> edges;
    graph* gr = 0;
public:
    int id;
    char mark; // Для маркировки

    int x, y;
    int old_x, old_y;

    node() : id(0), mark(0) {};

    node(int i, char m) : id(i), mark(m) {};

    ~node() {
        for (auto el : edges) delete el;
    }

    void create_edge(node* other) {
        edge* ed = new edge(this, other, 0);
        edges.insert(ed);
        other->edges.insert(ed);
        if(gr) gr->edges.insert(ed);
    }
};