#include <cstdlib>
#include <cmath>
#include <iostream>
#include "amatrix.hpp"
using namespace std;

void
make_transpose_naive(const Matrix_info *m, Matrix_info *mt)
{
  // размеры
  const auto rows = m->rows, cols = m->cols;
  
  // подготовить место
  realloc_matrix(mt, cols, rows);
  
  // код без умножений
  auto m_row = m->data; // начало текущей строки в m
  auto mt_col = mt->data; // начало текущего столбца в mt
  for (size_t i = 0; i < rows; ++i)
  {
    auto mt_pos = mt_col; // позиция записи
    for (size_t j = 0; j < cols; ++j)
    {
      // элементы строки идут в массиве друг за другом непосредственно (шаг = 1)
      *mt_pos = m_row[j];
      // элементы столбца идут в массиве с шагом в одну строку
      // (шаг = rows -- количество столбцов в mt)
      mt_pos += rows;
    }

    ++mt_col; // перейти на следующий столбец в mt
    m_row += cols; // перейти к следующей строке в m
  }
}

bool
multiply_v3(const Matrix_info *a, const Matrix_info *b, Matrix_info *c)
{
  const auto n = a->rows, s = a->cols, m = b->cols;
  if (n == 0 || s == 0 || m == 0 || s != b->rows)
    return false;

  realloc_matrix(c, n, m);
  fill_matrix(c, 0.);
  auto a_row = a->data, // Указатель на текущую строку матрицы a.
       c_row = c->data; // Указатель на текущую строку матрицы c.

  for (size_t i = 0; i < n; ++i, a_row += s, c_row += m)
  {
    auto b_row = b->data; // Указатель на текущую строку матрицы b.
    for (size_t j = 0; j < s; ++j, b_row += m)
    {
      const auto aij = a_row[j];
      for (size_t k = 0; k < m; ++k)
        c_row[k] += aij * b_row[k];
    }
  }

  return true;
}

bool
orto_4k(Matrix_info* a, double tol) {
  int rows = a->rows,
      cols = a->cols;

  Matrix_info b = {};
  alloc_matrix(&b, cols, rows);

  Matrix_info c = {};
  alloc_matrix(&c, rows, rows);

  make_transpose_naive(a, &b);
  multiply_v3(a, &b, &c);


  cout << "Resulting matrix (c): " << endl;
  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < rows; ++j)
    {
      printf("%5.1f", get_element(&c, i, j));
    }
    cout << endl;
  }
  cout << endl;


  for (size_t i = 0; i < rows; ++i)
  {
    if (abs(get_element(&c, i, i) - 1) > tol ) return false;
    for (size_t j = 0; j < rows; ++j)
      if ((i != j) && (get_element(&c, i, j) > tol)) return false;
  }
  return true;
}

int
main()
{
  double con = sqrt(2.0)/2;
  double matrix[] {
    con, con, 0,
    -con, con, 0,
    0, 0, 1
  };
  int rows = 3,
      cols = 3;

  Matrix_info m = {};
  alloc_matrix(&m, rows, cols);

  cout << "Input matrix (a): " << endl;
  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < cols; ++j)
    {
      set_element(&m, i, j, matrix[cols*i + j]);
      printf("%5.1f", get_element(&m, i, j));
    //  cout << "(" << cols*i+j << ")";
    }
    cout << endl;
  }
  cout << endl;

  double E = cols * 1e-16;
  if (orto_4k(&m, E))
    cout << "Input matrix is ortogonal" << endl;
  else
    cout << "Input matrix is not ortogonal" << endl;
}


