#include "../Views.h"

#include <math.h>
#include <set>

namespace Graphs
{

    class edge;
    class node;
    class graph;

    extern std::vector<Color*> colors;

    void init_colors();

    class node
    {
        friend class edge;
    private:
        
        graph* gr = 0;
    public:
        std::set<edge*> edges;

        int id;
        char mark; // Для маркировки

        int x, y;
        int old_x = 0, old_y = 0;

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

        graph() {
            init_colors();
        };

        ~graph() {
            for (auto x : nodes) delete x;
            nodes.clear();
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

        int normalize(int distance);

        void draw(Graphs::BufferHDC& hdc);
    private:

    };

    class edge
    {
    private:
        graph* gr;
    public:
        node* point1;
        node* point2;
        int value;

        edge(node* p1, node* p2, int val, graph* agr) : value(val), point1(p1), point2(p2), gr(agr) {};

        // Дублирование ребра запрещено
        edge(edge& other) = delete;
        edge operator=(edge& other) = delete;

        ~edge();

        void draw(Graphs::BufferHDC& hdc);
    };
}