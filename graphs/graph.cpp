#include "graph.h"
namespace Graphs
{

	std::vector<Color*> colors;

	Color text_color = Color(0, 0, 0);

	void edge::draw(Graphs::BufferHDC& hdc) {
		Gdiplus::Color col(255, 80, 80);
		Gdiplus::Pen pen = Gdiplus::Pen(col, gr->edge_width);
		int dradius = gr->node_radius / 2;
		hdc.graphic->DrawLine(&pen, (int)point1->x + dradius, point1->y + dradius, point2->x + dradius, point2->y + dradius);
		if (!value)
			return;
		int rx = (point1->x + point2->x) / 2 + dradius, ry = (point1->y + point2->y) / 2 + dradius;
		rx -= 15;
		ry -= 15;
		SolidBrush br(Color(255, 255, 255));
		hdc.graphic->FillEllipse(&br, rx, ry, gr->node_radius - 20, gr->node_radius - 20);
		Gdiplus::RectF rc(rx, ry, gr->node_radius - 20, gr->node_radius - 20);
		br.SetColor(Color(0, 0, 0));
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		hdc.graphic->DrawString(std::to_wstring(value).c_str(), -1, get_font(gr->text_size), rc, &format, &br);
	}

	edge::~edge() {
		point1->edges.erase(this);
		point2->edges.erase(this);
		gr->edges.erase(this);
	}

	node::~node() {
		for (auto el : edges)
			delete el;
	}

	edge* node::create_edge(node* other) {
		edge* ed = new edge(this, other, 0, gr);
		edges.insert(ed);
		other->edges.insert(ed);
		if (gr) gr->edges.insert(ed);
		return ed;
	}

	void node::draw(Graphs::BufferHDC& hdc) {
		int radius = gr->node_radius;

		Gdiplus::Color col(255, 255, 80);
		col = *colors[(char)(mark - 'a')];
		col = Gdiplus::Color(col.GetR() / 2, col.GetG() / 2, col.GetB() / 2);
		Gdiplus::SolidBrush br = Gdiplus::SolidBrush(col);
		hdc.graphic->FillEllipse(&br, x, y, radius, radius);

		br.SetColor(*colors[(char)(mark - 'a')]);
		hdc.graphic->FillEllipse(&br, x + 2, y + 2, radius - 4, radius - 4);

		/*Gdiplus::SolidBrush br = Gdiplus::SolidBrush(*colors[(char)(mark - 'a')]);
		hdc.graphic->FillEllipse(&br, x, y, radius, radius);*/

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		br.SetColor(text_color);

		Gdiplus::RectF rc(x, y, radius, radius);

		hdc.graphic->DrawString(std::to_wstring(id).c_str(), -1, get_font(gr->text_size), rc, &format, &br);
		rc.X += radius - 10;
		rc.Y += 10;
		rc.Width = 20;
		rc.Height = 20;
		br.SetColor(Color(128, 128, 128));
		hdc.graphic->FillEllipse(&br, rc.X, rc.Y, rc.Width, rc.Height);
		br.SetColor(Color(255, 255, 255));
		hdc.graphic->FillEllipse(&br, rc.X + 1, rc.Y + 1, rc.Width - 2, rc.Height - 2);
		br.SetColor(Color(0, 0, 0));
		int m = mark - 'a';
		wchar_t* char_ = std::to_wstring(m).data();
		hdc.graphic->DrawString(char_, -1, get_font(gr->text_size), rc, &format, &br);
	}

	bool node::is_neighbour(node* other) {
		for (auto edge : edges) {
			if (edge->point1 == other || edge->point2 == other) {
				return true;
			}
		}
		return false;
	}

	void deikstra_node::draw(Graphs::BufferHDC& hdc) {
		int radius = gr->node_radius;

		Gdiplus::Color col(255, 255, 80);
		col = *colors[(char)(mark - 'a')];
		col = Gdiplus::Color(col.GetR() / 2, col.GetG() / 2, col.GetB() / 2);
		Gdiplus::SolidBrush br = Gdiplus::SolidBrush(col);
		hdc.graphic->FillEllipse(&br, x, y, radius, radius);

		br.SetColor(*colors[(char)(mark - 'a')]);
		hdc.graphic->FillEllipse(&br, x + 2, y + 2, radius - 4, radius - 4);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		br.SetColor(Color(0, 0, 0));

		Gdiplus::RectF rc(x, y, radius, radius);

		hdc.graphic->DrawString(std::to_wstring(id).c_str(), -1, get_font(gr->text_size), rc, &format, &br);
		rc.X += radius - 10;
		rc.Y -= 10;
		rc.Width = 20;
		rc.Height = 20;
		br.SetColor(Color(128, 128, 128));
		hdc.graphic->FillEllipse(&br, rc.X, rc.Y, rc.Width, rc.Height);
		br.SetColor(Color(255, 255, 255));
		hdc.graphic->FillEllipse(&br, rc.X + 1, rc.Y + 1, rc.Width - 2, rc.Height - 2);
		br.SetColor(Color(0, 0, 0));
		wchar_t* char_ = std::to_wstring(value).data();
		hdc.graphic->DrawString(char_, -1, get_font(gr->text_size), rc, &format, &br);
	}

	int graph::normalize(int distance) {
		int minimal = distance;
		for (auto a : nodes) {
			a->old_x = a->x;
			a->old_y = a->y;
		}
		for (auto a : nodes) {
			double dx = 0, dy = 0;
			for (auto edg : a->edges) {
				node* b;
				if (edg->point1 == a)
					b = edg->point2;
				else
					b = edg->point1;
				if (a != b) {
					double x = a->old_x - b->old_x;
					double y = a->old_y - b->old_y;

					double d = std::sqrt(x * x + y * y);
					minimal = std::min(minimal, (int)d);
					double delta = d - distance;
					delta /= 8;
					if (delta < 2 && delta > -2)
						delta = 0;
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
			for (int i = 0; i < 255; i += 20) {
				colors.push_back(new Color(i, 80, i));
				colors.push_back(new Color(80, i, 80));
			}
		}
	}

}