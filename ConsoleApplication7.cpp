#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
using namespace std;

// Итератор для чтения чисел из потока 
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

int main() {
    setlocale(LC_ALL, "Russian");

    // Ввод вектора V
    cout << "Введите элементы вектора V (не менее 5):" << endl;
    vector<int> V;
    ptin_iterator vec_begin(cin);
    ptin_iterator vec_end;

    // Чтение потока ввода от начала до конца сразу в вектор
    while (vec_begin != vec_end) {
        V.push_back(*vec_begin);
        ++vec_begin;
    }

    if (V.size() < 5) {
        cout << "Ошибка: Вектор должен содержать не менее 5 элементов!" << endl;
        return 1;
    }

    // Ввод списка L
    cin.clear();
    cin.ignore(10000, '\n');

    cout << "\nВведите элементы списка L (не менее 5):" << endl;
    list<int> L;
    ptin_iterator list_begin(cin);
    ptin_iterator list_end;

    // Чтение потока ввода от начала до конца сразу в список
    while (list_begin != list_end) {
        L.push_back(*list_begin);
        ++list_begin;
    }

    if (L.size() < 5) {
        cout << "Ошибка: Список должен содержать не менее 5 элементов!" << endl;
        return 1;
    }

    // Вывод исходных данных
    cout << "\nВектор V: ";
    for (int x : V) cout << x << " ";
    cout << endl;

    cout << "Список L: ";
    for (int x : L) cout << x << " ";
    cout << endl;

    // Находим 5-й элемент списка (индекс 4)
    auto it = L.begin();
    advance(it, 4);

    // Вставка после 5-го элемента: первые 5 элементов вектора в обратном порядке
    // Один вызов insert() 
    L.insert(next(it), V.rbegin() + (V.size() - 5), V.rend());

    // Вывод результата
    cout << "\nРезультат (после 5-го элемента вставлены первые 5 элементов V в обратном порядке):" << endl;
    cout << "Список L: ";
    for (int x : L) cout << x << " ";
    cout << endl;

    return 0;
}
