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




template <class T>
void
print(vector<T> &v)
{
  size_t i = 1;
  for (auto &e : v)
    cout << i++ << " | "<<  e << endl;
}

vector<string>
blockPostfixPurge(vector<string> &A)
{
  vector<string> B;

  size_t n = A.size() / 2;

  // 'mid' is on center or below
  //  (example: for 8 mid=4; for 5 mid=3)
  size_t mid = A.size() / 2 + A.size() % 2;

  // 'rest' is count of elem from 'm' to end
  //  (example: for 8 rest=4; for 5 rest=2)
  size_t rest = A.size() - mid;

  size_t delta = mid;

  cout << "#A.size()  = " << A.size() << endl;
  cout << "#n         = " << n << endl;
  cout << "#mid       = " << mid << endl;
  cout << "------------" << endl;

  for (size_t i = 0; i < n; i++)
  {
    delta++;
    rest = A.size() - delta;

    for (size_t j = i; j < rest; j++)
      if (A[j + delta] == A[j])
      {
        cout << "@+ ( " << j << ", " << j + delta + 1 << " ) ";
        cout << A[j] << " -- " << A[j + delta] << endl;
        B.push_back(A[i]);
      }
      else if (j + delta + 1 == A.size())
      {
        cout << "@- ( " << j << ", " << j + delta + 1 << " ) ";
        cout << A[j] << " -- " << A[j + delta] << endl;
        B.erase(B.begin(), B.end());
        goto END;
      }
      else
      {
        cout << "@  ( " << j << ", " << j + delta + 1 << " ) ";
        cout << A[j] << " -- " << A[j + delta] << endl;
      }
  }
  cout << "------------" << endl;

  END:
  A.erase(A.end() - B.size(), A.end());
  return B;
}

int
main()
{
  string str1 = "yes im an eloquent";
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

    str2,
    str4,
    str3,
    str4, //
    str5, //
    str5, //
    str4,
  };
  vector<string> B;

  cout << endl << "---- A:" << endl;
  print(A);

  cout << endl << endl << endl;

  //B = blockInDaHouse(A);
  B = blockPostfixPurge(A);
  cout << endl;
  cout << endl << "---- operated A:" << endl;
  print(A);
  cout << endl << "---- B:" << endl;
  print(B);

  return 0;
}
