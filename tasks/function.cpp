#include "functions.h"
#include <format>
namespace Graphs
{
	void DFS_rec(node* current, int& counter) {
		for (auto edge : current->edges) {
			if (counter >= current->gr->nodes.size()) {
				break;
			}
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			if (next->mark == 'a') {
				node* new_current = next;
				counter++;
				new_current->mark = counter + 'a';
				DFS_rec(new_current, counter);
			}
		}
	}

	void DFS(node* curr) {
		int counter = 1;
		curr->mark = 'a' + counter;
		for (auto edge : curr->edges) {
			node* next = edge->get_node1() != curr ? edge->get_node1() : edge->get_node2();
			if (next->mark == 'a') {
				node* new_curr = next;
				counter++;
				new_curr->mark = counter + 'a';
				DFS_rec(new_curr, counter);
			}
		}
	}

	// Breadth First Search - обход в ширину

	void BFS(node* curr) {
		int counter = 1;
		int depth_cnt = 1;
		int sz = curr->gr->nodes.size();
		curr->mark = 'a' + counter;
		counter++;

		std::vector<node*> cur_lvl = { curr };
		std::vector<node*> next_lvl;

		//while (counter < sz) {
		while (!cur_lvl.empty()) {
			for (auto cur_node : cur_lvl) {
				for (auto edge : cur_node->edges) {
					node* next = edge->get_node1() != cur_node ? edge->get_node1() : edge->get_node2();
					if (next->mark == 'a') {

						next->mark = counter + 'a';
						next_lvl.push_back(next);
					}
				}
			}
			counter++;
			cur_lvl = next_lvl;
			next_lvl.clear();
		}
	}

	void comp_rec(node* current, int& counter) {
		for (auto edge : current->edges) {
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			if (next->mark == 'a') {
				node* new_current = next;
				new_current->mark = counter + 'a';
				comp_rec(new_current, counter);
			}
		}
	}

	int comp_cnt(graph* gr) {
		int cnt = 0;
		for (auto x : gr->nodes) {
			x->mark = 'a';
		}
		for (auto cur_node : gr->nodes) {
			if (cur_node->mark == 'a') {
				cnt++;
				cur_node->mark = cnt + 'a';
				comp_rec(cur_node, cnt);
			}
			else {
				continue;
			}
			/*for (auto edge : cur_node->edges) {
				node* next = edge->get_node1() != cur_node ? edge->get_node1() : edge->get_node2();
				if (next->mark == 'a') {
					next->mark = cnt + 'a';
					comp_rec(cur_node, cnt);
				}
			}*/

		}
		return cnt;
	}

	/*void has_cycle_rec(node* prev, node* current, bool& flag) {
		if (flag) {
			return;
		}
		for (auto edge : current->edges) {
			node* next = edge->get_node1() != current ? edge->get_node1() : edge->get_node2();
			if (next->mark != 't') {
				node* new_current = next;
				new_current->mark = 't';
				has_cycle_rec(current, new_current, flag);
			}
			else if (next != prev) {
				flag = true;
				return;
			}
		}
	}

	bool has_cycle(graph* gr) {
		bool res = false;
		for (auto cur_node : gr->nodes) {
			for (auto edge : cur_node->edges) {
				node* next = edge->get_node1() != cur_node ? edge->get_node1() : edge->get_node2();
				next->mark = 't';
				has_cycle_rec(cur_node, next, res);
				if (res) {
					return res;
				}
			}
		}
		return res;
	}*/

	bool is_tree(graph* gr) {
		if (comp_cnt(gr) == 1)
			return gr->nodes.size() - gr->edges.size() == 1;
			//return !(has_cycle(gr));
		return false;
	}


	std::vector<int> codding_Prufer(graph& gr) {
		if (!is_tree(&gr)) {
			//throw std::exception("invalid argument");
			return {};
		}

		auto set_comparator = [](node* first, node* second) { return first->id < second->id; };
		std::set<node*, decltype(set_comparator)> st(set_comparator);

		std::vector<int> ans;
		for (auto node : gr.nodes) { st.insert(node); }
		auto it = st.begin();

		while (st.size() > 2) {
			node* cur_node = *it;

			if (cur_node->edges.size() == 1) { // Если узел является листом
				edge* edg = *cur_node->edges.begin(); // Получение ребра 1-го эл-та сета
				node* neigh = (edg->get_node1() != cur_node ? edg->get_node1() : edg->get_node2());
				int id = neigh->id;
				ans.push_back(id);

				st.erase(it);
				gr.erase(cur_node);
				it = st.begin();
			}
			else {
				it++;
			}
		}
		while(gr.nodes.size())
			gr.erase(*gr.nodes.begin());
		return ans;
	}

	int get_anticode(std::set<int> s){
		int i = 1;
		for(auto x: s){
			if(x != i)
				return i;
			i++;
		}
		return i;
	}

