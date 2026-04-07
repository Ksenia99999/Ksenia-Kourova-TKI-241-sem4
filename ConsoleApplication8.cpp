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

// Функция для проверки наличия хотя бы 3 различных чисел
bool check_three_distinct(const vector<int>& V) {
    vector<int> temp = V;
    sort(temp.begin(), temp.end());
    auto last = unique(temp.begin(), temp.end());
    temp.erase(last, temp.end());

    if (temp.size() < 3) {
        cerr << "Ошибка: Вектор должен содержать не менее трех различных чисел" << endl;
        return false;
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Введите целые числа (для окончания ввода введите любой нечисловой символ):" << endl;

    // Чтение вектора из потока ввода 
    ptin_iterator begin_iter(cin);
    ptin_iterator end_iter;
    vector<int> V(begin_iter, end_iter);

    if (V.size() < 3) {
        cerr << "Ошибка: Вектор должен содержать не менее трех элементов" << endl;
        return 1;
    }

    // Проверка, что в векторе есть хотя бы 3 различных числа
    if (!check_three_distinct(V)) {
        return 1;
    }

    print_container(V, "\nИсходный вектор V: ");

    // Создаем копию вектора для сортировки
    vector<int> sortedV = V;
    sort(sortedV.begin(), sortedV.end());

    print_container(sortedV, "Отсортированный вектор: ");

    // Находим минимальный и максимальный элементы
    int min_val = *min_element(sortedV.begin(), sortedV.end());
    int max_val = *max_element(sortedV.begin(), sortedV.end());

    cout << "Минимальный элемент: " << min_val << endl;
    cout << "Максимальный элемент: " << max_val << endl;

    // Выводим все элементы, кроме минимального и максимального
    cout << "\nРезультат (все элементы, кроме минимального " << min_val
        << " и максимального " << max_val << "): ";

    // Используем copy_if для копирования элементов
    copy_if(sortedV.begin(), sortedV.end(), ptout_iterator(cout, " "),
        [min_val, max_val](int x) {
            return x != min_val && x != max_val;
        });

    cout << endl;

    return 0;
}
