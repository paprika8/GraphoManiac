#pragma once

#include "UIStructs.h"
#include "RGN.h"
#include "text_util/text_drawer.h"

#include <Windows.h>

#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>

#pragma comment (lib, "Gdiplus.lib")

namespace Graphs
{
	//перечисление кнопок мыши
	enum mouse_buttons
	{
		//правая кнопка
		R,
		//левая кнопка
		L,
		//кнопка колёсика
		M,
		//доп кнопка 1
		X1,
		//доп кнопка 2
		X2
	};
	//перечисление видов нажатий
	enum click_event
	{
		//нажатие кнопки
		down,
		//отпуск кнопки
		up,
		//дабл клик
		dbl,
		//удержание кнопки (не используется)
		wait
	};
	//структура информации о нажатии кнопки клавиатуры
	struct struct_key_event
	{
		//кол-во авто-нажатий кнопки клавиатуры
		int repeat : 16;
		//код кнопки клавиатуры
		int scan_code : 8;
		//расширенные кнопки alt и ctrl (правые кнопки)
		int is_extended : 1;
		//зарезервировано, не используется
		int reserve : 4;
		//зажат ли alt
		int is_alt : 1;
		//первое нажатие кнопки
		int is_first_message : 1;
		//последнее нажатие кнопки (кнопка отпущенна)
		int transition_state : 1;

		struct_key_event(int y) {
			int* uni = (int*)this;
			*uni = y;
		}
	};

	struct View;

