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
    MemoryRiver<int, 0> delete_index;

    int size = 0;

public:
    vector<int> delete_seat;

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

    void query_transfer(Date day, const MyChar<24> &s, const MyChar<24> &t, bool is_cost) {
        vector<TrainStation> start_stations = station_data.query(s);
        vector<TrainStation> end_stations = station_data.query(t);

        CompareTrans final_ans;
        bool is_first = true;

        if (start_stations.empty() || end_stations.empty()) {
            cout << 0 << '\n';
            return;
        }

        for (int i = 0; i < start_stations.size(); ++i) {
            vector<TrainInfo> res1 = train_data.query(start_stations[i].trainID);
            Train train1;
            train_index.read(train1, res1[0].index);

            pair<Date, Clock> early1 = int_to_read_time(train1.salesDate.startDate, train1.startTime,
                                                        start_stations[i].lea_time);
            pair<Date, Clock> late1 = int_to_read_time(train1.salesDate.endDate, train1.startTime,
                                                       start_stations[i].lea_time);

            if (early1.first <= day && late1.first >= day) {
                // 舍去day不在运行时间范围内的火车
                for (int j = 0; j < end_stations.size(); ++j) {
                    TrainStation start_station = start_stations[i];
                    TrainStation end_station = end_stations[j];

                    if (start_station.trainID == end_station.trainID) {
                        continue;
                    } //舍弃掉同一辆火车的情况
                    Train train2;
                    vector<TrainInfo> res2 = train_data.query(end_station.trainID);
                    train_index.read(train2, res2[0].index);

                    for (int m = start_station.stationNum + 1; m <= train1.stationNum; ++m) {
                        for (int n = 0; n < end_station.stationNum; ++n) {
                            if (train1.stations[m].stationName == train2.stations[n].stationName) {
                                // 有相同的中转站
                                TrainStation mid_station1 = start_stations[m];
                                TrainStation mid_station2 = end_stations[n];
                                pair<Date, Clock> lea_start, arr_mid, lea_mid, arr_end;

                                Date ini_day1 = Date::find_ini_day(day, train1.startTime, start_station.lea_time);
                                lea_start = int_to_read_time(ini_day1, train1.startTime, start_station.lea_time);
                                arr_mid = int_to_read_time(ini_day1, train1.startTime, mid_station1.arr_time);

                                pair<Date, Clock> lea_temp = int_to_read_time(
                                    day, train2.startTime, mid_station2.lea_time);
                                pair<Date, Clock> late = int_to_read_time(
                                    train2.salesDate.endDate, train2.startTime, mid_station2.lea_time);
                                pair<Date, Clock> early = int_to_read_time(
                                    train2.salesDate.startDate, train2.startTime, mid_station2.lea_time);
                                int tim, pri;

                                if (arr_mid.first > late.first || (
                                        arr_mid.first == late.first && arr_mid.second > late.second)) {
                                    // 到中转站的时候train的中专火车已经离开
                                    continue;
                                }

                                if (arr_mid.first < early.first) {
                                    // 提前到达
                                    lea_mid = {early.first, lea_temp.second};
                                    arr_end = int_to_read_time(early.first, lea_mid.second,
                                                               end_station.arr_time - mid_station2.lea_time);
                                } else {
                                    if (arr_mid.second < lea_temp.second) {
                                        lea_mid = {arr_mid.first, lea_temp.second};
                                    } else {
                                        Date temp = arr_mid.first;
                                        ++temp;
                                        lea_mid = {temp, lea_temp.second};
                                    }
                                    arr_end = int_to_read_time(lea_mid.first, lea_mid.second,
                                                               end_station.arr_time - mid_station2.lea_time);
                                }

                                int price1 = mid_station1.price - start_station.price;
                                int price2 = end_station.price - mid_station2.price;

                                int time1 = mid_station1.arr_time - start_station.lea_time;
                                int time2 = end_station.arr_time - mid_station2.lea_time;
                                int time_mid = lea_mid.second - arr_mid.second + 24 * 60 * (
                                                   lea_mid.first - arr_mid.first);

                                pri = price1 + price2;
                                tim = time1 + time2 + time_mid;
                                CompareInfo a(price1, time1, train1.trainID, i, m);
                                CompareInfo b(price2, time2, train2.trainID, n, j);

                                CompareTrans temp_ans(a, b, lea_start, arr_mid, lea_mid, arr_end, tim, pri);

                                if (is_first) {
                                    final_ans = temp_ans;
                                } else {
                                    if (is_cost) {
                                        if (!final_ans.compareCost(temp_ans)) {
                                            final_ans = temp_ans;
                                        }
                                    } else {
                                        if (!final_ans.compareTime(temp_ans)) {
                                            final_ans = temp_ans;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        vector<TrainInfo> a_train = train_data.query(final_ans.a.trainID);
        vector<TrainInfo> b_train = train_data.query(final_ans.b.trainID);
        Train a_, b_;
        train_index.read(a_, a_train[0].index);
        train_index.read(b_, b_train[0].index);
        Seat seat1, seat2;
        Date ini_day1 = Date::find_ini_day(day, a_.startTime, a_.stations[final_ans.a.start_].leaveTime);
        int d_day1 = Date::get_d_day(a_.salesDate.startDate, ini_day1);
        Date ini_day2 = Date::find_ini_day(final_ans.trans_leave.first, b_.startTime,
                                           b_.stations[final_ans.b.start_].leaveTime);
        int d_day2 = Date::get_d_day(b_.salesDate.startDate, ini_day2);

        seat_index.read(seat1, 100 * a_.index + d_day1);
        seat_index.read(seat2, 100 * b_.index + d_day2);
        int seat_num1 = cal_cur_ticket(final_ans.a.start_, final_ans.a.end_, seat1, a_.seatNum);
        int seat_num2 = cal_cur_ticket(final_ans.b.start_, final_ans.b.end_, seat2, b_.seatNum);

        cout << a_.trainID << ' ' << a_.stations[final_ans.a.start_].stationName << ' ' << final_ans.start_leave.first
                <<
                ' ' << final_ans.start_leave.second << " -> " << a_.stations[final_ans.a.end_].stationName << ' ' <<
                final_ans.trans_arrive.first << ' ' << final_ans.trans_arrive.second << ' ' << final_ans.a.price << ' '
                <<
                seat_num1 << '\n';
        cout << b_.trainID << ' ' << b_.stations[final_ans.b.start_].stationName << ' ' << final_ans.trans_leave.first
                <<
                ' ' << final_ans.trans_leave.second << " -> " << b_.stations[final_ans.b.start_].stationName << ' ' <<
                final_ans.end_arrive.first << ' ' << final_ans.end_arrive.second << ' ' << final_ans.b.price << ' ' <<
                seat_num2 << '\n';
    }

    static void clean_train_file() {
        std::filesystem::path path1("train.data");
        std::filesystem::path path2("train.index");
        std::filesystem::path path3("station.data");
        std::filesystem::path path4("seat.index");
        std::filesystem::path path5("delete.index");
        if (exists(path1)) std::filesystem::remove(path1);
        if (exists(path2)) std::filesystem::remove(path2);
        if (exists(path3)) std::filesystem::remove(path3);
        if (exists(path4)) std::filesystem::remove(path4);
        if (exists(path4)) std::filesystem::remove(path5);
    }

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
