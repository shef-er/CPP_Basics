/*
 * =====================================================================================
 * Механика и математическое моделирование
 * Основы программирования C++, 1 семестр
 * ------------------------
 * Лабораторная работа 7
 *
 * Задание:
 *   Дан набор строк, требуется создать программу, которая на основе префиксного дерева будет
 *     генерировать исходный код (в отдельный файл). Данный исходный код должен быть функцией,
 *     которая по заданной тестовой строке определяет, какая из исходного набора строк совпадает
 *     с тестовой.
 *
 *
 * Очень простой пример:
 *
 *   Пусть дан набор строк:
 *   --------
 *     { "ab", "ad" }
 *   --------
 *
 *   Тогда сгенерированный исходный код может выглядеть так (–1 в качестве результата означает
 *     ситуацию “строка не найдена”):
 *   --------
 *     size_t token(const char *p)
 *     {
 *       switch (*p++)
 *       {
 *         case 'a':
 *           switch (*p++)
 *           {
 *             case 'b': if (*p == '\0') return 0;
 *             case 'd': if (*p == '\0') return 1;
 *             default: return -1;
 *           }
 *         default: return -1;
 *       }
 *     }
 *   --------
 *
 *
 * Средний пример:
 *
 *   Пусть дан набор строк:
 *   --------
 *     { "why", "when", "then", "to", "there", "the" }
 *   --------
 *
 *   Тогда сгенерированный исходный код может выглядеть так (–1 в качестве результата означает
 *     ситуацию “строка не найдена”):
 *   --------
 *     size_t token(const char *p)
 *     {
 *         switch (*p++)
 *         {
 *         case 'w':
 *             switch (*p++)
 *             {
 *             case 'h':
 *                 switch (*p++)
 *                 {
 *                 case 'y': if (*p == '\0') return 0;
 *                 case 'e':
 *                     switch (*p++)
 *                     {
 *                     case 'n': if (*p == '\0') return 1;
 *                     default: return -1;
 *                     }
 *                 default: return -1;
 *                 }
 *             default: return -1;
 *             }
 *         case 't':
 *             switch (*p++)
 *             {
 *             case 'h':
 *                 switch (*p++)
 *                 {
 *                 case 'e': if (*p == '\0') return 5;
 * 
 *                     switch (*p++)
 *                     {
 *                     case 'n': if (*p == '\0') return 2;
 *                     case 'r':
 *                         switch (*p++)
 *                         {
 *                         case 'e': if (*p == '\0') return 4;
 *                         default: return -1;
 *                         }
 *                     default: return -1;
 *                     }
 *                 default: return -1;
 *                 }
 *             case 'o': if (*p == '\0') return 3;
 *             default: return -1;
 *             }
 *         default: return -1;
 *         }
 *     }
 *   --------
 *
 * =====================================================================================
 */
// trie_inorder.cpp
// Simplistic trie.
#include <cstddef>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <fstream>  // вывод в файл: ofstream
using namespace std;

/// Условное значение "позиция не найдена".
const size_t NOT_FOUND = -1;

/// Найти позицию нулевого символа в массиве символов.
size_t
find_null(const char *word, size_t word_len)
{
  for (size_t i = 0; i < word_len; ++i)
    if (word[i] == '\0')
      return i;
  return NOT_FOUND;
}

/// Найти позицию заданного символ в массиве символов.
size_t
find_char(const char *word, size_t word_len, char ch)
{
  for (size_t i = 0; i < word_len && word[i] != '\0'; ++i)
    if (word[i] == ch)
      return i;
  return NOT_FOUND;
}

/// Максимально возможное число потомков одного узла префиксного дерева.
const size_t MAX_TRIE_CHILDREN = 8;
/// Узел префиксного дерева.
struct Trie_node
{
  Trie_node *children[MAX_TRIE_CHILDREN];
  size_t label[MAX_TRIE_CHILDREN];
  char prefix[MAX_TRIE_CHILDREN];

  /// "Конструктор по умолчанию": вызывается при создании узла.
  Trie_node() { prefix[0] = '\0'; }

  /// Поиск свободной позиции в массиве потомков.
  size_t
  find_empty_slot()
  {
    return find_null(prefix, MAX_TRIE_CHILDREN);
  }
  /// Поиск номера потомка, отвечающего очередному символу.
  size_t
  find_char(char ch) const
  {
    return ::find_char(prefix, MAX_TRIE_CHILDREN, ch);
  }

