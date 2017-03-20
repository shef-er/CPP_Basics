/*
 * =====================================================================================
 * Механика и математическое моделирование
 * Основы программирования C++, 2 семестр
 * ------------------------
 * Самостоятельная работа 10, вариант 7
 *
 *  Задание:
 *    Определить, обладает ли произвольная матрица свойством диагонального преобладания.
 *
 *    [Матрицей с диагональным преобладанием называют квадратную матрицу (aij), для которой верно 
 *      2|aii| > Σj |aij| для всех индексов i.]
 *
 */
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

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
bool
matrix_diagonal_domination(const vector<vector<T>> &input)
{

	if (input.empty() || input[0].empty())
		return 0;

	size_t X = input.size();
	size_t Y = input[0].size();

	for (size_t i = 0; i < X; i++)
	{
		if (Y > input[i].size())
			Y = input[i].size();
	}

	cout << endl;
	for (size_t i = 0; i < X; i++)
	{
		for (size_t j = 0; j < Y; j++)
		{
			cout << input[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	for (size_t i = 0; i < X; i++)
	{
		cout << input[i][i] << " * 2 > ";
		T sum = 0;
		for (size_t j = 0; j < Y; j++)
		{
			cout << abs(input[i][j]) << " ";
			sum += abs(input[i][j]);
		}
		cout << endl;
		cout << endl;
		if (input[i][i] * 2 <= sum)
			return false;
	}

	return true;
}


int
test() {
	vector<vector<double>> matrix1(read_matrix<double>("matrix1.txt"));
	vector<vector<double>> matrix2(read_matrix<double>("matrix2.txt"));

	cout << endl << "tests!" << endl;

	cout << endl;
	if (matrix_diagonal_domination(matrix1))
		cout << "matrix1 is diagonal dominated" << endl;
	else
	{
		cout << "matrix1 is not diagonal dominated" << endl;
		return 1;
	}

	cout << endl;
	if (matrix_diagonal_domination(matrix2))
		cout << "matrix2 is diagonal dominated" << endl;
	else
	{
		cout << "matrix2 is not diagonal dominated" << endl;
		return 2;
	}

	return 0;
}

int
main()
{
	test();

	return 0;
}
