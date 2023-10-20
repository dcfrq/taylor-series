#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>

//����������� ������ ���������
using namespace std; //����������� std

//������� ��� ������ ������� ������ ��� ������ �� ���������
void outputRedStr(const string& str) {
    string redStr = "\x1B[31m" + str + "\033[0m";
    cout << redStr << endl;
}

//����� �������� ������ � ���������� ���������
void outputError(const string& err) {
    string redError = "\x1B[31m" + err + "\033[0m";
    cerr << redError << endl;
    exit(1);
}

//������ ��� ����� ������
double inputData(string text, string type) {
    cout << text; //����� ������
    double value = 0.0; //������������� ����������, ������� ����� ��������� � �������
    cin >> value; //���������� ����������

    //�������� �� ������ �����, ��� ������� ���� ������ ���� ���������
    if (cin.fail()) { //������ ����� ��� ���� ����
        outputRedStr("�� ����� ������������ ��������, ����������, �������� ���� ������� � ������ �������."); //����� ������� ������ � ���������� �� ������
        cout << "�� ������� ����� ����������\n"; //����� �������������� ������ ��� ������� ��������� ��������� �������������
        cin.clear(); //������� ������ �����
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //����� �������� ��� ����� ����� � �������

        return inputData(text, type); //������ ����� (��� ���� ���� ��� ������������)
    } 

    if (type == "double" && (value <= -1 || value >= 1)) { //���� �������� ��� ������ - double, ����� ������ ����� �� -1 �� 1 �� ������������
        outputRedStr("�� ����� ������������ ��������, ����������, �������� ���� ������� � ������ �������.");
        cout << "������� ��� ���.\n";

        return inputData(text, type);
    }

    if (type == "int" && value != (int)value || (value > INT_MAX && value < INT_MIN)) { //���� �������� ��� ������ - int, ����� ������ ����� ����� � �������� INT_MIN � INT_MAX
        outputRedStr("�� ����� ������������ ��������, ����������, �������� ���� ������� � ������ �������.");
        cout << "������� ������������� �������� � �������� (INT_MIN, INT_MAX)\n";

        return inputData(text, type);
    }
    return value;
}

//�������, ����������� ��� �������
double calculateTaylor(double x, double absError, int numberMax) {
    int count = 1,  //����� �������� ����� ����
        denominator = 1; //����������� ����� ��� ������ ����

    double sumSeries = 1.0, //����� ���� (� ������ ������� ���������� - 1)
        numerator = -x * x, //��������� ����� ��� ������ ����
        currentMember = 1.0; //������� ���� ����

    while (abs(currentMember) > absError && count <= numberMax) {
        count++;
        currentMember = numerator / denominator;

        numerator *= (-x * x);
        denominator *= count; 
        sumSeries += currentMember;
    }

    if (abs(currentMember) > absError && count > numberMax) // ���� �� ���������� �������� � ��������� ����� ���������
        outputError("�� ������� ������� �������� �������� �� ������������ ����� ���������");
   
    if (x < -1 || x > 1) // ���� x �� �������� � �������� ��������
        outputError("x �� �������� � �������� �������� (-1, 1)");
 
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

    //���� ���������� � ������� ������� inputData
    numberMax = inputData("������� ������������ ����� ��������� (������ 0): ", "int");
    if (numberMax <= 0) //���� ������������ ����� ��������� ������ ��� ����� 0, ����� ������
        outputError("�� ����� ������������ ��������, ������������ ����� ��������� ������ ���� ������ 0, ���������� ���������");

    start = inputData("������� ������ ��������� (-1, 1): ", "double");
    end = inputData("������� ����� ��������� (-1, 1): ", "double");
    if (start > end) //����� ����� ���������, ���� start > end, ��������� ��������� � �������
        outputError("�� ����� ������������ ��������, ������ ��������� ������ ���� ������, ��� ����� ���������, ���������� ���������");

    step = inputData("������� ��� (������ 1e-12): ", "double"); //��� ������ ���� ������������� � ������, ��� 1e-12, ����� ��������� ����� ��� ����������
    if (step < 1e-12)
        outputError("�� ����� ������������ ��������, ��� ������ ���� ������, ��� 1e-12, ���������� ���������");

    absError = inputData("������� �����������: ", "double");
    
    //�������� ����� �������
    int prec = int(ceil(log10(1 / absError))); //���������� �������� �����
    
    cout << "|===============|===============|===============|\n"
            "|       X       |Taylor e^(-x*x)|    C++ exp    |\n"
            "|===============|===============|===============|\n";
    
    for (double x = start; x <= end; x += step) {
        
        stringstream stdBuf, ourBuf;

        if (abs(x) < 1e-12) //����� x ������������ � ����, � ������� ����� ������������ ������� ��������
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