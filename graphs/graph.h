#include "../Views.h"
#include <string>
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
    public:
				graph* gr = 0;
        int id;
        std::string mark = null; // Для маркировки

        std::set<edge*> edges;

        int x, y;
        int old_x, old_y;

        node(graph* agr) : id(0), mark(0), gr(agr) {};

        node(int i, char m, graph* agr) : id(i), mark(m), gr(agr) {};

        ~node() {
            for (auto el : edges) delete el;
        }

        void create_edge(node* other);

        void draw(Graphs::BufferHDC& hdc);
    };

    class graph
    {
    public:
        friend class node;
        friend class edge;

        std::set<node*> nodes;
        std::set<edge*> edges;

        int node_radius = 50;
        double edge_width = 2.5;

        graph() {};

        ~graph() {
            for (auto x : nodes) delete x;
        }

        void insert(node* n) {
            nodes.insert(n);
        }

        void erase(node* n) {
            nodes.erase(n);
            delete n;
        }

        node* find(int x, int y) {
            for (auto n : nodes) {
                int radius = node_radius / 2;

                int xx = n->x - x + radius;
                int yy = n->y - y + radius;


                if (radius * radius > xx * xx + yy * yy) {
                    return n;
                }
            }
            return 0;
        }

        void normalize(int distance) {
            for (auto a : nodes) {
                a->old_x = a->x;
                a->old_y = a->y;
            }
            for (auto a : nodes) {
                int dx = 0, dy = 0;
                for (auto b : nodes) {
                    if (&a != &b) {
                        int x = a->old_x - b->old_x;
                        int y = a->old_x - b->old_x;

                        double d = std::sqrt(x * x + y * y);
                        int delta = d - distance;
                        delta /= 2;
                        dx += delta / d * x;
                        dy += delta / d * y;
                    }
                }
                a->x += dx;
                a->y += dy;
            }
        }

        void draw(Graphs::BufferHDC& hdc);
    private:

    };

    class edge
    {
    private:
        graph* gr;
        node* point1;
        node* point2;
    public:
        int value;

        edge(node* p1, node* p2, int val, graph* agr) : value(val), point1(p1), point2(p2), gr(agr) {};

        // Дублирование ребра запрещено
        edge(edge& other) = delete;
        edge operator=(edge& other) = delete;

        ~edge();

        void draw(Graphs::BufferHDC& hdc);

        node* get_node1() {
            return point1;
        }

        node* get_node2() {
            return point2;
        }
    };
}