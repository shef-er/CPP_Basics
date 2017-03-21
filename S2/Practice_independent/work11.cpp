/*
* =====================================================================================
* Механика и математическое моделирование
* Основы программирования C++, 2 семестр
* ------------------------
* Самостоятельная работа 11, вариант 7
*
* Задание:
*    Пусть исходный набор строк можно условно представить в виде следующих друг за другом блоков
*      (A и X — некоторые последовательности строк):
*   --------
*    A
*    X
*    A
*   --------
*
*    т.е. блок A повторяется в конце набора. При этом A — максимальный по размеру такой блок.
*    Требуется удалить финальный блок A. Результирующий набор строк должен иметь вид:
*
*   --------
*    A
*    X
*   --------
*
*    Примечание. В данном задании предполагается реализация квадратичного алгоритма,
*      находящего блок A, после чего для удаления финальной копии блока A достаточно
*      воспользоваться функцией-членом вектора resize.
*----------------------------------------------------------------------------
*/
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

void
squareCountingSort(vector<string> &A, vector<string> &B)
{
	size_t n = A.size();

	for (size_t i = 0; i < n; i++)
	{
		size_t c = 0;

		for (size_t j = 0; j < i; j++)
		if (A[j].size() <= A[i].size())
			c = c + 1;

		for (size_t j = i + 1; j < n; j++)
		if (A[j].size() < A[i].size())
			c = c + 1;
		B[c] = A[i];
	}
}

void
finishIt(vector<string> &A, vector<string> &B)
{
	if (A.back() == B[B.size() - 2])
	{
		cout << "resized";
		A.resize(A.size() - 1);
	}
	else
		cout << "the same";

}




vector<string>
blockInDaHouse(vector<string> &A)
{
	vector<string> B;

	// n starts from center
	size_t n = A.size();
	size_t m = A.size() / 2 + A.size() % 2;
	size_t p = n - m;
	cout << "$m " << m << endl;
	cout << "$p " << p << endl;

	for (size_t i = 0; i < p; i++)
	{
		m++;
		for (size_t j = i; j < n; j++)
		{
			if (A[j + m] == A[j])
			{
				B.push_back(A[i]);
			}
		}


		for (size_t j = 0; j < A.size(); j++)
		{
			cout << "@  ( " << j << ", " << j + m << " ) ";
			cout << A[j] << " -?- " << A[j + m] << endl;

			if (B[j] == A[j + m])
			{
				B.push_back(A[i]);
				B.pop_back();
				goto END;
			}
		}
	}

	END:
	cout << "#m " << m << endl;
	//A.erase(A.begin() + m, A.end());
	return B;
}

template <class T>
void
print(vector<T> &v)
{
	for (auto &e : v)
		cout << e << endl;
}

int
main()
{
	string str1 = "yea im an eloquent";
	string str2 = "soft kitty warm kitty";
	string str3 = "stein um stein";
	string str4 = "true beauty is so painful my dear";
	string str5 = "wasting time with the devil in the details";

	vector<string> A = {
		str4, //
		str5, //
		str5, //
		str2, 
		str1, 
		str2, 
		str2, 
		str4, 
		str3,
		str4, //
		str5, //
		str5  //
	};
	vector<string> B;

	cout << endl << "---- A:" << endl;
	print(A);

	cout << endl << endl << endl;

	B = blockInDaHouse(A);
	cout << endl;
	cout << endl << "---- operated A:" << endl;
	print(A);
	cout << endl << "---- B:" << endl;
	print(B);

	return 0;
}
