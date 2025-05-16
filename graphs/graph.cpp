#include "graph.h"
namespace Graphs
{

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
		Gdiplus::Color col(255, 255, 80);
		Gdiplus::SolidBrush br = Gdiplus::SolidBrush(col);
		hdc.graphic->FillEllipse(&br, x, y, radius, radius);
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		br.SetColor(Color(0, 0, 0));

		Gdiplus::RectF rc(x, y, radius, radius);

		hdc.graphic->DrawString(std::to_wstring(id).c_str(), -1, get_font(12), rc, &format, &br);
	}

	void graph::draw(Graphs::BufferHDC& hdc) {
		for (auto x : edges)
			x->draw(hdc);
		for (auto x : nodes)
			x->draw(hdc);
	}
}