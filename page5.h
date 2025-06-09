#pragma once

#include "consts.h"

#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "Text.h"
#include "tasks/functions.h"

namespace Graphs
{

	void create_p5() {
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
		comp1->size = Size_(pointUI(100 * 10, percent), pointUI(70 * 10, percent));
		comp1->margin.type = MarginType::TOP | MarginType::LEFT;
		comp1->is_vert_orientation = 1;
		comp1->background.SetColor(LIGHT);
		comp->add(comp1);

		Composite* comp2 = new Composite(comp);
		comp2->size = Size_(pointUI(100 * 10, percent), pointUI(30 * 10, percent));
		comp2->margin.type = MarginType::BOTTOM | MarginType::LEFT;
		comp2->is_vert_orientation = 1;
		comp2->background.SetColor(LIGHT);
		comp->add(comp2);

		Button* back_but = new Button(comp1);
		back_but->size = Size_(pointUI(4000, fr), pointUI(100, percent));
		back_but->margin = Margin(0, 0, pointUI(5, percent), 0);
		back_but->background.SetColor(BLUE);
		back_but->down_colour = DARK_BLUE;
		back_but->normal_colour = BLUE;
		// Меняем цвет обводки на белый
		// Меняем размер обводки на 5 пикселей (условно)
		// back_but->text = L"Назад";
		comp1->add(back_but);

		Button* re_but = new Button(comp1);
		re_but->size = Size_(pointUI(3600, fr), pointUI(100, percent));
		re_but->margin = Margin(0, 0, pointUI(5, percent), 0);
		re_but->background.SetColor(BLUE);
		re_but->down_colour = DARK_BLUE;
		re_but->normal_colour = BLUE;
		// Меняем цвет обводки на белый
		// Меняем размер обводки на 5 пикселей (условно)
		// back_but->text = L"Заново";
		comp1->add(re_but);

		Line_Text* condition = new Line_Text(comp1);
		condition->size = Size_(pointUI(100 * 10, percent), pointUI(30 * 10, percent));
		condition->margin = Margin(0, 10, pointUI(5, percent), 10, MarginType::CONTENT);
		condition->text = L"Задайте граф для вычисления количества компонент связности.";
		condition->text_size = 24;
		condition->text_brush.SetColor(WHITE);
		condition->background.SetColor(DARK);
		comp1->add(condition);

		Line_Text* output = new Line_Text(comp1);
		output->size = Size_(pointUI(100 * 10, percent), pointUI(60));
		output->margin = Margin(0, 10, pointUI(5, percent), 10, MarginType::CONTENT);
		output->text = L"Число компонент связности: ";
		output->text_size = 24;
		output->text_brush.SetColor(WHITE);
		output->background.SetColor(DARK);
		comp1->add(output);

		Button* check_but = new Button(comp2);
		check_but->size = Size_(pointUI(3000, fr), pointUI(25 * 10, percent));
		check_but->margin = Margin(0, 0, 0, 0);
		check_but->background.SetColor(BLUE);
		check_but->down_colour = DARK_BLUE;
		check_but->normal_colour = BLUE;
		comp2->add(check_but);

		//создаём текстовый блок
		GraphView* tx = new GraphView(screen);
		tx->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		tx->margin = Margin(10, 10, 10, 10, MarginType::PARENT);
		tx->background.SetColor(WHITE);

		check_but->click = [=](Button*)->void {
			int cnt = comp_cnt(&tx->gr);
			output->text = L"Число компонент связности: " + std::to_wstring(cnt);
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
			output->paint(hdc);
			};

		screen->add(comp);
		screen->add(tx);
	}
}