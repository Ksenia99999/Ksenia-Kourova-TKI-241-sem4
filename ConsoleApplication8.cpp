#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <locale>

using namespace std;

/**
 * @brief Пользовательский итератор ввода для чтения целых чисел из потока
 */
class ptin_iterator {
    istream* in_stream;
    int value;
    bool is_end;
public:
    using iterator_category = input_iterator_tag;
    using value_type = int;
    using difference_type = ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;

    ptin_iterator() : in_stream(nullptr), value(0), is_end(true) {}

    ptin_iterator(istream& s) : in_stream(&s), value(0), is_end(false) {
        ++(*this);
    }

    reference operator*() const { return value; }
    pointer operator->() const { return &value; }

    ptin_iterator& operator++() {
        if (in_stream && (*in_stream >> value)) {
        }
        else {
            is_end = true;
            value = 0;
        }
        return *this;
    }

    ptin_iterator operator++(int) {
        ptin_iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const ptin_iterator& other) const {
        return (is_end && other.is_end) ||
            (is_end == other.is_end && in_stream == other.in_stream);
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

// Функция для вывода вектора
template<typename Container>
void print_container(const Container& cont, const string& message) {
    cout << message;
    copy(cont.begin(), cont.end(), ptout_iterator(cout, " "));
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Введите целые числа (для окончания ввода введите любой нечисловой символ):" << endl;
    
    // Чтение вектора из потока ввода от начала до конца
    vector<int> V;
    ptin_iterator begin(cin);
    ptin_iterator end;
    copy(begin, end, back_inserter(V));

    if (V.size() < 3) {
        cerr << "Ошибка: Вектор должен содержать не менее трех элементов" << endl;
        return 1;
    }

    // Проверка, что в векторе есть хотя бы 3 различных числа
    vector<int> temp = V;
    sort(temp.begin(), temp.end());
    auto last = unique(temp.begin(), temp.end());
    temp.erase(last, temp.end());

    if (temp.size() < 3) {
        cerr << "Ошибка: Вектор должен содержать не менее трех различных чисел" << endl;
        return 1;
    }

    print_container(V, "\nИсходный вектор V: ");

    // Создаем копию вектора для сортировки (исходный вектор не изменяем)
    vector<int> sortedV = V;
    sort(sortedV.begin(), sortedV.end());

    print_container(sortedV, "Отсортированный вектор: ");

    // Находим минимальный и максимальный элементы
    int min_val = sortedV.front();
    int max_val = sortedV.back();

    cout << "Минимальный элемент: " << min_val << endl;
    cout << "Максимальный элемент: " << max_val << endl;

    // Подсчитываем количество вхождений min и max в отсортированной копии
    int count_min = upper_bound(sortedV.begin(), sortedV.end(), min_val) - 
                    lower_bound(sortedV.begin(), sortedV.end(), min_val);
    int count_max = upper_bound(sortedV.begin(), sortedV.end(), max_val) - 
                    lower_bound(sortedV.begin(), sortedV.end(), max_val);

    // Определяем диапазон для вывода из отсортированной копии
    auto start = sortedV.begin() + count_min;
    auto end_it = sortedV.end() - count_max;

    cout << "\nРезультат (все элементы, кроме минимального " << min_val
        << " и максимального " << max_val << "): ";

    if (start < end_it) {
        copy(start, end_it, ptout_iterator(cout, " "));
        cout << endl;
    }
    else {
        cout << "(пусто)" << endl;
    }

    return 0;
}
