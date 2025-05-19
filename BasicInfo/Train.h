//
// Created by 43741 on 2025/5/17.
//

#ifndef TRAIN_H
#define TRAIN_H
#include "../Tool/MyChar.h"
#include "../Tool/Time.h"
#include "../STLite/utility.h"

using sjtu::pair;

inline pair<Date, Clock> intToReadTime(Date day, Clock clock, int time) {
    clock.minute += time;
    if (clock.minute >= 60) {
        clock.hour += clock.minute / 60;
        clock.minute %= 60;
    }
    if (clock.hour >= 24) {
        day.day += clock.hour / 24;
        clock.hour %= 24;
    }
    while (day.day > Month[day.month]) {
        day.day -= Month[day.month];
        day.month += 1;
    }
    return {day, clock};
}

class Station {
    MyChar<24> stationName;
    int price = 0; //到这一站的票价
    int arriveTime = 0;
    int leaveTime = 0;

public:
    Station() = default;

    Station(const MyChar<24> &stationName, int price, int arr, int lea): stationName(stationName), price(price),
                                                                         arriveTime(arr), leaveTime(lea) {
    }

    Station(const Station &other) = default;

    Station &operator=(const Station &other) {
        if (this != &other) {
            stationName = other.stationName;
            price = other.price;
            arriveTime = other.arriveTime;
            leaveTime = other.leaveTime;
        }
        return *this;
    }

    bool operator==(const Station &other) const {
        return stationName == other.stationName;
    }

    bool operator!=(const Station &other) const {
        return !(*this == other);
    }
};

class Train {
    MyChar<24> trainID;
    int stationNum = 0;
    Station stations[100];
    int seatNum = 0;
    Clock startTime;
    MyChar<2> type;
    SaleDate salesDate;

public:
    Train() = default;

    Train(const MyChar<24> &trainID) : trainID(trainID), startTime(), salesDate() {
    }

    Train(const Train &other): trainID(other.trainID), stationNum(other.stationNum), seatNum(other.seatNum),
                               startTime(other.startTime), type(other.type), salesDate(other.salesDate) {
        for (int i = 1; i <= other.stationNum; ++i) {
            stations[i] = other.stations[i];
        }
    }

    Train &operator=(const Train &other) {
        if (this != &other) {
            trainID = other.trainID;
            stationNum = other.stationNum;
            startTime = other.startTime;
            seatNum = other.seatNum;
            type = other.type;
            salesDate = other.salesDate;
            for (int i = 1; i <= stationNum; ++i) {
                stations[i] = other.stations[i];
            }
        }
        return *this;
    }

    bool operator==(const Train &other) const {
        return trainID == other.trainID;
    }

    bool operator<(const Train &other) const {
        return trainID < other.trainID;
    }

    bool operator!=(const Train &other) const {
        return !(*this == other);
    }
};

class TrainInfo {
    Date date;
    Clock clock;
    int index = 0;
    int maxSeats = 0;

public:
    TrainInfo() = default;

    TrainInfo(const Date &d, const Clock &c, int ind, int se): date(d), clock(c), index(ind), maxSeats(se) {
    }

    TrainInfo(const TrainInfo &other) = default;

    TrainInfo &operator=(const TrainInfo &other) {
        if (this != &other) {
            date = other.date;
            clock = other.clock;
            index = other.index;
            maxSeats = other.maxSeats;
        }
        return *this;
    }

    bool operator==(const TrainInfo &other) const {
        return index == other.index;
    }

    bool operator<(const TrainInfo &other) const {
        return index < other.index;
    }
};

class TrainStation {
    TrainInfo info;
    MyChar<24> trainID;
    int stationNum = 0; //这是火车的第几站
    int price = 0;
    int arr_time = 0;
    int lea_time = 0;

public:
    TrainStation() = default;

    TrainStation(const TrainInfo &info, const MyChar<24> &trainID, int staNum, int pri, int arr, int lea): info(info),
        trainID(trainID), stationNum(staNum), price(pri), arr_time(arr), lea_time(lea) {
    }

    TrainStation(const TrainStation &other) = default;

    TrainStation &operator=(const TrainStation &other) {
        if (this != &other) {
            info = other.info;
            trainID = other.trainID;
            price = other.price;
            stationNum = other.stationNum;
            arr_time = other.arr_time;
            lea_time = other.lea_time;
        }
        return *this;
    }
};

#endif //TRAIN_H
