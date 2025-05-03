#include "node.h"

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

}