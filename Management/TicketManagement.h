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
    int total;

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
