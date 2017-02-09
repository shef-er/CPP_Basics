/*
 * =====================================================================================
 * Механика и математическое моделирование
 * Основы программирования C++, 1 семестр
 * ------------------------
 * Лабораторная работа 6
 *
 * Задание:
 *   Воплотить в виде функции recursive_trace описанный ниже алгоритм (поиск в глубину со случайным
 *     выбором направлений). Функция изменяет массив maze.
 *
 *   * Поставить в (x, y) пустую клетку.
 *   * Перебирать все направления из набора { запад, восток, север, юг } в случайном порядке:
 *     * Если можно идти через две клетки в выбранном направлении, то
 *       * записать в соседнюю с (x, y) в выбранном направлении клетку пустую клетку;
 *       * запустить recursive_trace в следующей в выбранном направлении клетке.
 *
 * Пример. Допустим выбрано направление “восток”. Тогда “соседняя с (x, y) в выбранном направлении
 *   клетка” имеет координаты (x+1, y), а “следующая в выбранном направлении клетка” —
 *   координаты (x+2, y).
 *
 * Функция traceable вычисляет предикат “можно идти через клетку (x, y)”.
 *
 * --------
 *  bool traceable(int x, int y)
 *  {
 *    return x > 0 && y > 0
 *      && x < maze_width-1 && y < maze_height-1
 *      && maze[y][x] == WALL;
 *  }
 * --------
 *
 * Таким образом, “можно идти через две клетки в выбранном направлении” означает,
 *   что traceable(x+1, y) и traceable(x+2, y) возвращают “истину”.
 *
 * =====================================================================================
 */
// maze_gen.cpp
#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;

#define KEY_H 104;
#define KEY_J 106;
#define KEY_K 107;
#define KEY_L 108;

struct actor {
  int y = 1;
  int x = 1;
  char ch = '@';
};

////////////////////////////////////////////////////////////////////////////////
// Global state.
////////////////////////////////////////////////////////////////////////////////
int last_key;
// map size: 48x16
char maze[16][49]
{
  "################################################",
  "#           #            #                     #",
  "######      #######  ##########     #######    #",
  "#                    #        #         #      #",
  "#           #        #    #   ########  #   ####",
  "# ###########        #    #        #    #      #",
  "#      #    #######  #             #    #      #",
  "#      #          #  #########  ####   #########",
  "#                 #                    #       #",
  "###############   ######## #############       #",
  "#                 #      #       #             #",
  "#   ###########   #      ######       #  #######",
  "#      #                      #       #        #",
  "#      #    ##############    ###############  #",
  "#      #                 #            #        #",
  "################################################"
};
int portals[3][2] {
  {1, 14},
  {22, 3},
  {39, 14}
};
int score = 0;

actor* player = new actor;
actor* victim = new actor;
actor* enemy  = new actor;

////////////////////////////////////////////////////////////////////////////////
// Procedures.
////////////////////////////////////////////////////////////////////////////////

bool
collision(int &x, int &y)
{
  return maze[y][x] == '#';
}

bool
iteract(actor* a, actor* b)
{
  return a->x == b->x &&
         a->y == b->y;
}

int
rand_x()
{
  return rand() % 47 + 1;
}

int
rand_y()
{
  return rand() % 14 + 1;
}

int*
rand_dirs()
{

  int dirs[4] {1, 2, 3, 4};
  random_shuffle(std::begin(dirs), std::end(dirs));

  return dirs;
}

void
rand_spawn(actor* a)
{
  int x_new = rand_x();
  int y_new = rand_y();
  if(!collision(x_new, y_new))
  {
    a->x = x_new;
    a->y = y_new;
  }
}

void
draw_actor(actor* a, int color_pair)
{
  attron(COLOR_PAIR(color_pair));
  mvaddch(a->y, a->x, a->ch | A_BOLD);
}

