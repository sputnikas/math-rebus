///////////////////////////////////////////////////////////////////////////////////////////////////
// Данная программа решает числовые ребусы вида:
// muha + muha = slon
// udar + udar = draka
// voda + spirt = vodka
// Решение происходит методом перебора вариантов
// На входе программа принимает строку, содержащую одну из данных фраз, в которой есть "+" и "="
// Решения выводит в консоль или говорит, что решений нет
// Для больших чисел - 9 цифр в перестановке и больше, когда алгоритм неэффективен, показывает, что
// программа работает, выводя фразу, содержащую текущее значение перестановки:
//    "Permutation value >= %lld"  value
// При 10 неизвестных цифрах применение программы займёт много времени... Пример:
// love + friends = divines
// 
// Компиляция
// g++ -std=c++11 .\math_rebus.cpp -o .\math_rebus.exe
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <regex>

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция возводит 10 в степень n
///////////////////////////////////////////////////////////////////////////////////////////////////
long long pow10(long long n) {
    long long r = 1;
    while (n > 0) {
        n--;
        r *= 10;
    }
    return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция возвращает n единиц - число вида 111..11
///////////////////////////////////////////////////////////////////////////////////////////////////
long long ones(long long n) {
    long long r = 0;
    while (n > 0) {
        n--;
        r = 10*r + 1;
    }
    return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция делает массив цифр из числа (цифры могут повторяться)
///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<int> gen_ints(long long word) {
    std::vector<int> r;
    while (word > 0) {
        r.insert(r.begin(), word % 10);
        word /= 10;
    }
    return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция делает массив букв из строки (буквы не повторяются)
///////////////////////////////////////////////////////////////////////////////////////////////////
void gen_chars(std::vector<char> *chars, std::string word) {
    for (int j = 0; j<word.length(); j++) {
        char c = word[j];
        //std::cout << c << std::endl;
        bool b = true;
        for (int k = 0; k<chars->size(); k++) {
            if (chars->at(k) == c) {
                b = false;
            }
        }
        if (b) {
            chars->push_back(c);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция ищет индекс символа в массиве символов
///////////////////////////////////////////////////////////////////////////////////////////////////
int index_of_char(std::vector<char> chars, char c) {
    for (int i = 0; i<chars.size(); i++) {
        if (chars[i] == c) {
            return i;
        } 
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция ищет индекс цифры в массиве цифр
///////////////////////////////////////////////////////////////////////////////////////////////////
int index_of_int(std::vector<int> ints, int c) {
    for (int i = 0; i<ints.size(); i++) {
        if (ints[i] == c) {
            return i;
        } 
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция проверяет, что все цифры числа различны
///////////////////////////////////////////////////////////////////////////////////////////////////
bool check_all_digits_different(long long n) {
    int a[100];
    int ndigits = 0;
    while (n > 0) {
        int s = n % 10;
        for (int i = 0; i<ndigits; i++) {
            if (s == a[i]) {
                return false;
            }
        }
        a[ndigits] = s;
        ndigits++;
        n /= 10;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция ищет следующее число со всеми различными цифрами
///////////////////////////////////////////////////////////////////////////////////////////////////
long long next_digits_different(long long value) {
    value++;
    if (value > 9876543210) {
        return -1;
    }
    while (!check_all_digits_different(value)) {
        value++;
    }
    return value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция генерует число, ставя соответствие буквам lex числа из ints в соответствии с chars
///////////////////////////////////////////////////////////////////////////////////////////////////
long long gen_int(std::string lex, std::vector<char> chars, std::vector<int> ints) {
    long long r = 0;
    for (int i = 0; i<lex.length(); i++) {
        int ind = index_of_char(chars, lex[i]);
        if (ind != -1 && ind < ints.size()) {
            r = 10*r + ints[ind];
        } else {
            return -1;
        }
    }
    return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Функция проверяет, что число word соответствует слову lex при chars, соответствующих ints
///////////////////////////////////////////////////////////////////////////////////////////////////
bool check_int(int word, std::string lex, std::vector<char> chars, std::vector<int> ints) {
    std::vector<char> chars2;
    gen_chars(&chars2, lex);
    //std::cout << 9*ones(lex.length()) << " " << pow10(lex.length() - 1) << " " << word << std::endl;
    if (word > 9*ones(lex.length()) || (word < pow10(lex.length() - 1))) {
        return false;
    };
    std::vector<int> ints2 = gen_ints(word);
    std::vector<int> ints3(chars2.size(), -1);
    // На первом этапе создаём массив целых, соответствующих chars2
    // и проверяем что разным цифрам соответствуют разные буквы
    for (int i = 0; i<lex.length(); i++) {
        int ind = index_of_char(chars2, lex[i]);
        int digit = ints2[i];
        if (ints3[ind] != -1) {
            if (digit != ints3[ind]) {
                return false;
            }
        } else {
            if (index_of_int(ints3, digit) != -1) {
                return false;
            }
            ints3[ind] = digit;
        }
    }
    for (int i = 0; i<chars2.size(); i++) {
        // Проверяем, что буквы в слове, которые есть в chars совпадают с цифрами в результате
        int ind = index_of_char(chars, chars2[i]);
        if (ind != -1) {
            if (ints3[i] != ints[ind]) {
                return false;
            }
        } else {
            // Проверяем, что буквы в слове, которых нет в chars соответствуют цифрам, которых нет в ints
            if (index_of_int(ints, ints3[i]) != -1) {
                return false;
            }
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Основная функция программы
// Алгоритм:
// 1. Инициализируем переменные:
// 1.1. Строку берём из stdin
// 1.2. Удаляем из неё пробелы
// 1.3. Извлекаем первое и второе слово и результат в массив lex
// 1.4. Создаём массив chars из букв первого и второго слов
// 1.5. По количеству букв определяем минимальное и максимальное значение для перестановки value
// 2. Основной цикл:
// 2.1. Создаём на основе перестановки value массив цифр ints, соответствующий chars
// 2.2. Генерируем числа по словам lex[0], lex[1] и находим сумму sum
// 2.3. Проверяем, что сумма соответствует lex[2] и не противоречит chars и ints
// 2.4. Если это так - выводим решение в консоль и увеличиваем счётчик решений
// 3. Если счётчик решений нулевой - выводим, что решений нет
///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    std::string str;//  = "muha + muha = slon"; // Строка - может содержать буквы, пробелы, обязательно + и =
    std::cout << "Input rebus in form: muha + muha = slon" << std::endl;
    std::getline(std::cin, str);
    std::regex re("\\s+");
    std::string str2 = regex_replace(str, re, "");
    std::string lex[3];
    int pos_plus = str2.find("+");
    int pos_equal = str2.find("=");
    if (pos_plus == std::string::npos || pos_equal == std::string::npos || pos_plus >= pos_equal - 1) {
        std::cout << "Check string for plus and equal signs: " << str << std::endl;
        return 1; 
    }
    lex[0] = str2.substr(0, pos_plus);
    lex[1] = str2.substr(pos_plus + 1, pos_equal - pos_plus - 1);
    lex[2] = str2.substr(pos_equal + 1, str2.length() - pos_equal - 1);
    std::vector<char> chars;
    for (int i = 0; i<2; i++) {
        gen_chars(&chars, lex[i]);
    }
    //for (int k = 0; k<chars.size(); k++) {
    //    std::cout << chars[k] << " ";
    //}
    if (chars.size() > 10) {
        std::cout << "Mistake! Signs more than 10. Check expression!\n";
        return 0;
    } else {
        long long min_lex_value = pow10(chars.size() - 1);
        long long max_lex_value = 9*ones(chars.size());
        std::cout << "Minimal value: " << min_lex_value << std::endl;
        std::cout << "Maximal value: " << max_lex_value << std::endl;
        long long step = min_lex_value;
        long long value = min_lex_value;
        int solves = 0;
        while (value < max_lex_value && value != -1) {
            if (value > step && value > 100000000) {
                std::cout << "Permutation value >= " << value << std::endl;
                step += min_lex_value;
            }
            std::vector<int> ints = gen_ints(value);
            //for (int k = 0; k<chars.size(); k++) {
            //    std::cout << ints[k] << " ";
            //}
            long long a1 = gen_int(lex[0], chars, ints);
            long long a2 = gen_int(lex[1], chars, ints);
            if (a2 >= pow10(lex[1].length() - 1)) {
                long long sum = a1 + a2;
                bool ch = check_int(sum, lex[2], chars, ints);
                // std::cout << a1 << " + " << a2 << " = " << sum  << "; check = " << ch << std::endl;
                if (ch) {
                    std::cout << a1 << " + " << a2 << " = " << sum << std::endl;
                    solves++;
                    //getchar();
                }
            }
            value = next_digits_different(value);
        }
        if (solves == 0) {
            std::cout << "No solutions!" << std::endl;
        }
    }
    return 0;
}