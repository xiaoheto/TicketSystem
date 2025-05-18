//
// Created by 43741 on 2025/5/17.
//

#ifndef TRAIN_H
#define TRAIN_H
#include "../Tool/MyChar.h"
#include "../Tool/Time.h"

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
    std::pair<Date, Date> salesDate;

public:
    Train() = default;

    Train(const MyChar<24> &trainID) : trainID(trainID) {
    }

    Train(const Train &other): trainID(other.trainID), stationNum(other.stationNum), seatNum(other.seatNum),
    startTime(other.startTime),type(other.type),salesDate(other.salesDate) {
        for(int i = 0;i < other.stationNum;++i) {
            stations[i] = other.stations[i];
        }
    }
};
#endif //TRAIN_H
