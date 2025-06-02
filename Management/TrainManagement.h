//
// Created by 43741 on 2025/5/18.
//

#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <iostream>
#include "../BPT/BPlusTree.h"
#include "../Management/TrainManagement.h"
#include "../Management/UserManagement.h"
#include "../Management/TicketManagement.h"
#include "../Tool/CommandParser.h"
#include "../Tool/MyChar.h"
#include "../Tool/Time.h"

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::ios;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::getline;
using std::pair;

vector<string> getStr(const string &input) {
    vector<string> res;
    Command temp(input, '|');
    temp.position = 0;
    while (temp.count--) {
        res.push_back(temp.getNext());
    }
    return res;
}

vector<int> getInt(const string &input) {
    vector<int> res;
    res.push_back(0);
    Command temp(input, '|');
    temp.position = 0;
    while (temp.count--) {
        res.push_back(temp.str_to_int(temp.getNext()));
    }
    return res;
}

Clock getStartTime(const string &input) {
    Clock clock(input);
    return clock;
}

SaleDate getSaleDate(const string &input) {
    Command temp(input, '|');
    temp.position = 0;
    string ans1 = temp.getNext();
    string ans2 = temp.getNext();
    SaleDate ans(ans1, ans2);
    return ans;
}

// int main() {
//     string input;
//     cin >> input;
//     Clock res = getStartTime(input);
//     cout << res << '\n';
//     return 0;
// }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    UserManagement user_management;
    TrainManagement train_management;
    TicketManagement ticket_management;
    string input;
    while (getline(cin, input)) {
        Command command(input);
        if (command.cmd == "add_user") {
            MyChar<24> cur_username;
            MyChar<24> username;
            MyChar<32> password;
            MyChar<24> name;
            MyChar<32> mailAddr;
            int privilege = 10;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-c") {
                    cur_username = command.getNext();
                } else if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-p") {
                    password = command.getNext();
                } else if (key == "-n") {
                    name = command.getNext();
                } else if (key == "-m") {
                    mailAddr = command.getNext();
                } else if (key == "-g") {
                    privilege = command.str_to_int(command.getNext());
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ' << user_management.add_user(
                cur_username, username, password, name, mailAddr, privilege) << '\n';
        } else if (command.cmd == "login") {
            MyChar<24> username;
            MyChar<32> password;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-p") {
                    password = command.getNext();
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ' << user_management.login(username, password) << '\n';
        } else if (command.cmd == "logout") {
            string key = command.getNext();
            MyChar<24> username;
            username = command.getNext();
            cout << command.timeStamp << ' ' << user_management.logout(username) << '\n';
        } else if (command.cmd == "query_profile") {
            MyChar<24> cur_username;
            MyChar<24> username;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-c") {
                    cur_username = command.getNext();
                } else if (key == "-u") {
                    username = command.getNext();
                }
                command.current += 2;
            }
            if (!user_management.query_profile(cur_username, username).second) {
                cout << command.timeStamp << ' ' << -1 << '\n';
            } else {
                cout << command.timeStamp << ' ' << user_management.query_profile(cur_username, username).first << '\n';
            }
        } else if (command.cmd == "modify_profile") {
            MyChar<24> cur_username;
            MyChar<24> username;
            MyChar<32> password;
            MyChar<24> name;
            MyChar<32> mailAddr;
            int privilege = -1;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-c") {
                    cur_username = command.getNext();
                } else if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-p") {
                    password = command.getNext();
                } else if (key == "-n") {
                    name = command.getNext();
                } else if (key == "-m") {
                    mailAddr = command.getNext();
                } else if (key == "-g") {
                    privilege = command.str_to_int(command.getNext());
                }
                command.current += 2;
            }
            if (!user_management.modify_profile(cur_username, username, password, name, mailAddr, privilege).second) {
                cout << command.timeStamp << ' ' << -1 << '\n';
            } else {
                cout << command.timeStamp << ' ' << user_management.modify_profile(
                            cur_username, username, password, name, mailAddr, privilege).first
                        << '\n';
            }
        } else if (command.cmd == "add_train") {
            MyChar<24> trainID;
            int stationNum;
            int seatNum;
            vector<string> stations;
            vector<int> prices;
            Clock startTime;
            vector<int> travelTimes;
            vector<int> stopoverTimes;
            SaleDate saleDate;
            MyChar<2> type;

            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-i") {
                    trainID = command.getNext();
                } else if (key == "-n") {
                    stationNum = command.str_to_int(command.getNext());
                } else if (key == "-m") {
                    seatNum = command.str_to_int(command.getNext());
                } else if (key == "-s") {
                    string s = command.getNext();
                    stations = getStr(s);
                } else if (key == "-p") {
                    string p = command.getNext();
                    prices = getInt(p);
                } else if (key == "-x") {
                    string x = command.getNext();
                    startTime = getStartTime(x);
                } else if (key == "-t") {
                    string t = command.getNext();
                    travelTimes = getInt(t);
                } else if (key == "-o") {
                    string o = command.getNext();
                    if (o == "_") {
                        stopoverTimes.clear();
                    } else {
                        stopoverTimes = getInt(o);
                    }
                } else if (key == "-d") {
                    string d = command.getNext();
                    saleDate = getSaleDate(d);
                } else if (key == "-y") {
                    type = command.getNext();
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ' << train_management.add_train(
                trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, stopoverTimes, saleDate,
                type) << '\n';
        } else if (command.cmd == "delete_train") {
            MyChar<24> trainID;
            string i = command.getNext();
            trainID = command.getNext();
            cout << command.timeStamp << ' ' << train_management.delete_train(trainID) << '\n';
        } else if (command.cmd == "release_train") {
            MyChar<24> trainID;
            string i = command.getNext();
            trainID = command.getNext();
            cout << command.timeStamp << ' ' << train_management.release_train(trainID) << '\n';
        } else if (command.cmd == "query_train") {
            MyChar<24> trainID;
            Date d = Date();
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-i") {
                    trainID = command.getNext();
                } else if (key == "-d") {
                    string temp = command.getNext();
                    Date day(temp);
                    d = day;
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ';
            train_management.query_train(trainID, d);
        } else if (command.cmd == "query_ticket") {
            Date d = Date();
            MyChar<24> s;
            MyChar<24> t;
            string p;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-s") {
                    s = command.getNext();
                } else if (key == "-t") {
                    t = command.getNext();
                } else if (key == "-d") {
                    string temp = command.getNext();
                    Date tt(temp);
                    d = tt;
                } else if (key == "-p") {
                    p = command.getNext();
                }
                command.current += 2;
            }
            if (p == "time") {
                cout << command.timeStamp << ' ';
                train_management.query_ticket_time(d, s, t);
            } else if (p == "cost") {
                cout << command.timeStamp << ' ';
                train_management.query_ticket_cost(d, s, t);
            }
        } else if (command.cmd == "query_transfer") {
            Date d = Date();
            MyChar<24> s;
            MyChar<24> t;
            string p;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-s") {
                    s = command.getNext();
                } else if (key == "-t") {
                    t = command.getNext();
                } else if (key == "-d") {
                    string temp = command.getNext();
                    Date tt(temp);
                    d = tt;
                } else if (key == "-p") {
                    p = command.getNext();
                }
                command.current += 2;
            }
            if (p == "time") {
                train_management.query_transfer_time(d, s, t);
            } else if (p == "cost") {
                train_management.query_transfer_cost(d, s, t);
            }
        } else if (command.cmd == "buy_ticket") {
            MyChar<24> username;
            MyChar<24> trainID;
            Date d = Date();
            MyChar<24> st;
            MyChar<24> en;
            int n;
            string q;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-i") {
                    trainID = command.getNext();
                } else if (key == "-d") {
                    string temp = command.getNext();
                    Date dd(temp);
                    d = dd;
                } else if (key == "-f") {
                    st = command.getNext();
                } else if (key == "-t") {
                    en = command.getNext();
                } else if (key == "-n") {
                    n = command.str_to_int(command.getNext());
                } else if (key == "-q") {
                    q = command.getNext();
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ';
            if (q == "true") {
                ticket_management.buy_ticket(username, trainID, d, st, en, n, true, user_management, train_management);
            } else if (q == "false") {
                ticket_management.buy_ticket(username, trainID, d, st, en, n, false, user_management, train_management);
            }
        } else if (command.cmd == "query_order") {
            string u = command.getNext();
            MyChar<24> username = command.getNext();
            ticket_management.query_order(username, user_management, train_management);
        } else if (command.cmd == "refund_ticket") {
            MyChar<24> username;
            int n;
            if (command.count == 3) {
                string key = command.getNext();
                username = command.getNext();
                n = 1;
            }
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-n") {
                    n = command.str_to_int(command.getNext());
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ';
            cout << ticket_management.refund_ticket(username, user_management, train_management, n) << '\n';
        } else if (command.cmd == "clean") {
            user_management.clean_user_file();
            train_management.clear_train_file();
            ticket_management.clear_ticket_file();
            user_management.flag = false;
            cout << 0 << '\n';
        } else if (command.cmd == "exit") {
            cout << "bye" << '\n';
            user_management.LoginInStack.clear();
            break;
        } else {
            throw Error("Invalid\n");
        }
    }
    return 0;
}


