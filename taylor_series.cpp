#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>

//Подключение нужных библиотек
using namespace std; //Подключение std

//Функция для вывода красной строки без выхода из программы
void outputRedStr(const string& str) {
    string redStr = "\x1B[31m" + str + "\033[0m";
    cout << redStr << endl;
}

//Вывод красивых ошибок и завершение программы
void outputError(const string& err) {
    string redError = "\x1B[31m" + err + "\033[0m";
    cerr << redError << endl;
    exit(1);
}

//функия для ввода данных
double inputData(string text, string type) {
    cout << text; //Вывод текста
    double value = 0.0; //Инициализация переменной, которая будет вводиться с консоли
    cin >> value; //Считывание переменной

    //Проверка на ошибки ввода, для каждого вида ошибки свои сообщения
    if (cin.fail()) { //Ошибка ввода или ввод букв
        outputRedStr("Вы ввели некорректное значение, пожалуйста, измените свое решение в лучшую сторону."); //Вывод красной строки с сообщением об ошибке
        cout << "Не вводите буквы пожалуйста\n"; //Вывод дополнительной строки для лучшего понимания программы пользователем
        cin.clear(); //очистка потока ввода
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //игнор рекурсии при вводе буков в консоль

        return inputData(text, type); //Повтор ввода (еще один шанс для пользователя)
    } 

    if (type == "double" && (value <= -1 || value >= 1)) { //Если указаный тип данных - double, нужно ввести число от -1 до 1 не включительно
        outputRedStr("Вы ввели некорректное значение, пожалуйста, измените свое решение в лучшую сторону.");
        cout << "Давайте еще раз.\n";

        return inputData(text, type);
    }

    if (type == "int" && value != (int)value || (value > INT_MAX && value < INT_MIN)) { //Если указаный тип данных - int, нужно ввести целое число в пределах INT_MIN и INT_MAX
        outputRedStr("Вы ввели некорректное значение, пожалуйста, измените свое решение в лучшую сторону.");
        cout << "Введите целочисленное значение в пределах (INT_MIN, INT_MAX)\n";

        return inputData(text, type);
    }
    return value;
}

//Функция, вычисляющая ряд тейлора
double calculateTaylor(double x, double absError, int numberMax) {
    int count = 1,  //Номер текущего члена ряда
        denominator = 1; //Знаменатель дроби для членов ряда

    double sumSeries = 1.0, //Сумма ряда (с учетом первого слагаемого - 1)
        numerator = -x * x, //Числитель дроби для членов ряда
        currentMember = 1.0; //Текущий член ряда

    while (abs(currentMember) > absError && count <= numberMax) {
        count++;
        currentMember = numerator / denominator;

        numerator *= (-x * x);
        denominator *= count; 
        sumSeries += currentMember;
    }

    if (abs(currentMember) > absError && count > numberMax) // если не достигнута точность и превышено число слагаемых
        outputError("Не удалось достичь заданной точности за максимальное число слагаемых");
   
    if (x < -1 || x > 1) // если x не попадает в заданный интервал
        outputError("x не попадает в заданный интервал (-1, 1)");
 
    return sumSeries;
}


double cTaylor(double x) {
    double ans = exp(-x * x);
    return ans;
}


int main() {
    setlocale(LC_ALL, "ru");
    int numberMax;
    double start = 0.0,
           end = 0.0,
           step = 0.0,
           absError = 0.0;

    //Ввод переменных с помощью функции inputData
    numberMax = inputData("Введите максимальное число слагаемых (больше 0): ", "int");
    if (numberMax <= 0) //Если максимальное число слагаемых меньше или равно 0, вывод ошибки
        outputError("Вы ввели некорректное значение, максимальное число слагаемых должно быть больше 0, завершение программы");

    start = inputData("Введите начало интервала (-1, 1): ", "double");
    end = inputData("Введите конец интервала (-1, 1): ", "double");
    if (start > end) //После ввода интервала, если start > end, завершаем программу с ошибкой
        outputError("Вы ввели некорректное значение, начало интервала должно быть больше, чем конец интервала, завершение программы");

    step = inputData("Введите шаг (больше 1e-12): ", "double"); //Шаг должен быть положительным и больше, чем 1e-12, чтобы программа могла его обработать
    if (step < 1e-12)
        outputError("Вы ввели некорректное значение, шаг должен быть больше, чем 1e-12, завершение программы");

    absError = inputData("Введите погрешность: ", "double");
    
    //Красивый вывод таблицы
    int prec = int(ceil(log10(1 / absError))); //Количество выходных нулей
    
    cout << "|===============|===============|===============|\n"
            "|       X       |Taylor e^(-x*x)|    C++ exp    |\n"
            "|===============|===============|===============|\n";
    
    for (double x = start; x <= end; x += step) {
        
        stringstream stdBuf, ourBuf;

        if (abs(x) < 1e-12) //Когда x приближается к нулю, в таблице будет отображаться нулевое значение
            x = 0;

        ourBuf << fixed;
        ourBuf << setprecision(12) << setw(15)
            << calculateTaylor(x, absError, numberMax);
        string ourNum = "\x1B[32m" + ourBuf.str().substr(0, prec + 3) +
            "\033[0m" + ourBuf.str().substr(prec + 3);

        stdBuf << fixed;
        stdBuf << setprecision(12) << setw(15)
               << cTaylor(x);
        string stdNum = "\x1B[32m" + stdBuf.str().substr(0, prec + 3) +
            "\033[0m" + stdBuf.str().substr(prec + 3);

        cout << "|";
        cout << setw(15) << x << "|";
        cout << ourNum << "|" << stdNum << "|" << endl;
        cout << "|===============|===============|===============|\n";
    }
    return 0;
}