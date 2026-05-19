#include <cassert>
#include <sstream>
#include <iostream>
#include "console_io.hpp"

// Вспомогательная функция для перенаправления cin
void setInput(const std::string& data) {
    static std::streambuf* oldCin = std::cin.rdbuf();
    static std::istringstream* currentInput = nullptr;
    delete currentInput;
    currentInput = new std::istringstream(data);
    std::cin.rdbuf(currentInput->rdbuf());
}

// Тесты для DateValidator
void testDateValidator() {
    assert(DateValidator::isValidFormat("01.01.2000") == true);
    assert(DateValidator::isValidFormat("29.02.2020") == true);
    assert(DateValidator::isValidFormat("31.12.2023") == true);
    assert(DateValidator::isValidFormat("32.01.2000") == false);
    assert(DateValidator::isValidFormat("00.01.2000") == false);
    assert(DateValidator::isValidFormat("29.02.2021") == false);
    assert(DateValidator::isValidFormat("31.04.2000") == false);
    assert(DateValidator::isValidFormat("abcd") == false);

    assert(DateValidator::isValidDate(29, 2, 2020) == true);
    assert(DateValidator::isValidDate(29, 2, 2021) == false);
    assert(DateValidator::isLeapYear(2020) == true);
    assert(DateValidator::isLeapYear(2021) == false);
}

// Тесты для InputReader
void testReadString() {
    setInput("hello\n");
    std::string result = InputReader::readString("Enter: ");
    assert(result == "hello");
}

void testReadInt() {
    setInput("42\n");
    int result = InputReader::readInt("Enter int: ");
    assert(result == 42);
}

void testReadIntInvalidThenValid() {
    setInput("abc\n123\n");
    int result = InputReader::readInt("Enter int: ");
    assert(result == 123);
}

void testReadDouble() {
    setInput("3.14\n");
    double result = InputReader::readDouble("Enter double: ");
    assert(result == 3.14);
}

void testReadDate() {
    setInput("31.12.2023\n");
    std::string result = InputReader::readDate("Enter date: ");
    assert(result == "31.12.2023");

    setInput("32.01.2000\n01.01.2000\n");
    result = InputReader::readDate("Enter date: ");
    assert(result == "01.01.2000");
}

int main() {
    testDateValidator();
    testReadString();
    testReadInt();
    testReadIntInvalidThenValid();
    testReadDouble();
    testReadDate();

    std::cout << "Все тесты пройдены успешно!\n";
    return 0;
}