/*
  Пусть дана последовательность некоторых значений (произвольного типа) A,
    элементы которой можно сравнивать с помощью двух разных критериев
    качества — компараторов c1 и c2 (параметры).
  Доминируемым элементом называется такой 
    a ∈ A, что ∃b ∈ A (¬c1(b, a) ∧ c2(a, b)) ∨ (¬c2(b, a) ∧ c1(a, b)).
  Т.е. по одному критерию b, по крайней мере, не хуже a,
    а по другому — строго лучше.
  Требуется удалить из A все доминируемые элементы за O(|A| log |A|)-время.

*/
#include <numeric>
#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

template <class T, class C>
bool
extrathicc(vector<T>& A, C c1, C c2)
{
  /* ---- before ---- */
  cout << "before = { ";
  for (auto a : A)
    cout << a << " ";
  cout << "}" << endl;
  /* ----        ---- */
  
  sort(A.begin(), A.end());

  /* ---- after ---- */
  cout << endl;
  cout << "after  = { ";
  for (auto a : A)
    cout << a << " ";
  cout << "}" << endl;
  /* ----       ---- */

  return false;
}


int 
test()
{
  vector<int> A = {0, 5, 4, 5, 2, 1, 0};

  cout << extrathicc(A, less<int>(), less_equal<int>()) << endl;
  return 0;
}

int 
main()
{
  test();
  return 0;
}
