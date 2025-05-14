#include "graph.h"
namespace Graphs
{

	std::function<bool(const node&, const node&)> cmp = [](const node& a, const node& b) { return a.id < b.id; };

	void edge::draw(Graphs::BufferHDC& hdc) {
		Gdiplus::Color col(255, 80, 80);
		Gdiplus::Pen* pen = new Gdiplus::Pen(col, 5.);
		hdc.graphic->DrawLine(0, point1->x, point1->y, point2->x, point2->y);
	}

	edge::~edge() {
		point1->edges.erase(this);
		point2->edges.erase(this);
		point1->gr->edges.erase(this);
	}

	void node::create_edge(node* other) {
		edge* ed = new edge(this, other, 0);
		edges.insert(ed);
		other->edges.insert(ed);
		if (gr) gr->edges.insert(ed);
	}

	void node::draw(Graphs::BufferHDC& hdc) {
		Gdiplus::Color col(255, 255, 80);
		Gdiplus::SolidBrush br = Gdiplus::SolidBrush(col);
		hdc.graphic->FillEllipse(&br, x, y, 50, 50);
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		br.SetColor(Color(0, 0, 0));

		Gdiplus::RectF rc(x, y, 50, 50);

		hdc.graphic->DrawString(std::to_wstring(id).c_str(), -1, get_font(12), rc, &format, &br);
	}

	void graph::draw(Graphs::BufferHDC& hdc) {
		for (auto x : edges)
			x->draw(hdc);
		for (auto x : nodes)
			x.draw(hdc);
	}
}