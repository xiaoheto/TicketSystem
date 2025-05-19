//
// Created by 43741 on 2025/5/18.
//

#ifndef TICKET_H
#define TICKET_H
#include <iostream>
#include <string>
#include "../Tool/MyChar.h"
#include "../Tool/Time.h"

class Ticket {
    MyChar<24> trainID;
    Date day;//从始发站出发的日期
    Clock initial_time;//从始发站出发的时间
    int start_station;
    int leave_station;
    int time;
    int seat;

public:
    Ticket() = default;

    Ticket(const MyChar<24> &id,const Date &day,const Clock &clock,int st,int en,int time,int seat):
    trainID(id),day(day),initial_time(clock),start_station(st),leave_station(en),time(time),seat(seat) {
    }

    Ticket(const Ticket &other) = default;
};
#endif //TICKET_H
