#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <locale>
#include <functional>

using namespace std;

/**
 * @brief Структура point с целочисленными членами x, y и строковым членом s
 */
struct point {
    int x;
    int y;
    string s;

    /**
     * @brief Конструктор по умолчанию
     */
    point() : x(0), y(0), s("") {}

    /**
     * @brief Конструктор с параметрами
     */
    point(int x_val, int y_val, const string& s_val) : x(x_val), y(y_val), s(s_val) {}

    /**
     * @brief Оператор сравнения "меньше" для точки
     * Точка A меньше точки B, если A.x < B.x или (A.x == B.x и A.y < B.y)
     */
    bool operator<(const point& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

/**
 * @brief Операция чтения point из потока
 * @param is Входной поток
 * @param p Объект point для заполнения
 * @return Ссылка на входной поток
 */
istream& operator>>(istream& is, point& p) {
    is >> p.x >> p.y >> p.s;
    return is;
}

/**
 * @brief Операция записи point в поток
 * @param os Выходной поток
 * @param p Объект point для вывода
 * @return Ссылка на выходной поток
 */
ostream& operator<<(ostream& os, const point& p) {
    os << p.x << " " << p.y << " " << p.s;
    return os;
}

/**
 * @brief Пользовательский итератор ввода для чтения объектов point из потока
 */
class ptin_iterator {
    std::istream* in_stream;
    point value;
    bool end_marker;

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = point;
    using difference_type = std::ptrdiff_t;
    using pointer = const point*;
    using reference = const point&;

    ptin_iterator() : in_stream(nullptr), end_marker(true) {}

    ptin_iterator(std::istream& s) : in_stream(&s), end_marker(false) {
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
 * @brief Пользовательский итератор вывода для записи объектов point в поток
 */
class ptout_iterator {
    std::ostream* out_stream;

public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    ptout_iterator(std::ostream& s) : out_stream(&s) {}

    ptout_iterator& operator*() { return *this; }

    ptout_iterator& operator=(const point& value) {
        if (out_stream) {
            *out_stream << value << endl;
        }
        return *this;
    }

    ptout_iterator& operator++() { return *this; }
    ptout_iterator& operator++(int) { return *this; }
};

/**
 * @brief Функция для вывода содержимого вектора точек
 */
template<typename Container>
void printVector(const Container& v, const string& message) {
    cout << message << endl;
    copy(v.begin(), v.end(), ptout_iterator(cout));
}

/**
 * @brief Функция для чтения вектора точек из файла
 */
template<typename Container>
bool readPointsFromFile(const string& filename, Container& cont) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << endl;
        return false;
    }

    ptin_iterator begin(file);
    ptin_iterator end;
    copy(begin, end, back_inserter(cont));
    file.close();

    return true;
}

/**
 * @brief Функция для записи вектора точек в файл
 */
template<typename Container>
bool writePointsToFile(const string& filename, const Container& cont) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << " для записи" << endl;
        return false;
    }

    copy(cont.begin(), cont.end(), ptout_iterator(file));
    file.close();

    return true;
}

/**
 * @brief Главная функция программы
 * Читает точки из файла, перегруппировывает элементы,
 * перемещая в начало все элементы, которые меньше точки (0, 0)
 */
int main() {
    setlocale(LC_ALL, "Russian");

    string name;
    cout << "Введите имя файла: ";
    cin >> name;

    // Чтение вектора из файла
    vector<point> V;
    if (!readPointsFromFile(name, V)) {
        return 1;
    }

    if (V.empty()) {
        cerr << "Ошибка: Файл пуст" << endl;
        return 1;
    }

    // Вывод исходного вектора
    printVector(V, "Исходный вектор точек:");

    // Создаем точку (0, 0) для сравнения
    point zero_point(0, 0, "");

    // Используем stable_partition для перегруппировки элементов
    // Перемещаем в начало все элементы, которые меньше точки (0, 0)
    auto partition_point = stable_partition(V.begin(), V.end(),
        bind(less<point>(), placeholders::_1, zero_point));

    // Вывод преобразованного вектора
    cout << "\nПреобразованный вектор точек (элементы меньше (0,0) в начале):" << endl;
    printVector(V, "");

    // Записываем обратно в файл
    if (!writePointsToFile(name, V)) {
        return 1;
    }

    cout << "\nРезультат записан в файл " << name << endl;

    return 0;
}
