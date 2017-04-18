#include <iterator>
#include <utility> // pair
#include <vector>
#include <iostream>

enum Sequence_type
{
  seq_const = 0,
  seq_down = 1,
  seq_up = 2,
  seq_down_down = 4,
  seq_down_up = 5,
  seq_up_down = 6,
  seq_up_up = 7,
  seq_other = 3
};

// Конечный автомат.
// Возвращает пару "тип последовательности", итератор начала второго куска (если подходящий тип последовательности).
template <class It>
std::pair<Sequence_type, It>
sequence_type(It from, It to)
{
  // Заполнить ...
  enum FA_State
  {
    fa_const = 0,
    fa_down = 1,
    fa_up = 2,
    fa_down_other = 3,
    fa_up_other = 4,

    fa_final_down = 5,
    fa_final_up = 6,
  };

  FA_State state = fa_const;
  Sequence_type type = seq_const;
  It tmp = from;
  It lbl = from;
  bool running = true;

  for (; tmp + 1 < to && running; ++tmp)
    switch (state)
  {
    case fa_const:
      if (*(tmp + 1) < *tmp)
      {
        type = seq_down;
        state = fa_down;
      }
      else if (*(tmp + 1) > *tmp)
      {
        type = seq_up;
        state = fa_up;
      }
      break;

    case fa_down:
      if (*(tmp + 1) > *tmp)
      {
        std::cout << "_d";
        type = seq_other;
        state = fa_down_other;
        lbl = tmp+1;
      }
      break;

    case fa_up:
      if (*(tmp + 1) < *tmp)
      {
        type = seq_other;
        state = fa_up_other;
        lbl = tmp+1;
      }
      break;

    case fa_down_other:
      if (*(tmp + 1) < *tmp)
      {
        std::cout << "_do";
        type = seq_down_down;
        state = fa_final_down;
      }
      if (*(tmp + 1) > *tmp)
      {
        type = seq_down_up;
        state = fa_final_up;
      }
      break;

    case fa_up_other:
      if (*(tmp + 1) < *tmp)
      {
        type = seq_up_down;
        state = fa_final_down;
      }
      if (*(tmp + 1) > *tmp)
      {
        type = seq_up_up;
        state = fa_final_up;
      }
      break;

    case fa_final_down:
      if (*(tmp + 1) > *tmp)
      {
        type = seq_other;
        //lbl = tmp;
      }
      break;

    case fa_final_up:
      if (*(tmp + 1) < *tmp)
      {
        type = seq_other;
        //lbl = tmp;
      }
      break;

  }

  std::pair<Sequence_type, It> result(type, lbl);

  std::cout << "(" << state << ":" << type << ")";
  std::cout << "[" << *(lbl - 1) << " <" << *(lbl) << "> " << *(lbl+1) << "] ";
  std::cout << std::endl;
  return result;
}

// Тесты.
int test_sequence_type()
{
  using namespace std;
  // Тест на пустую.
  {
    int i;
    if (sequence_type(&i, &i).first != seq_const)
      return 1;
  }

  // Тест на константу -- 1 элемент.
  {
    int i = 0;
    if (sequence_type(&i, &i + 1).first != seq_const)
      return 2;
  }

  // Тест на константу -- 2 элемента.
  {
    int i[] { 1, 1 };
    if (sequence_type(begin(i), end(i)).first != seq_const)
      return 3;
  }

  // Тест на константу -- 3 элемента.
  {
    int i[] { -1, -1, -1 };
    if (sequence_type(begin(i), end(i)).first != seq_const)
      return 4;
  }

  // Тест на монотонно возрастающую.
  {
    int i[] { -1, 0 };
    if (sequence_type(begin(i), end(i)).first != seq_up)
      return 5;
  }

  // Тест на монотонно возрастающую.
  {
    int i[] { -1, 0, 1 };
    if (sequence_type(begin(i), end(i)).first != seq_up)
      return 6;
  }

  // Тест на монотонно возрастающую.
  {
    int i[] { 0, 0, 1, 1 };
    if (sequence_type(begin(i), end(i)).first != seq_up)
      return 7;
  }

  // Тест на монотонно убывающую.
  {
    int i[] { 1, 0 };
    if (sequence_type(begin(i), end(i)).first != seq_down)
      return 8;
  }

  // Тест на монотонно убывающую.
  {
    int i[] { 1, 0, -1 };
    if (sequence_type(begin(i), end(i)).first != seq_down)
      return 9;
  }

  // Тест на монотонно убывающую.
  {
    int i[] { 1, 1, 0, 0 };
    if (sequence_type(begin(i), end(i)).first != seq_down)
      return 10;
  }

  // Тест на случай down-down.
  {
    int i[] { 5, 3, 6, 4 };
    if (sequence_type(begin(i), end(i)) != make_pair(seq_down_down, begin(i) + 2))
      return 11;
  }

  // Тест на случай down-down.
  {
    int i[] { 5, 5, 3, 3, 6, 6, 4, 4 };
    if (sequence_type(begin(i), end(i)) != make_pair(seq_down_down, begin(i) + 4))
      return 12;
  }

  // Тест на случай down-up.
  //{
    //int i[] { 5, 3, 0, 4 };
    //if (sequence_type(begin(i), end(i)) != make_pair(seq_down_up, begin(i) + 2))
      //return 13;
  //}

  // Тест на случай down-up.
  //{
  //  int i[] { 5, 5, 3, 3, 0, 0, 4, 4 };
  //  if (sequence_type(begin(i), end(i)) != make_pair(seq_down_up, begin(i) + 4))
  //    return 14;
  //}

  // Тест на случай up-down.
  {
    int i[] { 3, 5, 4, 0 };
    if (sequence_type(begin(i), end(i)) != make_pair(seq_up_down, begin(i) + 2))
      return 15;
  }

  // Тест на случай down-up.
  {
    int i[] { 3, 3, 5, 5, 4, 4, 0, 0 };
    if (sequence_type(begin(i), end(i)) != make_pair(seq_up_down, begin(i) + 4))
      return 16;
  }

  // Тест на случай up-up.
  {
    int i[] { 3, 5, 0, 4 };
    if (sequence_type(begin(i), end(i)) != make_pair(seq_up_up, begin(i) + 2))
      return 17;
  }

  // Тест на случай up-up.
  {
    int i[] { 3, 3, 5, 5, 0, 0, 4, 4 };
    if (sequence_type(begin(i), end(i)) != make_pair(seq_up_up, begin(i) + 4))
      return 18;
  }

  return 0; // Все тесты прошли успешно.
}

int main()
{
  std::cout << test_sequence_type() << std::endl;
  //std::cin.ignore();
  return 0;
}
