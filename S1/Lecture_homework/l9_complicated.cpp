//  Задание:
//    Доработать "Многоножку", добавив функции для перестановки указанных
//    сегментов node, для удаления группы элементов начиная с указанного
//    места и для вставки элементов в указанное место. 
//
#include <iostream>
using namespace std;

struct node
{
    int data;
    node *next = 0;
};

template <typename T>
node
*new_node(T data, node *p = 0) {
  node *n = new node();
  n->data = data;
  n->next = p;
  return n;
}

///////////////////////////////////////

void
print_list(node *l)
{
  node *p = l;
  while (p != 0)
  {
    printf("%3d", p->data);
    p = p->next;
  }
  cout << endl;
}

void
append_node(node* l, int data = 0)
{
  node *p = l;
  while (p->next != 0)
    p = p->next;
  p->next = new_node(data);
}

/*
void
add_node_at(node* &l, size_t pos = 0, int data = 0)
{
  node* p = l;
  for (size_t i = 0; i <= pos; ++i)
    if (p->next != 0)
      p = p->next, printf("%3d", i);
    else
      printf("%3d", i);

  cout << endl;
  if (p->next != 0)
  {
    *p = *new_node(data, 0);
  }
  else
    *p = *new_node(data, new_node(p->data, p->next));
}*/

void
add_node_at(node* &l, size_t pos = 0, int data = 0)
{
  node* p = l;
  for (size_t i = 0; i <= pos && (p->next != 0); ++i)
      p = p->next;

  if (p->next != 0)
  {
    *p = *new_node(data, 0);
  }
  else
    p = *new_node(data, new_node(p->data, p->next));
}

void
del_node_at(node* &l, size_t pos = 0)
{
  node* p = l;
  for (size_t i = 0; (i < pos) && (p->next != 0); ++i)
    p = p->next;
  *p = *new_node(0, new_node(0, p->next));
}

///////////////////////////////////////

int
main()
{
  size_t lsize = 8;
  node *l = 0;
  node *p = 0;

  for (size_t i = 0; i < lsize; ++i)
  {
    node *n = new_node(i);

    if (l == 0)
      l = n;
    else
      p->next = n;
    p = n;
  }

  add_node_at(l, 10, 42);

  print_list(l);

  return 0;
}
