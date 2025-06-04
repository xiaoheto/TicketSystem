//
// Created by 43741 on 2025/6/2.
//

#ifndef TIME_H
#define TIME_H
#include <iostream>
#include <string>

using std::ostream;

int Month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
class UserManagement;
class TrainManagement;
class TicketManagement;

class Clock {
    friend class TrainManagement;
    friend class TicketManagement;

public:
    int hour = 0;
    int minute = 0;

    Clock() = default;

    Clock(int h, int m): hour(h), minute(m) {
    }

    Clock(const Clock &other) = default;

    Clock(const string &s) {
        hour = std::stoi(s.substr(0,2));
        minute = std::stoi(s.substr(3,2));
    }

    Clock &operator=(const Clock &other) {
        if (this != &other) {
            hour = other.hour;
            minute = other.minute;
        }
        return *this;
    }

    static bool checkClockValid(const Clock &clock) {
        return clock.hour >= 0 && clock.hour < 24 && clock.minute >= 0 && clock.minute < 60;
    }

    int operator-(const Clock &other) const {
        return (hour - other.hour) * 60 + minute - other.minute;
    }

    friend ostream &operator<<(ostream &out,const Clock &obj) {
        out << obj.hour << ':' << obj.minute;
        return out;
    }
};

class SalesDate;
class TrainManagement;
class TicketManagement;
class Date {
    friend class TrainManagement;
    friend class TicketManagement;

public:
    int month = 0;
    int day = 0;

    Date() = default;

    Date(const Date &other) = default;

    Date(int m,int d):month(m),day(d) {
    }

    Date(const string &s) {
        month = std::stoi(s.substr(0,2));
        day = std::stoi(s.substr(3,2));
    }

    Date &operator=(const Date &other) {
        if (this != &other) {
            month = other.month;
            day = other.day;
        }

        return *this;
    }

    static bool checkDateValid(const Date &date) {
        return date.month > 0 && date.month <= 12 && date.day > 0 && date.day < Month[date.month];
    }

    bool operator==(const Date &other) const {
        return month == other.month && day == other.day;
    }

    bool operator<(const Date &other) const {
        if (month != other.month) {
            return month < other.month;
        }
        return day < other.day;
    }

    bool operator>(const Date &other) const {
        if (month != other.month) {
            return month > other.month;
        }
        return day > other.day;
    }

    int operator-(const Date &other) const {
        if (month == other.month) {
            return day - other.day;
        }
        int temp = 0;
        for(int i = other.month + 1;i < month;++i) {
            temp += Month[i];
        }
        return temp + day + Month[other.month] - other.day;
    }

    friend ostream &operator<<(ostream &out,const Date &obj) {
        out << obj.month << '-' << obj.day;
        return out;
    }

    static Date find_ini_day(const Date &cur_date,const Clock &startTime,int time) {
        Date res = cur_date;
        Clock ini_time = startTime;

        ini_time.minute += time;

        if (ini_time.minute >= 60) {
            ini_time.hour += ini_time.minute / 60;
            ini_time.minute %= 60;
        }

        int d_day = 0;
        if (ini_time.hour >= 24) {
            d_day = ini_time.hour / 24;
            ini_time.hour %= 64;
        }

        if (cur_date.day > d_day) {
            res.day -= d_day;
            return res;
        }
        --res.month;
        res.day = Month[res.month] + cur_date.day - d_day;
        return res;
    }

    static int get_d_day(const Date &ini_day, const Date &cur_day) {
        int res = 0;
        for (int i = ini_day.month + 1;i < cur_day.month;++i) {
            res += Month[i];
        }

        if (ini_day.month != cur_day.month) {
            res += (Month[ini_day.month] - ini_day.day) + 1;
            res += cur_day.day;
            return res;
        }
        res += (cur_day.day - ini_day.day + 1);
        return res;
    }
};

class UserManagement;
class TrainManagement;
class TicketManagement;
class SalesDate {
    friend class UserManagement;
    friend class TrainManagement;
    friend class TicketManagement;

public:
    Date startDate = Date();
    Date endDate = Date();

    SalesDate() = default;

    SalesDate(const Date &d1,const Date &d2):startDate(d1),endDate(d2) {
    }

    SalesDate(const SalesDate &other) = default;

    SalesDate &operator=(const SalesDate &other) {
        if (this != &other) {
            startDate = other.startDate;
            endDate = other.endDate;
        }
        return *this;
    }
};
#endif //TIME_H
