#include <iostream>
#include <cstdlib>
using namespace std;

// Cтерадианы в кв.градусы
double sterad2sqdeg(double s)
{
    return s*3282.80635;
}

// Градусы в кв.секунды
double sqdeg2sqsec(double sqd)
{
    return sqd*12960000;
}

// Кв.градусы в стерадианы
double sqdeg2sterad(double sqd)
{
    return sqd/3282.80635;
}

// Кв.секунды в кв.градусы
double sqsec2sqdeg(double sqs)
{
    return sqs/12960000;
}

int main()
{
  cout << "Введите значение: ";
  double angle = 0.0;
  cin >> angle;
  cout << endl;
  cout << "Cтерадианы в кв.градусы = " << sterad2sqdeg(angle) << endl;
  cout << "Кв.градусы в кв.секунды = " << sqdeg2sqsec(angle)  << endl;
  cout << endl;
  cout << "Кв.градусы в стерадианы = " << sqdeg2sterad(angle) << endl;
  cout << "Кв.секунды в кв.градусы = " << sqsec2sqdeg(angle)  << endl;
  cout << endl;
  cout << "Cтерадианы в кв.секунды = " << sqdeg2sqsec(sterad2sqdeg(angle)) << endl;
  cout << "Кв.секунды в стерадианы= "  << sqdeg2sterad(sqsec2sqdeg(angle)) << endl;
  cout << endl;
  return EXIT_SUCCESS;
}
