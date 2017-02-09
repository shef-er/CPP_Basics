// amatrix.hpp
// Общие задачи обслуживания динамических массивов, которые можно интерпретировать как матрицы.
// Работа на C++ в стиле C.
#ifndef AMATRIX_HPP_INCLUDED_AMA700
#define AMATRIX_HPP_INCLUDED_AMA700

// Используются упакованные двумерные массивы.
// Под "упакованным" понимается массив, все элементы которого расположены в памяти без разрывов.
// Таким образом, фактически двумерный массив является одномерным с особым режимом индексирования.
// Элемент с двумерным индексом (i, j) имеет одномерный индекс (i * cols + j),
// где cols -- количество столбцов (т.е. строки идут друг за другом в памяти).
#include <cstddef> // size_t
#include <cstdlib> // calloc, realloc, free

/// Описание матрицы.
struct Matrix_info
{
  double *data; ///< Указатель на массив элементов.
  size_t rows,  ///< Количество строк.
         cols;  ///< Размер строки (количество столбцов).
};


// Ключевое слово inline позволяет размещать определения функций 
// непосредственно в заголовочном файле, что часто удобно, так как не требуется отдельный .cpp файл.

/// Определить количество элементов матрицы mtx.
inline size_t matrix_size(const Matrix_info *mtx)
{
  return mtx->rows * mtx->cols;
}

/// Создать динамический массив нужного размера, заполнить поля mtx.
/// Возвращает true в случае успеха и false в случае ошибки.
/// В случае ошибки выделения памяти обнуляет поля *mtx.
inline bool alloc_matrix(Matrix_info *mtx, size_t rows, size_t cols)
{
  // Проверить, представимо ли произведение rows на cols в виде значения size_t.
  const auto elements = rows * cols;
  if (elements / rows == cols &&
    (mtx->data = (double*)std::calloc(elements, sizeof(double))))
  {
    mtx->rows = rows;
    mtx->cols = cols;
    return true;
  }
  else
  {
    mtx->data = nullptr;
    mtx->rows = 0;
    mtx->cols = 0;
    return false;
  }
}

/// Освободить динамический массив, привязанный к матрице.
/// Обнуляет поля *mtx.
inline void free_matrix(Matrix_info *mtx)
{
  std::free(mtx->data);
  mtx->data = nullptr;
  mtx->rows = 0;
  mtx->cols = 0;
}

/// Переформатировать существующую матрицу под новый размер.
/// Возвращает true в случае успеха и false в случае ошибки.
/// В случае ошибки выделения памяти оставляет поля *mtx неизменными.
inline bool realloc_matrix(Matrix_info *mtx, size_t rows, size_t cols)
{
  const auto elements = rows * cols;
  // Проверить, представимо ли произведение rows на cols в виде значения size_t.
  if (elements / rows != cols)
    return false;

  if (matrix_size(mtx) < elements)
  {
    const auto bytes = elements * sizeof(double);
    if (bytes / sizeof(double) != elements)
      return false; // Столько байт нельзя выделить.

    // Попытаться расширить блок.
    const auto data = (double*)std::realloc(mtx->data, bytes);
    if (!data)
      return false;

    // Удалось расширить блок.
    mtx->data = data;
    mtx->rows = rows;
    mtx->cols = cols;
    return true;
  }
  else
  {
    mtx->rows = rows;
    mtx->cols = cols;
    return true;
  }
}

/// Присвоить всем элементам матрицы заданное значение.
inline void fill_matrix(Matrix_info *mtx, double value)
{
  const auto elements = matrix_size(mtx);
  const auto data = mtx->data;
  for (size_t i = 0; i < elements; ++i)
    data[i] = value;
}

/// Заполнить матрицу значениями из статического массива (если требуется, выделяет память).
/// Значения полей *mtx должны быть инициализированы (хотя бы нулями).
inline void matrix_from_array(Matrix_info *mtx, size_t rows, size_t cols, const double *arr2)
{
  // Подготовить место.
  realloc_matrix(mtx, rows, cols);
  // Скопировать поэлементно подряд.
  const auto sz = matrix_size(mtx);
  const auto data = mtx->data;
  for (size_t i = 0; i < sz; ++i)
    data[i] = arr2[i]; // вместо цикла можно использовать memcpy из cstring
}

/// Создать единичную матрицу размера n x n.
/// Значения полей *mtx должны быть инициализированы (хотя бы нулями).
inline void identity(Matrix_info *mtx, size_t n)
{
  // Подготовить место.
  realloc_matrix(mtx, n, n);
  // Большинство элементов единичной матрицы -- нули.
  fill_matrix(mtx, 0.0);
  // Но по диагонали идут единицы.
  auto write_pos = mtx->data;
  for (size_t i = 0; i < n; ++i)
  {
    *write_pos = 1.0;
    write_pos += (n + 1);
  }
}

/// Скопировать содержимое одной матрицы в другую.
/// Значения полей *dst должны быть инициализированы (хотя бы нулями).
inline void copy_matrix(const Matrix_info *src, Matrix_info *dst)
{
  // Подготовить место под копию.
  realloc_matrix(dst, src->rows, src->cols);
  // Скопировать всё содержимое массива подряд.
  const auto elements = matrix_size(src);
  const auto sd = src->data;
  const auto dd = dst->data;
  for (size_t i = 0; i < elements; ++i)
    dd[i] = sd[i];
}

/// Сравнить матрицы на равенство.
inline bool matrices_are_equal(const Matrix_info *a, const Matrix_info *b)
{
  // Если размеры матриц не равны, то и матрицы не равны.
  if (a->rows != b->rows || a->cols != b->cols)
    return false;

  // Размеры совпали, сравним поэлементно.
  const auto elements = matrix_size(a);
  auto ad = a->data, bd = b->data;
  for (size_t i = 0; i < elements; ++i)
    if (ad[i] != bd[i])
      return false;

  // Равны поэлементно => равны.
  return true;
}


/// Получить указатель на строку с заданным номером (только для чтения).
inline const double* get_row(const Matrix_info *m, size_t i)
{
  return m->data + i * m->cols;
}

/// Получить указатель на строку с заданным номером.
inline double* get_row(Matrix_info *m, size_t i)
{
  return m->data + i * m->cols;
}

/// Извлечь элемент по номерам строки и столбца.
inline double get_element(const Matrix_info *m, size_t i, size_t j)
{
  return m->data[i * m->cols + j];
}

/// Установить элемент с заданными номерами строки и столбца.
inline void set_element(Matrix_info *m, size_t i, size_t j, double value)
{
  m->data[i * m->cols + j] = value;
}

#endif//AMATRIX_HPP_INCLUDED_AMA700
