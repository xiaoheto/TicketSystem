//
// Created by 43741 on 2025/5/18.
//

#include "TrainManagement.h"

TrainManagement::TrainManagement(): train_data("train.data"), station_data("station.data") {
    train_index.initialise("train.index");
    seat_index.initialise("seat.index");

    train_index.get_info(total, 1);
}

int TrainManagement::add_train(const MyChar<24> &trainID, int stationNum, int seatNum,
                               vector<string> stationNames, const vector<int> &prices, Clock startTime,
                               vector<int> travelTimes,
                               vector<int> stopoverTimes, const SaleDate &saleDate, const MyChar<2> &type) {
    Train train(trainID, stationNum, seatNum, startTime, saleDate, type);
    for (int i = 1; i <= stationNum; ++i) {
        train.stations[i].stationName = stationNames[i - 1];
        if (i == 1) {
            train.stations[i].price = 0;
            train.stations[i].leaveTime = train.stations[i].arriveTime = 0;
        } else if (i != stationNum) {
            train.stations[i].stationName = stationNames[i - 1];
            train.stations[i].arriveTime = train.stations[i - 1].leaveTime + travelTimes[i - 2];
            train.stations[i].leaveTime = train.stations[i].arriveTime + stopoverTimes[i - 2];
        } else {
            train.stations[i].arriveTime = train.stations[i - 1].leaveTime + travelTimes[i - 2];
            train.stations[i].leaveTime = 0;
        }
    }

    vector<TrainInfo> res = train_data.query(trainID);
    if (!res.empty()) {
        return -1;
    }
    int offset;
    if (delete_index.empty()) {
        offset = train_index.write(train);
    } else {
        offset = delete_index.back();
        delete_index.pop_back();
    }
    train_index.update(train, offset);
    return 0;
}

int TrainManagement::delete_train(const MyChar<24> &trainID) {
    TrainInfo info;
    vector<TrainInfo> res = train_data.query(trainID);
    if (res.empty()) {
        return -1;
    }
    info = res[0];
    Train train;
    train_index.read(train, res[0].index);
    if (train.is_release) {
        return -1;
    }
    train_data.erase(trainID, info);
    delete_index.push_back(info.index);
    return 0;
}

int TrainManagement::release_train(const MyChar<24> &trainID) {
    vector<TrainInfo> res = train_data.query(trainID);
    if (res.empty()) {
        return -1;
    }
    TrainInfo info = res[0];
    Train train;
    train_index.read(train, info.index);
    if (train.is_release) {
        return -1;
    }
    for (int i = 1; i <= train.stationNum; ++i) {
        TrainStation temp(info, train.trainID, i, train.stations[i].price, train.stations[i].arriveTime,
                          train.stations[i].leaveTime);
        station_data.insert(train.stations[i].stationName, temp);
    }
    train.is_release = true;
    train_index.update(train, info.index);
    return 0;
}

void TrainManagement::query_train(const MyChar<24> &trainID, Date date) {
    vector<TrainInfo> res = train_data.query(trainID);
    Train train;
    train_index.read(train, res[0].index);
    if (res.empty()) {
        cout << -1 << '\n';
    }

    int now_time = Date::now_to_start(res[0].date.startDate, date);
    Seat seat;
    int cur_seat = train.seatNum;
    seat_index.read(seat, sizeof(Seat) * (100 * res[0].index + now_time));
    cout << train.trainID << ' ' << train.type << '\n';
    pair<Date, Clock> arr_time;
    pair<Date, Clock> lea_time;
    for (int i = 1; i <= train.stationNum; ++i) {
        if (i == 1) {
            cout << train.stations[i].stationName << ' ' << "xx-xx xx:xx" << ' ' << "->" << train.stations[i].leaveTime
                    << ' ' << 0 << ' ' << train.seatNum << '\n';
        } else if (i != train.stationNum) {
            arr_time = intToReadTime(date, train.startTime, train.stations[i].arriveTime);
            lea_time = intToReadTime(date, train.startTime, train.stations[i].leaveTime);
            cur_seat += seat.seat[i];
            cout << train.stations[i].stationName << ' ' << arr_time.first << ' ' << arr_time.second << " -> " <<
                    lea_time.first << ' ' << lea_time.second << train.stations[i].price << ' ' << cur_seat << '\n';
        } else {
            arr_time = intToReadTime(date, train.startTime, train.stations[i].arriveTime);
            cout << train.stations[i].stationName << ' ' << arr_time.first << arr_time.second << " -> " << "xx-xx xx:xx"
                    << train.stations[i].price << " x" << '\n';
        }
    }
}

