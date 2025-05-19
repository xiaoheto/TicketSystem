#include "Time.h"

int Month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// ===== Clock 类实现 =====
Clock::Clock(int h, int m) : hour(h), minute(m) {}

Clock::Clock(const string &input) {
    hour = std::stoi(input.substr(0, 2));
    minute = std::stoi(input.substr(3, 2));
}

bool Clock::checkClockValid(int h, int m) {
    return (h >= 0 && h < 24) && (m >= 0 && m < 60);
}

Clock &Clock::operator=(const Clock &other) {
    if (this != &other) {
        hour = other.hour;
        minute = other.minute;
    }
    return *this;
}

bool Clock::operator==(const Clock &other) const {
    return hour == other.hour && minute == other.minute;
}

bool Clock::operator<(const Clock &other) const {
    if (hour != other.hour) return hour < other.hour;
    return minute < other.minute;
}

string Clock::int_to_clock(int n) {
    if (n < 10) return "0" + std::to_string(n);
    return std::to_string(n);
}

ostream &operator<<(ostream &out, const Clock &obj) {
    out << Clock::int_to_clock(obj.hour) << ':' << Clock::int_to_clock(obj.minute);
    return out;
}

// ===== Date 类实现 =====
Date::Date(int m, int d) : month(m), day(d) {}

Date::Date(const string &input) {
    month = std::stoi(input.substr(0, 2));
    day = std::stoi(input.substr(3, 2));
}

Date &Date::operator=(const Date &other) {
    if (this != &other) {
        month = other.month;
        day = other.day;
    }
    return *this;
}

bool Date::CheckDateValid(int m, int d) const {
    return !(m <= 0 || m > 12 || d <= 0 || d > Month[m]);
}

bool Date::operator==(const Date &other) const {
    return month == other.month && day == other.day;
}

bool Date::operator<(const Date &other) const {
    if (month != other.month) return month < other.month;
    return day < other.day;
}

int Date::operator-(const Date &other) const {
    if (month == other.month) return day - other.day;

    int ans = 0;
    for (int i = other.month + 1; i < month; ++i) {
        ans += Month[i];
    }
    ans += Month[other.month] - other.day;
    ans += day;
    return ans;
}

string Date::int_to_date(int n) {
    if (n < 10) return "0" + std::to_string(n);
    return std::to_string(n);
}

ostream &operator<<(ostream &out, const Date &obj) {
    out << Date::int_to_date(obj.month) << '-' << Date::int_to_date(obj.day);
    return out;
}

// ===== SaleDate 类实现 =====
SaleDate::SaleDate(const Date &st, const Date &en) : startDate(st), endDate(en) {}

SaleDate::SaleDate(const SaleDate &other) : startDate(other.startDate), endDate(other.endDate) {}

SaleDate &SaleDate::operator=(const SaleDate &other) {
    if (this != &other) {
        startDate = other.startDate;
        endDate = other.endDate;
    }
    return *this;
}

bool SaleDate::operator==(const SaleDate &other) const {
    return startDate == other.startDate && endDate == other.endDate;
}

bool SaleDate::CheckDayInSale(const Date &day) const {
    if (day.month < startDate.month || day.month > endDate.month) return false;
    if ((day.month == startDate.month && day.day < startDate.day) ||
        (day.month == endDate.month && day.day > endDate.day)) return false;
    if (day.day <= 0 || day.day > Month[day.month]) return false;
    return true;
}
