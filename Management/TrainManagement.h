#ifndef TRAINMANAGEMENT_H;
#define TRAINMANAGEMENT_H;

#include "../BPT/BPlusTree.h"
#include "../BPT/MemoryRiver.h"
#include "../Tool/MyChar.h"
#include "../BasicInfo/Train.h"
#include <filesystem>

#include "priority_queue.h"

using sjtu::priority_queue;

class TicketManagement;

class TrainManagement {
    friend class TicketManagement;
    BPTree<MyChar<24>, TrainInfo, 40> train_data; // trainID -> trainInfo
    MemoryRiver<Train> train_index;

    MemoryRiver<Seat, 0> seat_index;

    BPTree<MyChar<24>, TrainStation> station_data; // trainID -> TrainStation

    vector<int> delete_seat;
    MemoryRiver<int, 0> delete_index;

    int size = 0;

public:
    TrainManagement(): train_data("train.data"), station_data("station.data") {
        train_index.initialise("train.index");
        seat_index.initialise("seat.index");
        delete_index.initialise("delete.index");
        train_index.get_info(size, 1);

        int temp;
        for (int i = 0; i < size; ++i) {
            delete_index.read(temp, 2 * sizeof(int) + i * sizeof(int));
            delete_seat.push_back(temp);
        }
    }

    ~TrainManagement() {
        train_index.write_info(size, 1);
        delete_index.write_info(static_cast<int>(delete_seat.size()), 1);
        for (int i = 0; i < delete_seat.size(); ++i) {
            delete_index.write_info(delete_seat[i], 2 * sizeof(int) + i * sizeof(int));
        }
    }

    int add_train(const MyChar<24> &trainID, int stationNum, int seatNum,
                  vector<string> stationNames, const vector<int> &prices, Clock startTime,
                  vector<int> travelTimes,
                  vector<int> stopoverTimes, const SalesDate &saleDate, const MyChar<2> &type) {
        auto it = train_data.query(trainID);
        if (!it.empty()) {
            return -1;
        }

        Station stations[100];
        int offset;
        if (delete_seat.empty()) {
            offset = delete_seat.back();
            delete_seat.pop_back();
        } else {
            offset = train_index.get_index();
        }
        TrainInfo info(saleDate, startTime, offset, seatNum);
        train_data.insert(trainID, info);

        Train train(trainID, stationNum, type, seatNum, startTime, saleDate, offset);

        for (int i = 1; i <= stationNum; ++i) {
            if (i == 1) {
                train.stations[i].stationName = stationNames[i - 1];
                train.stations[i].price = 0;
                train.stations[i].arriveTime = 0;
                train.stations[i].leaveTime = 0;
            } else if (i != stationNum) {
                train.stations[i].stationName = stationNames[i - 1];
                train.stations[i].price = train.stations[i - 1].price + prices[i - 2];
                train.stations[i].arriveTime = train.stations[i - 1].leaveTime + travelTimes[i - 2];
                train.stations[i].leaveTime = train.stations[i].arriveTime + stopoverTimes[i - 2];
            } else {
                train.stations[i].stationName = stationNames[i - 1];
                train.stations[i].price = train.stations[i - 1].price + prices[i - 2];
                train.stations[i].arriveTime = train.stations[i - 1].leaveTime + travelTimes[i - 2];
                train.stations[i].leaveTime = 0;
            }
        }
        train_index.write(train);
        return 0;
    }

    int delete_train(const MyChar<24> &trainID) {
        vector<TrainInfo> train_res = train_data.query(trainID);
        if (train_res.empty()) {
            return -1;
        }

        Train train;
        train_index.read(train, train_res[0].index);

        if (!train.is_release) {
            return -1;
        }

        train_data.erase(trainID, train_res[0]);
        delete_seat.push_back(train.index);
        return 0;
    }

