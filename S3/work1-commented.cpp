#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>

using namespace std;

template<typename T>
using edges_list = vector<pair<T, T>>;

template<typename T>
using neighbors_list = vector<vector<T>>;

template <class T> bool unique(pair<T, T> input, edges_list<T> edges) // const pair<T, T> &input, const edges_list<T> &edges
{
	for (auto &edge : edges)
	if ((edge.first == input.first && edge.second == input.second) || // edge == input
	(edge.first == input.second && edge.second == input.first))       // неориентированный граф.
			return false;
	return true;
}

template <class T> bool unique(T token, neighbors_list<T> neighbors) // const T &token, const neighbors_list<T> &neighbors
{
	for (auto &neigh : neighbors)
        // Сравнение только с головой: т.е. unique запрещает петли, а не дубликаты:
		if (token == neigh.front())
			return false;
	return true;
}

template <class T> edges_list<T> read_edges(const string &file_name)
{
	edges_list<T> result;
	pair <T, T> edge;
	ifstream file(file_name);
	T line; // string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		if (!(iss >> edge.first >> edge.second)) break;
		result.push_back(edge);
	}
	show_list(result); // show
	return result;
}

template <class T> neighbors_list<T> read_neighbors(const string &file_name)
{
	neighbors_list<T> result;
	ifstream file(file_name);
	string line;
	while (getline(file, line))
	{
		vector<T> neigh;
		istringstream iss(line);
		T vertex;
		while (iss >> vertex)
		{
			neigh.push_back(vertex);
		}
		result.push_back(neigh); // лучше push_back(move(neigh)), чтобы не копировать, а "отдать" объект
	}
	show_list(result); // show
	return result;
}

template <class T> void show(edges_list<T> input) // const edges_list<T> &input
{
	cout<<"edges list"<<endl;
	for (auto &edge : input)
	{
		cout <<"["<<edge.first<<","<<edge.second<<"]"<< endl;
	}
	cout << endl;
}

template <class T> void show(neighbors_list<T> input) // const neighbors_list<T> &input
{
	cout << "neighbors list" << endl;
	for (auto &vertex : input)
	{
		cout << vertex.front() << ": "; // крах, если вдруг vertex.empty()
		for (auto neigh = vertex.begin() + 1; neigh != vertex.end(); ++neigh)
		{
			cout << *neigh << " ";
		}
		cout << endl;
	}
	cout << endl;
}

template <class T> neighbors_list<T> edges_to_neighbors(edges_list<T> input) // const edges_list<T> &input
{
	neighbors_list<T> result;
	for (auto &edge : input)
	{
		vector<T> neigh;
		T vertex;
		if (unique(edge.first, result)) vertex = edge.first;
		else if (unique(edge.second, result)) vertex = edge.second;
		else
			continue;
		neigh.push_back(vertex);
		for (auto &bone : input)
		{
			if (bone.first == vertex) neigh.push_back(bone.second);
			else if (bone.second == vertex) neigh.push_back(bone.first); // неориентированный граф.
		}
		result.push_back(neigh); // push_back(move(neigh))
	}
	show(result);
	return result;
}

template <class T> edges_list<T> neighbors_to_edges(neighbors_list<T> input) // const neighbors_list<T> &input
{
	edges_list<T> result;
	pair<T, T> edge;
	for (auto &vertex : input)
	{
        // Крах, если vertex.empty().
		for (auto neigh = vertex.begin() + 1; neigh != vertex.end(); ++neigh)
		{
			edge = make_pair(vertex.front(), *neigh);
			if (unique(edge, result)) result.push_back(edge); // push_back(move(edge))
		}
	}
	show(result);
	return result;
}

// Вообще, в дублировании элементов нет ничего принципиально страшного.
// Если же необходимо гарантировать отсутствие дубликатов, то разумнее
// либо использовать вспомогательную структуру данных вроде set или unordered_set,
// либо в конце выполнить сортировку и удаление дубликатов.
// Оба способа дадут субквадратичное время (N log N в случае set или сортировки, среднее N в случае unordered_set,
// но сортировка может быть эффективнее на практике, так как не выделяет дополнительную динамическую память и
// имеет удобный для кэшей процессора характер обращений к памяти).

int main()
{
	int a = 1;
	cout << "Enter  (1)  if edges to neighbors list and (2) if neighbors to edges list" << endl;
	cin >> a;
	if (a == 1) { edges_to_neighbors(read_edges<string>("edges_list.txt")); }
	else if (a == 2) { neighbors_to_edges(read_neighbors<string>("neighbors_list.txt")); }
	return 0;
}

