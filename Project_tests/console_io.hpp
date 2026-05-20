#ifndef CONSOLE_IO_HPP
#define CONSOLE_IO_HPP

#include <string>
#include <vector>

class DateValidator {
public:
    static bool isValidFormat(const std::string& date);
    static bool isValidDate(int day, int month, int year);
    static bool isLeapYear(int year);
};

class InputReader {
public:
    static std::string readString(const std::string& prompt);
    static int readInt(const std::string& prompt);
    static double readDouble(const std::string& prompt);
    static std::string readDate(const std::string& prompt);
};

class ConsoleMenu {
public:
    explicit ConsoleMenu(const std::vector<std::string>& items);
    int run();

private:
    std::vector<std::string> items;
    int selectedIndex;

    void display() const;
    void gotoxy(int x, int y);
    void setCursorVisible(bool visible);
};

#endif 