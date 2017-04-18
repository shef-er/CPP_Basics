/*
Построить конечный автомат, распознающий последовательности из нулей и единиц,
содержащие чётное число единиц и не содержащие единиц,
идущих подряд друг за другом.

Примеры:
00000 — принять
01000 — не принимать (нечётное число единиц)
00011 — не принимать (две единицы подряд)
10100 — принять
10110 — не принимать (нечётное число единиц и две единицы подряд)
*/

#include <iterator>
#include <vector>
#include <utility>
#include <iostream>
#include <cassert>

// Конечный автомат.
template <class It>
bool
sequence_type(It from, It to)
{
	enum FA_State
	{
		zero = 0,
		one = 1,
		start = 2,
		fail = 4,
	};

	FA_State state = start;
	size_t acc = 0;
	bool type = false;

	for (It tmp = from; tmp + 1 < to; ++tmp)
	{
		switch (state)
		{

		case start:
			if (*tmp == 1)
			{
				acc++;
				state = one;
			}
			if (*tmp == 0)
				state = zero;
			break;

		case zero:
			if (*tmp == 1)
			{
				acc++;
				state = one;
			}
			break;

		case one:
			if (*tmp == 1)
				state = fail;
			if (*tmp == 0)
				state = zero;
			break;

		default:
			assert(!"impossible state!");
		}
	}

	if ( state != fail && acc % 2 == 0)
		type = true;

	std::cout << "(" << state << ", " << acc << " ) " << type << std::endl;

	return type;
}

// Тесты.
int test_sequence_type()
{
	using namespace std;

	{
		int i[] { 0, 0, 0, 0 };
		if (!sequence_type(begin(i), end(i)))
			return 1;
	}

	{
		int i[] { 0, 1, 0, 0, 0 };
		if (sequence_type(begin(i), end(i)))
			return 2;
	}

	{
		int i[] { 0, 0, 0, 1, 1 };
		if (sequence_type(begin(i), end(i)))
			return 3;
	}

	{
		int i[] { 1, 0, 1, 0, 0 };
		if (!sequence_type(begin(i), end(i)))
			return 4;
	}

	{
		int i[] { 1, 0, 1, 1, 0 };
		if (sequence_type(begin(i), end(i)))
			return 5;
	}

	return 0; // Все тесты прошли успешно.
}

int main()
{
	std::cout << test_sequence_type() << std::endl;
	//std::cin.ignore();
	return 0;
}
