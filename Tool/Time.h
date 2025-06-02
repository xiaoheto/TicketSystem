//
// Created by 43741 on 2025/6/2.
//

#ifndef TIME_H
#define TIME_H

int Month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
class UserManagement;
class TrainManagement;
class TicketManagement;

class Clock {
    friend class TrainManagement;
    friend class TicketManagement;

    int hour = 0;
    int minute = 0;

public:
    Clock() = default;

    Clock(int h, int m): hour(h), minute(m) {
    }

    Clock(const Clock &other) = default;

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
};

class TrainManagement;
class TicketManagement;
class Date {
    friend class TrainManagement;
    friend class TicketManagement;

    int month = 0;
    int day = 0;

public:
    Date() = default;

    Date(const Date &other) = default;

    Date(int m,int d):month(m),day(d) {
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
};

class UserManagement;
class TrainManagement;
class TicketManagement;
class SalesDate {
    Date startDate;
    Date endDate;

    friend class UserManagement;
    friend class TrainManagement;
    friend class TicketManagement;

public:
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
