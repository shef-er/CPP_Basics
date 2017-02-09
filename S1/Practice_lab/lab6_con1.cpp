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
#include "con1.hpp"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <utility>
using namespace con1;
using std::swap;

////////////////////////////////////////////////////////////////////////////////
// Global constants.
////////////////////////////////////////////////////////////////////////////////
const Achar
EMPTY{ ' ', color::grey, color::black },
WALL{ '#', color::grey, color::black },
PLAYER{ '@', color::yellow, color::black },
FLOOD{ '~', color::blue, color::dark_blue };

////////////////////////////////////////////////////////////////////////////////
// Global state.
////////////////////////////////////////////////////////////////////////////////
int maze_width = 9, maze_height = 9;
int maze_total_size;
int player_x, player_y;

Achar *maze_packed, *maze_packed_image;
Achar **maze, **maze_image;

////////////////////////////////////////////////////////////////////////////////
// Procedures.
////////////////////////////////////////////////////////////////////////////////
void make_maze(Achar *&packed, Achar **&rows)
{
	packed = new Achar[maze_total_size];
	rows = new Achar*[maze_height];

	rows[0] = packed;
	for (int row = 1; row < maze_height; ++row)
		rows[row] = rows[row - 1] + maze_width;
}


bool passable(int x, int y)
{
	return x >= 0 && y >= 0
		&& x < maze_width && y < maze_height
		&& maze_image[y][x] == EMPTY;
}

void flood_fill(int x, int y)
{
	if (passable(x, y))
	{
		maze_image[y][x] = FLOOD;
		flood_fill(x - 1, y);
		flood_fill(x + 1, y);
		flood_fill(x, y - 1);
		flood_fill(x, y + 1);
	}
}


bool traceable(int x, int y)
{
	return x > 0 && y > 0
		&& x < maze_width - 1 && y < maze_height - 1
		&& maze[y][x] == WALL;
}

void recursive_trace(int x, int y)
{
	// 4 random directions
	int dirs[] {1, 2, 3, 4};
	std::random_shuffle(&dirs[0], &dirs[4]);

	for (int i = 0; i < 4; i++) 
	{
		switch (dirs[i]) 
		{
			case 1: // Up
			{
					if (y - 2 <= 0)
						continue;

					if (maze[y - 2][x] == WALL)
					{
						maze[y - 1][x] = EMPTY;
						maze[y - 2][x] = EMPTY;
						recursive_trace(x, y - 2);
					}
					break;
			}

			case 2: // Down
			{
					if (y + 2 >= maze_height - 1)
						continue;

					if (maze[y + 2][x] == WALL)
					{
						maze[y + 1][x] = EMPTY;
						maze[y + 2][x] = EMPTY;
						recursive_trace(x, y + 2);
					}
					break;
			}
			case 3: // Left
			{
					if (x - 2 <= 0)
						continue;

					if (maze[y][x - 2] == WALL)
					{
						maze[y][x - 1] = EMPTY;
						maze[y][x - 2] = EMPTY;
						recursive_trace(x - 2, y);
					}
					break;
			}
			case 4: // Right
			{
					if (x + 2 >= maze_width - 1)
						continue;

					if (maze[y][x + 2] == WALL)
					{
						maze[y][x + 1] = EMPTY;
						maze[y][x + 2] = EMPTY;
						recursive_trace(x + 2, y);
					}
					break;
			}

		}

	}
}

void new_maze()
{
	// Delete the old one.
	delete[] maze;
	delete[] maze_image;
	delete[] maze_packed;
	delete[] maze_packed_image;

	// Enter new maze sizes.
	cls();
	input_echo(true);
	cursor_visible(true);

	cout << "Enter maze width and height: ";
	cin >> maze_width >> maze_height;

	input_echo(false);
	cursor_visible(false);
	cls();

	if (maze_width < 3)
		maze_width = 3;
	if (maze_height < 3)
		maze_height = 3;

	// Allocate memory for the new maze.
	maze_total_size = maze_width * maze_height;
	make_maze(maze_packed, maze);
	make_maze(maze_packed_image, maze_image);

	// Randomize.
	std::srand((unsigned)(3e+6 * seconds()));

	////////////////////////////
	// Generate maze.

	// Part 1. All is WALL.
	for (int i = 0; i < maze_total_size; ++i)
		maze_packed[i] = WALL;

	// Part 2. The Inner Maze.
	int stx = maze_width / 2, sty = maze_height / 2;
	stx += (stx & 1) == 0;
	sty += (sty & 1) == 0;

	recursive_trace(stx, sty);

	// Initialize the maze image.
	std::memcpy(maze_packed_image, maze_packed, maze_total_size * sizeof(Achar));
}

void new_game()
{
	new_maze();

	// Place the player.
	player_x = player_y = 1;
	maze_image[player_y][player_x] = PLAYER;
	maze[player_y][player_x] = EMPTY;
}

void draw_maze()
{
	at(0, 0);
	draw(maze_packed_image, maze_height, maze_width);
}

void move_player()
{
	int nx = player_x + is_pressed(key::right) - is_pressed(key::left);
	int ny = player_y + is_pressed(key::down) - is_pressed(key::up);
	if (maze[ny][nx] == EMPTY)
	{
		maze_image[player_y][player_x] = maze[player_y][player_x];
		maze_image[ny][nx] = PLAYER;
		player_y = ny;
		player_x = nx;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Game loop.
////////////////////////////////////////////////////////////////////////////////
int main()
{
	seconds(); // initialize the clock
	set_input_mode(mode::sync_input);
	new_game();

	while (!is_pressed(key::escape))
	{
		sync_input();
		if (is_pressed('r'))
		{
			reset_key('r');
			new_game();
		}

		if (is_pressed('f'))
		{
			maze_image[player_y][player_x] = EMPTY;
			flood_fill(player_x, player_y);
			maze_image[player_y][player_x] = PLAYER;
		}

		move_player();
		draw_maze();
		sleep(50);
	}

	return 0;
}