    int release_train(const MyChar<24> &trainID) {
        vector<TrainInfo> train_res = train_data.query(trainID);
        Train train;
        if (train_res.empty()) {
            return -1;
        }
        train_index.read(train, train_res[0].index);
        if (train.is_release) {
            return -1;
        }

        train.is_release = true;

        train_index.update(train, train.index);

        for (int i = 1; i <= train.stationNum; ++i) {
            Station cur_station = train.stations[i];
            TrainStation sta(train_res[0], trainID, train.stationNum, cur_station.price, cur_station.arriveTime,
                             cur_station.leaveTime);

            station_data.insert(trainID, sta);
        }

        return 0;
    }

    void query_train(const MyChar<24> &trainID, Date date) {
        vector<TrainInfo> train_res = train_data.query(trainID);
        if (train_res.empty()) {
            cout << -1 << '\n';
            return;
        }
        Train train;
        train_index.read(train, train_res[0].index);

        cout << train.trainID << ' ' << train.type << '\n';

        Seat seat;
        int d_day = Date::get_d_day(train.salesDate.startDate, date);
        seat_index.read(seat, get_seat_index(train.index, d_day));

        for (int i = 1; i <= train.stationNum; ++i) {
            Station cur_station = train.stations[i];
            if (i == 1) {
                cout << cur_station.stationName << ' ' << "xx-xx xx:xx" << ' ' << "->" << ' ' <<
                        int_to_read_time(date, train.startTime, cur_station.leaveTime).first << ' ' << int_to_read_time(
                            date, train.startTime,
                            cur_station.leaveTime).second << ' ' << 0 << ' ' << train.seatNum << '\n';
            } else if (i != train.stationNum) {
                pair<Date, Clock> arr_ = int_to_read_time(date, train.startTime, cur_station.arriveTime);
                pair<Date, Clock> lea_ = int_to_read_time(date, train.startTime, cur_station.leaveTime);
                cout << cur_station.stationName << ' ' << arr_.first << ' ' << arr_.second << ' ' << "->" << ' ' << lea_
                        .first << ' ' << lea_.second << ' ' << cur_station.price << ' ' << cal_cur_ticket(
                            i, i + 1, seat, train.seatNum) << '\n';
            } else {
                pair<Date, Clock> arr_ = int_to_read_time(date, train.startTime, cur_station.arriveTime);
                cout << cur_station.stationName << ' ' << arr_.first << ' ' << arr_.second << ' ' << "->" << ' ' <<
                        "xx-xx xx:xx" << cur_station.price << ' ' << "x" << '\n';
            }
        }
    }

