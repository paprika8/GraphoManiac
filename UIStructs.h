#pragma once
#include <Windows.h>
#include <math.h>
#include <functional>
#include <string.h>
typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef unsigned short ushort;

namespace Graphs
{

	template<typename T>
	T min(T a, T b){
		if(a < b)
			return a;
		return b;
	}

	template<typename T>
	T max(T a, T b){
		if(a > b)
			return a;
		return b;
	}

	enum MarginType : char
	{
		CONTENT = 0b01000000,
		HPARENT = 0b00000001,
		VPARENT = 0b00000010,
		PARENT = 0b00000011,
		LEFT = 0b00001000,
		HCENTER = 0b00001100,
		RIGHT = 0b00000100,
		TOP = 0b00010000,
		VCENTER = 0b00110000,
		BOTTOM = 0b00100000

	};
	MarginType operator ~ (MarginType a);
	MarginType operator & (MarginType a, MarginType b);
	MarginType operator & (MarginType a, int b);
	MarginType operator | (MarginType a, MarginType b);
	MarginType operator | (MarginType a, int b);
	enum CalcType : unsigned short
	{
		px,
		percent,
		fr
	};

	struct Margin;
	struct Padding;

	struct pointUI
	{
		using enum CalcType;
		short value : 14;
		CalcType type : 2;

		pointUI() {
			value = 0;
			type = px;
		}
		pointUI(short avalue, CalcType atype = px) {
			value = avalue;
			type = atype;
		}
		pointUI toAbsolut(short original) {
			pointUI copy = pointUI(*this);
			switch (type) {
			case px: {
				return copy;
			}
			case percent: {
				return copy.value = round(original / 1000. * value);
			}
			case fr: {
				return copy.value = round(original / 1000. * value);
			}
			default: {
				return*this;
			}

			}

		};
		void operator=(short a) {
			value = a;
		}
		void operator+=(short a) {
			value += a;
		}
		void operator-=(short a) {
			value -= a;
		}

		pointUI operator-() {
			return pointUI(-value, type);
		}

		operator short() {
			return value;
		}
	};

	struct Point_
	{
		short x;
		short y;
		Point_() {
			x = 0;
			y = 0;
		}
		Point_(short ax, short ay) {
			x = ax;
			y = ay;
		}
		Point_(int l) {
			*((int*)this) = l;
		}
		int toInt() {
			return *((int*)this);
		}
	};

	struct Size_
	{
		using enum CalcType;
		pointUI width;
		pointUI height;

		Size_() {
			width.type = px;
			height.type = px;
			width = 0;
			height = 0;
		}
		Size_(short w, short h) {
			width.type = px;
			height.type = px;
			width = w;
			height = h;
		}
		Size_(pointUI w, pointUI h) {
			width = w;
			height = h;
		}
		Size_(int l) {
			*((int*)this) = l;
		}
		Size_ plusRight(Size_ b) {
			return Size_((short)width + b.width, std::max((short)height, (short)b.height));
		}
		Size_ plusRight(Size_ b, Margin m);
		Size_ minusRight(Size_ b, Margin m);
		Size_ plusBottom(Size_ b) {
			return Size_(std::max((short)width, (short)b.width), (short)height + b.height);
		}
		Size_ plusBottom(Size_ b, Margin margin);
		Size_ minusBottom(Size_ b, Margin margin);
		int toInt() {
			return *((int*)this);
		}

		Size_ toAbsolut(Size_ original) {
			if(width.type == fr){
				auto h = height.toAbsolut(original.height);
				return Size_(width.toAbsolut(h),
						h);
			}
			if(height.type == fr){
				auto w = width.toAbsolut(original.width);
				return Size_(w,
							height.toAbsolut(w));
			}
			return Size_(width.toAbsolut(original.width),
						height.toAbsolut(original.height));
		}

	};

