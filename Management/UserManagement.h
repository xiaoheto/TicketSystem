//
// Created by 43741 on 2025/5/17.
//

#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H
#include <filesystem>
#include "../BPT/MemoryRiver.h"
#include "../BPT/BPlusTree.h"
#include "../BasicInfo/User.h"
#include "../STLite/map.h"
using sjtu::map;

class TicketManagement;
class UserManagement {
    BPTree<MyChar<24>, int> user_data;
    MemoryRiver<User> user_index;
    int size = 0;

    friend class TicketManagement;
public:
    map<MyChar<24>, int> LoginInStack;

    bool flag = false; // 是否已经添加过用户

    UserManagement();

    ~UserManagement();

    int add_first_user(const MyChar<24> &username, const MyChar<32> &password, const MyChar<24> &name,
                       const MyChar<32> &mailAddr);

    int add_user(const MyChar<24> &cur_username, const MyChar<24> &username, const MyChar<32> &password,
                 const MyChar<24> &name, const MyChar<32> &mailAddr, int privilege);

    int login(const MyChar<24> &username, const MyChar<32> &password);

    int logout(const MyChar<24> &username);

    std::pair<User, bool> query_profile(const MyChar<24> &cur_username, const MyChar<24> &username);

    std::pair<User, bool> modify_profile(const MyChar<24> &cur_username, const MyChar<24> &username,
                                         const MyChar<32> &password,
                                         const MyChar<24> &name, const MyChar<32> &mailAddr, int privilege);

    void clean_user_file();
};
#endif //USERMANAGEMENT_H