void TrainManagement::query_ticket_cost(Date day, const MyChar<24> &s, const MyChar<24> &t) {
    vector<TrainStation> start_station = station_data.query(s);
    vector<TrainStation> end_station = station_data.query(t);

    priority_queue<CompareInfo, CompareCost> ans;
    if (start_station.empty() || end_station.empty()) {
        cout << 0 << '\n';
        return;
    }

    bool flag = false;
    for (int i = 0; i < start_station.size(); ++i) {
        vector<TrainInfo> res = train_data.query(start_station[i].trainID);
        TrainInfo info = res[0];
        Train train;
        train_index.read(train, info.index);
        if (train.salesDate.startDate <= day && train.salesDate.endDate >= day) {
            //d在当前站的运行时间内
            for (int j = 0; j < end_station.size(); ++j) {
                if (start_station[i].trainID == end_station[j].trainID && start_station[i].stationNum < end_station[j].
                    stationNum) {
                    // 这是同一辆车
                    flag = true;
                    int tim = end_station[j].arr_time - start_station[i].lea_time;
                    int pri = end_station[j].price - start_station[i].price;
                    CompareInfo cmp_info(pri, tim, train.trainID, start_station[i].stationNum,
                                         end_station[j].stationNum);
                    ans.push(cmp_info);
                }
            }
        }
    }
    if (!flag) {
        cout << 0 << '\n';
        return;
    }
    for (int i = 0; i < ans.size(); ++i) {
        CompareInfo cmp_ans = ans.top();
        ans.pop();
        vector<TrainInfo> temp = train_data.query(cmp_ans.trainID);
        Train train;
        train_index.read(train, temp[0].index);

        Seat seat;
        int sale_seat = train.seatNum;
        Date ini_day = Date::find_ini_day(day, train.startTime, train.stations[cmp_ans.start_].leaveTime);
        int d_day = Date::now_to_start(train.salesDate.startDate, ini_day);
        seat_index.read(seat, 100 * train.index + d_day);

        for (int j = 0; j < cmp_ans.start_; ++j) {
            sale_seat += seat.seat[j];
        }
        int min = 1e5 + 10;
        for (int k = cmp_ans.start_; k < cmp_ans.end_; ++k) {
            min = std::min(min, seat.seat[k]);
        }
        sale_seat += min;
        pair<Date, Clock> start_time = intToReadTime(day, train.startTime, train.stations[cmp_ans.start_].leaveTime);
        pair<Date, Clock> end_time = intToReadTime(day, train.startTime, train.stations[cmp_ans.end_].arriveTime);
        cout << train.trainID << ' ' << train.stations[cmp_ans.start_].stationName << ' ' << start_time.first << ' ' <<
                start_time.second << " -> " << train.stations[cmp_ans.end_].stationName << end_time.first << ' ' <<
                end_time.second << cmp_ans.price << ' ' << sale_seat << '\n';
    }
}

