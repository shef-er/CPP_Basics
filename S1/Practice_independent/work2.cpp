#include <iostream>
#include <cstdlib>
using namespace std;

// Проверить попадание в прямоугольник.
bool in_rectangle(float x, float y,
  float left, float right, float bottom, float top)
  // координаты левой, правой, нижней и верхней граней
{
  return left <= x && x <= right // && -- "и"
    && bottom <= y && y <= top;
}

// Проверить попадание в круг.
bool in_circle(float x, float y, // координаты точки
  float cx, float cy, float r)   // координаты центра круга и его радиус
{
  // Константы -- после инициализации значения не изменяются.
  const float dx = x - cx,
              dy = y - cy;

  return dx * dx + dy * dy <= r * r;
}




// Проверить попадание в ромб.
bool in_rhomb(float x, float y, // координаты точки
  float cx, float cy, float rx, float ry)   // координаты центра ромба и его радиус
{
  // Константы -- после инициализации значения не изменяются.
  const float dx = x - cx,
              dy = y - cy;

  return (rx * abs((int)dx) + ry * abs((int)dy) <= rx * ry );
}

// Проверить попадание в заданную фигуру.
bool in_figure(float x, float y)
{
    bool a = in_circle(x, y, 0.0, 1.0, 1.0),
         b = in_rhomb(x, y, 0, 0, 1.0, 1.0);
    
  return ( (a && !b) || (!a && b) );
}

int main()
{
    double x = 0, y = 0.5;
//  cout << "Enter a sequence of coordinates x, y: ";
  // Цикл ввода пар значений x, y до первой ошибки ввода.
//  for (double x = 0, y = 0; cin >> x >> y;)
//  {
    cout << "(x, y) within the figure == " << in_figure(x, y) << endl;
//  }
  return 0;
}
