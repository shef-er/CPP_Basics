#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

double
vector_multiply(vector<double> &a, vector<double> &b)
{
	vector<double>::iterator it_a = a.begin();
	vector<double>::iterator it_b = b.begin();

	double sum = 0;
	for (; it_a != a.end() || it_b != b.end(); ++it_a, ++it_b)
		sum += (*it_a) * (*it_b);

	return sum;
}

double
vector_cos(vector<double> &a, vector<double> &b)
{
	return vector_multiply(a, b) / (sqrt(vector_multiply(a, a)) * sqrt(vector_multiply(b, b)));
}

void
vector_centralize(vector<double> &vector)
{
	int n = 0;
	double sum = 0;
	double mid = 0;
	for (auto& v : vector)
	{
		n++;
		sum += v;
	}
	if (n != 0)
	{
		mid = sum / n;
		for (auto& v : vector)
			v = v - mid;
	}
}

void
vector_normalize(vector<double> &vector)
{
	int max = 0;
	for (auto& v : vector)
	{
		int val = abs(v);
		if (max < val) max = val;
	}

	if (max != 0)
		for (auto& v : vector)
			v = v / max;
}

void
vector_prepare(vector<double> &vector) 
{
	vector_centralize(vector);
	vector_normalize(vector);
}

int
main()
{
	vector<double> a = { 1, 5, 10, 0, 0, 2, 4 };
	vector<double> b = { 5, 2, 2, 10, 2, 5, 0 };

	vector_prepare(a);
	vector_prepare(b);

	cout << vector_cos(a, b);
	cout << endl;

	return 0;
}
