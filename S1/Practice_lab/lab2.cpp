/*
 * =====================================================================================
 * Механика и математическое моделирование
 * Основы программирования C++, 1 семестр
 * ------------------------
 * Лабораторная работа 2
 *
 * Задание 1:
 *    Вариант 1: Реализовать и протестировать функцию xor. Использовать операции !, &&, ||.
 *    Вариант 2: Реализовать и протестировать функцию xnor. Использовать операции !, &&, ||.
 *
 * Задание 2:
 *    Вариант 1: Реализовать и протестировать функцию nand_or (реализация or через nand).
 *    ВАриант 2: Реализовать и протестировать функцию nor_and (реализация and через nor).
 *
 * =====================================================================================
 */
#include <iostream>
#include <string>
using namespace std;

bool nand(bool a, bool b)
{
  return !(a && b);
}

bool nor(bool a, bool b)
{
  return !(a || b);
}

//
// Решения заданий

// задание 1
bool valve_xor(bool a, bool b) 
{
    return (a && !b) || (!a && b);
}

bool valve_xnor(bool a, bool b) 
{
    return (a && b) || (!a && !b);
}


// задание 2
bool nand_or(bool a, bool b)
{
  return nand(!a,!b);
}

bool nor_and(bool a, bool b)
{
  return nor(!a,!b);
}

//
// конец решения

int main()
{
// Пример работы xor
    cout << "xor" << endl;
    cout << "a/b | 0 | 1" << endl;
    cout << "  0 | " << valve_xor(false, false) << " | " << valve_xor(true, false) << endl;
    cout << "  1 | " << valve_xor(false, true)  << " | " << valve_xor(true, true) << endl;

// Пример работы xnor
    cout << endl;
    cout << "xnor" << endl;
    cout << "a/b | 0 | 1" << endl;
    cout << "  0 | " << valve_xnor(false, false) << " | " << valve_xnor(true, false) << endl;
    cout << "  1 | " << valve_xnor(false, true)  << " | " << valve_xnor(true, true) << endl;

// Пример работы nor_and
    cout << endl;
    cout << "nor_and" << endl;
    cout << "a/b | 0 | 1" << endl;
    cout << "  0 | " << nor_and(false, false) << " | " << nor_and(true, false) << endl;
    cout << "  1 | " << nor_and(false, true)  << " | " << nor_and(true, true) << endl;

// Пример работы nand_or
    cout << endl;
    cout << "nand_or" << endl;
    cout << "a/b | 0 | 1" << endl;
    cout << "  0 | " << nand_or(false, false) << " | " << nand_or(true, false) << endl;
    cout << "  1 | " << nand_or(false, true)  << " | " << nand_or(true, true) << endl;

    return EXIT_SUCCESS;
}
