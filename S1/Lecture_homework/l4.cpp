//  Задание:
//      Написать программу, которая считает сумму n первых членов геометрической прогрессии и сравнивает полученный результат с известной из школьного курса математики формулой.

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

double geo_4k(double b1, double q, double n)
{
    double sum = b1,
            qn = q;

    for (int i = 2; i <= n; i++) {

        qn*=q;
        sum += b1 * mypow(q, i-1);

        cout << sum << endl;
    }

    return sum;
}

double geo_Sn_4k(double b1, double q, double n)
{
    if ( q == 1 ) return b1;
    return ( b1 * ( pow(q, n) - 1 ) ) / ( q - 1 );
}

int main()
{
    double b1, q, n, Sum, Sn;

    b1 = 2;
    q = 2;
    n = 4;

    Sum = 0;

    Sum = geo_4k(b1, q, n);
    Sn = geo_Sn_4k(b1, q, n);

    cout << "is cycle result equal to Sn formula result: ";
    cout << (Sn == Sum) << " (" << Sum << ", " << Sn << ")" << endl;

    return 0;
}
