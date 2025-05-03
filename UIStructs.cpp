#include "UIStructs.h"

namespace BoolApp {
	Size Size::plusRight(Size b, Margin m) {
		return Size((short)(width + b.width + m.left + m.right), std::max((int)height, b.height + m.top + m.bottom));
	}
	Size Size::minusRight(Size b, Margin m) {
		return Size((short)(width - b.width - m.left - m.right), height.value);
	}
	Size Size::plusBottom(Size b, Margin m) {
		return Size(std::max((int)width, b.width + m.left + m.right), (short)(height + b.height + m.top + m.bottom));
	}
	Size Size::minusBottom(Size b, Margin m) {
		return Size(width.value, (short)(height - b.height - m.top - m.bottom));
	}
	MarginType operator~(MarginType a) {
		return (MarginType)(~(char)a);
	}
	MarginType operator & (MarginType a, MarginType b) {
		return (MarginType)((char)a & (char)b);
	}
	MarginType operator & (MarginType a, int b) {
		return (MarginType)((char)a & b);
	}
	MarginType operator | (MarginType a, MarginType b) {
		return (MarginType)((char)a | (char)b);
	}
	MarginType operator | (MarginType a, int b) {
		return (MarginType)((char)a | b);
	}
}