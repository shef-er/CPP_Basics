//  Задание:
//    Написать программу, которая реализует в памяти следующую структуру
//    хранения, представленную на рисунке (модель "многонога"):
//
#include <iostream>
using namespace std;

struct node
{
    int data;
    node* next;
};

struct list
{
  node* head;
  node* tail;
  node* temp;
};


void
create_list(list &l)
{
  l.head = 0;
}

void
add_node_list(list &l, int val)
{
  node* n = new node();
  n->data = val;
  n->next = 0;
  if (l.head == 0)
    l.head = n;
  else
    l.tail->next = n;
  l.tail = n;
}

void
print_nodes_list(list l, int count)
{
  node* p = l.head;
  
  while(p != 0){
    cout << p->data << " ";
    p = p->next;
  }
  cout << endl;
}

int
main()
{
  int n = 4;
  cin >> n;
  list dynamic_list;
  create_list(dynamic_list);

  for(int i = 0; i < n; i++)
    add_node_list(dynamic_list, i+1);

  print_nodes_list(dynamic_list, n);

  return 0;
}