	struct Padding
	{
		using enum CalcType;
		pointUI left;
		pointUI top;
		pointUI right;
		pointUI bottom;

		Padding() {
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}

		virtual void reRect(Point_& point, Size_& contentSize) {
			Padding abs = toAbsolut(contentSize);
			point.x += abs.left;
			contentSize.width -= abs.left + abs.right;
			point.y += abs.top;
			contentSize.height -= abs.top + abs.bottom;
		}
		virtual void reRect(RECT& rect) {
			Padding abs = toAbsolut(Size_(rect.right - rect.left, rect.bottom - rect.top));
			rect.left += abs.left;
			rect.right -= abs.right;
			rect.top += abs.top;
			rect.bottom -= abs.bottom;
		}

		virtual void reSize(Size_& size) {
			size.width -= left + right;
			size.height -= top + bottom;
		}
		virtual void plusSize(Size_& size) {
			size.width += left + right;
			size.height += top + bottom;
		}
		Padding(Padding& orig) {
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		};
		Padding(const Padding& orig) {
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		Padding(short aleft,
				short atop,
				short aright,
				short abottom) {
			right = aright;
			top = atop;
			left = aleft;
			bottom = abottom;
		}
		Padding(ull l) {
			*((ull*)this) = l;
		}
		ull toLong() {
			return *((ull*)this);
		}

		Padding toAbsolut(Size_ original) {
			return Padding(left.toAbsolut(original.width),
						   top.toAbsolut(original.height),
						   right.toAbsolut(original.width),
						   bottom.toAbsolut(original.height));
		}

		Padding operator-(){
			return Padding(-left, -top, -right, -bottom);
		}
	};

	struct Margin
	{
	public:
		using enum CalcType;
		pointUI left;
		pointUI top;
		pointUI right;
		pointUI bottom;
		MarginType type = MarginType::LEFT | MarginType::TOP;

		Margin() {
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}

		Margin(Margin& orig) {
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		Margin(const Margin& orig) {
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		Margin(short aleft,
			   short atop,
			   short aright,
			   short abottom,
			   MarginType atype = MarginType::LEFT | MarginType::TOP) {
			right = aright;
			top = atop;
			left = aleft;
			bottom = abottom;
			type = atype;
		}
		Margin(ull l) {
			*((ull*)this) = l;
		}
		ull toLong() {
			return *((ull*)this);
		}
		Margin toAbsolut(Size_ original) {
			return Margin(left.toAbsolut(original.width),
						  top.toAbsolut(original.height),
						  right.toAbsolut(original.width),
						  bottom.toAbsolut(original.height));
		}

		virtual void reRect(Point_& point, Size_& contentSize, Size_& thisSize, MarginType type) {
			short x = point.x;
			short y = point.y;
			ushort w = thisSize.width;
			ushort h = thisSize.height;
			if (type & HPARENT) {
				w = contentSize.width - left - right;
				x = point.x + left;
			}
			else {
				switch (type & HCENTER) {
				case LEFT:
					x = point.x + left;
					break;
				case RIGHT:
					x = point.x + contentSize.width - right - w;
					break;
				default:
					x = point.x + contentSize.width / 2. - thisSize.width / 2.;
					break;
				}
			}
			if (type & VPARENT) {
				h = contentSize.height - bottom - top;
				y = point.y + bottom;
			}
			else {
				switch (type & VCENTER) {
				case TOP:
					y = point.y + top;
					break;
				case BOTTOM:
					y = point.y + contentSize.height - h - bottom;
					break;
				default:
					y = top + point.y + contentSize.height / 2. - thisSize.height / 2.;
					break;
				}
			}
			point.x = x;
			point.y = y;
			thisSize.width = w;
			thisSize.height = h;
			contentSize.width = contentSize.width - w - right - left;
			contentSize.height = contentSize.height - h - top - bottom;
		}

		virtual void reSize(Size_& size) {
			size.width += left + right;
			size.height += top + bottom;
		}
	};
}