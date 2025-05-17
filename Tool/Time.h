//
// Created by 43741 on 2025/5/17.
//

#ifndef TIME_H
#define TIME_H
#include <string>
using std::string;
using std::ostream;

int Month[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

class Clock {
    int hour;
    int minute;

public:

    Clock() = default;

    Clock(int h,int m):hour(h),minute(m) {
    }

    Clock(const Clock &other) = default;

    static bool checkClockValid(int h, int m) {
        return (h >= 0 && h < 24) && (m >= 0 && m < 60);
    }

    Clock &operator=(const Clock &other) {
        if (this != &other) {
            hour = other.hour;
            minute = other.minute;
        }
        return *this;
    }

    bool operator==(const Clock &other) const {
        return hour == other.hour && minute == other.minute;
    }

    bool operator<(const Clock &other) const {
        if (hour != other.hour) {
            return hour < other.hour;
        }
        return minute < other.minute;
    }

    static string int_to_clock(int n) {
        if (n < 10) {
            return "0" + std::to_string(n);
        }
        return std::to_string(n);
    }

    friend ostream &operator<<(ostream &out,const Clock &obj) {
        out << int_to_clock(obj.hour) << ':' << int_to_clock(obj.minute);
        return out;
    }
};

class Date {
    int month;
    int day;

public:
    Date() = default;

    Date(int m,int d):month(m),day(d) {
    }

    Date(const Date &other) = default;

    Date &operator=(const Date &other) {
        if (this != &other) {
            month = other.month;
            day = other.day;
        }
        return *this;
    }

    bool CheckDateValid(int m, int d) const {
        if (m <= 0 || m > 12 || d <= 0 || d > Month[m]) {
            return false;
        }
        return true;
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

    int operator-(const Date &other) const{
        if (month == other.month) {
            return day - other.day;
        }
        int ans = 0;
        for(int i = other.month + 1;i < month;++i) {
            ans += Month[i];
        }

        ans += Month[other.month] - other.day;
        ans += day;
        return ans;
    }

    static string int_to_date(int n) {
        if (n < 10) {
            return "0" + std::to_string(n);
        }
        return std::to_string(n);
    }

    friend ostream &operator<<(ostream &out,const Date &obj) {
        out << int_to_date(obj.month) << '-' << int_to_date(obj.day);
        return out;
    }
};
#endif //TIME_H
