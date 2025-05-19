// Time.hpp
#ifndef TIME_HPP
#define TIME_HPP

#include <string>
#include<iostream>
#include <cstring>
#include "../STLite/utility.h"

using sjtu::pair;
using std::string;
using std::ostream;

extern int Month[13]; // 只声明，定义放在 Time.cpp 中

class Date;

class Clock {
    int hour;
    int minute;

    friend pair<Date, Clock> intToReadTime(Date day, Clock clock, int time);

public:
    Clock() = default;

    Clock(int h, int m);

    Clock(const string &input);

    Clock(const Clock &other) = default;

    static bool checkClockValid(int h, int m);

    Clock &operator=(const Clock &other);

    bool operator==(const Clock &other) const;

    bool operator<(const Clock &other) const;

    static string int_to_clock(int n);

    friend ostream &operator<<(ostream &out, const Clock &obj);
};

class Date {
    int month;
    int day;

    friend class SaleDate;

    friend pair<Date, Clock> intToReadTime(Date day, Clock clock, int time);

public:
    Date() = default;

    Date(int m, int d);

    Date(const string &input);

    Date(const Date &other) = default;

    Date &operator=(const Date &other);

    bool CheckDateValid(int m, int d) const;

    bool operator==(const Date &other) const;

    bool operator<(const Date &other) const;

    int operator-(const Date &other) const;

    static string int_to_date(int n);

    friend ostream &operator<<(ostream &out, const Date &obj);
};

class SaleDate {
    Date startDate;
    Date endDate;

public:
    SaleDate() = default;

    SaleDate(const Date &st, const Date &en);

    SaleDate(const SaleDate &other);

    SaleDate &operator=(const SaleDate &other);

    bool operator==(const SaleDate &other) const;

    bool CheckDayInSale(const Date &day) const;
};

#endif // TIME_HPP
