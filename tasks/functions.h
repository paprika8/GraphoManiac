#pragma once

#include "../graphs/graph.h"
#include "../graphs/draw_graph.h"
#include <set>
#include <string>



namespace Graphs
{
	// Depth First Search - обход в глубину
	void DFS(node* curr);

	void BFS(node* curr);

	void comp_cnt(graph* gr);
}
