#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class BinaryCalculator {
private:
    int bitSize;

    std::vector<int> decimalToBinary(int num) {
        std::vector<int> binary(bitSize, 0);
        int index = bitSize - 1;
        bool isNegative = num < 0;
        if (isNegative) {
            num = -num;
        }
        while (num > 0 && index >= 0) {
            binary[index--] = num % 2;
            num /= 2;
        }
        if (isNegative) {
            for (int i = 0; i < bitSize; ++i) {
                binary[i] = binary[i] == 0 ? 1 : 0;  
            }
            for (int i = bitSize - 1; i >= 0; --i) {
                if (binary[i] == 1) {
                    binary[i] = 0;
                }
                else {
                    binary[i] = 1;
                    break;
                }
            }
        }
        return binary;
    }

    int binaryToDecimal(const std::vector<int>& binary) {
        int num = 0;
        bool isNegative = binary[0] == 1;
        std::vector<int> temp = binary;
        if (isNegative) {
            for (int i = 0; i < bitSize; ++i) {
                temp[i] = temp[i] == 0 ? 1 : 0; 
            }
            for (int i = bitSize - 1; i >= 0; --i) {
                if (temp[i] == 1) {
                    temp[i] = 0;
                }
                else {
                    temp[i] = 1;
                    break;
                }
            }
        }
        for (int i = 0; i < bitSize; ++i) {
            num = num * 2 + temp[i];
        }
        return isNegative ? -num : num;
    }

    void checkOverflow(int num) {
        int maxValue = (1 << (bitSize - 1)) - 1;
        int minValue = -(1 << (bitSize - 1));
        if (num > maxValue || num < minValue) {
            throw std::overflow_error("Overflow error");
        }
    }

public:
    BinaryCalculator(int bitSize) : bitSize(bitSize) {}

    int add(int a, int b) {
        int result = a + b;
        checkOverflow(result);
        return result;
    }

    int subtract(int a, int b) {
        int result = a - b;
        checkOverflow(result);
        return result;
    }

    int multiply(int a, int b) {
        int result = a * b;
        checkOverflow(result);
        return result;
    }
};


class Node {
public:
    int data;
    Node* next;

    Node(int data) : data(data), next(nullptr) {}
};

class LinkedList {
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    void push(int data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    int pop() {
        if (!head) {
            throw std::underflow_error("Stack underflow");
        }
        Node* temp = head;
        int data = head->data;
        head = head->next;
        delete temp;
        return data;
    }

    bool isEmpty() {
        return head == nullptr;
    }
};

class PostfixCalculator {
private:
    LinkedList stack;
    BinaryCalculator calculator;

public:
    PostfixCalculator(int bitSize) : calculator(bitSize) {}

    int evaluate(const std::string& expression) {
        std::string token;
        for (size_t i = 0; i < expression.size(); ++i) {
            char c = expression[i];

            if (isspace(c)) {
                continue;
            }

            if (isdigit(c) || (c == '-' && isdigit(expression[i + 1]))) {
                token += c;
                if (i + 1 == expression.size() || !isdigit(expression[i + 1])) {
                    stack.push(std::stoi(token));
                    token.clear();
                }
            }
            else if (c == '+' || c == '-' || c == '*') {
                if (stack.isEmpty()) throw std::invalid_argument("Invalid expression");

                int b = stack.pop();
                if (stack.isEmpty()) throw std::invalid_argument("Invalid expression");

                int a = stack.pop();
                int result = 0;

                switch (c) {
                case '+':
                    result = calculator.add(a, b);
                    break;
                case '-':
                    result = calculator.subtract(a, b);
                    break;
                case '*':
                    result = calculator.multiply(a, b);
                    break;
                default:
                    throw std::invalid_argument("Invalid operator");
                }

                stack.push(result);
            }
            else {
                throw std::invalid_argument("Invalid character in expression");
            }
        }

        if (stack.isEmpty()) throw std::invalid_argument("Invalid expression");

        int result = stack.pop();

        if (!stack.isEmpty()) throw std::invalid_argument("Invalid expression");

        return result;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        int bitSize;
        std::cout << "Введите размер битности: ";
        std::cin >> bitSize;
        std::cin.ignore();

        PostfixCalculator calculator(bitSize);

        std::string expression;
        std::cout << "Введите постфиксную нотацию: ";
        std::getline(std::cin, expression);

        int result = calculator.evaluate(expression);
        std::cout << "Результат: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка " << std::endl;
    }

    return 0;
}