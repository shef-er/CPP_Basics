/*
Пусть дана последовательность некоторых значений (произвольного типа) A,
элементы которой можно сравнивать с помощью двух разных критериев
качества — компараторов c1 и c2 (параметры).
Доминируемым элементом называется такой
a ∈ A, что ∃b ∈ A (¬c1(b, a) ∧ c2(a, b)) ∨ (¬c2(b, a) ∧ c1(a, b)).
Т.е. по одному критерию b, по крайней мере, не хуже a,
а по другому — строго лучше.
Требуется удалить из A все доминируемые элементы за O(|A| log |A|)-время.
*/
#include <numeric>
#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;


template <class T, class Comp1, class Comp2>
void
extrathicc(T& A, Comp1 c1, Comp2 c2)
{

	sort(A.begin(), A.end(), c1);
	reverse(A.begin(), A.end());

	auto Nend = unique(A.begin(), A.end(),
		[c2](const pair<int, int> &left, const pair<int, int> &right) {
			return !(c2(left, right));
		}
	);

	A.erase(Nend, A.end());

}


int
test()
{
	vector<pair<int, int>> A = { 
		{ 1, 1 }, 
		{ 2, 2 }, 
		{ 3, 1 }, 
		{ 6, 1 }, 
		{ 2, 3 }, 
		{ 2, 5 },
		{ 3, 4 }, 
		{ 3, 1 },
	};

	extrathicc(A, 
		[](const pair<int, int> &left, const pair<int, int> &right) {
			return left.first < right.first;
		},
		[](const pair<int, int> &left, const pair<int, int> &right) {
			return left.second < right.second;
		}
		);


		/* ---- unique ---- */
		cout << endl;
		cout << "unique { ";
		for (auto a : A)
			cout << " (" << a.first << ", " << a.second << ")";
		cout << "}" << endl;
		/* ----       ---- */

	return 0;
}

int
main()
{
	test();
	return 0;
}
