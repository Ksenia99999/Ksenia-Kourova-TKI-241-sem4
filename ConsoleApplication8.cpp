#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <locale>

using namespace std;

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


    int n;
    cout << "Введите количество элементов вектора V: ";
    cin >> n;

    if (n < 3) {
        cerr << "Ошибка: Вектор должен содержать не менее трех элементов" << endl;
        return 1;
    }

    vector<int> V;
    cout << "Введите " << n << " целых чисел (элементы могут повторяться):" << endl;
    for (int i = 0; i < n; ++i) {
        int value;
        cin >> value;
        V.push_back(value);
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

    cout << "Минимальный элемент: " << min_val << endl;
    cout << "Максимальный элемент: " << max_val << endl;

    // Шаг 3: находим позиции первого вхождения min_val и последнего вхождения max_val
    auto first_min = lower_bound(V.begin(), V.end(), min_val);
    auto last_max = upper_bound(V.begin(), V.end(), max_val) - 1;

    // Шаг 4: подсчитываем количество вхождений min и max
    int count_min = upper_bound(V.begin(), V.end(), min_val) - first_min;
    int count_max = last_max - lower_bound(V.begin(), V.end(), max_val) + 1;

    // Шаг 5: определяем диапазон для вывода (исключая все min и max)
    auto start = V.begin() + count_min;
    auto end = V.end() - count_max;

    cout << "\nРезультат (все элементы, кроме минимального " << min_val
        << " и максимального " << max_val << "): ";

    if (start < end) {
        copy(start, end, ptout_iterator(cout, " "));
        cout << endl;

        // Дополнительный вывод: показываем, сколько элементов осталось
        cout << "\nКоличество выведенных элементов: " << (end - start) << endl;
    }
    else {
        cout << "(пусто)";
        cout << "\nПосле удаления всех минимальных и максимальных элементов не осталось элементов" << endl;
    }

    cout << endl;


    return 0;
}
