#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
using vmatrix = vector<vector<double>>;

long double 
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
  cout << endl << " print:" << endl;
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
  vector<vector<double>> MTRX;

  public:
  Matrix(vector<vector<double>> A)
  {
    n = A.size();
    MTRX = A;
  }



};



vector<vector<double>>
multiply(vector<vector<double>> &A,  vector<vector<double>> &B)
{
  size_t n = A.size();
  vector<vector<double>> C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      for (size_t k = 0; k < n; k++)
        C[i][j] += A[i][k] * B[k][j];

  return C;
}


vector<vector<double>>
sum(vector<vector<double>> &A,  vector<vector<double>> &B)
{
  size_t n = A.size();
  vector<vector<double>> C(n, vector<double>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
      C[i][j] += A[i][j] + B[i][j];

  return C;
}


vector<vector<double>>
power(vector<vector<double>> &A, size_t p)
{
  vector<vector<double>> C(A);

  for (size_t i = 1; i < p; i++)
    C = multiply(A, C);

  return C;
}

vector<vector<double>>
exponent(vector<vector<double>> &A, size_t k = 1)
{
  vector<vector<double>> C;

  for (size_t i = 0; i < k; i++)
    ;

  return C;
}

int
main()
{
  vector<vector<double>> matrix(read_matrix<double>("l4.txt"));
  print_matrix(matrix);

  cout << endl << " sum";
  vector<vector<double>> C = sum(matrix, matrix);
  print_matrix(C);

  //MatrixExp E(matrix);
  //vector<vector<double>> C = E.compute();
  //print_matrix(C);

  return 0;
}


