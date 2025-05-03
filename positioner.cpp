#include "positioner.h"

namespace BoolApp
{
	void PositioningOne(PComponent* comp) {
		if (!comp->getPView()) {
			return;
		}
		Size absolutParentSize = comp->getAbsoluteSize();// размер текущего эл
		Padding padding = comp->padding;
		padding = padding.toAbsolut(absolutParentSize);// паддинг
		Point cord = (0, 0);
		padding.reRect(cord, absolutParentSize);// применение паддинга

		Size absolutSize = comp->getPView()->size.toAbsolut(absolutParentSize);// размер дочернего
		MarginType marginType = (comp->margin.type & ~MarginType::PARENT) | (comp->getPView()->margin.type & MarginType::PARENT);
		comp->getPView()->margin.toAbsolut(absolutParentSize).reRect(cord, absolutParentSize, absolutSize, marginType);// получить координаты и размер для размещения
		comp->getPView()->Move(cord, absolutSize);
		Positioning(comp->getPView());
	}
	void PositioningFew(PComposite* compos) {
		Point cursor = (0, 0);
		Size size = compos->getAbsoluteSize();
		compos->padding.reRect(cursor, size);

		MarginType buf = compos->margin.type & MarginType::CONTENT;
		compos->margin.type = compos->margin.type | MarginType::CONTENT;
		Size contSize = compos->GetContentSize(size);
		compos->margin.type = compos->margin.type & ~MarginType::CONTENT | buf;
		Margin tempMargin = Margin(0, 0, 0, 0);
		Point start = (0, 0);
		Size buffer = Size(size);
		tempMargin.reRect(start, buffer, contSize, compos->margin.type);
		MarginType MBuffer = compos->margin.type;

		Size contentSize = size;

		if (start.x < cursor.x) {
			MBuffer = (MarginType)(MBuffer ^ MarginType::HCENTER | MarginType::LEFT);
		}
		if (start.y < cursor.y) {
			MBuffer = (MarginType)(MBuffer ^ MarginType::VCENTER | MarginType::TOP);
		}
		cursor.y = start.y;
		cursor.x = start.x;
		for (int cont = 0; cont < compos->len(); cont++) {
			Margin amargin = compos->getPView(cont)->margin.toAbsolut(size);
			//Size asize = compos->getPView(cont)->size.toAbsolut(size);
			Size asize = compos->getPView(cont)->GetContentSize(compos->getPView(cont)->size).toAbsolut(size);
			Point cord = cursor;
			MarginType marginType = MBuffer & ~MarginType::PARENT | (compos->getPView(cont)->margin.type & MarginType::PARENT);
			// amargin.reRect ( cord , contentSize , asize , marginType);
			cord.x += amargin.left;
			cord.y += amargin.top;
			compos->getPView(cont)->Move(cord, asize);
			Positioning(compos->getPView(cont));

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
	void Positioning(ProcessView* pv) {
		Positionable* posit = dynamic_cast<Positionable*>(pv);
		if (posit) {
			posit->position();
		}
		else {
			PComponent* comp = dynamic_cast<PComponent*>(pv);
			if (comp) {
				PositioningOne(comp);
			}
			else {
				PComposite* compos = dynamic_cast<PComposite*>(pv);
				if (compos) {
					PositioningFew(compos);
				}
			}
		}
	}
}