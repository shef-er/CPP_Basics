// Задание: 
//    Программа "Рекурсия": Написать программу, обрабатывающую массив 
//    (лучше файл), содержащий положительные и не положительные числа в 
//    произвольном порядке, при помощи рекурсивной функции, которая сначала 
//    печатает все положительные числа, потом все остальные.
//
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

template <typename T>
void
parse_file(T* output, string filename, size_t size = -1)
{
  fstream fin(filename, ios_base::in);
  for(size_t i = 0; fin >> *(output + i) && i < size; ++i);

  cout << "parsed: ";
  for(size_t i = 0; i < size; ++i)
    cout << *(output + i) << " ";
  cout << endl;
}

void
recursive_filter(int* in, size_t size, string &pos, string &neg)
{
  string temp;
  string d = " ";

  if ( size != 0)
  {
    temp = to_string(*(in + size - 1));
    temp.append(d);

    if(*(in + size - 1) > 0)
      pos.append(temp);
    else
      neg.append(temp);

    recursive_filter(in, size - 1, pos, neg);
  }
  else
    return;
}

int
main()
{
  size_t size = 10;
  int buffer[size]{};
  string file = "l11.txt";

  string pos;
  string neg;

  parse_file(buffer, file, size);
  cout << endl;
  recursive_filter(buffer, size, pos, neg);

  cout << "positives: " << pos << endl;
  cout << "negatives: " << neg << endl;

  return 0;
}


