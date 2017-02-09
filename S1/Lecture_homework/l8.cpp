//  Задание:
//    Программа "Бюрократ": написать функцию, которая может "красиво" печатать
//    таблицу значений произвольной заданной функции по заданному массиву
//    аргументов.
//
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

double
parabola(double x)
{
  return x*x;
}

void
tabulateFunction(double(*f)(double), double arr[], int n, int columns = 4)
{
  cout << "(x; y)" << endl;
  for( int i = 0; i < n; i++ )
  {
    cout << "(";
    printf("%6.1f", arr[i]);
    cout << "; ";
    printf("%6.1f", f( arr[i]) );
    cout << ") ";
    if((i+1)%columns == 0)
      cout << endl;
  }
  cout << endl;
}

int
main()
{
  double args[] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
  
  tabulateFunction(parabola, args, sizeof(args)/sizeof(*args));
  return 0;
}
