//
// Created by 43741 on 2025/5/17.
//

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
    BPTree<MyChar<24>, int,24> user_data;
    MemoryRiver<User> user_index;
    int size = 0;

    friend class TicketManagement;
public:
    map<MyChar<24>, int> LoginInStack;

    bool flag = false; // 是否已经添加过用户

    UserManagement();

    ~UserManagement();

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


UserManagement::UserManagement(): user_data("user.data") {
    user_index.initialise("user.index");
    user_index.get_info(size, 1);
}

UserManagement::~UserManagement() {
    user_index.write_info(size, 1);
}

int UserManagement::add_user(const MyChar<24> &cur_username, const MyChar<24> &username, const MyChar<32> &password,
                             const MyChar<24> &name, const MyChar<32> &mailAddr, int privilege) {
    if (size == 0) {
        User user(username, password, name, mailAddr, 10);
        int offset = user_index.write(user);
        user_data.insert(username, offset);
        ++size;
        return 0;
    }
    User user(username, password, name, mailAddr, privilege);
    auto cur_user = LoginInStack.find(cur_username);
    if (cur_user == LoginInStack.end()) {
        return -1;
    } // -c未登录
    if (cur_user->second <= privilege) {
        return -1;
    } // -c的权限小于等于新用户的权限
    auto it = user_data.query(username);
    if (!it.empty()) {
        return -1;
    } // username已经存在
    int offset = user_index.write(user);
    user_data.insert(username, offset);
    ++size;
    return 0;
}

int UserManagement::login(const MyChar<24> &username, const MyChar<32> &password) {
    auto it = LoginInStack.find(username);
    if (it != LoginInStack.end()) {
        return -1;
    } // 用户已经登录
    auto index = user_data.query(username);
    if (index.empty()) {
        return -1;
    } //用户不存在
    User user;
    user_index.read(user, index[0]);
    if (user.password != password) {
        return -1;
    } //密码错误
    LoginInStack.insert({username, user.privilege});
    return 0;
}

int UserManagement::logout(const MyChar<24> &username) {
    auto it = LoginInStack.find(username);
    if (it == LoginInStack.end()) {
        return -1;
    } //用户没有登录
    LoginInStack.erase(it);
    return 0;
}

std::pair<User, bool> UserManagement::query_profile(const MyChar<24> &cur_username, const MyChar<24> &username) {
    auto it = LoginInStack.find(cur_username);
    User user;
    if (it == LoginInStack.end()) {
        return std::make_pair(user, false);
    }
    auto index = user_data.query(username);
    if (index.empty()) {
        return std::make_pair(user, false);
    }
    user_index.read(user, index[0]);
    if (it->second < user.privilege || (it->second == user.privilege && cur_username != username)) {
        return std::make_pair(user, false);
    }
    return std::make_pair(user, true);
}

std::pair<User, bool> UserManagement::modify_profile(const MyChar<24> &cur_username, const MyChar<24> &username,
                                                     const MyChar<32> &password, const MyChar<24> &name,
                                                     const MyChar<32> &mailAddr, int privilege) {
    auto it = LoginInStack.find(cur_username);
    User user;
    if (it == LoginInStack.end()) {
        return std::make_pair(user, false);
    }
    auto index = user_data.query(username);
    if (index.empty()) {
        return std::make_pair(user, false);
    }
    user_index.read(user, index[0]);
    if (it->second < user.privilege || (it->second == user.privilege && cur_username != username) || privilege > it->
        second) {
        return std::make_pair(user, false);
    } // -g大于-c的privilege或者...
    if (privilege != -1) {
        user.privilege = privilege;
    }
    if (!password.empty()) {
        user.password = password;
    }
    if (!name.empty()) {
        user.name = name;
    }
    if (!mailAddr.empty()) {
        user.mailAddr = mailAddr;
    }
    user_index.update(user, index[0]);
    return std::make_pair(user, true);
}

void UserManagement::clean_user_file() {
    std::ofstream ofs_data("user.data", std::ios::trunc | std::ios::binary);
    ofs_data.close();
    std::ofstream ofs_index("user.index", std::ios::trunc | std::ios::binary);
    int tmp = -1, tmp2 = 0;
    ofs_index.write(reinterpret_cast<char *>(&tmp), sizeof(int)); // root = -1
    ofs_index.write(reinterpret_cast<char *>(&tmp2), sizeof(int)); // size = 0
    ofs_index.close();

    size = 0;
    flag = false;
    LoginInStack.clear();

    user_index.initialise("user.index");
    user_index.get_info(size, 1);
}