    void query_ticket(Date day, const MyChar<24> &s, const MyChar<24> &t, bool is_cost) {
        vector<TrainStation> start_stations = station_data.query(s);
        vector<TrainStation> end_stations = station_data.query(t);

        priority_queue<CompareInfo, CompareCost> cost_res;
        priority_queue<CompareInfo, CompareTime> time_res;
        for (int i = 0; i < start_stations.size(); ++i) {
            for (int j = 0; j < end_stations.size(); ++j) {
                if (start_stations[i].trainID == end_stations[j].trainID) {
                    vector<TrainInfo> train_res = train_data.query(start_stations[i].trainID);

                    Train train;
                    train_index.read(train, train_res[0].index);

                    int st_ = 0, en_ = 0;
                    for (int m = 1; m <= train.stationNum; ++m) {
                        if (train.stations[m].stationName == s) {
                            st_ = m;
                            break;
                        }
                    }
                    for (int n = 1; n <= train.stationNum; ++n) {
                        if (train.stations[n].stationName == t) {
                            en_ = n;
                            break;
                        }
                    }

                    Station start_station = train.stations[st_];
                    Station end_station = train.stations[en_];

                    pair<Date, Clock> early = int_to_read_time(train.salesDate.startDate, train.startTime,
                                                               start_station.leaveTime);
                    pair<Date, Clock> late = int_to_read_time(train.salesDate.endDate, train.startTime,
                                                              start_station.leaveTime);

                    if (day < early.first || day > late.first) {
                        continue;
                    } // day不在运行时间内

                    int price = end_station.price - start_station.price;
                    int time = end_station.arriveTime - start_station.leaveTime;
                    CompareInfo cur(price, time, train.trainID, st_, en_);
                    if (is_cost) {
                        cost_res.push(cur);
                    } else {
                        time_res.push(cur);
                    }
                }
            }
        }

        if (is_cost) {
            cout << cost_res.size() << '\n';
            while (!cost_res.empty()) {
                CompareInfo cur = cost_res.top();
                cost_res.pop();
                vector<TrainInfo> temp = train_data.query(cur.trainID);
                Train cur_train;
                train_index.read(cur_train, temp[0].index);
                Station start_ = cur_train.stations[cur.start_];
                Station end_ = cur_train.stations[cur.end_];

                Date ini_day = Date::find_ini_day(day, cur_train.startTime, start_.leaveTime);
                pair<Date, Clock> start_time_ = int_to_read_time(ini_day, cur_train.startTime, start_.leaveTime);
                pair<Date, Clock> end_time_ = int_to_read_time(ini_day, cur_train.startTime, end_.arriveTime);

                int d_day = Date::get_d_day(cur_train.salesDate.startDate, ini_day);
                Seat seat;
                seat_index.read(seat, get_seat_index(cur_train.index, d_day));

                int cur_ticket = cal_cur_ticket(cur.start_, cur.end_, seat, cur_train.seatNum);

                cout << cur_train.trainID << ' ' << start_.stationName << ' ' << start_time_.first << ' ' << start_time_
                        .second << ' ' << "->" << ' ' << end_.stationName << ' ' << end_time_.first << ' ' << end_time_.
                        second << ' ' << cur.price << ' ' << cur_ticket << '\n';
            }
        } else {
            CompareInfo cur = time_res.top();
            time_res.pop();
            vector<TrainInfo> temp = train_data.query(cur.trainID);
            Train cur_train;
            train_index.read(cur_train, temp[0].index);
            Station start_ = cur_train.stations[cur.start_];
            Station end_ = cur_train.stations[cur.end_];

            Date ini_day = Date::find_ini_day(day, cur_train.startTime, start_.leaveTime);
            pair<Date, Clock> start_time_ = int_to_read_time(ini_day, cur_train.startTime, start_.leaveTime);
            pair<Date, Clock> end_time_ = int_to_read_time(ini_day, cur_train.startTime, end_.arriveTime);

            int d_day = Date::get_d_day(cur_train.salesDate.startDate, ini_day);
            Seat seat;
            seat_index.read(seat, get_seat_index(cur_train.index, d_day));

            int cur_ticket = cal_cur_ticket(cur.start_, cur.end_, seat, cur_train.seatNum);

            cout << cur_train.trainID << ' ' << start_.stationName << ' ' << start_time_.first << ' ' << start_time_.
                    second << ' ' << "->" << ' ' << end_.stationName << ' ' << end_time_.first << ' ' << end_time_.
                    second << ' ' << cur.price << ' ' << cur_ticket << '\n';
        }
    }

    void query_transfer()

    static pair<Date, Clock> int_to_read_time(Date ini_day, Clock startTime, int time) {
        startTime.minute += time;
        if (startTime.minute >= 60) {
            startTime.hour += startTime.minute / 60;
            startTime.minute %= 60;
        }

        if (startTime.hour >= 24) {
            ini_day.day += startTime.hour / 24;
            startTime.hour %= 24;
        }

        if (ini_day.day > Month[ini_day.month]) {
            ini_day.day -= Month[ini_day.month];
            ini_day.month += 1;
        }
        return {ini_day, startTime};
    }

    static int cal_cur_ticket(int st, int en, Seat seat, int maxSeat) {
        int min = 1e5 + 10;
        int temp = 0;
        for (int i = st; i < en; ++i) {
            temp += seat.seat[i];
            min = std::min(min, temp);
        }
        int max_seat = maxSeat;
        for (int i = 1; i < st; ++i) {
            max_seat += seat.seat[i];
        }

        return max_seat + min;
    }

    static int get_seat_index(int train_index, int d_day) {
        return 100 * train_index + d_day;
    }
};
#endif
