//
// Created by 43741 on 2025/5/17.
//

#ifndef TRAIN_H
#define TRAIN_H
#include <valarray>

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

class TrainManagement;
class TicketManagement;
class Station {
    MyChar<24> stationName;
    int price = 0; //到这一站的票价
    int arriveTime = 0;
    int leaveTime = 0;

    friend class Train;
    friend class TrainManagement;
    friend class TicketManagement;
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

struct Seat {
    int seat[100]{0};

    Seat &operator=(const Seat &other) {
        if (this != &other) {
            for (int i = 0; i < 100; ++i) {
                seat[i] = other.seat[i];
            }
        }
        return *this;
    }
};

class TrainManagement;
class TicketManagement;
class Train {
    MyChar<24> trainID;
    int stationNum = 0;
    MyChar<2> type;
    Station stations[100];
    int seatNum = 0;
    Clock startTime;
    SaleDate salesDate;
    bool is_release = false;
    int index = 0;

    friend class TrainManagement;
    friend class TicketManagement;
public:
    Train() = default;

    Train(const MyChar<24> &trainID, int stationNum, const MyChar<2> &type, Station stations[100], int seatNum,
          const Clock &startTime, const SaleDate &sale_date, bool isRelease): trainID(trainID), stationNum(stationNum),
                                                                              type(type),
                                                                              seatNum(seatNum), startTime(startTime),
                                                                              salesDate(sale_date),
                                                                              is_release(isRelease) {
        for (int i = 1; i <= stationNum; ++i) {
            this->stations[i] = stations[i];
        }
    }

    //trainID,stationNum,seatNum,startTime,saleDate,type;
    Train(const MyChar<24> &trainID, int stationNum, int seatNum, const Clock &startTime, const SaleDate &salesdate,
          const MyChar<2> &type): trainID(trainID), stationNum(stationNum), seatNum(seatNum), startTime(startTime),
                                  salesDate(salesdate) {
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

class TrainManagement;
class TicketManagement;
class TrainInfo {
    SaleDate date;
    Clock clock;
    int index = 0;
    int maxSeats = 0;

    friend class TrainManagement;
    friend class TicketManagement;
public:
    TrainInfo() = default;

    TrainInfo(const SaleDate &d, const Clock &c, int ind, int se): date(d), clock(c), index(ind), maxSeats(se) {
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

class TrainManagement;

class TrainStation {
    TrainInfo info;
    MyChar<24> trainID;
    int stationNum = 0; //这是火车的第几站
    int price = 0;
    int arr_time = 0;
    int lea_time = 0;

    friend class TrainManagement;

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

class TrainManagement;

class CompareInfo {
    int price;
    int time;
    MyChar<24> trainID;
    int start_ = 0;
    int end_ = 0;

    friend class CompareCost;
    friend class CompareTime;
    friend class CompareTrans;
    friend class TrainManagement;

public:
    CompareInfo() = default;

    CompareInfo(int p, int t, const MyChar<24> &trainID, int st, int en): price(p), time(t), trainID(trainID),
                                                                          start_(st),
                                                                          end_(en) {
    }

    CompareInfo(const CompareInfo &other) = default;

    CompareInfo &operator=(const CompareInfo &other) {
        if (this != &other) {
            price = other.price;
            time = other.time;
            trainID = other.trainID;
            start_ = other.start_;
            end_ = other.end_;
        }
        return *this;
    }
};

class CompareCost {
    bool operator()(const CompareInfo &a, const CompareInfo &b) const {
        if (a.price != b.price) {
            return a.price < b.price;
        }
        return a.trainID < b.trainID;
    }
};

class CompareTime {
    bool operator()(const CompareInfo &a, const CompareInfo &b) const {
        if (a.time != b.time) {
            return a.time < b.time;
        }
        return a.trainID < b.trainID;
    }
};

class TrainManagement;
class CompareTrans {
    CompareInfo a;
    CompareInfo b;
    pair<Date, Clock> start_leave;
    pair<Date, Clock> end_arrive;
    pair<Date, Clock> trans_arrive;
    pair<Date, Clock> trans_leave;
    int time;
    int cost;

    friend class TrainManagement;
public:
    CompareTrans() = default;

    CompareTrans(const CompareInfo &a,const CompareInfo &b,const pair <Date,Clock> &lea_start,
        const pair <Date,Clock> &arr_mid,const pair <Date,Clock> &lea_mid,const pair <Date,Clock> &arr_end,int time,int price):start_leave(lea_start),trans_arrive(arr_mid),trans_leave(lea_mid),end_arrive(arr_end),time(time),cost(price) {
        this->a = a;
        this->b = b;
    }

    CompareTrans(const CompareTrans &other) = default;

    bool compareCost(const CompareTrans &other) const {
        if (cost != other.cost) {
            return cost < other.cost;
        }
        if (time != other.time) {
            return time < other.time;
        }
        if (a.trainID != other.a.trainID) {
            return a.trainID < other.a.trainID;
        }
        return b.trainID < other.b.trainID;
    }

    bool compareTime(const CompareTrans &other) const {
        if (time != other.time) {
            return time < other.time;
        }
        if (cost != other.cost) {
            return cost < other.cost;
        }
        if (a.trainID != other.a.trainID) {
            return a.trainID < other.a.trainID;
        }
        return b.trainID < other.b.trainID;
    }
};

#endif //TRAIN_H
