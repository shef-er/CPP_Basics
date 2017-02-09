#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

double array_4k(double arr[], size_t size) {
    double
        min_pos = arr[0],
        max_neg = arr[0];

    for( int i = 0; i < (int)size; i++ )
    {
        if( abs(max_neg) > abs(arr[i]) && (arr[i] < 0) ) {
            max_neg = arr[i];
        }
        if( abs(min_pos) > abs(arr[i]) && (arr[i] > 0) ) {
            min_pos = arr[i];
        }
    }

    cout << min_pos << " - " << "(" << max_neg << ") = ";
    return ( min_pos  - max_neg );
}

int main()
{
    double arr[] = {8, -10, -4, -14, 5, 15, 2, -6, 3, -3};


    cout << "IN: { ";
    for( int i : arr )
        cout << i << ", ";
    cout << " }" << endl;

    cout << "OUT: ";
    cout << array_4k(arr, (sizeof(arr)/sizeof(*arr)) );
    return EXIT_SUCCESS;
}


