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
    node* next = 0;
};

template <typename T>
node*
new_node(T data, node* p = 0) {
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
append_node_at(node *l, size_t pos, int data = 0)
{
  node *p = l;

  for (size_t i = 1; (i < pos) && (p->next != 0); ++i)
    p = p->next;

  if (p == l)
    *p = *new_node(data, new_node(p->data, p->next)), printf("whoo");
  else
    p->next = new_node(data, p->next);
}

void
remove_node_at(node* &l, size_t pos)
{
  node* p = l;

  if (pos == 0)
  {
    l = p->next;
    delete p;
    return;
  }

  // p-nth is pos - 1  where pos:= 0,1,2..
  for (size_t i = 1; (i < pos) && (p->next->next != 0); ++i)
    p = p->next;

  node* t = p->next;
  if(p->next != 0)
    p->next = p->next->next;
  delete t;
}

void
drop_nodes(node* &l, size_t pos, size_t chunk_size)
{
  node* p = l;
  for (size_t i = pos; i < pos + chunk_size; ++i)
    remove_node_at(l, pos);
}

void
add_nodes(node* &l, size_t pos, size_t chunk_size)
{
  node* p = l;
  for (size_t i = pos; i < pos + chunk_size; ++i)
  {
    append_node_at(l, pos, 42 + i);
    print_list(l);
  }
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

  add_nodes(l, 3, 2);

  print_list(l);

  return 0;
}
