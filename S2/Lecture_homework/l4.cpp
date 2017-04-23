#include <stdlib.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

using vec_matrix = std::vector<std::vector<double>>;

template <class T>
vector<vector<T>>
read_matrix(const string &file_name, size_t X = 3, size_t Y = 3)
{
  vector<vector<T>> result(Y, vector<T>(X, 0));
  T val;
  ifstream file(file_name);

  cout << " read:" << endl;
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
  cout << endl;
  return result;
}

template <class T>
void
print_matrix(vector<vector<T>> const& input)
{
  for (auto &j : input)
  {
    for (auto &i : j)
      cout << i << " ";
    cout << endl;
  }
}

double 
fact(unsigned n)
{
    double f = n;
    for (unsigned i = 2; i < n; ++i)
        f *= i;
    return f;
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
operator+ (vec_matrix & A,  vec_matrix & B)
{
  size_t n = A.size();
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      C[i][j] += A[i][j] + B[i][j];
  return C;
}

vec_matrix
operator* (vec_matrix & A, double s)
{
  size_t n = A.size();
  vec_matrix C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      C[i][j] += A[i][j] * s;
  return C;
}

vec_matrix
operator* (vec_matrix & A,  vec_matrix & B)
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
pow(vec_matrix &A, size_t p)
{
  vec_matrix C(A);
  for (size_t i = 1; i < p; i++)
    C = C * A;
  return C;
}


class Matrix
{
  size_t n = 0;
  vec_matrix M;

  public:
  Matrix(vec_matrix A)
  {
    M = A;
  }

  Matrix(string file)
  {
    M = read_matrix<double>(file);
  }

  void
  exponent(size_t n = 1)
  {
    vec_matrix B;
    vec_matrix C(unit_matrix(M.size()));
    double factorial = 1;

    for (size_t k = 1; k < n+1; k++)
    {

      B = pow(M, k);
      cout << "(" << k << ") power: \n----------" << endl;
      print_matrix(B);
      cout << endl << endl;


      factorial = fact(k);
      cout << "(" << k << ") factorial: \n----------" << endl;
      printf("%4.2f \n", factorial);
      cout << endl << endl;


      B = B * (1.0 / factorial);
      cout << "(" << k << ") multiply: \n----------" << endl;
      print_matrix(B);
      cout << endl << endl;


      C = C + B;
      cout << "(" << k << ") exponent: \n----------" << endl;
      print_matrix(C);
      cout << endl << endl;

    }
    M = C;
  }

  vec_matrix
  get()
  {
    return M;
  }

  void
  set(vec_matrix & A)
  {
    M = A;
  }

};


int
main()
{
  Matrix M("l4.txt");
  M.exponent(2);

  return 0;
}

