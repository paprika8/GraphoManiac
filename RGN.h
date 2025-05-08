#pragma once

#include <windows.h>   

#include <gdiplus.h>

using namespace Gdiplus;

namespace Graphs
{
	//Регион. Ограничивает зону работы елемента интерфейса
	struct RGN
	{
		virtual bool heve_point(int x, int y) = 0;

		//не работает
		virtual void set_clip(Graphics* g) = 0;
		//не работает
		virtual Gdiplus::Region* get_Region(Gdiplus::Region* full) = 0;
	};

	struct RGN_rect : public RGN
	{
		int x, y, width, height;
		RGN_rect(int ax, int ay, int awidth, int aheight) {
			x = ax;
			y = ay;
			width = awidth;
			height = aheight;
		}
		bool heve_point(int ax, int ay) {
			return ax >= x && ax < width + x && ay >= y && ay < height + y;
		}
		void set_clip(Graphics* g) override {
			Gdiplus::Region rgn(Gdiplus::Rect(x, y, width, height)); // Прямоугольник
			g->SetClip(&rgn);
		}
		Gdiplus::Region* get_Region(Gdiplus::Region* full) override {
			return new Gdiplus::Region(Gdiplus::Rect(x, y, width, height));;
		}
	};

	struct RGN_elips : public RGN
	{
		int x, y;
		double a, b; // y^2 / a^2 + x^2 / b^2 = 1
		RGN_elips(int ax, int ay, double aa, double ab) {
			x = ax;
			y = ay;
			a = aa;
			b = ab;
		}
		bool heve_point(int x, int y) override {
			x -= this->x;
			y -= this->y;
			return (y * y) / (a * a) + (x * x) / (b * b) <= 1;
		}
		void set_clip(Graphics* g) override {
			Gdiplus::Rect rect(x, y, a * 2, b * 2);
			Gdiplus::Region rectRegion(rect);
			Gdiplus::GraphicsPath ellipsePath;
			ellipsePath.AddEllipse(rect);
			rectRegion.Union(&ellipsePath);
			g->SetClip(&rectRegion);
		}
		Gdiplus::Region* get_Region(Gdiplus::Region* full) override {
			Gdiplus::Rect rect(x, y, a * 2, b * 2);
			Gdiplus::Region *rectRegion = new Gdiplus::Region(rect);
			Gdiplus::GraphicsPath ellipsePath;
			ellipsePath.AddEllipse(rect);
			rectRegion->Union(&ellipsePath);
			return rectRegion;
		}
	};

	struct RGN_and : public RGN
	{
		RGN* a, * b;
		RGN_and(RGN* aa, RGN* ab) {
			a = aa;
			b = ab;
		}
		bool heve_point(int x, int y);

		void set_clip(Graphics* g) override {
			Gdiplus::Region *fill = new Gdiplus::Region();
			g->GetClip(fill);
			Gdiplus::Region *r1 = a->get_Region(fill);
			Gdiplus::Region *r2 = b->get_Region(fill);

			r1->Intersect(r2);

			g->SetClip(r1);

			delete r1;
			delete r2;
		}
		Gdiplus::Region* get_Region(Gdiplus::Region* full) override {
			Gdiplus::Region *r1 = a->get_Region(full);
			Gdiplus::Region *r2 = b->get_Region(full);

			r1->Intersect(r2);

			delete r2;
			return r1;
		}

		virtual ~RGN_and() {
			delete a;
			delete b;
		}
	};

	struct RGN_or : public RGN
	{
		RGN* a, * b;
		RGN_or(RGN* aa, RGN* ab) {
			a = aa;
			b = ab;
		}
		bool heve_point(int x, int y);

		void set_clip(Graphics* g) override {
			Gdiplus::Region *fill = new Gdiplus::Region();
			g->GetClip(fill);
			Gdiplus::Region *r1 = a->get_Region(fill);
			Gdiplus::Region *r2 = b->get_Region(fill);

			r1->Union(r2);

			g->SetClip(r1);

			delete r1;
			delete r2;
		}
		Gdiplus::Region* get_Region(Gdiplus::Region* full) override {
			Gdiplus::Region *r1 = a->get_Region(full);
			Gdiplus::Region *r2 = b->get_Region(full);

			r1->Union(r2);

			delete r2;
			return r1;
		}

		virtual ~RGN_or() {
			delete a;
			delete b;
		}
	};

	struct RGN_not : public RGN
	{
		RGN* a;
		RGN_not(RGN* aa) {
			a = aa;
		}
		bool heve_point(int x, int y);

		void set_clip(Graphics* g) override {
			Gdiplus::Region *fill = new Gdiplus::Region();
			g->GetClip(fill);
			Gdiplus::Region *r1 = a->get_Region(fill);

			r1->Exclude(r1);

			g->SetClip(r1);

			delete r1;
		}
		Gdiplus::Region* get_Region(Gdiplus::Region* full) override {
			Gdiplus::Region *r1 = a->get_Region(full);

			r1->Exclude(r1);

			return r1;
		}

		virtual ~RGN_not() {
			delete a;
		}
	};
}