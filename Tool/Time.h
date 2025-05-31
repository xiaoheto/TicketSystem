// Time.hpp
#ifndef TIME_HPP
#define TIME_HPP

#include <string>
#include<iostream>

using std::pair;
using std::string;
using std::ostream;

extern int Month[13]; // 只声明，定义放在 Time.cpp 中

class Date;

class Clock {
    int hour = 0;
    int minute = 0;

    friend pair<Date, Clock> intToReadTime(Date day, Clock clock, int time);
    friend class Date;
public:
    Clock() = default;

    Clock(int h, int m);

    Clock(const string &input);

    Clock(const Clock &other) = default;

    static bool checkClockValid(int h, int m);

    Clock &operator=(const Clock &other);

    bool operator==(const Clock &other) const;

    bool operator<(const Clock &other) const;

    bool operator>(const Clock &other) const;

    bool operator>=(const Clock &other) const;

    static string int_to_clock(int n);

    int operator-(const Clock &other) const;

    friend ostream &operator<<(ostream &out, const Clock &obj);
};

class TrainManagement;
class Date {
    int month;
    int day;

    friend class SaleDate;
    friend class TrainManagement;
    friend pair<Date, Clock> intToReadTime(Date day, Clock clock, int time);

public:
    Date() = default;

    Date(int m, int d);

    Date(const string &input);

    Date(const Date &other) = default;

    Date &operator=(const Date &other);

    bool CheckDateValid(int m, int d) const;

    bool operator==(const Date &other) const;

    bool operator!=(const Date &other) const;

    bool operator<(const Date &other) const;

    bool operator<=(const Date &other) const;

    bool operator>=(const Date &other) const;

    bool operator>(const Date &other) const;

    int operator-(const Date &other) const;

    Date &operator++();

    Date operator++(int n);

    static string int_to_date(int n);

    static int now_to_start(const Date &start_day, const Date &now) {
        int sum = 0;
        for (int i = start_day.month + 1; i < now.month; i++) {
            sum += Month[i];
        }
        if (start_day.month != now.month) {
            sum += (Month[start_day.month] - start_day.day + 1);
            sum += now.day;
        } else {
            sum += (now.day - start_day.day + 1);
        }
        return sum;
    }

    static Date find_ini_day(const Date &curDay, const Clock &startTime, int time) {
        Date cur_day = curDay;
        Clock ini_clock = startTime;
        ini_clock.minute += time;
        if (ini_clock.minute >= 60) {
            ini_clock.hour += ini_clock.minute / 60;
            ini_clock.minute %= 60;
        }

        int d_day = 0;
        if (ini_clock.hour >= 24) {
            d_day = ini_clock.hour / 24;
            ini_clock.hour %= 24;
        }

        cur_day.day -= d_day;
        while (cur_day.day <= 0) {
            cur_day.month--;
            if (cur_day.month == 0) {
                cur_day.month = 12;
            }
            cur_day.day += Month[cur_day.month];
        }

        return cur_day;
    }


    friend ostream &operator<<(ostream &out, const Date &obj);
};

class TrainManagement;
class TicketManagement;
class SaleDate {
    Date startDate;
    Date endDate;

    friend class TrainManagement;
    friend class TicketManagement;
public:
    SaleDate();

    SaleDate(const Date &st, const Date &en);

    SaleDate(const SaleDate &other);

    SaleDate &operator=(const SaleDate &other);

    bool operator==(const SaleDate &other) const;

    bool CheckDayInSale(const Date &day) const;
};

#endif // TIME_HPP
