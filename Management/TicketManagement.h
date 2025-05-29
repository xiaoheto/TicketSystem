//
// Created by 43741 on 2025/5/18.
//

#ifndef TICKETMANAGEMENT_H
#define TICKETMANAGEMENT_H
#include "../BPT/BPlusTree.h"
#include "../BasicInfo/Ticket.h"
#include "TrainManagement.h"
#include "../BasicInfo/Train.h"
#include "../BasicInfo/User.h"
#include "UserManagement.h"
#include "../STLite/vector.h"

using sjtu::vector;

class TicketManagement {
    BPTree<MyChar<24>, int> ticket_data;
    MemoryRiver<Ticket> ticket_index;
    BPTree<TrainTicketInfo, Ticket> waiting_data;

public:
    TicketManagement();

    void buy_ticket(const MyChar<24> &username, const MyChar<24> &trainID, const Date &d,
                    const MyChar<24> &start_station, const MyChar<24> &end_station, int n, bool q, UserManagement user,
                    TrainManagement train);

    void query_order(const MyChar<24> &username, UserManagement user, TrainManagement train);

    int refund_ticket(const MyChar<24> &username, UserManagement user, TrainManagement train, int n = -1);
};
#endif //TICKETMANAGEMENT_H
