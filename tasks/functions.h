#pragma once

#include "../graphs/graph.h"
#include "../graphs/draw_graph.h"
#include <set>
#include <string>
#include <algorithm>
#include <unordered_set>



namespace Graphs
{
	// Depth First Search - обход в глубину
	void DFS(node* curr);

	void BFS(node* curr);

	int comp_cnt(graph* gr);

	bool has_cycle(graph* gr);

	bool is_tree(graph* gr);

	std::vector<int> codding_Prufer(graph gr);

	graph decodding_Prufer(std::vector<int>& prufer_code);
}
