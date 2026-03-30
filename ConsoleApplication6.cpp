#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
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

// Итератор для вывода чисел в поток
class ptout_iterator {
    ostream* out_stream;
    string delimiter;

public:
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    ptout_iterator(ostream& s, const string& delim = "  ") 
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
void print_vector(const vector<int>& v, const string& message) {
    cout << message;
    copy(v.begin(), v.end(), ptout_iterator(cout, "  "));
    cout << endl;
}

int main() {
    // Ввод имени файла
    cout << "Введите имя файла: ";
    string name;
    cin >> name;

    // Ввод чисел
    cout << "Введите целые числа (для окончания введите любой нечисловой символ): " << endl;

    vector<int> numbers;
    ptin_iterator begin(cin);
    ptin_iterator end;
    
    // Чтение потока ввода от начала до конца сразу в вектор
    copy(begin, end, back_inserter(numbers));

    // Проверка на пустоту
    if (numbers.empty()) {
        cout << "Вы не ввели ни одного числа!" << endl;
        return 1;
    }

    // Вывод введённых чисел
    print_vector(numbers, "\nВведено " + to_string(numbers.size()) + " чисел: ");

    // Открытие файла
    ofstream file(name);
    if (!file.is_open()) {
        cout << "Не удалось создать файл " << name << endl;
        return 1;
    }

    // Запись в файл с заменой 0 на 10
    ostream_iterator<int> out(file, "  ");
    replace_copy(numbers.begin(), numbers.end(), out, 0, 10);

    file.close();

    // Вывод результата
    vector<int> result(numbers.size());
    replace_copy(numbers.begin(), numbers.end(), result.begin(), 0, 10);
    print_vector(result, "\nРезультат (с заменой 0 на 10): ");

    cout << "\nЧисла записаны в файл " << name << endl;

    return 0;
}
