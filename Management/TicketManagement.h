#ifndef TICKETMANAGEMENT_H
#define TICKETMANAGEMENT_H

#include <iostream>
#include <filesystem>

#include "../BPT/BPlusTree.h"
#include "../BPT/MemoryRiver.h"
#include "../Tool/MyChar.h"
#include "../Tool/Time.h"
#include "../Management/UserManagement.h"
#include "../Management/TrainManagement.h"
#include "../BasicInfo/Ticket.h"

class TicketManagement {
    BPTree<MyChar<24>, int, 60> ticket_data;
    MemoryRiver<Ticket, 0> ticket_index;
    BPTree<TrainTicketInfo, Ticket, 16> waiting_data;

public:
    TicketManagement(): ticket_data("ticket.data"), waiting_data("waiting.data") {
        ticket_index.initialise("ticket.index");
    }

    ~TicketManagement();

    void buy_ticket(int time, const MyChar<24> &username, const MyChar<24> &trainID, const Date &d,
                    const MyChar<24> &start_station, const MyChar<24> &end_station, int n, bool q, UserManagement &user,
                    TrainManagement &train) {
        vector<int> user_res = user.user_data.query(username);
        if (user_res.empty()) {
            cout << -1 << '\n';
            return;
        }

        auto it = user.LogInStack.find(username);
        if (it == user.LogInStack.end()) {
            cout << -1 << '\n';
            return;
        }

        vector<TrainInfo> train_res = train.train_data.query(trainID);
        if (train_res.empty()) {
            cout << -1 << '\n';
            return;
        }

        Train cur_train;
        train.train_index.read(cur_train, train_res[0].index);

        if (!cur_train.is_release) {
            cout << -1 << '\n';
            return;
        }

        int st_ = 0, en_ = 0;
        for (int i = 1; i <= cur_train.stationNum; ++i) {
            if (cur_train.stations[i].stationName == start_station) {
                st_ = i;
                break;
            }
        }
        for (int i = 0; i <= cur_train.stationNum; ++i) {
            if (cur_train.stations[i].stationName == end_station) {
                en_ = i;
                break;
            }
        }

        Station start_ = cur_train.stations[st_], end_ = cur_train.stations[en_];
        Seat seat;
        Date ini_day = Date::find_ini_day(d, cur_train.startTime, start_.leaveTime);
        int d_day = Date::get_d_day(cur_train.salesDate.startDate, ini_day);

        train.seat_index.read(seat, TrainManagement::get_seat_index(cur_train.index, d_day));
        int cur_ticket = TrainManagement::cal_cur_ticket(st_, en_, seat, cur_train.seatNum);

        int price = n * (end_.price - start_.price);
        int offset = ticket_index.get_index();
        if (cur_ticket >= n) {
            seat.seat[st_] -= n;
            seat.seat[en_] += n;

            Ticket ticket(username, trainID, st_, en_, ini_day, success, time, n, offset);
            ticket_data.insert(username, offset);
            ticket_index.write(ticket);

            train.seat_index.update(seat, TrainManagement::get_seat_index(cur_train.index, d_day));

            cout << price << '\n';
            return;
        }
        if (!q) {
            cout << -1 << '\n';
            return;
        }
        Ticket ticket(username, trainID, st_, en_, ini_day, pending, time, n, offset);
        waiting_data.insert({trainID, ini_day}, ticket);

        ticket_data.insert(username, offset);
        ticket_index.write(ticket);
        cout << "queue\n";
    }

