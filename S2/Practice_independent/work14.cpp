#include <numeric>
#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;


template <class T, class C>
bool
extrathicc(vector<T> &a, vector<T> &c, C comp)
{
	vector<int> b(c);

	/* ---- ---- */
	cout << "a = { ";
	for (auto aa : a)
		cout << aa << " ";
	cout << "}" << endl;

	cout << "b = { ";
	for (auto bb : b)
		cout << bb << " ";
	cout << "}" << endl;
	/* ---- ---- */
	
	sort(a.begin(), a.end(), comp);
	sort(b.begin(), b.end(), comp);

	/* ---- ---- */
	cout << endl;
	cout << "a sorted = { ";
	for (auto aa : a)
		cout << aa << " ";
	cout << "}" << endl;
	cout << "b sorted = { ";
	for (auto bb : b)
		cout << bb << " ";
	cout << "}" << endl;
	/* ---- ---- */

	return includes(b.begin(), b.end(), a.begin(), a.end(), comp);
}


int 
test()
{
	vector<int> A = {0, 1, 5, 5};
	vector<int> B = {0, 5, 4, 5, 2, 1, 0};

	cout << extrathicc(A, B, less<int>()) << endl;
	return 0;
}

int 
main()
{
	test();
	return 0;
}
