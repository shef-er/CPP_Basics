#include <locale>  // locale, ctype, ctype_base
#include <iostream>
#include <fstream>
#include <string>
#include <climits> // CHAR_BIT
#include <map>
#include <stdint.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

ctype_base::mask*
my_ctype_table(const locale &base_loc)
{
	static const auto table_size = 1u << CHAR_BIT;
	auto table = new ctype_base::mask[table_size];
	for (unsigned i = 0; i < table_size; ++i)
		table[i] = isalnum((char)i, base_loc) ?
		ctype_base::alpha
		: ctype_base::space;

	table['-'] = ctype_base::alpha; // "дефис" у нас тоже буква.
	return table;
}

void
count_words(istream &source, map<string, uint64_t> &counters)
{
	// Считаем, что source содержит слова, разделённые пробелами.
	for (string word; source >> word;)
		counters[word]++;
}

// Вывести в поток произвольный map.
template <class K, class V>
ostream& print(ostream &os, const map<K, V> &m)
{
	for (auto &kv : m)
		os << kv.first << ' ' << kv.second << '\n';
	return os;
}

int
main()
{
	locale base_loc("Russian_Russia.866");
	//locale base_loc("ru_RU.utf8");
	cout.imbue(base_loc); // Задать локаль потоку вывода.
	cout << "----------------" << endl;
	cout << "Base locale name:    " << base_loc.name() << endl;

	// Создать новую локаль, подставив фасет ctype с новой таблицей символов.
	locale my_loc(base_loc, new ctype<char>(my_ctype_table(base_loc), true));
	cout << "Changed locale name: " << my_loc.name() << endl;
	cout << "----------------" << endl;

	cin.imbue(my_loc); // Задать локаль потоку ввода.
	cout << "The text:\n";

	// ---- //

	map<string, uint64_t> counters;
	vector<pair<uint64_t, string>> vp;

	ifstream fin("input.txt");

	for (string word; fin >> word;)
	{
		cout << word << ' ';
		use_facet<ctype<char>>(base_loc).tolower(&word[0], &word[0] + word.size());
		counters[word]++;
	}
	cout << endl << endl;

	cout << "---- before sort:" << endl;
	print(cout, counters);
	cout << endl;

	vp.reserve(counters.size());
	for (auto &kv : counters)
		vp.emplace_back(kv.second, kv.first);

	sort(begin(vp), end(vp));

	cout << "---- after sort:" << endl;
	for (auto &p : vp)
		cout << p.second << ' ' << p.first << endl;

	return 0;
}

