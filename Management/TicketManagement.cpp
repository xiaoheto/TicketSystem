//
// Created by 43741 on 2025/5/18.
//

#include "TicketManagement.h"

TicketManagement::TicketManagement(): ticket_data("order_date"), waiting_data("waiting_data") {
}

void TicketManagement::buy_ticket(const MyChar<24> &username, const MyChar<24> &trainID, const Date &d,
                                  const MyChar<24> &start_station, const MyChar<24> &end_station, int n, bool q,
                                  UserManagement user, TrainManagement train) {
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
        seat.seat[start_index] -= n;
        seat.seat[end_index] += n;
        Ticket ticket(cur_user.username, cur_train.trainID, start_index, end_index, d, success,
                      cur_train.stations[end_index].arriveTime - cur_train.stations[start_index].leaveTime,n);
        int index = ticket_index.write(ticket);
        ticket_data.insert(cur_user.username, index);
        int pri = n * (cur_train.stations[end_index].price - cur_train.stations[start_index].price);

        train.seat_index.update(seat, 100 * cur_train.index + d_day);
        train.train_index.update(cur_train, cur_train.index);
        cout << pri << '\n';
        return;
    }
    if (!q) {
        cout << -1 << '\n';
        return;
    }
    Ticket ticket(cur_user.username, cur_train.trainID, start_index, end_index, d, pending,
                  cur_train.stations[end_index].arriveTime - cur_train.stations[start_index].price,n);
    int index = ticket_index.write(ticket);
    ticket_data.insert(cur_user.username, index);
    TrainTicketInfo info(cur_train.trainID, ini_day);

    waiting_data.insert(info, ticket);
    cout << "queue" << '\n';
}

void TicketManagement::query_order(const MyChar<24> &username, UserManagement user, TrainManagement train) {
}

int TicketManagement::refund_ticket(const MyChar<24> &username, UserManagement user,TrainManagement train, int n) {
    auto it = user.LoginInStack.find(username);
    if (it == user.LoginInStack.end()) {
        return -1;
    }

    User cur_user;
    vector<int>user_res = user.user_data.query(username);

    user.user_index.read(cur_user,user_res[0]);
    vector<int> order = ticket_data.query(username);

    if (n > order.size()) {
        return -1;
    }

    Ticket ticket = ticket_index.read(order[n - 1]);
    vector<TrainInfo> train_res = train.train_data.query(ticket.trainID);
    if (train_res.empty()) {
        return -1;
    }

    Train cur_train;
    train.train_index.read(cur_train,train_res[0].index);

    Date ini_day = Date::find_ini_day(ticket.day,cur_train.startTime,cur_train.stations[ticket.start_].arriveTime);
    int d_day = Date::now_to_start(ini_day,cur_train.salesDate.startDate);

    Seat seat;
    train.seat_index.read(seat,100 * cur_train.index + d_day);

    if (ticket.state == pending) {
        waiting_data.erase({cur_train.trainID,ini_day},ticket);
    }

    if (ticket.state == success) {
        //TODO:把候补票转化为正式票，更新座位
        seat.seat[ticket.start_] += n;
        seat.seat[ticket.end_] -= n;

        vector<Ticket> waiting_ticket = waiting_data.query({cur_train.trainID,ini_day});
        for(int i = 0;i < waiting_ticket.size();++i) {
            Ticket cur_process_ticket = waiting_ticket[i];
            if (cur_process_ticket.day != ini_day) {
                continue;
            }

            int st_ = cur_process_ticket.start_;
            int en_ = cur_process_ticket.end_;

            vector<TrainInfo> cur_process_info = train.train_data.query(cur_process_ticket.trainID);
            Train cur_process_train;
            train.train_index.read(cur_process_train,cur_process_info[0].index);
            int cur_seat_ = cur_process_train.seatNum;
            for(int j = 0;j < st_;++j) {
                cur_seat_ += seat.seat[i];
            }

            int min = 1e5;
            for(int k = st_;k < en_;++k) {
                min = std::min(min,seat.seat[k]);
            }

            cur_seat_ += min;

            if (cur_seat_ >= cur_process_ticket.num) { // 能够成功购票
                seat.seat[st_] -= cur_process_ticket.num;
                seat.seat[en_] += cur_process_ticket.num;
                train.seat_index.update(seat,100 * cur_process_train.index + d_day);

                waiting_data.erase({cur_process_train.trainID,ini_day},cur_process_ticket);

                cur_process_ticket.state = success;
                //TODO:更新order的内容
            }
        }
    }
    ticket.state = refunded;
}
