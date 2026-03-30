#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include <locale>

using namespace std;

//ptin_iterator: чтение целых чисел из потока
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
        if (in_stream && !(*in_stream >> value)) {
            is_end = true;
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
    bool operator!=(const ptin_iterator& other) const { return !(*this == other); }
};

//ptout_iterator: вывод целых чисел в поток
class ptout_iterator {
    ostream* out_stream;
public:
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    ptout_iterator(ostream& s) : out_stream(&s) {}

    ptout_iterator& operator*() { return *this; }

    ptout_iterator& operator=(int value) {
        if (out_stream) *out_stream << value << " ";
        return *this;
    }

    ptout_iterator& operator++() { return *this; }
    ptout_iterator operator++(int) { return *this; }
};

// Проверка минимального размера контейнера
template<typename Container>
void requireSize(const Container& c, size_t minSize, const string& name) {
    if (c.size() < minSize) {
        cerr << "Ошибка: " << name << " должен содержать не менее " << minSize << " элементов!" << endl;
        exit(1);
    }
}

// Вывод контейнера через copy + ptout_iterator
template<typename Container>
void printContainer(const Container& c, const string& label) {
    cout << label;
    copy(c.begin(), c.end(), ptout_iterator(cout));
    cout << endl;
}

// Чтение контейнера из cin через конструктор диапазона
template<typename Container>
Container readContainer(const string& prompt, const string& name, size_t minSize) {
    cout << prompt << endl;
    ptin_iterator begin(cin), end;
    Container c(begin, end);  // Конструктор с итераторами — чтение сразу
    requireSize(c, minSize, name);
    return c;
}

void solveVariant11(vector<int>& V, list<int>& L) {
    // Позиция после 5-го элемента списка (индекс 4)
    auto insertPos = L.begin();
    advance(insertPos, 4);

    // Диапазон: первые 5 элементов вектора в обратном порядке
    auto firstRev = V.rbegin() + (V.size() - 5);
    auto lastRev = V.rend();

    // Один вызов insert() — как требует задание
    L.insert(next(insertPos), firstRev, lastRev);
}

int main() {

    setlocale(LC_ALL, "Russian");

    // Ввод данных
    auto V = readContainer<vector<int>>(
        "Введите элементы вектора V (не менее 5):", "Вектор", 5);

    cin.clear();
    cin.ignore(10000, '\n');

    auto L = readContainer<list<int>>(
        "\nВведите элементы списка L (не менее 5):", "Список", 5);

    // Вывод исходных данных
    printContainer(V, "\nВектор V: ");
    printContainer(L, "Список L: ");

    // Решение варианта
    solveVariant11(V, L);

    // Вывод результата
    printContainer(L, "\nРезультат (после 5-го элемента вставлены первые 5 элементов V в обратном порядке):\nСписок L: ");

    return 0;
}
