// utf_word_freqs.cpp
#include <cassert>
#include <cstddef>
#include <cstdint>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <iterator>
#include <algorithm>
#include <functional>

#include <vector>
#include <unordered_map>

#include "utf8/utf8.h" // UTF8-CPP: http://utfcpp.sourceforge.net/

////////////////////////////////////////////////////////////////////////////////
// Вспомогательные функции

/// Применить отображение к каждому элементу контейнера на месте.
/// @param container контейнер, к элементам которого применяется отображение
/// @param map применяемое отображение (ассоциативный контейнер)
/// @return ссылка на container
template <class Container, class Map>
Container& apply_map(Container &container, const Map &map)
{
	for (auto &item : container)
	{
		auto it = map.find(item);
		if (it != map.end())
			item = it->second;
	}
	return container;
}

/// Получить новую строку как результат замены подстроки.
/// @param where исходная строка
/// @param pos позиция начала заменяемой подстроки
/// @param count длина заменяемой подстроки
/// @param on_what строка, на которую производится замена подстроки
/// @return объект новой строки -- результата замены
std::string replace
(
const std::string &where,
std::size_t pos,
std::size_t count,
const std::string &on_what
)
{
	std::string result(where);
	result.replace(pos, count, on_what);
	return result;
}

/// Прочитать всё содержимое файлового потока file в строку.
std::string file_to_string(std::ifstream &file)
{
	std::string result;
	if (file.is_open())
	{
		// Найти размер.
		file.seekg(0, std::ios::end);
		result.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		// Прочитать всё содержимое файла "скопом" в result.
		file.read(&result[0], result.size());
	}

	return result;
}


////////////////////////////////////////////////////////////////////////////////
// Определения, связанные с Юникодом
namespace Unicode
{
	using std::string;

	/// Код символа (номер в таблице Юникод).
	using Code = std::uint32_t;

	/// Отображение код -> код.
	using Code_map = std::unordered_map<Code, Code>;

	/// Размер поддерживаемой части таблицы.
	const Code CODEPOINT_BOUNDARY = 0xE01F0;

	/// Булевская таблица.
	using Bools = std::vector<bool>;

	/// Описание символа (кодовой позиции).
	struct Codepoint_entry
	{
		Code code;          ///< Собственно код (UCS-4 == UTF-32).
		char cc[3];         ///< Код категории (две буквы и завершающий нуль).
		string category[4]; ///< Полное описание категории (подкатегории).
		string name;        ///< Название символа в таблице Юникод.
	};

	/// Таблица описаний символов.
	using Codepoint_info = std::vector<Codepoint_entry>;

	/// Прочитать описание таблицы символов из потока.
	/// См. http://www.unicode.org/notes/tn36/
	/// Таблица: http://www.unicode.org/notes/tn36/Categories.txt
	/// @param categories поток ввода, читающий из Categories.txt
	Codepoint_info read_unicode_info(std::istream &categories)
	{
		Codepoint_info result;
		result.reserve(CODEPOINT_BOUNDARY);
		std::istringstream lp; // парсер строк таблицы.
		Codepoint_entry entry{};

		// Обработать файл построчно.
		for (string line; std::getline(categories, line);)
		{
			lp.str(line);

			// Прочитать код символа (номер кодовой позиции).
			lp >> std::hex >> entry.code;
			assert(lp.good());

			// Прочитать код категории.
			lp >> entry.cc[0] >> entry.cc[1];

			// Колонки таблицы разделены символами табуляции.
			assert(lp.peek() == '\t');
			lp.ignore();

			// Прочитать описание категории.
			for (auto &cat : entry.category)
				std::getline(lp, cat, '\t');

			// Всё, что осталось в строчке -- название символа.
			assert(lp.good() && lp.peek() != '\t');
			std::getline(lp, entry.name);
			lp.clear();

			result.push_back(entry);
		}

		return result;
	}

