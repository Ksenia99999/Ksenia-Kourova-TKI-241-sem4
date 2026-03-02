#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

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

    // Постфиксный ++ (нужен для интерфейса)
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

// Класс для записи чисел с двумя пробелами после каждого
class ptout_iterator {
    ostream* out_stream;   // Куда записываем
    string separator;      // Разделитель (два пробела)

public:
    // Типы для итератора вывода
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    // Конструктор
    ptout_iterator(ostream& s, const string& sep) : out_stream(&s), separator(sep) {}

    // Оператор * для итератора вывода
    ptout_iterator& operator*() { return *this; }

    // Оператор присваивания - здесь происходит запись
    ptout_iterator& operator=(int value) {
        if (out_stream) {
            *out_stream << value << separator;  // Число + два пробела
        }
        return *this;
    }

    // Операторы ++ (ничего не делают, но должны быть)
    ptout_iterator& operator++() { return *this; }
    ptout_iterator& operator++(int) { return *this; }
};

int main() {
    // Просим пользователя ввести имя файла
    cout << "Введите имя файла: ";
    string name;
    cin >> name;

    // Читаем числа
    cout << "Введите целые числа (для окончания введите любой символ, например 'q'):" << endl;

    vector<int> numbers;

    // Создаем итераторы для чтения
    ptin_iterator begin(cin);   // Начало - читаем с клавиатуры
    ptin_iterator end;           // Конец

    // Копируем все введенные числа в вектор
    copy(begin, end, back_inserter(numbers));

    // Проверяем, что числа введены
    if (numbers.empty()) {
        cout << "Вы не ввели ни одного числа!" << endl;
        return 1;
    }

    // Показываем, что ввели
    cout << "\nВведено " << numbers.size() << " чисел: ";
    for (int x : numbers) {
        cout << x << " ";
    }
    cout << endl;

    // Открываем файл для записи
    ofstream file(name);
    if (!file.is_open()) {
        cout << "Не удалось создать файл " << name << endl;
        return 1;
    }

    // Создаем итератор для записи с двумя пробелами
    ptout_iterator out(file, "  ");

    // Копируем числа с заменой 0 на 10
    replace_copy(numbers.begin(), numbers.end(), out, 0, 10);

    // Закрываем файл
    file.close();

    // Показываем результат
    cout << "\nРезультат (с заменой 0 на 10): ";
    for (int x : numbers) {
        if (x == 0) cout << "10 ";
        else cout << x << " ";
    }
    cout << endl;

    cout << "\nЧисла записаны в файл " << name << endl;
    cout << "После каждого числа добавлено два пробела." << endl;

    return 0;
}