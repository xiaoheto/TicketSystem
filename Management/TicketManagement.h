//
// Created by 43741 on 2025/5/18.
//


//
// Created by 43741 on 2025/5/18.
//

#ifndef TICKETMANAGEMENT_H
#define TICKETMANAGEMENT_H
#include "../BPT/BPlusTree.h"
#include "../BasicInfo/Ticket.h"
#include "TrainManagement.h"
#include "../Management/UserManagement.h"
#include "../Management/TrainManagement.h"
#include "../STLite/vector.h"

using sjtu::vector;

class TicketManagement {
    BPTree<MyChar<24>, int> ticket_data; //username -> index
    MemoryRiver<Ticket> ticket_index;
    BPTree<TrainTicketInfo, Ticket> waiting_data;
    int total = 0;

public:
    TicketManagement();

    ~TicketManagement();

    void buy_ticket(const MyChar<24> &username, const MyChar<24> &trainID, const Date &d,
                    const MyChar<24> &start_station, const MyChar<24> &end_station, int n, bool q, UserManagement &user,
                    TrainManagement &train);

    void query_order(const MyChar<24> &username, UserManagement &user, TrainManagement &train);

    int refund_ticket(const MyChar<24> &username, UserManagement &user, TrainManagement &train, int n = -1);

    void clear_ticket_file();
};
#endif //TICKETMANAGEMENT_H


TicketManagement::TicketManagement(): ticket_data("ticket.date"), waiting_data("waiting.data") {
    ticket_index.initialise("ticket.index");
    ticket_index.get_info(total, 1);
}

TicketManagement::~TicketManagement() {
    ticket_index.write_info(total, 1);
}


void TicketManagement::buy_ticket(const MyChar<24> &username, const MyChar<24> &trainID, const Date &d,
                                  const MyChar<24> &start_station, const MyChar<24> &end_station, int n, bool q,
                                  UserManagement &user, TrainManagement &train) {
    auto it = user.LoginInStack.find(username);
    if (it == user.LoginInStack.end()) {
        cout << -1 << '\n';
        return;
    }

    vector<int> user_res = user.user_data.query(username);
    User cur_user;
    user.user_index.read(cur_user, user_res[0]);

    vector<TrainInfo> info_res = train.train_data.query(trainID);
    if (info_res.empty()) {
        cout << -1 << '\n';
        return;
    }

    Train cur_train;
    train.train_index.read(cur_train, info_res[0].index);
    if (!cur_train.is_release) {
        cout << -1 << '\n';
        return;
    }

    int start_index = -1, end_index = -1;
    for (int i = 1; i <= cur_train.stationNum; ++i) {
        if (cur_train.stations[i].stationName == start_station) {
            start_index = i;
            break;
        }
    }
    for (int i = 1; i <= cur_train.stationNum; ++i) {
        if (cur_train.stations[i].stationName == end_station) {
            end_index = i;
            break;
        }
    }

    Seat seat;
    Date ini_day = Date::find_ini_day(d, cur_train.startTime, cur_train.stations[start_index].leaveTime);
    int d_day = Date::now_to_start(cur_train.salesDate.startDate, ini_day);

    train.seat_index.read(seat, 100 * cur_train.index + d_day);

    int sale_seat = cur_train.seatNum;
    for (int i = 0; i < start_index; ++i) {
        sale_seat += seat.seat[i];
    }

    int min = 1e5;
    for (int i = start_index; i < end_index; ++i) {
        min = std::min(min, seat.seat[i]);
    }

    sale_seat += min;

    if (sale_seat >= n) {
        // 能够成功购票
        seat.seat[start_index] -= n;
        seat.seat[end_index] += n;
        int index = ticket_index.get_index();
        Ticket ticket(cur_user.username, cur_train.trainID, start_index, end_index, d, success,
                      cur_train.stations[end_index].arriveTime - cur_train.stations[start_index].leaveTime, n, index);
        ticket_data.insert(cur_user.username, index);
        ticket_index.update(ticket, index);
        int pri = n * (cur_train.stations[end_index].price - cur_train.stations[start_index].price);

        train.seat_index.update(seat, 100 * cur_train.index + d_day);
        train.train_index.update(cur_train, cur_train.index);
        cout << pri << '\n';
        return;
    }
    if (!q) {
        cout << -1 << '\n';
        return;
    } //不接受候补
    int index = ticket_index.get_index();
    Ticket ticket(cur_user.username, cur_train.trainID, start_index, end_index, d, pending,
                  cur_train.stations[end_index].arriveTime - cur_train.stations[start_index].leaveTime, n, index);
    ticket_data.insert(cur_user.username, index);
    TrainTicketInfo info(cur_train.trainID, ini_day);

    waiting_data.insert(info, ticket);
    cout << "queue" << '\n';
}

