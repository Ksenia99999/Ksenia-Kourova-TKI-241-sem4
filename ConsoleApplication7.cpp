#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include <limits>

using namespace std;

// Класс для чтения чисел из потока (например, с клавиатуры)
class ptin_iterator {
    istream* in_stream;   // Откуда читаем
    int value;            // Текущее число
    bool is_end;          // Флаг конца

public:
    // Типы, нужные для работы итератора
    using iterator_category = input_iterator_tag;
    using value_type = int;
    using difference_type = ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;

    // Конструктор конца итератора
    ptin_iterator() : in_stream(nullptr), value(0), is_end(true) {}

    // Конструктор для чтения из потока
    ptin_iterator(istream& s) : in_stream(&s), value(0), is_end(false) {
        ++(*this);  // Сразу читаем первое число
    }

    // Получить текущее число
    reference operator*() const { return value; }

    // Получить указатель на текущее число
    pointer operator->() const { return &value; }

    // Перейти к следующему числу
    ptin_iterator& operator++() {
        // Пробуем прочитать следующее число
        if (in_stream && (*in_stream >> value)) {
            // Прочитали успешно - ничего не делаем
        }
        else {
            // Читать больше нечего
            is_end = true;
            value = 0;
        }
        return *this;
    }

    // Постфиксный ++ 
    ptin_iterator operator++(int) {
        ptin_iterator temp = *this;
        ++(*this);
        return temp;
    }

    // Сравнение итераторов
    bool operator==(const ptin_iterator& other) const {
        return (is_end && other.is_end) ||
            (is_end == other.is_end && in_stream == other.in_stream);
    }

    bool operator!=(const ptin_iterator& other) const {
        return !(*this == other);
    }
};

// Класс для вывода элементов контейнера с разделителем
class ptout_iterator {
    ostream* out_stream;   // Куда выводим
    string separator;      // Разделитель

public:
    // Типы для итератора вывода
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    // Конструктор
    ptout_iterator(ostream& s, const string& sep = " ") : out_stream(&s), separator(sep) {}

    // Оператор * для итератора вывода
    ptout_iterator& operator*() { return *this; }

    // Оператор присваивания - здесь происходит вывод
    ptout_iterator& operator=(int value) {
        if (out_stream) {
            *out_stream << value << separator;
        }
        return *this;
    }

    // Операторы ++ (ничего не делают, но должны быть)
    ptout_iterator& operator++() { return *this; }
    ptout_iterator& operator++(int) { return *this; }
};

// Функция для вывода разделителя
void print_separator() {
    cout << "----------------------------------------" << endl;
}

// Функция для вывода контейнера
template<typename Container>
void print_container(const Container& cont, const string& name, ostream& out = cout) {
    out << name << ": ";
    copy(cont.begin(), cont.end(), ptout_iterator(out, " "));
    out << endl;
}

// Функция для вывода первых 5 элементов в обратном порядке
void print_first_five_reversed(const vector<int>& V) {
    cout << "Первые 5 элементов вектора V в обратном порядке: ";
    for (int i = 4; i >= 0; --i) {
        cout << V[i] << " ";
    }
    cout << endl;
}

int main() {

    print_separator();
    cout << "Задание: Вставить после 5-го элемента списка" << endl;
    cout << "первые 5 элементов вектора в обратном порядке" << endl;
    print_separator();
    cout << endl;

    // Ввод вектора V
    cout << "Введите элементы вектора V (целые числа, не менее 5):" << endl;
    cout << "Для окончания ввода введите любой символ (например, 'q'):" << endl;

    vector<int> V;
    ptin_iterator vec_begin(cin);
    ptin_iterator vec_end;
    copy(vec_begin, vec_end, back_inserter(V));

    // Проверяем, что вектор содержит не менее 5 элементов
    if (V.size() < 5) {
        cout << "\nОшибка: Вектор должен содержать не менее 5 элементов!" << endl;
        cout << "Введено " << V.size() << " элементов." << endl;
        return 1;
    }

    print_container(V, "Вектор V");

    // Очищаем поток ввода для следующего ввода
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Ввод списка L
    cout << "\nВведите элементы списка L (целые числа, не менее 5):" << endl;
    cout << "Для окончания ввода введите любой символ (например, 'q'):" << endl;

    list<int> L;
    ptin_iterator list_begin(cin);
    ptin_iterator list_end;
    copy(list_begin, list_end, back_inserter(L));

    // Проверяем, что список содержит не менее 5 элементов
    if (L.size() < 5) {
        cout << "\nОшибка: Список должен содержать не менее 5 элементов!" << endl;
        cout << "Введено " << L.size() << " элементов." << endl;
        return 1;
    }

    print_container(L, "Список L");

    // Выводим информацию о том, что будем делать
    cout << endl;
    print_separator();
    cout << "Выполнение задания:" << endl;
    cout << "Вставляем после 5-го элемента списка" << endl;
    cout << "первые 5 элементов вектора в обратном порядке" << endl;
    print_separator();

    // Показываем первые 5 элементов вектора
    cout << "Первые 5 элементов вектора V: ";
    for (int i = 0; i < 5; ++i) {
        cout << V[i] << " ";
    }
    cout << endl;

    print_first_five_reversed(V);

    // Находим 5-й элемент списка
    auto it = L.begin();
    advance(it, 4);  // Перемещаемся к 5-му элементу (индексация с 0)

    cout << "5-й элемент списка L: " << *it << endl;
    cout << "Вставляем после него..." << endl;

    // Один вызов insert для вставки первых 5 элементов вектора в обратном порядке
    // Используем обратные итераторы для вставки в обратном порядке
    L.insert(next(it), V.rbegin() + (V.size() - 5), V.rend());

    // Выводим результат
    cout << endl;
    print_separator();
    cout << "РЕЗУЛЬТАТ:" << endl;
    print_separator();
    print_container(L, "Список L после вставки");

    // Дополнительная информация для наглядности
    cout << endl;
    cout << "Пояснение:" << endl;
    cout << "- Исходный вектор V содержал " << V.size() << " элементов" << endl;
    cout << "- Исходный список L содержал " << (L.size() - 5) << " элементов" << endl;
    cout << "- После 5-го элемента списка (" << *it << ") вставлены элементы: ";
    for (int i = 4; i >= 0; --i) {
        cout << V[i];
        if (i > 0) cout << ", ";
    }
    cout << endl;
    cout << "- Использован один вызов функции-члена insert()" << endl;

    return 0;
}
