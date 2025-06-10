#pragma once

#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "Text.h"
#include "tasks/functions.h"

namespace Graphs
{

	int parsing6(std::wstring str) {
		int res = 0;
		for (auto x : str) {
			if (x >= L'0' && x <= L'9') {
				res *= 10;
				res += x - L'0';
			}
		}
		return res;
	}

	void create_p6() {
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
		re_but->padding = Padding(5, 5, 5, 5);
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
		condition->text = L"Введите число компонент связности для графа";
		condition->text_size = 11 * condition->size.height * 0.01;
		condition->text_brush.SetColor(WHITE);
		condition->background.SetColor(DARK);
		comp1->add(condition);

		std::wstring inp_win_text = L"Число компонент связности: ";

		Edit_Text* input = new Edit_Text(comp1);
		input->size = Size_(pointUI(100 * 10, percent), pointUI(60));
		input->margin = Margin(0, 10, pointUI(5, percent), 10, MarginType::CONTENT);
		input->text = inp_win_text;
		input->text_size = 24 * input->size.height * 0.01;
		input->text_brush.SetColor(WHITE);
		input->background.SetColor(DARK);
		comp1->add(input);

		Button* check_but = new Button(comp2);
		check_but->size = Size_(pointUI(3600, fr), pointUI(75 * 10, percent));
		check_but->margin = Margin(0, 0, 0, 0);
		check_but->padding = Padding(5, 5, 5, 5);
		check_but->background.SetColor(BLUE);
		check_but->down_colour = DARK_BLUE;
		check_but->normal_colour = BLUE;
		check_but->text_size = 24 * check_but->size.height * 0.001;
		check_but->text = L"Проверить";
		comp2->add(check_but);

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

		check_but->click = [=](Button*)->void {
			int cnt = comp_cnt(&tx->gr);
			std::wstring inp_num = input->text;
			inp_num.erase(inp_num.begin(), inp_num.begin() + inp_win_text.size());
			if (inp_num == std::to_wstring(cnt)) {
				colors[0] = new Gdiplus::Color(0, 255, 0);
				text_color = Color(0, 0, 0);
			}
			else {
				colors[0] = new Gdiplus::Color(255, 0, 0);
				text_color = Color(255, 255, 255);
			}
			for (auto node : tx->gr.nodes) {
				node->mark = 'a';
			}

			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
			input->paint(hdc);
			};

		screen->add(comp);
		b2->add(tx);
		screen->add(b1);
	}
}