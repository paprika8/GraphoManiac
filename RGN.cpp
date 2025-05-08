#include "RGN.h"

namespace Graphs
{

	bool RGN_and::heve_point(int x, int y) {
		return a->heve_point(x, y) && b->heve_point(x, y);
	}

	bool RGN_or::heve_point(int x, int y) {
		return a->heve_point(x, y) || b->heve_point(x, y);
	}

	bool RGN_not::heve_point(int x, int y) {
		return !a->heve_point(x, y);
	}

}
