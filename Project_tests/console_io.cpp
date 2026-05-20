#include "console_io.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#error "This implementation is for Windows. Use _WIN32 to compile."
#endif

bool DateValidator::isValidFormat(const std::string& date) {
    std::regex pattern(R"(^\d{2}\.\d{2}\.\d{4}$)");
    if (!std::regex_match(date, pattern))
        return false;

    int day, month, year;
    char dot;
    std::istringstream ss(date);
    ss >> day >> dot >> month >> dot >> year;
    return isValidDate(day, month, year);
}

bool DateValidator::isValidDate(int day, int month, int year) {
    if (month < 1 || month > 12 || day < 1)
        return false;

    static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int maxDay = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year))
        maxDay = 29;

    return day <= maxDay;
}

bool DateValidator::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

std::string InputReader::readString(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int InputReader::readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        bool valid = true;
        if (input.empty()) valid = false;
        else {
            size_t start = (input[0] == '-') ? 1 : 0;
            if (start >= input.size()) valid = false;
            else {
                for (size_t i = start; i < input.size(); ++i) {
                    if (!std::isdigit(static_cast<unsigned char>(input[i]))) {
                        valid = false;
                        break;
                    }
                }
            }
        }

        if (valid) {
            try {
                size_t pos;
                int value = std::stoi(input, &pos);
                if (pos == input.size())
                    return value;
            }
            catch (...) {
                std::cout << "Ошибка: число слишком большое или маленькое.\n";
                continue;
            }
        }
        std::cout << "Ошибка: введите целое число (например, -123, 0, 42).\n";
    }
}

double InputReader::readDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        bool valid = true;
        bool dotSeen = false;
        if (input.empty()) valid = false;
        else {
            size_t start = (input[0] == '-') ? 1 : 0;
            if (start >= input.size()) valid = false;
            else {
                for (size_t i = start; i < input.size(); ++i) {
                    if (input[i] == '.') {
                        if (dotSeen) { valid = false; break; }
                        dotSeen = true;
                    }
                    else if (!std::isdigit(static_cast<unsigned char>(input[i]))) {
                        valid = false;
                        break;
                    }
                }
            }
        }

        if (valid) {
            try {
                size_t pos;
                double value = std::stod(input, &pos);
                if (pos == input.size())
                    return value;
            }
            catch (...) {
                std::cout << "Ошибка: число вне диапазона double.\n";
                continue;
            }
        }
        std::cout << "Ошибка: введите число (целое или дробное, например 3.14 или -2.5).\n";
    }
}

std::string InputReader::readDate(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        if (DateValidator::isValidFormat(input))
            return input;
        std::cout << "Ошибка: дата должна быть в формате ДД.ММ.ГГГГ и существовать (например, 31.12.2023).\n";
    }
}

ConsoleMenu::ConsoleMenu(const std::vector<std::string>& items)
    : items(items), selectedIndex(0) {
}

void ConsoleMenu::gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ConsoleMenu::setCursorVisible(bool visible) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void ConsoleMenu::display() const {
    system("cls");
    for (size_t i = 0; i < items.size(); ++i) {
        if (i == static_cast<size_t>(selectedIndex))
            std::cout << "> " << items[i] << std::endl;
        else
            std::cout << "  " << items[i] << std::endl;
    }
    std::cout << "\nUse arrows ↑/↓ to navigate, Enter to select." << std::endl;
}

int ConsoleMenu::run() {
    setCursorVisible(false);
    display();
    while (true) {
        int key = _getch();
        if (key == 224) { 
            key = _getch();
            if (key == 72) { 
                selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
                display();
            }
            else if (key == 80) { 
                selectedIndex = (selectedIndex + 1) % items.size();
                display();
            }
        }
        else if (key == 13) { 
            setCursorVisible(true);
            return selectedIndex;
        }
    }
}