	void decodding_Prufer(std::vector<int>& prufer_code, graph& gr) {
		int n = prufer_code.size() + 2;
		std::vector<int> code(n - 2);
		for (int i = prufer_code.size() - 1, j = 0; i >= 0; --i, j++) {
			if (prufer_code[i] > n) { // Не код Прюфера
				//throw std::exception("invalid argument");
				return;
			}
			code[j] = prufer_code[i]; // Переворачиваем кодовое слово для удобной работы
		}//TODO

		std::set<int> s(prufer_code.begin(), prufer_code.end());

		while(code.size()){
			int id1 = code.back();
			int id2 = get_anticode(s);
			if(!gr.find(id1))
				gr.insert(new node(id1, 'a', &gr));
			if(!gr.find(id2))
				gr.insert(new node(id2, 'a', &gr));

			gr.find(id1)->create_edge(gr.find(id2));

			s.insert(id2);
			s.erase(id1);
			code.pop_back();
			for(auto x: code)
				s.insert(x);
		}
		int id1 = get_anticode(s);
		s.insert(id1);
		int id2 = get_anticode(s);
		if(!gr.find(id1))
				gr.insert(new node(id1, 'a', &gr));
			if(!gr.find(id2))
				gr.insert(new node(id2, 'a', &gr));

		gr.find(id1)->create_edge(gr.find(id2));
	}

	bool is_deikstra_empty(std::vector<int>& v) {
		for (auto x : v) {
			if (x != -2)
				return false;
		}
		return true;
	}

	void deikstra(graph* gr, std::vector<int>& start) {
		if (start.size() != 1 || comp_cnt(gr) != 1)
			return;
		for (auto x : gr->nodes) {
			((deikstra_node*)x)->value = 0;
			x->mark = 'a';
		}
		std::vector<int> ids(gr->nodes.size(), -1);
		std::vector<int> res(gr->nodes.size(), -1);
		ids[start[0]] = 0;
		res[start[0]] = 0;
		while (!is_deikstra_empty(ids)) {
			int id = 0;
			int min = -1;
			for (int i = 0; i < ids.size(); i++) {
				if (ids[i] == -1 || ids[i] == -2)
					continue;
				if (min == -1) {
					min = ids[i];
					id = i;
				}
				else {
					if (min > ids[i]) {
						min = ids[i];
						id = i;
					}
				}
			}
			res[id] = ids[id];
			ids[id] = -2;
			id++;
			deikstra_node* current = gr->find(id);
			for (auto ed : current->edges) {
				deikstra_node* _n = 0;
				if (ed->point1 == current) {
					_n = ed->point2;
				}
				else {
					_n = ed->point1;
				}
				if (ids[_n->id - 1] == -2);
				else if (ids[_n->id - 1] == -1){
					ids[_n->id - 1] = current->value + ed->value;
				}
				else {
					ids[_n->id - 1] = std::min(res[current->id - 1] + ed->value, ids[_n->id - 1]);
				}
			}
		}
		for (int i = 0; i < res.size(); i++) {
			deikstra_node* current = gr->find(i + 1);
			current->value = res[i];
		}
	}

	std::wstring accept_9(graph* gr) {
		std::wstring res;
		std::vector<int> start;
		start.push_back(0);
		int n = gr->nodes.size();
		res += L"    ";
		for(int i = 0; i < n; i++)
			res += std::format(L"{:4}", i + 1);
		res += L"\n";

		std::vector<int> buff(n);

		for(int i = 0; i < n; i++){
			start[0] = i;
			deikstra(gr, start);
			for (auto x : gr->nodes){
				buff[((deikstra_node*)x)->id - 1] = ((deikstra_node*)x)->value;
				((deikstra_node*)x)->value = 0;
			}
			res += std::format(L"{:4}", i + 1);
			for(int j = 0; j < n; j++)
				res += std::format(L"{:4}", buff[j]);
			res += L"\n";
		}
		return res;
	}

	//https://publications.hse.ru/pubs/share/folder/0rhqzr8ukk/133671897.pdf
	void accept_12(graph* gr) {
		for (auto x : gr->nodes) {
			x->mark = 'a';
		}
		std::vector<std::vector<int>> mt;
		int n = gr->nodes.size();
		std::vector<int> flags(n);
		int cnt = 0;
		for(int i = 0; i < n; i++){	
			mt.push_back(std::vector<int>());
			for(int j = 0; j < n; j++){
				mt[i].push_back(0);
			}
			mt[i][i] = 1;
		}
		for(auto ed: gr->edges){
			int a = ed->point1->id;
			int b = ed->point2->id;
			--a;--b;
			mt[a][b] = 1;
			mt[b][a] = 1;
		}
		//матрицу заполнили
		char colour = 'a' + 1;
		while(cnt < n){
			int f = 0;
			for(;f < n && flags[f]; f++);

			gr->find(f + 1)->mark = colour;

			int inx = 0;
			for(;inx < n && (flags[inx] || mt[f][inx]); inx++);

			if(inx == n){
				flags[f] = 1;
				colour++;	
				cnt++;
			}
			else{
				gr->find(inx + 1)->mark = colour;
				flags[inx] = 1;
				cnt++;
				for(int i = 0;i < n; i++) mt[f][i] |= mt[inx][i];
			}
		}
	}

}