  /// Рекурсивная операция вставки распознаваемой строки с заданной меткой lbl.
  void
  add_word(const char *word, size_t lbl)
  {
    if (*word == '\0')
      return;

    auto index = find_char(*word);
    if (index == NOT_FOUND)
    {
      index = find_empty_slot();
      if (index == NOT_FOUND)
      {
        assert(!"Trie: impossible to insert the next child: the node is full.");
        return;
      }

      prefix[index] = *word;
      if (index + 1 < MAX_TRIE_CHILDREN)
        prefix[index + 1] = '\0'; // The next empty slot.

      children[index] = new Trie_node;
      label[index] = NOT_FOUND; // Unassigned label at first.
    }

    if (word[1] == '\0')
      label[index] = lbl;
    else
      children[index]->add_word(word + 1, lbl);
  }

  /// Извлечение метки, отвечающей заданному слову
  /// (с которой оно было ранее вставлено add_word).
  size_t
  find_word(const char *word) const
  {
    if (*word == '\0')
      return NOT_FOUND;

    auto index = find_char(*word);
    if (index == NOT_FOUND)
      return NOT_FOUND;

    // Это был последний ненулевой символ
    // (== последний символ си-строки)?
    if (word[1] == '\0')
      return label[index];

    return children[index]->find_word(word + 1);
  }

  /// Обход поддерева слева-направо.
  void
  inorder(
    void (*open)(),
    void (*visit)(char, size_t),
    void (*close)())
  {
    if (prefix[0] == '\0')
      return;

    open();
    for (size_t i = 0; i < MAX_TRIE_CHILDREN && prefix[i] != '\0'; ++i)
    {
      visit(prefix[i], label[i]);
      children[i]->inorder(open, visit, close);
    }
    close();
  }
};

/// Префиксное дерево.
struct Trie
{
  Trie_node *root;

  void
  add_word(const char *word, size_t label)
  {
    assert(word);
    if (!root)
      root = new Trie_node;
    root->add_word(word, label);
  }

  size_t
  find_word(const char *word) const
  {
    assert(word);
    if (!root)
      return NOT_FOUND;
    return root->find_word(word);
  }

  void
  inorder(
    void (*open)(),
    void (*visit)(char ch, size_t label),
    void (*close)())
  {
    if (root)
      root->inorder(open, visit, close);
  }
};

///////////////////////////////////////////////////////////////////////////////
/// Вспомогательный код для "распечатки" дерева.
ofstream file("test.cpp"); // Файл для вывода результата.
char tab[7]; // Строка текущих отступов.
bool start;  // Флаг "ожидается первый потомок".

void
print_open()
{
  auto pos = find_null(tab, sizeof(tab) - 1);
  if (pos != NOT_FOUND)
    tab[pos] = '\t';
  file << '\n';
  file << tab << "switch (*p++)\n";
  file << tab << "{";

  start = true;
}

void
print_visit(char ch, size_t label)
{
  file << '\n';
  file << tab << "case '" << ch << "': ";

  if (label != NOT_FOUND)
    file << "if (*p == '\\0') return " << label << ";";

  start = false;
}

void
print_close()
{
  auto pos = find_null(tab, sizeof(tab));
  if (pos != 0)
    tab[pos-1] = '\0';

  file << '\n';
  file << tab << "\tdefault: return -1;\n";
  file << tab << "\t}\n";

  start = true;
}


int
main()
{

/*
  const char *words[]
  {
    "water",
    "was",
    "while",
    "when",
    "where",
    "why",
    "what",
    "whilst",
    "then",
    "there",
    "these",
    "today",
    "to",
    "the"
  };
*/

  const char *words[]
  {
    "why", "when", "then", "to", "there", "the"
  };

  Trie trie{};

  // Поместить все строки из words в префиксное дерево.
  // В качестве метки -- индекс строки в массиве.
  for (size_t i = 0, iend = end(words) - begin(words); i < iend; ++i)
    trie.add_word(words[i], i);

  file << "size_t token(const char *p)\n{";
  // Вывести префиксное дерево в файл вместе с метками.
  trie.inorder(print_open, print_visit, print_close);
  file << "}\n";
/*
  // Выводить метки слов, которые вводит пользователь.
  for (string word; cin >> word;)
  {
    const auto label = trie.find_word(word.c_str()); // token(word.c_str())
    if (label == NOT_FOUND)
      cout << "<not found>\n";
    else
      cout << label << ": \"" << words[label] << "\"\n";
  }
*/
  return 0;
}
