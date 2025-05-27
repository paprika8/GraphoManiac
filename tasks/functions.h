#pragma once

#include "../graphs/graph.h"
#include <set>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <map>



namespace Graphs
{
	// Depth First Search - обход в глубину
	void DFS(node* curr);

	void BFS(node* curr);

	int comp_cnt(graph* gr);

	bool has_cycle(graph* gr);

	bool is_tree(graph* gr);

	std::vector<int> codding_Prufer(graph& gr);

	void decodding_Prufer(std::vector<int>& prufer_code, graph& gr);

	void deikstra(graph* gr, std::vector<int>& start);

	std::wstring accept_9(graph* gr);

	void accept_12(graph* gr);

	void accept_7(graph* gr);
}
