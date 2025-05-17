//
// Created by 43741 on 2025/5/17.
//

#ifndef TRAIN_H
#define TRAIN_H
#include "../Tool/MyChar.h"

class Station {
    MyChar<24> stationName;
    int price = 0; //到这一站的票价
    int arriveTime;
    int leaveTime;

public:
    Station() = default;

    Station(const MyChar<24> &stationName,int price,int arr,int lea):
    stationName(stationName),price(price),arriveTime(arr),leaveTime(lea) {
    }
};
#endif //TRAIN_H