void TrainManagement::query_ticket_time(Date day, const MyChar<24> &s, const MyChar<24> &t) {
    vector<TrainStation> start_station = station_data.query(s);
    vector<TrainStation> end_station = station_data.query(t);

    priority_queue<CompareInfo, CompareTime> ans;
    if (start_station.empty() || end_station.empty()) {
        cout << 0 << '\n';
        return;
    }
    bool flag = false;
    for (int i = 0; i < start_station.size(); ++i) {
        vector<TrainInfo> res = train_data.query(start_station[i].trainID);
        TrainInfo info = res[0];
        Train train;
        train_index.read(train, info.index);
        if (train.salesDate.startDate <= day && train.salesDate.endDate >= day) {
            for (int j = 0; j < end_station.size(); ++j) {
                if (start_station[i].trainID == end_station[j].trainID && start_station[i].stationNum < end_station[j].
                    stationNum) {
                    flag = true;
                    int tim = end_station[j].arr_time - start_station[i].lea_time;
                    int pri = end_station[j].price - start_station[i].price;
                    CompareInfo cmp_info(pri, tim, train.trainID, start_station[i].lea_time, end_station[j].arr_time);
                    ans.push(cmp_info);
                }
            }
        }
    }
    if (!flag) {
        cout << 0 << '\n';
        return;
    }

    for (int i = 0; i < ans.size(); ++i) {
        CompareInfo cmp_ans = ans.top();
        ans.pop();
        vector<TrainInfo> temp = train_data.query(cmp_ans.trainID);
        Train train;
        train_index.read(train, temp[0].index);

        Seat seat;
        int sale_seat = train.seatNum;
        Date ini_day = Date::find_ini_day(day, train.startTime, train.stations[cmp_ans.start_].leaveTime);
        int d_day = Date::now_to_start(train.salesDate.startDate, ini_day);
        seat_index.read(seat, 100 * train.index + d_day);

        for (int j = 0; j < cmp_ans.start_; ++j) {
            sale_seat += seat.seat[j];
        }
        int min = 1e5;
        for (int k = cmp_ans.start_; k < cmp_ans.end_; ++k) {
            min = std::min(min, seat.seat[k]);
        }
        sale_seat += min;
        pair<Date, Clock> start_time = intToReadTime(day, train.startTime, train.stations[cmp_ans.start_].leaveTime);
        pair<Date, Clock> end_time = intToReadTime(day, train.startTime, train.stations[cmp_ans.end_].arriveTime);
        cout << train.trainID << ' ' << train.stations[cmp_ans.start_].stationName << ' ' << start_time.first << ' ' <<
                start_time.second << " -> " << train.stations[cmp_ans.end_].stationName << ' ' << end_time.first << ' '
                << end_time.second << ' ' << cmp_ans.price << sale_seat << '\n';
    }
}

