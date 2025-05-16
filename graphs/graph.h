#include "../Views.h"

#include <math.h>
#include <set>

namespace Graphs
{

    class edge;
    class node;
    class graph;


    class node
    {
        friend class edge;
    private:
        graph* gr = 0;
    public:
        int id;
        char mark; // Для маркировки

        std::set<edge*> edges;

        int x, y;
        int old_x, old_y;

        node() : id(0), mark(0) {};

        node(int i, char m) : id(i), mark(m) {};

        ~node() {
            for (auto el : edges) delete el;
        }

        void create_edge(node* other);

        void draw(Graphs::BufferHDC& hdc);
    };

    extern std::function<bool(const node&, const node&)> cmp;

    class graph
    {
    public:

        std::set<node, decltype(cmp)> nodes = std::set<node, decltype(cmp)>(cmp);
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

        void normalize(int distance) {
            for (auto a : nodes) {
                a.old_x = a.x;
                a.old_y = a.y;
            }
            for (auto a : nodes) {
                int dx = 0, dy = 0;
                for (auto b : nodes) {
                    if (&a != &b) {
                        int x = a.old_x - b.old_x;
                        int y = a.old_x - b.old_x;

                        double d = std::sqrt(x * x + y * y);
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

        void draw(Graphs::BufferHDC& hdc);
    };

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

        ~edge();

        void draw(Graphs::BufferHDC& hdc);

        node* get_node1(){
            return point1;
        }

        node* get_node2(){
            return point2;
        }
    };
}