#pragma once

#include "../Views.h"
#include <string>
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
    public:
        std::set<edge*> edges;
		graph* gr = 0;

        int id;
        char mark = 'a'; // Для маркировки

        int x = 0, y = 0;
        int old_x = 0, old_y = 0;

        node(graph* agr) : id(0), mark(0), gr(agr) {};

        node(int i, char m, graph* agr) : id(i), mark(m), gr(agr) {};

        virtual ~node();

        void create_edge(node* other);

        virtual void draw(Graphs::BufferHDC& hdc);
    };

    class deikstra_node : public node
    {
    public:
        int value = 0;

        deikstra_node(graph* agr) : node(agr) {};

        deikstra_node(int i, char m, graph* agr) : node(i, m, agr) {};

        void draw(Graphs::BufferHDC& hdc) override;
    };

    class graph
    {
    public:
        //friend class node;
        //friend class edge;

        std::set<node*> nodes;
        std::set<edge*> edges;

        int node_radius = 50;
        double edge_width = 2.5;

        graph() {
            init_colors();
        };

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
        node* find(int id) {
            for (auto n : nodes) {
                if (n->id == id) {
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
        int value = 0;

        edge(node* p1, node* p2, int val, graph* agr) : value(val), point1(p1), point2(p2), gr(agr) {};

        // Дублирование ребра запрещено
        edge(edge& other) = delete;
        edge operator=(edge& other) = delete;

        virtual ~edge();

        void draw(Graphs::BufferHDC& hdc);

        node* get_node1() {
            return point1;
        }

        node* get_node2() {
            return point2;
        }
    };
}