#include "graph.h"
namespace Graphs
{

	std::vector<Color*> colors;

	void edge::draw(Graphs::BufferHDC& hdc) {
		Gdiplus::Color col(255, 80, 80);
		Gdiplus::Pen pen = Gdiplus::Pen(col, gr->edge_width);
		int dradius = gr->node_radius / 2;
		hdc.graphic->DrawLine(&pen, point1->x + dradius, point1->y + dradius, point2->x + dradius, point2->y + dradius);
	}

	edge::~edge() {
		point1->edges.erase(this);
		point2->edges.erase(this);
		point1->gr->edges.erase(this);
	}

	void node::create_edge(node* other) {
		edge* ed = new edge(this, other, 0, gr);
		edges.insert(ed);
		other->edges.insert(ed);
		if (gr) gr->edges.insert(ed);
	}

	void node::draw(Graphs::BufferHDC& hdc) {
		int radius = gr->node_radius;
		//Gdiplus::Color col(255, 255, 80);
		Gdiplus::SolidBrush br = Gdiplus::SolidBrush(*colors[(char)(mark - 'a')]);
		hdc.graphic->FillEllipse(&br, x, y, radius, radius);
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		br.SetColor(Color(0, 0, 0));

		Gdiplus::RectF rc(x, y, radius, radius);

		hdc.graphic->DrawString(std::to_wstring(id).c_str(), -1, get_font(12), rc, &format, &br);
	}

	int graph::normalize(int distance) {
		int minimal = distance;
		for (auto a : nodes) {
			a->old_x = a->x;
			a->old_y = a->y;
		}
		for (auto a : nodes) {
			int dx = 0, dy = 0;
			for (auto edg : a->edges) {
				node* b;
				if (edg->point1 == a)
					b = edg->point2;
				else
					b = edg->point1;
				if (a != b) {
					int x = a->old_x - b->old_x;
					int y = a->old_y - b->old_y;

					double d = std::sqrt(x * x + y * y);
					minimal = std::min(minimal, (int)d);
					int delta = d - distance;
					delta /= 8;
					dx -= delta / d * x;
					dy -= delta / d * y;
				}
			}
			a->x += dx;
			a->y += dy;
		}
		return minimal;
	}

	void graph::draw(Graphs::BufferHDC& hdc) {
		for (auto x : edges)
			x->draw(hdc);
		for (auto x : nodes)
			x->draw(hdc);
	}
	void init_colors() {
		if (!colors.size()) {
			colors.push_back(new Color(255, 255, 80));
			colors.push_back(new Color(255, 80, 80));
			colors.push_back(new Color(255, 80, 255));
			colors.push_back(new Color(255, 255, 80));
			colors.push_back(new Color(255, 255, 255));
			colors.push_back(new Color(80, 255, 255));
			colors.push_back(new Color(80, 80, 255));
			colors.push_back(new Color(80, 80, 80));
			for(int i = 0; i < 255; i+= 20){
				colors.push_back(new Color(i, 80, i));
				colors.push_back(new Color(80, i, 80));
			}
		}
	}
}