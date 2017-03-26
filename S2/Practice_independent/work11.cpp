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




void
print(vector<string> &v) 
{
  size_t i = 0;
  for (auto &e : v)
    printf("%2zu | %s \n", i++, e.c_str());
  if (i == 0)
    cout << "empty" << endl;
}

vector<string>
blockPostfixPurge(vector<string> &A)
{
  vector<string> B;
  // 'mid' is on center or below
  //  (example: for 8 mid=4; for 5 mid=3)
  size_t mid = A.size() / 2 + A.size() % 2;

  cout << endl << "------------" << endl;
  cout << "size = " << A.size() << endl;
  cout << "mid  = " << mid << endl;
  cout << endl;

  for(size_t i = mid, j = 0; i < A.size(); i++)
    if (A[i] == A[j])
    {
      printf(" + ( %2zu, %2zu )    %10s    %10s \n", i, j, A[i].c_str(), A[j].c_str());
      B.push_back(A[i]);
      j++;
    }
    else if (i == A.size() - 1)
    {
      printf(" x ( %2zu, %2zu )    %10s    %10s \n", i, j, A[i].c_str(), A[j].c_str());
      B.erase(B.begin(), B.end());
    }
    else
      printf("   ( %2zu, %2zu )    %10s    %10s \n", i, j, A[i].c_str(), A[j].c_str());

  cout << "------------" << endl;

  A.erase(A.end() - B.size(), A.end());
  return B;
}

bool
test(vector<string> &A)
{

  cout << endl << "---- input:" << endl;
  print(A);
  cout << "----" << endl;


  vector<string> B = blockPostfixPurge(A);

  cout << endl << "---- output:" << endl;
  print(A);
  cout << "----" << endl;

  cout << endl << "---- buffer:" << endl;
  print(B);
  cout << "----" << endl;

  return false;
}


int
main()
{
  string str1 = "aaa";
  string str2 = "bbb";
  string str3 = "ccc";
  string str4 = "ddd";
  string str5 = "zzz";

  vector<string> A = {
    str1, //
    str1, //
    str2, //
    str3,
    str5,
    str5,
    str3,
    str3,
    str3,
    str5,
    str1, //
    str1, //
    str2, //
  };

  vector<string> B = {
    str1, //
    str1, //
    str2, //
    str5,
    str1, //
    str1, //
    str2, //
  };

  vector<string> C = {
    str1, //
    str1, //
    str2, //
    str3,
    str5,
    str5,
    str3,
    str3,
    str3,
    str5,
    str1, //
    str1, //
    str2, //
    str5,
  };

  cout << endl << endl;
  cout << "######## TEST 1 ########" << endl;
  test(A);

  cout << endl << endl;
  cout << "######## TEST 2 ########" << endl;
  test(B);

  cout << endl << endl;
  cout << "######## TEST 3 ########" << endl;
  test(C);

  return 0;
}
