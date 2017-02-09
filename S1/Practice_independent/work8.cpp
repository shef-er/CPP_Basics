// 8 лабораторная (6 вариант)
//    Даны две строки, которые назовём текст и слово, а также количество
//    допустимых несовпадений d. Требуется найти первое с начала текста
//    вхождение слова в текст (или убедиться в его отсутствии). При этом
//    “вхождением” считается совпадение символов в тексте и слове в
//    соответствующих позициях за исключением не более чем d из них.
//
//  Пример:
//    Дано: текст = “ATTACAUATTAUGGA”, слово = “AATTAGG”, d = 2. Первое
//    вхождение на позиции 6 “UATTAUG”.
//
//----------------------------------------------------------------------------
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

char*
crop_token(char* text, char* token, size_t delta = 0)
{
  size_t d = 0;   // mismatch counter
  size_t i = 0;   // match counter

  const size_t buff_size = 48;  // result buffer size
  char buffer[buff_size]{};     // result buffer

  printf(" \"%s\" in \"%s\"\n\n", token, text);

  for (size_t p = 0; text[p] && token[i]; ++p)
  {

    // matched
    if (text[p] == token[i])
    {
      // writes matches to a buffer
      buffer[i] = text[p];
      i++;
    }
    // mismatched
    else
    {

      // counts mismatch
      if(d <=  delta)
      {
        buffer[i] = text[p];
        i++;
        d++;
      }

      // over mismatch limit
      if (d > delta)
      {
        p = p-i+1;
        d = 0;
        i = 0;

        for(size_t bi = 0; bi < buff_size; bi++)
          buffer[bi] = 0;
      }

    }

    printf(" [ %2c  %16s ] d:%-2d i:%-2d  \n",text[p], buffer, d, i );
  }
  return buffer;
}


int
main()
{
  char text[] = "ATTACAUATTAUGGA";
  char token[] = "AATTAGG";
  size_t delta = 2;

  std::cout << crop_token(text, token, delta) << std::endl;

  return EXIT_SUCCESS;
}