void TicketManagement::query_order(const MyChar<24> &username, UserManagement &user, TrainManagement &train) {
    auto it = user.LoginInStack.find(username);
    if (it == user.LoginInStack.end()) {
        cout << -1 << '\n';
        return;
    }

    vector<int> user_res = user.user_data.query(username);
    User cur_user;
    user.user_index.read(cur_user, user_res[0]);

    vector<int> ticket_res = ticket_data.query(username);
    cout << ticket_res.size() << '\n';
    for (int i = ticket_res.size() - 1; i >= 0; --i) {
        Ticket cur_ticket;
        ticket_index.read(cur_ticket, ticket_res[i]);
        cout << '[' << cur_ticket.state << ']' << ' ';
        vector<TrainInfo> train_res = train.train_data.query(cur_ticket.trainID);

        Train cur_train;
        train.train_index.read(cur_train, train_res[0].index);

        Station start_station = cur_train.stations[cur_ticket.start_];
        Station end_station = cur_train.stations[cur_ticket.end_];

        Date ini_day = Date::find_ini_day(cur_ticket.day, cur_train.startTime, start_station.arriveTime);
        pair<Date, Clock> start_time = intToReadTime(ini_day, cur_train.startTime, start_station.leaveTime);
        pair<Date, Clock> end_time = intToReadTime(ini_day, cur_train.startTime, end_station.arriveTime);
        cout << cur_train.trainID << ' ' << start_station.stationName << ' ' << start_time.first << ' ' << start_time.
                second << " -> " << end_station.stationName << ' ' << end_time.first << ' ' << end_time.second <<
                end_station.price - start_station.price << ' ' << cur_ticket.num << '\n';
    }
}

int TicketManagement::refund_ticket(const MyChar<24> &username, UserManagement &user, TrainManagement &train, int n) {
    auto it = user.LoginInStack.find(username);
    if (it == user.LoginInStack.end()) {
        return -1;
    }

    vector<int> order_res = ticket_data.query(username);
    Ticket cur_ticket;
    ticket_index.read(cur_ticket, order_res[order_res.size() - n]);
    vector<TrainInfo> train_res = train.train_data.query(cur_ticket.trainID);
    Train cur_train;
    int st_ = cur_ticket.start_;
    int en_ = cur_ticket.end_;
    train.train_index.read(cur_train, train_res[0].index);

    if (cur_ticket.state == pending) {
        waiting_data.erase({cur_ticket.trainID, cur_ticket.day}, cur_ticket);
    } else if (cur_ticket.state == success) {
        vector<Ticket> wait_line = waiting_data.query({cur_ticket.trainID, cur_ticket.day});
        Seat seat;
        train.seat_index.read(seat, 100 * cur_ticket.index + (cur_ticket.day - cur_train.salesDate.startDate));
        seat.seat[st_] += n;
        seat.seat[en_] -= n;
        for (int i = 0; i < wait_line.size(); ++i) {
            Ticket wait_ticket = wait_line[i];
            int wait_st = wait_ticket.start_;
            int wait_en = wait_ticket.end_;

            int cur_seat = TrainManagement::cal_seat(wait_st, wait_en, seat, cur_train.seatNum);
            if (cur_seat > wait_ticket.num) {
                seat.seat[wait_st] -= wait_ticket.num;
                seat.seat[wait_en] += wait_ticket.num;
                wait_ticket.state = success;
                waiting_data.erase({wait_ticket.trainID, wait_ticket.day}, wait_ticket);
                ticket_index.update(wait_ticket, wait_ticket.index);
            }
            train.seat_index.update(seat, 100 * cur_train.index + (cur_ticket.day - cur_train.salesDate.startDate));
        }

        cur_ticket.state = refunded;
        ticket_index.update(cur_ticket, cur_ticket.index);
        train.seat_index.update(seat, 100 * cur_train.index + (cur_ticket.day - cur_train.salesDate.startDate));
        train.train_index.update(cur_train, cur_train.index);
    }
    return 0;
}

void TicketManagement::clear_ticket_file() {
    std::ofstream ticket_data("ticket.data", std::ios::trunc | std::ios::binary);
    ticket_data.close();

    std::ofstream waiting_data("waiting.data", ios::trunc | ios::binary);
    waiting_data.close();

    std::ofstream ticket_index("ticket.index");
    int tmp1 = -1, tmp2 = 0;
    ticket_index.write(reinterpret_cast<char *>(&tmp1), sizeof(int));
    ticket_index.write(reinterpret_cast<char *>(&tmp2), sizeof(int));
    ticket_index.close();
}