	//Обеспечивает буферизацию отрисовки окна, создаёт буфер в конструкторе и применяет буфер в деструкторе (при удалении). Работает с WinApi. Обьяснение сложное
	struct BufferHDC
	{
		static std::deque<BufferHDC*> block;
		HDC src;
		HDC buffer = 0;
		Gdiplus::Graphics* graphic;
		int cx;
		int cy;
		Point_ offset = Point_(0, 0);
		BufferHDC(HDC asrc, Size_ size) {

			block.push_back(this);
			while(block[0] != this)
				Sleep(20);

			src = asrc;

			cx = size.width;
			cy = size.height;

			// Create new bitmap-back device context, large as the dirty rectangle.
			buffer = CreateCompatibleDC(asrc);
			hBmp = CreateCompatibleBitmap(asrc, cx, cy);
			hOldBmp = (HBITMAP)SelectObject(buffer, hBmp);

			// Do the painting into the memory bitmap.
			OffsetViewportOrgEx(buffer, 0, 0, //-( pPaintStruct->rcPaint.left ) , -( pPaintStruct->rcPaint.top ) , 
								&ptOldOrigin);

			//RECT rcPs = pPaintStruct->rcPaint;
			//that->getModel ()->VPaint ( hwnd , buffer , &pPaintStruct->rcPaint , TRUE , that );

			graphic = new Gdiplus::Graphics(buffer);
		}
		BufferHDC(HDC asrc, Size_ size, View*) {
			block.push_back(this);
			while(block[0] != this)
				Sleep(20);
			
			src = asrc;

			cx = size.width;
			cy = size.height;

			// Create new bitmap-back device context, large as the dirty rectangle.
			buffer = CreateCompatibleDC(asrc);
			hBmp = CreateCompatibleBitmap(asrc, cx, cy);
			hOldBmp = (HBITMAP)SelectObject(buffer, hBmp);
			BitBlt(buffer, 0, 0, //pPaintStruct->rcPaint.left , pPaintStruct->rcPaint.top ,
				   cx, cy, src, 0, 0, SRCCOPY);

			// Do the painting into the memory bitmap.
			OffsetViewportOrgEx(buffer, 0, 0, //-( pPaintStruct->rcPaint.left ) , -( pPaintStruct->rcPaint.top ) , 
								&ptOldOrigin);

			graphic = new Gdiplus::Graphics(buffer);
		}
		~BufferHDC() {
			if(!buffer)
				return;

			SetViewportOrgEx(buffer, ptOldOrigin.x, ptOldOrigin.y, NULL);

			delete graphic;

			// Blit the bitmap into the screen. This is really fast operation and although
			// the CustomPaint() can be complex and slow there will be no flicker any more.
			BitBlt(src, 0, 0, //pPaintStruct->rcPaint.left , pPaintStruct->rcPaint.top ,
				   cx, cy, buffer, 0, 0, SRCCOPY);

			// Clean up.
			SelectObject(buffer, hOldBmp);
			DeleteObject(hBmp);
			DeleteDC(buffer);

			block.pop_front();
		}
	private:
		HBITMAP hBmp;
		HBITMAP hOldBmp;
		POINT ptOldOrigin;
	};
	//элемент интерфейса
	struct View
	{
		Padding padding;
		Margin margin;
		Size_ size;
		View* parent = 0;
		//цвет фона
		Gdiplus::SolidBrush background = Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0));
		//регион, ограничивающий область обработку событий мыши
		RGN* rgn = new RGN_rect(0, 0, 0, 0);

		View(View* aparent) {
			parent = aparent;
		}

		virtual void child_deleted(View* child) {};

		~View() {
			if (parent) parent->child_deleted(this);
		}

		//функция проверки на принадлежность точки к элементу интерфейса через регион
		bool is_my(int X, int Y) {
			X -= abs_position.x;
			Y -= abs_position.y;
			//return X >= abs_position.x && Y >= abs_position.y && X < abs_position.x + abs_size.width && Y < abs_position.y + abs_size.height;
			return rgn->has_point(X, Y);
		}
		//функция обработки перемещения элемента интерфейса
		int move(int X, int Y);
		//функция отрисовки
		int paint(BufferHDC& hdc);
		//функция обработки изменения размера
		int resize(int width, int height);

		//функции обработки всяких событий
		//событие выхода курсора мыши из элемента
		virtual int mouse_leave_event() { return 0; };
		//событие перемещения мыши в окне
		virtual int mouse_move_event(int x, int y, int virtual_key) { return 0; };
		//событие нажатия мыши в элементе
		virtual int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) { return 0; };
		//событие скрола
		virtual int mouse_wheel_event(int x, int y, int delta, int virtual_key) { return 0; };
		//событие нажатия клавиши
		virtual int key_event(struct_key_event key, int virtual_key) { return 0; };
		//событие ввода букав с учётом зажатых shift и caps
		virtual int char_event(struct_key_event key, wchar_t c) { return 0; };
		//событие изменения размера элемента
		virtual int resize_event() {
			((RGN_rect*)rgn)->width = abs_size.width;
			((RGN_rect*)rgn)->height = abs_size.height;
			return 0;
		};
		//событие перемещения элемента
		virtual int move_event() { return 0; };

		//событие отрисовки контента элемента
		virtual int paint_event(BufferHDC& hdc) { return 0; };
		//событие отрисовки фона элемента
		virtual int paint_background_event(BufferHDC& hdc) {
			hdc.graphic->FillRectangle(&background, abs_position.x, abs_position.y, abs_size.width, abs_size.height);
			return 0;
		};
		//событие установки фокуса на элементе (например, нажатие кнопки мыши. обрабатывается в Composite). нужно для захвата мыши или клавиатуры
		virtual int set_focus_event() { return 0; };
		//событие удаления фокуса с элемента (например, нажатие кнопки мыши на другом элементе. обрабатывается в Composite)
		virtual int kill_focus_event() { return 0; };

		//функция захвата мыши
		virtual void capture(View* view) {
			parent->capture(view);
		}
		//функция освобождения мыши
		virtual void re_capture(View* view) {
			parent->re_capture(view);
		}

		//функция захвата событий клавиатуры
		virtual void key_capture(View* view) {
			parent->key_capture(view);
		}
		//функция освобождения событий клавиатуры
		virtual void key_re_capture(View* view) {
			parent->key_re_capture(view);
		}

		//получение абсолютного размера элемента
		Size_ get_abs_size() {
			return abs_size;
		}
		//функция получения размера контента интерфейса
		virtual Size_ get_content_size(Size_ size = Size_()) {
			return size;
		}
	protected:
		Point_ abs_position;
		Size_ abs_size;
		Padding abs_padding;
	};
	//view хранящий в себе другие view
	struct Composite : public View
	{
		//массив view
		std::vector<View*> children;
		//располагать в линию вертикально, иначе горизонтально
		bool is_vert_orientation = 0;

		Composite(View* aparent) : View(aparent) {

		}
		//добавление view
		void add(View* value);
		void insert(int index, View* value);
		//удаление view
		void remove(View* value);
		void remove(int index);

		void child_deleted(View* child) {
			remove(child);
		};

		Size_ get_content_size(Size_ size = Size_()) override {
			if (!(margin.type & MarginType::CONTENT))
				return this->size;
			Size_ res;
			Size_ asize;
			Margin amargin(0, 0, 0, 0);
			for (int i = 0; i < children.size(); i++) {
				asize = children[i]->get_content_size(children[i]->size).toAbsolut(size);
				amargin = children[i]->margin.toAbsolut(size);
				if (!is_vert_orientation)
					res = res.plusRight(asize, amargin);
				else
					res = res.plusBottom(asize, amargin);
			}
			(-abs_padding).reSize(res);
			return res;
		}

		~Composite() {
			for (int i = 0; i < children.size(); i++)
				delete children[i];
		}

		int mouse_leave_event() override { return 0; };
		int mouse_move_event(int x, int y, int virtual_key) override {
			View* v = 0;
			for (auto child : children)
				if (child->is_my(x, y)) {
					v = child;
					break;
				}
			if (have_mouse != v) {
				if (have_mouse) have_mouse->mouse_leave_event();
				have_mouse = v;
			}
			if (v)
				return v->mouse_move_event(x, y, virtual_key);
			return 0;
		};
		int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) override {
			if (have_mouse) have_mouse->set_focus_event();
			if (have_mouse) return have_mouse->mouse_event(button, type, x, y, virtual_key);
			return 0;
		};
		int mouse_wheel_event(int x, int y, int delta, int virtual_key) override {
			if (have_mouse) return have_mouse->mouse_wheel_event(x, y, delta, virtual_key);
			return 0;
		};
		int key_event(struct_key_event key, int virtual_key) override { return 0; };
		int char_event(struct_key_event key, wchar_t c) override { return 0; };
		int resize_event() override;
		int move_event() override;
		int paint_event(BufferHDC& hdc) override {
			for (auto x : children)
				x->paint(hdc);
			return 0;
		};
		int paint_background_event(BufferHDC& hdc) override { return 0; };
		int set_focus_event() override { return 0; };
		int kill_focus_event() override { return 0; };
	private:
		//int old_mouse_x, old_mouse_y;
		View* have_mouse = 0;
	};
}