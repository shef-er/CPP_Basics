/*
 * =====================================================================================
 * Механика и математическое моделирование
 * Основы программирования C++, 1 семестр
 * ------------------------
 * Лабораторная работа 4
 *
 *  Код   Мнемоника   Смысл
 *  0     halt        останов
 *  1     move        вперёд
 *  2     left        налево
 *  3     right       направо
 *
 *  У команд 1–3 есть однобайтовый параметр — дальность хода (move) в метрах и угол поворота
 *    (left, right) в градусах. Таким образом, команда halt занимает один байт в памяти, а команды
 *    move, left и right — два байта (код команды и аргумент команды).
 *
 *  Пример программы робота (uint8_t определён в <cstdint>):
 * --------
 *    std::uint8_t program[]
 *    {
 *      1, 5,   // продвинуться вперёд на 5 метров
 *      3, 45,  // повернуться направо на 45 градусов
 *      1, 3,   // продвинуться вперёд на 3 метра
 *      2, 90,  // повернуться налево на 90 градусов
 *      1, 3,   // продвинуться вперёд на 3 метра
 *      2, 135, // повернуться налево на 135 градусов
 *      1, 5,   // продвинуться вперёд на 5 метров
 *      2, 45,  // повернуться налево на 45 градусов
 *      1, 3,   // продвинуться вперёд на 3 метра
 *      3, 90,  // повернуться направо на 90 градусов
 *      1, 3,   // продвинуться вперёд на 3 метра
 *      3, 135, //повернуться направо на 135 градусов
 *      0       // останов
 *    };
 * --------
 *
 * Предположим, что состояние робота описывается тремя переменными: координатами на плоскости
 *   x, y и углом поворота α (удобнее в радианах или в градусах?). Напишем функцию, принимающую
 *   начальное состояние робота и программу, заданную массивом байт, и возвращающую конечное
 *   состояние робота после выполнения данной программы.
 *
 * Так как нам придётся принимать и возвращать состояние робота, состоящее из трёх значений,
 *   удобно описать это состояние структурой, содержащей три поля.
 *
 * --------
 *    #include <cstdint> // std::uint8_t
 *
 *    // Состояние робота.
 *    struct Robot_state
 *    {
 *      double x, y, alpha;
 *    };
 *
 *    // Определение операции вывода состояния робота в поток выводы.
 *    std::ostream& operator<<(std::ostream & os, const Robot_state & robot)
 *    {
 *      return os << "(" << robot.x << ", " << robot.y << "; " << robot.alpha << ")";
 *    }
 * --------
 *
 * Теперь можно оперировать группой из трёх значений как одним значением, в частности возвращать
 *   из функции “разом”:
 *
 * --------
 *   Robot_state robot_walk(Robot_state robot, uint8_t program[], size_t max_commands)
 *   {
 *     // ???
 *   }
 * --------
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <cmath>
using namespace std;

#define PI 3.14159265

std::uint8_t program[]
{
        1, 10,  // продвинуться вперёд на 10 метров
        3, 45,  // повернуться направо на 45 градусов
        1, 12,  // продвинуться вперёд на 12 метров
        2, 135, // повернуться налево на 135 градусов
        1, 2,   // продвинуться вперёд на 2 метра
        0       // останов
};

// Состояние робота.
struct Robot_state
{
    double x, y, alpha;
};

// Определение операции вывода состояния робота в поток выводы.
std::ostream& operator<<(std::ostream & os, const Robot_state & robot)
{
    return os << "(" << robot.x << ", " << robot.y << "; " << robot.alpha << ")";
}

Robot_state robot_walk(Robot_state robot, uint8_t program[], size_t max_commands, Robot_state robot_states[])
{
    for (int i = 0; i <= (int)max_commands; i += 2) {
        switch (program[i]) {
        case 0:
            return robot;
            break;
        case 1:
            robot.x += cos(robot.alpha * PI / 180)*program[i + 1];
            robot.y += sin(robot.alpha * PI / 180)*program[i + 1];
            break;
        case 2:
            robot.alpha += program[i + 1];
            break;
        case 3:
            robot.alpha -= program[i + 1];
            break;
        default:
            i -= 1;
            break;
        }
        robot_states[(i/2)] = robot;

    }
    return robot;
}

int main()
{
    Robot_state robot;
    Robot_state robot_states[]{};

    robot.x = 0;
    robot.y = 0;
    robot.alpha = 0;


    for (int i = 0; i <= (int)((sizeof(program)/sizeof(*program))/2); i++) {
        cout << "    " << i+1 << ": " << robot_states[i] << endl;
    }


    cout << "final: " << robot_walk(robot,
        program, (sizeof(program)/sizeof(*program)),
        robot_states, ((sizeof(program)/sizeof(*program))/2) ) << endl;


    return EXIT_SUCCESS;
}
