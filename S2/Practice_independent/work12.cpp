/*
* =====================================================================================
* Механика и математическое моделирование
* Основы программирования C++, 2 семестр
* ------------------------
* Самостоятельная работа 12, вариант 7
*
* Задание:
*     Для конкатенации последовательности файлов определить частоты (количества) всех
*       встречающихся в нём троек идущих друг за другом букв без учёта регистра.
*       Тройки с нулевыми частотами не выводить.
*
*----------------------------------------------------------------------------
*/

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <climits> // CHAR_BIT

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <iterator>
#include <algorithm>
#include <functional>

#include <vector>
#include <map>
#include <unordered_map>

using namespace std;
using Letter_seq = std::map< std::string, std::uint64_t >;

ctype_base::mask*
my_ctype_table(const locale &base_loc)
{
  static const auto table_size = 1u << CHAR_BIT;
  auto table = new ctype_base::mask[table_size];
  for (unsigned i = 0; i < table_size; ++i)
    table[i] = isalnum((char)i, base_loc) ?
    ctype_base::alpha
    : ctype_base::space;

  table['-'] = ctype_base::space;
  table[','] = ctype_base::space;
  table['0'] = ctype_base::space;
  table['1'] = ctype_base::space;
  table['2'] = ctype_base::space;
  table['3'] = ctype_base::space;
  table['4'] = ctype_base::space;
  table['5'] = ctype_base::space;
  table['6'] = ctype_base::space;
  table['7'] = ctype_base::space;
  table['8'] = ctype_base::space;
  table['9'] = ctype_base::space;
  return table;
}

void
count_seq(istream &source, Letter_seq &counters)
{

  // Считаем, что source содержит слова, разделённые пробелами.
  for (string word; source >> word;)
  {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    for (size_t i = 0; i+2 < word.size(); i++)
      counters[word.substr(i, 3)]++;
  }
}

// Вывести в поток произвольный map.
template <class K, class V>
ostream& print(ostream &os, const map<K, V> &m)
{
  for (auto &kv : m)
    os << kv.first << ' ' << kv.second << '\n';
  return os;
}

int
main(int argc, char *argv[])
{
  //locale base_loc("Russian_Russia.866");
  locale base_loc("ru_RU.utf8");

  ifstream input;
  ofstream output;

  Letter_seq counters;
  vector<pair<uint64_t, string>> vp;

  // ---- //

  cout.imbue(base_loc); // Задать локаль потоку вывода.

  // Создать новую локаль, подставив фасет ctype с новой таблицей символов.
  locale my_loc(base_loc, new ctype<char>(my_ctype_table(base_loc), true));
  cin.imbue(my_loc); // Задать локаль потоку ввода.
  input.imbue(my_loc);

  cout << "----------------" << endl;
  cout << "Base locale name:    " << base_loc.name() << endl;
  cout << "Changed locale name: " << my_loc.name() << endl;
  cout << "----------------" << endl;

  // ---- //

  if (argc == 1)
    return -1;

  for (int i = 1; i < argc ; i++)
  {
    input.close();
    input.clear();
    input.open(argv[i]);

    if (!input.is_open()) // открыть не удалось.
      return -1;
    if (!input)
      throw 1;

    count_seq(input, counters);

    cout << argv[i] << endl;
  }
  cout << "----------------" << endl;

  vp.reserve(counters.size());
  for (auto &kv : counters)
    vp.emplace_back(kv.second, kv.first);

  sort(begin(vp), end(vp), greater<>());

  output.open ("output.txt");
  for (auto &p : vp)
    output << p.second << " " << p.first << endl;
  output.close();

  for (auto &p : vp)
    cout << p.second << " " << p.first << endl;

  return 0;
}

