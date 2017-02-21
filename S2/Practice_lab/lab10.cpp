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
	size_t n = vector.size();
	if (n != 0)
	{
		double sum = 0;
		for (auto& v : vector)
			sum += v;

		double mid = sum / n;
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

template <class T>
vector<vector<T>>
read_matrix(const string &file_name, size_t X = 3, size_t Y = 3)
{
	vector<vector<T>> result(Y, vector<T>(X, 0));
	T val;
	ifstream file(file_name);

	for (size_t j = 0; j < Y; j++)
	{
		for (size_t i = 0; i < X; i++)
		{
			file >> val;
			cout << val << " ";
			result[j][i] = val;
		}
		cout << endl;
	}
	return result;
}

template <class T>
vector<T>
compute_matrix_from_file(const string &file_name, size_t X = 3, size_t Y = 3)
{
	vector<vector<T>> input(read_matrix<T>(file_name, X, Y));

	cout << endl;
	for (size_t j = 0; j < Y; j++)
	{
		for (size_t i = 0; i < X; i++)
		{
			cout << input[j][i] << " ";
		}
		cout << endl;
	}

	vector<T> output;
	for (size_t i = 0; i < Y; i++)
	{
		vector_prepare(input[i]);
		vector_prepare(input[Y-i-1]);
		output.push_back(vector_cos(input[i], input[Y - i - 1]));
	}
		
	return output;
}

int
main()
{
	vector<double> vctr(compute_matrix_from_file<double>("matrix.txt"));

	for (auto& v : vctr)
		cout << v << " ";
	cout << endl;

	return 0;
}
