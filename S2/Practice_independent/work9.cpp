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

void
compute_middle_4k(vector<double>& input, vector<double>& output, size_t chunk = 2)
{
  size_t out_size = input.size() - chunk + 1;
  output.clear();
  for (size_t i = 0; i < out_size; i++)
  {
    double o = 0;
    size_t ii = i + chunk;
    for (size_t j = i; j < ii; j++)
      o += input[j];
    output.push_back(o / chunk);
  }

  for (size_t i = 0; i < out_size; i++)
  {
    size_t ii = i + chunk;
    printf("%f = ", output[i]);
    cout << "(";
    for (size_t j = i; j < ii; j++)
      printf("%f ", input[j]);
    cout << ")" << endl;
  }

}

int
main()
{
  vector<double> a, b;

  for (size_t i = 0; i < 10; i++)
    a.push_back(i);

  compute_middle_for_k(a, b, );

  return EXIT_SUCCESS;
}
