//
// Created by 43741 on 2025/5/18.
//

#ifndef TICKET_H
#define TICKET_H
#include "../Tool/MyChar.h"
#include "../Tool/Time.h"

enum State {
    success, pending, refunded
};

class TicketManagement;

class Ticket {
    MyChar<24> username;
    MyChar<24> trainID;
    int start_ = 0;
    int end_ = 0;
    Date day = Date();
    State state = success;
    int time = 0; // 交易时间
    int num = 0;
    int index;

    friend class TicketManagement;

public:
    Ticket() = default;

    Ticket(const MyChar<24> &username, const MyChar<24> &trainID, int start, int end, const Date &date, State state,
           int time,int num)
        : username(username), trainID(trainID), start_(start), end_(end), day(date), state(state), time(time),num(num) {
    }

    Ticket(const Ticket &other) = default;

    Ticket &operator=(const Ticket &other) {
        if (this != &other) {
            username = other.username;
            trainID = other.trainID;
            start_ = other.start_;
            end_ = other.end_;
            day = other.day;
            state = other.state;
            time = other.time;
        }
        return *this;
    }

    bool operator==(const Ticket &other) const {
        return time == other.time;
    }

    bool operator<(const Ticket &other) const {
        return time < other.time;
    }

    bool operator!=(const Ticket &other) const {
        return !(*this == other);
    }

    bool operator>(const Ticket &other) const {
        return time > other.time;
    }
};

class TrainTicketInfo {
    MyChar<24> trainID;
    Date day = Date(); // initial_day

public:
    TrainTicketInfo() = default;

    TrainTicketInfo(const MyChar<24> &trainID, const Date &d): trainID(trainID), day(d) {
    }

    TrainTicketInfo(const TrainTicketInfo &other) = default;

    TrainTicketInfo &operator=(const TrainTicketInfo &other) {
        if (this != &other) {
            trainID = other.trainID;
            day = other.day;
        }
        return *this;
    }
};
#endif //TICKET_H
