#include "RGN.h"

namespace Graphs
{

	bool RGN_and::has_point(int x, int y) {
		return a->has_point(x, y) && b->has_point(x, y);
	}

	bool RGN_or::has_point(int x, int y) {
		return a->has_point(x, y) || b->has_point(x, y);
	}

	bool RGN_not::has_point(int x, int y) {
		return !a->has_point(x, y);
	}

}
