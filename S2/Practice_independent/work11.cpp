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
  if (A.back().compare(B[B.size()-2]) == 0)
  {
    cout << "resized";
    A.resize(A.size() - 1);
  }
  else
    cout << "the same";

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
  string str1="sample phrase.";
  string str2="an example.";
  string str3="this is a test string.";
  string str4="useful.";
  string str5="this is a useful test string.";

  vector<string> A = {str2, str1, str2, str2, str4, str3, str5, str4, str5};
  vector<string> B(A.size());

  squareCountingSort(A, B);
  cout << endl << "---- A:" << endl;
  print(A);
  //cout << "---- B:" << endl;
  //print(B);

  cout << endl << "---- "; 
  finishIt(A, B);
  cout << " A:" << endl;
  print(A);

  return 0;
}
