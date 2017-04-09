#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

using vec_matrix = std::vector<std::vector<double>>;

double 
factorial(int n)
{
    if (n < 0) 
        return 0;
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

template <class T>
vector<vector<T>>
read_matrix(const string &file_name, size_t X = 3, size_t Y = 3)
{
  vector<vector<T>> result(Y, vector<T>(X, 0));
  T val;
  ifstream file(file_name);

  cout << endl << " read:" << endl;
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
void
print_matrix(vector<vector<T>> &input)
{
  for (auto &j : input)
  {
    for (auto &i : j)
      cout << i << " ";
    cout << endl;
  }
}

class Matrix
{
  size_t n = 0;
  vec_matrix MATRIX;

  public:
  Matrix(vec_matrix M)
  {
    n = M.size();
    MATRIX = A;
  }



};


/* ---- */


vec_matrix
sum(vec_matrix &A,  vec_matrix &B)
{
  size_t n = A.size();
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      C[i][j] += A[i][j] + B[i][j];

  return C;
}


vec_matrix
multiply(vec_matrix &A,  vec_matrix &B)
{
  size_t n = A.size();
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      for (size_t k = 0; k < n; k++)
        C[i][j] += A[i][k] * B[k][j];

  return C;
}


vec_matrix
multiply(vec_matrix &A, double s)
{
  size_t n = A.size();
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      C[i][j] += A[i][j] * s;

  return C;
}


vec_matrix
operator+ (vec_matrix &A,  vec_matrix &B)
{
  size_t n = A.size();
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      C[i][j] = C[i][j] + A[i][j] + B[i][j];

  return C;
}


vec_matrix
operator* (vec_matrix &A, double s)
{
  size_t n = A.size();
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      C[i][j] += A[i][j] * s;

  return C;
}


vec_matrix
operator* (vec_matrix &A,  vec_matrix &B)
{
  size_t n = A.size();
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      for (size_t k = 0; k < n; k++)
        C[i][j] += A[i][k] * B[k][j];

  return C;
}


vec_matrix
power(vec_matrix &A, size_t p)
{
  vec_matrix C(A);

  for (size_t i = 1; i < p; i++)
    C = multiply(A, C);

  return C;
}


vec_matrix
unit_matrix(size_t n)
{
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    C[i][i] = 1;

  return C;
}


vec_matrix
exponent(vec_matrix &A, size_t n = 1)
{
  vec_matrix B;
  vec_matrix C(unit_matrix(A.size()));
  double f = 1;

  for (size_t k = 0; k < n; k++)
  {
    B = power(A, k);

    cout << "---------- ( " << k+1 << " ) power: " << endl;
    print_matrix(B);
    cout << "----------" << endl << endl;

    f = factorial(k+1);

    printf ("---------- ( %zu ) factorial: \n %4.2f \n", k+1, f );
    cout << "----------" << endl << endl;

    B = multiply(B, 1.0 / f);

    cout << "---------- ( " << k+1 << " ) multiply: " << endl;
    print_matrix(B);
    cout << "----------" << endl << endl;

    C = sum(C, B);
    C = C + B;

    cout << "---------- ( " << k+1 << " ) exponent:" << endl;
    print_matrix(C);
    cout << "----------" << endl << endl;

    cout << "--------------------" << endl << endl;
  }

  return C;
}


int
main()
{
  vec_matrix M(read_matrix<double>("l4.txt"));
  cout << endl;

  //vec_matrix C = exponent(matrix, 2);
  vec_matrix C =  0 * M;

  cout << "###############" << endl;
  cout << "----- result: " << endl;
  print_matrix(C);
  cout << "-----" << endl << endl;

  printf ( " \n %4.2f \n", factorial(3) );

  return 0;
}


