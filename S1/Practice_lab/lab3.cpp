/*
 * =====================================================================================
 * Механика и математическое моделирование
 * Основы программирования C++, 1 семестр
 * ------------------------
 * Лабораторная работа 3
 *
 * Задание:
 *   Напишите тело функции compute_step.
 *
 * Попробуйте ответить на следующие вопросы:
 *   1. Правильно ли моделируется ситуация, когда пользователь задал Δm = 0 (свободное падение)?
 *   2. Правильно ли моделируется ситуация, когда пользователь задал Δm > (m0 – масса-пустого)?
 *   3. Что произойдёт, если пользователь задаст Δt = 0?
 *   4. Что произойдёт, если пользователь задаст Δt < 0?
 *   5. Возможно ли с заданной точностью (например, до 1 метра) определить момент пересечения
 *        x = 0 (посадка или падение)?
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <cmath>
using namespace std;

// Вспомогательные типы

using Acceleration = double; // м/с2
using Velocity = double; // м/с
using Position = double; // м
using Time = double; // с
using Mass = double; // кг
using Direction = double; // вверх или вниз

const Direction
upwards = +1.0,  // Вверх
downwards = -1.0;  // Вниз

// Параметры модели

Acceleration g;
Velocity I;
Mass empty_m;

// Переменные модели

Time t0, t1;
Mass m0, m1;
Position x0, x1;
Velocity v0, v1;
Acceleration w0, w1;
double r, delta_t_hach, delta_m_hach;

// Консольное приложение
int main()
{
  void select_g();  // Выбор g.
  void select_I();  // Выбор I.
  void step_loop(); // Симуляция полёта.

  while (true)
  {
    select_g();
    select_I();

    t0 = 0; // Обнуляем время.
    cout << "\nEmpty mass = ";
    cin >> empty_m; // Масса пустого.
    cout << "Propellant mass = ";
    cin >> m0; // Масса рабочего тела.
    m0 += empty_m; // Полная масса.

    cout << "x0 = ";
    cin >> x0; // Начальная высота.
    cout << "v0 = ";
    cin >> v0; // Начальная скорость.

    cout << '\n';
    step_loop();

    // Сброс и очистка потока ввода.
    cin.clear();
    cin.sync();
    cin.ignore(cin.rdbuf()->in_avail());
  }

  return EXIT_SUCCESS;
}


// Выбор ускорения свободного падения
void select_g()
{
  const Acceleration
    g_mercury = 3.7,   // Меркурий
    g_moon = 1.62,  // Луна
    g_europa = 1.315, // Европа
    g_pluto = 0.617, // Плутон
    g_ceris = 0.27;  // Церера

  cout << "\nSelect location:\n"
    "0. No gravity\n"
    "1. Mercury\n"
    "2. Moon\n"
    "3. Europa\n"
    "4. Pluto\n"
    "5. Ceris\n";

  switch (cin.get())
  {
  case '0': g = 0.0; break;
  case '1': g = g_mercury; break;
  case '2': g = g_moon; break;
  case '3': g = g_europa; break;
  case '4': g = g_pluto; break;
  case '5': g = g_ceris; break;
  default:
    cout << " g = ";
    cin >> g;
  }

  cin.ignore();
}

// Выбор удельного импульса
void select_I()
{
  const Velocity
    I_RD_253 = 3100,   // Протон-М 1 ст.; гидразин + азотная кислота
    I_NK_33 = 3250,   // Союз-М 1 ст.; керосин + кислород
    I_J_2 = 4170,   // Saturn V, 2/3 ст.; водород + кислород
    I_RD_0120 = 4460,   // Энергия, ускоритель; водород + кислород
    I_arcjet = 16000,  // КА, водород, проект
    I_SPD_230 = 26500,  // КА, ксенон
    I_VISTA = 157000; // D-T ТЯРД, водород, проект

  cout << "\nSelect engine:\n"
    "1. RD-253\n"
    "2. NK-33\n"
    "3. J-2\n"
    "4. RD-0120\n"
    "5. Arcjet\n"
    "6. SPD-230\n"
    "7. VISTA\n";

  switch (cin.get())
  {
  case '1': I = I_RD_253; break;
  case '2': I = I_NK_33; break;
  case '3': I = I_J_2; break;
  case '4': I = I_RD_0120; break;
  case '5': I = I_arcjet; break;
  case '6': I = I_SPD_230; break;
  case '7': I = I_VISTA; break;
  default:
    cout << " I = ";
    cin >> I;
  }

  cin.ignore();
}


// Один полёт
void step_loop()
{
  void compute_step(Time delta_t, Mass delta_m, Direction dir);
  void finish_step();

  cout << "Enter delta-m delta-t for each step\n";

  Time delta_t;
  Mass delta_m;
  while (cin >> delta_m >> delta_t)
  {

    if (delta_m > (m0 - empty_m)) {

      r = delta_m / delta_t;
      delta_t_hach = (m0 - empty_m) / r;
      delta_m_hach = delta_t_hach*r;

      compute_step(delta_t_hach, -abs(delta_m_hach),
        delta_m < 0 ? downwards : upwards);

      compute_step(delta_t - delta_t_hach, 0,
        delta_m < 0 ? downwards : upwards);
    }
    else {
      compute_step(delta_t, -abs(delta_m),
        delta_m < 0 ? downwards : upwards);
    }

    finish_step();
  }
}

// "Перевернуть страницу"
void finish_step()
{
  cout << '\n' << w0 << " ~ " << w1 << "m/s2;\n";
  cout << t1 << "s, " << x1 << "m: ";
  cout << (m1 - empty_m) << "kg; " << v1 << "m/s;\n\n";

  t0 = t1;
  m0 = m1;
  x0 = x1;
  v0 = v1;
  w0 = w1;
}

// Вычислить конечные значения переменных на очередном шаге.
void compute_step(Time delta_t, Mass delta_m, Direction dir)
{
  double delta_r = delta_m / delta_t;

  t1 = t0 + delta_t;
  m1 = m0 + delta_m;

  x1 = x0 + (v0*delta_t) - (g*delta_t*delta_t / 2);
  v1 = v0 - g*delta_t;
  w1 = - g;

  if (delta_m != 0) {
    x1 += I*(delta_t + (m1 / delta_r)*log(m0 / m1))*dir;
    v1 += I*log(m0 / m1);
    w1 += -(I*delta_r / m1);
  }
}