    void query_order(const MyChar<24> &username, UserManagement &user, TrainManagement &train) {
        auto it = user.LogInStack.find(username);
        if (it == user.LogInStack.end()) {
            cout << -1 << '\n';
        }

        vector<int> ticket_res = ticket_data.query(username);

        for (int i = 0; i < ticket_res.size(); ++i) {
            Ticket cur_ticket;
            ticket_index.read(cur_ticket, ticket_res[i]);

            vector<TrainInfo> train_res = train.train_data.query(cur_ticket.trainID);

            Train cur_train;
            train.train_index.read(cur_train, train_res[0].index);

            Date ini_day = cur_ticket.day;
            int d_day = Date::get_d_day(cur_train.salesDate.startDate, ini_day);

            Station start_station = cur_train.stations[cur_ticket.start_];
            Station end_station = cur_train.stations[cur_ticket.end_];
            pair<Date, Clock> start_time = TrainManagement::int_to_read_time(
                ini_day, cur_train.startTime, start_station.leaveTime);
            pair<Date, Clock> end_time = TrainManagement::int_to_read_time(
                ini_day, cur_train.startTime, end_station.arriveTime);

            int price = end_station.price - start_station.price;
            cout << '[' << cur_ticket.state << ']' << ' ' << cur_train.trainID << ' ' << start_station.stationName <<
                    ' ' << start_time.first << ' ' << start_time.second << ' ' << "->" << ' ' << end_station.stationName
                    << ' ' << end_time.first << ' ' << end_time.second << ' ' << price << cur_ticket.num << '\n';
        }
    }

    int refund_ticket(const MyChar<24> &username, int n, UserManagement &user, TrainManagement &train) {
        auto it = user.LogInStack.find(username);
        if (it == user.LogInStack.end()) {
            return -1;
        }

        vector<int> user_res = user.user_data.query(username);
        User cur_user;
        user.user_index.read(cur_user, user_res[0]);

        if (n == -1) {
            n = 1;
        }
        vector<int> ticket_res = ticket_data.query(username);
        Ticket cur_ticket;
        ticket_index.read(cur_ticket, ticket_res[ticket_res.size() - n]);

        vector<TrainInfo> train_res = train.train_data.query(cur_ticket.trainID);
        Train cur_train;
        train.train_index.read(cur_train, train_res[0].index);

        int d_day = Date::get_d_day(cur_ticket.day, cur_train.salesDate.startDate);
        Seat seat;
        train.seat_index.read(seat, TrainManagement::get_seat_index(cur_train.index, d_day));

        if (cur_ticket.start_ == refunded) {
            return -1;
        }

        if (cur_ticket.state == pending) {
            waiting_data.erase({cur_train.trainID, cur_ticket.day}, cur_ticket);
        }

        if (cur_ticket.start_ == success) {
            int st_ = cur_ticket.start_, en_ = cur_ticket.end_;

            seat.seat[st_] += cur_ticket.num;
            seat.seat[en_] -= cur_ticket.num;

            vector<Ticket> wait_line = waiting_data.query({cur_train.trainID, cur_ticket.day});

            for (int i = 0; i < wait_line.size(); ++i) {
                Ticket cur_process_ticket = wait_line[i];

                int cur_st = cur_process_ticket.start_;
                int cur_en = cur_process_ticket.end_;

                int cur_seat_num = TrainManagement::cal_cur_ticket(cur_st, cur_en, seat, cur_train.seatNum);

                if (cur_seat_num >= cur_process_ticket.num) {
                    seat.seat[cur_st] -= cur_process_ticket.num;
                    seat.seat[cur_en] += cur_process_ticket.num;

                    cur_process_ticket.state = success;
                    waiting_data.erase({cur_train.trainID, cur_ticket.day}, cur_process_ticket);

                    ticket_index.update(cur_process_ticket, cur_process_ticket.index);
                }
            }
        }
        cur_ticket.state = refunded;
        ticket_index.update(cur_ticket, cur_ticket.index);

        train.seat_index.update(seat, TrainManagement::get_seat_index(cur_train.index, d_day));

        return 0;
    }

    static void clean_ticket_file() {
        std::filesystem::path path1("ticket.data");
        std::filesystem::path path2("ticket.index");
        std::filesystem::path path3("waiting.data");
        if (exists(path1)) std::filesystem::remove(path1);
        if (exists(path2)) std::filesystem::remove(path2);
        if (exists(path3)) std::filesystem::remove(path3);
    }
};
#endif
