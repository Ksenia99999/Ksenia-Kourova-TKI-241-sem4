#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <locale>

using namespace std;

/**
 * @brief Пользовательский итератор ввода для чтения целых чисел из потока
 */
class ptin_iterator {
    istream* in_stream;
    int value;
    bool end_marker;

public:
    using iterator_category = input_iterator_tag;
    using value_type = int;
    using difference_type = ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;

    ptin_iterator() : in_stream(nullptr), value(0), end_marker(true) {}

    ptin_iterator(istream& s) : in_stream(&s), value(0), end_marker(false) {
        ++(*this);
    }

    reference operator*() const { return value; }
    pointer operator->() const { return &value; }

    ptin_iterator& operator++() {
        if (in_stream && !(*in_stream >> value)) {
            end_marker = true;
        }
        return *this;
    }

    ptin_iterator operator++(int) {
        ptin_iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const ptin_iterator& other) const {
        return (end_marker && other.end_marker) ||
            (end_marker == other.end_marker && in_stream == other.in_stream);
    }

    bool operator!=(const ptin_iterator& other) const {
        return !(*this == other);
    }
};

/**
 * @brief Пользовательский итератор вывода для записи целых чисел в поток
 */
class ptout_iterator {
    ostream* out_stream;
    string delimiter;

public:
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    ptout_iterator(ostream& s, const string& delim = " ")
        : out_stream(&s), delimiter(delim) {}

    ptout_iterator& operator*() { return *this; }

    ptout_iterator& operator=(int value) {
        if (out_stream) {
            *out_stream << value << delimiter;
        }
        return *this;
    }

    ptout_iterator& operator++() { return *this; }
    ptout_iterator& operator++(int) { return *this; }
};

int main() {
    setlocale(LC_ALL, "Russian");

    string filename;
    cout << "Введите имя файла с вектором V (например, vector.txt): ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << endl;
        return 1;
    }

    ptin_iterator it_begin(file);
    ptin_iterator it_end;

    vector<int> V;
    copy(it_begin, it_end, back_inserter(V));
    file.close();

    if (V.size() < 3) {
        cerr << "Ошибка: Вектор должен содержать не менее трех элементов" << endl;
        return 1;
    }

    cout << "\nИсходный вектор V: ";
    copy(V.begin(), V.end(), ptout_iterator(cout, " "));
    cout << endl;

    // Шаг 1: сортируем вектор
    sort(V.begin(), V.end());

    cout << "Отсортированный вектор: ";
    copy(V.begin(), V.end(), ptout_iterator(cout, " "));
    cout << endl;

    // Шаг 2: находим минимальный и максимальный элементы
    int min_val = V.front();
    int max_val = V.back();

    // Шаг 3: находим позиции первого вхождения min_val и последнего вхождения max_val
    auto first_min = lower_bound(V.begin(), V.end(), min_val);
    auto last_max = upper_bound(V.begin(), V.end(), max_val) - 1;

    // Шаг 4: выводим элементы между ними (исключая все вхождения min и max)
    // first_min указывает на первый min, last_max указывает на последний max
    int count_min = upper_bound(V.begin(), V.end(), min_val) - first_min;
    int count_max = last_max - lower_bound(V.begin(), V.end(), max_val) + 1;

    auto start = V.begin() + count_min;
    auto end = V.end() - count_max;

    cout << "\nРезультат (все элементы, кроме минимального " << min_val
        << " и максимального " << max_val << "): ";
    if (start < end) {
        copy(start, end, ptout_iterator(cout, " "));
    }
    else {
        cout << "(пусто)";
    }
    cout << endl;

    return 0;
}