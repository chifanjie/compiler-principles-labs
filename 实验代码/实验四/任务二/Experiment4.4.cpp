#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int getPrecedence(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

string infixToRPN(const string& expression) {
    stack<char> operators;
    string result;
    unordered_map<char, int> precedence;

    for (char c : expression) {
        if (isalnum(c)) {
            result += c;
        } else if (c == '(') {
            operators.push(c);
        } else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                result += operators.top();
                operators.pop();
            }
            operators.pop(); // Remove '('
        } else if (isOperator(c)) {
            while (!operators.empty() && getPrecedence(c) <= precedence[operators.top()]) {
                result += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
    }

    while (!operators.empty()) {
        result += operators.top();
        operators.pop();
    }

    return result;
}

int main() {
    ifstream inputFile("4.4input.txt");
    ofstream outputFile("4.4output.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "无法打开文件" << endl;
        return 1;
    }

    string infixExpression;
    while (getline(inputFile, infixExpression)) {
        string rpnExpression = infixToRPN(infixExpression);
        outputFile << rpnExpression << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "已完成转换，并将结果写入 output.txt 文件" << endl;

    return 0;
}
