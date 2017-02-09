#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

int math_4k(double a, double b, double c, double &root)
{
    double top, bot;
    if(sin(b)) {
        top = pow(a,c) * ( pow(c,2) - pow(a,2) );
        bot = sin(b);
        root = cbrt(cbrt(top/bot));
        return 1;
    }
    return -1;
}

int main()
{
    double
        root,
        a = 1,
        b = 2,
        c = 3;
    math_4k(a,b,c,root);
    cout << root << endl;
  return 0;
}