bool
move_actor(actor* a, int x_new, int y_new)
{
  if(!collision(x_new, y_new))
    {
      a->x = x_new;
      a->y = y_new;
      return 1;
    }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

void
new_game()
{
  player->x = player->y = 1;
  rand_spawn(player);
  score = 0;

  victim->x = 2;
  victim->y = 1;
  rand_spawn(victim);

  enemy->x = 46;
  enemy->y = 14;
  rand_spawn(enemy);

  victim->ch = 'Q';
  enemy->ch = 'W';
}

void
draw_maze()
{
  int y = 0;

  attron(COLOR_PAIR(1));
  for (auto &row : maze)
  {
    mvaddstr(y++, 0, row);
  }

  attron(COLOR_PAIR(11));
  for (auto &portal : portals)
  {
    mvaddch(portal[1], portal[0], '&');
  }
}

void
teleportable(actor* a) {
  int i = 0;
  int modifer = 1;
  for (auto &portal : portals)
  {
    if (a->x == portal[0] &&
        a->y == portal[1])
    {
      move_actor(a,
          portals[abs(i-modifer)][0],
          portals[abs(i-modifer)][1]);
      break;
    }
    i++;
  }
}

void
fireball(actor* a)
{
  char target = '#';
  char bullet = '*';

  bool dir_left  = last_key == KEY_H;
  bool dir_down  = last_key == KEY_J;
  bool dir_top   = last_key == KEY_K;
  bool dir_right = last_key == KEY_L;

  int delta_x = - dir_left + dir_right;
  int delta_y = - dir_top  + dir_down;

  attron(COLOR_PAIR(11));

  if (dir_left || dir_right)
  {
    mvaddch(a->y, a->x + delta_x, bullet | A_BOLD);
    for (int i = a->x; (i > 0) && (i < 47); i+= delta_x)
    {
      if (maze[a->y][i] == target)
      {
        maze[a->y][i] = ' ';
        mvaddch(a->y, i, ' ');
        break;
      }
    }
  }

  if (dir_top || dir_down)
  {
    mvaddch(a->y + delta_y, a->x, bullet | A_BOLD);
    for (int i = a->y; (i > 0) && (i < 15); i+=delta_y)
    {
      if (maze[i][a->x] == target)
      {
        maze[i][a->x] = ' ';
        mvaddch(i, a->x, ' ');
        break;
      }
    }
  }

}

void
turn_player(actor* p)
{
  int x_new = p->x + (last_key == KEY_RIGHT) - (last_key == KEY_LEFT);
  int y_new = p->y + (last_key == KEY_DOWN)  - (last_key == KEY_UP);
  move_actor(p, x_new, y_new);
}

void
turn_victim(actor* p, actor* v) {
  // +1 cell away from player
  int x_new = v->x + (v->x - p->x) % 2;
  int y_new = v->y + (v->y - p->y) % 2;

  // TODO: improve behavior
  move_actor(v, x_new, y_new);
}

void
turn_enemy(actor* p, actor* e) {
  // +1 cell closer to player
  int x_new = e->x - (e->x - p->x) % 2;
  int y_new = e->y - (e->y - p->y) % 2;

  // TODO: improve behavior
  move_actor(e, x_new, y_new);
}

void
turn_end()
{
  if (iteract(player, victim) ||
      iteract(enemy, victim ))
  {
    score += iteract(player, victim);
    victim->x = 2;
    victim->y = 1;
    rand_spawn(victim);
  }

  if (iteract(player, enemy))
  {
    new_game();
  }
}

void
game_turn()
{
  turn_player(player);
  turn_victim(player, victim);
  turn_enemy(player, enemy);
  teleportable(player);
  turn_end();

  draw_maze();
  fireball(player);
  draw_actor(victim, 3);
  draw_actor(player, 2);
  draw_actor(enemy, 4);
}

////////////////////////////////////////////////////////////////////////////////
// Game loop.
////////////////////////////////////////////////////////////////////////////////
int
main()
{
  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);   // maze colors
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // player colors
  init_pair(3, COLOR_BLUE, COLOR_BLACK);    // victim colors
  init_pair(4, COLOR_RED, COLOR_BLACK);     // enemy colors

  init_pair(11, COLOR_GREEN, COLOR_BLACK);   // portal and bullet colors

  raw();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  new_game();
  do
    {
      if (last_key == 'r')
      {
        clear();
        new_game();
      }

      game_turn();
      refresh();
    }
  while (last_key = getch(), last_key != '\x1b'); // escape ASCII code

  endwin();

  delete player;
  delete victim;
  delete enemy;
  cout << "Your score: " << score << endl;
  return 0;
}
