#include <numeric>
#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
using namespace std;

template <class T>
T gauss_area(const vector<T> &a, const vector<T> &b)
{
	double ip1(0), ip2(0);
	ip1 = inner_product(begin(a), end(a) - 1, begin(b) + 1, b.front() * a.back());
	ip2 = inner_product(begin(a) + 1, end(a), begin(b), a.front() * b.back());

	return (ip1 - ip2) / 2;
}


int 
test()
{
	vector<double> X = {0, 1, 2, 5, 4, 5, 0};
	vector<double> Y = {0, 5, 4, 5, 2, 1, 0};

	cout << gauss_area(X, Y) << endl;

	return 0;
}

int 
main()
{
	test();
	return 0;
}