void TrainManagement::query_transfer_cost(Date day, const MyChar<24> &s, const MyChar<24> &t) {
    vector<TrainStation> start_station = station_data.query(s);
    vector<TrainStation> end_station = station_data.query(t);

    CompareTrans final_ans;
    bool is_first = false;

    int train1_start, train1_mid, train2_mid, train2_end;
    if (start_station.empty() || end_station.empty()) {
        cout << 0 << '\n';
        return;
    }

    for (int i = 0; i < start_station.size(); ++i) {
        vector<TrainInfo> res1 = train_data.query(start_station[i].trainID);
        Train train1;
        train_index.read(train1, res1[0].index);
        if (train1.salesDate.startDate <= day && train1.salesDate.endDate >= day) {
            for (int j = 0; j < end_station.size(); ++j) {
                if (start_station[i].trainID == end_station[j].trainID) {
                    continue;
                }
                Train train2;
                vector<TrainInfo> res2 = train_data.query(end_station[j].trainID);
                train_index.read(train2, res2[0].index);
                for (int m = start_station[i].stationNum; m <= train1.stationNum; ++m) {
                    for (int n = 0; n <= end_station[j].stationNum; ++n) {
                        if (train1.stations[m].stationName == train2.stations[n].stationName) {
                            // 有相同站可以用于中转
                            pair<Date, Clock> lea_start, arr_mid, lea_mid, arr_end;

                            lea_start = {day, train1.startTime};

                            arr_mid = intToReadTime(day, train1.startTime, train1.stations[m].arriveTime);

                            pair<Date, Clock> lea_tmp = intToReadTime(day, train2.startTime,
                                                                      train2.stations[n].leaveTime);
                            int tim;
                            int pri;
                            if (train1.stations[m].arriveTime < train2.stations[n].leaveTime) {
                                //火车1到达中转站的时间在火车2离开中转站的时间之前
                                lea_mid = {arr_mid.first, lea_tmp.second};
                                tim = train1.stations[m].arriveTime + (lea_tmp.second - arr_mid.second) + train2.
                                      stations[j].arriveTime - train2.stations[n].leaveTime;
                            } else {
                                Date temp = arr_mid.first;
                                temp++;
                                lea_mid = {temp, lea_tmp.second};
                                tim = train1.stations[m].arriveTime + (lea_tmp.second - arr_mid.second) + train2.
                                      stations[j].arriveTime - train2.stations[n].leaveTime;
                            }
                            pri = train1.stations[m].price - train1.stations[i].price + train2.stations[j].price -
                                  train2.stations[n].price;
                            arr_end = intToReadTime(lea_mid.first, lea_mid.second,
                                                    train2.stations[train2.stationNum].arriveTime - train2.stations[n].
                                                    leaveTime);
                            CompareInfo a(train1.stations[m].price - train1.stations[i].price,
                                          train1.stations[m].arriveTime - train1.stations[i].leaveTime, train1.trainID,
                                          i, m);
                            CompareInfo b(train2.stations[j].price - train2.stations[n].price,
                                          train2.stations[j].arriveTime - train2.stations[n].leaveTime, train2.trainID,
                                          j, n);

                            CompareTrans temp(a, b, lea_start, arr_mid, lea_mid, arr_end, tim, pri);
                            if (is_first) {
                                final_ans = temp;
                                is_first = false;
                                train1_start = i;
                                train1_mid = m;
                                train2_mid = n;
                                train2_end = j;
                            } else {
                                if (temp.compareCost(final_ans)) {
                                    final_ans = temp;
                                    is_first = false;
                                    train1_start = i;
                                    train1_mid = m;
                                    train2_mid = n;
                                    train2_end = j;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vector<TrainInfo> res1 = train_data.query(final_ans.a.trainID);
    vector<TrainInfo> res2 = train_data.query(final_ans.b.trainID);
    Train train_ans1, train_ans2;
    train_index.read(train_ans1, res1[0].index);
    train_index.read(train_ans2, res2[0].index);

    //train1
    Date ini_day1 = Date::find_ini_day(day, train_ans1.startTime, train_ans1.stations[train1_start].leaveTime);
    int d_day1 = Date::now_to_start(train_ans1.salesDate.startDate, ini_day1);

    Seat seat1;
    seat_index.read(seat1, 100 * train_ans1.index + d_day1);

    int sale_seat1 = train_ans1.seatNum;
    for (int j = 0; j < train1_start; ++j) {
        sale_seat1 += seat1.seat[j];
    }
    int min = 1e5;
    for (int k = train1_start; k < train1_mid; ++k) {
        min = std::min(min, seat1.seat[k]);
    }
    sale_seat1 += min;

    cout << train_ans1.trainID << ' ' << train_ans1.stations[train1_start].stationName << ' ' << intToReadTime(
                ini_day1, train_ans1.startTime,
                train_ans1.stations[train1_start].leaveTime).first << ' ' << intToReadTime(
                ini_day1, train_ans1.startTime, train_ans1.stations[train1_start].leaveTime).second <<
            " -> " << train_ans1.stations[train1_mid].stationName << ' ' << train_ans1.stations[train1_mid].stationName
            << ' '
            << intToReadTime(ini_day1, train_ans1.startTime, train_ans1.stations[train1_mid].arriveTime).first << ' ' <<
            intToReadTime(ini_day1, train_ans1.startTime, train_ans1.stations[train1_mid].leaveTime).second << ' ' <<
            train_ans1.stations[train1_mid].price - train_ans1.stations[train1_start].price << ' ' << sale_seat1 <<
            '\n';

    //train2
    Date ini_day2 = Date::find_ini_day(final_ans.trans_leave.first, train_ans2.startTime,
                                       train_ans2.stations[train2_mid].arriveTime);
    int d_day2 = Date::now_to_start(train_ans2.salesDate.startDate, ini_day2);

    Seat seat2;
    seat_index.read(seat2, 100 * train_ans2.index + d_day2);

    int sale_seat2 = train_ans2.seatNum;
    for (int i = 0; i < train2_mid; ++i) {
        sale_seat2 += seat2.seat[i];
    }

    for (int i = train2_mid; i < train2_end; ++i) {
        min = std::min(min, seat2.seat[i]);
    }
    sale_seat2 += min;

    cout << train_ans2.trainID << ' ' << train_ans2.stations[train2_mid].stationName << ' ' << intToReadTime(
                ini_day2, train_ans2.startTime, train_ans2.stations[train2_mid].leaveTime).first << ' ' <<
            intToReadTime(ini_day2, train_ans2.startTime, train_ans2.stations[train2_mid].leaveTime).second << " -> "
            << train_ans2.stations[train2_end].stationName << ' ' <<
            intToReadTime(ini_day2, train_ans2.startTime, train_ans2.stations[train2_end].arriveTime).first << ' ' <<
            intToReadTime(ini_day2, train_ans2.startTime, train_ans2.stations[train2_end].arriveTime).second << ' ' <<
            train_ans2.stations[train2_end].price - train_ans2.stations[train2_mid].price << ' ' << sale_seat2 << '\n';
}