TrainManagement::TrainManagement(): train_data("train.data"), station_data("station.data") {
    train_index.initialise("train.index");
    seat_index.initialise("seat.index");

    train_index.get_info(total, 1);
}

TrainManagement::~TrainManagement() {
    train_index.write_info(total, 1);
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
    if (res.empty()) {
        cout << -1 << '\n';
        return;
    }

    train_index.read(train, res[0].index);

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

    if (start_station.empty() || end_station.empty()) {
        cout << 0 << '\n';
        return;
    }

    for (int i = 0; i < start_station.size(); ++i) {
        vector<TrainInfo> res1 = train_data.query(start_station[i].trainID);
        Train train1;
        train_index.read(train1, res1[0].index);
        pair<Date, Clock> early1 = intToReadTime(train1.salesDate.startDate, train1.startTime,
                                                 start_station[i].arr_time);
        pair<Date, Clock> late1 = intToReadTime(train1.salesDate.endDate, train1.startTime, start_station[i].arr_time);

        if (early1.first <= day && late1.first >= day) {
            for (int j = 0; j < end_station.size(); ++j) {
                if (start_station[i].trainID == end_station[j].trainID) {
                    continue;
                }
                Train train2;
                vector<TrainInfo> res2 = train_data.query(end_station[j].trainID);
                train_index.read(train2, res2[0].index);
                for (int m = start_station[i].stationNum + 1; m <= train1.stationNum; ++m) {
                    for (int n = 0; n < end_station[j].stationNum; ++n) {
                        if (train1.stations[m].stationName == train2.stations[n].stationName) {
                            pair<Date, Clock> lea_start, arr_mid, lea_mid, arr_end;

                            Date ini_day1 = Date::find_ini_day(day, train1.startTime, train1.stations[i].leaveTime);
                            lea_start = intToReadTime(ini_day1, train1.startTime, train1.stations[i].leaveTime);
                            arr_mid = intToReadTime(ini_day1, train1.startTime, train1.stations[m].arriveTime);

                            pair<Date, Clock> lea_tmp = intToReadTime(day, train2.startTime,
                                                                      train2.stations[n].leaveTime);
                            pair<Date, Clock> early = intToReadTime(train2.salesDate.startDate, train2.startTime,
                                                                    train2.stations[n].leaveTime);
                            pair<Date, Clock> late = intToReadTime(train2.salesDate.endDate, train2.startTime,
                                                                   train2.stations[n].leaveTime);

                            int tim;
                            int pri;

                            if (arr_mid.first > late.first || (
                                    arr_mid.first == late.first && arr_mid.second > late.second)) {
                                continue;
                            }
                            if (arr_mid.first < early.first) {
                                lea_mid = {early.first, lea_tmp.second};
                                arr_end = intToReadTime(early.first, lea_mid.second,
                                                        train2.stations[j].arriveTime - train2.stations[n].leaveTime);
                            } else {
                                if (arr_mid.second < lea_tmp.second) {
                                    lea_mid = {arr_mid.first, lea_tmp.second};
                                } else {
                                    Date temp = arr_mid.first;
                                    ++temp;
                                    lea_mid = {temp, lea_tmp.second};
                                }

                                arr_end = intToReadTime(lea_mid.first, lea_mid.second,
                                                        train2.stations[j].arriveTime - train2.stations[n].leaveTime);
                            }
                            pri = train1.stations[m].price - train1.stations[i].price + train2.stations[j].price -
                                  train2.stations[n].price;
                            tim = train1.stations[m].arriveTime - train1.stations[i].leaveTime + train2.stations[j].
                                  arriveTime - train2.stations[n].leaveTime;

                            CompareInfo a(train1.stations[m].price - train1.stations[i].price,
                                          train1.stations[m].arriveTime - train1.stations[i].leaveTime, train1.trainID,
                                          i, m);
                            CompareInfo b(train2.stations[j].price - train2.stations[n].price,
                                          train2.stations[j].arriveTime - train2.stations[n].leaveTime, train2.trainID,
                                          n, j);

                            CompareTrans temp_ans(a, b, lea_start, arr_mid, lea_mid, arr_end, tim, pri);

                            if (is_first) {
                                final_ans = temp_ans;
                            } else {
                                if (!final_ans.compareCost(temp_ans)) {
                                    final_ans = temp_ans;
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
    int d_day1 = Date::now_to_start(ini_day1, day);
    Date ini_day2 = Date::find_ini_day(final_ans.trans_leave.first, b_.startTime,
                                       b_.stations[final_ans.b.start_].leaveTime);
    int d_day2 = Date::now_to_start(ini_day2, final_ans.trans_leave.first);

    seat_index.read(seat1, 100 * a_.index + d_day1);
    seat_index.read(seat2, 100 * b_.index + d_day2);
    int seat_num1 = cal_seat(final_ans.a.start_, final_ans.a.end_, seat1, a_.seatNum);
    int seat_num2 = cal_seat(final_ans.b.start_, final_ans.b.end_, seat2, b_.seatNum);

    cout << a_.trainID << ' ' << a_.stations[final_ans.a.start_].stationName << ' ' << final_ans.start_leave.first <<
            ' ' << final_ans.start_leave.second << " -> " << a_.stations[final_ans.a.end_].stationName << ' ' <<
            final_ans.trans_arrive.first << ' ' << final_ans.trans_arrive.second << ' ' << final_ans.a.price << ' ' <<
            seat_num1 << '\n';
    cout << b_.trainID << ' ' << b_.stations[final_ans.b.start_].stationName << ' ' << final_ans.trans_leave.first <<
            ' ' << final_ans.trans_leave.second << " -> " << b_.stations[final_ans.b.start_].stationName << ' ' <<
            final_ans.end_arrive.first << ' ' << final_ans.end_arrive.second << ' ' << final_ans.b.price << ' ' <<
            seat_num2 << '\n';
}


void TrainManagement::query_transfer_time(Date day, const MyChar<24> &s, const MyChar<24> &t) {
    vector<TrainStation> start_station = station_data.query(s);
    vector<TrainStation> end_station = station_data.query(t);

    CompareTrans final_ans;
    bool is_first = false;

    if (start_station.empty() || end_station.empty()) {
        cout << 0 << '\n';
        return;
    }

    for (int i = 0; i < start_station.size(); ++i) {
        vector<TrainInfo> res1 = train_data.query(start_station[i].trainID);
        Train train1;
        train_index.read(train1, res1[0].index); //找到第一辆车
        pair<Date, Clock> early1 = intToReadTime(train1.salesDate.startDate, train1.startTime,
                                                 start_station[i].arr_time);
        pair<Date, Clock> late1 = intToReadTime(train1.salesDate.endDate, train1.startTime, start_station[i].arr_time);

        if (early1.first <= day && late1.first >= day) {
            //开始时间在这辆车的售票时间范围内
            for (int j = 0; j < end_station.size(); ++j) {
                if (start_station[i].trainID == end_station[j].trainID) {
                    continue; //同一辆车，跳过
                }
                Train train2;
                vector<TrainInfo> res2 = train_data.query(end_station[j].trainID);
                train_index.read(train2, res2[0].index);
                for (int m = start_station[i].stationNum + 1; m <= train1.stationNum; ++m) {
                    for (int n = 0; n < end_station[j].stationNum; ++n) {
                        if (train1.stations[m].stationName == train2.stations[n].stationName) {
                            pair<Date, Clock> lea_start, arr_mid, lea_mid, arr_end;

                            Date ini_day1 = Date::find_ini_day(day, train1.startTime, train1.stations[i].leaveTime);
                            lea_start = intToReadTime(ini_day1, train1.startTime, train1.stations[i].leaveTime);
                            arr_mid = intToReadTime(ini_day1, train1.startTime, train1.stations[m].arriveTime);

                            pair<Date, Clock> lea_tmp = intToReadTime(day, train2.startTime,
                                                                      train2.stations[n].leaveTime);

                            pair<Date, Clock> early = intToReadTime(train2.salesDate.startDate, train2.startTime,
                                                                    train2.stations[n].leaveTime);
                            pair<Date, Clock> late = intToReadTime(train2.salesDate.endDate, train2.startTime,
                                                                   train2.stations[n].leaveTime);

                            int tim;
                            int pri;

                            if (arr_mid.first > late.first || (
                                    arr_mid.first == late.first && arr_mid.second > late.second)) {
                                continue;
                            }
                            if (arr_mid.first < early.first) {
                                lea_mid = {early.first, lea_tmp.second};
                                arr_end = intToReadTime(early.first, lea_mid.second,
                                                        train2.stations[j].arriveTime - train2.stations[n].leaveTime);
                            } else {
                                if (arr_mid.second < lea_tmp.second) {
                                    //当天就可以换乘
                                    lea_mid = {arr_mid.first, lea_tmp.second};
                                } else {
                                    Date temp = arr_mid.first;
                                    ++temp;
                                    lea_mid = {temp, lea_tmp.second};
                                }

                                arr_end = intToReadTime(lea_mid.first, lea_mid.second,
                                                        train2.stations[j].arriveTime - train2.stations[n].leaveTime);
                            }
                            pri = train1.stations[m].price - train1.stations[i].price + train2.stations[j].price -
                                  train2.stations[n].price;
                            tim = train1.stations[m].arriveTime - train1.stations[i].leaveTime + train2.stations[j].
                                  arriveTime - train2.stations[n].leaveTime;

                            CompareInfo a(train1.stations[m].price - train1.stations[i].price,
                                          train1.stations[m].arriveTime - train1.stations[i].leaveTime, train1.trainID,
                                          i, m);
                            CompareInfo b(train2.stations[j].price - train2.stations[n].price,
                                          train2.stations[j].arriveTime - train2.stations[n].leaveTime, train2.trainID,
                                          n, j);

                            CompareTrans temp_ans(a, b, lea_start, arr_mid, lea_mid, arr_end, tim, pri);

                            if (is_first) {
                                final_ans = temp_ans;
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
    vector<TrainInfo> a_train = train_data.query(final_ans.a.trainID);
    vector<TrainInfo> b_train = train_data.query(final_ans.b.trainID);
    Train a_, b_;
    train_index.read(a_, a_train[0].index);
    train_index.read(b_, b_train[0].index);

    Seat seat1, seat2;
    Date ini_day1 = Date::find_ini_day(day, a_.startTime, a_.stations[final_ans.a.start_].leaveTime);
    int d_day1 = Date::now_to_start(ini_day1, day);

    Date ini_day2 = Date::find_ini_day(final_ans.trans_leave.first, b_.startTime,
                                       b_.stations[final_ans.b.start_].leaveTime);
    int d_day2 = Date::now_to_start(ini_day2, final_ans.trans_leave.first);
    seat_index.read(seat1, 100 * a_.index + d_day1);
    seat_index.read(seat2, 100 * b_.index + d_day2);
    int seat_num1 = cal_seat(final_ans.a.start_, final_ans.a.end_, seat1, a_.seatNum);
    int seat_num2 = cal_seat(final_ans.b.start_, final_ans.b.end_, seat2, b_.seatNum);

    cout << a_.trainID << ' ' << a_.stations[final_ans.a.start_].stationName << ' ' << final_ans.start_leave.first <<
            ' ' << final_ans.start_leave.second << " -> " << a_.stations[final_ans.a.end_].stationName << ' ' <<
            final_ans.trans_arrive.first << ' ' << final_ans.trans_arrive.second << ' ' << final_ans.a.price << ' ' <<
            seat_num1 << '\n';
    cout << b_.trainID << ' ' << b_.stations[final_ans.b.start_].stationName << ' ' << final_ans.trans_leave.first <<
            ' ' << final_ans.trans_leave.second << " -> " << b_.stations[final_ans.b.start_].stationName << ' ' <<
            final_ans.end_arrive.first << ' ' << final_ans.end_arrive.second << ' ' << final_ans.b.price << ' ' <<
            seat_num2 << '\n';
}

int TrainManagement::cal_seat(int start_, int end_, Seat seat, int max_seat) {
    for (int i = 1; i < start_; ++i) {
        max_seat += seat.seat[i];
    }
    int min = 1e5;
    for (int i = start_; i < end_; ++i) {
        min = std::min(min, seat.seat[i]);
    }

    max_seat += min;
    return max_seat;
}

void TrainManagement::clear_train_file() {
    std::ofstream train_data("train.data", std::ios::trunc | std::ios::binary);
    train_data.close();

    std::ofstream station_data("station.data", std::ios::trunc | std::ios::binary);
    station_data.close();

    std::ofstream train_index("train.index", std::ios::trunc | std::ios::binary);
    int tmp1 = -1, tmp2 = 0;
    train_index.write(reinterpret_cast<char *>(&tmp1), sizeof(int));
    train_index.write(reinterpret_cast<char *>(&tmp2), sizeof(int));
    train_index.close();

    std::ofstream seat_index("seat.index", std::ios::trunc | std::ios::binary);
    seat_index.close();

    delete_index.clear();
}