	/// Создать булевскую таблицу для проверки принадлежности символа заданной категории.
	/// @param info заранее прочитанное описание символов Юникод
	/// @param cc определяет выбор по коду категории, может содержать один символ (первый)
	Bools select_category(const Codepoint_info &info, const char *cc)
	{
		Bools result(CODEPOINT_BOUNDARY);
		for (auto &entry : info)
		{
			if (entry.code < CODEPOINT_BOUNDARY
				&& entry.cc[0] == cc[0]
				&& (cc[1] == '\0' || entry.cc[1] == cc[1]))
				result[entry.code] = true;
		}

		return result;
	}

	/// Построить отображение кодов заглавных букв в коды соответствующих маленьких букв.
	/// @param info заранее прочитанное описание символов Юникод
	/// @param letter булевская таблица принадлежности символа буквам (для ускорения поиска)
	Code_map upper_to_lower_mapping(const Codepoint_info &info, const Bools &letter)
	{
		// Отображение названий маленьких букв (из которых убрали слово SMALL и заменили его на CAPITAL) в их коды.
		// Засчёт подмены названия small является отображением названия заглавной буквы в
		// код соответствующей ей маленькой буквы.
		std::unordered_map<string, Code> small;
		// Найти все маленькие буквы.
		for (auto &entry : info)
		{
			if (entry.code < CODEPOINT_BOUNDARY && letter[entry.code])
			{
				const auto p = entry.name.find("SMALL");
				if (p != string::npos) // действительно маленькая буква.
					small.emplace(replace(entry.name, p, 5, "CAPITAL"), entry.code);
			}
		}

		// Найти все заглавные буквы и заполнить result.
		Code_map result;
		for (auto &entry : info)
		{
			if (entry.code < CODEPOINT_BOUNDARY && letter[entry.code])
			{
				const auto p = small.find(entry.name);
				if (p != small.end())
					result.emplace(entry.code, p->second);
			}
		}

		return result;
	}

	/// Текст в кодировке UTF-32.
	using Utf32 = std::vector<Code>;
}

namespace std
{
	template <>
	struct hash<Unicode::Utf32>
	{
		// Comining algorithm based upon 64-bit FNV-1a.
		size_t operator()(const Unicode::Utf32 &val) const
		{
			uint64_t h = 0xCBF29CE484222325ull;
			for (auto item : val)
				h = (h ^ item) * 0x100000001B3ull;
			return h;
		}
	};
}

namespace Unicode
{
	// Две функции, используеющие библиотеку UTF8-CPP.

	/// Транскодирование из UTF-8 (в виде string) в UTF-32 (в виде Utf32).
	Utf32 unchecked_utf8_to_utf32(const string &input)
	{
		assert(utf8::is_valid(begin(input), end(input)));
		Utf32 result;
		result.reserve(input.size());
		utf8::unchecked::utf8to32(begin(input), end(input), back_inserter(result));
		result.shrink_to_fit();
		return result;
	}

	/// Транскодирование из UTF-32 (в виде Utf32) в UTF-8 (в виде string).
	string unchecked_utf32_to_utf8(const Utf32 &input)
	{
		string result;
		result.reserve(input.size());
		utf8::unchecked::utf32to8(begin(input), end(input), back_inserter(result));
		result.shrink_to_fit();
		return result;
	}
}


////////////////////////////////////////////////////////////////////////////////
// Программа составления частотного словаря

/// Частотный словарь.
using Word_frequencies = std::unordered_map< Unicode::Utf32, std::uint64_t >;

