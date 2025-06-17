#pragma once

#include "start_page.h"
#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "Text.h"
#include "tasks/functions.h"
#include "positioner.h"

namespace Graphs
{

	std::vector<int> parsing10_11(std::wstring str) {
		std::vector<int> res;
		int i = 0;
		for (auto x : str) {
			if (x >= L'0' && x <= L'9') {
				i *= 10;
				i += x - L'0';
			}
			else {
				if (i) {
					res.push_back(i);
					i = 0;
				}
			}
		}
		if (i) {
			res.push_back(i);
			i = 0;
		}
		return res;
	}

	void create_p10_11() {
		Composite* screen = dynamic_cast<Composite*>(win->screen);
		screen->background.SetColor(LIGHT);
		while (screen->children.size()) {
			delete screen->children.back();
		}

		//задаём выравнивание дочерних элементов по центру
		screen->margin.type |= MarginType::HCENTER | MarginType::VCENTER;

		Composite* comp = new Composite(screen);
		comp->size = Size_(pointUI(30 * 10, percent), pointUI(100 * 10, percent));
		comp->margin.type = MarginType::LEFT;
		comp->is_vert_orientation = 1;
		comp->background.SetColor(LIGHT);

		Composite* comp1 = new Composite(comp);
		comp1->size = Size_(pointUI(100 * 10, percent), pointUI(90 * 10, percent));
		comp1->margin.type = MarginType::TOP | MarginType::LEFT;
		comp1->is_vert_orientation = 1;
		comp1->background.SetColor(LIGHT);
		comp->add(comp1);

		Composite* comp2 = new Composite(comp);
		comp2->size = Size_(pointUI(100 * 10, percent), pointUI(10 * 10, percent));
		comp2->margin.type = MarginType::BOTTOM | MarginType::LEFT;
		comp2->is_vert_orientation = 1;
		comp2->background.SetColor(LIGHT);
		comp->add(comp2);

		Button* back_but = new Button(comp1);
		back_but->size = Size_(pointUI(4000, fr), pointUI(78, percent));
		back_but->margin = Margin(0, 0, pointUI(5, percent), 0);
		back_but->padding = Padding(5, 5, 5, 5);
		back_but->background.SetColor(BLUE);
		back_but->down_colour = DARK_BLUE;
		back_but->normal_colour = BLUE;
		// Меняем цвет обводки на белый
		// Меняем размер обводки на 5 пикселей (условно)
		back_but->text_size = 24 * back_but->size.height * 0.01;
		back_but->text = L"Назад";
		comp1->add(back_but);

		Button* re_but = new Button(comp1);
		re_but->size = Size_(pointUI(3600, fr), pointUI(78, percent));
		re_but->margin = Margin(0, 0, pointUI(5, percent), 0);
		re_but->padding = Padding(5, 0, 5, 5);
		re_but->background.SetColor(BLUE);
		re_but->down_colour = DARK_BLUE;
		re_but->normal_colour = BLUE;
		// Меняем цвет обводки на белый
		// Меняем размер обводки на 5 пикселей (условно)
		re_but->text_size = 24 * re_but->size.height * 0.01;
		re_but->text = L"Заново";
		comp1->add(re_but);

		Line_Text* condition = new Line_Text(comp1);
		condition->size = Size_(pointUI(100 * 10, percent), pointUI(15 * 10, percent));
		condition->margin = Margin(0, 10, pointUI(5, percent), 10, MarginType::CONTENT);
		condition->text = L"Введите идентификаторы вершин для построения дерева по коду Прюфера.\nНажмите \"Кодировать\" для получения кода\nНажмите \"Декодировать\" для получения графа";
		condition->text_size = 11 * condition->size.height * 0.01;
		condition->text_brush.SetColor(WHITE);
		condition->background.SetColor(DARK);
		comp1->add(condition);

		Button* check_but1 = new Button(comp2);
		check_but1->size = Size_(pointUI(3600, fr), pointUI(75 * 10, percent));
		check_but1->margin = Margin(0, 0, 0, 0);
		check_but1->padding = Padding(5, 5, 5, 5);
		check_but1->background.SetColor(BLUE);
		check_but1->down_colour = DARK_BLUE;
		check_but1->normal_colour = BLUE;
		check_but1->text_size = 24 * check_but1->size.height * 0.001;
		check_but1->text = L"Кодировать";
		comp2->add(check_but1);

		Button* check_but2 = new Button(comp2);
		check_but2->size = Size_(pointUI(3600, fr), pointUI(75 * 10, percent));
		check_but2->margin = Margin(0, 0, 0, 0);
		check_but2->padding = Padding(5, 5, 5, 5);
		check_but2->background.SetColor(BLUE);
		check_but2->down_colour = DARK_BLUE;
		check_but2->normal_colour = BLUE;
		check_but2->text_size = 24 * check_but2->size.height * 0.001;
		check_but2->text = L"Декодировать";
		comp2->add(check_but2);

		//создаём текстовый блок

		Composite* b1 = new Composite(comp);
		b1->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		b1->margin = Margin(10, 10, 10, 10, MarginType::PARENT);
		b1->margin.type |= MarginType::PARENT;
		b1->is_vert_orientation = 1;
		b1->background.SetColor(WHITE);
		Composite* b2 = new Composite(b1);
		b2->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		b2->margin = Margin(5, 5, 5, 5, MarginType::PARENT);
		b2->margin.type |= MarginType::PARENT;
		b2->is_vert_orientation = 1;
		b2->background.SetColor(DARK);
		b1->add(b2);

		GraphView* tx = new GraphView(b2);
		tx->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		tx->margin = Margin(2, 2, 2, 2, MarginType::PARENT);
		tx->background.SetColor(WHITE);

		std::wstring inp_win_text = L"Код Прюфера: ";

		Edit_Text* input = new Edit_Text(comp1);
		input->size = Size_(pointUI(100 * 10, percent), pointUI(60));
		input->margin = Margin(0, 10, pointUI(5, percent), 10, MarginType::CONTENT);
		input->text = inp_win_text;
		input->text_size = 11 * condition->size.height * 0.01;
		input->text_brush.SetColor(WHITE);
		input->background.SetColor(DARK);
		comp1->add(input);

		check_but1->click = [=](Button*)->void {
			std::wstring str = L"";
			std::vector<int> res = codding_Prufer(tx->gr);
			for (auto i : res)
				str += std::to_wstring(i) + L" ";
			input->text = inp_win_text + str;

			for (auto key : tx->lock_ids) {
				key.second = 0;
			}

			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
			input->paint(hdc);
			};
		check_but2->click = [=](Button*)->void {
			std::wstring str = input->text;

			std::vector<int> res = parsing10_11(str);
			decodding_Prufer(res, tx->gr);
			for (auto i : res)
				str += std::to_wstring(i);
			input->text = inp_win_text + L"";

			for (auto key : tx->gr.nodes) {
				tx->lock_ids[key->id] = 1;
			}

			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
			input->paint(hdc);
			};

		re_but->click = [=](Button*)->void {
			create_p10_11();
			};

		back_but->click = [=](Button*)->void {
			create_ps();
			};

		screen->add(comp);
		b2->add(tx);
		screen->add(b1);
	}
}