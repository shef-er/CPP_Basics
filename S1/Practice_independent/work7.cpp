//  Лабораторная 7 (14 вариант):
//    Найти первое вхождение заданного слова в тексте, представленном массивом
//    строк, при условии, что слово может быть разбито переносом (знаком -,
//    завершающим строку) в любой позиции. Вхождение определяется как пара 
//    чисел: индекс строки в массиве и индекс первого символа слова в строке.
//
//----------------------------------------------------------------------------

//
// Задача.
//
// Вычленить в строке слова, заполнив массив си-строк указателями на первые
//   буквы слов,
// а пробельные символы заменив нулями.
// Память под массив выделяется извне (пользователем).
// Функция принимает указатель на целевой массив и его размер,
// возвращает указатель на последний необработанный символ (нулевой указатель,
//   если вся строка обработана).
// Элемент, следующий за последним записанным в массив указателем должен быть
//   нулём.
//
// Алгоритм.
//
// Пока исходная строка не кончилась, циклически повторяем два действия:
//
// 1. Проходим по исходной строке, затирая нулями пробельные символы.
// Если встретился непробельный символ, то записываем указатель на него на
//   следующую позицию в массиве.
// 2. Двигаемся по строке до первого пробельного символа.
//

#include <iostream>
#include <cassert>
#include <cstddef> // size_t
#include <cctype> // isspace
#include <cstring>


struct tp
{
  size_t row = 0;
  size_t col = 0;
};


/// Затереть последовательность пробельных символов нулевым символом.
char*
zero_spaces(char *text)
{
  while (std::isspace(*text))
    *text++ = '\0';
  return text;
}

/// Пропустить все непробельные символы (кроме завершающего нуля).
char*
skip_non_spaces(char *line)
{
  while (*line != '\0' && !std::isspace(*line))
    ++line;
  return line;
}


/// Выполнить вычленение "слов" в строке text, указатели на слова записать в 
///   массив words.
/// Массив words создаётся кодом, вызывающим эту функцию.
/// Параметром words_size передаётся размер массива words.
char*
split(char *text, char *words[], std::size_t words_size)
{
  assert(words_size > 1);
  const auto max_words = words_size - 1;
  for (std::size_t word = 0; word < max_words; ++word)
  {
    text = zero_spaces(text);
    if (*text == '\0')
    {
      words[word] = nullptr;
      return nullptr;
    }

    words[word] = text;
    text = skip_non_spaces(text);
  }

  words[max_words] = nullptr;

  // Перейти к началу следующего слова.
  text = zero_spaces(text);
  // Возможно, строка кончилась (т.е. был "хвост" из пробельных символов),
  //   тогда вернуть nullptr.
  return *text != '\0' ? text : nullptr;
}


//----------------------------------------------------------------------------
// all the stuff after this line is written by me

bool
are_equal(char * text1, char * text2)
{
  for (; *text1 && *text2; ++text1, ++text2)
  {
    if (*text1 == '-')
      ++text1;
    if (*text1 != *text2)
      return false;
  }
  return true;
}

bool
flatten(char ** text, char * out, size_t max_lines = 10)
{
  for (size_t i = 0; i < max_lines && text[i] != nullptr; ++i)
  {
      strcat(out, " ");
      strcat(out, text[i]);
  }
  return true;
}

tp
find_token(char ** input, char * token) {
  tp pos;
  size_t max_lines = 100;

  for (size_t i = 0; (i < max_lines) && (input[i] != nullptr); ++i)
  {
      char* r = strstr(input[i], token);
      if (r != nullptr)
      {
        pos.row = i+1;
        pos.col = r - input[i] + 1;

        return pos;
      }
  }

  return pos;
}

tp
search_token(char** input, char* token) {
  tp pos;
  const auto count = 100;
  char * words[count];
  char temp[100];
  char text[10000];

  // converts char** to char*
  flatten(input, text);
  split(text, words, count);

  for (size_t i = 0; i < count; i++)
  {
    if (strchr(words[i], '-'))
    {
      strcpy(temp, words[i]);
      strcat(words[i], words[i + 1]);

      if (are_equal(words[i], token))
      {
        pos = find_token(input, temp);
        return pos;
      }
    }
    else if (are_equal(words[i], token))
    {
      pos = find_token(input, token);
      return pos;
    }
  }
  return pos;
}

void
print_text(char ** text, size_t max_lines = 10)
{
  for (size_t i = 0; (i < max_lines) && (text[i] != nullptr); ++i)
  {
    printf("%s\n", text[i]);
  }
}

int main()
{
  char* text[] {
    "If you have ghosts, you have everything",
    "You can say any-",
    "thing you want",
    "And you can do anything you want to do",
    nullptr
  };
  char token[] = "anything";

  print_text(text);

  tp result = search_token(text, token);
  printf("token \"%s\" found on %d, %d \n", token, result.row, result.col);

  return EXIT_SUCCESS;
}