/// Построить частотный словарь на основе заданного текста.
/// @param text исходный текст в кодировке UTF-32 уже приведённый к одному регистру (только маленькие или только заглавные)
/// @param letter булевская таблица для определения факта принадлежности кода буквам
Word_frequencies count_words
(
const Unicode::Utf32 &text,
const Unicode::Bools &letter
)
{
	using namespace Unicode;
	// Вспомогательные функции.
	const auto is_letter = [&](Code code)
	{ return code < CODEPOINT_BOUNDARY && letter[code]; };

	const auto next_letter = [&](Utf32::const_iterator from)
	{ return find_if(from, end(text), is_letter); };

	const auto next_non_letter = [&](Utf32::const_iterator from)
	{ return find_if_not(from, end(text), is_letter); };

	// Перебрать и посчитать все слова (последовательности букв разделённые небуквами).
	Word_frequencies result;
	for (auto from = next_letter(begin(text)); from != end(text);)
	{
		const auto to = next_non_letter(from);
		result[Utf32(from, to)]++;
		from = next_letter(to);
	}

	for (auto &kv : result)
		kv.second = 1. + log10(kv.second);

	return result;
}


double
map_multiply(const Word_frequencies &a, const Word_frequencies &b) 
{
	double sum = 0;
	Word_frequencies::const_iterator it_a = a.begin();

	for (; it_a != a.end(); ++it_a)
	{
		auto it_b = b.find(it_a->first);
		if (it_b != b.end())
			sum += it_a->second * it_b->second;
	}

	return sum;
}

double
map_cos(const Word_frequencies &a, const Word_frequencies &b) {
	return map_multiply(a, b) / (sqrt(map_multiply(a, a)) * sqrt(map_multiply(b, b)));
}



/// Точка входа.
/// Варианты вызова (командная строка):
/// 1. Без параметров: читает из input.txt, пишет в cout.
/// 2. Один параметр: имя файла вместо input.txt.
/// 3. Два параметра: два имени файла (исходный файл и куда сохранить результат).
int main(int argc, char *argv[])
{
	using namespace Unicode;
	using namespace std;

	// Попробуем открыть файл с исходным текстом.
	ifstream input;
	if (argc == 1) // файл по умолчанию.
		input.open("input.txt");
	else
		input.open(argv[1]);

	if (!input.is_open()) // открыть не удалось.
		return -1;

	// Читаем информацию о символах Юникод из файла Categories.txt.
	ifstream categories_desc("Categories.txt");
	const auto unicode_info = read_unicode_info(categories_desc);
	const auto letter = select_category(unicode_info, "L");
	const auto up2lo = upper_to_lower_mapping(unicode_info, letter);

	// Читаем исходный файл в строку и транскодируем в UTF-32.
	auto text = unchecked_utf8_to_utf32(file_to_string(input));
	// Приводим к нижнему регистру и считаем слова.
	const auto sf = count_words(apply_map(text, up2lo), letter);

	ifstream fdicts("fdicts.txt");
	string best_lang;
	double best_cos = -2;


	for (string lang_name, dict_fn; fdicts >> lang_name >> dict_fn;)
	{
		ifstream dict_file(dict_fn);
		Word_frequencies wf;
		string word;
		double freq;

		while (dict_file >> word >> freq)
			wf[unchecked_utf8_to_utf32(word)] = 1. + log10(freq);

		const auto cosf = map_cos(wf, sf);
		printf(" %1.3f |  %30s \n", cosf, lang_name.c_str());


		if (best_cos < cosf)
		{
			best_lang = lang_name;
			best_cos = cosf;
		}

	}

	cout << best_lang << endl;

/*
	// Сортировка слов по частотам.
	vector<pair<uint64_t, string>> fs;
	fs.reserve(sf.size());
	for (auto &kv : sf) // транскодируем слова обратно в UTF-8.
		fs.emplace_back(kv.second, unchecked_utf32_to_utf8(kv.first));

	std::cout << "#### PRINT ####" << std::endl;
	for (auto &e : fs)
		std::cout << e.first << " |  " << e.second << std::endl;

	sort(begin(fs), end(fs), greater<>{}); // собственно сортировка.

	// Вывод результата.
	ofstream output;
	if (argc > 2)
		output.open(argv[2]);

	ostream &out = output.is_open() ? output : cout;
	for (auto &kv : fs)
		out << kv.second << '\t' << kv.first << '\n';
*/
	return 0;
}
