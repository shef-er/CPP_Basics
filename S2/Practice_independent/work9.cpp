// 9 самостоятельная (6 вариант)
//  Вычислить вектор арифметических средних каждой группы соседних k элементов в исходном 
//    векторе: bi = (ai + ai+1 + … + ai+k–1)/k.
//
//----------------------------------------------------------------------------
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

template <class T>
void
compute_middle_4k(vector<T>& input, vector<T>& output, size_t chunk = 2)
{
	size_t out_size = input.size() - chunk + 1;
	output.clear();
	for (size_t i = 0; i < out_size; i++)
	{
		T o = 0;
		size_t ii = i + chunk;
		for (size_t j = i; j < ii; j++)
			o += input[j];
		output.push_back(o / chunk);
	}

	for (size_t i = 0; i < out_size; i++)
	{
		size_t ii = i + chunk;
		cout << output[i] << " = ( ";
		for (size_t j = i; j < ii; j++)
			cout << input[j] << " ";
		cout << ")" << endl;
	}
	cout << endl;
}

int
test() {
	vector<double> a, b;
	vector<float> c, d;

	for (size_t i = 1; i < 10; i++)
	{
		a.push_back(i);
		c.push_back(i);
	}

	compute_middle_4k(a, b);
	for (size_t i = 0; i < b.size(); i++)
	if (b[i] != 0.5 + i)
		return 1;

	compute_middle_4k(c, d);
	for (size_t i = 0; i < d.size(); i++)
	if (d[i] != 0.5 + i)
		return 2;

	return 0;
}

int
main()
{
	cout << "Test: " << test() << endl;
	return EXIT_SUCCESS;
}
