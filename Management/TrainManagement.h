//
// Created by 43741 on 2025/5/18.
//

#ifndef TRAINMANAGEMENT_H
#define TRAINMANAGEMENT_H
#include <iostream>
using std::cout;
#include "../BasicInfo/Train.h"
#include "../BPT/BPlusTree.h"
#include "../BPT/MemoryRiver.h"
#include "../STLite/vector.h"
#include "../Tool/Time.h"
#include "../STLite/utility.h"
#include "../STLite/priority_queue.h"

using sjtu::pair;
using sjtu::vector;
using sjtu::priority_queue;

class TicketManagement;
class TrainManagement {
    BPTree<MyChar<24>, TrainInfo> train_data; //trainID -> index
    MemoryRiver<Train> train_index;
    MemoryRiver<Seat, 0> seat_index;
    BPTree<MyChar<24>, TrainStation> station_data; // stationName -> trainStation
    int total = 0;
    vector<int> delete_index;

    friend class TicketManagement;
public:
    TrainManagement();

    int add_train(const MyChar<24> &trainID, int stationNum, int seatNum, vector<string> stationNames,
                  const vector<int> &prices, Clock startTime, vector<int> travelTimes, vector<int> stopoverTimes,
                  const SaleDate &saleDate, const MyChar<2> &type);

    int delete_train(const MyChar<24> &trainID);

    int release_train(const MyChar<24> &trainID);

    void query_train(const MyChar<24> &trainID, Date date);

    void query_ticket_time(Date day, const MyChar<24> &s, const MyChar<24> &t);

    void query_ticket_cost(Date day, const MyChar<24> &s, const MyChar<24> &t);

    void query_transfer_time(Date day, const MyChar<24> &s, const MyChar<24> &t);

    void query_transfer_cost(Date day, const MyChar<24> &s, const MyChar<24> &t);
};
#endif //TRAINMANAGEMENT_H
