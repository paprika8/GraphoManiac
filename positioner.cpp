#include "positioner.h"
#include "Main.h"

namespace Graphs
{
	/*void PositioningOne(View* comp) {
		if (!comp->getPView()) {
			return;
		}
		Size_ absolutParentSize = comp->getAbsoluteSize();// размер текущего эл
		Padding padding = comp->padding;
		padding = padding.toAbsolut(absolutParentSize);// паддинг
		Point_ cord = (0, 0);
		padding.reRect(cord, absolutParentSize);// применение паддинга

		Size_ absolutSize = comp->getPView()->size.toAbsolut(absolutParentSize);// размер дочернего
		MarginType marginType = (comp->margin.type & ~MarginType::PARENT) | (comp->getPView()->margin.type & MarginType::PARENT);
		comp->getPView()->margin.toAbsolut(absolutParentSize).reRect(cord, absolutParentSize, absolutSize, marginType);// получить координаты и размер для размещения
		comp->getPView()->Move(cord, absolutSize);
		Positioning(comp->getPView());
	}*/

	void PositioningFew(Composite* compos, int deep) {
		Point_ cursor = (0, 0);
		Size_ size = compos->get_abs_size();
		compos->padding.reRect(cursor, size);

		MarginType buf = compos->margin.type & MarginType::CONTENT;
		compos->margin.type = compos->margin.type | MarginType::CONTENT;
		Size_ contSize = compos->get_content_size(size);
		compos->margin.type = compos->margin.type & ~MarginType::CONTENT | buf;
		Margin tempMargin = Margin(0, 0, 0, 0);
		Point_ start = (0, 0);
		Size_ buffer = Size_(size);
		tempMargin.reRect(start, buffer, contSize, compos->margin.type);
		MarginType MBuffer = compos->margin.type;

		Size_ contentSize = size;

		if (start.x < cursor.x) {
			MBuffer = (MarginType)(MBuffer ^ MarginType::HCENTER | MarginType::LEFT);
		}
		if (start.y < cursor.y) {
			MBuffer = (MarginType)(MBuffer ^ MarginType::VCENTER | MarginType::TOP);
		}
		cursor.y = start.y;
		cursor.x = start.x;
		for (int cont = 0; cont < compos->children.size(); cont++) {
			Margin amargin = compos->children[cont]->margin.toAbsolut(size);
			//Size_ asize = compos->getPView(cont)->size.toAbsolut(size);
			Size_ asize = compos->children[cont]->get_content_size(compos->children[cont]->size).toAbsolut(size);
			Point_ cord = cursor;
			MarginType marginType = MBuffer & ~MarginType::PARENT | (compos->children[cont]->margin.type & MarginType::PARENT);
			// amargin.reRect ( cord , contentSize , asize , marginType);
			cord.x += amargin.left;
			cord.y += amargin.top;
			compos->children[cont]->move(cord.x, cord.y);
			compos->children[cont]->resize(asize.width, asize.height);
			Positioning(compos->children[cont], deep);

			if (compos->is_vert_orientation) {
				if (marginType & MarginType::LEFT)
					contentSize = contentSize.minusBottom(asize, amargin);
				cursor.y += asize.height + amargin.top + amargin.bottom;
			}
			else {
				if (marginType & MarginType::TOP)
					contentSize = contentSize.minusRight(asize, amargin);
				cursor.x += asize.width + amargin.left + amargin.right;
			}
			contentSize = size;
		}
	}
	void Positioning(View* pv, int deep) {
		Positionable* posit = dynamic_cast<Positionable*>(pv);
		if (posit) {
			posit->position();
		}
		else {
			Composite* comp = dynamic_cast<Composite*>(pv);
			if (comp) {
				PositioningFew(comp, deep);
			}
		}
		if (!deep)
		{
			if(win)
				InvalidateRect(win->hwnd, 0, 0);
		}
		
	}
}