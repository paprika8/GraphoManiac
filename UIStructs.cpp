#include "UIStructs.h"

namespace Graphs {
	Size_ Size_::plusRight(Size_ b, Margin m) {
		return Size_((short)(width + b.width + m.left + m.right), std::max((int)height, b.height + m.top + m.bottom));
	}
	Size_ Size_::minusRight(Size_ b, Margin m) {
		return Size_((short)(width - b.width - m.left - m.right), height.value);
	}
	Size_ Size_::plusBottom(Size_ b, Margin m) {
		return Size_(std::max((int)width, b.width + m.left + m.right), (short)(height + b.height + m.top + m.bottom));
	}
	Size_ Size_::minusBottom(Size_ b, Margin m) {
		return Size_(width.value, (short)(height - b.height - m.top - m.bottom));
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