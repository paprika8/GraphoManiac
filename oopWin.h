#pragma once
#include <Windows.h>
#include "UIStructs.h"
#include <set>
#include <string>
#include <functional>
#include <gdiplus.h>
//using namespace Gdiplus;

#pragma comment (lib, "Gdiplus.lib")

Gdiplus::Color operator-(Gdiplus::Color start, Gdiplus::Color DeltaColor);
Gdiplus::Color operator+(Gdiplus::Color start, Gdiplus::Color DeltaColor);
Gdiplus::Color operator-(Gdiplus::Color start, int Delta);
Gdiplus::Color operator+(Gdiplus::Color start, int Delta);

namespace Graphs
{

	extern HINSTANCE instance;
	extern std::set<std::wstring> isRegistry;
	class View;
	class ProcessView;
	void init(HINSTANCE ainstance);
	int run();
	LRESULT wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam);
	class PComposite;
	class PComponent;
	class Composite;
	class Component;

	class Builder
	{
	public:
		virtual void build(ProcessView* apv) = 0;
	};
	class ProcessView
	{
	public:
		HWND hwnd;
		View* view;
		ProcessView* parent = 0;
		Point point = Point(0, 0);
		Size size = Size(0, 0);
		Margin margin = Margin(0, 0, 0, 0);
		Padding padding = Padding(0, 0, 0, 0);
		ProcessView(HWND ahwnd, View* aview);
		virtual ~ProcessView();

		virtual void construction() = 0;

		virtual Size GetContentSize(Size size) {
			return size;
		}

		Size getAbsoluteSize() {
			LPRECT buf = (LPRECT)malloc(sizeof(RECT));
			GetClientRect(hwnd, buf);
			Size size = Size(buf->right, buf->bottom);
			free(buf);
			return size;
		}

		virtual void Move(Point apoint, Size asize) {
			point = apoint;
			MoveWindow(hwnd, point.x, point.y, asize.width, asize.height, 1);
		}
	protected:

	};

	class SizeBuilder : public Builder
	{
	public:
		void build(ProcessView* apv) override;

		SizeBuilder(Size asize, Margin amargin, Padding apadding) {
			size = asize;
			margin = amargin;
			padding = apadding;
		}

	private:
		Size size;
		Margin margin;
		Padding padding;
	};

	class DefaultBuilder : public Builder
	{
	public:
		void build(ProcessView* apv) override {
			apv->size = (100, 100);
		}
	};

	class View
	{
	public:
		ProcessView* PV = 0;
		View* parent = 0;
		Gdiplus::Color background = Gdiplus::Color(255, 0, 0, 0);
		std::function<void(ProcessView*, Point, Size)> resize = [](ProcessView*, Point, Size) -> void {};
		virtual std::wstring getSzWindowClass() = 0;
		virtual void Register(WNDCLASS&) {};
		virtual void paint(HWND hwnd) {
			PAINTSTRUCT pstruct;
			HDC hdc = BeginPaint(hwnd, &pstruct);
			Gdiplus::Graphics g(hdc);
			auto rcDirty = pstruct.rcPaint;
			Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(background);
			g.FillRectangle(brush, rcDirty.left, rcDirty.top, (int)(rcDirty.right - rcDirty.left), (int)(rcDirty.bottom - rcDirty.top));
			delete brush;
			EndPaint(hwnd, &pstruct);
		};
		virtual LRESULT wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam, ProcessView* ptr) { return DefWindowProc(ahwnd, message, wparam, lparam); };
		virtual void childDeleted(View*) {};
		void setBuilder(Builder* abuilder) {
			delete builder;
			builder = abuilder;
		}
		void Construct();

		virtual ProcessView* VConstruct(ProcessView* apv) = 0;

		void sregister() {
			int i = GetLastError();
			WNDCLASS wca = { 0 };
			// адрес ф-ции обработки сообщений
			wca.lpfnWndProc = (WNDPROC)Graphs::wndProc;
			// стиль окна
			wca.style = CS_HREDRAW | CS_VREDRAW;
			// дискриптор экземпляра приложения
			// название класса
			wca.hInstance = instance;
			wca.lpszClassName = getSzWindowClass().c_str();
			// загрузка курсора
			wca.hCursor = LoadCursor(NULL, IDC_ARROW);
			// загрузка цвета окон
			wca.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
			// резерв места для указателя на PV
			wca.cbWndExtra = sizeof(void*);
			Register(wca);
			// регистрация класса
			RegisterClass(&wca);
			i = GetLastError();
		}

		virtual ~View() {
			if (parent) {
				parent->childDeleted(this);
			}
			if (PV)
				PV->view = 0;
			disable();
		}

		View(Builder* abuilder = new DefaultBuilder()) {
			builder = abuilder;
		}

		virtual void enable();
		virtual void disable();

	protected:
		bool enabled = false;
		Builder* builder;

	};
	const std::function<void(ProcessView*, Point, Size)> right_form = [](ProcessView* pv, Point p, Size s) -> void {
		HRGN rgn = CreateRectRgn(0, 0, s.width, s.height);
		HRGN rgn2 = CreateRectRgn(0, 0, s.width - s.height, s.height);
		HRGN round_rgn = CreateEllipticRgn(s.width - s.height * 2, -s.height * 0.5, s.width, s.height * 1.5);
		CombineRgn(rgn, rgn, round_rgn, RGN_AND);
		CombineRgn(rgn, rgn, rgn2, RGN_OR);
		DeleteObject(round_rgn);
		pv->padding.right = s.height / 5;
		//HRGN rgn = CreateRectRgn(0, 0, 0 + s.width, 0 + s.height);
		SetWindowRgn(pv->hwnd, rgn, 1);
		};
	const std::function<void(ProcessView*, Point, Size)> left_form = [](ProcessView* pv, Point p, Size s) -> void {
		HRGN rgn = CreateRectRgn(0, 0, s.width, s.height);
		HRGN rgn2 = CreateRectRgn(s.height, 0, s.width, s.height);
		HRGN round_rgn = CreateEllipticRgn(0, -s.height * 0.5, s.height * 2, s.height * 1.5);
		CombineRgn(rgn, rgn, round_rgn, RGN_AND);
		CombineRgn(rgn, rgn, rgn2, RGN_OR);
		DeleteObject(round_rgn);
		pv->padding.right = s.height / 5;
		//HRGN rgn = CreateRectRgn(0, 0, 0 + s.width, 0 + s.height);
		SetWindowRgn(pv->hwnd, rgn, 1);
		};
}